#ifndef __STRUCT__
#define __STRUCT__

#include <vector>

struct ScreenCoord{
  int x, y;
};

struct vertex{
  float x, y, z, w;

  vertex() {}
  vertex(float X, float Y, float Z) : x(X), y(Y), z(Z) {}

  ScreenCoord _2D(){
    ScreenCoord sc;
    sc.x = (x+1)*400;
    sc.y = (y+1)*400;
    return sc;
  }
};

struct texture_coordinate{
  float u, v, w;
};

struct vertex_normal{
  float x, y, z;
};

struct face {
  std::vector<int> vertices;
  std::vector<int> texture_coordinates;
  std::vector<int> vertex_normals;
};

#endif
