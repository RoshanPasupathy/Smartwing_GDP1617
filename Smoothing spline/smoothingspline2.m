function [coeffs,j] = smoothingspline2( x, y, sig, lmda)
%size of xarray
sizex = size(x,1);

% find position of interest and spline through that only
maxi1  = 1;
maxy1 = y(1);

maxi2 = 2;
maxy2 = y(2);

%find max
for i = 1:sizex
    if (y(i) > maxy1)
        maxy2 = maxy1;
        maxi2 = maxi1;
        maxy1 = y(i);
        maxi1 = i;
    elseif (y(i) > maxy2)
        maxy2 = y(i);
        maxi2 = i;
    end
end       
if (maxi1 < maxi2)
    j = maxi1;
    
else
    j = maxi2;
end

n = sizex - 1;
h = zeros(n,1);
u = sym(zeros(n,1));
v = sym(zeros(n-1,1));
w = sym(zeros(n-1,1));
q = sym(zeros(sizex,1));

mu = 2 * (1 - lmda)/(3 * lmda);


h(1) = x(2) - x(1);
h(2) = x(3) - x(2);

%pad arrays
u(1) = 0;
v(1) = 0;
w(1) = 0;

%set b = qs first and last element to 0
q(1) = 0;
q(sizex) = 0;

%iterate from 2 to n-1
for i = 2:1:n-1
    h(i + 1) = x(i + 2) - x(i + 1);
    u(i) = (9 * sig(i-1)/(h(i-1) * h(i-1))) + (9 * sig(i) * (h(i) + h(i-1)) * (h(i) + h(i-1))/(h(i)*h(i-1)*h(i)*h(i-1))) + (9 * sig(i+1)/(h(i)*h(i)));
    u(i) = (mu * u(i)) + (2 * (x(i+1) - x(i-1)));
    
    v(i) = (-9 * sig(i) * (h(i) + h(i-1))/(h(i) * h(i) * h(i-1))) + (-9 * sig(i+1) * (h(i+1) + h(i))/(h(i+1) * h(i) * h(i)));
    v(i) = mu*v(i) + h(i);
    
    w(i) = mu * 9 * sig(i+1)/(h(i) * h(i+1));
    q(i) = (3 * (y(i+1) - y(i))/h(i)) - (3 * (y(i) - y(i-1))/h(i-1));
end
u(n) = (9 * sig(n-1)/(h(n-1) * h(n-1))) + (9 * sig(n) * (h(n) + h(n-1)) * (h(n) + h(n-1))/(h(n)*h(n-1)*h(n)*h(n-1))) + (9 * sig(n+1)/(h(n)*h(n)));
u(n) = (mu * u(n)) + (2 * (x(n+1) - x(n-1)));
q(n) = (3 * (y(n+1) - y(n))/h(n)) - (3 * (y(n) - y(n-1))/h(n-1));

disp('Estimating b');
b = Quincunx(n, u, v,  w, q);

disp('calculating rest of the parameters ...');
%calculate the coefficients for only jth cubic
if (j == 1)
    d = y(j) - (mu * 3 * b(2) * sig(j)/h(1));
    d2 = (-3 * b(2) * (h(1) + h(2))/(h(1)*h(2))) + (3 * b(3)/h(2));
    d2 = y(j+1) - (mu * d2 * sig(j));
    a = b(2)/(3*h(1));
    c = ((d2 - d)/h(1)) - (b(2) * h(1)/3);
elseif (j < n)
    a = (b(j + 1) - b(j))/(3 * h(j));
    
    d = (3 * b(j-1)/h(j-1)) + (-3 * b(j) * (h(j-1) + h(j))/(h(j-1)*h(j))) + (3 * b(j+1)/h(j));
    d = y(j) - (mu * d * sig(j));
    
    d2 = (3 * b(j)/h(j)) + (-3 * b(j+1) * (h(j) + h(j+1))/(h(j)*h(j+1))) + (3 * b(j+2)/h(j+1));
    d2 = y(j+1) - (mu * d2 * sig(j+1));
    
    c = ((d2 - d)/h(j)) - ((b(j+1) + (2*b(j))) * h(j)/3);
end

coeffs = [a,b(j),c, d];
end

