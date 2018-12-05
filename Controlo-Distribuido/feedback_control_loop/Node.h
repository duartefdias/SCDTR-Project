#ifndef Node_h
#define Node_h

class Node{
  private:
   
  public:
   Node(int i, float ki1, float ki2, float ci, float oi, int Li);
   set(int i, float ki1, float ki2, float ci, float oi, int Li);
   updateGain(float kii);
   int index;
   float d[2] = {0,0};
   float d_av[2] = {0,0};
   float y[2] = {0,0};
   float k[2];
   float n;
   float m;
   float c;
   float o;
   int L;
};

#endif
