function [ upi ] = PIController( ref, y )
    global i_ant e_ant y_ant
    K2 = Kp() * Ki() * T()/2;
    
    e = ref-y;
    p = Kp()*e;
    i = i_ant + K2 * (e + e_ant);
    
    % Integrator Windup
    if i < -10
        i = -10;
    elseif i > 10
        i = 10;
    end
    
    upi = p+i;
    y_ant = y;
    i_ant = i;
    e_ant = e;
end

