#include "nios_draw.h"

int fillIconBmp[729] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,
							0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,
							0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,
							0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,
							0,0,0,0,1,0,0,0,1,1,1,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,
							0,0,0,0,1,0,1,1,0,0,1,1,1,1,0,0,0,1,0,0,0,0,0,0,0,0,0,
							0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,
							0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,
							0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,
							0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,
							0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,
							0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,
							0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,
							0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,
							0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,
							0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,
							0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

int colorSampleIConBmp[729] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
									0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
									0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
									0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
									0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
									0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
									0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
									0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
									0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
									0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
									0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
									0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
									0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
									0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,
									0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,
									0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,
									0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,
									0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,
									0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,
									0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,
									0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,
									0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,
									0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,
									0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,
									0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,
									0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
									0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

int cpyIconBmp[729] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,1,1,1,1,1,0,0,1,1,1,1,1,1,1,0,0,1,0,0,0,0,0,0,1,
							0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,1,0,
							0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,
							0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0,0,1,0,0,
							0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0,1,0,0,0,
							0,0,0,1,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,1,0,0,0,0,
							0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,
							0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,
							0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,
							0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,
							0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,
							0,0,0,1,1,1,1,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

int cutIconBmp[729] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,1,1,1,1,1,1,0,0,1,0,0,0,0,0,0,1,0,1,1,1,1,1,1,1,0,
							0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,
							0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,
							0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,
							0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,
							0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,
							0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,
							0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,
							0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,
							0,0,1,0,0,0,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,1,0,0,0,0,0,
							0,0,1,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,1,0,0,0,0,0,
							0,0,1,1,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,1,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

int pencilIconBmp[729] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
								0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
								0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
								0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
								0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
								0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
								0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
								0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
								0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
								0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
								0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
								0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,
								0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,
								0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,
								0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,
								0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,
								0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,
								0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,
								0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,
								0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,
								0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,
								0,0,0,0,1,1,1,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,
								0,0,0,1,0,0,0,0,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,
								0,0,1,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,
								0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
								0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
								0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

int pongIconBmp[729] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,1,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,1,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,1,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,1,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,1,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,1,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,
							0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,
							0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,
							0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,
							0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

int snakeIconBmp[729] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,
							0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,1,1,0,0,0,0,0,0,
							0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,1,1,0,0,0,0,0,
							0,0,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,0,0,0,0,1,1,0,0,0,0,
							0,0,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,0,0,0,0,1,1,1,0,0,0,
							0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,
							0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,
							0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,
							0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,
							0,0,0,1,1,1,1,1,1,1,1,1,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,
							0,0,0,1,1,1,1,1,1,1,1,1,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

int clearIconBmp[729] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,1,1,1,1,1,1,0,0,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,
							0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,
							0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,
							0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,
							0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,
							0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,
							0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,
							0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,1,1,0,0,0,0,
							0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,1,1,0,0,0,
							0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,1,1,0,0,
							0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,1,1,0,
							0,0,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,0,1,0,0,0,0,0,1,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

int startIconBmp[1458] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,
							0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,
							0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,
							0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,
							0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,
							0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,
							0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,
							0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,1,0,0,0,0,
							0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,
							0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,
							0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,
							0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,
							0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,
							0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,
							0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,
							0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,
							0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,
							0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,
							0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,
							0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

int cursorSprite[25] = { 1,1,1,1,1,
							1,1,1,0,0,
							1,0,1,1,0,
							1,0,0,1,1,
							0,0,0,0,1 };


