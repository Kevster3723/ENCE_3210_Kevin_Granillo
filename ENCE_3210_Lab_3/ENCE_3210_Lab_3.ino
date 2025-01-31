/*
*
* Kevin Granillo
* January 31, 2025
* 
* This program uses two different timers, interrupts, and volatile variables.
* This program uses one clock to control an LED blinking at 1Hz and a printed message
* It uses the second timer which runs at 10Hz and is responsible for turning on an LED when a button is pressed
*
*
*/
 //Defines Pins
#define RED_LED 5
#define GREEN_LED 4
#define BUTTON 2

#define LEDTime 50 // LED Time is half a second

#define TimerCompareValue1 31250 // Half a second for timer to reset
#define TimerCompareValue2 157 // 100ms for timer to reset

// Volatile variables that change in multiple places
volatile unsigned int gTimerOneFlag = 0; 
volatile unsigned int gISRFlag1 = 0;
volatile unsigned long gCounter1 = 0;
volatile unsigned long gCounter2 = 0;

void setup() {
  Serial.begin(9600); // Sets up communication
  pinMode(RED_LED,OUTPUT); // Sets LEDs as outputs and buttons as inputs
  pinMode(GREEN_LED,OUTPUT);
  pinMode(BUTTON,INPUT);

  //attachInterrupt(digitalPinToInterrupt(BUTTON), isr_button,CHANGE);

  noInterrupts();
  TCCR1B = 0; // Timer 1
  TCCR2B = 0; // Timer 2
  TCCR1A = 0;
  TCCR2A = 0; 
  OCR1A = TimerCompareValue1; // Clock Reset half a second
  OCR2A = TimerCompareValue2; // Clock Reset 100ms

  TCCR1B |= (1<<WGM12); // Clock 1 set to CTC
  TCCR1B |= (1<<CS12); // Prescale by 256
  TCCR2B |= (1<<WGM21); // Clock 2 set to CTC
  TCCR2B |= (1<<CS22); //Clock 2 prescale by 1024
  TCCR2B |= (1<<CS21);
  TCCR2B |= (1<<CS20);


  TIMSK1 |= (1<<OCIE1A); // Timer interrupt
  TIMSK2 |= (1<<OCIE2A); // Timer interrupt

  interrupts();
}

void loop() {

  if(gISRFlag1) // When flag 1 is triggered, turn on LED for half a second, turn off for half a second
  {
    digitalWrite(GREEN_LED,HIGH);
    delay(LEDTime);
    digitalWrite(GREEN_LED,LOW);
    if(gCounter1 == 6) // Prints message every three seconds
    {
      Serial.println("Have a Good Day");
      gCounter1 = 0; // Resets Counter
    }
    
  }
}

ISR(TIMER1_COMPA_vect)
{
  gISRFlag1 = ~gISRFlag1; // Triggers flag
  gCounter1++; // Adds to counter
}

ISR(TIMER2_COMPA_vect)
{
  gCounter2++; // Adds to counter
  if(gCounter2 == 10){ // Frequency of 10 Hz
     if(digitalRead(BUTTON)) // If button is pressed
    {
      digitalWrite(RED_LED,HIGH); // Turn on LED
    } else
    {
      digitalWrite(RED_LED,LOW); // Turn off LED
    } 
    gCounter2 = 0; // Reset counter
  }
}