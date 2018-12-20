#include "Node.h"
#define MAX 10

// TODO:
//  What to do in case there is no feasible solution (too bright or too dim)

float rho = 0.07;

// Compute optimal solution with consensus algorithm
struct solution consensus_algorithm(){
  int i, l=0;
  struct solution sol;  
  
  // Reset node values
  set_array(my_node.d, 0, my_node.N);        
  set_array(my_node.d_av, 0, my_node.N); 
  set_array(my_node.y, 0, my_node.N);
  
  Serial.print("BEGINNING CONSENSUS. My node:");
  Serial.print(" i=");  Serial.print(my_node.index);
  Serial.print(" k"); Serial.print(my_node.index+1); Serial.print("1=");  Serial.print(my_node.k[0]);
  Serial.print(" k"); Serial.print(my_node.index+1); Serial.print("2=");  Serial.print(my_node.k[1]);
  Serial.print(" n=");  Serial.print(my_node.n);
  Serial.print(" m=");  Serial.print(my_node.m);
  Serial.print(" c=");  Serial.print(my_node.c);
  Serial.print(" o=");  Serial.print(my_node.o);
  Serial.print(" N=");  Serial.print(my_node.N);
  Serial.print(" L=");  Serial.println(my_node.L);
  
  for (i=0; i <= 50 && Negotiation; i++)
  {
    // Compute own solution and send
    sol = primal_solve(my_node,rho);
    copy_array(sol.d, my_node.d, my_node.N);
    Serial.print("SEND: "); Serial.print(my_node.d[0]); Serial.print(" "); Serial.println(my_node.d[1]);
    sendNegotiation(my_node.d, my_node.N);

    // Wait to receive solution computed from other nodes ...
    while (!ReceivedSolution && Negotiation) {
      delay(2);
      l++;
      if (l > 50) {
        Negotiation = 0;
        break;
      }
    }
    ReceivedSolution = false;      

    // Compute averages using the received information
    for (int j=0; j<my_node.N; j++){
      my_node.d_av[j] = (my_node.d[j]+other_solution.d[j])/2;
    }
    
    // Update local lagrangians
    for (int j=0; j<my_node.N; j++){
      my_node.y[j] = my_node.y[j] + rho*(my_node.d[j]-my_node.d_av[j]);
    }
    
    if (max_abs_diff(my_node.d, other_solution.d, my_node.N) < 0.005 || !Negotiation){
      Negotiation = 0;
      break;
    }
  }
  
  // Consensus was obtained!
  Negotiation = 0;
  sendNegotiationState(Negotiation);
  sendNegotiationState(Negotiation);
  Serial.print("Consensus at iteration ");  Serial.println(i);
  copy_array(my_node.d, sol.d, my_node.N);  
  return sol;
}

struct solution primal_solve(Node node, float rho){
  // Initialize variables
  struct solution sol;
  float d_best[MAX], d[MAX], d_u[MAX], d_b1[MAX], d_b0[MAX], d_10[MAX], d_11[MAX];
  set_array(d_best,0,MAX); set_array(d,0,MAX); set_array(d_u,0,MAX);
  set_array(d_b1,0,MAX); set_array(d_b0,0,MAX); set_array(d_10,0,MAX); set_array(d_11,0,MAX);
    bool sol_unconstrained=0, sol_boundary_linear=0, sol_boundary_0=0;
  bool sol_boundary_5=0, sol_linear_0=0, sol_linear_5=0;    
  float cost_best=1000000, cost_unconstrained=1000000, cost_boundary_linear=1000000;
  float cost_boundary_0=1000000, cost_boundary_5=1000000, cost_linear_0=1000000, cost_linear_5=1000000;
  float z[MAX]; set_array(z,0,MAX);

  // Set z auxiliary variable
  for (int j=0; j<node.N; j++){
    z[j] = rho*node.d_av[j] - node.y[j];
  }
  z[node.index] = z[node.index] - node.c;

  // Unconstrained Minimum - S0
  for(int j=0; j<node.N; j++)
    d_u[j] = (1/rho)*z[j];
  sol_unconstrained = check_feasibility(node,d_u);
  if (sol_unconstrained){
    cost_unconstrained = evaluate_cost(node, d_u, rho);
    if (cost_unconstrained < cost_best) {
      copy_array(d_u, sol.d, node.N);
      sol.cost = cost_unconstrained;
      return sol;  // If solution exists, then it is optimal, can return now   
    }
  }

