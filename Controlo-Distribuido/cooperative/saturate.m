function [ ui ] = saturate( ui )

    if ui < 0
        ui = 0;
    elseif ui > 5
        ui = 5;
    end
end

