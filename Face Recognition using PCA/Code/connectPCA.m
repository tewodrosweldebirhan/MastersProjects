%Connection to GUI
function [I_matched, I_matched2, I_matched3,indexMin] = connectPCA(I_test)
    kvalue = 80;
    [Dmatrix, projMatrix, featureM] = pca_algorithm(kvalue);
    x_test = double(reshape(I_test',1,[]));    

    %calculate the feature vector
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
    
    % indM = find(Lt == indexMin);
    
    V_matched = Dmatrix(indexMin,:);
    I_matched = vec2mat(V_matched,64);
    
    V_matched2 = Dmatrix(indexSort(2),:);
    I_matched2 = vec2mat(V_matched2,64);
    % I_matched3 = I_matched;
    V_matched3 = Dmatrix(indexSort(3),:);
    I_matched3 = vec2mat(V_matched3,64);
end