function [error_arr, weight_arr, readings_arr] = testC(comPort,C_mat, comp)
[Arduino, flag] = setupSerial(comPort);
error_arr = [];
weight_arr = [];
readings_arr = [];
readings = zeros(1,6);
readings(1,1) = 1;
runflag = 'y';
Forces = [0 0 0];
Position = 0;
fprintf(Arduino,'%c', 'c'); %Send request char 
while ((runflag == 'y') && (flag == 1))
    disp('Please place weight and toggle switch');
    %Lift
    Forces(1) = input('Mass of lift weight placed in g:  ');
    Position = input('Position of lift weight in cm: ');
    %Drag
    Forces(2) = input('Mass of drag weight placed in g:  ');
    %Pitching moment
    Forces(3) = Forces(1) * 9.81 * Position/100;
    
    for r = 2:1:6
        readings(1,r) = readSerial(Arduino,'%d');
    end
    
    m_weight = readings * C_mat;
    disp(sprintf('Measured Lift:  %.6f, Measured Drag:  %.6f, Measured Pitching Moment:  %.6f',m_weight(1), m_weight(2), m_weight(3)));
    err = Forces - m_weight;
    errp = [0 0 0];
    for e = 1:1:3
        if (Forces(e) == 0)
            errp(e) =  err(e) * 100 * 2/(Forces(e) + m_weight(e));
            if (isinf(errp(e)))
                errp(e) = err(e) * 100/0.01;
            end
        else
            errp(e) = err(e) * 100/Forces(e);
        end
    end 
    disp(sprintf('error percentage L:  %.6f, D:  %.6f, M:  %.6f ',errp(1), errp(2), errp(3)));
    readings_arr = [readings_arr ; readings];
    error_arr = [error_arr ; err];
    weight_arr = [weight_arr ; Forces];
    runflag = input('Proceed? y or n? ','s');
    if (runflag == 'n')
        fprintf(Arduino,'%c', 'e');
    else
        fprintf(Arduino,'%c', 'g');
    end
end
fclose(Arduino);
delete(Arduino);
end
    

