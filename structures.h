#ifndef __STRUCT__
#define __STRUCT__

#include <vector>
#include <stdlib.h>
#include <iostream>
#include <cassert>

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

  matrice multiply(matrice m2){
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
  float x, y, z, w;

  vertex() : x(0), y(0), z(0), w(1) {}
  vertex(float X, float Y, float Z) : x(X), y(Y), z(Z), w(1) {}
  vertex(matrice m){
    assert(m.rows==4 && m.cols==1);

    w = m.get(3,0);
    x = m.get(0,0) / w;
    y = m.get(1,0) / w;
    z = m.get(2,0) / w;
    w = 1;
  }

  matrice toMat(){
    matrice mres = matrice(4, 1);
    mres.set(0, 0, x);
    mres.set(1, 0, y);
    mres.set(2, 0, z);
    mres.set(3, 0, w);

    return mres;
  }
};

struct texture_coordinate{
  float u, v, w;
};

struct vertex_normal{
  float x, y, z;

  vertex_normal() : x(0), y(0), z(0) {};
};

struct face {
  std::vector<int> vertices;
  std::vector<int> texture_coordinates;
  std::vector<int> vertex_normals;
};

#endif
