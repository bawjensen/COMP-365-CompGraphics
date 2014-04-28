#include "glass.inc"

lathe {
  linear_spline
  // quadratic_spline
  4,
  <1.5,0>, <2,0.5>, <2.5,0>, <2,-0.5>
  texture {
    finish {
      specular 1
      roughness 0.001
      ambient 0
      diffuse 0
      reflection 0.1
    }
    pigment { color rgbf<1.0, 1.0, 1.0, 1.0> }
  }
  interior {
    ior 1.5
    // fade_distance 5
    // fade_power 1
    caustics 1
  }
  rotate 135*z
  scale 10 // in all directions
  translate <0, 18, 0>
}