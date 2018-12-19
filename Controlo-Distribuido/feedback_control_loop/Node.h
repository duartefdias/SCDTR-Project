#ifndef Node_h
#define Node_h
#define MAX_NODES 10

class Node{
  private:
   
  public:
   Node(int i, float ki[], float ci, float oi, int Li, int num_Nodes);
   set(int i, float ki[], float ci, float oi, int Li, int num_Nodes);
   updateGain(float kii);
   int index;
   float d[MAX_NODES] = {0};
   float d_av[MAX_NODES] = {0};
   float y[MAX_NODES] = {0};
   float k[MAX_NODES] = {0};
   float n;
   float m;
   float c;
   float o;
   int L;
   int N;
};

#endif
