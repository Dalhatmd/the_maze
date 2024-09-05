#include "ray.h"

void textures_init(RaycasterState *rcState)
{
for (int i = 0; i < 9; i++)
{
   for (int x = 0; x < TEXWIDTH; x++)
   {
	   for (int y = 0; y < TEXHEIGHT; y++)
	   {
		   int xorcolor = (x * 256 / TEXWIDTH) ^ (y * 256 / TEXHEIGHT);
		   //int xcolor = x * 256 / TEXWIDTH;
		   int ycolor = y * 256 / TEXHEIGHT;
		   int xycolor = y * 128 / TEXHEIGHT + x * 128 / TEXWIDTH;
		   rcState->textures[0][TEXWIDTH * y + x] = 65536 * 254 * (x != y && x != TEXWIDTH - y); //flat red texture with black cross
											rcState->textures[1][TEXWIDTH * y + x] = xycolor + 256 * xycolor + 65536 * xycolor;
											rcState->textures[2][TEXWIDTH * y + x] = 256 * xycolor + 65536 * xycolor;
											rcState->textures[3][TEXWIDTH * y + x] = xorcolor + 256 * xorcolor + 65536 * xorcolor;
											rcState->textures[4][TEXWIDTH * y + x] = 256 * xorcolor;
											rcState->textures[5][TEXWIDTH * y + x] = xorcolor + 65536 * xorcolor;
											rcState->textures[6][TEXWIDTH * y + x] = 256 * xorcolor + 65536 * 256 * xorcolor;
											rcState->textures[7][TEXWIDTH * y + x] = xycolor + 65536 * 256 * xycolor;
											rcState->textures[8][TEXWIDTH * y + x] = xycolor + 65536 * 256 * xorcolor;
	   }
   }
}
}
