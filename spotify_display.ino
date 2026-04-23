#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <Arduino.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <SpotifyEsp32.h>
#include <WiFi.h>

#define TFT_CS 1
#define TFT_RST 2
#define TFT_DC 3
#define TFT_SCLK 4
#define TFT_MOSI 5

// EC11 Rotary Encoder pins
#define ENC_CLK 6  // CLK (A) pin
#define ENC_DT  7  // DT  (B) pin
#define ENC_SW  8  // Push-button pin (optional)

// MX Switch pins
#define BTN_PLAYPAUSE 9   // Play / Pause
#define BTN_NEXT      10  // Next track
#define BTN_PREV      11  // Previous track

char *SSID = "Akshardham2.4";
const char *PASSWORD = "guest1AKDM";
const char *CLIENT_ID = "95ee158cd174437eba9fc5333eb1e8cc";
const char *CLIENT_SECRET = "86d5f43a8fef4a8cb75944566479c0ae";

String lastArtist;
String lastTrackname;

// Rotary encoder state
volatile int  encoderDelta  = 0;  // accumulated steps (+/-)
volatile bool lastClkState  = HIGH;
unsigned long lastEncTime   = 0;  // debounce timestamp
int currentVolume           = 50; // start at 50% — updated from encoder

// MX switch debounce timestamps
unsigned long lastPlayPauseTime = 0;
unsigned long lastNextTime      = 0;
unsigned long lastPrevTime      = 0;
#define BTN_DEBOUNCE_MS 200  // ignore repeated presses within 200ms

// ISR: called on every CLK falling edge
void IRAM_ATTR onEncoderTick() {
  unsigned long now = millis();
  if (now - lastEncTime < 5) return; // debounce: ignore edges < 5 ms apart
  lastEncTime = now;

  bool clk = digitalRead(ENC_CLK);
  bool dt  = digitalRead(ENC_DT);

  if (clk != lastClkState) {
    // If DT differs from CLK the encoder turned clockwise (right = volume up)
    if (dt != clk) {
      encoderDelta++;
    } else {
      encoderDelta--;
    }
    lastClkState = clk;
  }
}

Spotify sp(CLIENT_ID, CLIENT_SECRET);
Adafruit_ST7735 tft =
    Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

void setup() {
  Serial.begin(115200);

  // Rotary encoder setup
  pinMode(ENC_CLK, INPUT_PULLUP);
  pinMode(ENC_DT,  INPUT_PULLUP);
  pinMode(ENC_SW,  INPUT_PULLUP);
  lastClkState = digitalRead(ENC_CLK);
  attachInterrupt(digitalPinToInterrupt(ENC_CLK), onEncoderTick, CHANGE);

  // MX switch setup
  pinMode(BTN_PLAYPAUSE, INPUT_PULLUP);
  pinMode(BTN_NEXT,      INPUT_PULLUP);
  pinMode(BTN_PREV,      INPUT_PULLUP);

  tft.initR(INITR_BLACKTAB); // the type of screen
  tft.setRotation(
      1); // this makes the screen landscape! remove this line for portrait
  Serial.println("TFT Initialized!");
  tft.fillScreen(ST77XX_BLACK); // make sure there is nothing in the buffer

  WiFi.begin(SSID, PASSWORD);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.printf("\nConnected!\n");

  tft.setCursor(0, 0); // make the cursor at the top left
  tft.write(WiFi.localIP().toString().c_str()); // print out IP on the screen

  sp.begin();
  while (!sp.is_auth()) {
    sp.handle_client();
  }
  Serial.println("Connected to Spotify!");
  sp.set_log_level(SPOTIFY_LOG_VERBOSE);
}

// Handle volume changes from the rotary encoder
void handleEncoder() {
  int delta = 0;
  noInterrupts();
  delta = encoderDelta;
  encoderDelta = 0;
  interrupts();

  if (delta == 0) return;

  // Each encoder detent = 1 volume step; scale as you like
  currentVolume += delta;
  currentVolume = constrain(currentVolume, 0, 100);

  Serial.printf("Volume: %d\n", currentVolume);
  sp.set_volume(currentVolume);

  // Brief on-screen feedback
  tft.fillRect(0, 110, 160, 18, ST77XX_BLACK);
  tft.setCursor(10, 112);
  tft.setTextColor(ST77XX_GREEN, ST77XX_BLACK);
  tft.printf("Vol: %d%%", currentVolume);
  tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
}

// Show a brief action label at the bottom of the screen
void showAction(const char* label) {
  tft.fillRect(0, 110, 160, 18, ST77XX_BLACK);
  tft.setCursor(10, 112);
  tft.setTextColor(ST77XX_CYAN, ST77XX_BLACK);
  tft.print(label);
  tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
}

// Handle MX switch button presses
void handleButtons() {
  unsigned long now = millis();

  // --- Play / Pause ---
  if (digitalRead(BTN_PLAYPAUSE) == LOW && now - lastPlayPauseTime > BTN_DEBOUNCE_MS) {
    lastPlayPauseTime = now;
    Serial.println("Button: Play/Pause");
    sp.toggle_play_pause();
    showAction("Play / Pause");
  }

  // --- Next Track ---
  if (digitalRead(BTN_NEXT) == LOW && now - lastNextTime > BTN_DEBOUNCE_MS) {
    lastNextTime = now;
    Serial.println("Button: Next Track");
    sp.next_track();
    showAction("Next Track");
  }

  // --- Previous Track ---
  if (digitalRead(BTN_PREV) == LOW && now - lastPrevTime > BTN_DEBOUNCE_MS) {
    lastPrevTime = now;
    Serial.println("Button: Previous Track");
    sp.previous_track();
    showAction("Prev Track");
  }
}

void loop() {
  sp.handle_client();
  handleEncoder(); // check encoder every loop iteration
  handleButtons(); // check MX switches every loop iteration
  String currentArtist = sp.current_artist_names();
  String currentTrackname = sp.current_track_name();

  if (lastArtist != currentArtist && currentArtist != "Something went wrong" &&
      !currentArtist.isEmpty()) {
    tft.fillScreen(ST77XX_BLACK);
    lastArtist = currentArtist;
    Serial.println("Artist: " + lastArtist);
    tft.setCursor(10, 10);
    tft.write(lastArtist.c_str());
  }

  if (lastTrackname != currentTrackname &&
      currentTrackname != "Something went wrong" &&
      currentTrackname != "null") {
    lastTrackname = currentTrackname;
    Serial.println("Track: " + lastTrackname);
    tft.setCursor(10, 40);
    tft.write(lastTrackname.c_str());
  }
  delay(2000);
}
