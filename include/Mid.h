/**
 * @file Mid.h
 * @author Agrippino, Cilfone, Di Sanzo, Hertter, Ibañez
 * @brief Mid player class declaration
 * @version 0.1
 * @date 2022-06-06
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef MID_H
#define MID_H

#include "Robot.h"

using namespace std;

class Mid : public Robot
{
public:
    Mid(string robotID, MQTTClient2 *client, Controller *controller);
    void updateRobot();
};

#endif