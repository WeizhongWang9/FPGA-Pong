#include "chu_init.h"
#include "ball.h"
#include <math.h>
/*
	float x = 0,y = 0;
	float dir_x = 0,dir_y = 0;
	int ball_speed = 100; // pixels per second
*/

	ball::ball()
	{
		resetball(0);
	}

	ball::~ball()
	{

	};

	void ball::move(float timepass)
	{
		x = x + dir_x * timepass * ball_speed;
		y = y + dir_y * timepass * ball_speed;
	} //based on real time

	void ball::setSpeed(float speed)
	{
		ball_speed = speed;
	}

	void ball::setDirection(float dx,float dy) //auto normalize
	{
		norm(dx,dy);
		dir_x = dx;
		dir_y = dy;
	}

	int ball::checkHitLeftPaddle(int paddleX,int paddleY,int paddleHeight,int paddleWidth)
	{
		if (x < paddleX + paddleWidth &&
		        x > paddleX &&
		        y < paddleY + paddleHeight &&
		        y > paddleY)
		{

		        // (t is 0.5 if hit at top, 0 at center, -0.5 at bottom)
		        float t = ((y - paddleY) / (float)paddleHeight) - 0.5f;

		        setDirection(fabs(dir_x),t);
		        return 1;
		}
		return 0;
	}

	int ball::checkHitRightPaddle(int paddleX,int paddleY,int paddleHeight,int paddleWidth)
	{
	    if (x > paddleX &&
	        x < paddleX + paddleWidth &&
	        y < paddleY + paddleHeight &&
	        y > paddleY) {

	        // (t is 0.5 if hit at top, 0 at center, -0.5 at bottom)
	        float t = ((y - paddleY) / (float)paddleHeight) - 0.5f;
	        setDirection(-1 * fabs(dir_x),t);
	        return 1;
	    }
	    return 0;
	}

	void ball::checkHitTopWall(int wallUp)
	{
	    if (y < wallUp) {
	    	setDirection(dir_x,fabs(dir_y));
	    }
	}
	void ball::checkHitBottomWall(int wallBottom)
	{
	    if (y > wallBottom) {
	    	setDirection(dir_x,-fabs(dir_y));
	    }
	}

	int ball::checkOutOfBound(int wallLeft,int wallRight)
	{
		if(x < wallLeft)
			return -1;
		else if (x > wallRight)
			return 1;
		else
			return 0;
	}

	void ball::resetball(int direction)
	{
		x = resetX;
		y = resetY;
		ball_speed = resetSpeed;
		dir_x = direction ? 1 : -1;
		dir_y = 0;
	}

	void ball::norm(float &x,float &y  )
	{
        float length = sqrt((x * x) + (y * y));
        if (length != 0.0f) {
            length = 1.0f / length;
            x *= length;
            y *= length;
        }
	}

