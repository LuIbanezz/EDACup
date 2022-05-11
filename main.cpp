/**
 * @file main.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-05-10
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <iostream>
#include <string>
#include "MQTTClient2.h"

#define EQUIPO      "1"
#define ROBOT_NUMBER  6

using namespace std;

int main(int argc, char** argv)
{
    MQTTClient2 mqttClient2;

    const int port = 1883;
    if (!mqttClient2.connect("controller","127.0.0.1", port, "user", "vdivEMMN3SQWX2Ez"))
    {
        cout << "Could not connect." << endl;
        return 1;
    }

    cout << "Connected." << endl;

    for(int i = 1; i <= ROBOT_NUMBER; i++)
    {
        string topic = "robot"; topic += EQUIPO; topic += "."; topic += i;
        mqttClient2.subscribe(topic + "/display/leftEye/set");
        for (char c = '1'; c < ('1' + 4); c++)
        {
            mqttClient2.subscribe(topic + "/motor" + c + "/current");
            mqttClient2.subscribe(topic + "/motor" + c + "/rpm");
            mqttClient2.subscribe(topic + "/motor" + c + "/temperature");
        }
    mqttClient2.subscribe(topic + "/display/leftEye/set");
    mqttClient2.subscribe(topic + "/display/rightEye/set");
    }
     


}