  // Compute minimum constrained by linear boundary - S1
  for(int j=0; j<node.N; j++)
    d_b1[j] = z[j]/rho - (node.k[j]/node.n) * (node.o-node.L+(1/rho)*(dot_product(node.k, z, node.N)));
  sol_boundary_linear = check_feasibility(node, d_b1);
  if (sol_boundary_linear){
    cost_boundary_linear = evaluate_cost(node, d_b1, rho); 
    if (cost_boundary_linear < cost_best) {
      copy_array(d_b1, d_best, node.N); 
      cost_best = cost_boundary_linear;
    }
  }

  // Compute minimum constrained by 0 boundary - S2
  for(int j=0; j<node.N; j++)
    d_b0[j] = (1/rho)*z[j];
  d_b0[node.index] = 0;
  sol_boundary_0 = check_feasibility(node,d_b0);
  if (sol_boundary_0){
    cost_boundary_0 = evaluate_cost(node,d_b0,rho);
    if (cost_boundary_0 < cost_best){
      copy_array(d_b0, d_best, node.N); 
      cost_best = cost_boundary_0;
    }
  }

  // Compute minimum constrained by 5 boundary - S3
  for(int j=0; j<node.N; j++)
    d_b1[j] = (1/rho)*z[j];
  d_b1[node.index] = 5;
  sol_boundary_5 = check_feasibility(node, d_b1);  
  if (sol_boundary_5){
    cost_boundary_5 = evaluate_cost(node, d_b1,rho);
    if (cost_boundary_5 < cost_best){
      copy_array(d_b1, d_best, node.N);
      cost_best = cost_boundary_5;
    }
  }

  // Compute minimum constrained to linear and 0 boundary - S4
  for(int j=0; j<node.N; j++)
    d_10[j] = (1/rho)*z[j] - 
              (1/node.m)*node.k[j]*(node.o-node.L) +
              (1/rho/node.m)*node.k[j]*(node.k[node.index]*z[node.index]-dot_product(z, node.k, node.N));
  d_10[node.index] = 0;
  sol_linear_0 = check_feasibility(node,d_10);
  if (sol_linear_0){
    cost_linear_0 = evaluate_cost(node, d_10,rho);
    if (cost_linear_0 < cost_best){
      copy_array(d_10, d_best, node.N);
      cost_best = cost_linear_0;
    }
  }

  // Compute minimum constrained to linear and 5 boundary - S5
  for(int j=0; j<node.N; j++)
    d_11[j] = (1/rho)*z[j] - 
              (1/node.m)*node.k[j]*(node.o-node.L + 5*node.k[node.index]) +
              (1/rho/node.m)*node.k[j]*(node.k[node.index]*z[node.index]-dot_product(z, node.k, node.N)); 
  d_11[node.index] = 5;
  sol_linear_5 = check_feasibility(node, d_11);
  if (sol_linear_5){
    cost_linear_5 = evaluate_cost(node, d_11, rho);
    if (cost_linear_5 < cost_best){
      copy_array(d_11, d_best, node.N); 
      cost_best = cost_linear_5;
    }
  }

  // Return solution
  copy_array(d_best, sol.d, node.N);
  sol.cost = cost_best;  
  return sol;
}



// Checks feasibility of a given solution d for a given Node
bool check_feasibility(Node node, float d[]){
  float tol = 0.001;
  if (d[node.index] < 0 - tol){   // below allowed dimming level
    return false;
  }
  if (d[node.index] > 5 + tol){   // above allowed dimming level
    return false;
  }
  if (dot_product(d, node.k, node.N)  < node.L - node.o - tol){     // below lighting constraints 
    return false;
  }
  return true;
}

// Evaluate the cost of a given solution d to Node n
float evaluate_cost (Node n, float d[], float rho){
  float r2 = 0; 
  float r3 = 0;
  for (int j=0; j<n.N; j++){
    r2 += n.y[j]*(d[j]-n.d_av[j]);
    r3 += pow(d[j]-n.d_av[j],2);
  }  
  return n.c*d[n.index] + r2 + rho/2 * r3;
}


// Computes the dot product between 2 arrays of floats
float dot_product(float v[], float u[], int N){
  float result = 0;
  for (int i = 0; i < N; i++)
      result += v[i]*u[i];
  return result;  
}

// sets all elements of array v[] to value val
void set_array(float v[], float val, int N) {
  for (int j = 0; j<N; j++){
    v[j] = val;
  }
}

// Computes the max absolute difference between the elements with the same index of arrays u[], v[]
float max_abs_diff(float u[], float v[], int N){
  float max_diff = 0;
  float diff = 0;
  for (int j=0; j<N; j++){
    diff = abs(u[j]-v[j]);
    if (diff > max_diff) max_diff = diff;
  }  
  return diff;
}

// copy array of floats from v to u
float copy_array(float v[], float u[], int N) {
  for (int j=0; j < N; j++){
    u[j] = v[j];
  }
}
