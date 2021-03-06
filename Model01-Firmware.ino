// -*- mode: c++ -*-
// Copyright 2016 Keyboardio, inc. <jesse@keyboard.io>
// See "LICENSE" for license details

#ifndef BUILD_INFORMATION
#define BUILD_INFORMATION "locally built"
#endif


/**
 * These #include directives pull in the Kaleidoscope firmware core,
 * as well as the Kaleidoscope plugins we use in the Model 01's firmware
 */


// The Kaleidoscope core
#include "Kaleidoscope.h"

// Support for storing the keymap in EEPROM
#include "Kaleidoscope-EEPROM-Settings.h"
#include "Kaleidoscope-EEPROM-Keymap.h"

// Support for communicating with the host via a simple Serial protocol
#include "Kaleidoscope-FocusSerial.h"

// Support for keys that move the mouse
#include "Kaleidoscope-MouseKeys.h"

// Support for macros
#include "Kaleidoscope-Macros.h"

// Support for controlling the keyboard's LEDs
#include "Kaleidoscope-LEDControl.h"


// Support for "Numpad" mode, which is mostly just the Numpad specific LED mode
// #include "Kaleidoscope-NumPad.h"

// Support for the "Boot greeting" effect, which pulses the 'LED' button for 10s
// when the keyboard is connected to a computer (or that computer is powered on)
#include "Kaleidoscope-LEDEffect-BootGreeting.h"

// Support for LED modes that set all LEDs to a single color
#include "Kaleidoscope-LEDEffect-SolidColor.h"

// Support for an LED mode that makes all the LEDs 'breathe'
#include "Kaleidoscope-LEDEffect-Breathe.h"

// Support for LED modes that pulse the keyboard's LED in a rainbow pattern
#include "Kaleidoscope-LEDEffect-Rainbow.h"

// Support for an LED mode that lights up the keys as you press them
// #include "Kaleidoscope-LED-Stalker.h"

// Support for an LED mode that prints the keys you press in letters 4px high
// #include "Kaleidoscope-LED-AlphaSquare.h"

// Support for shared palettes for other plugins, like Colormap below
#include "Kaleidoscope-LED-Palette-Theme.h"

// Support for an LED mode that lets one configure per-layer color maps
#include "Kaleidoscope-Colormap.h"

// Support for Keyboardio's internal keyboard testing mode
#include "Kaleidoscope-HardwareTestMode.h"

// Support for host power management (suspend & wakeup)
#include "Kaleidoscope-HostPowerManagement.h"

// Support for magic combos (key chords that trigger an action)
#include "Kaleidoscope-MagicCombo.h"

// Support for USB quirks, like changing the key state report protocol
#include "Kaleidoscope-USB-Quirks.h"

#include "Kaleidoscope-OneShot.h"
// #include "Kaleidoscope-Escape-OneShot.h"
#include "Kaleidoscope-LED-ActiveModColor.h"

#include <Kaleidoscope-Qukeys.h>

/** This 'enum' is a list of all the macros used by the Model 01's firmware
  * The names aren't particularly important. What is important is that each
  * is unique.
  *
  * These are the names of your macros. They'll be used in two places.
  * The first is in your keymap definitions. There, you'll use the syntax
  * `M(MACRO_NAME)` to mark a specific keymap position as triggering `MACRO_NAME`
  *
  * The second usage is in the 'switch' statement in the `macroAction` function.
  * That switch statement actually runs the code associated with a macro when
  * a macro key is pressed.
  */

enum { MACRO_VERSION_INFO,
       MACRO_ANY,
       MACRO_TOGGLE_QUKEYS
     };



