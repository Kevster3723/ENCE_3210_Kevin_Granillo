/*
*
* Kevin Granillo
* March 1, 2025
*
* This program takes an input value and compares it to a reference value
* This simulates a 14 bit SAR ADC
* When it compares the values, if the input is greater than the treshold value the bit is stored as a 1
* Otherwise the bit is stored as a 0
* It uses these bits to calculate the voltage from the binary value by dividing the max voltage by the respective power of 2
*
*
*
*/

// 14 bit SAR
#define bitsize 14
// Sets Reference voltage, input voltage, and threshold voltage
float Vref;
float Vin;
float thresh;
float quantized = 0; // Converted voltage value
int count; 
int bitval;
int bits[bitsize]; // binary voltage value
float Vmax; // Volatage used to calculate quantized value

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  Vref = 3.0; // 3 Volt Reference Source
  Vin = 2.752; // Input Voltage
  SAR14Bit(Vref,Vin);
  Serial.println(quantized,3); // Converted value
  delay(500);
}

void SAR14Bit(float Vref, float Vin)
{
  // Sets threshold and max voltage to half of reference voltage
  Vref /= 2;
  thresh = Vref;
  Vmax = Vref;
  quantized = 0; // Sets quantized value to 0
  for(count = 0; count<bitsize;count++) // Itereates bit by bit starting with LSB
  {
    Vref /= 2; // Halves reference voltage
    if(Vin >= thresh) // If input is greater than threshold, bit is a 1 
    {
      bitval = 1;
      thresh += Vref; // Makes threshold higher value
    } else // If input is less than threshold, bit is a 0
    {
      bitval = 0;
      thresh -= Vref; // Makes threshold smaller
    }
    bits[count] = bitval; // Bits are saved in array
    quantized += bits[count]*(Vmax/(pow(2,count))); // Calculates voltage based on bit value
  }
}
