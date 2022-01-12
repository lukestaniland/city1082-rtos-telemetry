#include "mbed.h"
#include "display.h"
#include <cmath>

#define THERM_GND P10_0 //Defines P10_0 as THERM_GND in code, to make code more readable
#define THERM_VCC P10_3 //Defines P10_3 as THERM_VCC in code, to make code more readable
#define THERM_OUT P10_1 //Defines P10_1 as THERM_OUT in code, to make code more readable

#define R_REFERENCE int(10000)
#define A_COEFF float(0.0009032679)
#define B_COEFF float(0.000248772)
#define C_COEFF float(2.041094E-07)
#define ABSOLUTE_ZER float(-273.15)

 AnalogIn tempVoltage(THERM_OUT);
 float temperatureC;
/* Send Thread */

void sendThread(void)
{
    // this is for reading the thermistor. Setting ground and vcc as outputs, and tempVoltage as an input
    DigitalOut ground(THERM_GND); 
    DigitalOut vcc(THERM_VCC);
    // initialise the thermistor power. Setting vcc as 1 and ground as 0, to give 3.3V across the thermistor
    vcc= true;
    ground = false;

    float    temp;  // AD result of measured voltage 
    float    lightLev;   // AD result of measured current
    int      cycles;       // A counter value               
    uint32_t i = 0;
    while (true) {
        i++; // fake data update
        temp = readTemp(); //f is added to floating point numbers
        lightLev = fmod((i * 0.1f) * 5.5f, 100); //dummy value
        cycles = i;
        displaySendUpdateSensor(temp, lightLev, cycles); //sends temp, lightLev and cycles to a queue for displaying
        ThisThread::sleep_for(1s);
        
    }
}

float readTemp()
{
    float temperatureF;
    float refVoltage = tempVoltage.read() * 2.4; // Range of ADC 0->2*Vref
    float refCurrent = refVoltage  / R_REFERENCE; // 10k Reference Resistor, SWAP OUT R_REFERENCE
    float thermVoltage = 3.3 - refVoltage;    // Assume supply voltage is 3.3v
    float thermResistance = thermVoltage / refCurrent; 
    float logrT = (float32_t)log((float64_t)thermResistance);
    /* Calculate temperature from the resistance of thermistor using Steinhart-Hart Equation */
    float stEqn = (float32_t)((A_COEFF) + ((B_COEFF) * logrT) + //A_COEFF, B_COEFF
                             ((C_COEFF) * pow((float64)logrT, (float32)3))); //C_COEFF
    temperatureC = (float32_t)(((1.0 / stEqn) - 273.15)  + 0.5);
    temperatureF = (temperatureC * 9.0/5.0) + 32;
    return temperatureC;

}
