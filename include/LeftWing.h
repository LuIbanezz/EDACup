#ifndef LEFTWING_H
#define LEFTWING_H

#include "Robot.h"
using namespace std;
class LeftWing : public Robot
{
    public:
    LeftWing(string robotID, MQTTClient2 *client, Controller *controller);
    void updateRobot();
};

#endif