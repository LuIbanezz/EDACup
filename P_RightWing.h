#ifndef RIGHTWING_H
#define RIGHTWING_H
#include "Robot.h"

class RightWing : public Robot
{
    public:
    RightWing(string robotID, MQTTClient2 *client, Controller *controller);
    void updateRobot();
};

#endif