/**
 * @file main.cpp
 * @author Agrippino, Cilfone, Di Sanzo, Hertter, Ibañez
 * @brief EDACup controller Main module
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


using namespace std;

int main(int argc, char** argv)
{
    char equipo = argv[1][0];

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
    if (equipo == '1')
    {
        controller.createTeam1(&mqttClient2); 
        controller.team = 1;
    }
    else
    {
        controller.createTeam2(&mqttClient2);
        controller.team = 2;
    }
    
    controller.createAwayTeam();
    
    controller.start();

    mqttClient2.run();
    
    cout << "Disconnected." << endl;
}


