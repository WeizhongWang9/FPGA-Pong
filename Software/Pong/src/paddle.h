
class paddle
{
public:
	float x = 0,y = 0;
	float dir_y = 0;
	float paddleSpeed = 0.03; // pixels per second

	int resetX1 = 80,resetY = 240;
	int resetX2 = 540;

	paddle(int loc);
	~paddle();

	void move(float timepass);
	void setSpeed(int speed);
	void setDirection(float dir_y);

	void reset(int loc);

};
