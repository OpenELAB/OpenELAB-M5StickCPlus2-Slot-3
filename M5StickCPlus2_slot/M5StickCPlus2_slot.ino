#include <M5StickCPlus2.h>
#include "Slot.h"
#include "time.h"

#ifndef _countof
#define _countof(a) (sizeof(a) / sizeof(a[0]))
#endif

#define Automation 1 // Zero the startup counter in preparation for the next startup
#define LOOP_WAIT 30 // Cyclic wait time to control frame rate
#define FLUSH_DELAY 100 // Refresh delay for background flicker effect
#define FLUSH_COUNT 5 // Blink count, determines how many times it blinks

#define Time_MAX 80 // Maximum value of stopping time in automatic mode
#define Time_MIN 45 //Minimum value of stopping time in automatic mode
#define InitTime_MAX 100 // Maximum time to stop automatically in the initial state
#define InitTime_MIN 80 // Minimum time to stop automatically in the initial state
#define StartCount_MAX 6 // Maximum number of spindle starts in automatic mode

Slot slots[SLOT_COUNT]; // Define a set of slots' reels, each of which is a Slot object
const int symbolIndices[] = { 2, 4, 5, 0, 3, 4, 2, 5, 3, 1, 0}; // An array of the order of the icons, determining the order of the icons displayed in each column
enum SlotsState { SLOTS_INIT, SLOTS_START, SLOTS_STOP = SLOT_COUNT + 1, SLOTS_FLUSH }; // Defining the state of a slot machine
int state = SLOTS_INIT; // Initialization state, default is SLOTS_INIT




//
// setup()//initialization function
//
void setup() {
  M5.begin(); // Initialization M5StickC Plus2
  M5.Lcd.setRotation(3); // Setting screen rotation
  M5.Lcd.fillScreen(TFT_BLACK); // Set the screen background color to black
  M5.Lcd.setSwapBytes(true); // Setting the byte-swap mode for compatibility with image data

  Slot::initShadow(); // Initialize the shadow effect (via a static function of the Slot class)
  Slot::setReel(symbolIndices, _countof(symbolIndices)); // Setting the symbol order of the initial reels

  for (int i = 0; i < SLOT_COUNT; i++) {
    slots[i].init(i, i * SLOT_COUNT); // Initialize each Slot object
    slots[i].draw(); // Draws each Slot in its initial state
  }

  srand(time(0)); // Setting the random number seed
}


//
// loop()//The main function, in which the slot machine LCD needs to be refreshed at all times, should not have a delay similar to delay_ms
//
void loop() {
  unsigned long tick = millis(); // Get current heartbeat in milliseconds
  static unsigned long flushTick; // Record the heartbeat that refreshes the background
  static int flushCount; // Record the number of background refreshes
  static int Slot_Start = 0; // Control the number of spindle starts
  static int Slot_Stop = 0; // Whether to stop the spindle
  static int Count_Stop = 0; // Count the heartbeats of the stop spindle
  static int Count_Num; // Randomly generated stop times in automatic mode

  M5.update(); // Update button status

  if (Automation) { // If automatic mode is enabled
    if (M5.BtnA.wasPressed() || Slot_Stop == 1) { // Stop triggered by button press or auto mode
      if (state == SLOTS_INIT) { // If in the initialization state
        for (int i = 0; i < SLOT_COUNT; i++) {
          slots[i].start(); // Activate each spindle
        }
        state++;
        Slot_Start = 1; // Record startup status
        Count_Num = rand() % (InitTime_MAX - InitTime_MIN - 1) + InitTime_MIN; // Randomly generated stop times
      } else if (state < SLOTS_STOP) { // If the spindle is running
        Slot_Start++;
        slots[state - 1].stop(); // Stop the current axis
        state++;
      }
    }
    Slot_Stop = 0; // Reset stop sign
  } else { // manual mode
    if (M5.BtnA.wasPressed()) { // Press the button to start or stop the spindle
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

  // Determine if all axes are stopped
  if (state == SLOTS_STOP) {
    bool stopAll = true;
    int symbol = -1;
    for (int i = 0; i < SLOT_COUNT; i++) {
      int n = slots[i].getSymbol(); // Getting the symbol of a rotating shaft
      if (n == -1) {
        stopAll = false; // If there are still spindles that have not stopped, jump out of the
      } else {
        symbol = (n == symbol || symbol == -1) ? n : -2; // Determine if the symbols are the same
      }
    }
    if (stopAll) {
      if (symbol >= 0) {
        flushTick = tick; // If the symbols are the same, start blinking
        flushCount = 0;
        state = SLOTS_FLUSH;
      } else {
        state = SLOTS_INIT; // Otherwise return to the initial state
      }
    }
  }

  // Background flashing logic
  if (state == SLOTS_FLUSH) {
    if (tick >= flushTick + FLUSH_DELAY) {
      flushTick = tick;
      for (int i = 0; i < SLOT_COUNT; i++) {
        slots[i].flush((flushCount & 1) ? TFT_WHITE : TFT_BLUE); // Flashes white and blue alternately
      }
      if (++flushCount >= FLUSH_COUNT * 2) {
        state = SLOTS_INIT; // Reset state when blinking is complete
      }
    }
  }

  // Auto Stop Logic
  if (Automation) {
    if (Slot_Start && Slot_Start < StartCount_MAX) {
      Count_Stop++;
      if (Count_Stop == Count_Num && state < SLOTS_STOP) {
        Slot_Stop = 1; // Automatic stopping of spindles
        Count_Stop = 0;
        Count_Num = rand() % (Time_MAX - Time_MIN - 1) + Time_MIN; // Randomly generate the next stop time
      }
    } else if (Slot_Start == StartCount_MAX) {
      Slot_Start = 0; // Reset Autostart Status
    }
  }

  // Update each axis and redraw the screen
  for (int i = 0; i < SLOT_COUNT; i++) {
    if (slots[i].update()) {
      slots[i].draw();
    }
  }

  // Control cycle wait time to ensure stable update frequency
  int ms = millis() - tick;
  if (ms < LOOP_WAIT) {
    delay(LOOP_WAIT - ms);
  }
}
