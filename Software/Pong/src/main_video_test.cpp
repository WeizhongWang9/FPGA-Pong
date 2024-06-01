/*****************************************************************//**
 * @file main_video_test.cpp
 *
 * @brief Basic test of 4 basic i/o cores
 *
 * @author p chu
 * @version v1.0: initial release
 *********************************************************************/

//#define _DEBUG
#include "chu_init.h"
#include "gpio_cores.h"
#include "vga_core.h"
#include "sseg_core.h"
#include "playground.h"
#include "ps2_core.h"
#include "ball.h"
#include "paddle.h"
#include "spi_core.h"

// external core instantiation
GpoCore led(get_slot_addr(BRIDGE_BASE, S2_LED));

FrameCore frame(FRAME_BASE);
GpvCore bar(get_sprite_addr(BRIDGE_BASE, V7_BAR));
GpvCore gray(get_sprite_addr(BRIDGE_BASE, V6_GRAY));
GpvCore background(get_sprite_addr(BRIDGE_BASE, V5_USER5));
PlayGroundCore playGround(get_sprite_addr(BRIDGE_BASE, V4_USER4), 0);
SpriteCore ghost(get_sprite_addr(BRIDGE_BASE, V3_GHOST), 1024);
SpriteCore mouse(get_sprite_addr(BRIDGE_BASE, V1_MOUSE), 1024);
OsdCore osd(get_sprite_addr(BRIDGE_BASE, V2_OSD));
SsegCore sseg(get_slot_addr(BRIDGE_BASE, S8_SSEG));
SpiCore spi(get_slot_addr(BRIDGE_BASE, S9_SPI));

GpiCore sw(get_slot_addr(BRIDGE_BASE, S3_SW));
Ps2Core ps2(get_slot_addr(BRIDGE_BASE, S11_PS2));



int domain(int x, int upper,int lower)
{

	if(x > upper)
		return upper;
	else if(x < lower)
		return lower;
	else
		return x;

}

void writeLine(OsdCore *osd, uint16_t xstart, uint16_t ystart, char ch[], int size ,int reverse = 0)
{
	for(int i = 0; i < size-1; i++)
	{
		osd->wr_char(xstart+i, ystart, ch[i], reverse);
	}
}


const int TEXTMIDDLEWIDTH = 40;

const int FIRSTLINE = 7;
const int SECONDLINE = 15;
const int THRIDLINE = 17;

void TitleScreenText()
{
	char pongTxt[5] = "Pong";
	char singlePlayerTxt[11] = "One-Player";
	char twoPlayerTxt[12] = "Two-Players";

	writeLine(&osd,TEXTMIDDLEWIDTH,FIRSTLINE,pongTxt,5);
	writeLine(&osd,TEXTMIDDLEWIDTH,SECONDLINE,singlePlayerTxt,11);
	writeLine(&osd,TEXTMIDDLEWIDTH,THRIDLINE,twoPlayerTxt,12);

	osd.bypass(0);

}

void mouseMove(SpriteCore *sprite,int &cx,int &cy, int x, int y,
		int XLeftBoundry, int XRightBoundry,
		int YUpBoundry,int YBottomBoundry)
{
	cx = domain(cx+x,XRightBoundry,XLeftBoundry);
	cy = domain(cy+y,YBottomBoundry,YUpBoundry);
	sprite->move_xy(cx, cy);

}

int mouseClickListening(SpriteCore *sprite,int cx,int cy, int leftClick)
{
	if(leftClick)
	{
		if(cx > TEXTMIDDLEWIDTH*8 && cx < TEXTMIDDLEWIDTH*8 + 11*8)
		{
			if(cy > SECONDLINE*16 - 8 && cy < SECONDLINE*16 + 8)
				return 1;
			else if(cy > THRIDLINE*16 - 8 && cy < THRIDLINE*16 + 8)
				return 2;
			else
				return 0;
		}
	}
	return 0;
}

const int16_t GameXBoundry = 640, GameYBoundry = 480;

