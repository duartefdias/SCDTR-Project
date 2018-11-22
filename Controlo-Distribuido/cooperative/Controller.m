function [ u ] = Controller( ref, measurement, mG, bG )

    uff = FeedForwardController(ref, G0(mG, bG, ref))
    upi = PIController(ref, measurement)
    u = uff + upi;    
end

