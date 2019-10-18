#define MANUFACTURER_ID 52700
#define DEVICE_ID 1
#define REVISION 0

#define COMOBJ_firmwareTransfer 0
#define COMOBJ_button1 1
#define COMOBJ_button1_double 2
#define COMOBJ_button1_multi 3
#define COMOBJ_button1_long 4
#define COMOBJ_button1_long_touchstart 5
#define COMOBJ_button1_long_touchend 6
#define COMOBJ_button1_long_touchState 7
#define COMOBJ_button2 8
#define COMOBJ_button2_double 9
#define COMOBJ_button2_multi 10
#define COMOBJ_button2_long 11
#define COMOBJ_button2_long_touchstart 12
#define COMOBJ_button2_long_touchend 13
#define COMOBJ_button2_long_touchState 14
#define COMOBJ_button3 15
#define COMOBJ_button3_double 16
#define COMOBJ_button3_multi 17
#define COMOBJ_button3_long 18
#define COMOBJ_button3_long_touchstart 19
#define COMOBJ_button3_long_touchend 20
#define COMOBJ_button3_long_touchState 21
#define COMOBJ_button4 22
#define COMOBJ_button4_double 23
#define COMOBJ_button4_multi 24
#define COMOBJ_button4_long 25
#define COMOBJ_button4_long_touchstart 26
#define COMOBJ_button4_long_touchend 27
#define COMOBJ_button4_long_touchState 28
#define COMOBJ_button5 29
#define COMOBJ_button5_double 30
#define COMOBJ_button5_multi 21
#define COMOBJ_button5_long 32
#define COMOBJ_button5_long_touchstart 33
#define COMOBJ_button5_long_touchend 34
#define COMOBJ_button5_long_touchState 35
#define COMOBJ_env_temperature 36
#define COMOBJ_env_humidity 37
#define COMOBJ_env_pressure 38
#define PARAM_button1_mode 0
#define PARAM_button1_longTouchMode 1
#define PARAM_button1_multiTouchEnabled 2
#define PARAM_button2_mode 3
#define PARAM_button2_longTouchMode 4
#define PARAM_button2_multiTouchEnabled 5
#define PARAM_button3_mode 6
#define PARAM_button3_longTouchMode 7
#define PARAM_button3_multiTouchEnabled 8
#define PARAM_button4_mode 9
#define PARAM_button4_longTouchMode 10
#define PARAM_button4_multiTouchEnabled 11
#define PARAM_button5_mode 12
#define PARAM_button5_longTouchMode 13
#define PARAM_button5_multiTouchEnabled 14
#define PARAM_clickFeedback_active 15
#define PARAM_clickFeedback_frequency 16
#define PARAM_clickFeedback_duration 17
#define PARAM_led_color_red 18
#define PARAM_led_color_green 19
#define PARAM_led_color_blue 20
#define PARAM_led_brightness 21
#define PARAM_temp_offset 22
#define PARAM_humidity_offset 23
#define PARAM_envData_sendPeriod 24
#define PARAM_cap_trigger_offset_main 25
#define PARAM_cap_trigger_offset_sub 26

