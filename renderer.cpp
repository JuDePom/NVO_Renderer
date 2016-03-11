#include "renderer.h"

struct Light {
  float x, y, z;

  Light(float X, float Y, float Z) : x(X), y(Y), z(Z) {}
};

Light light = Light(0., 1., 0.);
float ambientLight = 0.1f;

vertex cross(vertex vt1, vertex vt2){
  vertex vn;
  vn.x = (vt1.y*vt2.z-vt1.z*vt2.y);
  vn.y = (vt1.z*vt2.x-vt1.x*vt2.z);
  vn.z = (vt1.x*vt2.y-vt1.y*vt2.x);
  return vn;
}

vertex barycentre(vertex v1, vertex v2, vertex v3, int x, int y){
  vertex vt1 = vertex(v3.x-v1.x, v2.x-v1.x, v1.x-x);
  vertex vt2 = vertex(v3.y-v1.y, v2.y-v1.y, v1.y-y);

  vertex u = cross(vt1, vt2);

  vertex v;
  v.x = 1. - (u.x+u.y)/u.z;
  v.y = u.y/u.z;
  v.z = u.x/u.z;

  return v;
}

void fillTriangle(vertex v1, vertex v2, vertex v3, texture_coordinate uv1, texture_coordinate uv2, texture_coordinate uv3, vertex_normal vn1, vertex_normal vn2, vertex_normal vn3, TGAImage &image, TGAImage &diffuse, TGAImage &normal_map, TGAImage &specular_map, float* z_buffer, matrice view_port){

  double alpha = 30 * M_PI/180;

  // Matrice de rotation
  matrice r = matrice(4, 4);
  r.set(0, 0, cos(alpha));
  r.set(0, 2, -sin(alpha));
  r.set(2, 2, cos(alpha));
  r.set(2, 0, sin(alpha));
  r.set(1, 1, 1);
  r.set(3, 3, 1);

  // Matrice de projection
  matrice hard_coded = matrice(4, 4);
  hard_coded.set(0,0, 1.);
  hard_coded.set(1,1, 1.);
  hard_coded.set(2,2, 1.);
  hard_coded.set(3,3, 1.);
  hard_coded.set(3,2, -1/3.);

  matrice res = view_port.multiply(hard_coded.multiply(r));

  matrice m1 = res.multiply(v1.toMat());
  matrice m2 = res.multiply(v2.toMat());
  matrice m3 = res.multiply(v3.toMat());

  v1 = vertex(m1);
  v2 = vertex(m2);
  v3 = vertex(m3);

  int minx = std::min(std::min(v1.x, v2.x), v3.x);
  int miny = std::min(std::min(v1.y, v2.y), v3.y);
  int maxx = std::max(std::max(v1.x, v2.x), v3.x);
  int maxy = std::max(std::max(v1.y, v2.y), v3.y);

  minx = std::max(0, minx);
  miny = std::max(0, miny);
  maxx = std::min(image.get_width()-1, maxx);
  maxy = std::min(image.get_height()-1, maxy);

  for (int i=minx; i<=maxx; i++){
    for (int j=miny; j<=maxy; j++){
      vertex bar = barycentre(v1, v2, v3, i, j);
      if ( bar.x>=0 && bar.y>=0 && bar.z>=0 ) {
        float z = bar.x*v1.z + bar.y*v2.z + bar.z*v3.z;
        if ( z > z_buffer[i+j*1000] ) {
          z_buffer[i+j*1000] = z;

          int texture_x = (bar.x*uv1.u + bar.y*uv2.u + bar.z*uv3.u) * diffuse.get_width();
          int texture_y = (bar.x*uv1.v + bar.y*uv2.v + bar.z*uv3.v) * diffuse.get_height();

          vertex_normal vn;
          /*
          == Gouraud shading ==
          vn.x = vn1.x * bar.x + vn2.x * bar.y + vn3.x * bar.z;
          vn.y = vn1.y * bar.x + vn2.y * bar.y + vn3.y * bar.z;
          vn.z = vn1.z * bar.x + vn2.z * bar.y + vn3.z * bar.z;
          */

          TGAColor normal_m = normal_map.get(texture_x, texture_y);
          vn.x = normal_m.r;
          vn.y = normal_m.g;
          vn.z = normal_m.b;

          float length = std::sqrt( vn.x*vn.x + vn.y*vn.y + vn.z*vn.z);
          vn.x /= length;
          vn.y /= length;
          vn.z /= length;

          float dot = vn.x*light.x + vn.y*light.y + vn.z*light.z;

          dot = std::min( std::max( dot, ambientLight), 1.f);

          TGAColor color = diffuse.get(texture_x, texture_y);
          color.r *= dot;
          color.g *= dot;
          color.b *= dot;

          image.set(i, j, color);
        }
      }
    }
  }
}
