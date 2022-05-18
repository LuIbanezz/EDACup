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
#include "raylib.h"



typedef vector<Robot*> team;

using namespace std;

int main(int argc, char** argv)
{
    Controller controller;
    MyListener listener(&controller);
    MQTTClient2 mqttClient2;
    mqttClient2.setListener(&listener);
    
    const int port = 1883;
    if (!mqttClient2.connect("controller","127.0.0.1", port, "user", "vdivEMMN3SQWX2Ez"))
    {
        cout << "Could not connect." << endl;
        return 1;
    }
    
    cout << "Connected." << endl;
    mqttClient2.subscribeToTopics();
    controller.createTeam1(&mqttClient2);
    
    controller.start();

    mqttClient2.run();
    

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
    cout << "Disconnected." << endl;
}


