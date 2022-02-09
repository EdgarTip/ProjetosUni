%%
%CODIGO DESENVOLVIDO POR:
%Edgar Filipe Ferreira Duarte
%Pedro Guilherme da Cruz Ferreira
%DEI 2020/2021

function [] = plotData(data, labels, exp, ids, colors)
    fs=50;
    deltaT = 1/fs/60 ;
    %time = 1:length(data);% Antigo
    time = 1*deltaT: deltaT: length(data)*deltaT; %Novo
    figure(1);
    limits=zeros(3,2); %3 axis, 2 limits
    activities = find(labels(:,1)==exp);
    disp(length(activities))
    
for axis=1 : 3
    
    limits(axis,1) = round(max(data(:,axis))+0.4,1);
    limits(axis,2) = round(min(data(:,axis))-0.4,1);
    subplot(3,1,axis);
    plot(time,data(:,axis),'black');
    switch axis
        case 1
            ylabel('ACC X');
        case 2
            ylabel('ACC Y');
        case 3
            ylabel('ACC Z');
    end
    xlabel('Time [minutos]');
   
   % Lables: experiment number id | user number id | activity number id | label start point | label end point 
    for i=1:length(activities)  %Percorre linhas das lables
           hold on
           %plot(time(labels(i,4):labels(i,5)),data(labels(i,4):labels(i,5),axis),'color',colors{labels(i,3)}%);    %Antigo
           plot(time( labels(activities(i),4):labels(activities(i),5)), data(labels(activities(i),4):labels(activities(i),5),axis),'color',colors{labels(activities(i),3)} );    
           
           ylim( [ limits(axis,2) limits(axis,1) ] );
           %text(labels(i,4) ,data(labels(i,4)), ids(labels(i,3)), 'FontSize', 6, 'fontweight', 'bold');
           if(rem(i,2) == 0)
                text(time(labels(i,4)) ,limits(axis,2)+0.2 , ids(labels(i,3)), 'FontSize', 6, 'fontweight', 'bold');
           else
                text(time(labels(i,4)) ,limits(axis,1)-0.2 , ids(labels(i,3)), 'FontSize', 6, 'fontweight', 'bold');
           end
           hold off

    end
end
end