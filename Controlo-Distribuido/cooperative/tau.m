function [ t ] = tau(mtau, btau, illuminance)
    t = mtau*illuminance + btau;
end

