/*
  EEPROMW.cpp - Library for EEPROM cycling to reduce wear.
  Created by Joe Jackson, January 19, 2021.
  Released into the public domain.
*/

#include <Arduino.h>
#include <EEPROM.h>
#include "EEPROMW.h"

EEPROMW::EEPROMW()
{
	init();
}

void EEPROMW::init()
{
    uint16_t addr = findAddr();
    if (addr == 1025) 
    {
        configEeprom();
    }
    else 
    {
        _Current = addr;
        _Writes = addr + 1;
    }
}
uint16_t EEPROMW::currentAddr()
{
    return _Current;
}

byte EEPROMW::read()
{
    return EEPROM.read(EEPROMW::currentAddr());
}

void EEPROMW::write(byte v) 
{
    EEPROM.write(currentAddr(), v);
    addWrite();
}

uint16_t EEPROMW::findAddr()
{
    uint16_t r = 0;
    bool found = false;
    for (uint16_t i = 0; i < EEPROM.length() - 2; i++) 
    {
        uint8_t data = EEPROM.read(i);
        if (data == 0xdd && EEPROM.read(i + 1) == 0xdd) 
        {
            r = i + 2;
            found = true;
            break;
        }
    }
    return (found) ? r : 1025;
}

void EEPROMW::configEeprom()
{
    // erase the eeprom...
    for (int i = 0; i < EEPROM.length(); i++) 
    {
        EEPROM.write(i, 0xff);
    }

    uint16_t addr = MAX_ADDR;
    pad(addr);
    _Current = addr;
    _Writes = addr + 1;
}

void EEPROMW::addWrite() 
{
    uint16_t count = writeCount() + 1;
    if (count > 65000) 
    {
        moveData(_Current);
    }
    else 
    {
        uint8_t lsb = count & 0xff;
        uint8_t msb = count >> 8;
        EEPROM.write(_Writes, lsb);
        EEPROM.write(_Writes + 1, msb);
    }
}

uint16_t EEPROMW::writeCount() 
{
    uint8_t x = EEPROM.read(_Writes);
    uint8_t y = EEPROM.read(_Writes + 1);
    uint16_t r = x | y << 8;
    return r;
}

void EEPROMW::moveData(uint16_t addr) 
{
    uint16_t newAddr = addr - 3;
    if (newAddr >= MIN_ADDR) 
    {
        EEPROM.write(newAddr, EEPROM.read(addr));
        pad(newAddr);

        // erase old data...
        EEPROM.write(addr, 0xff);
        EEPROM.write(addr + 1, 0xff);
        EEPROM.write(addr + 2, 0xff);
    }
    else 
    {
        while (1); // he's dead jim, buy a new one...
    }
}

void EEPROMW::pad(uint16_t addr)
{
    EEPROM.write(addr + 1, 0);
    EEPROM.write(addr + 2, 0);
    EEPROM.write(addr - 1, 0xdd);
    EEPROM.write(addr - 2, 0xdd);
}