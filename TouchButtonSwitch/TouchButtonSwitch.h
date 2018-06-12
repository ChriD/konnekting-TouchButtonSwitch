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
#define PARAM_multiTouchPeriod 0
#define PARAM_longHoldPeriod 1

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
    /* Index 10 - button1_long */ KnxComObject(KNX_DPT_1_001, 0x34)
};
const byte KnxDevice::_numberOfComObjects = sizeof (_comObjectsList) / sizeof (KnxComObject); // do not change this code

byte KonnektingDevice::_paramSizeList[] = {
    /* Index 0 - multiTouchPeriod */ PARAM_UINT16,
    /* Index 1 - longHoldPeriod */ PARAM_UINT16
};
const int KonnektingDevice::_numberOfParams = sizeof (_paramSizeList); // do not change this code