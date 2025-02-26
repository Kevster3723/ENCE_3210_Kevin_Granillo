/*
*
* Kevin Granillo
* February 26, 2025
*
* This code is made for a thermometer that uses a one wire digital temperature sensor
* The criteria for the thermometer are that it must switch between celcius and fahrenheit
* It must flip the screen orientation depending on the gyro readings
* It must have a button for the light on the back lit display
* Lastly, it must lock the reading when a button is pressed
*
* Side note, everything works well, the only issue ecnountered was with the mechanical buttons which bounce
*
*/

// Defining all buttons used
#define BUTTON_LOCK 3
#define BUTTON_CAL 5
#define BUTTON_UNITS 4
#define BUTTON_LIGHT 6
#define BUTTON_ONOFF 2

#define LED 9 // LED for backlit display

#define TEMP_DIGITAL 8 // Digital Temp Sensor
#define TEMP_ANALOG A0 // Potentiometer playing as analog Temp Sensor

#define TIMERCOMPAREVALUE1 46875 // Resets Timer 1 after 3 seconds
#define TIMERCOMPAREVALUE2 255 // Resets Timer 2 after overflow

#define ONE_WIRE_BUS 8 // Temp Sensor Pin
#define OUTPUT_READABLE_ACCELGYRO

// All included libraries for Temp sensor, gyro, and Screen
#include <SPI.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <I2Cdev.h>
#include <MPU6050.h>
#include "images.h" // Images displayed on screen (LOCK, UNCLOCKED LOCK, C,F)

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

enum State_e{OFF,ON,LOCK}; // Defines state machine (On, Off, and Locked)
enum State_e gState = OFF; // Default state is off

// Global Variables for Temp
float gTempC = 0;
float gTempF = 0;

// Global Variables used as flags
char gFlag_OnOff = 0;
char gFlag_Lock = 0;
char gFlag_Units = 0; 
char gFlag_Light = 0;
char gFlag_ReadTemp = 0;

// Variables for Gyroscope
int16_t ax, ay, az;
int16_t gx, gy, gz;
MPU6050 accelgyro;
//MPU6050 accelgyro(0x69); // <-- use for AD0 high



void setup() {
  Serial.begin(9600); // Begin Serial Comunication 

  // Buttons and Sensors are Inputs, LED is output
  pinMode(BUTTON_LOCK,INPUT);
  pinMode(BUTTON_CAL,INPUT);
  pinMode(BUTTON_UNITS,INPUT);
  pinMode(BUTTON_LIGHT,INPUT);
  pinMode(BUTTON_ONOFF,INPUT);
  pinMode(TEMP_DIGITAL,INPUT);
  pinMode(TEMP_ANALOG,INPUT);
  pinMode(LED,OUTPUT);

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
  Wire.begin();
#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
  Fastwire::setup(400, true);
#endif

  sensors.begin();
  accelgyro.initialize();

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds
  // Clear the buffer
  display.clearDisplay();

  attachInterrupt(digitalPinToInterrupt(BUTTON_ONOFF), isr_onoff,CHANGE); // Attach interrupt to OnOff button
  attachInterrupt(digitalPinToInterrupt(BUTTON_LOCK), isr_lock, CHANGE); // Attach interrupt to Lock button

  noInterrupts(); // Disable interrupts
  TCCR1B = 0; // Timer 1
  TCCR1A = 0;

  TCCR2B = 0; // Timer 2
  TCCR2A = 0;

  TCCR2B |= (1<<WGM21); // Clock set to CTC
  TCCR2B |= (1<<CS22); //Clock 2 prescale by 1024
  TCCR2B |= (1<<CS21);
  TCCR2B |= (1<<CS20);

  OCR1A = TIMERCOMPAREVALUE1; // Reset after 3 seconds
  OCR2A = TIMERCOMPAREVALUE2; // Reset after overflow

  TIMSK1 |= (1<<OCIE1A); // Attach interrupt to timer 1
  TIMSK2 |= (1<<OCIE2A); // Attach interrupt to timer 2
  
  interrupts(); // Enable interrupts
}

