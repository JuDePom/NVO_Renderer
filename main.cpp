#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <limits>

#include "tgaimage.h"
#include "renderer.h"
#include "structures.h"

std::vector<vertex> vertices;
std::vector<texture_coordinate> texture_coordinates;
std::vector<vertex_normal> vertex_normals;
std::vector<face> faces;

void readobj(const char *filename);
void draw(TGAImage &image, TGAImage &diffuse, TGAImage &normal_map, TGAImage &specular_map);


const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor black = TGAColor(  0,   0,   0, 255);
const TGAColor gray  = TGAColor(140, 140, 140, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);

const int render_width = 1000;
const int render_height = 1000;

matrice view_port = matrice(4, 4);

float z_buffer[render_width*render_height];


int main(int argc, char** argv) {
  TGAImage image(render_width, render_height, TGAImage::RGB);
  TGAImage diffuse, normal_map, specular_map;

  view_port.set(0,0, render_width/2);
  view_port.set(1,1, render_height/2);
  view_port.set(2,2, 1);
  view_port.set(3,3, 1);

  view_port.set(0,3, render_width/2);
  view_port.set(1,3, render_height/2);

  for (size_t i=render_width*render_height; i--; z_buffer[i] = -std::numeric_limits<float>::max());

  readobj("./afr/african_head_fun.obj");
  diffuse.read_tga_file("./afr/african_head_diffuse.tga");
  diffuse.flip_vertically();

  normal_map.read_tga_file("./afr/african_head_nm.tga");
  normal_map.flip_vertically();

  specular_map.read_tga_file("./afr/african_head_spec.tga");
  specular_map.flip_vertically();

  draw(image, diffuse, normal_map, specular_map);

  image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
  image.write_tga_file("output.tga");
  return 0;
}

void readobj(const char *filename){
  std::ifstream in;
  in.open (filename, std::ifstream::in);
  if (in.fail()) {
    std::cerr << "Cannot open " << filename << std::endl;
    return;
  }
  std::string line;
  while (!in.eof()) {
    std::getline(in, line);
    std::istringstream iss(line.c_str());
    char trash;
    if (!line.compare(0, 2, "v ")) {
      iss >> trash;
      vertex v;
      iss >> v.x >> v.y >> v.z;
      vertices.push_back(v);
    } else if (!line.compare(0, 2, "vt")) {
      iss >> trash >> trash;
      texture_coordinate vt;
      iss >> vt.u >> vt.v >> vt.w ;
      texture_coordinates.push_back(vt);
    } else if (!line.compare(0, 2, "vn")) {
      iss >> trash >> trash;
      vertex_normal vn;
      iss >> vn.x >> vn.y >> vn.z;
      vertex_normals.push_back(vn);
    } else if (!line.compare(0, 2, "f ")) {
      int vert, tex, norm;
      face f;
      iss >> trash;
      for( int i = 0; i < 3; i++) {
        iss >> vert >> trash >> tex >> trash >> norm;
        f.vertices.push_back(--vert);
        f.texture_coordinates.push_back(--tex);
        f.vertex_normals.push_back(--norm);
        faces.push_back(f);
      }
    }
  }
}

void draw(TGAImage &image, TGAImage &diffuse, TGAImage &normal_map, TGAImage &specular_map){
  for (int iface=0; iface < (int)faces.size(); iface++){
    if (faces[iface].vertices.size() > 2){
      vertex v1 = vertices[faces[iface].vertices[0]];
      vertex v2 = vertices[faces[iface].vertices[1]];
      vertex v3 = vertices[faces[iface].vertices[2]];

      texture_coordinate uv1 = texture_coordinates[faces[iface].texture_coordinates[0]];
      texture_coordinate uv2 = texture_coordinates[faces[iface].texture_coordinates[1]];
      texture_coordinate uv3 = texture_coordinates[faces[iface].texture_coordinates[2]];

      vertex_normal vn1 = vertex_normals[faces[iface].vertex_normals[0]];
      vertex_normal vn2 = vertex_normals[faces[iface].vertex_normals[1]];
      vertex_normal vn3 = vertex_normals[faces[iface].vertex_normals[2]];

      fillTriangle(v1, v2, v3, uv1, uv2, uv3, vn1, vn2, vn3, image, diffuse, normal_map, specular_map, z_buffer, view_port);
    }
  }
}
