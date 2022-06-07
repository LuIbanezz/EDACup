/**
 * @file RightWing.h
 * @author Agrippino, Cilfone, Di Sanzo, Hertter, Ibañez
 * @brief Right back defedender class declaration
 * @version 0.1
 * @date 2022-06-06
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef RIGHTWING_H
#define RIGHTWING_H
#include "Robot.h"

class RightWing : public Robot
{
public:
    RightWing(string robotID, MQTTClient2 *client, Controller *controller);
    void updateRobot();

private:
    void playingRightWing();
    Vector2 calculateGoalDestination();
};

#endif