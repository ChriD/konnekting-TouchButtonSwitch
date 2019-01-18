/*
  SetupFlashMemory.h
  Created by Christian Dürnberger (ChriD), 2019
  Released into the public domain.

  used to setup the flash memory for the Konnekting KonnektingDevice
  change something here if you want to use some SPI flash or equivalent
*/


#include "KonnektingDevice.h"
#include <FlashAsEEPROM.h>


byte readMemory(int index) {
    Debug.println(F("FLASH read on index %d"),index);
    return EEPROM.read(index);
}

void writeMemory(int index, byte val) {
    Debug.println(F("FLASH write value %d on index %d"),val, index);
    EEPROM.write(index, val);
}

void updateMemory(int index, byte val) {
    Debug.println(F("FLASH update"));
    if (EEPROM.read(index) != val) {
        EEPROM.write(index, val);
    }
}

void commitMemory() {
    Debug.println(F("FLASH commit"));
    EEPROM.commit();
}


void setupFlashMemory()
{
    Konnekting.setMemoryReadFunc(&readMemory);
    Konnekting.setMemoryWriteFunc(&writeMemory);
    Konnekting.setMemoryUpdateFunc(&updateMemory);
    Konnekting.setMemoryCommitFunc(&commitMemory);
}