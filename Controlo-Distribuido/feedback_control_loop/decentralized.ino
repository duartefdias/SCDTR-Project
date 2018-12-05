#include <Wire.h>
#include <EEPROM.h>
#include "Node.h"
#include "globals.h"

// Decentralized version of consensus algorithm
// TODO: What to do in case there is no feasible solution (too bright or too dim)


void loophole(){
  struct solution sol;
  int input;
  sendNegotiationState(Negotiation);
  //Serial.println(Negotiation);

  if (Negotiation){
    sol = consensus_algorithm();
    Serial.print("solution = ");
    Serial.print(sol.d[0]);
    Serial.print(" ");
    Serial.println(sol.d[1]);
    Serial.println();
  }

  if (Serial.available()){
    input = Serial.parseInt();
    if (input == -1) {
      Serial.println("Disable negotiations");
      Negotiation = 0;    // Empty desk
      sendNegotiationState(Negotiation);
      Serial.flush();
    }
    else if (input == 1) {
      Serial.println("Enable negotiations");
      Negotiation = 1;   // Occupied desk
      sendNegotiationState(Negotiation);
      Serial.flush(); 
    }
  }
  delay(10);
}
