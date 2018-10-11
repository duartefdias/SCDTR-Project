int LED1 = 5; //Led connected to analog pin D5
int dutyCycle = 255;
unsigned long time;
int Vi = 204;
int Vf = 255;
unsigned long t1 = 100000; // 100ms
unsigned long t2 = 300000; // 600ms


const int LDRpin = A5;
double mapfloat(double val, double in_min, double in_max, double out_min, double out_max) {
    return (val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void setup() {
  // put your setup code here, to run once:
  pinMode(LED1, OUTPUT);
  pinMode(LDRpin, INPUT);
  Serial.begin(250000);
  analogWrite(LED1, Vi);
}

void loop() {
  int LDR = analogRead(LDRpin);
  float Vout = mapfloat(LDR, 0, 1023, 0, 5);
 
  if(time > t1){
    Serial.println("Increment");
    analogWrite(LED1, Vf);
    t1 = 10000000;
  }
  
  if(time > t2){
    exit(0);
  }

  //Serial.print("Time: ");
  time = micros();
  Serial.print(time);
  Serial.print(", ");
  Serial.println(Vout);
}
