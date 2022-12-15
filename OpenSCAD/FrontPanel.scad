LAYER_HEIGHT = 0.4;

use <ukmaker_openscad_lib/Generics.scad>

use <ukmaker_openscad_lib/Standoffs.scad>

use <ukmaker_openscad_lib/TFTs.scad>


use <ukmaker_openscad_lib/I2CNavKey.scad>
include <ukmaker_openscad_lib/I2CNavKey.h>


panel_thickness = 3;

panel_rounding = 5;

trellis_button_side = 11;
trellis_button_grid = 15;
trellis_button_squish = 2;

trellis_width = 60;
trellis_height = 60;

trellis_rows = 4;
trellis_cols = 8;

tbt_side = 15.5;
tbt_grid = 25;

tft_w = 80;
tft_h = 50;

encoder_side = 12.5;
encoder_recess_depth = 1;
encoder_hole_dia = 8;


box_margin = 10;
box_width = tft_w + i2c_navkey_pcb_side + 3 * box_margin;
box_height =  trellis_button_grid * (trellis_rows-1)+ trellis_button_side + 2 * box_margin + tft_h + box_margin;

speaker_dia = 40;
speaker_lip = 1;
speaker_mounting_hole_dia = 3;
speaker_mounting_hole_grid = 33;

module button(h, w, r) {
    linear_extrude(height = h) rounded_square(h, w, r);
}

module buttons(h, w, r, s, rows, cols) {
    
    for(y = [0:rows-1]) {
        for(x = [0:cols-1]) {
            translate([x * s, y * s, 0])
            button(h,w,r);
        }
    }
    
}

module tft() {
    // overall 46x34 (56x34)
    // screen 35x28
    translate([0,0,-1]) {
        color("Red")
        linear_extrude(1)
        square([80,50]);
        
        color("Black")
        linear_extrude(4)
        translate([7.5,0,1])
        square([70,50]);
    }
}

module knob(c) {
    
    translate([0,0,3]) {
        translate([0,0,-10])
    cylinder(h=10, r=3.5);
    color("Grey")
    cylinder(h=18,r=6);
    translate([0,0,18])
    color(c)
    cylinder(h=1,r=6);
    }
}

module knobs() {
    knob("Red");
    
    translate([30,0,0])
    knob("Blue");
    
    translate([60,0,0])
    knob("Green");
    
    translate([90,0,0])
    knob("Black");
    
    
}
        
$fn=100;

module trellis() {
    buttons(trellis_button_side, trellis_button_side, 1, trellis_button_grid, 4, 8);
    
    screw_spacing = trellis_button_side+(trellis_button_grid-trellis_button_side)/2;
    trellis_side = trellis_button_grid*3 + trellis_button_side;
    
    op_4_grid(trellis_side,trellis_side,
    screw_spacing,screw_spacing,screw_spacing,screw_spacing)
    countersink(2,panel_thickness+LAYER_HEIGHT);

    translate([trellis_side+trellis_button_grid - trellis_button_side,0,0])
    op_4_grid(trellis_side,trellis_side,
    screw_spacing,screw_spacing,screw_spacing,screw_spacing)
    countersink(2,panel_thickness+LAYER_HEIGHT);
}

module control_buttons() {
    buttons(tbt_side, tbt_side, 1, tbt_grid, 2, 2);
}

module panel() {
    color("White")
    cube([260, 105, 2.5]);
}

module option1() {
projection()
difference() {
    translate([-10, -20, 0.1])
    panel();

    translate([0, - (tbt_side/2 + tbt_grid/2),0])
    translate([0, (trellis_button_side/2 + trellis_button_grid * 3/2), 0])
    control_buttons();

    translate([tbt_side * 2 + tbt_grid, 0, 0])
    trellis();

    translate([235,-15,0])
    rotate([0,0,90])
    tft();

    translate([70,70,0])
    knobs();
}
}

module i2c_navkey_locate() {
    translate([box_width - i2c_navkey_dia_1/2 - box_margin,i2c_navkey_dia_1/2 + box_margin,-0.1])
    children();
}

module option2() {
    // panel
    difference() {
        
         linear_extrude(height = panel_thickness) rounded_square(box_width, box_height, panel_rounding);

        //cube([box_width, box_height, 2.5]);
        tl = (box_width - trellis_width * 2) / 2;
        translate([tl, trellis_button_grid * 5 - 2, -0.1])
        trellis();
        
        // navkey hole
        i2c_navkey_locate()
        cylinder(h=panel_thickness+LAYER_HEIGHT,d=i2c_navkey_dia_2 + i2c_navkey_rim_clearance);
        
        i2c_navkey_locate()
        cylinder(h=i2c_navkey_joypad_rim_thickness + 1,d=i2c_navkey_dia_1 + 1);
        
        // TFT hole
        translate([box_margin,box_margin,-0.1]) {
        mnt_cutout_bezel_tft_2_8(thickness, panel_thickness+LAYER_HEIGHT);
        }
        
        // encoder holes
        translate([-encoder_side-4, i2c_navkey_pcb_side/2 + encoder_side/2 + 1, -0.1])
        i2c_navkey_locate() {
        cylinder(h=panel_thickness+LAYER_HEIGHT, d=encoder_hole_dia);
        translate([-encoder_side/2,-encoder_side/2,-LAYER_HEIGHT])
        cube([encoder_side,encoder_side,encoder_recess_depth+LAYER_HEIGHT]);
        
        }

        translate([encoder_side+4, i2c_navkey_pcb_side/2 + encoder_side/2 + 1, -0.1])
        i2c_navkey_locate() {
        cylinder(h=panel_thickness+LAYER_HEIGHT, d=encoder_hole_dia);
        translate([-encoder_side/2,-encoder_side/2,-LAYER_HEIGHT])
        cube([encoder_side,encoder_side,encoder_recess_depth+LAYER_HEIGHT]);
        }
    }
        
     
    i2c_navkey_locate() {
        mirror([0,0,1])
        i2c_navkey_melt_standoffs(i2c_navkey_board_offset - panel_thickness + 0.5, 4, LAYER_HEIGHT );
    }
    
    translate([box_margin,box_margin,-LAYER_HEIGHT]) {
       mirror([0,0,1])
        mnt_melt_bezel_tft_2_8(LAYER_HEIGHT);
    }
    translate([box_margin,box_margin,0]) {
        // Add the bezel
        mnt_bezel_tft_2_8(thickness, thickness);
    }
}

//option2();
