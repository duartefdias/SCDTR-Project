#include "Arduino.h"
#include "Node.h"


Node::Node(int i, float ki[], float ci, float oi, int Li, int num_Nodes){
  index = i;
  for(int j=0; j<num_Nodes; j++)
    k[j] = ki[j];
  n = 0;
  N = num_Nodes;
  for (int j=0; j < N; j++){
    n = n + sq(k[j]);
  }
  m = n - sq(k[i]);
  c = ci;
  o = oi;
  L = Li;
}

Node::updateGain(float kii){
  k[index] = kii;
  n = 0;
  for (int j=0; j < N; j++){
    n = n + sq(k[j]);
  }
  m = n - sq(k[index]);
}

Node::set(int i, float ki[], float ci, float oi, int Li, int num_Nodes){
  index = i;
  N = num_Nodes;
  for (int j=0; j<N; j++){
    k[j] = ki[j];
  }
  n = 0;
  for (int j=0; j < N; j++){
    n = n + sq(k[j]);
  }
  m = n - sq(k[i]);
  c = ci;
  o = oi;
  L = Li;
    
  Serial.println("SETUP MY NODE");
  Serial.print(" i=");  Serial.print(index);
  Serial.print(" ki1=");  Serial.print(k[0]);
  Serial.print(" ki2=");  Serial.print(k[1]);
  Serial.print(" n=");  Serial.print(n);
  Serial.print(" m=");  Serial.print(m);
  Serial.print(" c=");  Serial.print(c);
  Serial.print(" o=");  Serial.print(o);
  Serial.print(" N=");  Serial.print(N);
  Serial.print(" L=");  Serial.println(L);
}