void draw_icon(tool icon, char selected,
	lastDrawingVar* lastDrawingData, alt_up_pixel_buffer_dma_dev* pixel_buffer) {
	/**************************************************************************
	 * draw_icon
	 **************************************************************************
	 * Parameters
	 * icon				: name of the icon to draw
	 * selected			: 1 = selected (draw a red frame around the icon if 0 the frame is black
	 * lastDrawingData	: Structure that save multiple variable used to keep track of previous
	 *					  shape drawn during the interactive draw
	 * pixel_buffer		: is the pointer used to write in the pixel_buffer of the video pipeline
	 *
	 * Return value
	 * none
	 *
	 * Side effects
	 * Draw the chosen icon then draw a the frame around. The frame color depend of the
	 * selected parameter
	 * If icon argument is invalid, the function draw nothing
	 *
	 *************************************************************************/
	 //Switch case to chose the icon to draw
	switch (icon) {
	case EMPTY_RECTANGLE:
		draw_selection_Frame(2, 2, 29, 29, selected, lastDrawingData, pixel_buffer);
		//draw icon
		soft_empty_rectangle_draw(6, 11, 25, 20, BLACK, NOT_ERASE_PREVIOUS_WORK, lastDrawingData, pixel_buffer);
		break;
	case FILLED_RECTANGLE:
		draw_selection_Frame(31, 2, 58, 29, selected, lastDrawingData, pixel_buffer);
		//draw icon
		alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 35, 11, 55, 20, BLACK, 0);
		break;
	case EMPTY_ELLIPSE:
		draw_selection_Frame(2, 31, 29, 58, selected, lastDrawingData, pixel_buffer);
		//draw icon
		draw_empty_ellipse(16, 45, 8, 5, BLACK, pixel_buffer, NOT_ERASE_PREVIOUS_WORK, lastDrawingData);
		break;
	case FILLED_ELLIPSE:
		draw_selection_Frame(31, 31, 58, 58, selected, lastDrawingData, pixel_buffer);
		//draw icon
		draw_empty_ellipse(45, 45, 8, 5, BLACK, pixel_buffer, NOT_ERASE_PREVIOUS_WORK, lastDrawingData);
		fill_to_edge_zone(45, 45, BLACK, pixel_buffer);
		break;
	case LINE:
		draw_selection_Frame(2, 60, 29, 87, selected, lastDrawingData, pixel_buffer);
		//draw icon
		soft_draw_line(8, 70, 20, 78, BLACK, NOT_ERASE_PREVIOUS_WORK, lastDrawingData, pixel_buffer);
		break;
	case FILL:
		draw_selection_Frame(31, 60, 58, 87, selected, lastDrawingData, pixel_buffer);
		//draw icon		
		draw_icon_array(31, 60, 27, 27, fillIconBmp, pixel_buffer);
		break;
	case COLOR_SAMPLE:
		draw_selection_Frame(2, 89, 29, 116, selected, lastDrawingData, pixel_buffer);
		//draw icon
		draw_icon_array(2, 89, 27, 27, colorSampleIConBmp, pixel_buffer);
		break;
	case CPY_PASTE:
		draw_selection_Frame(31, 89, 58, 116, selected, lastDrawingData, pixel_buffer);
		//draw icon		
		draw_icon_array(31, 89, 27, 27, cpyIconBmp, pixel_buffer);
		break;
	case CUT_PASTE:
		draw_selection_Frame(2, 118, 29, 145, selected, lastDrawingData, pixel_buffer);
		//draw icon		
		draw_icon_array(2, 118, 27, 27, cutIconBmp, pixel_buffer);
		break;
	case PENCIL:
		//draw selection perimiter
		draw_selection_Frame(31, 118, 58, 145, selected, lastDrawingData, pixel_buffer);
		//draw icon		
		draw_icon_array(31, 118, 27, 27, pencilIconBmp, pixel_buffer);
		break;
	case PONG:
		//draw selection perimiter
		draw_selection_Frame(2, 408, 29, 435, selected, lastDrawingData, pixel_buffer);
		//draw icon		
		draw_icon_array(3, 408, 27, 27, pongIconBmp, pixel_buffer);
		break;
	case SNAKE:
		//draw selection perimiter
		draw_selection_Frame(31, 408, 58, 435, selected, lastDrawingData, pixel_buffer);
		//draw icon
		draw_icon_array(32, 408, 27, 27, snakeIconBmp, pixel_buffer);
		break;
	case CLEAR:
		//draw selection perimiter
		draw_selection_Frame(2, 437, 29, 464, selected, lastDrawingData, pixel_buffer);
		//draw icon		
		draw_icon_array(2, 437, 27, 27, clearIconBmp, pixel_buffer);
		break;
	default:
		break;
	}
}

