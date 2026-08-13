#pragma once

#include <Arduino.h>

class OTAManager{
public:
bool update(const char* firmwareURL);
}