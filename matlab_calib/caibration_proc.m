clear all
%% Output struct of calibraton matrices and associated standard deviations
% Author: Roshan Pasupathy
% Based on: 'Design of a Six-component External Wing tunnel balance'
%           - M. Ferreira
% URL: https://fenix.tecnico.ulisboa.pt/downloadFile/844820067124978/Dissertation.pdf
%      last accessed[March 4th 2017]

%% Load file which contains calibration parameters
Refdat = load('calib_data100317_1.mat');
C_struct = struct;
R = Refdat.R;
Y = Refdat.Y;
N = size(R,1);
I = eye(N);

%% Anonymous func defintions

calc_C = @(a,w,y) (a' * w * a)\(a' * w * y);

% 70% of standard dev is Vw diagonal elements
calc_Vw = @(fact) (I * 0.7 * fact);

% output format => (sigmaL , signmaD , sigmaM)
calc_std_dev = @(err) sqrt([err(:,1)'*err(:,1)  err(:,2)'*err(:,2) err(:,3)'*err(:,3)]);

%% Body of script
%Check if Undetermined set of equations  
if (N < 6)
    disp('Too few loading conditions');
    disp('Undetermined set of equations');
else
    %% Take  I as Weighting matrix
    CI = calc_C(R, I, Y);
    error = Y - (R*CI);
    std_dev = calc_std_dev(error);
    
    value={'No weighting';
        'Calib Matrix';
        CI,
        'Error';
        std_dev;
        };
    C_struct.CI = value;
    disp(sprintf('Lift error: %2.5f , Drag error: %2.5f , Moment error: %2.5f', std_dev));
    CIstd_dev = std_dev;
    
    %% Weighting matrix = Vw inverse
    Vwl = calc_Vw(std_dev(1))
    Vwd = calc_Vw(std_dev(2)) %eye(N)
    Vwpm = calc_Vw(std_dev(3)) %eye(N)
    
    
    CVw = zeros(6,3);
    CVw(:,1) = calc_C(R, inv(Vwl), Y(:,1));
    CVw(:,2) = calc_C(R, inv(Vwd), Y(:,2));
    CVw(:,3) = calc_C(R, inv(Vwpm), Y(:,3));
    
    error = Y - (R * CVw);
    std_dev = calc_std_dev(error);
    
    value={'Vw inverse weighting';
        'Calib Matrix';
        CVw;
        'Error';
        std_dev;
        };
    disp(sprintf('Lift error: %2.5f , Drag error: %2.5f , Moment error: %2.5f', std_dev));
    C_struct.CVw = value;
    
    %% Regression solver
    disp(sprintf('To proceed further, >3 calibrations of %d loading conditions are required', N));
    runflag = input('Proceed? y or n?','s');
    i = 0;
    if (runflag == 'y')||(runflag == 'Y')
        %make three columns of 6N data,
        % Data  1  Data  2  Data  3
        % trans11  trans11  trans11
        %   ...      ...      ...
        % trans1N  trans1N  trans1N
        %  ...       ...      ...
        % trans6N  trans6N  trans6N
        read1 = load('calib_data100317_1.mat');
        read2 = load('calib_data100317_2.mat');
        read3 = load('calib_data100317_3.mat');
        
        %check if loading conditions were the same
        if (any(any((read1.Y ~= read2.Y) + (read2.Y ~= read3.Y))))
            disp('Loading conditions do not match for calib data');
        else
            Rhist = [];
            Rhist = [Rhist read1.R(:)];
            %Rhist = [Rhist read2.R(:)];
            Rhist = [Rhist read3.R(:)];
            Vr = cov(Rhist');
            C_mati_1 = CVw;
            while ((runflag == 'y')||(runflag == 'Y'))
                i = i + 1; % new iteration
                [C_mati, std_dev] = leastSquaresSolver(R, Y, C_mati_1, Vr);
                field = sprintf('CW%d', i);
                value{1} = sprintf('Regression number %d', i);
                value{3} = C_mati;
                value{5} = std_dev;
                C_struct.(field) = value;
                disp(sprintf('Lift error for %s : %2.5f , Drag error: %2.5f , Moment error: %2.5f','CI', CIstd_dev));
                disp(sprintf('Lift error for %s : %2.5f , Drag error: %2.5f , Moment error: %2.5f',field, std_dev));
                runflag = input('Proceed? y or n?','s');
                C_mati_1 = C_mati;
            end
        end
    end
    %% Display latest iteration info.
    disp(fieldnames(C_struct));
    fields = fieldnames(C_struct);
    disp(i);
    celldisp(C_struct.(fields{2 + i})); 
end
disp('**** Available Calibrations ****');
disp(fieldnames(C_struct));




