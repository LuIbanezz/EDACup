#include "LeftWing.h"

LeftWing::LeftWing(string robotID, MQTTClient2 *client, Controller *controller) : Robot(robotID, client, controller)
{
    basePosition = {-1.0f * sign, 1.5f * sign, 90.0f * sign};
}

void LeftWing::updateRobot()
{
    switch(controller->referee)
    {
        case playing:
            playingLeftWing();
            break;

        case preKickOff1:
            moveRobot(basePosition, PAUSE_SPEED);
            break;
        case preKickOff2:
            moveRobot(basePosition, PAUSE_SPEED);
            break;
        case kickOff:
            if(controller->receiver == robotID[7]-'0')
            {
                Vector2 robotToBall = {controller->ball.position.x -coordinates.x,
                controller->ball.position.y - coordinates.y};
                setSetpoint({coordinates.x, coordinates.y, 90.0f -
                Vector2Angle({0,0}, robotToBall)});
            }
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

void LeftWing::playingLeftWing()
{
    if(controller->receiver == robotID[7]-'0')
    {
        receivePass();
    }
}