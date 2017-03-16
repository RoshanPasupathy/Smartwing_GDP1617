clc 
clear

% Run this script to get data from load cells via Arduino
comPort = '/dev/tty.usbmodem1421'; %left
ServocomPort = '/dev/tty.usbmodem1411'; %right
%comPort2 = '/dev/tty.usbmodem1411';
%comPort2 = '/dev/tty.usbmodem1411';
[Arduino, sflag] = setupSerial(comPort);
[ServoArd, serflag] = setupSerial(ServocomPort);

%[ServoArd, s2flag] = setupSerial(comPort2);
samples = 15;
AoA_arr = 0:2:14;
AoA_arr = [AoA_arr -AoA_arr];
%AoA_arr = -16:8:16;
flap_arr = 0:3:30;
flap_arr = [flap_arr -flap_arr];
%flap_arr = -30:10:30;

AoAs = size(AoA_arr,2);
flap_angles = size(flap_arr,2);

%let servoarduino intialise


fprintf(ServoArd,'%s','f000');
pause(2);
fprintf(ServoArd,'%s','w000');

startin = input('Start?');

%begin request for data
fprintf(Arduino,'%c', 'c');
R = zeros(samples*flap_angles*AoAs,6);
R(:,1) = 1;
Force_mat =  zeros(samples*flap_angles*AoAs, 5);
%assign the flap angle and AoA for each flap angle
for aoa_i = 1:1:AoAs
    for fa_i = 1:1:flap_angles
        for smp = 1:1:samples
            Force_mat((aoa_i-1)*samples*flap_angles + (fa_i-1)*samples + smp,4) = AoA_arr(aoa_i);
            Force_mat((aoa_i-1)*samples*flap_angles + (fa_i-1)*samples + smp,5) = flap_arr(fa_i);
        end
    end
end

C_load = load('calib_matrix.mat');
C = C_load.C_struct.CW1{3};

for aoa_i = 1:1:AoAs
    %send command to servo arduino : w<angle>
   fprintf(ServoArd,'w%3s',int2str(AoA_arr(aoa_i)));
    
    for fa_i = 1:1:flap_angles
        % ______ Servo control code ______________
        %         command = sprintf('w%3s',int2str(AoA_arr(fa)));
        %send command to servo arduino : f<angle>
        fprintf(ServoArd,'f%3s',int2str(flap_arr(fa_i)));
        %tell arduino to wait
        fprintf(Arduino,'%c','d');
        for smp =  1:1:samples
            % Request data for sample
            fprintf(Arduino,'%c', 'r');
            current_it = (aoa_i-1)*samples*flap_angles + (fa_i-1)*samples + smp;
            R(current_it,2) = readSerial(Arduino,'%d');
            R(current_it,3) = readSerial(Arduino,'%d');
            R(current_it,4) = readSerial(Arduino,'%d');
            R(current_it,5) = readSerial(Arduino,'%d');
            R(current_it,6) = readSerial(Arduino,'%d');
            Force_mat(current_it,1:3) = R(current_it,:) * C;
            disp(sprintf('LIFT = %.6f g, DRAG = %.6f g, PM = %.6f gm, Aoa = %d, Flap angle = %d',  Force_mat(current_it,:)));
            %received all data for this sample > request new sample
        end
        %confirm this set of data received data received
        
    end
end
fprintf(Arduino,'%c', 'e');

fprintf(ServoArd,'%s','f000');
pause(4);
fprintf(ServoArd,'%s','w000');

%save all data
save('Wind_tunnel_lc_data.mat','R');
save('Wind_tunnel_force_data.mat','Force_mat');


garbage = 1;

%empty serial buffer
while Arduino.BytesAvailable > 0
    garbage = fscanf(Arduino,'%d');
end

fclose(ServoArd);
delete(ServoArd);
fclose(Arduino);
delete(Arduino);
