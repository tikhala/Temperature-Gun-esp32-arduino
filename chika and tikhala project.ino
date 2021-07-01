#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_MLX90614.h>
#include <Fonts/FreeMonoBold18pt7b.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

float temperature;

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

void setup() {
  Serial.begin(9600);
  mlx.begin();
  pinMode(4, OUTPUT);
  pinMode(7, INPUT_PULLUP);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }
  display.clearDisplay();
}

void loop() {
  if (digitalRead(7) == LOW) {
    temperature = mlx.readObjectTempC();
    if (temperature > 37) {
      digitalWrite(4, HIGH);
    } else {
      digitalWrite(4, LOW);
    }
    printText();
    delay(500);
  }
  display.clearDisplay();
}

void printText() {
  display.setFont(&FreeMonoBold18pt7b);
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(0, 50);
  display.print(temperature, 1);
  display.drawCircle(92, 35, 3, WHITE);
  display.setCursor(100, 50);
  display.print("C");
  display.display();
}
