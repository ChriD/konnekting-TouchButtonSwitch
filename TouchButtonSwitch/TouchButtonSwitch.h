#define MANUFACTURER_ID 52700
#define DEVICE_ID 1
#define REVISION 0

#define COMOBJ_firmwareTransfer 0
#define COMOBJ_temperatureValue 1
#define COMOBJ_temperatureAlarmLow 2
#define COMOBJ_temperatureAlarmHigh 3
#define COMOBJ_humidityValue 4
#define COMOBJ_humidityAlarmLow 5
#define COMOBJ_humidityAlarmHigh 6
#define COMOBJ_button1 7
#define COMOBJ_button1_double 8
#define COMOBJ_button1_multi 9
#define COMOBJ_button1_long 10
#define COMOBJ_button1_position_touchstart 11
#define COMOBJ_button1_position_touchend 12
#define COMOBJ_button2 13
#define COMOBJ_button2_double 14
#define COMOBJ_button2_multi 15
#define COMOBJ_button2_long 16
#define COMOBJ_button2_position_touchstart 17
#define COMOBJ_button2_position_touchend 18
#define PARAM_touch_threshold 0
#define PARAM_mode_longtouch_threshold 1
#define PARAM_mode_position_threshold 2
#define PARAM_light_intensity_standy 3
#define PARAM_light_intensity_proximity 4
#define PARAM_button1_mode 5
#define PARAM_button1_enableMultiTouch 6
#define PARAM_button2_mode 7
#define PARAM_button2_enableMultiTouch 8
#define PARAM_button3_mode 9
#define PARAM_button3_enableMultiTouch 10
#define PARAM_button4_mode 11
#define PARAM_button4_enableMultiTouch 12

KnxComObject KnxDevice::_comObjectsList[] = {
    /* Index 0 - firmwareTransfer */ KnxComObject(KNX_DPT_60000_60000, 0x2a),
    /* Index 1 - temperatureValue */ KnxComObject(KNX_DPT_9_001, 0x34),
    /* Index 2 - temperatureAlarmLow */ KnxComObject(KNX_DPT_1_001, 0x34),
    /* Index 3 - temperatureAlarmHigh */ KnxComObject(KNX_DPT_1_001, 0x34),
    /* Index 4 - humidityValue */ KnxComObject(KNX_DPT_9_007, 0x34),
    /* Index 5 - humidityAlarmLow */ KnxComObject(KNX_DPT_1_001, 0x34),
    /* Index 6 - humidityAlarmHigh */ KnxComObject(KNX_DPT_1_001, 0x34),
    /* Index 7 - button1 */ KnxComObject(KNX_DPT_1_001, 0x34),
    /* Index 8 - button1_double */ KnxComObject(KNX_DPT_1_001, 0x34),
    /* Index 9 - button1_multi */ KnxComObject(KNX_DPT_5_004, 0x34),
    /* Index 10 - button1_long */ KnxComObject(KNX_DPT_1_001, 0x34),
    /* Index 11 - button1_position_touchstart */ KnxComObject(KNX_DPT_1_001, 0x34),
    /* Index 12 - button1_position_touchend */ KnxComObject(KNX_DPT_1_001, 0x34),
    /* Index 13 - button2 */ KnxComObject(KNX_DPT_1_001, 0x34),
    /* Index 14 - button2_double */ KnxComObject(KNX_DPT_1_001, 0x34),
    /* Index 15 - button2_multi */ KnxComObject(KNX_DPT_5_004, 0x34),
    /* Index 16 - button2_long */ KnxComObject(KNX_DPT_1_001, 0x34),
    /* Index 17 - button2_position_touchstart */ KnxComObject(KNX_DPT_1_001, 0x34),
    /* Index 18 - button2_position_touchend */ KnxComObject(KNX_DPT_1_001, 0x34)
};
const byte KnxDevice::_numberOfComObjects = sizeof (_comObjectsList) / sizeof (KnxComObject); // do not change this code

byte KonnektingDevice::_paramSizeList[] = {
    /* Index 0 - touch_threshold */ PARAM_UINT16,
    /* Index 1 - mode_longtouch_threshold */ PARAM_UINT16,
    /* Index 2 - mode_position_threshold */ PARAM_UINT16,
    /* Index 3 - light_intensity_standy */ PARAM_UINT8,
    /* Index 4 - light_intensity_proximity */ PARAM_UINT8,
    /* Index 5 - button1_mode */ PARAM_UINT8,
    /* Index 6 - button1_enableMultiTouch */ PARAM_UINT8,
    /* Index 7 - button2_mode */ PARAM_UINT8,
    /* Index 8 - button2_enableMultiTouch */ PARAM_UINT8,
    /* Index 9 - button3_mode */ PARAM_UINT8,
    /* Index 10 - button3_enableMultiTouch */ PARAM_UINT8,
    /* Index 11 - button4_mode */ PARAM_UINT8,
    /* Index 12 - button4_enableMultiTouch */ PARAM_UINT8
};
const int KonnektingDevice::_numberOfParams = sizeof (_paramSizeList); // do not change this code