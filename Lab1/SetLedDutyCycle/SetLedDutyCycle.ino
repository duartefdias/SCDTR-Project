int LED1 = 5; //Led connected to analog pin D8
int dutyCycle = 255;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED1, OUTPUT);
  Serial.begin(9600);
  Serial.print("Insert a value between 0-255");
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0) {
      // read value from console
      dutyCycle = Serial.parseInt();
      Serial.print("Value received: ");
      Serial.println(dutyCycle);
  }

  analogWrite(LED1, dutyCycle);
}
