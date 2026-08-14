#include "SensorManager.h"

SensorManager::SensorManager() {
    data.distance = 0;
    data.humidity = 0;
    data.temperature = 0;

}

bool SensorManager::begin() {
   

    // Sensor Initialization

    return true;


}

SensorData SensorManager::read() {
    // Sensor reading logic

    data.temperature = 32.3;
    data.humidity = 56.3;
    data.distance = 21;

    return data;
}
