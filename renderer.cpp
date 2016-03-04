#include "renderer.h"

struct Light {
  float x, y, z;

  Light(float X, float Y, float Z) : x(X), y(Y), z(Z) {}
};

Light light = Light(0., 0., 1.);

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

void fillTriangle(vertex v1, vertex v2, vertex v3, texture_coordinate uv1, texture_coordinate uv2, texture_coordinate uv3, TGAImage &image, TGAImage &tex, float* z_buffer, float intensity, matrice view_port){
  double alpha = -20 * M_PI/180;

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
      vertex v = barycentre(v1, v2, v3, i, j);
      if ( v.x>=0 && v.y>=0 && v.z>=0 ) {
        float z = v.x*v1.z + v.y*v2.z + v.z*v3.z;
        if ( z > z_buffer[i+j*1000] ) {
          z_buffer[i+j*1000] = z;

          int tex_x = (v.x*uv1.u + v.y*uv2.u + v.z*uv3.u) * tex.get_width();
          int tex_y = (v.x*uv1.v + v.y*uv2.v + v.z*uv3.v) * tex.get_height();

          TGAColor fuck = tex.get(tex_x, tex_y);
          fuck.r *= intensity;
          fuck.g *= intensity;
          fuck.b *= intensity;

          image.set(i, j, fuck);
        }
      }
    }
  }
}

void triangle(vertex v1, vertex v2, vertex v3, texture_coordinate uv1, texture_coordinate uv2, texture_coordinate uv3, TGAImage &image, TGAImage &tex, float* z_buffer, matrice view_port){

  vertex vt1, vt2, vn;
  // Vectorisation
  vt1.x = (v2.x-v1.x);
  vt1.y = (v2.y-v1.y);
  vt1.z = (v2.z-v1.z);

  vt2.x = (v3.x-v1.x);
  vt2.y = (v3.y-v1.y);
  vt2.z = (v3.z-v1.z);

  vn = cross(vt1,vt2);

  // Normalisation
  float length = std::sqrt( vn.x*vn.x + vn.y*vn.y + vn.z*vn.z);
  vn.x /= length;
  vn.y /= length;
  vn.z /= length;

  // Produit scalaire
  float dot = vn.x*light.x + vn.y*light.y + vn.z*light.z;

  float intensity = std::abs(dot);

  //TGAColor ncolor = TGAColor(color.r * intensity, color.g * intensity, color.b * intensity, color.a);
  fillTriangle(v1, v2, v3, uv1, uv2, uv3, image, tex, z_buffer, intensity, view_port);
}
