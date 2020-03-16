#ifndef UTILS_H
#define UTILS_H

#include <cmath>

struct vect_3d
{
  float x,y,z;
};

vect_3d vect_3d_pv1qv2(float p, const vect_3d& v1, 
		       float q, const vect_3d& v2);

float vect3d_dot(const vect_3d& v1, const vect_3d& v2);

float vect_3d_norm(const vect_3d& v);

#endif
