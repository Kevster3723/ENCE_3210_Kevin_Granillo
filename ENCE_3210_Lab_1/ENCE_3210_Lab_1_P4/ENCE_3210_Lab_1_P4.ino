/*
 * 
 * Kevin Granillo
 * January 16, 2025
 * 
 * This program reads an array filled with R, G, and B, and turns on the red, green, or blue LEDs corresponding to the letters
 * It also counts the amount of occurences for each letter and prints them
 * 
 * 
 */
const char Red = 'R'; // Characters are defined
const char Blue = 'B';
const char Green = 'G';
char charArray[10] = {'R','R','G','R','B','G','R','B','R','G'}; // Array of letters is defined
#define RED_LED 13 // LED pins are defined
#define GREEN_LED 12
#define BLUE_LED 11
long RED_counter = 0; // RGB counters are defined
long BLUE_counter = 0;
long GREEN_counter = 0;
long charArray_length = 10; // Array length is defined

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(RED_LED, OUTPUT); // LEDs are set as outputs
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  digitalWrite(RED_LED,LOW); // All LEDs are turned off
  digitalWrite(GREEN_LED,LOW);
  digitalWrite(BLUE_LED,LOW);

  for(int i=0;i<charArray_length;i++){ // For loop that itterates the array
    if(charArray[i]== Red){ // If "R" red counter goes up by 1, and Red LED turns on
      RED_counter++;
      digitalWrite(RED_LED, HIGH);
      delay(500);
      digitalWrite(RED_LED,LOW);
      delay(100);
    } else if(charArray[i] == Blue){ // If "B" blue counter goes up by 1, and blue LED turns on
      BLUE_counter++;
      digitalWrite(BLUE_LED,HIGH);
      delay(500);
      digitalWrite(BLUE_LED,LOW);
      delay(100);
    } else if(charArray[i] == Green){ // If "G" green counter goes up by 1, and green LED turns on
      GREEN_counter++;
      digitalWrite(GREEN_LED,HIGH);
      delay(500);
      digitalWrite(GREEN_LED,LOW);
      delay(100);
    }
    
  }
  Serial.println("Red Counter: "); // Counters are printed
  Serial.println(RED_counter);
  Serial.println("Green Counter: ");
  Serial.println(GREEN_counter);
  Serial.println("Blue Counter: ");
  Serial.println(BLUE_counter);

}

void loop() {
  // put your main code here, to run repeatedly:

}
