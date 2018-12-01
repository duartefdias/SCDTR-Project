#include "Node.h"

// TODO: test for different requirements and check feasibility
//  What to do in case there is no feasible solution (too bright or too dim)
// Implement distributed version of consensus using I2C communication

struct solution consensus_algorithm(){
  int i;
  struct solution sol;
  
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
  }
  // Consensus was obtained!
  Negotiation = 0;
  sendNegotiationState(Negotiation);
  Serial.print("Consensus at iteration ");
  Serial.println(i);
  sol.d[0] = my_node.d_av[0]; sol.d[1] = my_node.d_av[1];

  // Reset node values
  my_node.d[0] = 0; my_node.d[1] = 0;    
  my_node.d_av[0] = 0; my_node.d_av[1] = 0;
  my_node.y[0] = 0; my_node.y[1] = 0;
  
  return sol;
}

struct solution primal_solve(Node node, float rho){
  struct solution sol;
  float d_best[]={-1,-1}, d[]={-1,-1}, d_u[]={-1,-1}, d_b1[]={-1,-1};
  float d_b0[]={-1,-1}, d_10[]={-1,-1}, d_11[]={-1,-1};
  bool sol_unconstrained=0, sol_boundary_linear=0, sol_boundary_0=0;
  bool sol_boundary_5=0, sol_linear_0=0, sol_linear_5=0;
    
  float cost_best=1000000, cost_unconstrained=1000000, cost_boundary_linear=1000000;
  float cost_boundary_0=1000000, cost_boundary_5=1000000, cost_linear_0=1000000, cost_linear_5=1000000;
  float z[] = {0,0};
  z[0] = rho*node.d_av[0] - node.y[0];
  z[1] = rho*node.d_av[1] - node.y[1];
  z[node.index] = z[node.index] - node.c;

  // Unconstrained Minimum - S0
  d_u[0] = (1/rho)*z[0];
  d_u[1] = (1/rho)*z[1];
  sol_unconstrained = check_feasibility(node,d_u);
  if (sol_unconstrained){
    cost_unconstrained = evaluate_cost(node, d_u, rho);
    if (cost_unconstrained < cost_best) {
      sol.d[0] = d_u[0]; sol.d[1] = d_u[1];
      sol.cost = cost_unconstrained;
      return sol;  // If solution exists, then it is optimal, can return now   
    }
  }

  // Compute minimum constrained by linear boundary - S1
  d_b1[0] = z[0]/rho - (node.k[0]/node.n) * (node.o-node.L+(1/rho)*(node.k[0]*z[0]+node.k[1]*z[1]));
  d_b1[1] = z[1]/rho - (node.k[1]/node.n) * (node.o-node.L+(1/rho)*(node.k[0]*z[0]+node.k[1]*z[1]));
  sol_boundary_linear = check_feasibility(node, d_b1);
  if (sol_boundary_linear){
    cost_boundary_linear = evaluate_cost(node, d_b1, rho); 
    if (cost_boundary_linear < cost_best) {
      d_best[0] = d_b1[0]; d_best[1] = d_b1[1];
      cost_best = cost_boundary_linear;
    }
  }

  // Compute minimum constrained by 0 boundary - S2
  d_b0[0] = (1/rho)*z[0]; d_b0[1] = (1/rho)*z[1];
  d_b0[node.index] = 0;
  sol_boundary_0 = check_feasibility(node,d_b0);
  if (sol_boundary_0){
    cost_boundary_0 = evaluate_cost(node,d_b0,rho);
    if (cost_boundary_0 < cost_best){
      d_best[0] = d_b0[0]; d_best[1] = d_b0[1];
      cost_best = cost_boundary_0;
    }
  }

  // Compute minimum constrained by 5 boundary - S3
  d_b1[0] = (1/rho)*z[0]; d_b1[1] = (1/rho)*z[1];
  d_b1[node.index] = 5;
  sol_boundary_5 = check_feasibility(node, d_b1);  
  if (sol_boundary_5){
    cost_boundary_5 = evaluate_cost(node, d_b1,rho);
    if (cost_boundary_5 < cost_best){
      d_best[0] = d_b1[0]; d_best[1] = d_b1[1];
      cost_best = cost_boundary_5;
    }
  }

  // Compute minimum constrained to linear and 0 boundary - S4
  d_10[0] = (1/rho)*z[0] - 
            (1/node.m)*node.k[0]*(node.o-node.L) +
            (1/rho/node.m)*node.k[0]*(node.k[node.index]*z[node.index]-(z[0]*node.k[0]+z[1]*node.k[1]));
  d_10[1] = (1/rho)*z[1] - 
            (1/node.m)*node.k[1]*(node.o-node.L) +
            (1/rho/node.m)*node.k[1]*(node.k[node.index]*z[node.index]-(z[0]*node.k[0]+z[1]*node.k[1]));
  d_10[node.index] = 0;
  sol_linear_0 = check_feasibility(node,d_10);
  if (sol_linear_0){
    cost_linear_0 = evaluate_cost(node, d_10,rho);
    if (cost_linear_0 < cost_best){
      d_best[0] = d_10[0]; d_best[1] = d_10[1];
      cost_best = cost_linear_0;
    }
  }

  // Compute minimum constrained to linear and 5 boundary - S5
  d_11[0] = (1/rho)*z[0] - 
            (1/node.m)*node.k[0]*(node.o-node.L + 5*node.k[node.index]) +
            (1/rho/node.m)*node.k[0]*(node.k[node.index]*z[node.index]-(z[0]*node.k[0]+z[1]*node.k[1]));
  d_11[1] = (1/rho)*z[1] - 
            (1/node.m)*node.k[1]*(node.o-node.L + 5*node.k[node.index]) +
            (1/rho/node.m)*node.k[1]*(node.k[node.index]*z[node.index]-(z[0]*node.k[0]+z[1]*node.k[1]));
  d_11[node.index] = 5;
  sol_linear_5 = check_feasibility(node, d_11);
  if (sol_linear_5){
    cost_linear_5 = evaluate_cost(node, d_11, rho);
    if (cost_linear_5 < cost_best){
      d_best[0] = d_11[0]; d_best[1] = d_11[1];
      cost_best = cost_linear_5;
    }
  }

  // Return solution
  sol.d[0] = d_best[0]; sol.d[1] = d_best[1];
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
  if (d[0]*node.k[0]+d[1]*node.k[1]  < node.L - node.o - tol){     // below lighting constraints
    return false;
  }
  return true;
}

// Evaluate the cost of a given solution d to Node n
float evaluate_cost (Node n, float d[], float rho){
  return n.c*d[n.index] + n.y[0]*(d[0]-n.d_av[0]) + n.y[1]*(d[1]-n.d_av[1]) + rho/2 * (pow(d[0]-n.d_av[0],2) + pow(d[1]-n.d_av[1],2));
}
