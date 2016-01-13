#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "tgaimage.h"
#include "renderer.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);


struct vertex{
  float x, y, z;
};

struct face {
  int v1, v2, v3;
};

std::vector<vertex> vertices;
std::vector<face> faces;

void readobj(const char *filename);
void wire(TGAImage &image, TGAColor color);

int main(int argc, char** argv) {
  readobj("./african.obj");
  TGAImage image(800, 800, TGAImage::RGB);
  wire(image, red);
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
        } else if (!line.compare(0, 2, "f ")) {
            face f;
            int itrash;
            iss >> trash;
	    iss >> f.v1 >> trash >> itrash >> trash >> itrash;
	    iss >> f.v2 >> trash >> itrash >> trash >> itrash;
	    iss >> f.v3 >> trash >> itrash >> trash >> itrash;

	    f.v1--;
	    f.v2--;
	    f.v3--;

            faces.push_back(f);
        }
    }
    std::cerr << "# v# " << vertices.size() << " f# "  << faces.size() << std::endl;
}

void wire(TGAImage &image, TGAColor color){
  std::cerr << faces.size() <<  "gjkghkjgkgkgjvgjhg\n";

  vertex v_1a = vertices[faces[0].v1];
  vertex v_2a = vertices[faces[0].v2];
  vertex v_3a = vertices[faces[0].v3];

std::cerr << v_1a.x << " " << v_1a.y << std::endl;
std::cerr << v_2a.x << " " << v_2a.y << std::endl;
std::cerr << v_3a.x << " " << v_3a.y << std::endl;

  for (int i=0; i<(int)faces.size(); i++ ){
    vertex v_1 = vertices[faces[i].v1];
    vertex v_2 = vertices[faces[i].v2];
    vertex v_3 = vertices[faces[i].v3];

    line((v_1.x + 1) * 400, (v_1.y+1)*400, (v_2.x+1)*400, (v_2.y+1)*400, image, color);
    line((v_1.x + 1) * 400, (v_1.y+1)*400, (v_3.x+1)*400, (v_3.y+1)*400, image, color);
    line((v_3.x + 1) * 400, (v_3.y+1)*400, (v_2.x+1)*400, (v_2.y+1)*400, image, color);
  }
}

void projected_wire(float x, float y, float z, float t, TGAImage &image, TGAColor){

}