unsigned char get_pixel_color(int x, int y) {
	/**************************************************************************
	 * Get the color value of a specific pixel
	 **************************************************************************
	 * Parameters
	 * x : x coordinate of the pixel
	 * y : y coordinate of the pixel
	 *
	 * Valeur de retour
	 * specified pixel color
	 *
	 * Side effects
	 * none
	 *************************************************************************/
	unsigned char* pixel;
	unsigned char color;
	//pixel = (unsigned char *)ONCHIP_MEM_BASE+(y<<9)+x;
	pixel = (unsigned char*)ONCHIP_MEM_BASE + (y * 640) + x;
	color = *pixel;
	return color;
}

void cursor_save(Cursor* coordinate, alt_u8* cursorMem) {
	/**************************************************************************
	 * cursor_save
	 **************************************************************************
	 * Parametres
	 * coordinate	: structure that contain the x and y coordinate of the point of the cursor
	 * cursorMem	: Array where to save the pixel value
	 *
	 * Return
	 * None
	 *
	 * Side effects
	 * save pixel value in a 5x5 square starting at the x and y coordiante from the
	 * parameter coordinate
	 *
	 *************************************************************************/
	int x = coordinate->x;
	int y = coordinate->y;
	int iter = 0;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			cursorMem[iter] = get_pixel_color(x + j, y + i);
			iter++;
		}
	}
}

void cursor_erase(Cursor* coordinate, alt_u8* cursorMem, alt_up_pixel_buffer_dma_dev* pixel_buffer) {
	/**************************************************************************
	 * cursor_erase
	 **************************************************************************
	 * Parametres
	 * coordinate	: structure that contain the x and y coordinate of the point of the cursor
	 * cursorMem	: Array where to save the pixel value
	 * pixel_buffer : is the pointer used to write in the pixel_buffer of the video pipeline
	 *
	 * Return
	 * None
	 *
	 * Side effects
	 * Draw the content of the cursorMem starting at the x and y coordiante from the
	 * parameter coordinate
	 *
	 *************************************************************************/
	int x = coordinate->x;
	int y = coordinate->y;
	int iter = 0;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			alt_up_pixel_buffer_dma_draw(pixel_buffer, cursorMem[iter], x + j, y + i);
			iter++;
		}
	}
}

void cursor_draw_sprite(Cursor* coordinate, alt_up_pixel_buffer_dma_dev* pixel_buffer) {
	/**************************************************************************
	 * cursor_draw_sprite
	 **************************************************************************
	 * Parametres
	 * coordinate	: structure that contain the x and y coordinate of the point of the cursor
	 * pixel_buffer : is the pointer used to write in the pixel_buffer of the video pipeline
	 *
	 * Return
	 * None
	 *
	 * Side effects
	 * Draw the cursor 5x5 cursor using the globalVariable cursorSprite starting at
	 * the x and y coordiante from the parameter coordinate
	 *
	 * Each pixel of the cursor is black or white depending on the background color
	 * to enhence the visibility
	 *
	 *************************************************************************/
	int x = coordinate->x;
	int y = coordinate->y;
	int iter = 0;
	int color;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			//test the sprite mask
			if (cursorSprite[iter] == 1) {
				//check back ground color
				color = get_pixel_color(x + j, y + i);
				if (color > 128)color = 0;
				else color = 255;
				//draw the pixel
				alt_up_pixel_buffer_dma_draw(pixel_buffer, color, x + j, y + i);
				iter++;
			}
			else iter++;
		}
	}
}

