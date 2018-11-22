function [ v_des ] = predictor( vi, vf, ti, t, tau)
% G(s) = G0(x) / 1+Tau(x)*s
%  v_des(t) = vf - (vf-vi) * e^-(t-ti)/Tau(xf)

    
    v_des = vf - (vf-vi) * exp(-(t-ti)/tau);
end