void loop() {
  // put your main code here, to run repeatedly:
  display.clearDisplay(); // Clear display after each itteration 
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz); // Read Gyro

  switch(gState){ // State Machine
  
    case OFF:
      display.clearDisplay(); // When State is off, display is off
    break;

    case ON: // When State is on, the temperature is read, units are attached and temp is displayed
      ReadTemp();
      Units();
      DisplayTemp();
    break;

    case LOCK:
      DisplayTemp(); // When Lock state, Temp no longer read just displayed with units
      Units();
    break;

  }
  OnOff(); // Checks if OnOff button has been pressed
  LockStatus(); // Checks if Lock button has been pressed
  Light(); // Checks if Light button has been pressed
  display.display(); // Updates the display
}

void OnOff()
{
  if(gFlag_OnOff) // If the button has been pressed, on off state will be toggled
  {
    gState = ON;
  } else 
  {
    gState = OFF;
  }
}

void LockStatus()
{
  if(gFlag_Lock && gFlag_OnOff) // If the Lock button has been pressed change state to Lock
  {
    gState = LOCK;
  }
}

void Units()
{
  if(digitalRead(BUTTON_UNITS)) // If Units button has been pressed initialize timer 1
  {
    TCCR1B |= (1<<WGM12);
    TCCR1B |= (1<<CS12);
    TCCR1B |= (1<<CS10);
  } else // If button is no longer pressed, stop timer 1
  {
    TCCR1B &= (0<<WGM12);
    TCCR1B &= (0<<CS12);
    TCCR1B &= (0<<CS10);
  }
}

void ReadTemp()
{
  if(gFlag_ReadTemp) // If timer 2 has reset, read temperature
  {
    sensors.requestTemperatures();
  }
  gTempC = sensors.getTempCByIndex(0); // Temperature in Celsius 
  gTempF = sensors.getTempFByIndex(0); // Temperature in Fahrenheit
}

void DisplayTemp()
{
    display.clearDisplay();
    display.setTextSize(2);      // Normal 2:2 pixel scale
    display.setTextColor(SSD1306_WHITE); // Draw white text
    display.setCursor(0, 10);     // Start on left side halfway down
    display.cp437(true);  
  if(gFlag_Units)
  {
    display.print(gTempF); // Display Temp in Fahrenheit
    display.drawBitmap(0,0, images[3],SCREEN_WIDTH,SCREEN_HEIGHT,WHITE); // If Temp is in Fahrenheit, Display "F" after temp
  } else 
  {
    display.print(gTempC); // Display Temp in Celcius
    display.drawBitmap(0,0, images[2],SCREEN_WIDTH,SCREEN_HEIGHT,WHITE); // If Temp is in Fahrenheit, Display "F" after temp
  }
  if(gState == ON)
  {
    display.drawBitmap(0,0, images[1],SCREEN_WIDTH,SCREEN_HEIGHT,WHITE); // If not Locked, display open lock
  } else if(gState == LOCK)
  {
    display.drawBitmap(0,0, images[0],SCREEN_WIDTH,SCREEN_HEIGHT,WHITE); // If locked, display closed lock
  }
#ifdef OUTPUT_READABLE_ACCELGYRO
  // Read x acceleration and orient screen accordingly
    if (ax < 0){ 
      display.setRotation(2); // Flips screen 180 degrees
    }
    else{
      display.setRotation(0); // Shows default screen orientation
    }
  #endif
}

void Light()
{
  if(digitalRead(BUTTON_LIGHT) && gState != OFF) // If button light is pressed and the screen is not off
  {
    gFlag_Light = !gFlag_Light; // Toggle flag
  }
  if(gFlag_Light) // If true, turn on LED else turn off
  {
    digitalWrite(LED,HIGH);
  } else
  {
    digitalWrite(LED,LOW);
  } 
}


void isr_onoff() // Interrupt service routine for OnOff button
{
  if(digitalRead(BUTTON_ONOFF)) // If on off button pressed, toggle flag to turn screen on or off
  {
    gFlag_OnOff = !gFlag_OnOff;
  }
}

void isr_lock() // Interrupt Service Routine for Lock button
{
  if(digitalRead(BUTTON_LOCK) && gState != OFF) // If button is pressed toggle flag to lock and unlock
  {
    gFlag_Lock = !gFlag_Lock;
  }
}

ISR(TIMER1_COMPA_vect) // Interrupt service routine for Timer 1
{
  gFlag_Units = !gFlag_Units; // Flag is toggled after 3 seconds
}

ISR(TIMER2_COMPA_vect) // Interrupt service routine for Timer 2
{
  gFlag_ReadTemp = !gFlag_ReadTemp; // After timer 2 overflow toggle flag to read Temp
}