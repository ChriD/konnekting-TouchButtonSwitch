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
#define PARAM_button1_mode 0
#define PARAM_button1_longTouchMode 1
#define PARAM_button1_multiTouchEnabled 2

KnxComObject KnxDevice::_comObjectsList[] = {
    /* Index 0 - firmwareTransfer */ KnxComObject(KNX_DPT_60000_60000, 0x2a),
    /* Index 1 - button1 */ KnxComObject(KNX_DPT_1_001, 0x34),
    /* Index 2 - button1_double */ KnxComObject(KNX_DPT_1_001, 0x34),
    /* Index 3 - button1_multi */ KnxComObject(KNX_DPT_5_004, 0x34),
    /* Index 4 - button1_long */ KnxComObject(KNX_DPT_1_001, 0x34),
    /* Index 5 - button1_long_touchstart */ KnxComObject(KNX_DPT_1_001, 0x34),
    /* Index 6 - button1_long_touchend */ KnxComObject(KNX_DPT_1_001, 0x34),
    /* Index 7 - button1_long_touchState */ KnxComObject(KNX_DPT_1_001, 0x34)
};
const byte KnxDevice::_numberOfComObjects = sizeof (_comObjectsList) / sizeof (KnxComObject); // do not change this code

byte KonnektingDevice::_paramSizeList[] = {
    /* Index 0 - button1_mode */ PARAM_UINT8,
    /* Index 1 - button1_longTouchMode */ PARAM_UINT8,
    /* Index 2 - button1_multiTouchEnabled */ PARAM_UINT8
};
const int KonnektingDevice::_numberOfParams = sizeof (_paramSizeList); // do not change this code