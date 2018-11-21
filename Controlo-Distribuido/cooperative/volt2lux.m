function [ lux ] = volt2lux ( Vi, m, C )

    lux = C * (5/Vi-1)^(1/m);
end

