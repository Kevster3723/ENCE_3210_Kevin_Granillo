/*
* 
* Kevin Granillo
* March 1, 2025
*
* This program is Solar Charge Controller
* It uses a switching component to switch between solar power and battery power
* If the voltage of the solar panel is greater than 2.5, this becomes the power supplier
* If less than 2.5, the battery becomes the power supplier
* If the voltage of the battery is less than the voltage of the solar pane, the battery is charged by the solar panel
* Lastly, the battery stops being charged after it is fully charged
*
*
* Side note: Not all of the components are available so the switching element is emulated by using an OLED screen
*
*
*/

// Libraries for screen
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Defining Screen Dimensions
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

float gBattery = 4; // Battery Voltage
float gSolar = 5; // Solar Voltage
float gFullyCharged = 4; // Battery fully charged

  enum State_e{SOLAR,BATTERY,CHARGE}; // Solar controlling microcontroller, Battery Controlling Microcontroller, Solar charging battery
  enum State_e gState = SOLAR; // Default State is Solar charging

void setup() {
  // Begin serial communication
  Serial.begin(9600);

    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  // Displays startup logo then clears the screen
  display.display();
  display.clearDisplay();
  display.display();

}

void loop() {
  // put your main code here, to run repeatedly:
    display.clearDisplay();
    display.setTextSize(1);      // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE); // Draw white text
    display.setCursor(0, 10);     // Start on left side halfway down
    display.cp437(true);

  switch(gState){
    case SOLAR:
      display.print("SOLAR POWER!"); // Solar is controlling microcontroller
    break;

    case BATTERY:
      display.print("BATTERY POWER!"); // Battery is powering microcontroller
    break;

    case CHARGE:
      display.print("BATTERY CHARGING"); // Solar is charging battery
      if(gBattery = gFullyCharged) // If battery is at max voltage change solar to microcontroller
      {
        gState = SOLAR;
      }
    break;
  }
  CheckVoltage(); // Checks voltage of battery and solar
  display.display();
}

void CheckVoltage()
{
  if(gSolar >= 2.5) // If solar is greater than 2.5 V
  {
    gState = SOLAR; // Solar for microcontrollers
  } else 
  {
    gState = BATTERY; // If solar is less than 2.5V, Battery for microcontrollers
  }
  if(gSolar<gBattery) // If solar voltage is less than battery voltage
  {
    gState = CHARGE; // Charge battery
  }
}