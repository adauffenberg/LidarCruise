clear
clc
hold on
axis square
axis([-110 110 -30 180])

%for xs must put smaller number first to find intersections properly

%IMAGE 1 TEST %NOTE WRONG SCALE (DONT USE)
% I=imread('testsituation1.jpg');
% hi = imagesc(-112,-40,I);
% %use between 3 and 90 row to work in xs and ys
% xs=[-35 -30; -14 -4; -26 -15;42 45;-4 -2];
% ys=[30 130; 68 67; 110 111; 30 130; 65 85];

%IMAGE 2 TEST
I=imread('testsituation2.jpg');
hi = imagesc(-110,-25,I);
%use between 3 and 90 row to work in xs and ys
xs=[-40 -35; 14 20; -16 -10; -10 -9; -32 -21; -9 -3;-3 -2.9; 5 11; 5 6; -30 -23; -23 -22.9; -18 -12];
ys=[130 30; 130 20; 22 22; 22 32; 29 29; 75 75; 75 90; 57 57; 57 71; 101 101; 101 115; 113 113];

% %IMAGE 3 TEST
% I=imread('testsituation3.jpg');
% hi = imagesc(-110,-30,I);
% %use between 3 and 90 row to work in xs and ys
% xs=[-21 -18; -29 -21; -40 -29; -10 -3;-12 -10; 5 10; 4.5 5; -2 4; -6 -2 ];
% ys=[45 15; 90 45; 130 90; 42 43; 60 43; 22 22.2; 38 22; 81 82; 100 81];

rectangle('Position',[-3 -16 6 16])
x_intersect=repmat(200,length(xs),90);
y_intersect=repmat(200,length(xs),90);
distancelist=repmat(200,length(xs),90);

for j=1:length(xs) %go through all objects listed
car1x=xs(j,:);
car1y=ys(j,:);

for i=1:90 %check each laser point
    x=[0 130*cosd(i+45.1)];
    y=[0 130*sind(i+45.1)];
    if j==1;
    plot(x,y)
    end
    
    %find polynomial that fits both lines
    p1=polyfit(car1x,car1y,1);
    p2=polyfit(x,y,1);
    
    %find where polynomials intersect
    xtemp = fzero(@(x) polyval(p1-p2,x),3);
    ytemp = polyval(p1,xtemp);
    
    %filter list based on x value, intersections outside lines don't count
    if xtemp>car1x(1) && xtemp<car1x(2)
        x_intersect(j,i) = xtemp;
        y_intersect(j,i) = ytemp;
       %plot(x_intersect(j,i),y_intersect(j,i),'r*')
    end
    
end
plot(car1x,car1y,'g','LineWidth',3);
%pause
end
%check for shadowing
for i=1:90
    for j=1:length(xs)
        %if radius is between 0 and 130 save to list of distances
        if sqrt(x_intersect(j,i)^2+y_intersect(j,i)^2)>0 && sqrt(x_intersect(j,i)^2+y_intersect(j,i)^2)<130;
            distancelist(j,i)=sqrt(x_intersect(j,i)^2+y_intersect(j,i)^2);
        end
    end
end
%find which of the objects along each laser line is closest
[M,I]=min(distancelist);
%take the closest point along each laser line (determined by the index "I") and only save that point to plot
for i=1:90
    %replace lasers that haven't hit anything with max range (130 ft)
    if x_intersect(I(i),i)~=200
        xplotpoints(i)=x_intersect(I(i),i);
    else
        xplotpoints(i)=130*cosd(i+45.1);
    end
    if y_intersect(I(i),i)~=200
        yplotpoints(i)=y_intersect(I(i),i);
    else
        yplotpoints(i)=130*sind(i+45.1);
    end
end

plot(xplotpoints,yplotpoints,'*r')

            
