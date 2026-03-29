// My measurements for lilygo:
// Breadboard + TTGO: 37mm wide x 59mm long x 16.95mm tall
// USB-C on short side (37mm), add space for a cutout, centered, 9.5mm wide

// Hardware dimensions
hw_x = 58;  // long side
hw_y = 36;   // short side
hw_z = 16.95;  // height of breadboard + TTGO

//Add extra clearance
side_clearance = 1;
wire_clearance = 7; // headroom above hw_z for wires

wall = 3; // wall thickness
floor_t = 3;// floor thickness
lid_t = 4; // lid plate thickness

//Inner dims
inner_x = hw_x + side_clearance * 2;
inner_y = hw_y + side_clearance * 2;
inner_z = hw_z + wire_clearance; // total inner height

// Outer dimensions
outer_x = inner_x + wall * 2; // long axis (59mm side) = 58+2+3*2=66
outer_y = inner_y + wall * 2; // short axis (37mm side, USB-C here)
outer_z = floor_t + inner_z; // total box height
//outer_z = 3 + 16.95+7 = 26.95

// USB-C cutout
usb_bottom = floor_t + 13.92 - 2;
usb_top    = floor_t + 19.3;  // 2mm margin above, =3+19.3=22.3
usb_h      = usb_top - usb_bottom;
usb_w      = 9.5 + 3;  // 1.5mm breathing room each side

//lid 
friction_gap = 0.2;
rim_x = inner_x - friction_gap;
rim_y = inner_y - friction_gap;
rim_z = 4.5;
rim_wall = 2.5;

// Identation for copper tape
panel_w = 20;
panel_h = 16;
panel_depth = 1.0;
panel_z = floor_t + 4;

// Add holes toallow wire to come through
wire_hole_d = 3;

// ---- TRAY ----
module tray() {
    difference() {
        
        //box itself
        cube([outer_x, outer_y, outer_z]);

        translate([wall, wall, floor_t])
            cube([inner_x, inner_y, outer_z]);

        translate([-0.1, outer_y/2 - usb_w/2, usb_bottom])
            cube([wall + 0.2, usb_w, usb_h]);

        //Copper tape indents

        // first long wall
        translate([outer_x/2 - panel_w/2, -0.1, panel_z])
            cube([panel_w, panel_depth + 0.1, panel_h]);

        // other long wall
        translate([outer_x/2 - panel_w/2, outer_y - panel_depth - 0.1, panel_z])
            cube([panel_w, panel_depth + 0.1, panel_h]);

        //opposite of usbc wall
        translate([outer_x - panel_depth - 0.1, outer_y/2 - panel_w/2, panel_z])
            cube([panel_depth + 0.1, panel_w, panel_h]);

        //holes in indents

        // first long wall
        translate([outer_x/2, -0.1, panel_z + panel_h/2])
            rotate([-90, 0, 0])
            cylinder(d=wire_hole_d, h=wall + 0.2, $fn=20);

        // other long wall
        translate([outer_x/2, outer_y + 0.1, panel_z + panel_h/2])
            rotate([90, 0, 0])
            cylinder(d=wire_hole_d, h=wall + 0.2, $fn=20);

        // opposite of usbc wall
        translate([outer_x + 0.1, outer_y/2, panel_z + panel_h/2])
            rotate([0, -90, 0])
            cylinder(d=wire_hole_d, h=wall + 0.2, $fn=20);
    }
}

// ---- LID ----
module lid() {
    union() {
        cube([outer_x, outer_y, lid_t]);

        // Rim for friction fit
        translate([(outer_x - rim_x)/2, (outer_y - rim_y)/2, -rim_z])
            difference() {
                cube([rim_x, rim_y, rim_z]);
                translate([rim_wall, rim_wall, -0.1])
                    cube([rim_x - rim_wall*2, rim_y - rim_wall*2, rim_z + 0.2]);
            }
    }
}


// rendering part, save stls separately when sending to 3d printer
tray();

translate([outer_x + 15, 0, rim_z])
    lid();
