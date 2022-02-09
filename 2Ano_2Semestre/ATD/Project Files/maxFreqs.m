
function [allFreqs] = maxFreqs(input_data,exp,labels,activitiesID,fs, activities_to_consider, n_frame)


%where the windows overlap
n_overlap = n_frame/2;

% number of frames in the window (150)
number_per_frames= round(n_frame*fs);

%number of frames overlaped (75)
number_per_frames_overlaped = round(n_overlap*fs); 

allFreqs = [];



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





   
    
    %Go through all the activities with the same id
    for j=1:length(edges(:,1))
        
        this_iteration = [];
        %Go through their axis
        for k=1:1:3

            most_relevant_freq = [];
            number_elements =edges(j,2) - edges(j,1);

            data2 = detrend(input_data(edges(j,1): edges(j,2), k)); 

 
            %Now we apply the hamming window (previously seen has the
            %better window, to get the max frequence value, store it
            %and later use the value to plot it in a 3D enviromment

            %means the window is bigger that the data avaiable
            if(number_elements - number_per_frames <1)

                hamming_w = hamming(numel(data2));
                frame = data2.*hamming_w;

                [frequencies_frame,dft2] = dftCalc(frame, fs);
                
             
                
                %Get the max value
                [maxMag, ind] = max(dft2);
                most_relevant_freq = [most_relevant_freq, abs(frequencies_frame(ind))];

       

            %means that the window is not bigger that the avaiable data
            else
                hamming_w = hamming(number_per_frames);

                step = number_per_frames - number_per_frames_overlaped;
                final = number_elements - number_per_frames;

                for l = 1:step:final
                    frame = data2(l:l+number_per_frames-1).*hamming_w;
                    [frequencies_frame,dft2] = dftCalc(frame,50);
                      
                    [maxMag, ind] = max(dft2);
                    most_relevant_freq = [most_relevant_freq, abs(frequencies_frame(ind))];
                    

                    % We now store the max 
                    %X axis
                end
            end
            %We add the axis values to the current iteration
            this_iteration = [this_iteration,most_relevant_freq.']; 
        end
        %We add the lines that where created this iteration. We also have a
        %identifier in position 4 of the array telling us which position of the
        %array it is now going to take
        this_iteration = [this_iteration, ones(size(this_iteration,1),1)*i];
        allFreqs = [allFreqs;this_iteration];
    end

end
end
