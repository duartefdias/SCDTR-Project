
function cost = evaluate_cost(node,d,rho)
    cost =  node.c*d(node.index) + node.y'*(d-node.d_av) + ...
            rho/2*norm(d-node.d_av)^2;
end