void draw_color_palette(int selectedColor, lastDrawingVar* lastDrawingData, alt_up_pixel_buffer_dma_dev* pixel_buffer) {
	/**************************************************************************
	 * draw_color_palette
	 **************************************************************************
	 * Parameters
	 * selectedColor	: 8 bit RGB color value to draw in the selected color Box in the tool bar
	 * lastDrawingData	: Structure that save multiple variable used to keep track of previous
	 *					  shape drawn during the interactive draw
	 * pixel_buffer		: is the pointer used to write in the pixel_buffer of the video pipeline
	 *
	 * Return value
	 * none
	 *
	 * Side effects
	 * Draw the color palette in the tool bar
	 *************************************************************************/
	 //draw the selected color
	alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 3, 147, SECOND_COLUMN_X_END, 174, selectedColor, 0);
	//draw the color palette
	alt_up_pixel_buffer_dma_draw_box(pixel_buffer, FIRST_COLUMN_X_START, 176, FIRST_COLUMN_X_END, 203, BLACK, 0);
	alt_up_pixel_buffer_dma_draw_box(pixel_buffer, SECOND_COLUMN_X_START, 176, SECOND_COLUMN_X_END, 203, WHITE, 0);

	alt_up_pixel_buffer_dma_draw_box(pixel_buffer, FIRST_COLUMN_X_START, 205, FIRST_COLUMN_X_END, 232, LILLA, 0);
	alt_up_pixel_buffer_dma_draw_box(pixel_buffer, SECOND_COLUMN_X_START, 205, SECOND_COLUMN_X_END, 232, PURPLE, 0);

	alt_up_pixel_buffer_dma_draw_box(pixel_buffer, FIRST_COLUMN_X_START, 234, FIRST_COLUMN_X_END, 261, RED, 0);
	alt_up_pixel_buffer_dma_draw_box(pixel_buffer, SECOND_COLUMN_X_START, 234, SECOND_COLUMN_X_END, 261, BROWN, 0);

	alt_up_pixel_buffer_dma_draw_box(pixel_buffer, FIRST_COLUMN_X_START, 263, FIRST_COLUMN_X_END, 290, SKY_BLUE, 0);
	alt_up_pixel_buffer_dma_draw_box(pixel_buffer, SECOND_COLUMN_X_START, 263, SECOND_COLUMN_X_END, 290, BLUE, 0);

	alt_up_pixel_buffer_dma_draw_box(pixel_buffer, FIRST_COLUMN_X_START, 292, FIRST_COLUMN_X_END, 319, LIGHT_PINK, 0);
	alt_up_pixel_buffer_dma_draw_box(pixel_buffer, SECOND_COLUMN_X_START, 292, SECOND_COLUMN_X_END, 319, PINK, 0);

	alt_up_pixel_buffer_dma_draw_box(pixel_buffer, FIRST_COLUMN_X_START, 321, FIRST_COLUMN_X_END, 348, YELLOW, 0);
	alt_up_pixel_buffer_dma_draw_box(pixel_buffer, SECOND_COLUMN_X_START, 321, SECOND_COLUMN_X_END, 348, ORANGE, 0);

	alt_up_pixel_buffer_dma_draw_box(pixel_buffer, FIRST_COLUMN_X_START, 350, FIRST_COLUMN_X_END, 377, DARK_GREEN, 0);
	alt_up_pixel_buffer_dma_draw_box(pixel_buffer, SECOND_COLUMN_X_START, 350, SECOND_COLUMN_X_END, 377, NEON_GREEN, 0);

	alt_up_pixel_buffer_dma_draw_box(pixel_buffer, FIRST_COLUMN_X_START, 379, FIRST_COLUMN_X_END, 406, FLUO_GREEN, 0);
	alt_up_pixel_buffer_dma_draw_box(pixel_buffer, SECOND_COLUMN_X_START, 379, SECOND_COLUMN_X_END, 406, DARK_BLUE, 0);

	//draw frame around the current color
	soft_empty_rectangle_draw(3, 147, SECOND_COLUMN_X_END, 174, NOT_ERASE_PREVIOUS_WORK, BLACK, lastDrawingData, pixel_buffer);
}

