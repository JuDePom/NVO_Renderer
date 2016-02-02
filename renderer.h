#include "tgaimage.h"
#include "structures.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <cmath>

void line(ScreenCoord p1, ScreenCoord p2, TGAImage &image, TGAColor color);
void triangle(vertex v1, vertex v2, vertex v3, TGAImage &image, TGAColor color, float* z_buffer, TGAImage tex);
void fillTriangle(vertex v1, vertex v2, vertex v3, TGAImage &image, TGAColor color, float* z_buffer, TGAImage tex);
