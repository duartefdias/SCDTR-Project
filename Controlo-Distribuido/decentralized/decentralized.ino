#include <Wire.h>
#include <EEPROM.h>
#include "Node.h"
#include "globals.h"

// Decentralized version of consensus algorithm
//  What to do in case there is no feasible solution (too bright or too dim)
int L1 = 200, L2 = 100;  // Illuminance requirements
int c1 = 1, c2 = 2;
float o1 = 0.12, o2 = 0.08; // Ambient noise
float k11 = 70, k12 = 2.87, k21 = 2.80, k22 = 30; // Illuminance gains and cross-gains
float K[][2] = {
  {k11,k12},
  {k21,k22}
};
float previous[2] = {-1,-1}; 



void setup(){
  Serial.begin(115200);
  Wire.begin(own_addr);
  TWAR = (own_addr << 1) | 1;  // enable broadcasts to be received
  Wire.onReceive(receiveEvent);   // set up receive handler
  if (own_addr == 1){
    my_node.set(0, K[0][0], K[0][1], c1, o1, L1);
  } else if (own_addr == 2){
    my_node.set(1, K[1][0], K[1][1], c2, o2, L2);
  }
}

void loop(){
  struct solution sol;
  int i;
  sendOccupancy(occupancy);

  if (Negotiation){
    for (i=0; i <= 50; i++)
    {
      // Compute own solution and send
      sol = primal_solve(my_node,rho);
      my_node.d[0] = sol.d[0];
      my_node.d[1] = sol.d[1];
      Serial.print("SEND: ");
      Serial.print(my_node.d[0]);
      Serial.print(" "); 
      Serial.print(my_node.d[1]);
      sendNegotiation(my_node.d[0], my_node.d[1]);
  
      // Wait to receive solution computed from other nodes ...
      while (!ReceivedSolution && Negotiation) {
        delay(1);        
      }
      ReceivedSolution = false;      
  
      // Compute averages using the received information
      my_node.d_av[0] = (my_node.d[0]+other_solution.d[0])/2;
      my_node.d_av[1] = (my_node.d[1]+other_solution.d[1])/2;
      // Update local lagrangians
      my_node.y[0] = my_node.y[0] + rho*(my_node.d[0]-my_node.d_av[0]);
      my_node.y[1] = my_node.y[1] + rho*(my_node.d[1]-my_node.d_av[1]);

      if ((abs(my_node.d[0]-other_solution.d[0]) < 0.02 && abs(other_solution.d[1]-my_node.d[1]) < 0.02) || !Negotiation){
        break;
      }
      previous[0] = my_node.d[0]; previous[1] = my_node.d[1];
    }
    // Consensus was obtained!
    Negotiation = false;
    sendOccupancy(0);
    previous[0] = -1; previous[1] = -1;
    Serial.print("Consensus at iteration ");
    Serial.println(i);
    
    Serial.print("SOLUTION: ");
    Serial.print(my_node.d[0]);
    Serial.print(" ");
    Serial.println(my_node.d[1]);
    Serial.println();
    delay(5000);    
  }
    
  delay(100);
}
