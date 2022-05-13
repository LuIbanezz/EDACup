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
#include "MyListener.h"


#define ASCII_NUMBER_CONST 48
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
        string topic = "robot"; topic += EQUIPO; topic += "."; topic += i + ASCII_NUMBER_CONST;
        mqttClient2.subscribe(topic + "/display/leftEye/set");
        for (char c = '1'; c < ('1' + 4); c++)
        {
            mqttClient2.subscribe(topic + "/motor" + c + "/current");
            mqttClient2.subscribe(topic + "/motor" + c + "/rpm");
            mqttClient2.subscribe(topic + "/motor" + c + "/temperature");
        }
    
    mqttClient2.subscribe(topic + "/display/leftEye/set");
    string holis = topic + "/display/leftEye/set";
    mqttClient2.subscribe(topic + "/display/rightEye/set");
    mqttClient2.subscribe("ball/motion/state");
    mqttClient2.subscribe(topic + "/motion/state");
    }
    MyListener listener;
    mqttClient2.setListener(&listener); 
    mqttClient2.run();
    // MQTTListener mensajes;
    // mensajes = cliente.getMessages();

    // for (auto i : mensajes)
    // {
    //     size_t indice;
    //     if ((indice = i.topic.find("motor")) != -1)
    //     {
    //     int motor = i.topic.at(indice + 5) - '1'; // se obtiene el número de motor

    //     if (i.topic.find("voltage") != -1)
    //         tensionMotoresRead[motor] = vecToFloat(i.payload);
    //     if (i.topic.find("current") != -1)
    //         corrienteMotoresRead[motor] = vecToFloat(i.payload);
    //     if (i.topic.find("rpm") != -1)
    //         rpmMotores[motor] = vecToFloat(i.payload);
    //     if (i.topic.find("temperature") != -1)
    //         tempMotores[motor] = vecToFloat(i.payload);
    //     }
    // }


}
