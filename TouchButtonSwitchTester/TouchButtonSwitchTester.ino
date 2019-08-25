
/*
  Konnekting - TouchButtonSwitch  Test App (0xCDDC)
  Created by Christian Dürnberger (ChriD), 2019
  https://github.com/ChriD
  App for testing the Switch Hardware
*/

#include "Arduino.h"
#include <BME280I2C.h> /*https://github.com/finitespace/BME280*/
#include <Wire.h>
#include "src/vendor/LedPattern_RGB.h"
#include "src/TouchButton.h"

// define seriald for the Adafruit Itsy-Bitsy M0
// SERIAL_BCU is mandatory to work, the SERIAL_DBG will only be used in DEBUG Mode
#define SERIAL_DBG              SERIAL_PORT_USBVIRTUAL



#define PROG_BUTTON_PIN   10

// ### RGB LED #################################################################################################
LedPattern_RGB *rgbLed;

#define PIN_RBGLED_RED    13
#define PIN_RBGLED_GREEN  12
#define PIN_RBGLED_BLUE   11

#define RED     111,255,255
#define YELLOW  111,111,255
#define GREEN   255,111,255
#define CYAN    255,111,111
#define BLUE    255,255,111
#define MAGENTA 111,255,111
#define WHITE   111,111,111
#define OFF     255,255,255

const uint8_t ledPattern_Test[] = {
     LedPattern::CMD_SET, OFF,
     LedPattern::CMD_REPEAT, LedPattern::repeatForever,
        LedPattern::CMD_FADETO, 100, RED,
        LedPattern::CMD_FADETO, 100, GREEN,
        LedPattern::CMD_FADETO, 500, BLUE,
     LedPattern::CMD_ENDREPEAT,
  };

uint64_t lastLedPatternRunTime  = 0;


// ### Speaker #################################################################################################
#define SPEAKER_OUT_PIN   A0

uint64_t lastSpeakerTestRunTime = 0;
uint64_t toneCount = 0;
bool play = true;


// ### BME280I2C ###############################################################################################
BME280I2C bme;    // Default : forced mode, standby time = 1000 ms
                  // Oversampling = pressure ×1, temperature ×1, humidity ×1, filter off,
uint64_t lastBMETestRunTime = 0;


// ### Touch Buttons ############################################################################################
TouchButton         *buttons[5];
uint64_t            lastButtonRunTime  = 0;




// this method will be called whenever a button action (tap, doubletap, longtap,..) was regognized by the
// switch library. We use this method to send the approriate data to the KNX Bus for each button action
void onButtonAction(uint16_t _buttonId, uint16_t _type, uint16_t _value)
{
  tone(SPEAKER_OUT_PIN, 4000, 250);
  SERIAL_DBG.print("Button: ");
  SERIAL_DBG.print(_buttonId);
  SERIAL_DBG.print("\t\tAction: ");
  SERIAL_DBG.print(_type);
  SERIAL_DBG.print("\t\Value: ");
  SERIAL_DBG.println(_type);
}


void setup()
{
  SERIAL_DBG.begin(115200);
  while (!SERIAL_DBG) {}

  Wire.begin();

  while(!bme.begin() && millis() < 10000)
  {
    SERIAL_DBG.println("Could not find BME280 sensor!");
    delay(1000);
  }

  switch(bme.chipModel())
  {
     case BME280::ChipModel_BME280:
       SERIAL_DBG.println("Found BME280 sensor! Success.");
       break;
     case BME280::ChipModel_BMP280:
       SERIAL_DBG.println("Found BMP280 sensor! No Humidity available.");
       break;
     default:
       SERIAL_DBG.println("Found UNKNOWN sensor! Error!");
  }


  buttons[0] = new TouchButton(A2, 1);
  buttons[1] = new TouchButton(A1, 2);
  buttons[2] = new TouchButton(A3, 3);
  buttons[3] = new TouchButton(A4, 4);
  buttons[4] = new TouchButton(A5, 5);

  buttons[0]->attachCallbackOnButtonAction(makeFunctor((CallbackFunction_ButtonAction*)0,&onButtonAction));
  buttons[1]->attachCallbackOnButtonAction(makeFunctor((CallbackFunction_ButtonAction*)0,&onButtonAction));
  buttons[2]->attachCallbackOnButtonAction(makeFunctor((CallbackFunction_ButtonAction*)0,&onButtonAction));
  buttons[3]->attachCallbackOnButtonAction(makeFunctor((CallbackFunction_ButtonAction*)0,&onButtonAction));
  buttons[4]->attachCallbackOnButtonAction(makeFunctor((CallbackFunction_ButtonAction*)0,&onButtonAction));


  buttons[0]->setup();
  buttons[1]->setup();
  buttons[2]->setup();
  buttons[3]->setup();
  buttons[4]->setup();


  pinMode(PIN_RBGLED_RED, OUTPUT);
  pinMode(PIN_RBGLED_GREEN, OUTPUT);
  pinMode(PIN_RBGLED_BLUE, OUTPUT);

  rgbLed = new LedPattern_RGB(PIN_RBGLED_RED, PIN_RBGLED_GREEN, PIN_RBGLED_BLUE);
  rgbLed->start(ledPattern_Test);

}



