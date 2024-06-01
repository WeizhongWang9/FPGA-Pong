
#include "playground.h"

/**********************************************************************
 * Sprite core methods
 *********************************************************************/
PlayGroundCore::PlayGroundCore(uint32_t core_base_addr, int sprite_size) {
   base_addr = core_base_addr;
   size = sprite_size;
}
PlayGroundCore::~PlayGroundCore() {
}

void PlayGroundCore::wr_mem(int addr, uint32_t color) {
   io_write(base_addr, addr, color);
}

void PlayGroundCore::bypass(int by) {
   io_write(base_addr, BYPASS_REG, (uint32_t ) by);
}

void PlayGroundCore::move_xy(int obj,int x, int y) {
   io_write(base_addr, X0_REG+obj, x);
   io_write(base_addr, Y0_REG+obj, y);
   return;
}

void PlayGroundCore::setColor(int16_t color) {
   io_write(base_addr, ITEM_COLOR_REG, color);
   return;
}

void PlayGroundCore::setLengthAndWidth(int8_t length,int8_t width) {
   io_write(base_addr, LENGTH_REG, length);
   io_write(base_addr, WIDTH_REG, width);
   return;
}


void PlayGroundCore::setBallsize(int8_t size)
{
	io_write(base_addr, BALLSIZE_REG, size);
}
