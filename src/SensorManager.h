#pragma once

struct SensorData {
    float temperature;
    float humidity;
    float distance;
};

class SensorManager {
public:
   SensorManager();
   bool begin();
   SensorData read();

private:
   SensorData data;

};

