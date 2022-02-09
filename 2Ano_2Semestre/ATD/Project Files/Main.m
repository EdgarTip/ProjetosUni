%%
%CODIGO DESENVOLVIDO POR:
%Edgar Filipe Ferreira Duarte
%Pedro Guilherme da Cruz Ferreira
%DEI 2020/2021
% DATASET PL4:
    % exp 26, user 13
    % exp 27, user 13
    % exp 28, user 14
    % exp 29, user 14
    % exp 30, user 15
    % exp 31, user 15
    % exp 32, user 16
    % exp 33, user 16
    
Ids = ["W","WU","WD","SIT","STAND","LAYING","STAND-SIT","SIT-STAND","SIT-LIE","LIE-SIT","STAND-LIE","LIE-STAND"];
colors ={'red', 'green', 'blue', 'cyan', 'magenta', 'yellow', '#0072BD', '#D95319', '#EDB120', '#7E2F8E', '#77AC30', '#A2142F'};

users = [13,14,15,16];
T = 0.02;
fs = 1/T;

all_labelsIDS = ["WALKING","WALKING_UPSTAIRS","WALKING_DOWNSTAIRS","SITTING","STANDING", "LAYING","STAND_TO_SIT", "SIT_TO_STAND", "SIT_TO_LIE", "LIE_TO_SIT", "STAND_TO_LIE", "LIE_TO_STAND"];
labels_din = ["WALKING","WALKING_UPSTAIRS","WALKING_DOWNSTAIRS"];
labels_est = ["SITTING","STANDING", "LAYING"];
labels_trans = ["STAND_TO_SIT", "SIT_TO_STAND", "SIT_TO_LIE", "LIE_TO_SIT", "STAND_TO_LIE", "LIE_TO_STAND"];
all_labels = readFiles('labels.txt');

exp26 = readFiles('acc_exp26_user13.txt');

exp27 = readFiles('acc_exp27_user13.txt');
exp28 = readFiles('acc_exp28_user14.txt');
exp29 = readFiles('acc_exp29_user14.txt');
exp30 = readFiles('acc_exp30_user15.txt');
exp31 = readFiles('acc_exp31_user15.txt');
exp32 = readFiles('acc_exp32_user16.txt');
exp33 = readFiles('acc_exp33_user16.txt');

%%
%Ex2

choice = menu("Pick a user:","1 -exp 26","2- exp 27","3- exp 28", "4- exp 29", "5- exp 30", "6- exp 31", "7- exp 32" , "8- exp 33");

switch(choice)
    case 1
        plotData(exp26,all_labels,26,Ids,colors);
    case 2    
        plotData(exp27,all_labels,27,Ids,colors);
    case 3
        plotData(exp28,all_labels,28,Ids,colors);
    case 4
        plotData(exp29,all_labels,29,Ids,colors);
    case 5
        plotData(exp30,all_labels,30,Ids,colors);
    case 6 
        plotData(exp31,all_labels,31,Ids,colors);
    case 7
        plotData(exp32,all_labels,32,Ids,colors);
    case 8
        plotData(exp33,all_labels,33,Ids,colors);
end





%% Ex 3.1
choice = menu("Pick a dinamic activity:","1 -Walking","2- Walking Downstairs","3- Walking Upstairs");
%For walking

switch(choice)
    case 1
        
        Ids_to_consider =("WALKING");

        dftActivities(exp26,26,all_labels, 50, all_labelsIDS, Ids_to_consider);

        %dftActivities(exp27,27,all_labels, 50, all_labelsIDS, Ids_to_consider);
        %dftActivities(exp28,28,all_labels, 50, all_labelsIDS, Ids_to_consider);
        %dftActivities(exp29,29,all_labels, 50, all_labelsIDS, Ids_to_consider);
        %dftActivities(exp30,30,all_labels, 50, all_labelsIDS, Ids_to_consider);
        %dftActivities(exp31,31,all_labels, 50, all_labelsIDS, Ids_to_consider);
        %dftActivities(exp32,32,all_labels, 50, all_labelsIDS, Ids_to_consider);
        %dftActivities(exp33,33,all_labels, 50, all_labelsIDS, Ids_to_consider);
    
    case 2

        Ids_to_consider = ("WALKING_UPSTAIRS");

        dftActivities(exp26,26,all_labels, 50, all_labelsIDS, Ids_to_consider);

        %dftActivities(exp27,27,all_labels, 50, all_labelsIDS, Ids_to_consider);
        %dftActivities(exp28,28,all_labels, 50, all_labelsIDS, Ids_to_consider);
        %dftActivities(exp29,29,all_labels, 50, all_labelsIDS, Ids_to_consider);
        %dftActivities(exp30,30,all_labels, 50, all_labelsIDS, Ids_to_consider);
        %dftActivities(exp31,31,all_labels, 50, all_labelsIDS, Ids_to_consider);
        %dftActivities(exp32,32,all_labels, 50, all_labelsIDS, Ids_to_consider);
        %dftActivities(exp33,33,all_labels, 50, all_labelsIDS, Ids_to_consider);


    case 3
        Ids_to_consider = ("WALKING_DOWNSTAIRS");

        dftActivities(exp26,26,all_labels, 50, all_labelsIDS, Ids_to_consider);
        
        %dftActivities(exp27,27,all_labels, 50, all_labelsIDS, Ids_to_consider);
        %dftActivities(exp28,28,all_labels, 50, all_labelsIDS, Ids_to_consider);
        %dftActivities(exp29,29,all_labels, 50, all_labelsIDS, Ids_to_consider);
        %dftActivities(exp30,30,all_labels, 50, all_labelsIDS, Ids_to_consider);
        %dftActivities(exp31,31,all_labels, 50, all_labelsIDS, Ids_to_consider);
        %dftActivities(exp32,32,all_labels, 50, all_labelsIDS, Ids_to_consider);
        %dftActivities(exp33,33,all_labels, 50, all_labelsIDS, Ids_to_consider);
        
