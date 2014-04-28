#include "glass.inc"
#include "colors.inc"
#include "skies.inc"
#include "metals.inc" // Needed for "water"

global_settings { 
  assumed_gamma 2.2
  max_trace_level 5
}

camera {
	location <35, 50, 25> // Final look position
  look_at <0, 30, 0> // Final look focus
}

// Sun
light_source {
	<60, 100, 80> color White
}

box {
  <-1, -1,   -1>,  // Near lower left corner
  < 1, 1,  1>   // Far upper right corner
  texture {
    // T_Glass1      // Pre-defined from stones.inc
    pigment { White }
  }
  scale 100
  translate 100*y
  translate 65*x
  translate 80*z
}

box {
  <-1, -1,   -1>,  // Near lower left corner
  < 1, 1,  1>   // Far upper right corner
  texture {
    pigment {
      uv_mapping
      image_map {
        png "smallJet.png"
      }
      rotate 180*y
      scale 0.25
    }
  }
  scale 10
  // scale 10*x
  // scale 10*y
  // rotate 20*x
  // rotate 225*y
  translate 45*y
}


#include "chair_table/table.inc"