#include "chu_init.h"

class ball
{
public:
	float x = 0,y = 0;
	float dir_x = 0,dir_y = 0;
	float ball_speed = 0.25; // pixels per milisecond

	const int resetX = 320,resetY = 240;
	const float resetSpeed = 0.25;

	ball();
	~ball();

	void move(float timepass);
	void setSpeed(float speed);
	void setDirection(float dir_x,float dir_y);

	int checkHitLeftPaddle(int paddleX,int paddleY,int paddleHeight,int paddleWidth);
	int checkHitRightPaddle(int paddleX,int paddleY,int paddleHeight,int paddleWidth);
	void checkHitTopWall(int wallUp);
	void checkHitBottomWall(int wallBottom);
	int checkOutOfBound(int wallLeft,int wallRight);
	void resetball(int direction);

private:
	void norm(float &x,float &y  );

};
