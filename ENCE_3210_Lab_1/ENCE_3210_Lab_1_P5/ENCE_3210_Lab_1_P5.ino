/*
 * 
 * Kevin Granillo
 * January 16, 2025
 * 
 * This program detects a pattern in a given array. 
 * The green LED is turned on everytime the pattern is detected
 * If the pattern is not detected by the end, the red LED turns on
 * The number of occurences is counted and saved as a variable
 * 
 * 
 */

int patternCount = 0; // Pattern counter
int myArray[45]= {2,2,5,7,8,3,3,5,6,8,6,7,9,3,2,4,3,5,6,1,1,1,3,5,6,8,3,1,6,2,7,3,3,5,6,8,1,2,3,9,8,7,6,4,5}; // Given array
int patternArray[3] = {3,5,6}; // Pattern that is being searched for
#define RED_LED 13 // Defines LED pins
#define GREEN_LED 12
#define myArray_Length 42 // Array length -3 to account for "for" loop

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(RED_LED, OUTPUT); // LEDs are set as outputs
  pinMode(GREEN_LED, OUTPUT);
  digitalWrite(RED_LED,LOW); // LEDs begin off
  digitalWrite(GREEN_LED,LOW);

  for(int i=0;i<myArray_Length;i++){ // for loop that itterates through array and compares all three elements in pattern array to three elements in given array
    if(myArray[i] == patternArray[0] && myArray[i+1] == patternArray[1] &&  myArray[i+2] == patternArray[2]){
      patternCount++; // Pattern count goes up by 1 when pattern detected
      digitalWrite(GREEN_LED, HIGH); // Green LED turns off
      delay(500);
      digitalWrite(GREEN_LED, LOW);
      delay(500);
    }  
  }
  if(patternCount == 0){ // No patterns detected
    digitalWrite(RED_LED, HIGH); // Red LED turns on
  }
  Serial.print("Pattern Count: "); // Prints pattern count
  Serial.println(patternCount);

}

void loop() {
  // put your main code here, to run repeatedly:

}
