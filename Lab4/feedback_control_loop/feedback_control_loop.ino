#include <avr/io.h>
#include <avr/interrupt.h>

// Constants
const int LED1 = 5; // LED connected to analog pin5
const int LDRpin = A5; // LDR connected to A5 pin
float refValue = 0; // Desired illuminance in Lux
const int Vcc = 5;

/// LDR1 parameters
const float C = 25.8439;
const float m = -0.4934;

/// System parameters: Gain and Time constant function of illuminance x [ARDUINO 1]
// G0(x) = mG1*x + bG1
// Tau(x) = mtau1*x + btau1
const float mG1 = 0.1001;
const float bG1 = 44.9773;
const double mtau1 = -0.00001449;
const double btau1 = 0.0119;
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


// PI Controller parameters
const float Kp = 3;
const float Ki = 12;


// Sampling Period
const double T = 0.005;

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
  int LDR = analogRead(A5);
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

/// Computes the predicted output according to step response of system model G(s) in Volt
//  G(s) = G0(x) / 1+Tau(x)*s
//  v_des(t) = vf - (vf-vi) * e^-(t-ti)/Tau(xf)
float calculateDesiredY(float xf) {
  float Vf = lux2volt(xf);
  float Vi = lux2volt(xi);
  double taux = tau(xf);
  float t = micros();
  
  // The time constant Tau is in seconds therefore time must be converted from microseconds to seconds!!! 
  return Vf - (Vf - Vi) * exp(-(t-ti)*0.000001/taux);     
}

// Feed Forward Controller, returns the output voltage to obtain refValue illuminance
float FeedForwardController(float refValue) {
  // Calculate Ui value
  float Kff = 1 / (mG1 * refValue + bG1);
  return Kff * refValue;
}

// Computes the controller value for the feedback system
// Sums the Feed Forward controller with the PI Controller
float Controller(float ref, float measuredY) {

  // Calculate Feed Forward controller input tension
  float Uff = FeedForwardController(ref);
  //Serial.print("Uff: ");
  //Serial.println(Uff);

  // Compute error
  //float Upi = PIcontroller(calculateDesiredY(ref), lux2volt(measuredY));
  float Upi =  PIcontroller(lux2volt(ref), lux2volt(measuredY));

  //Serial.print("Upi: ");
  //Serial.println(Upi);

  // Compute PI controller input tension
  float u = Upi + Uff;
  //Serial.print("Applied u: ");
  //Serial.println(u);
  if(u > 5) {u = 5;}
  if(u < 0) {u = 0;}
  return u;
}

// PI Controller, returns the output voltage to obtain ref=y both are in Volt
float PIcontroller(float ref, float y) {
  float K1 = Kp * G0(ref);
  float K2 = Kp * Ki * (T / 2);
  
  float e = ref - y;
  float p = Kp*e;
  
  float i = i_ant + K2 * (e + e_ant);
  // Limit integral term to avoid windup
  if (i < -10) { i = -10;}
  if (i > 10)  { i = 10;}
  float u = p + i;
  y_ant = y;
  i_ant = i;
  e_ant = e;

  return u;
}


void setup() {
  // Setup pinmodes
  pinMode(LED1, OUTPUT);
  pinMode(LDRpin, INPUT);
  Serial.begin(250000);

  // Setup Timer interrupt (200 Hz)
  cli(); // stop interrupts
  TCCR1A = 0; // set entire TCCR1A register to 0
  TCCR1B = 0; // same for TCCR1B
  TCNT1  = 0; // initialize counter value to 0
  // set compare match register for 200 Hz increments
  OCR1A = 9999; // = 16000000 / (8 * 200) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS12, CS11 and CS10 bits for 8 prescaler
  TCCR1B |= (0 << CS12) | (1 << CS11) | (0 << CS10);
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  sei(); // allow interrupts

  // Reference value
  Serial.print("Insert desired Lux value [0, 300]: ");
  while (refValue == 0) {
    if (Serial.available() > 0) {
      refValue = Serial.parseInt();
    }
  }
}

void loop() {
  if (flag)
  {
    // Read current output - y
    float measuredY = readLDR();

    // Apply control law
    float u = Controller(refValue, measuredY);
        
    u = mapfloat(u, 0, 5, 0, 255);
    analogWrite(LED1, u);
    
    // Print measurement
    Serial.print("Measured illuminance: ");
    Serial.println(measuredY);
    flag = 0;
  }
}
