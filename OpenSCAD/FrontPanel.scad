trellis_button_side = 10.5;
trellis_button_grid = 15;
trellis_button_squish = 2;

trellis_rows = 4;
trellis_cols = 8;

tbt_side = 15.5;
tbt_grid = 25;

tft_w = 80;
tft_h = 50;

encoder_side = 14;
encoder_hole_dia = 8;

i2c_navkey_pcb_side = 42.5;
i2c_navkey_mounting_hole_centres = 34;
i2c_navkey_mounting_hole_dia = 3;

i2c_navkey_standoff_hole_dia = 2;
i2c_navkey_standoff_inner_dia = 2.9;
i2c_navkey_standoff_outer_dia = 4;
i2c_navkey_standoff_inner_height = 1;
i2c_navkey_standoff_outer_height = 3;

i2c_navkey_dia_1 = 34.4;
i2c_navkey_dia_2 = 32;
i2c_navkey_dia_3 = 22.9;
i2c_navkey_dia_4 = 8.1;
i2c_navkey_joypad_height = 3;
i2c_navkey_board_offset = 6;

box_margin = 10;
box_width = tft_w + i2c_navkey_pcb_side + 3 * box_margin;
box_height =  trellis_button_grid * (trellis_rows-1)+ trellis_button_side + 2 * box_margin + tft_h + box_margin;
module i2c_navkey() {
    t1 = (i2c_navkey_pcb_side - i2c_navkey_mounting_hole_centres)/2;
    t2 = i2c_navkey_pcb_side - t1;
    
    
    color("Gray")
    translate([-i2c_navkey_pcb_side/2, -i2c_navkey_pcb_side/2, 0]) {
    // PCB
    difference() {

        translate([0,0,-0.1])
        cube([i2c_navkey_pcb_side, i2c_navkey_pcb_side, 1.1]);
        
        translate([0,0,-0.1]) {
            translate([t1, t1, 0])
            cylinder(h=1.2, d = i2c_navkey_mounting_hole_dia);
            
            translate([t1, t2, 0])
            cylinder(h=1.2, d = i2c_navkey_mounting_hole_dia);
            
            translate([t2, t2, 0])
            cylinder(h=1.2, d = i2c_navkey_mounting_hole_dia);
            
            translate([t2, t1, 0])
            cylinder(h=1.2, d = i2c_navkey_mounting_hole_dia);
        }
    }
    
    // Jog dial
    translate([i2c_navkey_pcb_side/2,i2c_navkey_pcb_side/2,2.5]) {
        rotate_extrude(angle=360)
        polygon([
        [i2c_navkey_dia_1/2,0],
        [i2c_navkey_dia_1/2, 0.5],
        [i2c_navkey_dia_2/2,0.5],
        [i2c_navkey_dia_2/2,i2c_navkey_joypad_height],
        [i2c_navkey_dia_3/2,i2c_navkey_joypad_height-1]
        ]);
        
        cylinder(h=i2c_navkey_joypad_height-1, d=i2c_navkey_dia_3-0.5);
        cylinder(h=i2c_navkey_joypad_height-0.5, d=i2c_navkey_dia_4);

    }
}
}

module i2c_navkey_standoff() {
    
    translate([-i2c_navkey_pcb_side/2, -i2c_navkey_pcb_side/2, 0])
    rotate([180,0,0])
    difference() {
        union() {
            cylinder(h=i2c_navkey_standoff_outer_height, d=i2c_navkey_standoff_outer_dia);
            translate([0,0,i2c_navkey_standoff_outer_height])
            cylinder(h=i2c_navkey_standoff_inner_height, d=i2c_navkey_standoff_inner_dia);
        }
        
        cylinder(h=(i2c_navkey_standoff_inner_height+i2c_navkey_standoff_outer_height+0.2), d=i2c_navkey_standoff_hole_dia);
    }
}


module i2c_navkey_standoffs() {
    t1 = (i2c_navkey_pcb_side - i2c_navkey_mounting_hole_centres)/2;
    t2 = i2c_navkey_pcb_side - t1;

    translate([0,0,-0.1]) {
        translate([t1, t1, 0])
        i2c_navkey_standoff();
        
        translate([t1, t2, 0])
        i2c_navkey_standoff();
        
        translate([t2, t2, 0])
        i2c_navkey_standoff();
        
        translate([t2, t1, 0])
        i2c_navkey_standoff();
    }
}  

module roundedSquare(length, width, radius) {
    
    //translate([radius - (side_length / 2), radius - (side_length / 2), 0])
    translate([radius, radius, 0])
    hull() {
        circle(r=radius);
        
        translate([length - 2 * radius,0,0])
        circle(r=radius);
        
        translate([length - 2 * radius, width - 2 * radius,0])
        circle(r=radius);
        
        translate([0, width - 2 * radius,0])
        circle(r=radius);
    }
}

module button(h, w, r) {
    linear_extrude(height = h) roundedSquare(h, w, r);
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
        
         linear_extrude(height = 2.5) roundedSquare(box_width, box_height, 2);

        //cube([box_width, box_height, 2.5]);
        translate([trellis_button_grid/1.5, trellis_button_grid * 5, -0.1])
        trellis();
        
        // navkey hole
        i2c_navkey_locate()
        cylinder(h=3,d=i2c_navkey_dia_2);
        
        // TFT hole
        translate([box_margin,box_margin,-0.1])
        cube([tft_w, tft_h, 3]);
        
        
        // encoder holes
        translate([-encoder_side, i2c_navkey_pcb_side/2 + encoder_side/2, -0.1])
        i2c_navkey_locate()
        cylinder(h=3, d=encoder_hole_dia);

        translate([encoder_side, i2c_navkey_pcb_side/2 + encoder_side/2, -0.1])
        i2c_navkey_locate()
        cylinder(h=3, d=encoder_hole_dia);

    }
        
            
    i2c_navkey_locate() {
      //  translate([0,0,-i2c_navkey_joypad_height])
       // i2c_navkey();
        i2c_navkey_standoffs();
    }
}

option2();