void draw_selection_Frame(int x1, int y1, int x2, int y2, char selected,
	lastDrawingVar* lastDrawingData, alt_up_pixel_buffer_dma_dev* pixel_buffer) {
	/**************************************************************************
	 * draw_selection_Frame
	 **************************************************************************
	 * Parameters
	 * x1				: x coordinate of top left corner of the selection frame
	 * y1				: y coordinate of the top left corner of the selection frame
	 * x2				: x coordinate of the bottom right corner of the selection frame
	 * y2				: y coordinate of the botom right corner of the selection frame
	 * selected			: if = 1 the frame is red if = 0 the frame is black
	 * lastDrawingData	: Structure that save multiple variable used to keep track of previous
	 *						  shape drawn during the interactive draw
	 * pixel_buffer		: is the pointer used to write in the pixel_buffer of the video pipeline
	 *
	 * Return value
	 * none
	 *
	 * Side effects
	 * Draw a selection frame with the two pointe x1,y1 and x2,y2. if selected ==1
	 * the frame is Red if selected ==0 the frame is black
	 *
	 *************************************************************************/
	 //draw selection perimiter 2 pixel wide
	if (selected) {
		soft_empty_rectangle_draw(x1, y1, x2, y2, SELECTION_COLOR, NOT_ERASE_PREVIOUS_WORK, lastDrawingData, pixel_buffer);
		soft_empty_rectangle_draw(x1 + 1, y1 + 1, x2 - 1, y2 - 1, SELECTION_COLOR, NOT_ERASE_PREVIOUS_WORK, lastDrawingData, pixel_buffer);
	}
	else {
		soft_empty_rectangle_draw(x1, y1, x2, y2, TOOL_BOX_BACKGROUND_COLOR, NOT_ERASE_PREVIOUS_WORK, lastDrawingData, pixel_buffer);
		soft_empty_rectangle_draw(x1 + 1, y1 + 1, x2 - 1, y2 - 1, BLACK, NOT_ERASE_PREVIOUS_WORK, lastDrawingData, pixel_buffer);
	}
}

void draw_icon_array(int start_x, int start_y, int row, int col, int* image, alt_up_pixel_buffer_dma_dev* pixel_buffer) {
	/**************************************************************************
	 * draw_icon_array
	 **************************************************************************
	 * Parametres
	 * start_x	: x coordinate of top left corner of the icon
	 * start_y	: y coordinate of the top left corner of the icon
	 * row		: number of pixel in the vertical orientation
	 * col		: number of pixel in the horizontal orientation
	 * image	: array containting the icon PixelMask to draw the image
	 * pixel_buffer : is the pointer used to write in the pixel_buffer of the video pipeline
	 *
	 *
	 * Return
	 * None
	 *
	 * Side effects
	 * Draw the content of an array of a specified size starting at a specified x and y
	 * coordinate
	 *
	 * Draw in BLACK
	 *
	 *************************************************************************/
	int iter = 0;
	for (unsigned char j = 0; j < col; j++) {
		for (unsigned char i = 0; i < row; i++) {
			//test the icon mask
			if (image[iter] == 1) {
				alt_up_pixel_buffer_dma_draw(pixel_buffer, BLACK, start_x + i, start_y + j);
				iter++;
			}
			else {
				iter++;
			}
		}
	}
}

