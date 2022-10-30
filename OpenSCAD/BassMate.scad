include <FrontPanel.scad>

include <../../ukmaker_openscad_lib/Boxes.scad>

/**
* Model of an assembled BassMate
**/  

theta = asin ((20) / box_height);

LAYER_HEIGHT = 0.4;
thickness = 3;
screw_hole_dia = 3;
screw_hole_offset = thickness + 1.5 * screw_hole_dia - LAYER_HEIGHT;


module fitted_panel() {
// THe front panel
rotate([0,theta,0])
translate([box_height,1.5,60])
rotate([0,0,90])
    panel();
}

module panel() {
difference() {
    option2();
    translate([-1.5,-1.5,0])
    op_4_grid (box_width+3,
                        box_height+3,
                        screw_hole_offset,screw_hole_offset,screw_hole_offset,screw_hole_offset)
    countersunk(screw_hole_dia, thickness);
    
}
}

module body() {

sloping_body (box_height+3, box_width+3, 40, 60, 3, 5, 3);
}

//panel();

body();