function [A]=normal

% Reading image files from the directory 
file1=dir('*.jpg');
num=length(file1);
for n=1:num
  img{n} = imread(file1(n).name);
end

% Reading text files that contain the facial features
file=dir('*.txt');
num=length(file);
for n=1:num
    f{n} = load(file(n).name);
end

% J is predetermined facial features
J=[13 20; 50 20; 34 34; 16 50; 48 50];
% M is Fbar
M=f{1};
flag=true;
ft=zeros(5,2);
X=0;

while flag
    fto=ft;
%   findt function finds and applies the affine transformation
    A=findt(M,J);
    M=A;
    sum=zeros(5,2);
%   For each image we compare it with Fbar and apply the transformation
    for a=1:num
        A=findt(f{a},M);
        sum=sum+A;
    end
%   ft is the final average facial features
    ft=sum/num;
%   Checking for error between two consecutive ft, if lesser than 10^-6
%   stop running the while loop
    if max(max(abs(ft-fto)))<0.000001
        flag=false;
    end
    M=ft;
    X=X+1;
end

%  Image normalization
for a=1:num
    q=rgb2gray(img{a});
    t=transform(f{a},ft);
%   transform finds the affine transform only
    tran=maketform('affine',t);
%   Image is normalized and cropped to 64 x 64 window
    image=imtransform(q,tran,'XData',[1 64],'YData',[1 64]);
%   Saving the normalized image
    path=strcat('./normalized/',file1(a).name);
    imwrite(image,path,'jpg');
end
            
            
end