void draw_tool_bar(tool currentTool, lastDrawingVar* lastDrawingData, alt_up_pixel_buffer_dma_dev* pixel_buffer) {
	/**************************************************************************
	 * draw_tool_bar
	 **************************************************************************
	 * Parameters
	 * currentTool		: This parameter allow to know wich tool need a red selection frame
	 * lastDrawingData	: Structure that save multiple variable used to keep track of previous
	 *					  shape drawn during the interactive draw
	 * pixel_buffer		: is the pointer used to write in the pixel_buffer of the video pipeline
	 *
	 * Return value
	 * none
	 *
	 * Side effects
	 * Draw all the icon in the tool bar. Then draw a red selection frame around the
	 * currentTool parameter.
	 *
	 *************************************************************************/
	 //draw tool bar background
	alt_up_pixel_buffer_dma_draw_box(pixel_buffer, LEFT_LIMIT, TOP_LIMIT, DRAWING_ZONE_LEFT_LIMIT, BOTTOM_LIMIT, TOOL_BOX_BACKGROUND_COLOR, 0);
	//Draw all icon of the tool bar
	soft_empty_rectangle_draw(LEFT_LIMIT, TOP_LIMIT, DRAWING_ZONE_LEFT_LIMIT, BOTTOM_LIMIT, NOT_ERASE_PREVIOUS_WORK, BLACK, lastDrawingData, pixel_buffer);
	soft_empty_rectangle_draw(LEFT_LIMIT + 1, TOP_LIMIT + 1, DRAWING_ZONE_LEFT_LIMIT - 1, BOTTOM_LIMIT - 1, NOT_ERASE_PREVIOUS_WORK, BLACK, lastDrawingData, pixel_buffer);
	draw_icon(EMPTY_RECTANGLE, 0, lastDrawingData, pixel_buffer);
	draw_icon(FILLED_RECTANGLE, 0, lastDrawingData, pixel_buffer);
	draw_icon(EMPTY_ELLIPSE, 0, lastDrawingData, pixel_buffer);
	draw_icon(FILLED_ELLIPSE, 0, lastDrawingData, pixel_buffer);
	draw_icon(LINE, 0, lastDrawingData, pixel_buffer);
	draw_icon(FILL, 0, lastDrawingData, pixel_buffer);
	draw_icon(COLOR_SAMPLE, 0, lastDrawingData, pixel_buffer);
	draw_icon(CPY_PASTE, 0, lastDrawingData, pixel_buffer);
	draw_icon(CUT_PASTE, 0, lastDrawingData, pixel_buffer);
	draw_icon(PENCIL, 0, lastDrawingData, pixel_buffer);
	draw_icon(PONG, 0, lastDrawingData, pixel_buffer);
	draw_icon(SNAKE, 0, lastDrawingData, pixel_buffer);
	draw_icon(CLEAR, 0, lastDrawingData, pixel_buffer);
	//Draw the current selected tool
	draw_icon(currentTool, 1, lastDrawingData, pixel_buffer);
}

