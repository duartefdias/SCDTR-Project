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

// Feed Forward Controller, returns the output voltage to obtain refValue illuminance
float FeedForwardController(float refValue) {
  // Calculate Ui value
  float Kff = 1 / (mG * refValue + bG);
  //return Kff * refValue;
  return my_node.d[my_node.index];
}

// Computes the controller value for the feedback system
// Sums the Feed Forward controller with the PI Controller
float Controller(float ref, float measuredY) {

  // Calculate Feed Forward controller input tension
  float Uff = FeedForwardController(ref);
  //Serial.print("Uff: "); Serial.println(Uff);

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

/// PREDICTOR - Computes the predicted output according to step response of system model G(s) in Volt
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
