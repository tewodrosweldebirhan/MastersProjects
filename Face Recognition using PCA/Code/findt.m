function [P]=findt(F,J)
A=ones(5,1);
F=([F A]);
J=([J A]);
X=pinv(F)*J;
P=F*X;
P=P(:,1:2);
