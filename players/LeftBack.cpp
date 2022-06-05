#include "LeftBack.h"

LeftBack::LeftBack(string robotID, MQTTClient2 *client, Controller *controller) : Robot(robotID, client, controller)
{
    basePosition = {-2.5f * sign, 1.0f * sign, 90.0f * sign};
}

void LeftBack::updateRobot()
{
    switch(controller->referee)
    {
        case preKickOff1:
            moveRobot(basePosition, PAUSE_SPEED);
            break;
        case preKickOff2:
            moveRobot(basePosition, PAUSE_SPEED);
            break;
        case kickOff1:
            break;
        case kickOff2:
            break;
        case preFreeKick1:
            break;
        case preFreeKick2:
            break;
        case freeKick1:
            break;
        case freeKick2:
            break;
        case prePenaltyKick1:
            break;
        case prePenaltyKick2:
            break;
        case penaltyKick1:
            break;
        case penaltyKick2:
            break;
        case continueGame:
            break;
        case removeRobot1:
            break;
        case removeRobot2:
            break;
        case addRobot1:
            break;
        case addRobot2:
            break;
    }
}