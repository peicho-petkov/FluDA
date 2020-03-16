#include "utils.h"
vect_3d vect_3d_pv1qv2(float p, const vect_3d& v1, 
		       float q, const vect_3d& v2)
{
  vect_3d res;
  res.x=p*v1.x+q*v2.x;
  res.y=p*v1.y+q*v2.y;
  res.z=p*v1.z+q*v2.z;
  return res;
};

float vect3d_dot(const vect_3d& v1, const vect_3d& v2)
{
  return v1.x*v2.x+v1.y*v2.y+v1.z*v2.z;
};

float vect_3d_norm(const vect_3d& v)
{
  float norm = 0.;
  norm=vect3d_dot(v,v);
  norm=sqrt(norm);
  return norm;
};