/** The Model 01's key layouts are defined as 'keymaps'. By default, there are three
  * keymaps: The standard QWERTY keymap, the "Function layer" keymap and the "Numpad"
  * keymap.
  *
  * Each keymap is defined as a list using the 'KEYMAP_STACKED' macro, built
  * of first the left hand's layout, followed by the right hand's layout.
  *
  * Keymaps typically consist mostly of `Key_` definitions. There are many, many keys
  * defined as part of the USB HID Keyboard specification. You can find the names
  * (if not yet the explanations) for all the standard `Key_` defintions offered by
  * Kaleidoscope in these files:
  *    https://github.com/keyboardio/Kaleidoscope/blob/master/src/key_defs_keyboard.h
  *    https://github.com/keyboardio/Kaleidoscope/blob/master/src/key_defs_consumerctl.h
  *    https://github.com/keyboardio/Kaleidoscope/blob/master/src/key_defs_sysctl.h
  *    https://github.com/keyboardio/Kaleidoscope/blob/master/src/key_defs_keymaps.h
  *
  * Additional things that should be documented here include
  *   using ___ to let keypresses fall through to the previously active layer
  *   using XXX to mark a keyswitch as 'blocked' on this layer
  *   using ShiftToLayer() and LockLayer() keys to change the active keymap.
  *   keeping NUM and FN consistent and accessible on all layers
  *
  * The PROG key is special, since it is how you indicate to the board that you
  * want to flash the firmware. However, it can be remapped to a regular key.
  * When the keyboard boots, it first looks to see whether the PROG key is held
  * down; if it is, it simply awaits further flashing instructions. If it is
  * not, it continues loading the rest of the firmware and the keyboard
  * functions normally, with whatever binding you have set to PROG. More detail
  * here: https://community.keyboard.io/t/how-the-prog-key-gets-you-into-the-bootloader/506/8
  *
  * The "keymaps" data structure is a list of the keymaps compiled into the firmware.
  * The order of keymaps in the list is important, as the ShiftToLayer(#) and LockLayer(#)
  * macros switch to key layers based on this list.
  *
  *

  * A key defined as 'ShiftToLayer(FUNCTION)' will switch to FUNCTION while held.
  * Similarly, a key defined as 'LockLayer(NUMPAD)' will switch to NUMPAD when tapped.
  */

/**
  * Layers are "0-indexed" -- That is the first one is layer 0. The second one is layer 1.
  * The third one is layer 2.
  * This 'enum' lets us use names like QWERTY, FUNCTION, and NUMPAD in place of
  * the numbers 0, 1 and 2.
  */

enum { PRIMARY, FUNCTION, LOWER, MOUSE }; // layers


/**
  * To change your keyboard's layout from QWERTY to DVORAK or COLEMAK, comment out the line
  *
  * #define PRIMARY_KEYMAP_QWERTY
  *
  * by changing it to
  *
  * // #define PRIMARY_KEYMAP_QWERTY
  *
  * Then uncomment the line corresponding to the layout you want to use.
  *
  */

// #define PRIMARY_KEYMAP_QWERTY
#define PRIMARY_KEYMAP_NEH
// #define PRIMARY_KEYMAP_COLEMAK
// #define PRIMARY_KEYMAP_DVORAK
// #define PRIMARY_KEYMAP_CUSTOM



/* This comment temporarily turns off astyle's indent enforcement
 *   so we can make the keymaps actually resemble the physical key layout better
 */
// *INDENT-OFF*

