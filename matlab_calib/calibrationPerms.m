%% Create array of all possible loading conditions
%Lift_mass=linspace(0,6,4);
offset = 36;
Lift_mass = [0 2000 4000 6000] + offset;
%Drag_mass=linspace(0,0.9,4);
Drag_mass = [0 200 700 1000];
%Drag_mass = [0 500 1000 1500 2000];
P_Mom= [-500 0 500];
Test_Cases=zeros(length(Lift_mass)*length(Drag_mass)*length(P_Mom),3);
index=0;
for lift=1:length(Lift_mass)
    for drag=1:length(Drag_mass)
        for pm=1:length(P_Mom)
            index=index+1;
            Test_Cases(index,:)=[Lift_mass(lift),Drag_mass(drag),P_Mom(pm)];
        end
    end
end
%% Remove impossible rows ( i.e. no lift so cant have a pitching moment..)
rowsToRemove=[];
for i=1:length(Test_Cases)
   if (Test_Cases(i,1)==offset)&&(Test_Cases(i,3)~=0)
       % save indicies of offending rows
       rowsToRemove=[rowsToRemove,i];
   end
end
% Remove the rows
Test_Cases(rowsToRemove,:)=[];

%% work out where to put lift weight in cm for each test case
Lift_location=(Test_Cases(:,3)./(Test_Cases(:,1).*9.81)).*100;
Lift_location(Lift_location >= 20)=0;

%clearvars -except Lift_location Test_Cases

