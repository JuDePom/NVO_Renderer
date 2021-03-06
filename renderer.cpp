#include "renderer.h"

typedef vertex Light;

Light lightR = Light(1.f, 0.f, 0.f).normalize();
Light lightB = Light(1.f, 0.2f, 0.f).normalize();

float ambientLight = 0.05f;

vertex barycentre(vertex v1, vertex v2, vertex v3, int x, int y){
  vertex vt1 = vertex(v3.x-v1.x, v2.x-v1.x, v1.x-x);
  vertex vt2 = vertex(v3.y-v1.y, v2.y-v1.y, v1.y-y);

  vertex u = vt1 * vt2;

  vertex v;
  v.x = 1. - (u.x+u.y)/u.z;
  v.y = u.y/u.z;
  v.z = u.x/u.z;

  return v;
}

matrice rotation(float angle, char axis){
  double alpha = angle * M_PI/180;

  // Matrice de rotation
  matrice mres = matrice(4, 4).identity();

  int i, j;
  if ( axis == 'x'){
    i = 1; j = 2;
  }
  if ( axis == 'y'){
    i = 0; j = 2;
  }
  if ( axis == 'z'){
    i = 0; j = 1;
  }
  mres.set(i, i, cos(alpha));
  mres.set(i, j, -sin(alpha));
  mres.set(j, j, cos(alpha));
  mres.set(j, i, sin(alpha));
  return mres;
}

matrice projection() {
  // Matrice de projection
  matrice hard_coded = matrice(4, 4).identity();
  hard_coded.set(3,2, -1/5.);
  return hard_coded;
}


vertex_normal normal_mapping_global(TGAColor nColor){
  vertex_normal vn;
  vn.x = nColor.r;
  vn.y = nColor.g;
  vn.z = nColor.b;
  return vn;
}

vertex_normal gouraud_shading(vertex_normal vn1, vertex_normal vn2, vertex_normal vn3, vertex bar){
  vertex_normal vn;
  vn.x = vn1.x * bar.x + vn2.x * bar.y + vn3.x * bar.z;
  vn.y = vn1.y * bar.x + vn2.y * bar.y + vn3.y * bar.z;
  vn.z = vn1.z * bar.x + vn2.z * bar.y + vn3.z * bar.z;
  return vn;
}

void fillTriangle(vertex v1, vertex v2, vertex v3, texture_coordinate uv1, texture_coordinate uv2, texture_coordinate uv3, vertex_normal vn1, vertex_normal vn2, vertex_normal vn3, TGAImage &image, TGAImage &diffuse, TGAImage &normal_map, TGAImage &specular_map, float* z_buffer, matrice view_port){
  matrice transforms = view_port * projection() * rotation(10, 'x') * rotation(20, 'y');

  v1 = v1 * transforms;
  v2 = v2 * transforms;
  v3 = v3 * transforms;

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

          //vertex_normal vn = gouraud_shading(vn1, vn2, vn3, bar);
          vertex_normal vn = normal_mapping_global(normal_map.get(texture_x, texture_y));
          vn = vn.normalize();

          float lR = vn.dot(lightR);
          float lB = vn.dot(lightB);

          vertex vSpecR = (vn * lR) * 2.f;
          vSpecR = vSpecR - lightR;

          vertex vSpecB = (vn * lB) * 2.f;
          vSpecB = vSpecB - lightB;

          TGAColor specColor = specular_map.get(texture_x, texture_y);

          float specularR = std::pow( std::max(vSpecR.z, 0.f), 5 + specColor.b);
          float specularB = std::pow( std::max(vSpecB.z, 0.f), 5 + specColor.b);

          lR = std::max( lR + (1.0f * specularR), ambientLight );
          lB = std::max( lB + (1.0f * specularB), ambientLight );

          TGAColor color = diffuse.get(texture_x, texture_y);
          color.r = std::min(255.f * lR, 255.f);
          color.g = std::min(0.f, 255.f);
          color.b = std::min(255.f * lB, 255.f);

          image.set(i, j, color);
        }
      }
    }
  }
}
