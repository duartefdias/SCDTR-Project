// Pins
const int led = 5;
const int ldr = A5;

// ldr1 parameters
const float C = 25.8439;
const float m = -0.4934;
const float mG1 = 0.1001;
const float bG1 = 44.977;

// Constants
const int Vcc = 5;

// Variables
int ldrReadValue = 0;
int desiredLuxValue = 0;
int measuredLux = 0;
float Ui = 0;
float K = 1;


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
  K = 1 / (mG1 * desiredLuxValue + bG1);
  Ui = K * desiredLuxValue;
  if (Ui > 5) { Ui = 5; }
  Serial.println(K);
  Serial.println(Ui);
  

  // Set led value with Ui
  Ui = mapfloat(Ui, 0, 5, 0, 255);
  Serial.println(Ui);
  analogWrite(led, Ui);

  // Read ldr lux value
  measuredLux = readLDR();

  // Print measured lux value from ldr
  Serial.print("Measured lux: ");
  Serial.println(measuredLux);

  delay(500);
  
}
