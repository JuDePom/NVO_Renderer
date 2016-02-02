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
void wire(TGAImage &image, TGAColor color);
void draw(TGAImage &image, TGAColor color);


const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor black = TGAColor(  0,   0,   0, 255);
const TGAColor gray  = TGAColor(140, 140, 140, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);

float z_buffer[1000*1000];

int main(int argc, char** argv) {

  for (size_t i=1000*1000; i--; z_buffer[i] = -std::numeric_limits<float>::max());


  readobj("./african.obj");
  TGAImage image(1000, 1000, TGAImage::RGB);
  draw(image, white);
  //wire(image, gray);
  image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
  image.write_tga_file("output.tga");
  return 0;
}

void readobj(const char *filename){
  std::ifstream in;
  in.open (filename, std::ifstream::in);
  if (in.fail()) {
    std::cerr << "cant open " << filename << std::endl;
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
      iss >> trash;
      texture_coordinate vt;
      iss >> vt.u >> vt.v;
      texture_coordinates.push_back(vt);
    } else if (!line.compare(0, 2, "vn")) {
      iss >> trash;
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

void wire(TGAImage &image, TGAColor color){
  for (int iface=0; iface < (int)faces.size(); iface++){
    int vertNb = (int)faces[iface].vertices.size();

    for (int ivert=1; ivert < vertNb ; ivert++){
      vertex v1 = vertices[faces[iface].vertices[ivert-1]];
      vertex v2 = vertices[faces[iface].vertices[ivert]];

      line(v1._2D(), v2._2D(), image, color);
    }

    vertex v1 = vertices[faces[iface].vertices[vertNb-1]];
    vertex v2 = vertices[faces[iface].vertices[0]];
    line(v1._2D(), v2._2D(), image, color);
  }
}

void draw(TGAImage &image, TGAColor color, TGAImage &tex){
  for (int iface=0; iface < (int)faces.size(); iface++){
    int vertNb = (int)faces[iface].vertices.size();

    for (int ivert=2; ivert < vertNb ; ivert++){
      vertex v1 = vertices[faces[iface].vertices[ivert-2]];
      vertex v2 = vertices[faces[iface].vertices[ivert-1]];
      vertex v3 = vertices[faces[iface].vertices[ivert]];

      triangle(v1, v2, v3, image, color, z_buffer);
    }
  }
}

void projected_wire(float x, float y, float z, float t, TGAImage &image, TGAColor){

}
