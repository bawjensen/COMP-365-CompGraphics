#include "glass.inc" // Needed for jet and ring
#include "metals.inc" // Needed for "water"

global_settings { 
  assumed_gamma 1.0
  // max_trace_level 5 // Un-comment to speed rendering - does not affect quality of output greatly
}

// Camera location and focus position
camera {
  location <90, 35, 90> // Final look position
  // location <45, 35, 45> // Up-close position
  right     x*image_width/image_height
  look_at <0, 10, 0> // Final look focus
}

// Sun - Simple light source
light_source {
	<-2000, 2000, -1000> rgb <1.0, 1.0, 1.0>
}

// Image mapping a cloudy sky image
#declare Clouds =
pigment {
  image_map {
    jpeg "sky2.jpg"
  }
  scale 0.8*y
}

// Sky - Sphere of a pigment map gradient into a cloudy-sky image texture
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

// Ocean - Metal sheet textured with "waves", and other qualities to make it water-like
plane { y, 0
   texture {
      pigment {
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

// Choose a version based on how quickly you want the image to render (3 Options)

// // Glass ring #1 - Fastest render, best quality, but built-in functionality
// lathe {
//   linear_spline
//   4,
//   <1.5,0>, <2,0.5>, <2.5,0>, <2,-0.5>
//   texture {
//     finish {
//       specular 1
//       roughness 0.001
//       ambient 0
//       diffuse 0
//       reflection 0.1
//     }
//     pigment { color rgbf<1.0, 1.0, 1.0, 1.0> }
//   }
//   interior {
//     ior 1.5
//     caustics 1
//   }
//   rotate 135*z
//   scale 10 // in all directions
//   translate <0, 18, 0>
// }


// // Glass ring #2 - Fast render, worst quality, custom generated
// #include "glassRing1000.inc"


// // Glass ring #3 - Slow render, great quality, custom generated
#include "glassRing4000.inc"


// // Glass ring #4 - Very fast render, square quality, custom generated
// #include "glassRing4.inc"


// SU-37 Jet file with smoke texture and glass qualities finish / interior qualities
#include "su-37/glassJet.inc"