void tool_selection(Cursor* currentCursor, tool* currentTool, int startUsingTool,
	int* selectedColor, char* left_btn, lastDrawingVar* lastDrawingData,
	alt_up_pixel_buffer_dma_dev* pixel_buffer) {
	/**************************************************************************
	 * tool_selection
	 **************************************************************************
	 * Parameters
	 * currentCursor	: containt the actual cursor coordinate
	 * currentTool		: current tool selected
	 * startUsingTool	: varaiable that indicate a tool is in use to avoid switching tool
	 *					  while the user is using one
	 * selectedColor	: current selected color for the drawing
	 * left_btn			: variable that indicate a left button press on the mouse
	 * lastDrawingData  : Structure that save multiple variable used to keep track of previous
	 *					  shape drawn during the interactive draw
	 * pixel_buffer		: is the pointer used to write in the pixel_buffer of the video pipeline
	 *
	 * Return value
	 * none
	 *
	 * Side effects
	 * use the cursor coordinate and the left button of the ouse to detect the selection
	 * of different tools and colors
	 *
	 *************************************************************************/
	static int lastColor = BLACK;
	static tool lastTool = NO_TOOL;
	//inside the tool bar zone and no tool currently in use
	if (startUsingTool == 0 && currentCursor->x < DRAWING_ZONE_LEFT_LIMIT && *left_btn) {
		//in first or second column of tool
		if (currentCursor->x <= 29) {
			if (currentCursor->y < 29) *currentTool = EMPTY_RECTANGLE;
			else if (currentCursor->y < 58) *currentTool = EMPTY_ELLIPSE;
			else if (currentCursor->y < 87) *currentTool = LINE;
			else if (currentCursor->y < 116) *currentTool = COLOR_SAMPLE;
			else if (currentCursor->y < 145) *currentTool = CUT_PASTE;
			//color selection
			else if (currentCursor->y < 174 && currentCursor->y > 145) {//selected color dead zone
			}
			else if (currentCursor->y < 203 && currentCursor->y > 176) *selectedColor = BLACK;
			else if (currentCursor->y < 232) *selectedColor = LILLA;
			else if (currentCursor->y < 261) *selectedColor = RED;
			else if (currentCursor->y < 290) *selectedColor = SKY_BLUE;
			else if (currentCursor->y < 319) *selectedColor = LIGHT_PINK;
			else if (currentCursor->y < 348) *selectedColor = YELLOW;
			else if (currentCursor->y < 377) *selectedColor = DARK_GREEN;
			else if (currentCursor->y < 406) *selectedColor = FLUO_GREEN;
			//game and clear selection
			else if (currentCursor->y < 435 && currentCursor->y > 408) *currentTool = PONG;
			else if (currentCursor->y < 464 && currentCursor->y > 437) *currentTool = CLEAR;
		}
		//second column
		else if (currentCursor->x >= 32) {
			if (currentCursor->y < 29) *currentTool = FILLED_RECTANGLE;
			else if (currentCursor->y < 58) *currentTool = FILLED_ELLIPSE;
			else if (currentCursor->y < 87) *currentTool = FILL;
			else if (currentCursor->y < 116) *currentTool = CPY_PASTE;
			else if (currentCursor->y < 145) *currentTool = PENCIL;
			//color selection
			else if (currentCursor->y < 174 && currentCursor->y > 145) {//selected color dead zone
			}
			else if (currentCursor->y < 203 && currentCursor->y > 176) *selectedColor = WHITE;
			else if (currentCursor->y < 232) *selectedColor = PURPLE;
			else if (currentCursor->y < 261) *selectedColor = BROWN;
			else if (currentCursor->y < 290) *selectedColor = BLUE;
			else if (currentCursor->y < 319) *selectedColor = PINK;
			else if (currentCursor->y < 348) *selectedColor = ORANGE;
			else if (currentCursor->y < 377) *selectedColor = NEON_GREEN;
			else if (currentCursor->y < 406) *selectedColor = DARK_BLUE;
			//game selection
			else if (currentCursor->y < 435 && currentCursor->y > 408) *currentTool = SNAKE;
		}
		//clear old selection and select the new tool
		if (lastTool != *currentTool) {
			draw_icon(lastTool, TOOL_NOT_SELECTED, lastDrawingData, pixel_buffer);
			lastTool = *currentTool;
			draw_icon(*currentTool, TOOL_SELECTED, lastDrawingData, pixel_buffer);
		}
		//clear old selection and select the new color
		if (lastColor != *selectedColor) {
			lastColor = *selectedColor;
			draw_color_palette(*selectedColor, lastDrawingData, pixel_buffer);
		}
	}
}

void process_cursor_pos(Cursor* currentCursor, int* x_pos, int* y_pos) {
	/**************************************************************************
	 * process_cursor_pos
	 **************************************************************************
	 * Parameters
	 * currentCursor: containt the actual cursor coordinate
	 * x_pos		: varable used by ps2_process to update the x position of the cursor
	 * y_pos		: varable used by ps2_process to update the y position of the cursor
	 *
	 * Return value
	 * none
	 *
	 * Side effects
	 * Apply a scaling and hard limite the range on the x_pos and y_pos  variable
	 * then store the result in the currentCursor structure
	 *
	 *************************************************************************/
	if (*x_pos > RIGHT_LIMIT * SCALE_FACTOR_INV) {
		currentCursor->x = RIGHT_LIMIT;
		*x_pos = RIGHT_LIMIT * SCALE_FACTOR_INV;
	}
	else if (*x_pos < LEFT_LIMIT * SCALE_FACTOR_INV) {
		currentCursor->x = LEFT_LIMIT;
		*x_pos = LEFT_LIMIT * SCALE_FACTOR_INV;
	}
	else {
		currentCursor->x = *x_pos * SCALE_FACTOR;
	}
	if (*y_pos > BOTTOM_LIMIT * SCALE_FACTOR_INV) {
		currentCursor->y = BOTTOM_LIMIT;
		*y_pos = BOTTOM_LIMIT * SCALE_FACTOR_INV;
	}
	else if (*y_pos < TOP_LIMIT * SCALE_FACTOR_INV) {
		currentCursor->y = TOP_LIMIT;
		*y_pos = TOP_LIMIT * SCALE_FACTOR_INV;
	}
	else {
		currentCursor->y = *y_pos * SCALE_FACTOR;
	}
}

