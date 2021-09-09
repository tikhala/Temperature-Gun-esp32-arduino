#include <SPI.h>
#include <Wire.h>
#include <WiFi.h> 
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_MLX90614.h>
#include <Fonts/FreeMonoBold18pt7b.h>
#include <Fonts/FreeMono9pt7b.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     12 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

float temperature;
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

//defining the wifi network parameters------------------------------------------------------------------------------
const char* ssid = "Galaxya20";           
const char* password = "presidentdoro";

// Function declaration --------------------------------------------------------------------------------------------------

// Function for printing the temperature value-----------------------------------------------------------------------------
void printText() {
  display.setFont(&FreeMonoBold18pt7b);
  display.setTextColor(WHITE);
  display.setCursor(0, 50);
  display.print(temperature, 1);
  display.drawCircle(92, 35, 3, WHITE);
  display.setCursor(100, 50);
  display.print("C");
  display.display();
}

//connecting the esp to the wifi network-------------------------------------------------------------------
void connectwifi(){
   WiFi.begin(ssid, password);
  if (WiFi.status() != WL_CONNECTED) {
    delay(100);
    printNotConnected();}
    else {
      printConnected();}
  }

// functions for printing the Wi-fi connectivity status--------------------------------------------------------------------
void printNotConnected(){
  display.setFont(&FreeMono9pt7b);
  display.setTextColor(WHITE);
  display.setCursor(0,14); // cursor position (x,y)on the display
  display.print("NOTConn");
  display.display();
  
  }

 void printConnected(){
  display.setFont(&FreeMono9pt7b);
  display.setTextColor(WHITE);
  display.setCursor(0,14); // cursor position (x,y)on the display
  display.print("Connected");
  display.display();
  
  }
  
void setup() {
  Serial.begin(9600);
  mlx.begin();
        
//setting the pins for input and output -------------------------------------------------------------------
 
  pinMode(12, OUTPUT); // pin for buzzer defined as output
  pinMode(14, INPUT_PULLUP);// pin for button activator to start reading temperature

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }
  display.clearDisplay();
  }
 

void loop() {
  if (digitalRead(14) == LOW) { // push button inactive pin 14
    temperature = mlx.readObjectTempC();
    if (temperature > 37) {
      digitalWrite(12, HIGH);// buzzer here activated pin 12
    } else {
      digitalWrite(12, LOW);// buzzer here deactivated pin 12
    }
    connectwifi();
    printText();
   
    delay(500);
  }
  display.clearDisplay();
}
