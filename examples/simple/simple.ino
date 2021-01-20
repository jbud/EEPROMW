/**
 * WARNING: this example recursively re-writes data on your eeprom, 
 * if left running long enough it WILL eventually wear your eeprom beyond repair...
 *
 * Used properly eepromw can increase your eeprom lifetime by 
 * up to 220x a normal eeprom.
 *
 */

#include <EEPROMW.h>




EEPROMW eepromw();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  eepromw.write(255);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(eepromw.read());
  eepromw.write(eepromw.read()-1);
}