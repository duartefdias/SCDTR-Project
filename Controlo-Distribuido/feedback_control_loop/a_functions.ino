// Parses input from serial monitor
void parseInput(){
  if (Serial.peek() == 'c') {
    input = Serial.parseInt();
    Serial.print("New Cost = "); Serial.println(input);
    my_node.c = input;
    Negotiation = 1;
    sendNegotiationState(Negotiation);
    sendNegotiationState(Negotiation); 
    return;     
  } else if (Serial.peek() == 'r') {
    input = Serial.parseInt();
    Serial.print("New Reference value = "); Serial.println(input);
    HighValue = input;
    return;    
  }
  input = Serial.parseInt();
  if (input == -1 and occupancy == 1) {
    occupancy = 0;
    sendOccupancy(occupancy); // Empty desk
    my_node.L = LowValue;
    sendLuxLowerBound(my_node.L);
    my_node.updateGain(G0(my_node.L));      
    Negotiation = 1;
    sendNegotiationState(Negotiation);
    sendNegotiationState(Negotiation);
  }
  else if (input == 1 and occupancy == 0) {
    occupancy = 1;
    sendOccupancy(occupancy);
    my_node.L = HighValue;   // Occupied desk
    sendLuxLowerBound(my_node.L);
    my_node.updateGain(G0(my_node.L));      
    Negotiation = 1;
    sendNegotiationState(Negotiation);
    sendNegotiationState(Negotiation);
  }
  Serial.flush(); 
  return;
}

// Setup Interrupt
ISR(TIMER1_COMPA_vect) {
  flag = 1; //notify main loop
}

// Setup TIMER 1 Interrupt to 100 Hz sampling frequency 
void timerSetup(){ 
  cli(); // stop interrupts
  TCCR1A = 0; // set entire TCCR1A register to 0
  TCCR1B = 0; // same for TCCR1B
  TCNT1  = 0; // initialize counter value to 0
  // set compare match register for 100 Hz increments
  OCR1A = 19999; // = 16000000 / (8 * 100) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS12, CS11 and CS10 bits for 8 prescaler
  TCCR1B |= (0 << CS12) | (1 << CS11) | (0 << CS10);
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  sei(); // allow interrupts
}

// Equivalent of map function for floats
float mapfloat(double val, double in_min, double in_max, double out_min, double out_max) {
  return (val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


// Measures ambient noise and influence of each node on another
void calibrateSystem() {  
  float lum;

  if (Negotiation != 2) {
    Serial.println("Press ENTER to start calibration routine.");
    while(!Serial.available() && Negotiation != 2) {}
  }
  // Request for others to begin calibration routine
  sendNegotiationState(2);
  
  // Ambient noise
  Serial.println("Measuring ambient noise...");
  analogWrite(LED1, 0);
  noise = readLDR();
  if (debug) noise = 0.01;
  Serial.print("Ambient noise: ");
  Serial.println(noise);  
  sendLuxBackground(noise);
  if (!debug) delay(3000);

  // Measure k21
  Serial.println("Measuring 1's influence on 2 (k21) ...");
  if (own_addr==1) {
    analogWrite(LED1, 255);
    if (!debug) delay(5000);
    analogWrite(LED1, 0);
  }
  else if (own_addr==2) {
    if(!debug) delay(4000);
    lum = readLDR();
    k21 = (lum-noise)/5;
    if (debug) k21 = 2.04;
    Serial.print("k21 = ");
    Serial.print(k21);
    Serial.println(" LUX/dimming");    
    if(!debug) delay(1000);
  }

  // Measure k12
    Serial.println("Measuring 2's influence on 1 (k12) ...");
  if (own_addr==2) {
    analogWrite(LED1, 255);   // full brightness
    if (!debug) delay(5000);
    analogWrite(LED1, 0);
  }
  else if (own_addr==1) {
    if(!debug) delay(4000);
    lum = readLDR();
    k12 = (lum-noise)/5;              // dimming = 5
    if (debug) k12 = 1.98;
    Serial.print("k12 = ");
    Serial.print(k12);
    Serial.println(" LUX/dimming");       
    if(!debug) delay(1000);
  }
  Negotiation = 1;
}