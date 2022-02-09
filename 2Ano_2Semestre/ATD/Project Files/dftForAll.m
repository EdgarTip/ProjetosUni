%%
%CODIGO DESENVOLVIDO POR:
%Edgar Filipe Ferreira Duarte
%Pedro Guilherme da Cruz Ferreira
%DEI 2020/2021

function [] = dftForAll(input_data,exp,labels,fs,activitiesID, activities_to_consider)

axis = ["X", "Y", "Z"];

figure();

length_ids = 12;
if(numel(activities_to_consider) ~=0)
    length_ids = numel(activities_to_consider);
end

for k=1:3 
    %Gets all of the data from a specific activity in a specific experiment
    %(given as parameter).


    %Go through all of the activities and show their DFT
    disp(activitiesID);
    for i=1:length(activitiesID)

         has_match = 1;
        %means there is a filter applied to the users. WE will skip any
        %activity that does not have the same name as the one given
        if(numel(activities_to_consider) ~=0)

            has_match = 0;

            for j=1 : length(activities_to_consider) 

                %no match
                if(activitiesID(i) ~= activities_to_consider(j))
                    continue

                %a match has been found
                else
                    has_match = 1;
                    break;

                end
            end
        end

        %Means there was no match this cicle of the loop
        if(has_match == 0)
            continue
        end


        current_activity = find(labels(:,1) == exp & labels(:,3)== i);

        edges =  labels(current_activity(1),4:5);

        %Calculate the DFT for all of edges
        %We detrend the input so that we have a more accurate
        %mesurement of the dft
        number_elements = numel(input_data(edges(1,1):edges(1,2),1));


         %Runs through lines

         data2 = detrend(input_data(edges(1,1): edges(1,2), k));    


         [frequences,dft] = dftCalc(data2, fs);


         %plot the result      
         subplot(3,length_ids,(k-1)*length_ids+ i);
         plot(frequences,dft * 2/number_elements,"-");
         title( activitiesID{i} + " " +axis(k));
         ylabel('Magnitude, |X|')
         xlabel('Frequence [Hz]')
         %Make sure that the grahps do not glitch out
         set(gca,'XLim',[frequences(1) frequences(end)]);
    end
end
sgtitle("DTF of all activities for experience " + exp);
end

