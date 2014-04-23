#include "glass.inc"
#include "colors.inc"
#include "stones.inc"

camera {
	location <0, 0, -50>
	look_at <0, 0, 0>
}

light_source {
	<50, 0, 0> color White
}

#declare Red = texture {
	pigment { color rgb<0.8, 0.2, 0.2> }
	finish { ambient 0.2 diffuse 0.5 }
}
#declare Green = texture {
	pigment { color rgb<0.2, 0.8, 0.2> }
	finish { ambient 0.2 diffuse 0.5 }
}
#declare Blue = texture {
	pigment { color rgb<0.2, 0.2, 0.8> }
	finish { ambient 0.2 diffuse 0.5 }
}

#declare Base_Tex = texture {
	pigment {
		granite
		turbulence 3
		color_map {
			[0.000 color rgb <0.5, 0.5, 0.2>]
			[0.25 color rgb <0.7, 1, 0.2>]
			[0.5 color rgb <1, 0.4, 0.2>]
			[0.75 color rgb <.5, 0.9, 0.2>]
			[1 color rgb <0.3, 0.9, 0.2>]
		}
		scale 0.035
	}
	finish { specular .25 }
	normal {
		average normal_map {
			[0.0 bumps 150 scale 30]
			[0.2 dents 50 scale 12.5 turbulence .3]
			[0.6 bump_map{ gif "GBM4.gif" bump_size 50} rotate x*90
			rotate y*45 scale 30 ]
			[1.0 wrinkles 25 scale 5 scallop_wave rotate z*30]
		}//end of normal map
		rotate z*15 turbulence .4
	}//end of normal dec
}

#include "test.inc"

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