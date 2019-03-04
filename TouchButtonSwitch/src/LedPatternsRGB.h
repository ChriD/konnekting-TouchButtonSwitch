


#ifndef LedPatternsRGB_h
#define LedPatternsRGB_h

// due we have a common anode we have reverse values!
#define RED     255,0,0
#define YELLOW  255,255,0
#define GREEN   0,255,0
#define CYAN    0,255,255
#define BLUE    0,0,255
#define MAGENTA 255,0,255
#define WHITE   255,255,255
#define OFF     0,0,0











const uint8_t ledPattern_Setup[] = {
    LedPattern::CMD_SET, 0,
    LedPattern::CMD_REPEAT, LedPattern::repeatForever,
        LedPattern::CMD_FADETO,  25, 150,
        LedPattern::CMD_FADETO, 50, 10,
    LedPattern::CMD_ENDREPEAT
};


const uint8_t ledPattern_ProgX[] = {
    LedPattern::CMD_REPEAT, LedPattern::repeatForever,
        LedPattern::CMD_SET, 200,
        LedPattern::CMD_WAIT, 10,
        LedPattern::CMD_SET, 0,
        LedPattern::CMD_WAIT, 10,
    LedPattern::CMD_ENDREPEAT
};

const uint8_t ledPattern_Prog[] = {
    LedPattern::CMD_SET, 0,
    LedPattern::CMD_REPEAT, LedPattern::repeatForever,
        LedPattern::CMD_FADETO, 10, 150,
        LedPattern::CMD_FADETO, 10, 0,
    LedPattern::CMD_ENDREPEAT
};


const uint8_t ledPattern_Calibration[] = {
    LedPattern::CMD_FADETO,  25, 254,
    LedPattern::CMD_FINISHED
};


const uint8_t ledPattern_Normal[] = {
    LedPattern::CMD_FADETO,  25, 15,
    LedPattern::CMD_FINISHED
};


const uint8_t ledPattern_Touch[] = {
    LedPattern::CMD_FADETO,  25, 200,
    LedPattern::CMD_FADETO,  25, 0,
    LedPattern::CMD_FADETO,  25, 15,
    LedPattern::CMD_FINISHED
};

#endif