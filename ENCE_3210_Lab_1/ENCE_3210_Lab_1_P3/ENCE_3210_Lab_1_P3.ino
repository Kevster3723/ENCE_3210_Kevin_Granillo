/*
 * 
 * Kevin Granillo
 * January 16, 2025
 * 
 * Searches for the maximum value in an array
 * Then searches for the second maximum value in the array
 * 
 * 
 */
int myArray[50] = {}; // Defines empty array length 50
int Max = 0; // Defines variable for max value
int Second_max = 0; // Defines variable for second max
#define Array_length 50 // Defines array length
void setup() {
  Serial.begin(9600);
  for(int i = 0; i<Array_length;i++){ // For loop that generates random array of numbers
    myArray[i] = random(0,100);
    if(Max < myArray[i]) // Compares values in the array to current maximum
      Max = myArray[i]; // If value bigger tgan max, the value becomes the new max
  }
  for(int i =0; i<Array_length;i++){ // For loop compares Second max to max and current array value
    if(myArray[i]< Max && myArray[i] > Second_max){ // Second Max has to be less than max but greater than all other elements in array
      Second_max = myArray[i]; // If array value bigger than second max, value becomes new second max
    }
  }
  Serial.println(Max); // Prints max
  Serial.println(Second_max); // Prints second max
}
  

void loop() {
  // put your main code here, to run repeatedly:

}
