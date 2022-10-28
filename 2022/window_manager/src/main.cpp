#include <ILI9341_t3.h>
#include <XPT2046_Touchscreen.h>
#include <SPI.h>
#include "../lib/ArrayList.h"
#include "../lib/Automaton/src/Automaton.h"

#define CS_PIN  8
#define TIRQ_PIN  2
#define TFT_DC      9
#define TFT_CS      10
#define TFT_RST    255  // 255 = unused, connect to 3.3V
#define TFT_MOSI    11
#define TFT_SCLK    13
#define TFT_MISO    12
ILI9341_t3 tft = ILI9341_t3(TFT_CS, TFT_DC, TFT_RST, TFT_MOSI, TFT_SCLK, TFT_MISO);

XPT2046_Touchscreen ts(CS_PIN, TIRQ_PIN);

#define UP_BUTTON 3
#define DOWN_BUTTON 4
#define LEFT_BUTTON 5
#define RIGHT_BUTTON 6

/*
 * 0 - None
 * 1 - Up
 * 2 - Down
 * 3 - Left / Back
 * 4 - Right / Forward
 */
int buttonStatus = 0;

ArrayList<int> buttonEvents;
Atm_button upBtn;
Atm_button downBtn;
Atm_button leftBtn;
Atm_button rightBtn;

void button_press(int idx, int v, int up) {
    Serial.println(idx);
    Serial.println(v);
    Serial.println(up);
}

void setupSkeleton() {
    tft.fillScreen(ILI9341_BLACK);
    tft.drawRoundRect(0, 0, 320, 32, 8, ILI9341_CYAN);
}

void setup() {
    tft.begin();
    tft.setRotation(1);
    tft.fillScreen(0xffff);

    upBtn.begin(UP_BUTTON).onPress(button_press, 1);
    downBtn.begin(DOWN_BUTTON).onPress(button_press, 2);
    leftBtn.begin(LEFT_BUTTON).onPress(button_press, 3);
    rightBtn.begin(RIGHT_BUTTON).onPress(button_press, 4);

    setupSkeleton();
}

void loop() {
    // put your main code here, to run repeatedly:
    automaton.run();
}