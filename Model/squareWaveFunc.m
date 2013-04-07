function  x = squareWaveFunc(Xlow,Xhigh,T0,Tdelay,RisePercentage,FallPercentage,WidthPercentage,t)
    Trise  = 0.01   *  RisePercentage  * T0;
    Tfall  = 0.01   *  FallPercentage  * T0;
    Twidth = 0.01   *  WidthPercentage * T0;
    
    t = t - Tdelay + Trise / 2;
    t = t - floor(t/T0) * T0;
    
    if size(t,1) > 1
        disp('t must be rowvector or number it cant be matrix');
        return;
    end
    x = zeros(1,size(t,2));
    
    for ind=1:size(x,2)
        if t(1,ind) < Trise
            x(1,ind) = (Xhigh - Xlow) * t(1,ind)/Trise + Xlow; 
        elseif t(1,ind) < Trise + Twidth
            x(1,ind) = Xhigh;
        elseif t(1,ind) < Trise + Twidth + Tfall
            x(1,ind) = Xhigh - (Xhigh - Xlow)*(t(1,ind) - Trise - Twidth)/Tfall;
        else
            x(1,ind) = Xlow;
        end
    end
end