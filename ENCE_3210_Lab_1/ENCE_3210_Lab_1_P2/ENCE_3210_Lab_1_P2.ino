/*
 * 
 * Kevin Granillo
 * January 16, 2025
 * 
 * Finds the cosine of integer degrees between 1 and 360
 * Results are saved in an array
 * 
 * 
 */

#include <math.h> // Includes math library for cos function

double x[361] = {}; // Array of results

void setup() {
  Serial.begin(9600);
  for(int i = 0; i<361;i++){ // For loop adds results to array
    x[i] = cos(i*M_PI/180); // Converts degrees to radians
    Serial.println(x[i]); // Prints array result
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
