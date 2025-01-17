/*
 * Kevin Granillo
 * January 16, 2025
 * 
 * This program is a calculator with three different operations. 
 * When a button is pressed the red LED turns on showing that addition has been selected. 
 * When the button is pressed again, the green LED turns on showing subtraction has been selected
 * When the button is pressed for a third time, the blue LED turns on showing multiplication has been selected
 * 
 * When the second button is pressed, the selected operation will be applied to the first elements of an array and all the lights turn off
 * The operation is then applied to the rest of the array and all the lights turn off
 * The calculator can be used again by pressing the first button
 * 
 * 
 * 
 */
 // Assign pins to the buttons and LEDS
#define RED_LED 13
#define GREEN_LED 12
#define BLUE_LED 11
#define BUTTON_1 2
#define BUTTON_2 3

// Sets a button counter
int buttonCounter = 0;

// Sets arrays with numbers as constant integers
const int arrayOne[10] = {1,3,5,2,4,7,2,9,2,8};
const int arrayTwo[10] = {4,7,2,5,1,3,7,8,9,9};
// Answers are saved in this array
int answers[10] ={};
// Length of arrays for "for" loops
const int arrayLength = 10;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  // Sets LEDS as outputs
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  // Sets buttons as inputs with pullup resistors
  pinMode(BUTTON_1, INPUT_PULLUP);
  pinMode(BUTTON_2, INPUT_PULLUP);
  // Starts all LEDS as off
  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(BLUE_LED, LOW);

}

void loop() {
  // If the first button is pressed, the button counter goes up by 1
  if(digitalRead(BUTTON_1)){
    buttonCounter++;
    // Delay for digital debouncing
    delay(1000);
  }

  // If statement reads the amount of times button 1 has been pressed
  if(buttonCounter == 1){
    // Turns Red LED on and all other LEDs off
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BLUE_LED, LOW);
    // If statement activates if second button is pressed while Red LED is on
    if(digitalRead(BUTTON_2)){
      // Adds first elements in the arrays and saves them in the third array
      answers[0] = arrayOne[0]+arrayTwo[0];
      // Turns off all the LEDS
      digitalWrite(RED_LED, LOW);
      digitalWrite(GREEN_LED, LOW);
      digitalWrite(BLUE_LED, LOW);
      delay(1000);
      // Adds the rest of the elements in the array
      for(int i =1; i<arrayLength;i++){
        answers[i] = arrayOne[i]+arrayTwo[i];
      }
      // All LEDS are turned off and calculator is reset
      digitalWrite(RED_LED, HIGH);
      digitalWrite(GREEN_LED, HIGH);
      digitalWrite(BLUE_LED, HIGH);
      delay(1000);
      buttonCounter = 0;
    }
  }
  // If statement activates when first button is pressed twice
  if(buttonCounter == 2){
    // Green LED turns on and all others turn off
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(RED_LED, LOW);
    digitalWrite(BLUE_LED, LOW);
    // If statement activates when second button is pressed while Green LED is on
    if(digitalRead(BUTTON_2)){
      // Subtracts the first elements of the two arrays and saves the results in the third array
      answers[0] = arrayOne[0]-arrayTwo[0];
      // All LEDs turn off
      digitalWrite(RED_LED, LOW);
      digitalWrite(GREEN_LED, LOW);
      digitalWrite(BLUE_LED, LOW);
      delay(1000);
      // Subtracts the rest of the array elements and saves them in the third array
      for(int i =1; i<arrayLength;i++){
      answers[i] = arrayOne[i]-arrayTwo[i];
     }
     // Turns on all the LEDs
      digitalWrite(RED_LED, HIGH);
      digitalWrite(GREEN_LED, HIGH);
      digitalWrite(BLUE_LED, HIGH);
      delay(1000);
      // Resets calculator
      buttonCounter = 0;
  }
}
  // Turns on the Blue LED when the button is pressed is three times
  if(buttonCounter == 3){
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, LOW);
    digitalWrite(BLUE_LED, HIGH);
    //  If second button is pressed while blue LED is on multipllication is activated
    if(digitalRead(BUTTON_2)){
      // First two elements are multiplied together and saved in third array
      answers[0] = arrayOne[0]*arrayTwo[0];
      // All LEDs turn off
      digitalWrite(RED_LED, LOW);
      digitalWrite(GREEN_LED, LOW);
      digitalWrite(BLUE_LED, LOW);
      delay(1000);
      // The rest of the elements are multiplied
      for(int i =1; i<arrayLength;i++){
        answers[i] = arrayOne[i]*arrayTwo[i];
      }
      // All LEDs turn on
      digitalWrite(RED_LED, HIGH);
      digitalWrite(GREEN_LED, HIGH);
      digitalWrite(BLUE_LED, HIGH);
      delay(1000);
      // Calculator is reset
      buttonCounter = 0;
    }
    }
  } 
