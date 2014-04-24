#include "glass.inc"
#include "colors.inc"
#include "skies.inc"
#include "metals.inc" // Needed for "water"

global_settings { 
  assumed_gamma 2.2
  max_trace_level 5
}

camera {
	location <120, 35, 75>
	// location <50, 12.5, 25>
	// angle 40 
	right	x*image_width/image_height
	look_at <0, 10, 0>
}

light_source {
	<2000, 2000, -1000> color White
	// <200.0, 200.0, -150.0> colour red 1 green .5
}

// #declare Sky = sky_sphere { S_Cloud1 } // Purple sky
#declare Sky = sky_sphere { S_Cloud2 } // Purple sky

sky_sphere { Sky }                      // changed by dmf '95

// Define the ocean surface
plane { y, -10.0
   texture {
      T_Chrome_2D
      normal {
         waves 0.05
         frequency 5000.0
         scale 24000.0
      }
   }
}

// #include "boat.inc"
#include "test.inc"
#include "cessna1.inc"
#include "cessna2.inc"
#include "cessna3.inc"
// #include "dolphin1.inc"
// #include "dolphin2.inc"

// #declare Red = texture {
// 	pigment { color rgb<0.8, 0.2, 0.2> }
// 	finish { ambient 0.2 diffuse 0.5 }
// }
// #declare Green = texture {
// 	pigment { color rgb<0.2, 0.8, 0.2> }
// 	finish { ambient 0.2 diffuse 0.5 }
// }
// #declare Blue = texture {
// 	pigment { color rgb<0.2, 0.2, 0.8> }
// 	finish { ambient 0.2 diffuse 0.5 }
// }

// #declare Base_Tex = texture {
// 	pigment {
// 		granite
// 		turbulence 3
// 		color_map {
// 			[0.000 color rgb <0.5, 0.5, 0.2>]
// 			[0.25 color rgb <0.7, 1, 0.2>]
// 			[0.5 color rgb <1, 0.4, 0.2>]
// 			[0.75 color rgb <.5, 0.9, 0.2>]
// 			[1 color rgb <0.3, 0.9, 0.2>]
// 		}
// 		scale 0.035
// 	}
// 	finish { specular .25 }
// 	normal {
// 		average normal_map {
// 			[0.0 bumps 150 scale 30]
// 			[0.2 dents 50 scale 12.5 turbulence .3]
// 			[0.6 bump_map{ gif "GBM4.gif" bump_size 50} rotate x*90
// 			rotate y*45 scale 30 ]
// 			[1.0 wrinkles 25 scale 5 scallop_wave rotate z*30]
// 		}//end of normal map
// 		rotate z*15 turbulence .4
// 	}//end of normal dec
// }


// mesh {
// 	/* top side */
// 	triangle {
// 		<-10, 10, -10>, <10, 10, -10>, <10, 10, 10>
// 		texture { Base_Tex }
// 	}
// 	triangle {
// 		<-10, 10, -10>, <-10, 10, 10>, <10, 10, 10>
// 		texture { Base_Tex }
// 	}
// 	/* bottom side */
// 	triangle { <-10, -10, -10>, <10, -10, -10>, <10, -10, 10> }
// 	triangle { <-10, -10, -10>, <-10, -10, 10>, <10, -10, 10> }
// 	/* left side */
// 	triangle { <-10, -10, -10>, <-10, -10, 10>, <-10, 10, 10> }
// 	triangle { <-10, -10, -10>, <-10, 10, -10>, <-10, 10, 10> }
// 	/* right side */
// 	triangle {
// 		<10, -10, -10>, <10, -10, 10>, <10, 10, 10>
// 		texture { Base_Tex }
// 	}
// 	triangle {
// 		<10, -10, -10>, <10, 10, -10>, <10, 10, 10>
// 		texture { Base_Tex }
// 	}
// 	/* front side */
// 	triangle {
// 		<-10, -10, -10>, <10, -10, -10>, <-10, 10, -10>
// 		texture { Base_Tex }
// 	}
// 	triangle {
// 		<-10, 10, -10>, <10, 10, -10>, <10, -10, -10>
// 		texture { Base_Tex }
// 	}
// 	/* back side */
// 	triangle { <-10, -10, 10>, <10, -10, 10>, <-10, 10, 10> }
// 	triangle { <-10, 10, 10>, <10, 10, 10>, <10, -10, 10> }

// 	texture {
// 		pigment { color rgb<0.9, 0.9, 0.9> }
// 		finish { ambient 0.2 diffuse 0.7 }
// 	}
// }


// Light

// // Create the arches using CSG difference between two "squashed" paraboloids
// difference {
//    object { Paraboloid_Y
//       scale <20.0, 20.0, 5.0>
//       rotate 180*x
//       texture { T_Chrome_3C }
//    }
//    object { Paraboloid_Y
//       scale <18.0, 20.0, 18.0>
//       rotate 180*x
//       translate -2*y
//       texture { T_Copper_3C }
//    }
//    translate <0.0, 30.0, -25.0>
// }

// difference {
//    object { Paraboloid_Y
//       scale <20.0, 20.0, 5.0>
//       rotate 180*x
//       texture { T_Chrome_3C }
//    }
//    object { Paraboloid_Y
//       scale <18.0, 20.0, 18.0>
//       rotate 180*x
//       translate -2*y
//       texture { T_Copper_3C }
//    }
//    translate <0.0, 30.0, 50.0>
// }
