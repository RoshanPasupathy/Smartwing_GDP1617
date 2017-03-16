function [ read_out ] = readSerial( obj, type_id )
%Returns oldest serial value in serial buffer
%   reading protocol for serial to prevent timeouts
%   only reads if Bytes are available
tstart = cputime;
timeout = false;
while ((obj.BytesAvailable == 0)  && ~timeout) 
    if (cputime - tstart > 120)
        timeout = true;
    end
end
if timeout
    read_out = 0;
else
    read_out = fscanf(obj,type_id);
end
