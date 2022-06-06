#include "RightWing.h"

RightWing::RightWing(string robotID, MQTTClient2 *client, Controller *controller) : Robot(robotID, client, controller)
{
    basePosition = {-0.7f * sign, -1.5f * sign, 90.0f * sign};
}

void RightWing::updateRobot()
{

    switch (controller->referee)
    {
    case playing:
        playingRightWing();
        break;

    case preKickOff1:
        if (team == 2)
        {
            moveRobot(basePosition, PAUSE_SPEED);
        }
        else
        {
            direction = runUpDestination({controller->homeTeam[4]->coordinates.x,
                                          controller->homeTeam[4]->coordinates.y});
            Setpoint newPath = getPath(BALL_RADIUS + ROBOT_RADIUS + 0.1f);
            readyToKick = moveRobot(newPath, PAUSE_SPEED);
        }
        break;

    case preKickOff2:
        if (team == 2)
        {
            moveRobot(basePosition, PAUSE_SPEED);
        }
        else
        {
        }
        break;

    case kickOff:
        if (team == 2)
        {
            moveRobot(basePosition, PAUSE_SPEED);
        }
        else
        {
            if (passToRobot(5))
            {
                controller->referee = playing;
            }
        }
        break;

    case preFreeKick1:
        if (team == 2)
        {
            moveRobot(basePosition, PAUSE_SPEED);
        }
        else
        {
            moveRobot(basePosition, PAUSE_SPEED);
        }
        break;
    case preFreeKick2:
        if (team == 2)
        {
            moveRobot(basePosition, PAUSE_SPEED);
        }
        else
        {
        }
        break;
    case freeKick1:
        if (team == 2)
        {
            moveRobot(basePosition, PAUSE_SPEED);
        }
        else
        {
        }
        break;
    case freeKick2:
        if (team == 2)
        {
            moveRobot(basePosition, PAUSE_SPEED);
        }
        else
        {
        }
        break;
    case prePenaltyKick1:
        if (team == 2)
        {
            moveRobot(basePosition, PAUSE_SPEED);
        }
        else
        {
            // acá debería patear
        }
        break;
    case prePenaltyKick2:
        if (team == 2)
        {
            // acá debería patear, es messi
        }
        else
        {
            moveRobot({4, -2, -90}, PAUSE_SPEED);
        }
        break;
    case penaltyKick1:
        if (team == 2)
        {
            moveRobot(basePosition, PAUSE_SPEED);
        }
        else
        {
        }
        break;
    case penaltyKick2:
        if (team == 2)
        {
            moveRobot(basePosition, PAUSE_SPEED);
        }
        else
        {
        }
        break;
    case continueGame:
        break;
    }
}
    void RightWing::playingRightWing()
    {
        if (controller->receiver == robotID[7] - '0')
        {
            receivePass();
        }
        else if (!withBall)
        {
            moveRobot({2.0f, 1.0f, 90.0f}, MAX_SPEED);
        }
    }