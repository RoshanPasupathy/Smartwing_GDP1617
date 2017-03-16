function [b] = Quincunx(n,u,v,w,q)
%UNTITLED4 Summary of this function goes here
%   Detailed explanation goes here

v(2) = v(2)/u(2);
w(2) = w(2)/u(2);

for j = 3:1:n-1
    u(j) = u(j) - (u(j-2) * w(j-2) * w(j-2)) - (u(j-1) * v(j-1) * v(j-1));
	v(j) = (v(j) - (u(j-1) * v(j-1) * w(j-1)))/u(j);
	w(j) = w(j)/u(j);    
end
u(n) = u(n) - (u(n-2) * w(n-2) * w(n-2)) - (u(n-1) * v(n-1) * v(n-1));


%forward substitution
%good values between 2 to n. position 1 is padding
disp('Forward substitution ...');
for j = 3:1:n
    q(j) = q(j) - (v(j-1) * q(j-1)) - (w(j-2)*q(j-2));
end
for j = 2:1:n
    q(j) = q(j)/u(j);
end

disp('Backward substitution ...');
%back substitution
% At n row :  b(n) = q(n)
% remeber position 1 is padding. 
% hence b is asssinged from n to 2. b(size) and b(1) = 0
for j = n-1:-1:2
    q(j) = q(j) - (v(j) * q(j+1)) - (w(j)*q(j+2));
end
b = q;
end

