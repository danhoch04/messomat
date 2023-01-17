//
// Created by Daniel Hochegger on 17.01.2023.
//
#ifndef global_def
#define global_def

#include "imports.h"

typedef struct versionControl
{
    uint8_t size;
    uint8_t MajorVersion;
    uint8_t MinorVersion;
    uint8_t HeaderLength;
    uint8_t TransmitInterval;
    uint8_t CRC;
} versionControl;

#endif