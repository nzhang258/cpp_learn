#include </usr/include/eigen3/Eigen/Dense>
#include <vector>
#include <stdio.h>
#include <fstream>
#include <iostream>

struct V {
  float x,y,z;
  V() : x(0),y(0),z(0) {}
  V(float _x, float _y, float _z):x(_x),y(_y),z(_z) {}
};

struct F {
  int a, b, c;
  F() : a(-1), b(-1), c(-1) {}
  F(int _a, int _b, int _c) {}
};

struct Shape {
  std::vector<V> vertex;
  std::vector<F> face;
};

bool load_shape(const char *filename, Shape &shape) {
  std::ifstream ifs(filename);
  if (!ifs) {
    printf("%s failed to open %s\n", __func__, filename);
	return false;
  }
  int v_n,f_n;
  ifs>>v_n>>f_n;
  shape.vertex.resize(v_n);
  shape.face.resize(f_n);
  for(int i = 0; i<v_n; ++i) {
    V &v = shape.vertex[i];
	ifs>>v.x>>v.y>>v.z;
  }
  for(int i = 0; i<f_n; ++i) {
    F &f = shape.face[i];
	ifs>>f.a>>f.b>>f.c;
  }
  
  ifs.close();
  
  return true;
}

bool save_shape(const char *filename, Shape &shape) {
  
  FILE *ofs = fopen(filename, "w");
  if(!ofs) {
    printf("%s failed to open %s\n", __func__, filename);
	return false;
  }
  
  for (auto v1 : shape.vertex) {
    fprintf(ofs, "v %f %f %f\n", v1.x, v1.y, v1.z);
  }
  for (auto f : shape.face) {
    fprintf(ofs, "f %d %d %d\n", f.a, f.b, f.c);
  }
  
  fclose(ofs);
  
  return true;

}

int main(int argc, char **argv) {

  Shape model;
  load_shape("./face.txt", model);
  save_shape("./face.obj",model);
  return 0;
 
}
