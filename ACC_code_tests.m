
%run scene_creation first

%add zeroes to end of array
xplotpoints=horzcat(xplotpoints,[0 0]);
yplotpoints=horzcat(yplotpoints,[0 0]);

x=xplotpoints;
y=yplotpoints;

%hold off
%for g=1:90    
plot(x,y,'*r');
%pause(0.2)
%end
counter=0;
flag=0;
skip=0;
hold on

skipchecker=zeros(1,90);
for i=1:90
    %on previous loop, if skip is set, jump to next for iteration
    if skip==1;
        skip=0;
        continue
    end
    %do linear rsquared on current set of points being counted. 
    %If slope outside 50 degrees of horizontal (allows tracking of box with both sides at 45)
    %throw out past points by setting counter=0( must be side of object/wall)
    if counter>2
        for t=i-counter:i
           %grab values for points currently being counted
           if skipchecker(t)~=1;
           temparrayx(t)=x(t);
           temparrayy(t)=y(t);
           end
        end
        %remove zeroes and transpose
        temparrayx=temparrayx(temparrayx~=0);
        temparrayy=temparrayy(temparrayy~=0);
        temparrayx=temparrayx';
        temparrayy=temparrayy';
        %find best fit linear line and the slope
        slope=[ones(length(temparrayx),1) temparrayx]\temparrayy;
        %if slope >50 degrees (1.2) reset counter so points are not used
       if slope(2)>1.2 || slope(2)<-1.2
           counter=0;
                     
       else
       end
       temparrayx=[];
       temparrayy=[];
    end
        
    if abs(y(i+1)-y(i))<=2
        counter=counter+1;
    else
        if abs(y(i+2)-y(i))<=2 %ignore one bad data point
            counter=counter+2;
            %dont check the next point
            skip=1;
            %make note of which numbers were skipped in an array
            skipchecker(i+1)=1;
            
        else
            flag=1;
        end
    end
    %do check at end
    if i==90
        flag=1;
    end
    
    %run this when end of object is found
    if flag==1
        flag=0;
        if counter>=1 %if at least 2 units wide
            beginobj=i-counter;
            for l=beginobj:i;
                if skipchecker(l)==0;%don't add value if skipped
                objectarray(l-beginobj+1)=y(l);
                end
                %dont bother with max range points
                if sqrt(y(l)^2+x(l)^2)>125
                    counter=0;
                    continue
                end
            end
            summit=sum(objectarray);
            objectavgdis=summit/(counter+1);
            if std(objectarray)<5 %only plot with small standard deviation
            %rectangle('Position',[x(i) objectavgdis-0.5 x(beginobj)-x(i) 1],'LineWidth',4)
            plot([x(i) x(beginobj)],[objectavgdis objectavgdis],'w','LineWidth',4)
            
            else
            %rectangle('Position',[x(i) objectavgdis-1 x(beginobj)-x(i) 2])
            plot([x(i) x(beginobj)],[objectavgdis objectavgdis],'Color',[0,1,1],'LineWidth',4)
            end
            counter=0;
            clear objectarray;
         else
            counter=0;
        end
    end
    
end
   
    
