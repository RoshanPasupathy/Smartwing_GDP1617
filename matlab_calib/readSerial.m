function [ read_out ] = readSerial( obj, type_id )
%Returns oldest serial value in serial buffer
%   reading protocol for serial to prevent timeouts
%   only reads if Bytes are available
tstart = cputime;
while obj.BytesAvailable == 0
end 
read_out = fscanf(obj,type_id);
end
