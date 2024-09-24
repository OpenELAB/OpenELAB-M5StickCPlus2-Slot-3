&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;__Hi👋__，🎉 Congratulations on coming to the last step of your slot machine project! In this step, we'll take you step-by-step to implement a super cool feature: changing a manual stop to an automatic stop! ✨ Not only that, but we'll also show you how to implement a one-button switching feature through macro definitions, so that you can easily switch between auto and manual modes by just modifying a single number! 🎮 It's as easy as magic, making your projects more flexible and fun! 🔄
Ready? Let's do it together! 👾  

# M5StickCPlus2_Slot Project
## Project structure
``` 
│── README.md             # Project description file
│── M5StickCPlus2_slot    # Source code folder
  │── M5StickCPlus2_slot.ino   # Source Code Files
  │── Slot.cpp                 # Slot Function Implementation File
  │── SLot.h                   # Slot function definition file
  │── image                    # Image folder
    │── Ghostface.h                # Ghostface icons
    │── Happy.h                    # Happy icon
    │── Kiss.h                     # Kiss icon
    │── Spectre.h                  # Spectre logo
    │── Think.h                    # Think icon
    │── Vomit.h                    # Vomit icon
    │── slot_symbols.h             # Icon Data
```
## Installation and operation

### precondition
Software dependency: __Arduino IDE__, __VScode__ or __text__, etc.  
Hardware requirements: __USB-C cable__, __M5StickCPlus2__, etc.  
Dependencies: __M5StickCPlus2 library__, __Arduino library__, etc.  

### Installation of dependencies
1、这次的改动主要主要是两部分，首先我们先讲第一部分，在上节的基础上更改的部分如下，我们从上至下来一一分析，首先我们定义了Automation，自动与非自动的开关。Time_Max,Time_MIN,表示停止时间的选取区间。InitTime_MAX,InitTime_MIN,表示初始停止时间的区间选取。StartCount_MAX,距离开始已经停止的最大数。
2、接下来我们看setup(),我们添加一条srand(time(0)),给随机函数添加参数为time，在看loop()函数，首先利用Automation作为开关判断要不要自动，非自动并不需要更改，我们主要来看自动。在此段自动中主要是状态的判断是否需要停止。
```
#include "time.h"
#define Automation 1
#define Time_MAX 80
#define Time_MIN 45
#define InitTime_MAX 100
#define InitTime_MIN 80
#define StartCount_MAX 6
void setup() {
	srand(time(0));//在原有几的基础上添加的
}
void loop()  {
  static int Slot_Start = 0;
  static int Slot_Stop = 0;//判断转盘是否正在转动
  static int Count_Stop = 0;
  static int Count_Num;
  if(Automation){    //自动
  	M5.update();
  	if (M5.BtnA.wasPressed()||Slot_Stop == 1) {
  		if (state == SLOTS_INIT) {
  			for (int i = 0; i < SLOT_COUNT; i++) {
  				slots[i].start();
  			}
  			state++;
        		Slot_Start = 1;//已经开始了
        		Count_Num = rand() % (InitTime_MAX - InitTime_MIN - 1) + InitTime_MIN;//获取随机值
  		} else if (state < SLOTS_STOP) {          
        		Slot_Start++;//开始并且停止一列了后面与StartCount_MAX进行比较
  			slots[state - 1].stop();    
        		state++;
  		}
  	}
    Slot_Stop = 0;//每次都重新归0，用于让随机值用于随机停止
  }else{//非自动
    M5.update();
    if (M5.BtnA.wasPressed()) {
      if (state == SLOTS_INIT) {
        for (int i = 0; i < SLOT_COUNT; i++) {
          slots[i].start();
        }
        state++;
      } else if (state < SLOTS_STOP) {
        slots[state - 1].stop();
        state++;
      }
    }  
  }
}
```
3、接下来我们看第二部分，该部分主要是针对于使得每列随机停止，该段在loop结尾处，不可改变位置，首先我们只有自动模式才需要此段代码。
```
 if(Automation){//自动
   if(Slot_Start < StartCount_MAX){//用于判断是否还有没停止的列
    Count_Stop++;
    if((Count_Stop == Count_Num)&&state < SLOTS_STOP){//当该Count_Stop等于随机数Count_Num，就表示可以停止了，表示随机延时已经结束了。但是需要确保当前状态是转动的
      Slot_Stop = 1;//真正的停止信号
      Count_Stop = 0;//归0 初始化
      Count_Num = rand() % (Time_MAX - Time_MIN - 1) + Time_MIN;//为下一次取随机数
    }
   }else if(Slot_Start == StartCount_MAX){//当所有列已经停止了，那么归0，重新开始新的轮回
     Slot_Start = 0;//初始化归0
   }
 }
```
### compile and run
1、After completing the installation of the dependencies, open the good downloaded zip archive