// if the prog button is pressed we have to switch the device into the prog mode
// we have to debounce the button because we do not have hardware debounce
volatile unsigned long progButtonDebounceTime = 0;
void progButtonPressed()
{
  unsigned long tempTime = millis();
  if(tempTime >= progButtonDebounceTime)
  {
    if(tempTime - progButtonDebounceTime < 200)
      return;
  }
  progButtonDebounceTime = tempTime;
}



void printBME280Data(Stream* client)
{
   float temp(NAN), hum(NAN), pres(NAN);
   BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
   BME280::PresUnit presUnit(BME280::PresUnit_Pa);
   bme.read(pres, temp, hum, tempUnit, presUnit);

   client->print("Temp: ");
   client->print(temp);
   client->print("°"+ String(tempUnit == BME280::TempUnit_Celsius ? 'C' :'F'));
   client->print("\t\tHumidity: ");
   client->print(hum);
   client->print("% RH");
   client->print("\t\tPressure: ");
   client->print(pres);
   client->println("Pa");
}



boolean startCalib = true;

void loop()
{


  // RGB LED TEST
  if(millis() - lastLedPatternRunTime > 10)
  {
    pinMode(PIN_RBGLED_RED, OUTPUT);
    pinMode(PIN_RBGLED_GREEN, OUTPUT);
    pinMode(PIN_RBGLED_BLUE, OUTPUT);
    rgbLed->update();
    lastLedPatternRunTime = millis();
  }


  // speaker test (beep on startup)
  if(millis() - lastSpeakerTestRunTime > 1000 && toneCount < 10)
  {
    if(play)
    {
      tone(SPEAKER_OUT_PIN, 4000, 1000);
      toneCount++;
    }
    else
    {
      tone(SPEAKER_OUT_PIN, 4500, 1000);
      toneCount++;
    }
    play = !play;
    lastSpeakerTestRunTime = millis();
  }


  // BME test
  if(millis() - lastBMETestRunTime > 2000)
  {
    printBME280Data(&SERIAL_DBG);
    lastBMETestRunTime = millis();
  }

  if(startCalib)
  {
    buttons[0]->startCalibration();
    buttons[1]->startCalibration();
    buttons[2]->startCalibration();
    buttons[3]->startCalibration();
    buttons[4]->startCalibration();
    startCalib = false;
  }

   buttons[0]->task();
   buttons[1]->task();
   buttons[2]->task();
   buttons[3]->task();
   buttons[4]->task();


  // button info
  if(!startCalib && (millis() - lastButtonRunTime) > 50)
  {
    SERIAL_DBG.print(buttons[0]->getLastSampleValue());
    SERIAL_DBG.print("    ");
    SERIAL_DBG.print(buttons[1]->getLastSampleValue());
    SERIAL_DBG.print("    ");
    SERIAL_DBG.print(buttons[2]->getLastSampleValue());
    SERIAL_DBG.print("    ");
    SERIAL_DBG.print(buttons[3]->getLastSampleValue());
    SERIAL_DBG.print("    ");
    SERIAL_DBG.println(buttons[4]->getLastSampleValue());
    lastButtonRunTime = millis();

  }

}




