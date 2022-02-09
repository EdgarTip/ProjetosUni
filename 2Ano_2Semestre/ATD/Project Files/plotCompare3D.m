function [] = plotCompare3D(exp, exp1, exp2, user_number, labels,fs,activitiesID)

data = [exp1; exp2];

%Get the frequences from both the experiences from the users
allFreqs = maxFreqs(data(1:length(exp1),:),exp,labels,activitiesID,fs,[],7);
allFreqs = [allFreqs;maxFreqs(data(1+length(exp1): length(exp1)+length(exp2),:),exp + 1,labels,activitiesID,fs,[],7)];
allFreqs
%Create the 3D plot with all the information
figure();

disp(allFreqs);

%Dinamicas
disp(allFreqs(find(allFreqs(:,4)<4),1))
plot3(allFreqs(find(allFreqs(:,4)<4),1),allFreqs(find(allFreqs(:,4)<4),2),allFreqs(find(allFreqs(:,4)<4),3),"d","Color","b");


grid on
hold on;

%Estaticas
plot3(allFreqs(find(allFreqs(:,4)>=4 & allFreqs(:,4)< 7),1),allFreqs(find(allFreqs(:,4)>=4 & allFreqs(:,4)< 7),2),allFreqs(find(allFreqs(:,4)>=4 & allFreqs(:,4)< 7),3),"^","Color","r");
hold on;

%Transicao
plot3(allFreqs(find(allFreqs(:,4)>=7),1),allFreqs(find(allFreqs(:,4)>=7),2),allFreqs(find(allFreqs(:,4)>=7),3),"v","Color","g");

%Retangulos
%For dinamic activities limits are x : 1.3 -> 2.3, y : 1.2 -> 7.3 , 
%z : 0.7 - 2.5,
%For static activities limits are : 0,1 -> 0.75, y : 0.2 -> 0.6, z : 0.1 ->
%0.8
%The following code was extracted from https://www.mathworks.com/matlabcentral/answers/155704-how-do-i-plot-a-3d-box-containing-points

%Dinamic
X = [1.3;2.3;2.3;1.3;1.3];
Y = [7.3;7.3;0.6;0.6;7.3];
Z = [0.7;0.7;0.7;0.7;0.7];

plot3(X,Y,Z, "Color","b");   % draw a square in the xy plane with z = 0.1
plot3(X,Y,Z+1.8, "Color","b"); % draw a square in the xy plane with z = 2.5

for k=1:length(X)-1
    plot3([X(k);X(k)],[Y(k);Y(k)],[0.7;2.5], "Color","b");
end
 
%Transition
X = [0.1;0.8;0.8;0.1;0.1];
Y = [0.7;0.7;0.2;0.2;0.7];
Z = [0.1;0.1;0.1;0.1;0.1];

plot3(X,Y,Z, "Color","g");   % draw a square in the xy plane with z = 0.1
plot3(X,Y,Z+0.7, "Color","g"); % draw a square in the xy plane with z = 2.5

for k=1:length(X)-1
    plot3([X(k);X(k)],[Y(k);Y(k)],[0.1;0.8], "Color","g");
end
 
 
xlabel('Frequências X')
ylabel('Frequências Y')
zlabel('Frequências Z')
legend([{"DINAMICOS"},{"ESTATICOS"},{"TRANSICAO"}])
title("3D plot of user" + user_number); 

hold off

%%Calculo da sensibilidade e especificidade:


wrong_din = 0;
wrong_trans = 0;

%We go through the whole array and we see if the dinamic are in the area
%where they should be and the same to static. We also check to see if the 
for i=1:size(allFreqs,1)
    %Dinamic box
    if( allFreqs(i,1) >= 1.3 && allFreqs(i,1) <= 2.3 && allFreqs(i,2) >= 0.6 && allFreqs(i,2) <= 7.3 && allFreqs(i,3) >= 0.7 && allFreqs(i,3) <= 2.5)
        %Means that it is NOT a dinamic activity (means that an activity
        %is or transition or static).
        if(allFreqs(i,4)>=4)
            %Means that a transition action entered the dinamic box (never
            %happens) 
            if(allFreqs(i,4) >= 7)
                wrong_trans = wrong_trans +1;
            end
            wrong_din = wrong_din +1;  
        end
    else
        %Means that a dinamic activity was found outside its zone
        if(allFreqs(i,4)==1 || allFreqs(i,4)==2 || allFreqs(i,4)==3)
            wrong_din = wrong_din +1;
        end
        
        if(allFreqs(i,1) >= 0.1  && allFreqs(i,1) <= 0.75 && allFreqs(i,2) >= 0.2  && allFreqs(i,2) <= 0.6 && allFreqs(i,3) >= 0.1  && allFreqs(i,3) <= 0.8)

      
            if(allFreqs(i,4) < 7)
                wrong_trans = wrong_trans + 1;
            end
        else
            if(allFreqs(i,4)>= 7)
               wrong_trans = wrong_trans + 1; 
            end
        end
        
    end
end
disp("Correct dinamic :  "+(1-wrong_din/size(allFreqs,1))*100);
disp("Correct static:  "+(1-wrong_trans/size(allFreqs,1))*100);
end
