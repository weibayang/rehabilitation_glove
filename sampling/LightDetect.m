%% 确定基本参数
clc
clear
warning off
ImgNum = 580;                                                               % RawPictures总数

fig = figure(1);
set(fig,'position',[150,200,800,600]);
ax = axes('position',[0,0,1,1]);
set(ax,'xtick',[],'ytick',[]);

% %% 取出前置多余图像
% 
% while isempty(x)                                                            % 读取图像，直到检测到两点，跳出循环;isempty判断是否为空
%     Order = Order + 1;                                                      % 图像序号增加
%     
%     ImgFileName = ['C:\GetPicture\' num2str(Order) '.jpg'];                   % 更新图像文件地址
%     Img = imread(ImgFileName);                                              % 读取新图像
%     
%     
%     adap_thre = max_gray - D_value;                                         % 计算灰度值的控制范围
%     Img = medfilt2(rgb2gray(Img),[5 5]);                                    % 图像均值滤波，去除噪点
%     [y,x] = find(Img > adap_thre);                                          % 找出大于最小灰度值的所有亮点
% end

%% 进行图像识别
x = [];                                                                     % 亮点x坐标
y = [];                                                                     % 亮点y坐标
Order = 1;                                                                  % 图像序号
FrameNum = 0;                                                               % 帧数
FrameTime = 0;                                                              % 图像数据时间轴
max_gray = 255;                                                         % 设置图像最大灰度
D_value = 30;                                                           % 设定灰度值的最小阈值
flag = 0;


while Order <= ImgNum
    ImgFileName = ['GetPicture\' num2str(Order) '.jpg'];                   % 更新图像文件地址
    Img = imread(ImgFileName);                                              % 读取新图像
    adap_thre = max_gray - D_value;                                         % 计算灰度值的控制范围
    ImgTemp = medfilt2(rgb2gray(Img),[5 5]);                                % 图像均值滤波，去除噪点
    [y,x] = find(ImgTemp > adap_thre);                                      % 找出大于最小灰度值的所有亮点
    
    if ~isempty(x)
        FrameNum = FrameNum + 1;
        t = 1/20;                                                               % 帧间隔时间
        FrameTime(FrameNum) = (FrameNum - 1) * t;
        
%                 imshow(ImgTemp);
        
        image(ax, Img);
        hold on;
        plot(x,y,'r+');
        hold on;
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        PointNum = zeros(1,8);
        Point = zeros(8,2);
        PointX = [];
        PointY = [];
        k = 1;
        SP_Dis = 15;
        BP_Dis = 35;
        [y I] = sort(y);                                                         %按y方向检索
        x = x(I);                                                                %按y方向检索
        for i = 2:length(x)                                                     % 检索所有亮点的情况
            Distance1 = norm([x(i)-x(i-1) y(i)-y(i-1)]);                         % 计算相邻两个小白点之间的距离
            if Distance1 < SP_Dis                                                % 当间距小于设定范围时，说明属于同一个白点
                PointNum(k) = PointNum(k) + 1;                                  % 第k个亮点区域像素累加总数
                PointX{k}(PointNum(k)) = x(i);                                  % 第k个亮点区域x像素值累加
                PointY{k}(PointNum(k)) = y(i);                                  % 第k个亮点区域y像素值累加
                Point(k,:) = [mean(PointX{k}) mean(PointY{k})];                 % 计算当前第k个白点的中心位置
            elseif Distance1 > BP_Dis                                                % 当间距大于设定范围时，说明属于另一个白点
                temp = [];
                for j1 = 1: 8                                                      %8
                    if PointNum(j1) ~= 0
                        for j2 = 1:PointNum(j1)
                            temp{j1}(j2) = norm([PointX{j1}(j2) - x(i), PointY{j1}(j2) - y(i)]);
                        end
                        Distance2(j1) = min(temp{j1});
                    else
                        Distance2(j1) = SP_Dis+1;
                    end
                end
                if Distance2(1) < SP_Dis || norm(Point(1,:)) == 0
                    k = 1;
                elseif Distance2(2) < SP_Dis || norm(Point(2,:)) == 0
                    k = 2;
                elseif Distance2(3) < SP_Dis || norm(Point(3,:)) == 0
                    k = 3;
                elseif Distance2(4) < SP_Dis || norm(Point(4,:)) == 0
                    k = 4;
                elseif Distance2(5) < SP_Dis || norm(Point(5,:)) == 0
                    k = 5;
                elseif Distance2(6) < SP_Dis || norm(Point(6,:)) == 0
                    k = 6;
                elseif Distance2(7) < SP_Dis || norm(Point(7,:)) == 0
                    k = 7;
                elseif Distance2(8) < SP_Dis || norm(Point(8,:)) == 0
                    k = 8;
                else
                    msgbox('亮点检测出错！');
                end
                PointNum(k) = PointNum(k) + 1;                                  % 第k个亮点区域像素累加总数
                PointX{k}(PointNum(k)) = x(i);                                  % 第k个亮点区域x像素值累加
                PointY{k}(PointNum(k)) = y(i);                                  % 第k个亮点区域y像素值累加
                Point(k,:) = [mean(PointX{k}) mean(PointY{k})];                 % 计算当前第k个白点的中心位置
            end
        end
        
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        %     Fix_Point = Point;                                              % 根据第一帧图像确定白点顺序
        if FrameNum == 1                                                        % 判断亮点的位置，使它们连续变化
            Fix_Point = Point;                                              % 根据第一帧图像确定白点顺序
            Fix_Point_temp = Fix_Point;
        else
            for i = 1:8  %8
                for j = 1:8  %8
                    Dist(j) = norm(Fix_Point(j,:) - Point(i,:));
                end
                [Min_Dist, Index] = min(Dist);
                Fix_Point_temp(Index,:) = Point(i,:);
            end
            Fix_Point = Fix_Point_temp;
        end
        
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        Vector1 = Fix_Point(1,:) - Fix_Point(2,:);
        Vector2 = Fix_Point(3,:) - Fix_Point(4,:);
        Vector3 = Fix_Point(5,:) - Fix_Point(6,:);
        Vector4 = Fix_Point(7,:) - Fix_Point(8,:);
        
        StandTheta(1,FrameNum) = acosd(dot(Vector1,Vector2)/(norm(Vector1)*norm(Vector2)));
        StandTheta(2,FrameNum) = acosd(dot(Vector2,Vector3)/(norm(Vector2)*norm(Vector3)));
        StandTheta(3,FrameNum) = acosd(dot(Vector3,Vector4)/(norm(Vector3)*norm(Vector4)));
        StandTheta(4,FrameNum) = atan2d(Vector4(1),Vector4(2))-atan2d(Vector1(1),Vector1(2));
        StandTheta(5,FrameNum) = StandTheta(4,FrameNum)- StandTheta(3,FrameNum)- StandTheta(2,FrameNum)- StandTheta(1,FrameNum);

        
        set(ax,'xtick',[],'ytick',[]);
        plot(ax,[Fix_Point(1,1) Fix_Point(2,1)],[Fix_Point(1,2) Fix_Point(2,2)],'g+','MarkerSize',10,'linewidth',2);hold on;          % 显示Vector1
        plot(ax,[Fix_Point(3,1) Fix_Point(4,1)],[Fix_Point(3,2) Fix_Point(4,2)],'g+','MarkerSize',10,'linewidth',2);hold on;          % 显示Vector2
        plot(ax,[Fix_Point(5,1) Fix_Point(6,1)],[Fix_Point(5,2) Fix_Point(6,2)],'g+','MarkerSize',10,'linewidth',2);hold on;          % 显示Vector3
        plot(ax,[Fix_Point(7,1) Fix_Point(8,1)],[Fix_Point(7,2) Fix_Point(8,2)],'g+','MarkerSize',10,'linewidth',2);hold on;          % 显示Vector4
        text(Fix_Point(1,1),Fix_Point(1,2),'Point1','color','y');
        text(Fix_Point(2,1),Fix_Point(2,2),'Point2','color','y');
        text(Fix_Point(3,1),Fix_Point(3,2),'Point3','color','y');
        text(Fix_Point(4,1),Fix_Point(4,2),'Point4','color','y');
        text(Fix_Point(5,1),Fix_Point(5,2),'Point5','color','y');
        text(Fix_Point(6,1),Fix_Point(6,2),'Point6','color','y');
        text(Fix_Point(7,1),Fix_Point(7,2),'Point7','color','y');
        text(Fix_Point(8,1),Fix_Point(8,2),'Point8','color','y');
        pause(0.01);
        
        hold off;
        set(fig, 'NextPlot', 'replace');
    end
    Order = Order + 1;
end
save('FrameTime','FrameTime');
save('StandTheta','StandTheta');

% SigleTheta=StandTheta(1:3,:);
% SumTheta = sum(SigleTheta);
% 
% plot(SumTheta);
      
      
      
%% 霍尔信号与图像信号对应
% clear
% clc
load('FrameTime.mat');
load('StandTheta.mat');
DIPStandTheta = StandTheta(1,:);                       %DIP角度。
PIPStandTheta = StandTheta(2,:);                       %PIP角度。
MCPStandTheta = StandTheta(3,:);                       %MCP角度。
TIPStandTheta = StandTheta(4,:);                       %TIP角度。
StandHallVoltage = readmatrix('SavedVoltage.xlsx');

DIPStandHallVoltage = StandHallVoltage(1:2,:);     %1,2为需要取的行位置。
PIPStandHallVoltage = StandHallVoltage(3:4,:);     %3,4为需要取的行位置。
MCPStandHallVoltage = StandHallVoltage(5:6,:);     %5,6为需要取的行位置。
% TIPStandHallVoltage = StandHallVoltage(1:6,:);       %指间对应所有传感器的和。

Stm32Time = readmatrix('SavedStm32Time.xlsx')/1000;  %STM32时间
% hall= StandHallVoltage;
DIPStandHallVoltage = sum(DIPStandHallVoltage);
PIPStandHallVoltage = sum(PIPStandHallVoltage);
MCPStandHallVoltage = sum(MCPStandHallVoltage);
% TIPStandHallVoltage = sum(TIPStandHallVoltage);

DIPdTdH = diff(DIPStandHallVoltage);                         %对DIP数据处理
figure
plot(DIPdTdH);
ylabel('DIPdTdH');
DIPStm32Time = Stm32Time(DIPdTdH<-0.5);
DIPStandHallVoltage = DIPStandHallVoltage(DIPdTdH<-0.5);
% hall = hall(:,dTdH<-0.5);

PIPdTdH = diff(PIPStandHallVoltage);                        %对PIP数据处理
figure
plot(PIPdTdH);
ylabel('PIPdTdH');
PIPStm32Time = Stm32Time(PIPdTdH<-0.5);
PIPStandHallVoltage = PIPStandHallVoltage(PIPdTdH<-0.5);
% hall = hall(:,dTdH<-0.5);

MCPdTdH = diff(MCPStandHallVoltage);                        %对MCP数据处理
figure
plot(MCPdTdH);
ylabel('MCPdTdH');
MCPStm32Time = Stm32Time(MCPdTdH<-0.5);
MCPStandHallVoltage = MCPStandHallVoltage(MCPdTdH<-0.5);
% hall = hall(:,MCPdTdH<-0.5);

% TIPdTdH = diff(TIPStandHallVoltage);                       %对TIP数据处理
% figure
% plot(TIPdTdH);
% ylabel('TIPdTdH');
% TIPStm32Time = Stm32Time(TIPdTdH<-0.5);
% TIPStandHallVoltage = TIPStandHallVoltage(TIPdTdH<-0.5);
% hall = hall(:,dTdH<-0.5);


DataNum = 10000;                                                                %插值个数

DIPMaxTime_min = min([max(DIPStm32Time) max(FrameTime)]);                     %对DIP关节做图像与霍尔对应
DIPMinTime_max = max([min(DIPStm32Time) min(FrameTime)]);                           
DIPTime = linspace(DIPMinTime_max,DIPMaxTime_min,DataNum);
DIPStandHallVoltage_fix = interp1(DIPStm32Time,DIPStandHallVoltage,DIPTime);
DIPStandTheta_fix = interp1(FrameTime,DIPStandTheta,DIPTime);
% hall_fix(1,:) = interp1(Stm32Time,hall(1,:),Time);
% hall_fix(2,:) = interp1(Stm32Time,hall(2,:),Time);
DIPStandHallVoltage_fix = DataFilter(DIPStandHallVoltage_fix);
DIPStandTheta_fix = DataFilter(DIPStandTheta_fix);


PIPMaxTime_min = min([max( PIPStm32Time) max(FrameTime)]);                     %对PIP关节做图像与霍尔对应
PIPMinTime_max = max([min(PIPStm32Time) min(FrameTime)]);
PIPTime = linspace(PIPMinTime_max,PIPMaxTime_min,DataNum);
PIPStandHallVoltage_fix = interp1(PIPStm32Time,PIPStandHallVoltage,PIPTime);
PIPStandTheta_fix = interp1(FrameTime,PIPStandTheta,PIPTime);
% hall_fix(1,:) = interp1(Stm32Time,hall(1,:),Time);
% hall_fix(2,:) = interp1(Stm32Time,hall(2,:),Time);
PIPStandHallVoltage_fix = DataFilter(PIPStandHallVoltage_fix);
PIPStandTheta_fix = DataFilter(PIPStandTheta_fix);

MCPMaxTime_min = min([max(MCPStm32Time) max(FrameTime)]);                             %对MCP关节做图像与霍尔对应
MCPMinTime_max = max([min(MCPStm32Time) min(FrameTime)]);
MCPTime = linspace(MCPMinTime_max,MCPMaxTime_min,DataNum);
MCPStandHallVoltage_fix = interp1(MCPStm32Time,MCPStandHallVoltage,MCPTime);
MCPStandTheta_fix = interp1(FrameTime,MCPStandTheta,MCPTime);
% hall_fix(1,:) = interp1(Stm32Time,hall(1,:),Time);
% hall_fix(2,:) = interp1(Stm32Time,hall(2,:),Time);
MCPStandHallVoltage_fix = DataFilter(MCPStandHallVoltage_fix);
MCPStandTheta_fix = DataFilter(MCPStandTheta_fix);

TIPMaxTime_min = min([max(MCPStm32Time) max(FrameTime)]);                             %对TIP关节做图像与霍尔对应,霍尔值使用MCP
TIPMinTime_max = max([min(MCPStm32Time) min(FrameTime)]);
TIPTime = linspace(TIPMinTime_max,TIPMaxTime_min,DataNum);
TIPStandHallVoltage_fix = interp1(MCPStm32Time,MCPStandHallVoltage,MCPTime);
TIPStandTheta_fix = interp1(FrameTime,TIPStandTheta,MCPTime);
% hall_fix(1,:) = interp1(Stm32Time,hall(1,:),Time);
% hall_fix(2,:) = interp1(Stm32Time,hall(2,:),Time);
TIPStandHallVoltage_fix = DataFilter(TIPStandHallVoltage_fix);
TIPStandTheta_fix = DataFilter(TIPStandTheta_fix);

figure
plot(DIPStandHallVoltage_fix(1,:), DIPStandTheta_fix(1,:));
xlabel('Hall Voltage(V)');
ylabel('DIP Angle(°)');
title('Theta1')


figure
plot(PIPStandHallVoltage_fix(1,:), PIPStandTheta_fix(1,:));
xlabel('Hall Voltage(V)');
ylabel('PIP Angle(°)');
title('Theta2')

figure
plot(MCPStandHallVoltage_fix(1,:), MCPStandTheta_fix(1,:));
xlabel('Hall Voltage(V)');
ylabel('MCP Angle(°)');
title('Theta3')


save('DIPStandHallVoltage_fix','DIPStandHallVoltage_fix');
save('DIPStandTheta_fix','DIPStandTheta_fix');

save('PIPStandHallVoltage_fix','PIPStandHallVoltage_fix');
save('PIPStandTheta_fix','PIPStandTheta_fix');

save('MCPStandHallVoltage_fix','MCPStandHallVoltage_fix');
save('MCPStandTheta_fix','MCPStandTheta_fix');

save('TIPStandTheta_fix','TIPStandTheta_fix');
save('TIPTime','TIPTime');