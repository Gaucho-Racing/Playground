#include <ILI9341_t3.h>
#include <font_Arial.h> // from ILI9341_t3
#include <XPT2046_Touchscreen.h>
#include <SPI.h>

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

double max1 = 0;
double min1 = 1000;
double max2 = 0;
double min2 = 1000;

double throttle1 = 0.0;
double throttle2 = 0.0;

boolean neutralEngaged = false;

void showThrottleScreen() {
  tft.setTextSize(2);

  tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
  tft.setCursor(10, 100);
  tft.print("T: " + String(throttle1) + ", " + String(throttle2));
  
  tft.setTextColor(ILI9341_CYAN, ILI9341_BLACK);
  tft.setCursor(10, 150);
  tft.print("Min: " + String(min1) + ", " + String(min2));

  tft.setTextColor(ILI9341_CYAN, ILI9341_BLACK);
  tft.setCursor(10, 180);
  tft.print("Max: " + String(max1) + ", " + String(max2));
}

void showNeutralScreen() {
  tft.setTextSize(4);
  tft.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);
  tft.setCursor(10, 100);
  tft.print("NEUTRAL");
  
  tft.setTextSize(2);
  tft.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);
  tft.setCursor(10, 150);
  tft.print("T: " + String(throttle1) + ", " + String(throttle2));
}

void setup() {
  // put your setup code here, to run once:
  pinMode(21, INPUT);
  pinMode(22, INPUT);
  pinMode(3, INPUT);
  
  Serial.begin(38400);
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(ILI9341_BLACK);
  ts.begin();
  ts.setRotation(1);
  while (!Serial && (millis() <= 1000));

  tft.fillScreen(ILI9341_BLACK);
  tft.setTextSize(2);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead(3)) {
    // Neutral on
    Serial.println("Neutral: ON");
    if (!neutralEngaged) {
      neutralEngaged = true;
      tft.fillScreen(ILI9341_BLACK);
    }
    showNeutralScreen();
  }
  else {
    Serial.println("Neutral: OFF");
    // Neutral off
    if (neutralEngaged) {
      neutralEngaged = false;
      tft.fillScreen(ILI9341_BLACK);
    }
    showThrottleScreen();
  }
  
  throttle1 = analogRead(21);
  if (throttle1 > max1) max1 = throttle1;
  if (throttle1 < min1) min1 = throttle1;
  throttle2 = analogRead(22);
  if (throttle2 > max2) max2 = throttle2;
  if (throttle2 < min2) min2 = throttle2;
  Serial.println("Port 21: " + String(throttle1) + " – Port 22: " + String(throttle2));
  
  delay(100);
}
