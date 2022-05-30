#ifndef DEFLEFT_H
#define DEFLEFT_H

#include "Robot.h"
using namespace std;
class LeftDef : public Robot
{
    public:
    LeftDef(string robotID, MQTTClient2 *client, Controller *controller);
    void updateRobot();
};

#endif