![QQ_1726107516108](https://github.com/user-attachments/assets/cb2362f7-1871-418e-94dd-92ddfe7284b7)  

2、Connect the Plus2 to your computer using USB-C, select Tools->Port to choose your own port

![QQ_1726107673971](https://github.com/user-attachments/assets/17f0392a-b753-4aba-946c-ede75ba9092f)  

3、Click on compile and then click on upload when the compilation is complete

![QQ_1726107957719](https://github.com/user-attachments/assets/c1f953ad-5355-44e8-af0c-ac5da7542aa6)  


## How to contact the maintainer or developer
__OpenELAB:__   
[![OpenELAB](https://private-user-images.githubusercontent.com/180402004/366379735-e03723c2-c213-421f-a517-e482f67f3660.png?jwt=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJnaXRodWIuY29tIiwiYXVkIjoicmF3LmdpdGh1YnVzZXJjb250ZW50LmNvbSIsImtleSI6ImtleTUiLCJleHAiOjE3MjYwNDU2NTIsIm5iZiI6MTcyNjA0NTM1MiwicGF0aCI6Ii8xODA0MDIwMDQvMzY2Mzc5NzM1LWUwMzcyM2MyLWMyMTMtNDIxZi1hNTE3LWU0ODJmNjdmMzY2MC5wbmc_WC1BbXotQWxnb3JpdGhtPUFXUzQtSE1BQy1TSEEyNTYmWC1BbXotQ3JlZGVudGlhbD1BS0lBVkNPRFlMU0E1M1BRSzRaQSUyRjIwMjQwOTExJTJGdXMtZWFzdC0xJTJGczMlMkZhd3M0X3JlcXVlc3QmWC1BbXotRGF0ZT0yMDI0MDkxMVQwOTAyMzJaJlgtQW16LUV4cGlyZXM9MzAwJlgtQW16LVNpZ25hdHVyZT1mOWE1NGE1ZDhhOTNhMjJmNTRmZmMxYTk1YzA3MDNmNTY5MzQ1NGZkNDEzMDk4OTM4MTdjM2I1ZGNmMDYzODY2JlgtQW16LVNpZ25lZEhlYWRlcnM9aG9zdCZhY3Rvcl9pZD0wJmtleV9pZD0wJnJlcG9faWQ9MCJ9.qnnW0ggDBwf9vP7yfAQU-oMGHnGr0-FJKsb38NDInco)](https://openelab.io)  
__YouTube:__  
[![youtube_logo_200x150](https://private-user-images.githubusercontent.com/180402004/366385591-b212d544-4584-49ac-970d-811a87f367bc.png?jwt=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJnaXRodWIuY29tIiwiYXVkIjoicmF3LmdpdGh1YnVzZXJjb250ZW50LmNvbSIsImtleSI6ImtleTUiLCJleHAiOjE3MjYwNDY1MzAsIm5iZiI6MTcyNjA0NjIzMCwicGF0aCI6Ii8xODA0MDIwMDQvMzY2Mzg1NTkxLWIyMTJkNTQ0LTQ1ODQtNDlhYy05NzBkLTgxMWE4N2YzNjdiYy5wbmc_WC1BbXotQWxnb3JpdGhtPUFXUzQtSE1BQy1TSEEyNTYmWC1BbXotQ3JlZGVudGlhbD1BS0lBVkNPRFlMU0E1M1BRSzRaQSUyRjIwMjQwOTExJTJGdXMtZWFzdC0xJTJGczMlMkZhd3M0X3JlcXVlc3QmWC1BbXotRGF0ZT0yMDI0MDkxMVQwOTE3MTBaJlgtQW16LUV4cGlyZXM9MzAwJlgtQW16LVNpZ25hdHVyZT0yNDA0OTllOWFiZTk3ZGM4YjUxMGYxOGFkNmViODYxMTk1YWQ1Nzc0MTdlZDRiNWZjZmI0NjU5ZDg1NmIwMzcxJlgtQW16LVNpZ25lZEhlYWRlcnM9aG9zdCZhY3Rvcl9pZD0wJmtleV9pZD0wJnJlcG9faWQ9MCJ9.A9yqUAA24OPl3lSMY1O71Oa8Jn33ICzTJqlOhf4altc)](https://www.youtube.com/@OpenELAB)  
__X :__  
[ ![X_logo_150x150](https://private-user-images.githubusercontent.com/180402004/366386947-f22637e9-361e-4889-99c5-a940dae19ca8.png?jwt=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJnaXRodWIuY29tIiwiYXVkIjoicmF3LmdpdGh1YnVzZXJjb250ZW50LmNvbSIsImtleSI6ImtleTUiLCJleHAiOjE3MjYwNDY3MDcsIm5iZiI6MTcyNjA0NjQwNywicGF0aCI6Ii8xODA0MDIwMDQvMzY2Mzg2OTQ3LWYyMjYzN2U5LTM2MWUtNDg4OS05OWM1LWE5NDBkYWUxOWNhOC5wbmc_WC1BbXotQWxnb3JpdGhtPUFXUzQtSE1BQy1TSEEyNTYmWC1BbXotQ3JlZGVudGlhbD1BS0lBVkNPRFlMU0E1M1BRSzRaQSUyRjIwMjQwOTExJTJGdXMtZWFzdC0xJTJGczMlMkZhd3M0X3JlcXVlc3QmWC1BbXotRGF0ZT0yMDI0MDkxMVQwOTIwMDdaJlgtQW16LUV4cGlyZXM9MzAwJlgtQW16LVNpZ25hdHVyZT1hNjgzMjVjZmQwOTk2ZTA5ZWJmYmUzOWNjZjQwODUzYmI2YzUzMDk0ZDljMjlmOWE2ZmUyYTczMmM5MWNiMjNlJlgtQW16LVNpZ25lZEhlYWRlcnM9aG9zdCZhY3Rvcl9pZD0wJmtleV9pZD0wJnJlcG9faWQ9MCJ9.Xl_OOo_alC9lAL1wOYwLariKbjmPQba5MO6agG-ECmQ)
](https://twitter.com/openelabio)  
__FaceBook:__  
[![facebook_logo_cropped_150x150](https://private-user-images.githubusercontent.com/180402004/366389274-b03aa857-c5d9-4bed-b65c-ddbd0e674111.png?jwt=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJnaXRodWIuY29tIiwiYXVkIjoicmF3LmdpdGh1YnVzZXJjb250ZW50LmNvbSIsImtleSI6ImtleTUiLCJleHAiOjE3MjYwNDcwMjgsIm5iZiI6MTcyNjA0NjcyOCwicGF0aCI6Ii8xODA0MDIwMDQvMzY2Mzg5Mjc0LWIwM2FhODU3LWM1ZDktNGJlZC1iNjVjLWRkYmQwZTY3NDExMS5wbmc_WC1BbXotQWxnb3JpdGhtPUFXUzQtSE1BQy1TSEEyNTYmWC1BbXotQ3JlZGVudGlhbD1BS0lBVkNPRFlMU0E1M1BRSzRaQSUyRjIwMjQwOTExJTJGdXMtZWFzdC0xJTJGczMlMkZhd3M0X3JlcXVlc3QmWC1BbXotRGF0ZT0yMDI0MDkxMVQwOTI1MjhaJlgtQW16LUV4cGlyZXM9MzAwJlgtQW16LVNpZ25hdHVyZT1lOWUyMDA4ZThlMmU3N2Y5NzE5NDVlOGMwNTRlNTcwODA0NmZjZGU4M2ZhYmNhYWQ0ZTA1NTVkNTE4Yjk0YzQwJlgtQW16LVNpZ25lZEhlYWRlcnM9aG9zdCZhY3Rvcl9pZD0wJmtleV9pZD0wJnJlcG9faWQ9MCJ9.magjk7X-wGsifZ7FlRZfq5J4-EoMdjJZsNK5e0g4CXQ)](https://www.facebook.com/profile.php?id=61559154729457)  
__Discord__  
[ ![resized_image_150x150](https://private-user-images.githubusercontent.com/180402004/367483377-2d6baa26-ae89-46f9-a91d-a59316fbc007.png?jwt=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJnaXRodWIuY29tIiwiYXVkIjoicmF3LmdpdGh1YnVzZXJjb250ZW50LmNvbSIsImtleSI6ImtleTUiLCJleHAiOjE3MjYyODMxOTUsIm5iZiI6MTcyNjI4Mjg5NSwicGF0aCI6Ii8xODA0MDIwMDQvMzY3NDgzMzc3LTJkNmJhYTI2LWFlODktNDZmOS1hOTFkLWE1OTMxNmZiYzAwNy5wbmc_WC1BbXotQWxnb3JpdGhtPUFXUzQtSE1BQy1TSEEyNTYmWC1BbXotQ3JlZGVudGlhbD1BS0lBVkNPRFlMU0E1M1BRSzRaQSUyRjIwMjQwOTE0JTJGdXMtZWFzdC0xJTJGczMlMkZhd3M0X3JlcXVlc3QmWC1BbXotRGF0ZT0yMDI0MDkxNFQwMzAxMzVaJlgtQW16LUV4cGlyZXM9MzAwJlgtQW16LVNpZ25hdHVyZT0zYTU2OGM4N2RlNDk0NzRjNzgzMWFhOWViZDllMzcxM2U4NzU2YTliYjRhMDhhYzY5OTJmN2UzZTJhOWM4MTA2JlgtQW16LVNpZ25lZEhlYWRlcnM9aG9zdCZhY3Rvcl9pZD0wJmtleV9pZD0wJnJlcG9faWQ9MCJ9.Lst8WNRwCLuw2jWENd6e4jONWVMDrBicGpBKFo7C3wk)](https://discord.gg/VQspWyck)

__The source code was adapted from__
[M5StickCPlus](https://github.com/Sarah-C/M5StickC_Plus_Slot_Machine)

