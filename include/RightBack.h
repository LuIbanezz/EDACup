/**
 * @file RightBack.h
 * @author Agrippino, Cilfone, Di Sanzo, Hertter, Ibañez
 * @brief Right back defedender class declaration
 * @version 0.1
 * @date 2022-06-06
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef RIGHTBACK_H
#define RIGHTBACK_H
#include "Robot.h"
using namespace std;

class RightBack : public Robot
{
public:
    RightBack(string robotID, MQTTClient2 *client, Controller *controller);
    void updateRobot();
};

#endif
