clc 
clear

% Run this script to get data from load cells via Arduino
comPort = '/dev/tty.usbmodem1421';
[Arduino, flag] = setupSerial(comPort);
%No of loading conditions. set this
%N = 5;

%Y = ones(N, 3);
%Normal load 
%lift_forces =[200; 500; 700; 900; 1200; 1400; 1600; 1900; 2100; 2300; 2600; 2800; 3000; 3300; 3500;
%   700; 1400; 2100; 2800; 3500]; %eccentric load
calibrationPerms
L_loc = Lift_location;
Y = Test_Cases;
%Y = zeros(N,3);
%Y(:,1) = lift_forces;

%N = size(lift_forces,1);
N = size(Y, 1);

%unit row and 5 columns, one for each tranducer
R = zeros(N, 6);
%set unit row
R(:, 1) = 1;
offsets = zeros(1, 5);

%find offsets for the data collected
disp('getting offsets for transducers');
offsets(1,1) = readSerial(Arduino,'%d');
offsets(1,2) = readSerial(Arduino,'%d');
offsets(1,3) = readSerial(Arduino,'%d');
offsets(1,4) = readSerial(Arduino,'%d');
offsets(1,5) = readSerial(Arduino,'%d');
fprintf(Arduino,'%c', 'c'); %indicate to arduino that data has been received

%fill R matrix
for p =  1:1:N
    %display which loading condition
    disp(sprintf('Loading condition %d ; Lift = %f, Drag = %f , pitching moment = %f, Lift_location = %f cm', p , Y(p,:), L_loc(p)));
    R(p,2) = readSerial(Arduino,'%d');
    R(p,3) = readSerial(Arduino,'%d');
    R(p,4) = readSerial(Arduino,'%d');
    R(p,5) = readSerial(Arduino,'%d');
    R(p,6) = readSerial(Arduino,'%d');
    if (p == N)
        fprintf(Arduino,'%c', 'e'); %indicate to arduino that last data has been sent
    else
        fprintf(Arduino,'%c', 'g'); %indicate to arduino that data has been received
    end
end

%save all data 
save('calib_data100317_3.mat','R','Y','offsets');

fclose(Arduino);
delete(Arduino);