KEYMAPS(

#if defined (PRIMARY_KEYMAP_QWERTY)
  [PRIMARY] = KEYMAP_STACKED
  // (___,           Key_1, Key_2, Key_3, Key_4, Key_5, ___,
  (___,          ___,   Key_Backslash, Key_Minus, Key_Equals, ___, ___,
   Key_Backtick, Key_Q, Key_W, Key_E, Key_R, Key_T, Key_Delete,
   Key_Escape,   Key_A, Key_S, Key_D, Key_F, Key_G,
   ___,          Key_Z, Key_X, Key_C, Key_V, Key_B, Key_Enter,
   OSL(LOWER), OSM(LeftShift), Key_Escape, OSM(LeftAlt),
   Key_Tab,

   // ___,           Key_6, Key_7, Key_8,     Key_9,      Key_0,         ___,
   M(MACRO_TOGGLE_QUKEYS), ___, LSHIFT(Key_Minus), LSHIFT(Key_Equals), LSHIFT(Key_9), LSHIFT(Key_0), ___,
   Key_Backspace, Key_Y, Key_U, Key_I,     Key_O,      Key_P,         Key_LeftBracket,
                  Key_H, Key_J, Key_K,     Key_L,      Key_Semicolon, Key_Quote,
   Key_Enter,     Key_N, Key_M, Key_Comma, Key_Period, Key_Slash,     Key_RightBracket,
   OSM(RightAlt),  Key_Escape, Key_Spacebar, OSL(FUNCTION),
   Key_Tab),

  [FUNCTION] =  KEYMAP_STACKED
  (___, Key_F1,                     Key_F2,   Key_F3,                  Key_F4,                     Key_F5,                   Key_LEDEffectNext,
   ___, ___,                        ___,      ___,                     ___,         ___,                      ___,
   ___, ___,                        ___,      ___,                     ___,                 Consumer_VolumeIncrement,
   ___, Consumer_ScanPreviousTrack, Key_Mute, Consumer_PlaySlashPause, Consumer_ScanPreviousTrack, Consumer_VolumeDecrement, ___,
   ___, ___, ___, ___,
   ___,

   ___, Key_F6,     Key_F7,        Key_F8,                 Key_F9,         Key_F10,        Key_F11,
   ___, Key_mouseBtnL, Key_mouseL, Key_mouseDn, Key_mouseUp, Key_mouseR,            Key_F12,
        Key_mouseBtnR, Key_LeftArrow, Key_DownArrow,          Key_UpArrow,    Key_RightArrow, ___,
   ___, ___,        ___,           LCTRL(LSHIFT(Key_Tab)), LCTRL(Key_Tab), ___,            ___,     
   ___, ___, ___, ___,
   ___),

  // [LOWER] = KEYMAP_STACKED
  // (___,          ___, ___, ___, ___, ___, ___,
  //  ___, LSHIFT(Key_1), LSHIFT(Key_2), LSHIFT(Key_3), LSHIFT(Key_4), LSHIFT(Key_5), ___,
  //  ___, Key_1,         Key_2,         Key_3,         Key_4,         Key_5,
  //  ___, LSHIFT(Key_1), LSHIFT(Key_2), LSHIFT(Key_3), LSHIFT(Key_4), LSHIFT(Key_5), ___,
  //  ___, ___, ___, ___,
  //  ___,

  //  ___,  ___, ___, ___,     ___,         ___,         ___,
  //  ___,  LSHIFT(Key_6), LSHIFT(Key_7), LSHIFT(Key_8), LSHIFT(Key_9), LSHIFT(Key_0), ___,
  //        Key_6,         Key_7,         Key_8,         Key_9,         Key_0,         ___,
  //  ___,  LSHIFT(Key_6), LSHIFT(Key_7), LSHIFT(Key_8), LSHIFT(Key_9), LSHIFT(Key_0), ___,
  //  ___, ___, ___, ___,
  //  ___),

  [LOWER] = KEYMAP_STACKED
  (___, LSHIFT(Key_1), LSHIFT(Key_2), LSHIFT(Key_3), LSHIFT(Key_4), LSHIFT(Key_5), ___,
   ___, ___, ___, ___, ___, ___, ___,
   ___, ___, ___, ___, ___, ___,
   ___, ___, ___, ___, ___, ___, ___,
   ___, ___, ___, ___,
   ___,

   ___,  LSHIFT(Key_6), LSHIFT(Key_7), LSHIFT(Key_8), LSHIFT(Key_9), LSHIFT(Key_0), ___,
   ___,  ___,   Key_4, Key_5, Key_6, Key_KeypadAdd, ___,
         Key_E, Key_1, Key_2, Key_3, Key_0, ___,
   ___,  ___,   Key_7, Key_8, Key_9, Key_RightBracket, ___,
   ___, ___, ___, ___,
   ___),

#elif defined (PRIMARY_KEYMAP_NEH)
  [PRIMARY] = KEYMAP_STACKED
  (XXX, XXX, XXX, XXX, XXX, XXX, XXX,
   XXX, Key_Q, Key_W, Key_E, Key_R, Key_T, XXX,
   XXX, Key_A, Key_S, Key_D, Key_F, Key_G,
   XXX, Key_Z, Key_X, Key_C, Key_V, Key_B, XXX,
   Key_Enter, OSM(LeftShift), Key_Escape, XXX,
   XXX,

   M(MACRO_TOGGLE_QUKEYS), XXX, XXX, XXX, XXX, XXX, XXX,
   XXX, Key_Y, Key_U, Key_I,     Key_O,      Key_P,         Key_LeftBracket,
        Key_H, Key_J, Key_K,     Key_L,      Key_Semicolon, XXX,
   XXX, Key_N, Key_M, Key_Comma, Key_Period, Key_Slash,     XXX,
   XXX, Key_Backspace, ShiftToLayer(FUNCTION), Key_Tab,
   XXX),

  [FUNCTION] =  KEYMAP_STACKED
  (XXX, Key_F1,                     Key_F2,   Key_F3,                  Key_F4,                     Key_F5,                   Key_LEDEffectNext,
   XXX, ___,                        ___,      ___,                     ___,         ___,                      XXX,
   XXX, ___,                        ___,      ___,                     ___,                 Consumer_VolumeIncrement,
   XXX, Consumer_ScanPreviousTrack, Key_Mute, Consumer_PlaySlashPause, Consumer_ScanPreviousTrack, Consumer_VolumeDecrement, XXX,
   ___, ___, Key_Delete, ___,
   XXX,

   XXX, Key_F6,     Key_F7,        Key_F8,                 Key_F9,         Key_F10,        Key_F11,
   XXX, Key_mouseBtnL, Key_mouseL, Key_mouseDn, Key_mouseUp, Key_mouseR,            Key_F12,
        Key_mouseBtnR, Key_LeftArrow, Key_DownArrow,          Key_UpArrow,    Key_RightArrow, ___,
   XXX, ___,        LGUI(Key_Tab),           LCTRL(LSHIFT(Key_Tab)), LCTRL(Key_Tab), ___,            ___,
   XXX, ___, ___, ___,
   XXX),

  [LOWER] = KEYMAP_STACKED
  (XXX, XXX, XXX, XXX, XXX, XXX, XXX,
   XXX, LSHIFT(Key_2), LSHIFT(Key_Equals), Key_Equals,      LSHIFT(Key_0), LSHIFT(Key_6), XXX,
   XXX, LSHIFT(Key_1), LSHIFT(Key_Minus),  Key_Minus,       LSHIFT(Key_9), LSHIFT(Key_5),
   XXX, LSHIFT(Key_3), Key_Backslash,      Key_LeftBracket, LSHIFT(Key_4), LSHIFT(Key_Backslash), XXX,
   ___, XXX, XXX, XXX,
   XXX,

   XXX, XXX, XXX, XXX, XXX, XXX, XXX,
   XXX, LSHIFT(Key_Backtick), Key_4, Key_5, Key_6, Key_RightBracket, XXX,
        LSHIFT(Key_8),        Key_1, Key_2, Key_3, Key_0, XXX,
   XXX, LSHIFT(Key_7),        Key_7, Key_8, Key_9, Key_KeypadAdd, XXX,
   XXX, LSHIFT(Key_Quote), Key_Quote, Key_Backtick,
   XXX),

#elif defined (PRIMARY_KEYMAP_DVORAK)

  [PRIMARY] = KEYMAP_STACKED
  (___,           Key_1,            Key_2,      Key_3,      Key_4, Key_5, ___,
   Key_Backtick,  Key_Quote,        Key_Comma,  Key_Period, Key_P, Key_Y, Key_Delete,
   Key_Backslash, Key_A,            Key_O,      Key_E,      Key_U, Key_I,
   Key_Pipe,      Key_Semicolon,    Key_Q,      Key_J,      Key_K, Key_X, Key_Enter,
   Key_Tab, Key_LeftShift, Key_Escape, Key_LeftAlt,
   OSL(FUNCTION),

   ___,           Key_6, Key_7, Key_8, Key_9, Key_0, ___,
   Key_Backspace, Key_F, Key_G, Key_C, Key_R, Key_L, Key_Slash,
                  Key_D, Key_H, Key_T, Key_N, Key_S, Key_Minus,
   Key_Enter,     Key_B, Key_M, Key_W, Key_V, Key_Z, Key_Equals,
   Key_RightAlt,  Key_Backspace, Key_Spacebar, Key_Tab,
   OSL(FUNCTION)),

  [FUNCTION] =  KEYMAP_STACKED
  (___, Key_F1,                     Key_F2,        Key_F3,                  Key_F4,                     Key_F5,                   Key_LEDEffectNext,
   ___, ___,                        ___,           Key_LeftCurlyBracket,    Key_RightCurlyBracket,      ___,                      ___,
   ___, ___,                        ___,           Key_LeftBracket,         Key_RightBracket,           Consumer_VolumeIncrement,
   ___, Consumer_ScanPreviousTrack, Key_Mute,      Consumer_PlaySlashPause, Consumer_ScanPreviousTrack, Consumer_VolumeDecrement, ___,
   ___, ___, ___, ___,
   ___,

   ___, Key_F6,     Key_F7,        Key_F8,                 Key_F9,         Key_F10,        Key_F11,
   ___, ___,        LSHIFT(Key_9), LSHIFT(Key_0),          ___,            ___,            Key_F12,
        ___,        Key_LeftArrow, Key_DownArrow,          Key_UpArrow,    Key_RightArrow, ___,
   ___, ___,        ___,           LCTRL(LSHIFT(Key_Tab)), LCTRL(Key_Tab), ___,            ___,     
   ___, ___, ___, ___,
   ___),

#elif defined (PRIMARY_KEYMAP_COLEMAK) 

  [PRIMARY] = KEYMAP_STACKED
  (___,          Key_1, Key_2, Key_3, Key_4, Key_5, Key_LEDEffectNext,
   Key_Backtick, Key_Q, Key_W, Key_F, Key_P, Key_G, Key_Tab,
   Key_PageUp,   Key_A, Key_R, Key_S, Key_T, Key_D,
   Key_PageDown, Key_Z, Key_X, Key_C, Key_V, Key_B, Key_Escape,
   Key_LeftControl, Key_Backspace, Key_LeftGui, Key_LeftShift,
   ShiftToLayer(FUNCTION),

   M(MACRO_ANY),  Key_6, Key_7, Key_8,     Key_9,         Key_0,         LockLayer(NUMPAD),
   Key_Enter,     Key_J, Key_L, Key_U,     Key_Y,         Key_Semicolon, Key_Equals,
                  Key_H, Key_N, Key_E,     Key_I,         Key_O,         Key_Quote,
   Key_RightAlt,  Key_K, Key_M, Key_Comma, Key_Period,    Key_Slash,     Key_Minus,
   Key_RightShift, Key_LeftAlt, Key_Spacebar, Key_RightControl,
   ShiftToLayer(FUNCTION)),

#elif defined (PRIMARY_KEYMAP_CUSTOM)


#else 

#error "No default keymap defined. You should make sure that you have a line like '#define PRIMARY_KEYMAP_QWERTY' in your sketch"

#endif 


  [MOUSE] =  KEYMAP_STACKED
  (___, ___, ___, ___, ___, ___, XXX,
   ___, ___, ___, ___, ___, ___, ___,
   ___, ___, ___, ___, ___, ___,
   ___, ___, ___, ___, ___, ___, ___,
   ___, ___, ___, ___,
   ___,

   ___, ___, ___,           ___,               ___,               ___,        ___,
   ___, ___, Key_mouseBtnL, Key_mouseBtnM,     Key_mouseBtnR,     ___,        ___,
        ___, Key_mouseL,    Key_mouseDn,       Key_mouseUp,       Key_mouseR, ___,
   ___, ___, ___,           Key_mouseScrollUp, Key_mouseScrollDn, ___,        ___,
   ___, ___, ___, ___,
   ___),
	) // KEYMAPS(

