/*
 *
 * Kevin Granillo
 * January 24, 2025
 *
 * This program starts with an array that has random element put in using a for loop
 * When a button is pressed, an ISR is called and a second array is defined as y[n] = 2*x[n] - x[n-1]
 *
 *
 */

// Define button pin
#define Button 2

// Set up arrays and button flag
long gX[10] = {};
long gY[10] = {};
volatile int gFlag = 0;

void setup() {
  // Set up button pin and serial port
  Serial.begin(9600);
  pinMode(Button,INPUT_PULLUP);

  // For loop that puts random numbers in first array
  for(int i=0;i<10;i++)
  {
    gX[i] = random(0,100);
  }

  // Attach interrupt to button pin
  attachInterrupt(digitalPinToInterrupt(Button), isr_button, CHANGE);
}

void loop() {
  if(gFlag == 1) // If flag is active, run the operation
  {
    for(int i=0;i<10;i++)
    {
      gY[i] = 2*gX[i]-gX[i-1];
    }
    gFlag = 0; // Reset Flag
  }

}

void isr_button()
{
  gFlag = 1; // Change Flag
}