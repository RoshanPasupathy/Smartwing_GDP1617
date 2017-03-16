clc 
clear

% Run this script to get data from load cells via Arduino
comPort = '/dev/tty.usbmodem1421';
[Arduino, sflag] = setupSerial(comPort);

%begin request for data
fprintf(Arduino,'%c', 'c');

l1 = zeros(100,1);
l2 = zeros(100,1);
l3 = zeros(100,1);
d1 = zeros(100,1);
d2 = zeros(100,1);
Force_mat =  zeros(100, 3);

R =  zeros(1, 6);
R(1) = 1;

C_load = load('calib_matrix.mat');
C = C_load.C_struct.CW1{3};

%Wait to receive data
while Arduino.BytesAvailable == 0
end
flag=fread(Arduino,1,'uchar');

it = 1;
loop = 0;
runflag = 'y';
while (flag ~= 'e') && (runflag == 'y')
    current_it = loop*100 + it;
    l1(current_it) = readSerial(Arduino,'%d');
    l2(current_it) = readSerial(Arduino,'%d');
    l3(current_it) = readSerial(Arduino,'%d');
    d1(current_it) = readSerial(Arduino,'%d');
    d2(current_it) = readSerial(Arduino,'%d');
    
    R(2:6) = [l1(current_it) l2(current_it) l3(current_it) d1(current_it) d2(current_it)];
    Force_mat(current_it, :) = R * C;
    disp(sprintf('LIFT = %.6f g, DRAG = %.6f g, PM = %.6f gm', Force_mat(current_it, :)));
    
    it = it + 1;
    if (it < 101)
        fprintf(Arduino,'%c', 'g');
    else
        runflag = input('Proceed? y or n?','s');
        if (runflag == 'n')
            fprintf(Arduino,'%c', 'e');
        else
            %new loop
            fprintf(Arduino,'%c', 'g');
            it = 1;
            loop = loop + 1;
            l1 = [l1 ; zeros(100,1)];
            l2 = [l2 ; zeros(100,1)];
            l3 = [l3 ; zeros(100,1)];
            d1 = [l1 ; zeros(100,1)];
            d2 = [d2 ; zeros(100,1)];
            Force_mat = [Force_mat ; zeros(100,3)]; 
        end

    end
    %confirm this set of data received data received

end

%save all data 
save('Wind_tunnel_results.mat','l1','l2','l3','d1','d2', 'Force_mat');

fclose(Arduino);
delete(Arduino);