/* Re-enable astyle's indent enforcement */
// *INDENT-ON*

/** versionInfoMacro handles the 'firmware version info' macro
 *  When a key bound to the macro is pressed, this macro
 *  prints out the firmware build information as virtual keystrokes
 */

static void versionInfoMacro(uint8_t keyState) {
  if (keyToggledOn(keyState)) {
    Macros.type(PSTR("Keyboardio Model 01 - Kaleidoscope "));
    Macros.type(PSTR(BUILD_INFORMATION));
  }
}

/** anyKeyMacro is used to provide the functionality of the 'Any' key.
 *
 * When the 'any key' macro is toggled on, a random alphanumeric key is
 * selected. While the key is held, the function generates a synthetic
 * keypress event repeating that randomly selected key.
 *
 */

static void anyKeyMacro(uint8_t keyState) {
  static Key lastKey;
  bool toggledOn = false;
  if (keyToggledOn(keyState)) {
    lastKey.keyCode = Key_A.keyCode + (uint8_t)(millis() % 36);
    toggledOn = true;
  }

  if (keyIsPressed(keyState))
    kaleidoscope::hid::pressKey(lastKey, toggledOn);
}


/** macroAction dispatches keymap events that are tied to a macro
    to that macro. It takes two uint8_t parameters.

    The first is the macro being called (the entry in the 'enum' earlier in this file).
    The second is the state of the keyswitch. You can use the keyswitch state to figure out
    if the key has just been toggled on, is currently pressed or if it's just been released.

    The 'switch' statement should have a 'case' for each entry of the macro enum.
    Each 'case' statement should call out to a function to handle the macro in question.

 */

