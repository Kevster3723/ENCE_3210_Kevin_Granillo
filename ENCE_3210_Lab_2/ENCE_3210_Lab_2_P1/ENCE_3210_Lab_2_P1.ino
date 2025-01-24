/*
 *
 * Kevin Granillo
 * January 24, 2025
 *
 * This program compares the amount of times two buttons have been pressed
 * If button 1 has been clicked more times, a red LED turns on
 * If button 2 has been clicked more times, a green LED turns on
 * If the buttons have been clicked an equal amount of times, a blue LED turns on
 * This program us designed to use button interrupts to work
 *
 * Side note: Program may not work due to the physical properties of the buttons. 
 * The button may register more than one click when pressed due to imperfect contacts
 *
 */

 // Defining LED and Button Pins
#define RED_LED 13
#define GREEN_LED 12
#define BLUE_LED 11
#define BUTTON_1 2
#define BUTTON_2 3

// Defining button counters
int gButtonCounter_1 = 0;
int gButtonCounter_2 = 0;


void setup() {
  // Defining input modes
  Serial.begin(9600);
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  pinMode(BUTTON_1, INPUT_PULLUP);
  pinMode(BUTTON_2, INPUT_PULLUP);

  // All LEDs begin off
  digitalWrite(RED_LED,LOW);
  digitalWrite(GREEN_LED,LOW);
  digitalWrite(BLUE_LED,LOW);

  // Buttons are attached to interrupts
  attachInterrupt(digitalPinToInterrupt(BUTTON_2), isr_button_1 ,CHANGE);
  attachInterrupt(digitalPinToInterrupt(BUTTON_1), isr_button_2 ,CHANGE);

}

void loop() {

  // The respective LED is turned on depending in the number of clicks each button has
  if(gButtonCounter_1 > gButtonCounter_2){ // Button 1 > Button 2: Red LED
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED,LOW);
    digitalWrite(BLUE_LED, LOW);
  } else if (gButtonCounter_1 < gButtonCounter_2){ // Button 2 > Button 1: Green LED
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED,HIGH);
    digitalWrite(BLUE_LED, LOW);
  } else { // Button 1 = Button 2: Blue LED
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED,LOW);
    digitalWrite(BLUE_LED, HIGH);
  }
}

void isr_button_1(){ // Button 1 interrupt increases button 1 counter
  if(digitalRead(BUTTON_1))
  {
    gButtonCounter_1++;
  }
}

void isr_button_2(){ // Button 2 interrupt increases button 2 counter
  if(digitalRead(BUTTON_2))
  {
    gButtonCounter_2++;
  }
}