end
%% 3.2
%Calculates the DFT of all activities in a experience (for better visual clarity and
%better execution time, only the first occurance of each activity is shown
%NOTE: We recommend only executing 1 line at a time, seing that executing
%all of the lines could take some time (around 15-20 seconds if you
%have a good computer)
clc
ids_to_consider = [];
dftForAll(exp26,26,all_labels, 50, all_labelsIDS,ids_to_consider);
%%
dftForAll(exp27,27,all_labels, 50, all_labelsIDS,ids_to_consider);
dftForAll(exp28,28,all_labels, 50, all_labelsIDS,ids_to_consider);
dftForAll(exp29,29,all_labels, 50, all_labelsIDS,ids_to_consider);
dftForAll(exp30,30,all_labels, 50, all_labelsIDS,ids_to_consider);
dftForAll(exp31,31,all_labels, 50, all_labelsIDS,ids_to_consider);
dftForAll(exp32,32,all_labels, 50, all_labelsIDS,ids_to_consider);
dftForAll(exp33,33,all_labels, 50, all_labelsIDS,ids_to_consider);
%% 3.3

choice = menu("Pick a user:","1 -user 13","2- user 14","3- user 15", "4- user 16");

switch(choice)
    case 1

        user_number = 13;
        getLimits(26, exp26, exp27, user_number, all_labels, 50, all_labelsIDS);
        
    case 2
        %%
        clc
        user_number = 14;
        getLimits(28, exp28, exp29, user_number, all_labels, 50, all_labelsIDS)
    case 3
        user_number = 15;
        getLimits(30, exp30, exp31, user_number, all_labels, 50, all_labelsIDS)
    case 4
        user_number = 16;
        getLimits(32, exp32, exp33, user_number, all_labels, 50, all_labelsIDS)
end

%% Ex 3.4

steps = [];
stepsPerUser = zeros(4,3);    % 4 users, 3 atividades


[steps] = stepsMin(exp26,26,all_labels,steps);
[steps] = stepsMin(exp27,27,all_labels,steps);
[steps] = stepsMin(exp28,28,all_labels,steps);
[steps] = stepsMin(exp29,29,all_labels,steps);
[steps] = stepsMin(exp30,30,all_labels,steps);
[steps] = stepsMin(exp31,31,all_labels,steps);
[steps] = stepsMin(exp32,32,all_labels,steps);
[steps] = stepsMin(exp33,33,all_labels,steps);

userIndex=0;
disp("MÉDIA DE PASSOS POR MINUTO POR USER: ");
disp("               WALKING          WALKING UP      WALKING DOWN");
 for i=1 :2: 7 %Experiences
     userIndex= userIndex+1;
     for j=1: 3 %Number of activities
         stepsPerUser(userIndex,j) = mean( nonzeros( [steps(i,j),steps(i+1,j)]));
     end
     fprintf('User %2d      %10f        %10f        %10f\n',users(userIndex), stepsPerUser(userIndex,1),stepsPerUser(userIndex,2),stepsPerUser(userIndex,3));
 end
fprintf("\n");

 disp("               MEDIA         DESVIO PADRAO");
 fprintf("WALKING:      %10f    %10f\n", mean(stepsPerUser(:,1)),std(stepsPerUser(:,1)));
 fprintf("WALKING UP:   %10f    %10f\n", mean(stepsPerUser(:,2)),std(stepsPerUser(:,2)));
 fprintf("WALKING DOWN: %10f    %10f\n", mean(stepsPerUser(:,3)),std(stepsPerUser(:,3)));
 
 %% 3.5 / 3.6
% 3D plot to compare activities in each user

choice = menu("Pick a user:","1 -user 13","2- user 14","3- user 15", "4- user 16");

switch(choice)
    case 1

        user_number = 13;
        plotCompare3D(26, exp26, exp27, user_number, all_labels, 50, all_labelsIDS);
        
    case 2

        user_number = 14;
        plotCompare3D(28, exp28, exp29, user_number, all_labels, 50, all_labelsIDS)
    case 3
        user_number = 15;
        plotCompare3D(30, exp30, exp31, user_number, all_labels, 50, all_labelsIDS)
    case 4
        user_number = 16;
        plotCompare3D(32, exp32, exp33, user_number, all_labels, 50, all_labelsIDS)
end

%%
%Ex 4
choice = menu("Pick a user:","1 -exp 26","2- exp 27","3- exp 28", "4- exp 29", "5- exp 30", "6- exp 31", "7- exp 32" , "8- exp 33");

switch(choice)
    case 1
        sftfCalc(exp26);
    case 2    
        sftfCalc(exp27);
    case 3
        sftfCalc(exp28);
    case 4
        sftfCalc(exp29);
    case 5
        sftfCalc(exp30);
    case 6 
        sftfCalc(exp31);
    case 7
        sftfCalc(exp32);
    case 8
        sftfCalc(exp33);
end