const macro_t *macroAction(uint8_t macroIndex, uint8_t keyState) {
  switch (macroIndex) {

  case MACRO_VERSION_INFO:
    versionInfoMacro(keyState);
    break;

  case MACRO_ANY:
    anyKeyMacro(keyState);
    break;

  case MACRO_TOGGLE_QUKEYS:
    if (keyToggledOn(keyState))
      Qukeys.toggle();
    break;
  }

  return MACRO_NONE;
}



// These 'solid' color effect definitions define a rainbow of
// LED color modes calibrated to draw 500mA or less on the
// Keyboardio Model 01.


static kaleidoscope::plugin::LEDSolidColor solidRed(160, 0, 0);
static kaleidoscope::plugin::LEDSolidColor solidOrange(140, 70, 0);
static kaleidoscope::plugin::LEDSolidColor solidYellow(130, 100, 0);
static kaleidoscope::plugin::LEDSolidColor solidGreen(0, 160, 0);
static kaleidoscope::plugin::LEDSolidColor solidBlue(0, 70, 130);
static kaleidoscope::plugin::LEDSolidColor solidIndigo(0, 0, 170);
static kaleidoscope::plugin::LEDSolidColor solidViolet(130, 0, 120);

/** toggleLedsOnSuspendResume toggles the LEDs off when the host goes to sleep,
 * and turns them back on when it wakes up.
 */
