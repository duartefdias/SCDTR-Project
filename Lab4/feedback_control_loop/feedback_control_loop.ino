#include <avr/io.h>
#include <avr/interrupt.h>

const int LED1=5; // LED connected to analog pin5
const int LDRpin = A5; // LDR connected to A5 pin
float refValue = 0;
const float C = 25.8439;
const float m = -0.4934;

void setup() {
  // Setup pinmodes
  pinMode(LED1, OUTPUT);
  pinMode(LDRpin, INPUT);
  Serial.begin(250000);

  // Setup Timer interrupt
  cli();          //stop interrupts
  TCCR1A = 0;     // clear register
  TCCR1B = 0;     // clear register
  TCNT1 = 0;      //reset counter
  OCR1A = 15624;  //must be <65536  // = (16*10^6) / (1*1024) – 1
  TCCR1B |= (1 << WGM12); //CTC On
  // Set prescaler for 1024
  TCCR1B |= (1<<CS12)|(1<<CS10);
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  sei(); //allow interrupts

  // Reference value
  // Serial.print("Insert desired Lux value [0, 600]: ";
  refValue = 500;
  // refValue = Serial.parseInt();  
}

volatile bool flag;

// Interrupt
ISR(TIMER1_COMPA_vect){
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
  float LuxValue = C*(Vcc/Vi - 1)^(1/m);

  return LuxValue; 
}

float Controller(float refValue, float ) {
    
}

void loop() {
  if(flag)
  {
    // Read current output - y
    y = readLDR();

    // Apply control law 
    u = Controller(refValue, y)

    // Write output to system
    analogWrite(LED1, )
        
    flag = 0;
  }
  //other operations
}
