const int LDRpin = A5;

double mapfloat(double val, double in_min, double in_max, double out_min, double out_max) {
    return (val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void setup() {
  
  Serial.begin(9600);
  pinMode(LDRpin, INPUT);
}

void loop() {
  
  int LDR = analogRead(LDRpin);
  float Vout = mapfloat(LDR, 0, 1023, 0, 5);
  int RLDR = 10000*(5-Vout)/Vout;
  
  Serial.print("Resistência do LDR medida: ");
  Serial.println(RLDR);
  delay(500);
}
