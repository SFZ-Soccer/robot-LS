#include <Arduino.h>
#define NUM_SENSORS 16

float irValue[NUM_SENSORS] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
const float irXcon[NUM_SENSORS] = { 
    1.0000, 0.9239, 0.7071, 0.3827, 0.0000, -0.3827, -0.7071, -0.9239, 
    -1.0000, -0.9239, -0.7071, -0.3827, -0.0000,  0.3827,  0.7071,  0.9239 
};

const float irYcon[NUM_SENSORS] = { 
    0.0000, 0.3827, 0.7071, 0.9239, 1.0000,  0.9239,  0.7071,  0.3827, 
    0.0000, -0.3827, -0.7071, -0.9239, -1.0000, -0.9239, -0.7071, -0.3827 
};


float powerY = 0;
float powerX = 0;

int irmove() {
    get_irValue();
    get_irX();
    get_irY();
}   

//--Nebenfunktionen von IRmove ------------------------

void get_irValue() {

}

void get_irX() {
    for (int i = 0; i < NUM_SENSORS; i++)
    {
        powerX += irValue[i] * irXcon[i];
    }
    powerX / NUM_SENSORS;
}

void get_irY() {
    for (int i = 0; i < NUM_SENSORS; i++)
    {
        powerY += irValue[i] * irYcon[i];
    }

    powerY / NUM_SENSORS;
}       