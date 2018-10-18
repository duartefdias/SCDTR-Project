#include <avr/io.h>
#include <avr/interrupt.h>

const int LED1 = 5; // LED connected to analog pin5
const int LDRpin = A5; // LDR connected to A5 pin
float refValue = 0;
const int Vcc = 5;

// ldr1 parameters
const float C = 25.8439;
const float m = -0.4934;
const float mG1 = 0.1001;
const float bG1 = 44.977;

const float mtau1 = -pow(10, -5);
const float btau1 = 0.0119;

float xi = 0;
int ti = 0;

float K = 0;

int increment = 0;

float y_ant = 0;
float i_ant = 0;
float e_ant = 0;

// Adjust this
const float Kp = 1;
const float Ki = 1;

const float T = 0.005;

void setup() {
  // Setup pinmodes
  pinMode(LED1, OUTPUT);
  pinMode(LDRpin, INPUT);
  Serial.begin(250000);

  float tglobal = micros();

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
  Serial.print("Insert desired Lux value [0, 600]: ");
  while (refValue == 0) {
    if (Serial.available() > 0) {
      refValue = Serial.parseInt();
    }
  }
}

volatile bool flag;

// Interrupt
ISR(TIMER1_COMPA_vect) {
  flag = 1; //notify main loop
}

float mapfloat(double val, double in_min, double in_max, double out_min, double out_max) {
  return (val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


// Reads illuminance value from sensor in Lux
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



float Controller(float ref, float measuredY) {

  // Calculate Feed Forward controller input tension
  float Uff = FeedForwardController(ref);
  Serial.print("Uff: ");
  Serial.println(Uff);

  // Compute error
  float Upi = PIcontroller(calculateDesiredY(ref), lux2volt(measuredY));
  Serial.print("Upi: ");
  Serial.println(Upi);

  // Compute PI controller input tension
  float u = (Upi + Uff);
  return u;
}

float lux2volt(float lux) {
  return Vcc / ( pow((lux / C), m) + 1);
}

float calculateDesiredY(float xf) {
  float Vf = lux2volt(xf);
  float Vi = lux2volt(xi);

  float tau = mtau1 * xf + btau1;
  float t = micros();

  return Vf - (Vf - Vi) * exp(-(t - ti) / tau);
}

float G0(float x) {
  return (mG1 * x) + bG1;
}

float FeedForwardController(float refValue) {
  // Calculate Ui value
  K = 1 / (mG1 * refValue + bG1);
  float Ui = K * refValue;

  return Ui;
}

float PIcontroller(float ref, float y) {
  float K1 = Kp * G0(ref);
  Serial.print("K1: ");
  Serial.println(K1);
  float K2 = Kp * Ki * (T / 2);

  float e = ref - y;
  Serial.print("Error: ");
  Serial.println(e);
  float p = (K1 * ref) - (Kp * y);
  Serial.print("p: ");
  Serial.println(p);
  float i = i_ant + K2 * (e + e_ant);
  Serial.print("i: ");
  Serial.println(i);
  float u = p + i;
  y_ant = y;
  i_ant = i;
  e_ant = e;

  return u;
}

void loop() {
  if (flag)
  {
    // Read current output - y
    float measuredY = readLDR();

    // Apply control law
    float u = Controller(refValue, measuredY);
    Serial.print("Written u: ");
    Serial.println(u);
    if(u > 5) {u = 5;}
    if(u < 0) {u = 0;}
    
    u = mapfloat(u, 0, 5, 0, 255);

    // Write output to system
    analogWrite(LED1, u);

    if (increment == 0) {
      ti = micros();
    }

    // Print measurement
    Serial.print("Measured illuminance: ");
    Serial.println(measuredY);
    Serial.println();

    flag = 0;
    increment += 1;
  }
}
