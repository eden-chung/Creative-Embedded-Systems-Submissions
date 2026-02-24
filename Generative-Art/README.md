# Generative Art on ESP32 Project

![Image display](image_display.png)

## Materials List
- LilyGo ESP32
- Battery
- USB-C Cable

## Instructions to set up
### 1. Setting up ESP32

- Install USB driver using \
`brew install --cask wch-ch34x-usb-serial-driver`
- On VSCode, install platformio extension
- Create a new platformio project with board selected as TTGO T1
- Run these two commands from the project dir \
`pio pkg search "tft espi"` \
`pio pkg install -l bodmer/TFT_eSPI`

Replace the platformio.ini file with this [file](platformio.ini)

### 2. Scraping words from HackerNews
For this project, words are being shown from the front page of HackerNews over the past 30 days, from February 20th, 2026.

There are a few steps of preprocessing that has to be done before the text can be rendered on the ESP32.

- Scrape the words from HackerNews using this script, [scrape.py](scrape.py). This will output [hn_word_counts.csv](hn_word_counts.csv) \
`python scrape.py`
- To easily transfer this information to the ESP32, we run another Python script to create a C++ header that can be imported. Use this Python script, [csv_to_arduino.py](csv_to_arduino.py) by running \
`python csv_to_arduino.py`.
- The output file should be [words.h](words.h). Now, import this into the platform IO project.

### 3. Rendering the generative art on the ESP32

- Now, in the project directory, under `src`, we should have the [words.h](words.h) generated in the last step
- To show the text on the screen, import [main.cpp](main.cpp) into this same `src` folder.
- Now connect the ESP32 via USB-C cable and run: \
`pio run` \
`pio run -t upload`