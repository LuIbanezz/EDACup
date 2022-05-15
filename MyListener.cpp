/**
 * @file MyListener.cpp
 * @author Agrippino, Cilfone, Di Sanzo, Hertter, Ibañez
 * @brief 
 * @version 0.1
 * @date 2022-05-10
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <iostream>

#include "MyListener.h"

using namespace std;

MyListener::MyListener(Controller* controller)        // 
{
    this->controller = controller;
}

void MyListener::onMessage(string topic, vector<char> payload)
{
    vector<float> decodedMessage = decode(payload);
    if (topic.find("robot") != -1)
    {
        //Deja los numeros de equipo (del payload) en 1 y 2
        int robotTeam = topic[5] - '0';
        //Deja los indices de los robots (del payload) en 0,1,2...
        int robotIndex = topic[7] - '1';

        //TODO: sacar el switch y poner un arreglo de teams de la misma forma que hacemos con robots 
       
        controller->assignRobotMessage(robotTeam, robotIndex, decodedMessage, topic);
            
        //cout << controller->team1[robotIndex]->coordinates.x << endl;        //PRUEBA (imprime coordenadas)
    }
    else // en este caso, el decodedMessage es el estado de la pelota
    {
        controller->updateController(decodedMessage);

    }

    //TODO: usar la posicion de la pelota para actualizar al equipo (SIN GUARDAR EL DATO)


}



//void Robot::setSetpoint(Setpoint setpoint)
//{
//    this->setpoint = setpoint;
//
//    vector<char> payload(12);
//
//    *((float*)&payload[0]) = setpoint.position.x;
//    *((float*)&payload[4]) = setpoint.position.y;
//    *((float*)&payload[8]) = setpoint.rotation;
//
//    mqttClient->publish(robotId + "/pid/setpoint/set", payload);
//}


vector<float> MyListener::decode(vector<char> vecChar)
{
    vector<float> vecFloat;
    float flaux;
    char* ptrAuxChar = (char*)&flaux;

    for (int i = 0; i < (vecChar.size() / 4); i++)
    {
        for (int j = 0; j < 4; j++)
            //cambio btb el float auxiliar
            ptrAuxChar[j] = vecChar[(i * 4) + j]; 

        vecFloat.push_back(flaux);
    }
    return vecFloat;
}