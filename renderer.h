#include "tgaimage.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <cmath>

void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color);
void triangle(int x0, int y0, int x1, int y1, int x2, int y2, TGAImage &image, TGAColor color);
void fillTriangle(int x0, int y0, int x1, int y1, int x2, int y2, TGAImage &image, TGAColor color);
