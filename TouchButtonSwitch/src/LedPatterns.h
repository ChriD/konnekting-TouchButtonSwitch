


#ifndef LedPatterns_h
#define LedPatterns_h


const uint8_t ledPattern_Setup[] = {
    LedPattern::CMD_SET, 0,
    LedPattern::CMD_REPEAT, LedPattern::repeatForever,
        LedPattern::CMD_FADETO,  25, 150,
        LedPattern::CMD_FADETO, 50, 10,
    LedPattern::CMD_ENDREPEAT
};


const uint8_t ledPattern_Prog[] = {
    LedPattern::CMD_REPEAT, LedPattern::repeatForever,
        LedPattern::CMD_SET, 200,
        LedPattern::CMD_WAIT, 250,
        LedPattern::CMD_SET, 0,
        LedPattern::CMD_WAIT, 250,
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