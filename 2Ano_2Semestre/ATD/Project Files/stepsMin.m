%%
%CODIGO DESENVOLVIDO POR:
%Edgar Filipe Ferreira Duarte
%Pedro Guilherme da Cruz Ferreira
%DEI 2020/2021
function [steps] = stepsMin(rawData,exp,labels,steps)
meanSteps = zeros(1,3); %Will have the mean of steps per minute for each activity in the current experience

for i=1:3 % 3 Atividades dinamicas: "W"(1),"WU"(2),"WD"(3)  
    % Lables: experiment number id | user number id | activity number id | label start point | label end point 
    activityIndex = find(labels(:,1) == exp & labels(:,3)== i)'; % ARRAY DE INDICES ONDE TEM A EXPERIENCIA E A ATIVIDADE DESEJADA
    activityOcurrencesSteps=zeros(1,length(activityIndex)); %ARRAY COM OS PASSOS / MIN DE TODAS AS OCORRENCIAS DA ATIVIDADE
    
    if(numel(activityIndex) ~= 0)   % Se não estiver vazio
        for j=1: length(activityIndex)  % Percorre as ocorrencias da atividade na exp atual
            %limits = labels(activityIndex(1),4:5);  %range: data(labels(activityIndex(1),4) : data(labels(activityIndex(1),5))

            data = detrend( rawData( labels(activityIndex(j),4) : labels(activityIndex(j),5) , 1)); % PARA CONTAR PASSOS, O EIXO DO X (VERTICAL) É O Q INTERESAS (SO QUEREMOS O X, PORQUE POR LEITURA DOS GRAFICOS VEMOS QUE É O EIXO QUE INTERESAS)
            
            [freqs,dft] = dftCalc(data, 50);
            
            [~,ind] = max(dft); % INDICE DO VALOR MAXIMO DA DFT -> Equivale ao indice da frequencia mais predominante no sinal original
            
            realMax = abs(freqs(ind));  % FREQ MAIS PREDOMINANTE NO SINAL

            activityOcurrencesSteps(1,j)=realMax*60; %Hz -> steps/min = max * 60
        end
        meanSteps(i) = mean(activityOcurrencesSteps);% / length(activityIndex);   %Total steps per min / activiy ocurrences
    end   
end
steps = [steps;meanSteps];   %concatenate to the final matrix
end