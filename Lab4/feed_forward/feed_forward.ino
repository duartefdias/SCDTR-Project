// Pins
const int led = 5;
const int ldr = A5;

// ldr1 parameters
const float C = 25.8439;
const float m = -0.4934;
// ldr2 parameters
const float C2 = 25.8439;
const float m2 = -0.4934;

// Variables
int ldrReadValue = 0;
int desiredLuxValue = 0;
int measuredLux = 0;
int Ui = 0;

float mapfloat(double val, double in_min, double in_max, double out_min, double out_max) {
    return (val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void setup() {
  pinMode(led, OUTPUT);
  pinMode(ldr, INPUT);
  Serial.begin(9600);
  
  Serial.print("Insert desired lux value [0, 600]: ");
  while (desiredLuxValue == 0){
    if (Serial.available() > 0) {
      desiredLuxValue = Serial.parseInt();
    }
  }
}

// Reads illuminance value from sensor in Lux
float readLDR() {
  // Read LDR voltage
  int LDR = analogRead(A5);
  float Vi = mapfloat(LDR, 0, 1023, 0, 5);

  // Convert voltage into Lux
  float base = (Vcc/Vi-1);
  float exponent = 1/m; 
  
  float LuxValue = C*pow(base, exponent);

  return LuxValue; 
}

void loop() {

  // Calculate Ui value

  // Set led value with Ui
  analogWrite(led, Ui);

  // Read ldr lux value
  measuredLux = readLDR();

  // Print measured lux value from ldr
  Serial.print("Measured lux: ");
  Serial.println(measuredLux);

  delay(500);
  
}
