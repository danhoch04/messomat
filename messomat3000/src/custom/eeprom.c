//
// Created by Daniel Hochegger on 17.01.2023.
//
#include "custom/eeprom.h"

extern versionControl vc;

void eeprom_init()
{
    eeprom_read();
}

void eeprom_save()
{
    vc.CRC = crc_8((const unsigned char *)&vc, vc.size - 1);
    eeprom_update_block(&vc, 0, vc.size); // was soll gespeichert werden, wo ist die länge der daten
}

void eeprom_read()
{
    eeprom_read_block(&vc, 0, vc.size); // was soll gespeichert werden, wo ist die länge der daten
}