%Clear previous data
clear;
clc;

% Load the video in a MP4 object
xyloObj = VideoReader('bad apple15.mp4');

% Read the information about this MP4
nFrames = xyloObj.NumberOfFrames;
vidHeight = xyloObj.Height;
vidWidth = xyloObj.Width;

% Preallocate movie structure.
mov(1:nFrames) = struct('cdata', zeros(vidHeight, vidWidth, 3, 'uint8'), 'colormap', []);
mov1(1:nFrames) = struct('cdata', zeros(vidHeight, vidWidth, 3, 'uint8'), 'colormap', []);

% Define a matrix about the point way
PointData = zeros(31,4000,'uint8');
PointDataX = zeros(31,4000,'uint8');
PointDataY = zeros(31,4000,'uint8');

%This value is used to show the process
BWProcess = 0;

% Read one frame at a time.
for k = 1 : nFrames
    im = read(xyloObj, k);

    % here we process the image im
    i1=rgb2gray(im);
    i2=im2bw(i1);
    BW=edge(i2,'canny');
    [row,line]=size(BW);
    BW1=BW(1:4:row,1:4:line);
%     I2 = bwmorph(i2, 'thin',Inf);
    mov(k).cdata = BW;
    BW2=rot90(BW,3);
    mov1(k).cdata = BW2;
%     mov1(k).cdata = BW1;
    clc;
    BWProcess=BWProcess+1
end

%Define a count value for frame
CountFrame = 1;
CountFrameClt = 1;
CountFrameAxis = 1;
CountFrameCltAxis = 1;
CountFrameGet = 1;
CountFrameGetX = zeros(80,80,'double');
CountFrameGetY = zeros(80,80,'double');
CountFrameMovieGetX = zeros(80,80,'double');
CountFrameMovieGetY = zeros(80,80,'double');
CountFrameMovieGet = zeros(80,80,'double');
CountFrameMovieGet8bit = zeros(74,20000,'uint8');
CountFrameMax = 0;
CountFramePreMax = 0;
CountFrameLineGetFlag=0;

%This value is used to show the process about finding the output point
PointFindProcess = 0;

%Find the Point which the DAC should output
for k=1: nFrames
    BK=mov1(k).cdata;
    
    PointData(CountFrameClt,CountFrame)=255;
    PointDataX(CountFrameCltAxis,CountFrameAxis)=255;
    PointDataY(CountFrameCltAxis,CountFrameAxis)=255;
    if k <= 4200
        
        CountFrameMovieGet8bit(CountFrameClt,CountFrame)=255;
    end

    CountFrame=CountFrame+1;
    CountFrameAxis=CountFrameAxis+1;
    
    if CountFrame == 20001
        CountFrameClt=CountFrameClt+1;
        CountFrame=1;
    end
    
    
    CountFramePreMax = 0;
    
    for i=1:line

        CountFrameLineGetFlag=0;
        for j=1:row
            if (BK(i,j) == true)
                CountFramePreMax=CountFramePreMax+1;
                
                PointData(CountFrameClt,CountFrame)=i;
                
                PointDataX(CountFrameCltAxis,CountFrameAxis)=i;
                if(CountFrameLineGetFlag==0)
                    if k <= 4200
                        
                        
                        if(CountFrame==1)
                           if(CountFrameMovieGet8bit(CountFrameClt-1,20000)~=255)
                            CountFrameMovieGet8bit(CountFrameClt-1,20000)=CountFrameMovieGet8bit(CountFrameClt-1,20000)+96;
                           end
                        else
                            if(CountFrameMovieGet8bit(CountFrameClt,CountFrame-1)~=255)
                            CountFrameMovieGet8bit(CountFrameClt,CountFrame-1)=CountFrameMovieGet8bit(CountFrameClt,CountFrame-1)+96;
                            end
                        end
                        
                        CountFrameMovieGet8bit(CountFrameClt,CountFrame)=i;    
                        CountFrame=CountFrame+1;
                        CountFrameLineGetFlag=1;

                    end
                end
                
%                 CountFrame=CountFrame+1;
                
                if CountFrame == 20001
                    CountFrameClt=CountFrameClt+1;
                    CountFrame=1;
                end
                
                if CountFrameAxis == 20001
                    CountFrameCltAxis=CountFrameCltAxis+1;
                    CountFrameAxis=1;
                end
                    
                PointData(CountFrameClt,CountFrame)=j;
                
                PointDataY(CountFrameCltAxis,CountFrameAxis)=j;
                
                if k <= 4200
                    
                    CountFrameMovieGet8bit(CountFrameClt,CountFrame)=j;
                    
                end
                

                CountFrame=CountFrame+1;
                
                if CountFrame == 20001
                    CountFrameClt=CountFrameClt+1;
                    CountFrame=1;
                end
                
                

                
%                 if k <= 220
%                     CountFrameGetX(k,CountFrameGet)=i;
%                     CountFrameGetY(k,CountFrameGet)=j;
%                     CountFrameGetX(k,CountFrameGet)=round(CountFrameGetX(k,CountFrameGet)*32/4096*4095);
%                     CountFrameGetY(k,CountFrameGet)=round(CountFrameGetY(k,CountFrameGet)*32/4096*4095);
%                     
%                     CountFrameGet=CountFrameGet+1;
%                 
%                 end
                
            end
        end

    end
    clc;
    PointFindProcess=PointFindProcess+1
    CountFrameMax=max(CountFrameMax,CountFramePreMax);
end

FileCount = 0;

%创建文件
fid = fopen('data2200.txt','wt');  
%写头部
fprintf(fid,'%s','wo ni ta');
fprintf(fid,'%c\n',' ');    %换行


%依次写入数据
fprintf(fid,'%c','{');
fprintf(fid,'%c\n',' ');    %写完一行,换行
fprintf(fid,'%c\n',' ');    %写完一行,换行
for k=1:74; 
    fprintf(fid,'%c','{');
    for m=1:20000
        p=num2str(CountFrameMovieGet8bit(k,m));
        fprintf(fid,'%s ,',p);    %每个数据用空格隔开
    end
    clc;
    FileCount=FileCount+1
    fprintf(fid,'%c','}');
    fprintf(fid,'%c\n',' ');    %写完一行,换行
    fprintf(fid,'%c\n',' ');    %写完一行,换行
    fprintf(fid,'%c\n',' ');    %写完一行,换行
end
fprintf(fid,'%c\n',' ');    %写完一行,换行
fprintf(fid,'%c\n',' ');    %写完一行,换行
fprintf(fid,'%c','}');
fclose(fid); %关闭文件




% Size a figure based on the video's width and height.

% hf = figure;
% set(hf, 'position', [150 150 vidWidth vidHeight])

% Play back the movie once at the video's frame rate.

% movie(hf, mov, 1, xyloObj.FrameRate);
% movie(hf, mov, 1, 29.97);