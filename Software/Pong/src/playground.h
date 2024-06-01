//#ifndef _PLAYGROUND_H_INCLUDED
//#define _PLAYGROUND_H_INCLUDED

#include "chu_init.h"
#include <stdlib.h>

/**********************************************************************
 * Sprite Core
 *********************************************************************/
/**
 * sprite video core driver
 *
 * video subsystem HDL parameter:
 *  - CHROMA_KEY (KEY_COLOR) = 0
 *
 */
class PlayGroundCore {
public:
   /**
    * register map
    *
    */
   enum {
      BYPASS_REG = 0x2000,     /**< bypass control register */
      X0_REG = 0x2001,          /**< x-axis of sprite origin */
      Y0_REG = 0x2002,          /**< y-axis of sprite origin */
      X1_REG = 0x2003,          /**< x-axis of sprite origin */
      Y1_REG = 0x2004,          /**< y-axis of sprite origin */
      X2_REG = 0x2005,          /**< x-axis of sprite origin */
      Y2_REG = 0x2006,          /**< y-axis of sprite origin */
      ITEM_COLOR_REG = 0x2007, /**< sprite control register */
	  LENGTH_REG = 0x2008,
	  WIDTH_REG = 0x2009,
	  BALLSIZE_REG = 0x200a
   };
   /**
    * symbolic constants
    *
    */
   enum {
      KEY_COLOR = 0,  /**< chroma-key color */
   };

   enum {
	  PADDLE1 = 0,
	  PADDLE2 = 2,
	  BALL = 4
   };

   /* methods */
   PlayGroundCore(uint32_t core_base_addr, int size);
   ~PlayGroundCore();                  // not used

   /**
    * write a 32-bit word to memory module/registers of a video core
    * @param addr offset address within core
    * @param color data to be written
    *
    */
   void wr_mem(int addr, uint32_t color);

   /**
    * move a sprite to a location
    * @param x x-coordinate of sprite origin
    * @param y y-coordinate of sprite origin
    *
    * @note origin is the top-left corner of sprite
    */
   void move_xy(int obj ,int x, int y);

   /**
    * enable/disable core bypass
    * @param by 1: bypass current core; 0: not bypass
    * @note type of command depends on each individual sprite core
    */
   void bypass(int by);


   /*
    * set color of 3 sprites, first 3 digits are paddle1, then paddle2, then ball.
    * */

   void setColor(int16_t color);

   void setLengthAndWidth(int8_t length,int8_t width);

   void setBallsize(int8_t size);


private:
   uint32_t base_addr;
   int size;   // sprite memory size






};