void toggleLedsOnSuspendResume(kaleidoscope::plugin::HostPowerManagement::Event event) {
  switch (event) {
  case kaleidoscope::plugin::HostPowerManagement::Suspend:
    LEDControl.set_all_leds_to({0, 0, 0});
    LEDControl.syncLeds();
    LEDControl.paused = true;
    break;
  case kaleidoscope::plugin::HostPowerManagement::Resume:
    LEDControl.paused = false;
    LEDControl.refreshAll();
    break;
  case kaleidoscope::plugin::HostPowerManagement::Sleep:
    break;
  }
}

/** hostPowerManagementEventHandler dispatches power management events (suspend,
 * resume, and sleep) to other functions that perform action based on these
 * events.
 */
void hostPowerManagementEventHandler(kaleidoscope::plugin::HostPowerManagement::Event event) {
  toggleLedsOnSuspendResume(event);
}

/** This 'enum' is a list of all the magic combos used by the Model 01's
 * firmware The names aren't particularly important. What is important is that
 * each is unique.
 *
 * These are the names of your magic combos. They will be used by the
 * `USE_MAGIC_COMBOS` call below.
 */
enum {
  // Toggle between Boot (6-key rollover; for BIOSes and early boot) and NKRO
  // mode.
  COMBO_TOGGLE_NKRO_MODE,
  // Enter test mode
  COMBO_ENTER_TEST_MODE
};

