#ifndef globals_h
#define globals_h

struct solution {
  float d[2];
  float cost;
};

uint8_t own_addr = EEPROM.read(0);

float rho = 0.07;

bool consensus = false;
bool Negotiation = false;
bool ReceivedSolution = false;  // flag
int occupancy = 1;

struct solution other_solution;
  
Node my_node(-1,-1,-1,-1,-1,-1);

#endif
