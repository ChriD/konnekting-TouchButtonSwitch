


#ifndef LedPatternsRGB_h
#define LedPatternsRGB_h

#define STD_BRIGHTNESS 150

#define RED     STD_BRIGHTNESS,0,0
#define YELLOW  STD_BRIGHTNESS,STD_BRIGHTNESS,0
#define GREEN   0,STD_BRIGHTNESS,0
#define BLUE    0,0,STD_BRIGHTNESS
#define MAGENTA STD_BRIGHTNESS,0,STD_BRIGHTNESS
#define WHITE   STD_BRIGHTNESS,STD_BRIGHTNESS,STD_BRIGHTNESS
#define OFF     0,0,0



const uint8_t ledPattern_Touch[] = {
    LedPattern::CMD_SET, OFF,
    LedPattern::CMD_REPEAT, 1,
        LedPattern::CMD_FADETO, 20, GREEN,
        LedPattern::CMD_FADETO, 20, BLUE,
    LedPattern::CMD_ENDREPEAT,
    LedPattern::CMD_FINISHED
  };


const uint8_t ledPattern_Normal[] = {
    LedPattern::CMD_SET, OFF,
    LedPattern::CMD_SET, BLUE,
    LedPattern::CMD_FINISHED,
  };


const uint8_t ledPattern_Prog[] = {
     LedPattern::CMD_SET, OFF,
     LedPattern::CMD_REPEAT, LedPattern::repeatForever,
        LedPattern::CMD_FADETO, 100, RED,
     LedPattern::CMD_ENDREPEAT,
  };


const uint8_t ledPattern_Calibration[] = {
     LedPattern::CMD_SET, OFF,
     LedPattern::CMD_REPEAT, LedPattern::repeatForever,
        LedPattern::CMD_FADETO, 100, BLUE,
     LedPattern::CMD_ENDREPEAT,
  };


const uint8_t ledPattern_Setup[] = {
     LedPattern::CMD_SET, OFF,
     LedPattern::CMD_REPEAT, LedPattern::repeatForever,
        LedPattern::CMD_FADETO, 100, YELLOW,
     LedPattern::CMD_ENDREPEAT,
  };


#endif