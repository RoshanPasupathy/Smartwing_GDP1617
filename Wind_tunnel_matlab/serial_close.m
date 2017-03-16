function serial_close()
%UNTITLED2 Summary of this function goes here
%   Detailed explanation goes here
    open_Ser = instrfind;
    if size(open_Ser,1) ~= 0
        fclose(open_Ser);
        delete(open_Ser);
    end
end