KnxComObject KnxDevice::_comObjectsList[] = {
    /* Index 0 - firmwareTransfer */ KnxComObject(KNX_DPT_60000_60000, 0x2a),
    /* Index 1 - button1 */ KnxComObject(KNX_DPT_1_001, 0x34),
    /* Index 2 - button1_double */ KnxComObject(KNX_DPT_1_001, 0x34),
    /* Index 3 - button1_multi */ KnxComObject(KNX_DPT_5_004, 0x34),
    /* Index 4 - button1_long */ KnxComObject(KNX_DPT_1_001, 0x34),
    /* Index 5 - button1_long_touchstart */ KnxComObject(KNX_DPT_1_001, 0x34),
    /* Index 6 - button1_long_touchend */ KnxComObject(KNX_DPT_1_001, 0x34),
    /* Index 7 - button1_long_touchState */ KnxComObject(KNX_DPT_1_001, 0x34),
    /* Index 8 - button2 */ KnxComObject(KNX_DPT_1_001, 0x34),
    /* Index 9 - button2_double */ KnxComObject(KNX_DPT_1_001, 0x34),
    /* Index 10 - button2_multi */ KnxComObject(KNX_DPT_5_004, 0x34),
    /* Index 11 - button2_long */ KnxComObject(KNX_DPT_1_001, 0x34),
    /* Index 12 - button2_long_touchstart */ KnxComObject(KNX_DPT_1_001, 0x34),
    /* Index 13 - button2_long_touchend */ KnxComObject(KNX_DPT_1_001, 0x34),
    /* Index 14 - button2_long_touchState */ KnxComObject(KNX_DPT_1_001, 0x34),
    /* Index 15 - button3 */ KnxComObject(KNX_DPT_1_001, 0x34),
    /* Index 16 - button3_double */ KnxComObject(KNX_DPT_1_001, 0x34),
    /* Index 17 - button3_multi */ KnxComObject(KNX_DPT_5_004, 0x34),
    /* Index 18 - button3_long */ KnxComObject(KNX_DPT_1_001, 0x34),
    /* Index 19 - button3_long_touchstart */ KnxComObject(KNX_DPT_1_001, 0x34),
    /* Index 20 - button3_long_touchend */ KnxComObject(KNX_DPT_1_001, 0x34),
    /* Index 21 - button3_long_touchState */ KnxComObject(KNX_DPT_1_001, 0x34),
    /* Index 22 - button4 */ KnxComObject(KNX_DPT_1_001, 0x34),
    /* Index 23 - button4_double */ KnxComObject(KNX_DPT_1_001, 0x34),
    /* Index 24 - button4_multi */ KnxComObject(KNX_DPT_5_004, 0x34),
    /* Index 25 - button4_long */ KnxComObject(KNX_DPT_1_001, 0x34),
    /* Index 26 - button4_long_touchstart */ KnxComObject(KNX_DPT_1_001, 0x34),
    /* Index 27 - button4_long_touchend */ KnxComObject(KNX_DPT_1_001, 0x34),
    /* Index 28 - button4_long_touchState */ KnxComObject(KNX_DPT_1_001, 0x34),
    /* Index 29 - button5 */ KnxComObject(KNX_DPT_1_001, 0x34),
    /* Index 30 - button5_double */ KnxComObject(KNX_DPT_1_001, 0x34),
    /* Index 21 - button5_multi */ KnxComObject(KNX_DPT_5_004, 0x34),
    /* Index 32 - button5_long */ KnxComObject(KNX_DPT_1_001, 0x34),
    /* Index 33 - button5_long_touchstart */ KnxComObject(KNX_DPT_1_001, 0x34),
    /* Index 34 - button5_long_touchend */ KnxComObject(KNX_DPT_1_001, 0x34),
    /* Index 35 - button5_long_touchState */ KnxComObject(KNX_DPT_1_001, 0x34),
    /* Index 36 - env_temperature */ KnxComObject(KNX_DPT_9_001, 0x34),
    /* Index 37 - env_humidity */ KnxComObject(KNX_DPT_9_001, 0x34),
    /* Index 38 - env_pressure */ KnxComObject(KNX_DPT_14_001, 0x34)
};
const byte KnxDevice::_numberOfComObjects = sizeof (_comObjectsList) / sizeof (KnxComObject); // do not change this code

byte KonnektingDevice::_paramSizeList[] = {
    /* Index 0 - button1_mode */ PARAM_UINT8,
    /* Index 1 - button1_longTouchMode */ PARAM_UINT8,
    /* Index 2 - button1_multiTouchEnabled */ PARAM_UINT8,
    /* Index 3 - button2_mode */ PARAM_UINT8,
    /* Index 4 - button2_longTouchMode */ PARAM_UINT8,
    /* Index 5 - button2_multiTouchEnabled */ PARAM_UINT8,
    /* Index 6 - button3_mode */ PARAM_UINT8,
    /* Index 7 - button3_longTouchMode */ PARAM_UINT8,
    /* Index 8 - button3_multiTouchEnabled */ PARAM_UINT8,
    /* Index 9 - button4_mode */ PARAM_UINT8,
    /* Index 10 - button4_longTouchMode */ PARAM_UINT8,
    /* Index 11 - button4_multiTouchEnabled */ PARAM_UINT8,
    /* Index 12 - button5_mode */ PARAM_UINT8,
    /* Index 13 - button5_longTouchMode */ PARAM_UINT8,
    /* Index 14 - button5_multiTouchEnabled */ PARAM_UINT8,
    /* Index 15 - clickFeedback_active */ PARAM_UINT8,
    /* Index 16 - clickFeedback_frequency */ PARAM_UINT32,
    /* Index 17 - clickFeedback_duration */ PARAM_UINT32,
    /* Index 18 - led_color_red */ PARAM_UINT8,
    /* Index 19 - led_color_green */ PARAM_UINT8,
    /* Index 20 - led_color_blue */ PARAM_UINT8,
    /* Index 21 - led_brightness */ PARAM_UINT8,
    /* Index 22 - temp_offset */ PARAM_INT16,
    /* Index 23 - humidity_offset */ PARAM_INT16,
    /* Index 24 - envData_sendPeriod */ PARAM_UINT32,
    /* Index 25 - cap_trigger_offset_main */ PARAM_INT16,
    /* Index 26 - cap_trigger_offset_sub */ PARAM_INT16
};
const int KonnektingDevice::_numberOfParams = sizeof (_paramSizeList); // do not change this code