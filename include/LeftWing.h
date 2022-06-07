/**
 * @file LeftWing.h
 * @author Agrippino, Cilfone, Di Sanzo, Hertter, Ibañez
 * @brief LeftWing Forward class declaration
 * @version 0.1
 * @date 2022-06-06
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef LEFTWING_H
#define LEFTWING_H

#include "Robot.h"
using namespace std;
class LeftWing : public Robot
{
public:
    LeftWing(string robotID, MQTTClient2 *client, Controller *controller);
    void updateRobot();

private:
    void LeftWing::playingLeftWing();
};

#endif