const int16_t CanvasLength = 480, CanvasWidth = 240;
const int16_t CanvasLeft = 80, CanvasUp = 130;
const int16_t CanvasRight = CanvasLeft+CanvasLength, CanvasDown = CanvasUp+CanvasWidth;
const int16_t PaddleOffset = 20;
const int8_t Score1X = 10, ScoreY = 7, Score2X = 70;
const int8_t paddleWidth = 8, paddleHeight = 32;
const int8_t Ballsize = 4;
const int16_t CanvasMiddleX = CanvasLeft + CanvasLength/2;
const int16_t CanvasMiddleY = CanvasUp + CanvasWidth/2;

void playGroundObjectDisplayRefresh(ball *ball1, paddle *paddle1, paddle *paddle2)
{
	playGround.move_xy(playGround.PADDLE1, paddle1->x , paddle1->y);
	playGround.move_xy(playGround.PADDLE2, paddle2->x , paddle2->y);
	playGround.move_xy(playGround.BALL, ball1->x , ball1->y);
}

void gameInitialization(ball *ball1, paddle *paddle1, paddle *paddle2)
{
	osd.bypass(1);
	mouse.bypass(1);

	osd.clr_screen();
	osd.wr_char(Score1X, ScoreY, '0');
	osd.wr_char(Score2X, ScoreY, '0');

	for(int i = Score1X; i<= Score2X; i++)
	{
		osd.wr_char(i,CanvasUp/16,' ',1);
		osd.wr_char(i,CanvasDown/16,' ',1);
	}

	playGround.setLengthAndWidth(paddleHeight,paddleWidth);
	playGround.setBallsize(Ballsize);
	playGroundObjectDisplayRefresh(ball1, paddle1, paddle2);

	osd.bypass(0);
	background.bypass(0);
	playGround.bypass(0);

}

void scoreUpdate(int score1,int score2)
{
	osd.wr_char(Score1X, ScoreY, '0' + score1);
	osd.wr_char(Score2X, ScoreY, '0' + score2);
}

void gameFinishing()
{
	osd.bypass(1);

	osd.clr_screen();

	background.bypass(1);
	playGround.bypass(1);

}

void trackBall(ball *ball1, paddle *paddle2)
{
	if (abs((paddle2->y + paddleHeight/2) - (ball1->y)) > 4)
	{
		paddle2->dir_y = ((paddle2->y + paddleHeight/2) - (ball1->y) > 0) * -0.5 + ((paddle2->y + paddleHeight/2) - (ball1->y) < 0) * 0.5;
	}
	else
		paddle2->dir_y = 0;
}

float getPositionScaler(SpiCore *spi_p)
{
	 const uint8_t RD_CMD = 0x0b;
	 const uint8_t DATA_REG = 0x08;
	 const float raw_max = 127.0 / 2.0;  //128 max 8-bit reading for +/-2g

	 int8_t xraw, yraw, zraw;
	 float x;

	 spi_p->set_freq(400000);
	 spi_p->set_mode(0, 0);

	 spi_p->assert_ss(0);    // activate
	 spi_p->transfer(RD_CMD);  // for read operation
	 spi_p->transfer(DATA_REG);  //
	 xraw = spi_p->transfer(0x00);
	 yraw = spi_p->transfer(0x00);
	 zraw = spi_p->transfer(0x00);
	 spi_p->deassert_ss(0);
	 x = (float) xraw / raw_max;

	 float norm = (x+0.7) / 1.4;

	 if (norm < 0)
	 	 norm = 0;
	 else if (norm > 1)
	 	 norm = 1;

	 return norm ;

}

void SetColor(PlayGroundCore *playGround_p,GpiCore *sw_p)
{
	int16_t colorCode = sw_p->read();

	playGround_p->setColor(colorCode);
}

