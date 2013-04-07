f = 1 : 500;
R = 1e3;
C = 1e-6;
fc = 1/2/pi/R/C;
H = (1./(2*pi*j*f*C))./((1./(2*pi*j*f*C))+R);
Habs = abs(H);

figure(1);
plot(f,Habs);
line([fc fc],[0 max(Habs)]);
line([fc/5 fc/5],[0 max(Habs)]);
Habs(round(fc/5))*100