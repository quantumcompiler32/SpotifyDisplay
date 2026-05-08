# SpotifyDisplay

<img width="1710" height="978" alt="Screenshot 2026-05-07 at 11 48 56 PM" src="https://github.com/user-attachments/assets/4af5d4cb-7b99-4688-bc6f-e85761cd7ab9" />

Overview

Uses an ESP32 microcontroller and a 1.8-inch TFT screen to show the current track from an artist from a user's Spotify. Uses 3D software with hardware assembly and C++ programming with Arduino IDE. Uses Wi-Fi to get  data through the Spotify web API and SPI. Modeled in Fusion 360.


What can it do 

- Spotify track display (song title + artist)
- Wi-Fi connectivity via ESP32
- Custom 3D-printed frame aka no screws or friction fit
- SPI communication with TFT display
- Rotary encoder for volume control and track skipping
- MX switches for play/pause and skip


Bill of Materials (BOM)

| Name | Purpose | Qty | Unit Cost (USD) | Total | Distributor | Link |
|------|---------|-----|-----------------|-------|-------------|------|
| HiLetgo 1.8" ST7735R SPI TFT LCD Display | Displays song and artist info | 1 | $9.69 | $9.69 | Amazon | [Link](https://www.amazon.com/dp/B00LSG51MM) |
| C3 Mini V2.1.0 ESP32 | Main microcontroller and brain | 1 | $5.66 | $5.66 | AliExpress | [Link](https://www.aliexpress.us/item/3256804553736450.html) |
| WWZMD1B 6Pcs EC11 Rotary Encoder with Push Button | Volume and track control | 1 | $8.88 | $8.88 | Amazon | [Link](https://www.amazon.com/dp/B0C6Q67V97) |
| MX Switches | Play/pause and skip controls | 1 | $0.00 | $0.00 | Myself | - |

**Total Estimated Cost: ~$24.23**

Full BOM CSV available in [`bom/SpotifyDisplayV3BOM.csv`](bom/SpotifyDisplayV3BOM.csv).

<img width="1301" height="442" alt="image" src="https://github.com/user-attachments/assets/22c064c8-bc6e-4b7f-aba9-c0736102d18d" />




