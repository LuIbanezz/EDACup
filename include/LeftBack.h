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
