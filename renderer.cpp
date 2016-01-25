#include "renderer.h"

struct Light{
  float x, y, z;

  Light(float X, float Y, float Z) : x(X), y(Y), z(Z) {
  }
};

Light light = Light(0., 0., 1.);

void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
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


bool barycentre(int x0, int y0, int x1, int y1, int x2, int y2, int x, int y){
  vertex vt1, vt2;
  vt1.x = x2 - x0;
  vt1.y = x1 - x0;
  vt1.z = x0 - x;

  vt2.x = y2 - y0;
  vt2.y = y1 - y0;
  vt2.z = y0 - y;

  vertex u = cross(vt1, vt2);

  vertex v;
  v.x = 1. - (u.x+u.y)/u.z;
  v.y = u.y/u.z;
  v.z = u.x/u.z;

  if (v.x >= 0 && v.y >= 0 && v.z >= 0)
    return true;
  return false;
}

void fillTriangle(int x0, int y0, int x1, int y1, int x2, int y2, TGAImage &image, TGAColor color){
  int minx = (x0 < x1 ? x0 : x1 );
  minx = (minx < x2 ? minx : x2);

  int miny = (y0 < y1 ? y0 : y1 );
  miny = (miny < y2 ? miny : y2);

  int maxx = (x0 > x1 ? x0 : x1 );
  maxx = (maxx > x2 ? maxx : x2);

  int maxy = (y0 > y1 ? y0 : y1 );
  maxy = (maxy > y2 ? maxy : y2);

  for (int i=minx; i<maxx; i++){
    for (int j=miny; j<maxy; j++){
      if ( barycentre(x0, y0, x1, y1, x2, y2, i, j) ) {
        image.set(i, j, color);
      }
    }
  }


}

void triangle(vertex v1, vertex v2, vertex v3, TGAImage &image, TGAColor color){

  vertex vt1, vt2, vn;
  // Vectorisation
  vt1.x = (v2.x-v1.x);
  vt1.y = (v2.y-v1.y);
  vt1.z = (v2.z-v1.z);

  vt2.x = (v3.x-v1.x);
  vt2.y = (v3.y-v1.y);
  vt2.z = (v3.z-v1.z);

  vn = cross(vt1,vt2);

  //vn.x = (vt1.y*vt2.z-vt1.z*vt2.y);
  //vn.y = (vt1.x*vt2.z-vt1.z*vt2.x);
  //vn.z = (vt1.x*vt2.y-vt1.y*vt2.x);

  // Normalisation
  float length = std::sqrt( vn.x*vn.x + vn.y*vn.y + vn.z*vn.z);
  vn.x = vn.x / length;
  vn.y = vn.y / length;
  vn.z = vn.z / length;

  // Produit scalaire
  float dot = vn.x*light.x + vn.y*light.y + vn.z*light.z;

  float intensity = dot;

  if(intensity >= 0.){
    TGAColor ncolor = TGAColor(color.r * intensity, color.g * intensity, color.b * intensity, color.a);
    fillTriangle((v1.x+1)*400, (v1.y+1)*400, (v2.x+1)*400, (v2.y+1)*400, (v3.x+1)*400, (v3.y+1)*400, image, ncolor);
  }
}
