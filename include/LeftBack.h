/**
 * @file LeftBack.h
 * @author Agrippino, Cilfone, Di Sanzo, Hertter, Ibañez
 * @brief LeftBack defender class declaration
 * @version 0.1
 * @date 2022-06-06
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef LEFTBACK_H
#define LEFTBACK_H

#include "Robot.h"
using namespace std;
class LeftBack : public Robot
{
public:
    LeftBack(string robotID, MQTTClient2 *client, Controller *controller);
    void updateRobot();
};

#endif
