#include "tgaimage.h"
#include "structures.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <cmath>

void triangle(vertex v1, vertex v2, vertex v3, texture_coordinate uv1, texture_coordinate uv2, texture_coordinate uv3, TGAImage &image, TGAImage &tex, float* z_buffer, matrice view_port);
void fillTriangle(vertex v1, vertex v2, vertex v3, texture_coordinate uv1, texture_coordinate uv2, texture_coordinate uv3, TGAImage &image, TGAImage &tex, float* z_buffer, float intensity, matrice view_port);
