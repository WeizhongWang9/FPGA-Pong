#include "paddle.h"

	paddle::paddle(int loc)
	{
		reset(loc);
	}

	paddle::~paddle(){};

	void paddle::move(float timepass)
	{
		y = y + dir_y * paddleSpeed;
	}

	void paddle::setSpeed(int speed)
	{
		paddleSpeed = speed;
	}

	void paddle::setDirection(float dir_y)
	{
		dir_y = dir_y;
	}

	void paddle::reset(int loc)
	{
		x = loc ? resetX2 : resetX1;
		y = resetY;
		dir_y = 0;
	}



