function readme()
% Wind_tunnel_automated is the latest version
%% Wind_tunnel_automated.m
%  @Author: Load cell data collection: Roshan
%           Servo serial control: Fintan
%
%  Input before execution
%  * comPort : comport to which the load cell arduino is connected
%  * ServocomPort : comport to which the servo arduino is connected
%  * samples: Enter the number of samples you want to collect for each Aoa and
%             flap angle configuration
%  * AoA_arr: array of AoAs you want to measure
%  * fa_arr: array of flap angles you want to measure
%  * line 86: Set the name of file you want to save load cell data to
%  * line 87: Set the name of file you want to save force data to
%  
%  Input after pressing run:
%  * press enter when you are ready
%  
%  Arduino side:
%  * Upload Wind_tunnel_ir.ino to servo Arduino
%  * Upload TestRigAngleOfAttack.ino to load cell Arduino
%  Googledrive location: ./Code/wind_tunnel_Arduino/Wind_tunnel_tests/*
%
%  Incase of unexpected error press Ctrl+C
%  then run serial_close.m to close all serial objects and delete them
%
%  Side note: fprintf(ServoArd,'%s','f<val>') sets the flap angle
%             fprintf(ServoArd,'%s','w<val>') sets the angle of attack
%             <val> consists of int2str(val) and is a string of size 3
%
%% Data files
%  Wind_tunnel_force_data.m: N x 5 matrix. 
%  - col1: LIFT , col2: DRAG, col3: PITCHING MOMENT, col4: AoA, col5: flapA
%  
%  Wind_tunnel_lc_data.m: N x 6 matrix.
%  - col1: 1, col2: l1, col3: l2, col4: l3, col5: d1, col6 d2
%
%% Depenedencies
%  1. SetupSerial.m: Sets up Serial communication between Matlab and Arduino
%                      @Author: gianluca88 
%                      @Source: http://www.instructables.com/id/Arduino-and-Matlab-let-them-talk-using-serial-comm/
%  2. calibrationPerms.m: Described section 1.
%  3. readSerial.m: @Author: Roshan
%                            - Waits for data to be available in the Serial buffer
%                              before reading
%  4. serial_close.m: Closes all open serial objects
%  Added a new line in develop branch
clc
help readme
end

