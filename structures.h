#ifndef __STRUCT__
#define __STRUCT__

#include <vector>

struct vertex{
  float x, y, z, w;
};

vertex cross(vertex vt1, vertex vt2){
  vertex vn;
  vn.x = (vt1.y*vt2.z-vt1.z*vt2.y);
  vn.y = -(vt1.x*vt2.z-vt1.z*vt2.x);
  vn.z = (vt1.x*vt2.y-vt1.y*vt2.x);
  return vn;
}

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