/** Wrappers, to be used by MagicCombo. **/

/**
 * This simply toggles the keyboard protocol via USBQuirks, and wraps it within
 * a function with an unused argument, to match what MagicCombo expects.
 */
static void toggleKeyboardProtocol(uint8_t combo_index) {
  USBQuirks.toggleKeyboardProtocol();
}

/**
 *  This enters the hardware test mode
 */
static void enterHardwareTestMode(uint8_t combo_index) {
  HardwareTestMode.runTests();
}


/** Magic combo list, a list of key combo and action pairs the firmware should
 * recognise.
 */
USE_MAGIC_COMBOS({.action = toggleKeyboardProtocol,
                  // Left Fn + Esc + Shift
                  .keys = { R3C6, R2C6, R3C7 }
}, {
  .action = enterHardwareTestMode,
  // Left Fn + Prog + LED
  .keys = { R3C6, R0C0, R0C6 }
});

// First, tell Kaleidoscope which plugins you want to use.
// The order can be important. For example, LED effects are
// added in the order they're listed here.
KALEIDOSCOPE_INIT_PLUGINS(
  Qukeys,

  // The EEPROMSettings & EEPROMKeymap plugins make it possible to have an
  // editable keymap in EEPROM.
  EEPROMSettings,
  EEPROMKeymap,

  // Focus allows bi-directional communication with the host, and is the
  // interface through which the keymap in EEPROM can be edited.
  Focus,

  // FocusSettingsCommand adds a few Focus commands, intended to aid in
  // changing some settings of the keyboard, such as the default layer (via the
  // `settings.defaultLayer` command)
  FocusSettingsCommand,

  // FocusEEPROMCommand adds a set of Focus commands, which are very helpful in
  // both debugging, and in backing up one's EEPROM contents.
  FocusEEPROMCommand,

  // The boot greeting effect pulses the LED button for 10 seconds after the
  // keyboard is first connected
  BootGreetingEffect,

  // The hardware test mode, which can be invoked by tapping Prog, LED and the
  // left Fn button at the same time.
  HardwareTestMode,

  // LEDControl provides support for other LED modes
  LEDControl,

  // We start with the LED effect that turns off all the LEDs.
  LEDOff,

  // The rainbow effect changes the color of all of the keyboard's keys at the same time
  // running through all the colors of the rainbow.
  LEDRainbowEffect,

  // The rainbow wave effect lights up your keyboard with all the colors of a rainbow
  // and slowly moves the rainbow across your keyboard
  LEDRainbowWaveEffect,

  // These static effects turn your keyboard's LEDs a variety of colors
  solidOrange, solidGreen, solidBlue,

  // The MagicCombo plugin lets you use key combinations to trigger custom
  // actions - a bit like Macros, but triggered by pressing multiple keys at the
  // same time.
  MagicCombo,

  // The USBQuirks plugin lets you do some things with USB that we aren't
  // comfortable - or able - to do automatically, but can be useful
  // nevertheless. Such as toggling the key report protocol between Boot (used
  // by BIOSes) and Report (NKRO).
  USBQuirks,

  Macros,
  MouseKeys,
  HostPowerManagement,
  OneShot,
  ActiveModColorEffect
)

/** The 'setup' function is one of the two standard Arduino sketch functions.
 * It's called when your keyboard first powers up. This is where you set up
 * Kaleidoscope and any plugins.
 */