void start_button(tool currentTool, char* startButtonPressed, unsigned char* left_btn, unsigned char* lastLeft, alt_u8* cursorMem,
	Cursor* currentCursor, lastDrawingVar* lastDrawingData, alt_up_pixel_buffer_dma_dev* pixel_buffer) {
	/**************************************************************************
	 * start_button
	 **************************************************************************
	 * Parameters
	 * currentTool			: current tool selected
	 * startButtonPressed	: pointer to a flag that use the
	 * left_btn				: variable that indicate a left button press on the mouse	 *
	 * cursorMem		: Array where to save the pixel value
	 * currentCursor	: containt the actual cursor coordinate
	 * lastDrawingData  : Structure that save multiple variable used to keep track of previous
	 *					  shape drawn during the interactive draw
	 * pixel_buffer		: is the pointer used to write in the pixel_buffer of the video pipeline
	 *
	 * Return value
	 * none
	 *
	 * Side effects
	 * Draw a start button at in the middle of the screen. If it is clicked reset the
	 * drawing app screen (draw tool bar and drawing zone, update the cursor memory
	 * of the newly drawn screen)
	 *
	 *************************************************************************/
	alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 290, 220, 350, 260, 16, 0);
	draw_icon_array(293, 227, 54, 27, startIconBmp, pixel_buffer);
	//if cursor over the button
	if (*left_btn) {
		//if cursor over the start button while left clic register the clic
		if (currentCursor->x > 290 && currentCursor->x < 350) {
			if (currentCursor->y > 220 && currentCursor->y < 260) {
				*lastLeft = 1;
			}
		}
	}
	else//when left button release, change flag to indicate start has been pressed then draw the drawing app UI
	{
		if (*lastLeft) {
			*startButtonPressed = 1;
			*lastLeft = 0;
			alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 62, 0, 640, 480, BACKGROUD_COLOR, 0);
			draw_tool_bar(currentTool, lastDrawingData, pixel_buffer);
			cursor_save(currentCursor, cursorMem);
		}
	}

}

void cursor_draw(char startUsingTool, Cursor* currentCursor, alt_u8* cursorMem, int* x_pos, int* y_pos, alt_up_pixel_buffer_dma_dev* pixel_buffer) {
	/**************************************************************************
	 * cursor_draw
	 **************************************************************************
	 * Parameters
	 * startUsingTool	: varaiable that indicate a tool is in use to avoid switching tool
	 *					  while the user is using one
	 * currentCursor	: containt the actual cursor coordinate
	 * cursorMem		: Array where to save the pixel value
	 * x_pos			: varable used by ps2_process to update the x position of the cursor
	 * y_pos			: varable used by ps2_process to update the y position of the cursor
	 * pixel_buffer		: is the pointer used to write in the pixel_buffer of the video pipeline
	 *
	 * Return value
	 * none
	 *
	 * Side effects
	 * When no tool are in use, erase the cursor, update the cursor positiion,
	 * save the pixel value where the new cursor will be, then draw the new cursor
	 *
	 *************************************************************************/
	 /* Manage cursor when not using tool*/
	if (startUsingTool == 0) {
		//erase old cursor
		cursor_erase(currentCursor, cursorMem, pixel_buffer);
		//Apply scaling and verify cursor is within the boundarys of the screen
		process_cursor_pos(currentCursor, x_pos, y_pos);
		//Save the last cursor pixel color for next turn in the loop
		cursor_save(currentCursor, cursorMem);
		//Draw cursor
		cursor_draw_sprite(currentCursor, pixel_buffer);
	}
}