#include <avr/io.h>
#include <avr/interrupt.h>

void setup() {
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
}

volatile bool flag;

// Interrupt
ISR(TIMER1_COMPA_vect){
  //...
  //put here control operations
  //
  flag = 1; //notify main loop
}


void loop() {
  if(flag)
  {
    //post-interrupt operations
    flag = 0;
  }
  //other operations
}
