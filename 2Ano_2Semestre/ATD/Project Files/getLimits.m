function [] = getLimits(exp, exp1, exp2, user_number, labels,fs,activitiesID)

axis = ["X", "Y", "Z"];

data = [exp1; exp2];

allFreqs = maxFreqs(data(1:length(exp1),:),exp,labels,activitiesID,fs,[],7);
allFreqs = [allFreqs;maxFreqs(data(1+length(exp1): length(exp1)+length(exp2),:),exp + 1,labels,activitiesID,fs,[],7)];



for i=1:length(activitiesID)
    for k = 1:3
        aux =[];
        for j=1:size(allFreqs,1)
        
            if(allFreqs(j,4) == i)
                aux = [aux, allFreqs(j,k)];
            end
        end
        
        max_value = max(aux);
        min_value = min(aux);
        disp("Limits for " + activitiesID{1,i} + " in axis " + axis(k) + ": [" + min_value + "; " + max_value +"]");
    end
    
    
end
    
    
