function [P]=transform(F,J)
A=ones(5,1);
F=([F A]);
J=([J A]);
P=pinv(F)*J;
P=P(:,1:2);
A=[0;0;1];
P=[P A];
