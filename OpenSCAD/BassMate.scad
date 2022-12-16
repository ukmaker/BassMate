

include <FrontPanel.scad>
/**
* Uses the pre-release version 0.1 of ukmaker_openscad_lib
**/
include <ukmaker_openscad_lib/Boxes.scad>
$fn = 100;
/**
 * Model of an assembled BassMate
 **/

theta = asin ((20) / box_height);

LAYER_HEIGHT = 0.15;
thickness = 3;
screw_hole_dia = 3;
screw_hole_offset = thickness + 1.5 * screw_hole_dia - LAYER_HEIGHT;

power_socket_thread_dia = 11.5;
power_socket_flats = 10.5;
power_socket_height = 32;

headphones_dia = 6.8;
headphones_height = 15;


module headphone_socket() {

    translate([0,0,-LAYER_HEIGHT])
    cylinder(h = thickness *2, d= headphones_dia);

}

module power_socket() {
    // a cylinder with flats
    dy = (power_socket_thread_dia - power_socket_flats)/2;
    translate([0,0,-LAYER_HEIGHT])
    difference() {
        cylinder(h=thickness*2, d = power_socket_thread_dia);
        translate([-power_socket_thread_dia/2,-power_socket_thread_dia*1.5 + dy,-LAYER_HEIGHT])
        cube([power_socket_thread_dia,power_socket_thread_dia,thickness*3]);
        translate([-power_socket_thread_dia/2,power_socket_thread_dia*.5 - dy,-LAYER_HEIGHT])
        cube([power_socket_thread_dia,power_socket_thread_dia,thickness*3]);
    }
}

module
fitted_panel ()
{
  // THe front panel
  rotate ([ theta, 0, 0 ]) translate ([ thickness / 2, 0 / 2, 40 ]) panel ();
}

module
panel ()
{
  difference ()
  {
    option2 ();
    translate ([ -1.5, -1.5, 0 ])
        op_4_grid (box_width + 3.5, box_height + 3.5, screw_hole_offset,
                   screw_hole_offset, screw_hole_offset, screw_hole_offset)
            countersink (screw_hole_dia, thickness + 2 * LAYER_HEIGHT);
  }
}

module
body ()
{

  sloping_body (box_width + 3.5, box_height + 3.5, 40, 60, 3, 5, 3)
  {

    // Back panel cutouts are laid out on
    // a rectangle the width and rear_height of the boc
    // The sloping_body module rotates to the correct position
    // Speaker cutout on the right


    union() {
        translate ([ box_width - 30, 60 - 30, 0 ]) {                      
            speaker_grille_square(speaker_dia, thickness);
            // inset for the speaker rim
            translate([0,0,thickness-1])
            cylinder(h=speaker_lip+2*LAYER_HEIGHT, d=speaker_dia);
        }
        
        translate([box_width/2 - 24, headphones_height,0])
        headphone_socket();
        translate([box_width/2 - 24, power_socket_height,0])
        power_socket();
    }
    
     translate ([ box_width - 30, 60 - 30, 0 ]){
     union() {
        // holes are on a grid evenly spaced over the speaker
        l = speaker_mounting_hole_grid + 2 * speaker_mounting_hole_dia;
        w = l;
        offset = speaker_mounting_hole_dia/2;
        translate([-speaker_dia/2, -speaker_dia/2, 0])
        op_4_grid(l, w, offset, offset, offset, offset) {
            cylinder(h=speaker_mounting_hole_dia*3,d=speaker_mounting_hole_dia);
        }
    }
    }
  }
}

//fitted_panel ();
//power_socket();
body ();

// panel();