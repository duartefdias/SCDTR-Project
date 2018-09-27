void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int LDR = analogRead(A5);
  int Vout = (LDR * 0.0048828125);
  int RLDR = (10000.0 * (5 - Vout))/Vout;
  int LuxValue = 500.0 / RLDR;
  Serial.println(LuxValue);
  delay(500);
}
