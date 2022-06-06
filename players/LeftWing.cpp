#include "LeftWing.h"

LeftWing::LeftWing(string robotID, MQTTClient2 *client, Controller *controller) : Robot(robotID, client, controller)
{
    basePosition = {-1.0f * sign, 1.5f * sign, 90.0f * sign};
    outPosition = {-2.5f * sign, -4.0f, 0};
}

void LeftWing::updateRobot()
{
    if(!removed)
    {
        switch (controller->referee)
        {
        case playing:
            playingLeftWing();
            break;

        case preKickOff1:
            withBall = false;
            moveRobot(basePosition, PAUSE_SPEED);
            break;
        case preKickOff2:
            withBall = false;
            moveRobot(basePosition, PAUSE_SPEED);
            break;
        case kickOff:
            if (controller->receiver == robotID[7] - '0')
            {
                Vector2 robotToBall = {controller->ball.position.x - coordinates.x,
                                       controller->ball.position.y - coordinates.y};
                setSetpoint({coordinates.x, coordinates.y, 90.0f - Vector2Angle({0, 0}, robotToBall)});
            }
            break;

        case preFreeKick1:
        {
            Vector2 ballPosition = {controller->ball.position.x, controller->ball.position.y};
            Vector2 baseToBall = {basePosition.position.x - ballPosition.x,
                                  basePosition.position.y - ballPosition.y};

            if (Vector2Length(baseToBall) < 0.6f)
            {
                Vector2 newBasePosition = Vector2Add({basePosition.position.x,
                                                      basePosition.position.y},
                                                     Vector2Scale(Vector2Normalize(baseToBall), 0.6f));

                moveRobot({{newBasePosition}, 90.0f}, PAUSE_SPEED);
            }
            else
            {
                moveRobot(basePosition, PAUSE_SPEED);
            }
        }
        break;

        case preFreeKick2:
        {
            Vector2 ballPosition = {controller->ball.position.x, controller->ball.position.y};
            Vector2 baseToBall = {basePosition.position.x - ballPosition.x,
                                  basePosition.position.y - ballPosition.y};

            if (Vector2Length(baseToBall) < 0.6f)
            {
                Vector2 newBasePosition = Vector2Add({basePosition.position.x,
                                                      basePosition.position.y},
                                                     Vector2Scale(Vector2Normalize(baseToBall), 0.6f));

                moveRobot({{newBasePosition}, 90.0f}, PAUSE_SPEED);
            }
            else
            {
                moveRobot(basePosition, PAUSE_SPEED);
            }
        }
        break;

        case removeRobot1:
            if (team == 1)
            {
                if (controller->removedPlayers == LeftWingRemoval)
                {
                    removeRobot();
                }
                break;
            }
        case removeRobot2:
            if (team == 2)
            {
                if (controller->removedPlayers == LeftWingRemoval)
                {
                    removeRobot();
                }
                break;
            }
            break;
        case addRobot1:
            if (team == 1 && controller->removedPlayers == LeftWingRemoval - 1)
            {
                returnRobot();
                moveRobot(basePosition, PAUSE_SPEED);
            }
            break;
        case addRobot2:

            moveRobot(basePosition, PAUSE_SPEED);
            break;
            {
                if (Vector3Distance(controller->ball.position, coordinates) < 0.5f)
                {
                    moveRobot({basePosition.position.x - 0.5f,
                               basePosition.position.y, basePosition.rotation},
                              PAUSE_SPEED);
                }
            }
            break;
        case freeKick1:
            break;
        case freeKick2:
            break;
        case prePenaltyKick1:
        case penaltyKick1:
            if (team == 2)
            {
                moveRobot({-4, -2, -90}, PAUSE_SPEED);
            }
            else
            {
                moveRobot({-4, 2, -90}, PAUSE_SPEED);
            }
            break;

        case prePenaltyKick2:
        case penaltyKick2:
            if (team == 2)
            {
                moveRobot({4, -2, -90}, PAUSE_SPEED);
            }
            else
            {
                moveRobot({4, 2, -90}, PAUSE_SPEED);
            }
            break;
        case continueGame:
            break;
        case pauseGame:
            break;
        }
    }
    else if(controller->removedPlayers < LeftWingRemoval)
    {
        removed = false;
        returnRobot();
    }
    else if(controller->removedPlayers >= LeftWingRemoval)
    {
        removed = true;
        removeRobot();
    }
}
void LeftWing::playingLeftWing()
{
    if(controller->receiver == robotID[7]-'0')
    {
        if(receivePass())
        {
            auxTime = controller->getTime();
        }
    }
    else if(withBall)
    {
        // stopDribble();
        // if(!readyToKick)
        // {
        //     controller->receiver = 6;
        //     Vector3 receiverPosition = controller->homeTeam[controller->receiver - 1]->coordinates;
        //     direction = runUpDestination({receiverPosition.x, receiverPosition.y});
        //     Setpoint newPath = getPath(BALL_RADIUS + ROBOT_RADIUS + 0.1f);
        //     moveRobot(newPath, PAUSE_SPEED);
        //     if(Vector3Distance(coordinates, controller->ball.position) < 0.05f)
        //     {
        //         readyToKick = true;
        //     }
        // }
        // else
        // {
        //     passToRobot(6);
        // }
        if(controller->getTime() - auxTime < 1.0f)
        {
            stopDribble();
        }
        if(controller->getTime() - auxTime > 1.0f && controller->getTime() - auxTime < 2.0f)
        {
            startDribble();
            controller->receiver = 6;
            Vector3 receiverPosition = controller->homeTeam[controller->receiver - 1]->coordinates;
            Vector2 robotToReceiver =
            {receiverPosition.x - coordinates.x,
            receiverPosition.y - coordinates.y};
            float rotationAngle= 90.0f - Vector2Angle({0,0}, robotToReceiver);
            setSetpoint({coordinates.x, coordinates.y, rotationAngle});
        }
        if(controller->getTime() - auxTime > 2.0f )
        {
            if(passToRobot(6))
            {
                stopDribble();
                withBall = false;
            }
        }
        
    }
    else if(!withBall)
    {

    }
}