void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int LDR = analogRead(A5);
  Serial.println(LDR);
  delay(500);
}