void setup() {
  // First, call Kaleidoscope's internal setup function
  Kaleidoscope.setup();

#if defined (PRIMARY_KEYMAP_QWERTY)
  QUKEYS(
    kaleidoscope::plugin::Qukey(0, KeyAddr(2, 7), Key_LeftControl),  // Control/Escape
    kaleidoscope::plugin::Qukey(0, KeyAddr(2, 8), Key_RightControl), // Control/Backspace

    kaleidoscope::plugin::Qukey(0, KeyAddr(3, 6), Key_LeftGui),  // left palm key
    kaleidoscope::plugin::Qukey(0, KeyAddr(3, 9), Key_RightGui), // right palm key
  )
#elif defined (PRIMARY_KEYMAP_NEH)
  QUKEYS(
    kaleidoscope::plugin::Qukey(0, KeyAddr(1, 3), Key_LeftControl),         // Control/.
    kaleidoscope::plugin::Qukey(0, KeyAddr(1, 12), Key_LeftControl),        // Control/c

    // PRIMARY layer
    kaleidoscope::plugin::Qukey(0, KeyAddr(2, 7), Key_LeftControl),         // Control/Escape
    kaleidoscope::plugin::Qukey(0, KeyAddr(1, 8), Key_Spacebar),            // Space/LOWER
    kaleidoscope::plugin::Qukey(0, KeyAddr(2, 8), Key_LeftAlt),             // Backspace/Alt
    kaleidoscope::plugin::Qukey(0, KeyAddr(0, 7), ShiftToLayer(LOWER)),     // Enter/FUNCTION
    kaleidoscope::plugin::Qukey(0, KeyAddr(0, 8), Key_LeftGui),             // Tab/GUI

    // NUMSYM layer
    kaleidoscope::plugin::Qukey(2, KeyAddr(2, 3), Key_LeftControl),         // Control/[
    kaleidoscope::plugin::Qukey(2, KeyAddr(2, 4), Key_LeftShift),           // Shift/(
    kaleidoscope::plugin::Qukey(2, KeyAddr(2, 11), Key_LeftShift),          // Shift/1
    kaleidoscope::plugin::Qukey(2, KeyAddr(2, 12), Key_LeftControl),        // Control/2
  )
#endif
  // Qukeys.setHoldTimeout(600);
  // Qukeys.setOverlapThreshold(100);

  OneShot.time_out = 750;
  OneShot.disableStickabilityForModifiers();
  OneShot.disableStickabilityForLayers();

  ActiveModColorEffect.highlight_color = CRGB(0x00, 0xee, 0x00);

  // We set the brightness of the rainbow effects to 150 (on a scale of 0-255)
  // This draws more than 500mA, but looks much nicer than a dimmer effect
  LEDRainbowEffect.brightness(150);
  LEDRainbowWaveEffect.brightness(150);

  // Set the action key the test mode should listen for to Left Fn
  HardwareTestMode.setActionKey(R3C6);

  // The LED Stalker mode has a few effects. The one we like is called
  // 'BlazingTrail'. For details on other options, see
  // https://github.com/keyboardio/Kaleidoscope/blob/master/doc/plugin/LED-Stalker.md
  // StalkerEffect.variant = STALKER(BlazingTrail);

  // We want to make sure that the firmware starts with LED effects off
  // This avoids over-taxing devices that don't have a lot of power to share
  // with USB devices
  LEDOff.activate();

  // To make the keymap editable without flashing new firmware, we store
  // additional layers in EEPROM. For now, we reserve space for five layers. If
  // one wants to use these layers, just set the default layer to one in EEPROM,
  // by using the `settings.defaultLayer` Focus command, or by using the
  // `keymap.onlyCustom` command to use EEPROM layers only.
  EEPROMKeymap.setup(5);

  // We need to tell the Colormap plugin how many layers we want to have custom
  // maps for. To make things simple, we set it to five layers, which is how
  // many editable layers we have (see above).
  ColormapEffect.max_layers(5);
}

/** loop is the second of the standard Arduino sketch functions.
  * As you might expect, it runs in a loop, never exiting.
  *
  * For Kaleidoscope-based keyboard firmware, you usually just want to
  * call Kaleidoscope.loop(); and not do anything custom here.
  */

void loop() {
  Kaleidoscope.loop();
}
