function readme()
%  Ordered by run sequence
%% calibrationPerms.m
%  Author: Fintan
%  Purpose: Script outputs Y matrix for calibrations
%           - Output: Test_Cases and Lift_location
%  Notes: Takes into account that pitching moment = 0 in the absence of
%         lift forces
%
%% ArduinoSerial.m
%  Author: Roshan
%  Purpose: Run this script to get data from load cells via Arduino
%           
%  Parameters to set:
%    1. Comport
%    2. Y matrix
%    3. save file (Default: 'calib_data.mat')
%
%  Dependencies:
%    1. SetupSerial.m: Sets up Serial communication between Matlab and Arduino
%                      @Author: gianluca88 
%                      @Source: http://www.instructables.com/id/Arduino-and-Matlab-let-them-talk-using-serial-comm/
%    2. calibrationPerms.m: Described section 1.
%    3. readSerial.m: @Author: Roshan
%                     - Waits for data to be available in the Serial buffer
%                       before reading
%
%  Notes: 
%  - Incase of error please fclose(Arduino) and delete.
%    
%% calibration_proc.m                       
%  Author: Roshan (For theory refer to documentation of script)
%  Purpose: Run this obtain calibration parameters
%           - Displays standard deviation at each stage and asks if further
%             regression is necessary
%           - Output: C_struct containing cells which contain C matrices
%                     and associated standard deviation
%
%  Parameters to set:
%    1. Primary read file to set R and Y (Default: 'calib_data.mat')
%    2. Read files for calibration history (Default: 'calib_data1&2&3.mat')
%
%  Dependencies:
%    1. calib_data.mat: Data file provides value of R and Y
%    2. calib_data1&2&3.mat: Data gathered over time used to estimate Vr
%    3. Anonymous functions declared internally
%    4. leastSquaresSolver.m: @Author: Roshan , based on the work of 
%                                      M. Ferreira
%                             - Calculates C and associated standard dev
%  
%  Notes: 
%  - Atleast 3 sets of calibration data based on same loading samples are 
%    required to go beyond iteration 2 of regression
%  - Use celldisp(C_struct.*fieldname*) to show data.
%    **disp to order of 10^-18 may be required 
%  - Structured form of storing data keeps it organised but not necessarily
%    efficient. Prefer saving data as array
%  - Anonymous funcs not elegant but meh
clc
help readme
end

