int myArray[50] = {};
int Secondmax[49] = {};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  for(int i = 0; i<51;i++){
    myArray[i] = random(0,100);
    Serial.println(myArray[i]);
    if(myArray[0]) < myArray[i])
      myArray[0] = i;
    }
    
  }
  
}

void loop() {
  // put your main code here, to run repeatedly:

}
