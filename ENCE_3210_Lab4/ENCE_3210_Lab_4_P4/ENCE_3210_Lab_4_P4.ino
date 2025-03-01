/*
*
* Kevin Granillo
* March 1, 2025
*
* This program is used to control fan speed based off the current temperature
* It uses a potentiometer as an analog temp sensor which is read and ran through a 14 bit SAR
* The average temperature over a period of 5 seconds is taken 
* Three different Sensitivitie levels are also offered and can be changed with a button
* An LED is used to simulate the fan speed
*
*
*/

#define TEMP_SENSOR A0 // Potentiometer acting as temp sensor

// Buttons for onoff and Speed
#define BUTTON_ONOFF 2
#define BUTTON_SENSITIVITY 3

#define LED 9 // LED posing as fan 

#define TIMER_COMPARE_VALUE 78 // 5ms

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
char gFlag_ONOFF = 0;
int gSamples = 0;
float gAvg_Temp = 0;
float Temp = 0;
float Temp_Read;

enum State_e{SPEED_1,SPEED_2,SPEED_3}; // State Machine for different Speeds
enum State_e gState = SPEED_1; // Slow Speed as Initial

void setup() {
  Serial.begin(9600); // Begin Serial Communication
  // Initializing sensors, buttons, and LED
  pinMode(TEMP_SENSOR,INPUT);
  pinMode(BUTTON_ONOFF,INPUT);
  pinMode(BUTTON_SENSITIVITY,INPUT);
  pinMode(LED,OUTPUT);

  attachInterrupt(digitalPinToInterrupt(BUTTON_ONOFF), isr_ONOFF, CHANGE); // Interrupt for on off button
  attachInterrupt(digitalPinToInterrupt(BUTTON_SENSITIVITY), isr_SENSITIVITY, CHANGE); // interrupt for Speed button

  noInterrupts();

  // Timer 1
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;

  TCCR1A |= (1<<WGM11); // Wave Generation Mode
  TCCR1B |= (1<<WGM12) | (1<<WGM12); // Fast PWM
  TCCR1B |= (1<<CS10); // No prescale

  TCCR1A |= (1<<COM1A1) | (1<<COM1B1); //Clear OC1A/OC1B on compare match, set OC1A/OC1B at BOTTOM (non-inverting mode)

  // Timer 2
  TCCR2A = 0;
  TCCR2B = 0;

  OCR2A = TIMER_COMPARE_VALUE;

  TCCR2A |= (1<<WGM21); // Wave Generation Mode
  TCCR2B |= (1<<CS22) | (1 << CS21) | (1<<CS20); // External Clock Source 

  TIMSK2 |= (1<<OCIE2A); // Attach Interrupt
  interrupts();
}

void loop() {
  if(gFlag_ONOFF == 1) // If Fan is on turn on LED 
  {
    switch(gState)
    {
      case SPEED_1:
      analogWrite(LED,gAvg_Temp*0.33); // 33% of max speed
      break;

      case SPEED_2:
      analogWrite(LED,gAvg_Temp*0.66); // 66% of max speed
      break;

      case SPEED_3:
      analogWrite(LED,gAvg_Temp); // Max Speed
      break;
    }
  } else 
  {
    analogWrite(LED,0); // If off, turn LED off
  }

}

float SAR14Bit(float Vref, float Vin) // 14 bit SAR
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
  return quantized; // Return voltage value
}

void isr_ONOFF() 
{
  gFlag_ONOFF = !gFlag_ONOFF; // Turns fan on or off
}

void isr_SENSITIVITY() // Switches state when triggered
{
  if(gState == SPEED_1 && gFlag_ONOFF == 1)
  {
    gState = SPEED_2;
  } else if(gState == SPEED_2 && gFlag_ONOFF == 1)
  {
    gState = SPEED_3;
  } else if(gState == SPEED_3 && gFlag_ONOFF == 1)
  {
    gState = SPEED_1;
  }
}

ISR(TIMER2_COMPA_vect)
{
  gSamples++; // Used to wait 5 seconds

  Temp_Read = analogRead(TEMP_SENSOR)/300;

  Temp = SAR14Bit(Temp_Read,3.0); // Reads temp sensor and puts it through SAR
  gAvg_Temp += Temp; // Temperature added each sample

  if(gSamples == 1000) // 5ms * 1000 = 5s
  {
    gAvg_Temp /= 1000; // Average temp over 1000 samples
    gSamples = 0; // Resets Counter
    gAvg_Temp = 0; // Resets Avg temp value
  }
}