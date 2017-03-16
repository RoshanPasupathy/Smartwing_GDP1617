function [ C_mat, std_dev ] = leastSquaresSolver(R, Y, C_prev, Vr)
%% Solve using previous iteration data
N = size(R,1);
I = eye(N);
D = zeros(N, 6 * N);
err = Y - (R * C_prev);
std_dev = sqrt([err(:,1)'*err(:,1)+1  err(:,2)'*err(:,2) err(:,3)'*err(:,3)]);
C_mat = zeros(6, 3);
for force = 1:1:3
    for p = 1:1:N
        for j = 1:1:6
            D(p,((j - 1)*N) + p) = C_prev(j,force);
        end
    end
    %estimate Vw using previous std_dev
    Vw = (I * 0.7 * std_dev(force));
    W = inv(Vw + (D * Vr * D'));
    C_mat(:,force) = (R' * W * R)\(R' * W * Y(:, force));
    error_temp = Y(:,force) - (R * C_mat(:,force));
    %calculate new std_dev for each 'force'
    std_dev(force) = sqrt(error_temp' * error_temp);
end
end

