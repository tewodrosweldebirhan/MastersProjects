
% Testing all test images and calculating the accuracy
%-----------------------------------------------------
pcaerror = 0;
faceImgs_ = dir('test_images\*.jpg'); 
numfiles_ = length (faceImgs_);
lbl = 1;
oldFN='';
kvalue=30
[Dmatrix, projMatrix, featureM] = pca_algorithm(kvalue);
trainpath_ = 'test_images\';
for k = 1: numfiles_
    str_ = faceImgs_(k).name;
    testingimgname = strcat(trainpath_,str_);
    
    % labeling the images
    lenstr = length(str_);
    familyIndex = str2num(str_(lenstr-5:lenstr-4));  
    
    familyName = str_(1:lenstr-5); 
    if(k == 1)
        oldFN = familyName;
    elseif(length(oldFN) == length(familyName))
      if(oldFN ==  familyName)
          
      else
          lbl = lbl + 1; 
      end          
    else
      lbl = lbl + 1;
    end
    
    oldFN = familyName;
    I_test = imread (testingimgname);
    
    %---------------------------------
    x_test = double(reshape(I_test',1,[]));    
    
    %calculate the feature vector of test
    f_test = x_test * projMatrix;
    p=99; % total number of train images
    %compute the Euclidean distance  
    Eucl_distance = ones(p,1);
    
    for n=1:p
        f_training = featureM(n,:);
        Eucl_distance(n) = norm(f_training - f_test);
    end
    
    %find the minimum distance
    [sortedDist, indexSort] = sort(Eucl_distance);
    indexMin = indexSort(1); %index of minimum distance        
    
    V_matched = Dmatrix(indexMin,:);
    I_matched = vec2mat(V_matched,64);
    
    V_matched2 = Dmatrix(indexSort(2),:);
    I_matched2 = vec2mat(V_matched2,64);
    
    V_matched3 = Dmatrix(indexSort(3),:);
    I_matched3 = vec2mat(V_matched3,64);
%----------------------------------------------------
    % check if it matches
    modv = mod(indexMin,3);
    if(modv == 0)
       matchInd = indexMin/3;
    elseif(modv == 1)
        matchInd = (indexMin+2)/3;
    elseif(modv == 2)
        matchInd = (indexMin+1)/3;
    end    
    
    
    if(lbl ~= matchInd)
        pcaerror= pcaerror + 1;
    end
    Display the figure
    figure,
    subplot(2,2,1), imshow(I_test)
    title('Original Image')
    subplot(2,2,2), imshow(I_matched,[])
    title('1st Matched Image')
    subplot(2,2,3), imshow(I_matched2,[])
    title('2nd Matched Image')
    subplot(2,2,4), imshow(I_matched3,[])
    title('3rd Matched Image')
end
pca_accuracy = (1 - double(pcaerror/numfiles_))*100;
pca_accuracy

