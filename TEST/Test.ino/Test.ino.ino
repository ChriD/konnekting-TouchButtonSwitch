

#include <SPI.h>

/* Circuit
SCK: 13
MISO: 12 
MOSI: 11
SS: 9
*/



void setup() 
{
 Serial.begin(9600);
 SPI.begin();
}

  

void loop() 
{
  SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));
  SPI.endTransaction();


  delay(100); 
}
