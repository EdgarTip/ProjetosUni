%%
%CODIGO DESENVOLVIDO POR:
%Edgar Filipe Ferreira Duarte
%Pedro Guilherme da Cruz Ferreira
%DEI 2020/2021


function [] = dftActivities(input_data,exp,labels,fs,activitiesID,activities_to_consider)


axis = ["X","Y","Z"];

%Find all of the places where a specific activity takes place and stores
%them for further DTF aplication

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
    

    %Gets all of the data from a specific activity in a specific experiment
    %(given as parameter).
    current_activity = find(labels(:,1) == exp & labels(:,3)== i);
    
    %If the activity exists, then we get the edges of the timestamps 
    times = numel(current_activity);
    edges = [];
    
    %If there is no activity, go to the next cicle
    if(times == 0) 
        continue
    end
    
    for j = 1: times
        edges = [edges ; labels(current_activity(j),4:5)];
        times = times - 1;
    end
    
    % we get the number of elements between timestamps
    
    

    figure();

    %Due to each experience having the possibility of having multiple
    %occurances we have to calculate the medium


    %Now that we have the number of elements and the places where they are
    %we calculate the DFT of the dataset and create the hamming and hann
    %windows, for each of the  3D axis
      
    for k=1:3
        plotcounter = 1;
       for j=1:length(edges(:,1))
         %Calculate the DFT for all of edges
         %We detrend the input so that we have a more accurate
         %mesurement of the dft
         number_elements = edges(j,2) - edges(j,1);
         data2 = detrend(input_data(edges(j,1): edges(j,2),k)); 



       
        %plot the result
         subplot(3,5 *length(edges(:,1)),(k-1)*(5 *length(edges(:,1)))+plotcounter);
         [frequences,dft] = dftCalc(data2, fs);        
         plot(frequences,dft * 2/number_elements,"-");
         title( activitiesID{i} + " " + "(" + j + ")" +axis(k));
         ylabel('Magnitude, |X|')
         xlabel('Frequencia [Hz]')
         plotcounter = plotcounter + 1;
         %Make sure that the grahps do not glitch out
         set(gca,'XLim',[frequences(1) frequences(end)]);
         
         [~,ind] = max(dft);
         most_relevant_freq = abs(frequences(ind));
         
         y = 1;
          
         subplot(3,5 *length(edges(:,1)),(k-1)*(5 *length(edges(:,1)))+plotcounter);
         scatter(y, most_relevant_freq,'r');
         title( "Most relevant Frequence {" + j + "}");
         xlabel('Frequencia [Hz]')
         set(gca,'YLim',[0 most_relevant_freq*2]);
         
         plotcounter = plotcounter +1;
         %%
         %These values are the values that will set the steps the next
         %window in each of the window modes.
           
         %By experience n_frame = 3 had good results 
         n_frame = 2.5;
            
         %where the windows overlap (overlap = 50%) (1.5)
         n_overlap = n_frame/2;
            
         % number of frames in the window (150)
         number_per_frames= round(n_frame*fs);
            
         %number of frames overlaped (75)
         number_per_frames_overlaped = round(n_overlap*fs); 
          
         step = number_per_frames - number_per_frames_overlaped;
         final = number_elements - number_per_frames;
             
         %%
         %Rectangular window. This window was abborded in classes so we
         %thought it would be interesting to implement. We implement it
         %with 50% overlap so we get better results (so that the exits
         %are not too abrupt). We will store the max value of the frequencie of each
         %window so that in the end we can see the % of times a
         %frequence is the highest.
         %amount of frames in the for statement 
         
         best_frequences = [];
            

            
         %We calculate the dft of the current window. Then we search for
         %the largest frequency in the window to see if the largest is
         %close to the original largest
         for l = 1: step : final    
             window = data2(l : l +number_per_frames -1);
             [frequences_in_frame, dft2] = dftCalc(window , fs);
        
             [~, index] = max(dft2);
                         
             best_frequences = [best_frequences, abs(frequences_in_frame(index))];
         end
       
         %Creates plot         
         x = 1:length(best_frequences);
         
         subplot(3,5 *length(edges(:,1)),(k-1)*(5 *length(edges(:,1)))+ plotcounter);
         plot(x,best_frequences,'o');
    

         title("Rectangular " + j + " " +axis(k));
         ylabel('Frequence [Hz]')
         xlabel('Window Number')
         set(gca,'YLim',[0, most_relevant_freq*2]);

         
         plotcounter = plotcounter +1;
         %%
         %Hamming
         %Hamming window was choosen because it showed promisse in
         %classes. Same implementation as windows, the only difference
         %being that we multiply the window by the hamming window
            
         best_frequences = [];

         hamming_w = hamming(number_per_frames);
           



        for l = 1:step:final
            frame = data2(l:l+number_per_frames-1).*hamming_w;
            [frequencies_frame,dft2] = dftCalc(frame, fs);

            [maxMag, ind] = max(dft2);

            best_frequences = [best_frequences, abs(frequencies_frame(ind))];


            % We now store the max 
            %X axis
        end

         x = 1:length(best_frequences);
         
         subplot(3,5 *length(edges(:,1)),(k-1)*(5 *length(edges(:,1)))+ plotcounter);
         plot(x,best_frequences,'o');
         title("Hamming "+ j + " " + axis(k));
         ylabel('Frequence [Hz]')
         xlabel('Window Number')
         set(gca,'YLim',[0, most_relevant_freq*2]);

         plotcounter = plotcounter +1;
         
         
         
         %%Hann
         best_frequences = [];
         hann_window = hann(number_per_frames);
            

         for l = 1: step :final
             x_frame = data2(l : l +number_per_frames -1).*hann_window;
 
             [frequencies_frame,dft2] = dftCalc(x_frame, fs);
             
             [~, ind] = max(dft2);

             best_frequences = [best_frequences, abs(frequencies_frame(ind))];
            
         end

         x = 1:length(best_frequences);
         
         subplot(3,5 *length(edges(:,1)),(k-1)*(5 *length(edges(:,1)))+ plotcounter);
         plot(x,best_frequences,'o');
         title("Hann "+ j + " " + axis(k));
         ylabel('Frequence [Hz]')
         xlabel('Window Number')
         set(gca,'YLim',[0, most_relevant_freq*2]);

         plotcounter = plotcounter +1;
       end
    end
    sgtitle("Experience number " + exp);
end
end



            
            
    
    
