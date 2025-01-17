/*
 * 
 * Kevin Granillo
 * January 16, 2025
 * 
 * This program calculates twice the square of integers between 0 to 511
 * The results are saved in an array
 * 
 * Sidenote: This program isn't meant to work but rather explore with SRAM
 * When unsigned int is used, after a certain point the results cutoff
 * When unisgned long is used, the results exceed the memory available in SRAM
 * 
 */


#define ARRAY_SIZE 512 // Array size
unsigned long result[ARRAY_SIZE] = {0}; // Array in which results are saved
unsigned long twice_square(unsigned long value) // Defines function that calculates twice the square
{
  2*value*value;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  for (int i = 0; i < ARRAY_SIZE; i++){ // Loop that adds the results to an array
    result[i] = twice_square(i);
    Serial.print(i); // prints the results
    Serial.print(":");
    Serial.println(result[i]);
  }

}

void loop() {
  // put your main code here, to run repeatedly:

}
