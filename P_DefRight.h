#ifndef DEFRIGHT_H
#define DEFRIGHT_H
#include "Robot.h"
using namespace std;

class RightDef : public Robot
{
    public:
    RightDef(string robotID, MQTTClient2 *client, Controller *controller);
    void updateRobot();
};

#endif
