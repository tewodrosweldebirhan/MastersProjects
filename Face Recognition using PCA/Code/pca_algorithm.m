
% Implementation of PCA Algorithm
function [D_old, Phi, Ft] = pca_algorithm(kval)


%Create training data matrix D
faceImgs = dir('train_images\*.jpg'); 
numfiles = length (faceImgs);

% kval = 80; %
d = 64*64; %size of the image 
p = numfiles; %no of images
D = zeros(p,d);

trainpath = 'train_images\';
for k = 1: numfiles 
  str = faceImgs(k).name;
  
  trainimgname = strcat(trainpath,str);
  Iface = imread (trainimgname);
%   Iface = rgb2gray(Iface);
  X1 = reshape(Iface',1,[]); %convert it to row vector
  D(k,:) = X1;  
end

%calculate the mean of D
Xmean = mean((D')); 
Xmean = Xmean';

D_old = D;
D = D - Xmean;

%covariance matrix
Cov_1 = (D' * D)./(p-1);

%Eigen values and eigenvectors
[EgVec,DiagV] = eig(Cov_1);

%sort them
[Dvalues, ind] = sort(diag(DiagV),'descend');
EgnVector = EgVec(:,ind);

% k principal components
PComp = EgnVector(:,1:kval);

% projection matrix(d by k), phi
Phi = PComp;

% change each image to pca space
% feature vectors
Ft = zeros(p,kval);

for m=1:p
   x1 = D_old(m,:);
   f1 = x1 * Phi;
   Ft(m,:) = f1;
end

end
