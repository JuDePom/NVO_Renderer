#ifndef __STRUCT__
#define __STRUCT__

#include <vector>
#include <stdlib.h>
#include <iostream>
#include <cassert>
#include <cmath>

struct vertex;

struct ScreenCoord{
  int x, y;
};


struct matrice {
  float *data;
  unsigned int rows;
  unsigned int cols;

  matrice(unsigned int rows, unsigned int cols) : rows(rows), cols(cols) {
    data = (float*) calloc (rows * cols, sizeof(float));
  }

  float get(unsigned int i, unsigned int j) {
    return data[i + j*cols];
  }

  void set(unsigned int i, unsigned int j, float val) {
    data[i + j*cols] = val;
  }

  matrice identity(){
    assert(cols == rows);

    matrice mres = matrice(cols, cols);
    for (unsigned int i = cols; i--; mres.set(i, i, 1) );
    return mres;
  }

  matrice operator*(matrice m2){
    assert (cols == m2.rows);

    matrice mres = matrice (rows, m2.cols);

    for (unsigned int i = 0; i < rows; i++) {
      for (unsigned int j = 0; j < m2.cols; j++) {
        float val = 0;
        for (unsigned int k = 0; k < cols; k++) {
          val += get(i, k) * m2.get(k, j);
        }
        mres.set(i, j, val);
      }
    }
    return mres;
  }

  void display(){
    for (unsigned int i = 0; i < rows; i++) {
      for (unsigned int j = 0; j < cols; j++) {
        std::cout << data[i +j*cols] << "\t";
      }
      std::cout << std::endl;
    }
  }
};

struct vertex{
  float x, y, z, t;

  vertex() : x(0), y(0), z(0), t(1) {}
  vertex(float X, float Y, float Z) : x(X), y(Y), z(Z), t(1) {}

  vertex(matrice m){
    assert(m.rows==4 && m.cols==1);

    t = m.get(3,0);
    x = m.get(0,0) / t;
    y = m.get(1,0) / t;
    z = m.get(2,0) / t;
    t = 1;
  }

  vertex operator*(vertex vt){
    vertex vres;
    vres.x = (y * vt.z - z * vt.y);
    vres.y = (z * vt.x - x * vt.z);
    vres.z = (x * vt.y - y * vt.x);
    return vres;
  }

  vertex operator*(matrice m){
    matrice mtmp = toMat();
    return vertex( m * mtmp );
  }

  float dot(vertex vt){
    return vt.x*x + vt.y*y + vt.z*z;
  }

  vertex normalize(){
    vertex vres;
    float length = std::sqrt(x*x + y*y + z*z);
    vres.x = x / length;
    vres.y = y / length;
    vres.z = z / length;
    return vres;
  }

  matrice toMat(){
    matrice mres = matrice(4, 1);
    mres.set(0, 0, x);
    mres.set(1, 0, y);
    mres.set(2, 0, z);
    mres.set(3, 0, t);

    return mres;
  }
};

struct texture_coordinate{
  float u, v, w;
};

typedef vertex vertex_normal;

struct face {
  std::vector<int> vertices;
  std::vector<int> texture_coordinates;
  std::vector<int> vertex_normals;
};

#endif