void singleplayer(int &cx,int &cy)
{

	ball ball1;
	paddle paddle1(0);
	paddle paddle2(1);

	gameInitialization(&ball1,&paddle1,&paddle2);

	int scoreP1 = 0,scoreP2 = 0;

	int isWin = 0;
	int hasWinner = 0;
	float timePass = 0;

	unsigned long last = now_ms();
	int lbtn=0,rbtn=0,xmov=0,ymov=0;

	bool startDic = true;

	while(hasWinner == 0)
	{

		startDic =!startDic;
		ball1.resetball(startDic);
		paddle1.reset(0);
		paddle2.reset(1);
		playGroundObjectDisplayRefresh(&ball1, &paddle1, &paddle2);

		isWin = 0;
		while(isWin == 0 )
		{

				last = now_ms();

				SetColor(&playGround, &sw);

				ball1.move(timePass);

				ball1.checkHitBottomWall(CanvasDown);
				if(ball1.checkHitLeftPaddle(paddle1.x, paddle1.y, paddleHeight, paddleWidth))
				{
					ball1.setSpeed(ball1.ball_speed * 1.05);
				}
				if(ball1.checkHitRightPaddle(paddle2.x, paddle2.y, paddleHeight, paddleWidth))
				{
					ball1.setSpeed(ball1.ball_speed * 1.05);
				}

				ball1.checkHitTopWall(CanvasUp);
				isWin = ball1.checkOutOfBound(CanvasLeft, CanvasRight);

				trackBall(&ball1,&paddle2);
				paddle2.move(timePass);

				xmov=0;ymov=0;
				ps2.get_mouse_activity(&lbtn, &rbtn, &xmov, &ymov);
				ymov = -1 * ymov;
				mouseMove(&mouse,cx,cy,xmov,ymov,CanvasLeft,CanvasRight,CanvasUp,CanvasDown);
				paddle1.y = cy;

				playGroundObjectDisplayRefresh(&ball1, &paddle1, &paddle2);

				timePass = now_ms() - last;

		}

		if(isWin == -1)
			scoreP2++;
		else
			scoreP1++;

		scoreUpdate(scoreP1,scoreP2);

		if(scoreP2 == 5)
			hasWinner = 1;
		else if(scoreP1 == 5)
			hasWinner = -1;

	}

	char win1Text[14] = "Player1 wins!";
	char win2Text[14] = "Player2 wins!";
	if(hasWinner == -1)
	{
		writeLine(&osd,TEXTMIDDLEWIDTH,THRIDLINE,win1Text,14);
	}
	else
	{
		writeLine(&osd,TEXTMIDDLEWIDTH,THRIDLINE,win2Text,14);
	}
	osd.bypass(0);

	sleep_ms(5000);

	gameFinishing();

}

void twoplayer(int &cx,int &cy)
{

	ball ball1;
	paddle paddle1(0);
	paddle paddle2(1);

	gameInitialization(&ball1,&paddle1,&paddle2);

	int scoreP1 = 0,scoreP2 = 0;

	int isWin = 0;
	int hasWinner = 0;
	float timePass = 0;

	unsigned long last = now_ms();
	int lbtn=0,rbtn=0,xmov=0,ymov=0;

	float paddle2Controller;
	int paddle2Y;
	int resolutionCounter = 0;
	const int RESOLUTION = 5;
	bool startDic = 1;


	while(hasWinner == 0)
	{
		startDic =!startDic;
		ball1.resetball(startDic);
		paddle1.reset(0);
		paddle2.reset(1);
		playGroundObjectDisplayRefresh(&ball1, &paddle1, &paddle2);

		isWin = 0;

		while(isWin == 0)
		{
				last = now_ms();
				SetColor(&playGround, &sw);
				ball1.move(timePass);

				ball1.checkHitBottomWall(CanvasDown);
				ball1.checkHitTopWall(CanvasUp);
				if(ball1.checkHitLeftPaddle(paddle1.x, paddle1.y, paddleHeight, paddleWidth))
				{
					ball1.setSpeed(ball1.ball_speed * 1.05);
				}
				if(ball1.checkHitRightPaddle(paddle2.x, paddle2.y, paddleHeight, paddleWidth))
				{
					ball1.setSpeed(ball1.ball_speed * 1.05);
				}

				isWin = ball1.checkOutOfBound(CanvasLeft, CanvasRight);

				/*
				 * paddle2 controller
				 * get a value from 0 to 1*/


				if(resolutionCounter == RESOLUTION)
				{
					paddle2Controller = getPositionScaler(&spi);
					paddle2Y = CanvasUp + CanvasWidth * paddle2Controller;
					paddle2.y = paddle2Y;
					resolutionCounter=0;
				}
				else
					resolutionCounter++;

				xmov=0;ymov=0;
				ps2.get_mouse_activity(&lbtn, &rbtn, &xmov, &ymov);
				ymov = -1 * ymov;
				mouseMove(&mouse,cx,cy,xmov,ymov,CanvasLeft,CanvasRight,CanvasUp,CanvasDown);
				paddle1.y = cy;

				playGroundObjectDisplayRefresh(&ball1, &paddle1, &paddle2);

				timePass = now_ms() - last;

		}

		if(isWin == -1)
			scoreP2++;
		else
			scoreP1++;

		scoreUpdate(scoreP1,scoreP2);

		if(scoreP2 == 5)
			hasWinner = 1;
		else if(scoreP1 == 5)
			hasWinner = -1;

	}

	char win1Text[14] = "Player1 wins!";
	char win2Text[14] = "Player2 wins!";
	if(hasWinner == -1)
	{
		writeLine(&osd,TEXTMIDDLEWIDTH,THRIDLINE,win1Text,14);
	}
	else
	{
		writeLine(&osd,TEXTMIDDLEWIDTH,THRIDLINE,win2Text,14);
	}
	osd.bypass(0);

	sleep_ms(5000);

	gameFinishing();

}

