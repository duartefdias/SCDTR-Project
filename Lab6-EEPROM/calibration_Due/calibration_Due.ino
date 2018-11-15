#include <avr/io.h>
#include <avr/interrupt.h>
#include "constants.h"

// Global variables
int address = 2; // Uno or Due
float refValue = 0; // Desired illuminance in Lux
int input = 0;

// Returns system gain for desired illuminance x
float G0(float x) {
  return mG1*x + bG1;
}
// Returns the time constant for desired illuminance x
double tau(float x) {
  return mtau1*x + btau1;
}

/// Tensão e tempo inicial (antes do escalão)
float xi = 0;
int ti = 0;
int t_ans=0;
int t_dps=0;
int increment = 0;

/////////////////////////// PI Controller
// PI controller memory
float y_ant = 0;
float i_ant = 0;
float e_ant = 0;


// Setup Interrupt
volatile bool flag;
ISR(TIMER1_COMPA_vect) {
  flag = 1; //notify main loop
}

// Equivalent of map function for floats
float mapfloat(double val, double in_min, double in_max, double out_min, double out_max) {
  return (val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// Reads illuminance value from sensor, returns value in Lux
float readLDR() {
  // Read LDR voltage
  int LDR = analogRead(A0);
  float Vi = mapfloat(LDR, 0, 1023, 0, 5);

  // Convert voltage into Lux
  float base = (Vcc / Vi - 1);
  float exponent = 1 / m;
  float LuxValue = C * pow(base, exponent);
  return LuxValue;
}

// Converts value in Lux to Volt
float lux2volt(float lux) {
  float base = lux/C;
  float exponent = m;
  return Vcc/(pow(base, exponent) + 1);
}

// Measures ambient noise and influence of each node on another
void calibrateSystem() {  
  float o, lum, k12, k21;
  
  Serial.println("Press ENTER to start calibration routine.");
  while(!Serial.available()) {}
  
  // Ambient noise
  Serial.println("Measuring ambient noise...");
  analogWrite(LED1, 0);
  o = readLDR();
  Serial.print("Ambient noise: ");
  Serial.println(o);  
  delay(3000);

  // Measure k12
  Serial.println("Measuring 1's influence on 2...");
  if (address==1) {
    analogWrite(LED1, 255);
    delay(5000);
    analogWrite(LED1, 0);
  }
  else if (address==2) {
    delay(4000);
    lum = readLDR();
    k12 = lum/5;
    Serial.print("k12 = ");
    Serial.println(k12);    
    delay(1000);
  }

  // Measure k21
    Serial.println("Measuring 2's influence on 1");
  if (address==2) {
    analogWrite(LED1, 255);
    delay(5000);
    analogWrite(LED1, 0);
  }
  else if (address==1) {
    delay(4000);
    lum = readLDR();
    k21 = lum/5;
    Serial.print("k21 = ");
    Serial.println(k21);    
    delay(1000);
  }     
}

void setup() {
  // Setup pinmodes
  pinMode(LED1, OUTPUT);
  pinMode(LDRpin, INPUT);
  Serial.begin(250000);

  // Reference value
  calibrateSystem();
}

void loop() {

  if (flag)
  {
    // Read current output - y
    float measuredY = readLDR();
    
    // Print measurement
    Serial.print("Measured illuminance: ");
    Serial.println(measuredY);
    flag = 0;
  }

  delay(1000);
}
