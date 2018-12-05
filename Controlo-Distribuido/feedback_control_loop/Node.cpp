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
  if (index == 0) {
    k[0] = kii;
  }
  else if (index == 1) {
    k[1] = kii;
  }  
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
}
