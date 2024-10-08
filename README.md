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
1, this change is mainly two parts, first of all, we first talk about the first part, in the previous section on the basis of the changes in the following parts, we analyze one by one from the top to the next, first of all, we defined Automation, automatic and non-automatic switches. time_Max, Time_MIN, said to stop the time of the selection of the interval. initTime_MAX, initTime_MIN, said the initial stop time of the interval selection. startCount_MAX, the maximum number of stops from the beginning. InitTime_MAX, InitTime_MIN, indicates the interval for selecting the initial stop time, StartCount_MAX, the maximum number of stops from the start.  

2、Next we look at setup (), we add a srand (time (0)), to add a random function parameter for time, in the look at the loop () function, first of all, the use of Automation as a switch to determine whether the automatic, non-automatic does not need to be changed, we mainly look at the automatic. In this section of the automatic is mainly the state of the judgment whether to stop.  In automatic mode, the spindle will stop gradually according to the randomly generated stopping time Count_Num, without the need for the user to press the button several times. BtnA is pressed or the counter reaches the stop condition, the spool will stop automatically one by one. In state SLOTS_INIT, all spindles are started at once, and then a randomly generated count determines when to stop.
The spools stop automatically when button A is pressed or when the random counter reaches the stop time.
```
#include "time.h"
#define Automation 1
#define Time_MAX 80
#define Time_MIN 45
#define InitTime_MAX 100
#define InitTime_MIN 80
#define StartCount_MAX 6

void setup() {
	srand(time(0));//added on top of the original few
}

void loop()  {
  static int Slot_Start = 0; // Control the number of spindle starts
  static int Slot_Stop = 0; // Whether to stop the spindle
  static int Count_Stop = 0; //Count the heartbeats of the stop spindle
  static int Count_Num; // Randomly generated stop times in automatic mode

  M5.update(); //Update button status

  if (Automation) { // If automatic mode is enabled
    if (M5.BtnA.wasPressed() || Slot_Stop == 1) { // button press or auto mode trigger stop
      if (state == SLOTS_INIT) { // If in the initialization state
        for (int i = 0; i < SLOT_COUNT; i++) {
          slots[i].start(); // Activate each spindle
        }
        state++;
        Slot_Start = 1; // Record startup status
        Count_Num = rand() % (InitTime_MAX - InitTime_MIN - 1) + InitTime_MIN;// Randomly generated stop times
      } else if (state < SLOTS_STOP) { // If the spindle is running
        Slot_Start++;
        slots[state - 1].stop(); //Stop the current axis
        state++;
      }
    }
    Slot_Stop = 0; // Reset stop sign
  } else { // manual mode
    if (M5.BtnA.wasPressed()) { //Press the button to start or stop the spindle
      if (state == SLOTS_INIT) {
        for (int i = 0; i < SLOT_COUNT; i++) {
          slots[i].start(); // Activate each spindle
        }
        state++;
      } else if (state < SLOTS_STOP) {
        slots[state - 1].stop(); // stop spindle
        state++;
      }
    }
  }
}
```
3, next we look at the second part, the part is mainly aimed at making each column random stop, the paragraph at the end of the loop, can not change the location, first of all, we only need to automate the mode of this code.
That is, when the macro Automation is defined as 1 to enable automatic mode. In Automation mode, the user does not need to press a button each time to control the start and stop of the spool, but the code automatically controls the start and stop process of each spool. When Auto Mode is enabled, the system will automatically start the spindles and keep track of the elapsed time with the counter Count_Stop. Each time the counter reaches a pre-generated random stop time Count_Num, the system triggers the stopping of a spindle. When all spindles have been started, the system resets the state and prepares for the next round of operation.
```
if (Automation) { // If in automatic mode
  if (Slot_Start && Slot_Start < StartCount_MAX) { //When at least one shaft has been started and the maximum number of starts has not yet been reached
    Count_Stop++; // Counter increments to record time progress
    if (Count_Stop == Count_Num && state < SLOTS_STOP) { //If the counter reaches a randomly generated stop time
      Slot_Stop = 1; // Trigger auto-stop spindle
      Count_Stop = 0; //Counter resets to zero in preparation for the next stop
      Count_Num = rand() % (Time_MAX - Time_MIN - 1) + Time_MIN; //Randomly generate the next stop interval
    }
  } else if (Slot_Start == StartCount_MAX) { // If all spindles have been activated
    Slot_Start = 0; // Zero the startup counter in preparation for the next startup
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
[![OpenELAB_logo_resized_150](https://github.com/user-attachments/assets/5d3de375-359c-46a3-96bb-aaa211c6c636)](https://openelab.io)  
__YouTube:__  
[![youtube_logo_200x150](https://github.com/user-attachments/assets/d2365e7f-4ffe-4124-bf62-21eba19a71e4)](https://www.youtube.com/@OpenELAB)  
__X :__  
[![X_logo_150x150](https://github.com/user-attachments/assets/4ad5095f-2573-4791-9360-b355530093bf)](https://twitter.com/openelabio)  
__FaceBook:__  
[![facebook_logo_cropped_150x150](https://github.com/user-attachments/assets/52f2dc9a-a564-49a5-b72e-30eafbbc281f)](https://www.facebook.com/profile.php?id=61559154729457)  
__Discord__  
[![resized_image_150x150](https://github.com/user-attachments/assets/93ecd098-3391-45bb-9d80-b166c197a475)](https://discord.gg/VQspWyck)

__The source code was adapted from__
[M5StickCPlus](https://github.com/Sarah-C/M5StickC_Plus_Slot_Machine)

