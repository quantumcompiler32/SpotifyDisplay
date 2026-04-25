# SpotifyDisplay

<img width="1691" height="934" alt="Screenshot 2026-04-16 at 11 30 39 PM" src="https://github.com/user-attachments/assets/6edf5d63-de09-41ea-a1d9-3ae5f65073b1" />

A hardware project that uses an **ESP32 microcontroller** and a **1.8-inch TFT screen** to display the current track and artist from a user's Spotify account in real time. Features a **rotary encoder** for volume/track control and a custom 3D-printed enclosure.

---

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Bill of Materials (BOM)](#bill-of-materials-bom)
- [Wiring Diagram](#wiring-diagram)
- [Repository Structure](#repository-structure)
- [Setup & Usage](#setup--usage)
- [3D Printing](#3d-printing)

---

## Overview

This project fetches real-time song data from the **Spotify Web API** over Wi-Fi using the ESP32. The song title and artist are rendered on a 1.8" TFT display using the SPI protocol. A rotary encoder allows the user to adjust volume and skip tracks. The entire assembly fits inside a custom 3D-printed enclosure modeled in Onshape.

---

## Features

- Real-time Spotify track display (song title + artist)
- Wi-Fi connectivity via ESP32
- Rotary encoder for volume control and track skipping
- MX switches for play/pause and skip
- Custom 3D-printed enclosure (no screws - friction fit)
- SPI communication with TFT display

---

## Bill of Materials (BOM)

| Name | Purpose | Qty | Unit Cost (USD) | Total | Distributor | Link |
|------|---------|-----|-----------------|-------|-------------|------|
| HiLetgo 1.8" ST7735R SPI TFT LCD Display | Displays song and artist info | 1 | $9.69 | $9.69 | Amazon | [Link](https://www.amazon.com/dp/B00LSG51MM) |
| C3 Mini V2.1.0 ESP32 | Main microcontroller and brain | 1 | $5.66 | $5.66 | AliExpress | [Link](https://www.aliexpress.us/item/3256804553736450.html) |
| WWZMD1B 6Pcs EC11 Rotary Encoder with Push Button | Volume and track control | 1 | $8.88 | $8.88 | Amazon | [Link](https://www.amazon.com/dp/B0C6Q67V97) |
| MX Switches | Play/pause and skip controls | 1 | $0.00 | $0.00 | Myself | - |

**Total Estimated Cost: ~$24.23**

Full BOM CSV available in [`bom/SpotifyDisplayV3BOM.csv`](bom/SpotifyDisplayV3BOM.csv).

---

## Wiring Diagram

```
ESP32 C3 Mini Pinout
====================

TFT Display (ST7735R) -> ESP32 C3 Mini
--------------------------------------
VCC   ->  3.3V
GND   ->  GND
CS    ->  GPIO 1 (TFT_CS)
RESET ->  GPIO 2 (TFT_RST)
DC    ->  GPIO 3 (TFT_DC)
SDA   ->  GPIO 5 (MOSI / SPI)
SCL   ->  GPIO 4 (SCK / SPI)
BL    ->  3.3V (Backlight always on)

Rotary Encoder (EC11) -> ESP32 C3 Mini
--------------------------------------
CLK  ->  GPIO 6  (ENC_CLK)
DT   ->  GPIO 7  (ENC_DT)
SW   ->  GPIO 8  (ENC_SW, optional push button)
VCC  ->  3.3V
GND  ->  GND

MX Switches -> ESP32 C3 Mini
------------------------------
Switch 1 (Play/Pause)  ->  GPIO 9
Switch 2 (Next Track)  ->  GPIO 10
Switch 3 (Prev Track)  ->  GPIO 11
Other pin              ->  GND
(Internal pull-up resistors enabled in firmware)

Power
-----
ESP32 powered via USB-C
```

