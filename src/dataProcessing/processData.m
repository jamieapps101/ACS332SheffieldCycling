clear
figure(1)
clf

nFolders = 100;
nFiles = 70;
folderName = "output";
fileName = "output_";
refName = "outputReference.csv";
total = 0;
target = 0.02; % make this range from 0.7 to 1.7 over first 10 years of daya (ie 520 samples), then only sample that for rmsea = 1:(2-1)/5:2 
targets = 0.007:((0.017-0.007)/(52*10)):0.017;
targets = targets*1000;
resultsdata = [0 0 0 1 2 3 4 5 6 7 8 9 10 11 12];

processDataMode = 0;
sortDataMode = 1;

if processDataMode == 1
	for folderN = 1:nFolders
	   refPath = folderName + num2str(folderN) + "/" + refName
	   dataRef = csvread(refPath,1,0);
	   for fileN = 1:nFiles
	       if fileN == 1
		   path = folderName + num2str(folderN) + "/" + "output.txt";
	       else
		   path = folderName + num2str(folderN) + "/" + fileName + num2str(fileN) + ".txt";
	       end
	       % open file here and process it
	       data = csvread(path,1,0);
	       sampleValues = data(:,2);
           targetsSize = size(targets);
	       rmse = (mean((targets.' - sampleValues(targetsSize(2),:)).^2))^0.5; %rmse for that file
	       weights = dataRef(fileN,2:13); 
	       result = [folderN fileN rmse weights];
	       resultsdata = [resultsdata; result];
	   end
	end
	csvwrite("rmseData.csv", resultsdata);
else
	resultsdata = csvread("rmseData.csv",1,0);
end

if sortDataMode == 1
    sortedResults = sortrows(resultsdata,3);
    xLength = size(sortedResults);
    x = 1:xLength;
    y = sortedResults(:,3);
    figure(1)
    scatter(x, y')
    hold on
    grid minor
    
    topWeightSizes = [0.005 0.01 0.05 0.1];
    b = size(topWeightSizes);
    for a = 1:b(2)
        topResultsPortion = topWeightSizes(1,a)
        topResultIndex = round(xLength(1)*topResultsPortion)
        topResults = sortedResults(1:topResultIndex,:);
        csvwrite("topResultsRmseData" + num2str(a) + ".csv", topResults);
    end
end



