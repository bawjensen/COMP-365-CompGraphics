#include "glass.inc"
#include "colors.inc"
#include "skies.inc"
#include "metals.inc" // Needed for "water"

global_settings { 
  assumed_gamma 2.2
  max_trace_level 5
}

camera {
  location <90, 35, 90> // Final look position
	// location <45, 35, 45> // Testing
  look_at <0, 10, 0> // Final look focus
}

// Sun
light_source {
	<-2000, 2000, -1000> color White
}

// Image mapping the clouds
#declare Clouds =
pigment {
  image_map {
    jpeg "sky2.jpg"
  }
  scale 0.8*y
  // scale 1.5
}

// Sky
sky_sphere {
  pigment {
    gradient y
      pigment_map {
      [0.00 rgb <0.847, 0.749, 0.847> ] // horizon
      [0.05 rgb <0.875, 0.875, 1.0> ]   // horizon
      [0.10 Clouds ]
    }
  }
}

// Ocean
plane { y, 0
   texture {
      pigment {
        // image_map {
        //   jpeg "water3.jpg"
        // }
        // scale 1000
        color rgb <0.39, 0.41, 0.43>
      }
      finish {
        ambient 0.15
        brilliance 5
        diffuse 0.6
        metallic
        specular 0.80
        roughness 1/100
        reflection 0.65
      }
      normal {
         waves 0.05
         frequency 5000.0
         scale 30000.0
      }
   }
   rotate y*30
}

// Glass ring
lathe {
  linear_spline
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
    caustics 1
  }
  rotate 135*z
  scale 10 // in all directions
  translate <0, 18, 0>
}

#include "su-37/glassJet.inc"