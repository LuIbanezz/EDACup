#include "Keeper.h"

Keeper::Keeper(string robotID, MQTTClient2 *client, Controller *controller) : Robot(robotID, client,controller)
{
    
}

void Keeper::updateRobot()
{
    //positionGK();
}

void Keeper :: positionGK()
{
    shot=false;
    Vector2 ball2D = {controller->ball.position.x, controller->ball.position.y};
   
    // Define the angles between the ball and the goal
    Vector2 ballTo1st = Vector2Subtract(goal1_1, ball2D);
    float angleTo1st = angleBetweenVectors2(ballTo1st,{TEAM_SIGN * 1, 0});
    Vector2 ballTo2nd = Vector2Subtract(goal1_2, ball2D);
    float angleTo2nd = angleBetweenVectors2(ballTo2nd, {TEAM_SIGN * 1, 0});
    

    //Define the angle of the ball trajectory
    Vector2 ballSpeed2D = {controller->ball.speed.x, controller->ball.speed.y};
    float shotAngle = angleBetweenVectors2(ballSpeed2D, {TEAM_SIGN * 1, 0});
    
    if ((TEAM_SIGN) * shotAngle <= (TEAM_SIGN) * (angleTo1st - 5)
    && (((TEAM_SIGN) * shotAngle) >= (TEAM_SIGN) * (angleTo2nd + 5))
    && (Vector2Length(ballSpeed2D) > SHOT_SPEED))
    {
        shot = true;
    }


    Setpoint keeperSetPoint;
    if (shot)
    {
        float angleTo2nd = angleBetweenVectors2(ballTo2nd, {TEAM_SIGN * 1, 0});
        //Define the line that follows the trajectory of the ball when shot
        float shotSlope = tan(TEAM_SIGN* shotAngle * PI / 180);
        //Define the diving trajectory of the keeper (at a right angle with ball trajectory)
        Vector3 diveVector = Vector3Scale(Vector3CrossProduct(controller->ball.speed,{0,0,-1}),
        TEAM_SIGN);
        
        Vector2 diveVector2D={diveVector.x,diveVector.y};
        float diveAngle = angleBetweenVectors2(diveVector2D,{TEAM_SIGN * 1, 0}) * PI/180;
        float diveSlope = tan(diveAngle);

        float x = (coordinates.y - diveSlope * coordinates.x - ball2D.y+shotSlope*ball2D.x)
        / (shotSlope-diveSlope);
        float y = ball2D.y + shotSlope * (x - ball2D.x);
        Setpoint keeperSetPoint = {{x, y},90};
        
        setSetpoint(keeperSetPoint);
    }
    else if (sign=1 && (ball2D.x <= -3.5) && (ball2D.x >= -4.5)
             && (ball2D.y >= -1) && (ball2D.y <= 1))
    {
        ballInGKArea();
    }
    else if (sign=-1 && (ball2D.x >= 3.5) && (ball2D.x <= 4.5)
             && (ball2D.y >= -1) && (ball2D.y <= 1))
    {
        ballInGKArea();
    }
    else
    {
        
        float bisector = (angleTo2nd + angleTo1st) * 0.5;
        bisector *= PI / 180;
        float slope = tan(TEAM_SIGN *bisector);
        float forwardStep = FORWARD_STEP(ball2D.x);
        if (forwardStep <= 0)
            forwardStep = 0;
        float x = sign*(-4.5+0.5);
        float y = ball2D.y + slope * (x - ball2D.x);
        moveRobot({{x, y},0}, MAX_SPEED);
    }
}

void Keeper::ballInGKArea()
{
    // Just for testing
    moveRobot({controller->ball.position.x, controller->ball.position.y}, MAX_SPEED);
}

