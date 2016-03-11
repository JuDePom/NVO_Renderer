#include "tgaimage.h"
#include "structures.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <cmath>

void fillTriangle(vertex v1, vertex v2, vertex v3, texture_coordinate uv1, texture_coordinate uv2, texture_coordinate uv3, vertex_normal vn1, vertex_normal vn2, vertex_normal vn3, TGAImage &image, TGAImage &diffuse, TGAImage &normal_map, TGAImage &specular_map, float* z_buffer, matrice view_port);
