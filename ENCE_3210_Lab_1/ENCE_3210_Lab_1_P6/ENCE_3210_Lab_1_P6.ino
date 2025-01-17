/*
 * Kevin Granillo
 * January 16, 2025
 * 
 * This program turn on and off LEDs in a sequence when a button is pressed
 * The order is: R-RG-RGB-GB-B-RB
 * This sequence continues repeating until the button is pressed again
 * 
 * Sidenote: The button changes values multiple times because of the imperfections of the mechanical switch
 * Because of this the LEDS might not respond right away or button may need to be pressed again
 * 
 * 
 * 
 */
 
// LED and Button pins are defined
#define RED_LED 13
#define GREEN_LED 12
#define BLUE_LED 11
#define BUTTON 2

bool patternState = 1; // Pattern state toggles between zero and one which turns the sequency on and off

void setup() {
  Serial.begin(9600);

  pinMode(RED_LED, OUTPUT);   // LEDs are set as outputs
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  pinMode(BUTTON,INPUT_PULLUP); // Button is set as input with a pullup resistor
  attachInterrupt(digitalPinToInterrupt(BUTTON), isr_button,CHANGE);  // Iterrupt is added to button
  

}

void loop() {
  if(patternState == 0){ // Runs when the pattern state is 0
    digitalWrite(RED_LED, HIGH); // Follows the sequence mentioned above
    delay(200);
    digitalWrite(GREEN_LED, HIGH);
    delay(200);
    digitalWrite(BLUE_LED,HIGH);
    delay(200);
    digitalWrite(RED_LED,LOW);
    delay(200);
    digitalWrite(GREEN_LED,LOW);
    delay(200);
    digitalWrite(RED_LED,HIGH);
    delay(200);
    digitalWrite(RED_LED,LOW); // All LEDs turn off
    digitalWrite(GREEN_LED,LOW);
    digitalWrite(BLUE_LED,LOW);
    delay(200);
      
  } else if(patternState == 1){ // Runs when pattern state is 1
    digitalWrite(RED_LED,LOW); // All LEDs remain off
    digitalWrite(GREEN_LED,LOW);
    digitalWrite(BLUE_LED,LOW);
    
  }

delay(50); // Delay is used to digitally debounce the button, otherwise patternState will toggle multiple times between 0 and 1
}

void isr_button(){ // Interrupt for when button is pressed
  
  if(patternState == 0){ // If the LEDs are in sequence, turn them off
    patternState = 1;
  } else if(patternState == 1){ // If LEDs are off, turn pattern sequence on
    patternState = 0;
  } 
}
