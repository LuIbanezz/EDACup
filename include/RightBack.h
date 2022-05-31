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
