/*
  EEPROMW.h - Library for EEPROM cycling to reduce wear.
  Created by Joe Jackson, January 19, 2021.
  Released into the public domain.
*/
#ifndef EEPROMW_h
#define EEPROMW_h

#include <Arduino.h>
#include <EEPROM.h>


#define MAX_ADDR EEPROM.length() - 2
#define MIN_ADDR 2

class EEPROMW
{
public:
    EEPROMW();
    void init();
    byte read();
    void write(byte v);
    
private:
    uint16_t _Current;
    uint16_t _Writes;
    uint16_t currentAddr();
    uint16_t findAddr();
    void configEeprom();
    uint16_t writeCount();
    void addWrite();
    void moveData(uint16_t addr);
    void pad(uint16_t addr);

};

#endif
