function [ volt ] = lux2volt( lux, m, C)

    volt = 5 / ( (lux/C)^m + 1); 
end