int TitleScreen()
{
	static int cx = 0, cy = 0;
	int lbtn = 0, rbtn = 0, xmov = 0, ymov = 0;
	int gameMode = 0;
	int id = -1;
	TitleScreenText(); // display title screen

	while(id!=2)
	{
		id = ps2.init();
		//initialize mouse
	}

	mouse.bypass(0);

	while(gameMode == 0)//wait for the game mode being selected
	{
		lbtn = 0;
		xmov = 0;
		ymov = 0;
		ps2.get_mouse_activity(&lbtn, &rbtn, &xmov, &ymov);
		ymov = -1 * ymov;
		mouseMove(&mouse,cx,cy,xmov,ymov,0,GameXBoundry,0, GameYBoundry);
		gameMode = mouseClickListening(&mouse,cx,cy,lbtn);
	}



	if(gameMode == 1)
		singleplayer(cx,cy);
	else
		twoplayer(cx,cy);
}

void init()
{
	frame.bypass(1);
	bar.bypass(1);
	gray.bypass(1);
	background.bypass(1);
	ghost.bypass(1);
	mouse.bypass(1);
	osd.bypass(1);
	playGround.bypass(1);
	osd.clr_screen();

	osd.set_color(0xfff, 0x000);

}

void gsensor_check(SpiCore *spi_p, GpoCore *led_p) {
   const uint8_t RD_CMD = 0x0b;
   const uint8_t PART_ID_REG = 0x02;
   const uint8_t DATA_REG = 0x08;
   const float raw_max = 127.0 / 2.0;  //128 max 8-bit reading for +/-2g

   int8_t xraw, yraw, zraw;
   float x, y, z;
   int id;

   spi_p->set_freq(400000);
   spi_p->set_mode(0, 0);
   // check part id
   spi_p->assert_ss(0);    // activate
   spi_p->transfer(RD_CMD);  // for read operation
   spi_p->transfer(PART_ID_REG);  // part id address
   id = (int) spi_p->transfer(0x00);
   spi_p->deassert_ss(0);
   uart.disp("read ADXL362 id (should be 0xf2): ");
   uart.disp(id, 16);
   uart.disp("\n\r");
   // read 8-bit x/y/z g values once
   spi_p->assert_ss(0);    // activate
   spi_p->transfer(RD_CMD);  // for read operation
   spi_p->transfer(DATA_REG);  //
   xraw = spi_p->transfer(0x00);
   yraw = spi_p->transfer(0x00);
   zraw = spi_p->transfer(0x00);
   spi_p->deassert_ss(0);
   x = (float) xraw / raw_max;
   y = (float) yraw / raw_max;
   z = (float) zraw / raw_max;
   uart.disp("x/y/z axis g values: ");
   uart.disp(x, 3);
   uart.disp(" / ");
   uart.disp(y, 3);
   uart.disp(" / ");
   uart.disp(z, 3);
   uart.disp("\n\r");
}

int main() {



	init();

	while (1) {

		TitleScreen();

   } // while



} //main
