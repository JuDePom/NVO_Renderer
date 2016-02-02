#include "renderer.h"

struct Light{
  float x, y, z;

  Light(float X, float Y, float Z) : x(X), y(Y), z(Z) {
  }
};

Light light = Light(0., 0., 1.);

vertex cross(vertex vt1, vertex vt2){
  vertex vn;
  vn.x = (vt1.y*vt2.z-vt1.z*vt2.y);
  vn.y = -(vt1.x*vt2.z-vt1.z*vt2.x);
  vn.z = (vt1.x*vt2.y-vt1.y*vt2.x);
  return vn;
}

void line(ScreenCoord p1, ScreenCoord p2, TGAImage &image, TGAColor color) {
  int x0 = p1.x, y0 = p1.y;
  int x1 = p2.x, y1 = p2.y;

  bool steep = false;
  if (std::abs(x0-x1)<std::abs(y0-y1)) {
    std::swap(x0, y0);
    std::swap(x1, y1);
    steep = true;
  }
  if (x0>x1) {
    std::swap(x0, x1);
    std::swap(y0, y1);
  }
  int dx = x1-x0;
  int dy = y1-y0;
  int derror2 = std::abs(dy)*2;
  int error2 = 0;
  int y = y0;
  for (int x=x0; x<=x1; x++) {
    if (steep) {
      image.set(y, x, color);
    } else {
      image.set(x, y, color);
    }
    error2 += derror2;
    if (error2 > dx) {
      y += (y1>y0?1:-1);
      error2 -= dx*2;
    }
  }
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

void fillTriangle(vertex v1, vertex v2, vertex v3, TGAImage &image, TGAColor color, float* z_buffer){
  v1.x += 1; v1.x *= 500;
  v1.y += 1; v1.y *= 500;

  v2.x += 1; v2.x *= 500;
  v2.y += 1; v2.y *= 500;

  v3.x += 1; v3.x *= 500;
  v3.y += 1; v3.y *= 500;

  int minx = (v1.x < v2.x ? v1.x : v2.x );
  minx = (minx < v3.x ? minx : v3.x);

  int miny = (v1.y < v2.y ? v1.y : v2.y );
  miny = (miny < v3.y ? miny : v3.y);

  int maxx = (v1.x > v2.x ? v1.x : v2.x );
  maxx = (maxx > v3.x ? maxx : v3.x);

  int maxy = (v1.y > v2.y ? v1.y : v2.y );
  maxy = (maxy > v3.y ? maxy : v3.y);

  for (int i=minx; i<=maxx; i++){
    for (int j=miny; j<=maxy; j++){
      vertex v = barycentre(v1, v2, v3, i, j);
      if ( v.x>=0 && v.y>=0 && v.z>=0 ) {
        float z = v.x*v1.z + v.y*v2.z + v.z*v3.z;
        if ( z > z_buffer[i+j*1000] ) {
          z_buffer[i+j*1000] = z;
          image.set(i, j, color);
        }
      }
    }
  }
}

void triangle(vertex v1, vertex v2, vertex v3, TGAImage &image, TGAColor color, float* z_buffer){

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

  TGAColor ncolor = TGAColor(color.r * intensity, color.g * intensity, color.b * intensity, color.a);
  fillTriangle(v1, v2, v3, image, ncolor, z_buffer);
}
