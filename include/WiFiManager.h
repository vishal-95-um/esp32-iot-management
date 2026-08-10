#pragma once

#include "NVSManager.h"
#include "Provisioning.h"

class WiFiManager{
public:
   WiFiManager(NVSManager& nvs, ProvisionManager& provision);
   bool connect();
private:
   NVSManager& nvs;
   ProvisionManager& provision;
}