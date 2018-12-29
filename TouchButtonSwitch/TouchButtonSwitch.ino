
/*
  Konnekting - TouchButtonSwitch (0xCDDC)
  Created by Christian Dürnberger (ChriD), 2019
  https://github.com/ChriD
*/


#include "Arduino.h"
//#include "src/TouchButton.h"
#include "src/TouchSwitch_4X_V1.h"

/*
// we do use the same firmware for 1x,4x or 6x touch switches
// so we have to define which type of switch is being compiled
#define SWITCHTYPE        4

// define the pins for the touch buttons and proximity sensors
#define PIN_SENSOR_BUTTON_1       A1
#define PIN_SENSOR_BUTTON_2       A0
#define PIN_SENSOR_BUTTON_3       A5
#define PIN_SENSOR_BUTTON_4       A3
#define PIN_SENSOR_PROXIMITY_1    A2
#define PIN_SENSOR_PROXIMITY_2    A4

// define the led pins for the given buttons
#define PIN_SENSOR_BUTTON_1_LED   12
#define PIN_SENSOR_BUTTON_2_LED   13
#define PIN_SENSOR_BUTTON_3_LED   7
#define PIN_SENSOR_BUTTON_4_LED   9
*/

// This is the time in miliseconds how long the button should wait after startup/setup to allow touches
// There has to be a enough time for the user to put the frontplate on the switch before recalibration of
// the touch sensor has been done
#define STARTUP_IDLETIME       10000

#define SerialUSB SERIAL_PORT_USBVIRTUAL

//
TouchSwitch   *touchSwitch = new TouchSwitch_4X_V1();

//TouchButton   *touchButton1 = new TouchButton(A0);
//TouchButton   *touchButton2 = new TouchButton(A3);

//TouchButton   touchButton1(A0);
//TouchButton   touchButton2(A1);

void onButtonStateChanged(uint16_t _buttonId, uint16_t _state)
{
  SerialUSB.print(_buttonId);
  SerialUSB.print(" : ");
  SerialUSB.print(_state);
  SerialUSB.print("\n");
}

void onButtonAction(uint16_t _buttonId, uint16_t _type, uint16_t _value)
{
  SerialUSB.print(_buttonId);
  SerialUSB.print(" : ");
  SerialUSB.print(_type);
  SerialUSB.print(" | ");
  SerialUSB.print(_value);
  SerialUSB.print("\n");
}



void setup()
{

  SerialUSB.begin(115200);
  while (!SerialUSB) {
    ; // wait for serial port to connect. Needed for native USB
  }
  // TODO: create the switch object and define the proximity sensors and the touch buttons
  //touchButton1.setEventCallback(mqtt.set_callback([this] (char* topic, byte* payload, unsigned int length) { this->callback(topic, payload, length); })

/*
 // touchButton1->setCallbackOnButtonStateChanged(onButtonStateChanged);
  touchButton1->setCallbackOnButton(onButton);
  touchButton1->parmId(10);
  touchButton1->parmMultipleTapsEnabled(true);
  touchButton1->parmPositioningModeEnabled(false);
  touchButton1->parmBaseNoiseOffsetValue(15);

  //touchButton2->setCallbackOnButtonStateChanged(onButtonStateChanged);
  touchButton2->setCallbackOnButton(onButton);
  touchButton2->parmId(20);
  touchButton2->parmMultipleTapsEnabled(false);
  touchButton2->parmPositioningModeEnabled(true);
  touchButton2->parmBaseNoiseOffsetValue(15);


  if(!touchButton1->setup())
    SerialUSB.print("Error initializing Touch Button 1");
  if(!touchButton2->setup())
    SerialUSB.print("Error initializing Touch Button 2");

  //
  touchButton1->startCalibration();
  touchButton2->startCalibration();
  */


  //touchSwitch->attachCallbackOnButtonAction(makeFunctor((CallbackFunction_ButtonAction*)0,&onButtonAction));
  //makeFunctor((CallbackFunction_ButtonAction*)0, *this, &BaseSwitch::onButtonAction)

  if(!touchSwitch->setup())
    SerialUSB.print("Error initializing Touch Switch");
  touchSwitch->startCalibration();



}





void loop()
{
  // we have to call the knx.task form the konnekting library faster then ~ 400us to not miss any telegram
  // in next versions this should be obsolete but for now we have to stay with this pitfall
  // ATTENTION: Currently we do have some loops which will go above 400us due I2C handling, we have to ceck in
  // production mode if this will create any problems
  //touchButton1->task();
  //touchButton2->task();
  touchSwitch->task();

  /*
  SerialUSB.print(touchButton1->getLastSampleValue());
  SerialUSB.print(" | ");
  SerialUSB.print(touchButton1->getTriggerLevel());
  SerialUSB.print("             ");
  SerialUSB.print(touchButton2->getLastSampleValue());
  SerialUSB.print(" | ");
  SerialUSB.print(touchButton2->getTriggerLevel());
  SerialUSB.print("\n");
  */



  delay(2);


}
