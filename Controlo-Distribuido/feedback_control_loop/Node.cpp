#include "Arduino.h"
#include "Node.h"


Node::Node(int i, float ki1, float ki2, float ci, float oi, int Li){
  index = i;
  k[0] = ki1;
  k[1] = ki2;
  n = sq(k[0]) + sq(k[1]);
  m = n - sq(k[i]);
  c = ci;
  o = oi;
  L = Li;
}

Node::updateGain(float kii){
  k[index] = kii;
  n = sq(k[0]) + sq(k[1]);
  m = n - sq(k[index]);
}

Node::set(int i, float ki1, float ki2, float ci, float oi, int Li){
  index = i;
  k[0] = ki1;
  k[1] = ki2;
  n = sq(k[0]) + sq(k[1]);
  m = n - sq(k[i]);
  c = ci;
  o = oi;
  L = Li;
    
  Serial.println("SETUP MY NODE");
  Serial.print(" i=");
  Serial.print(index);
  Serial.print(" ki1=");
  Serial.print(k[0]);
  Serial.print(" ki2=");
  Serial.print(k[1]);
  Serial.print(" n=");
  Serial.print(n);
  Serial.print(" m=");
  Serial.println(m);
  Serial.print(" c=");
  Serial.println(c);
  Serial.print(" o=");
  Serial.println(o);
  Serial.print(" L=");
  Serial.println(L);
}
