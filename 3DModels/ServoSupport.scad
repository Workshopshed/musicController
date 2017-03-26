
module servoStand() {

offset = 0.25;
    
    difference() {
		union() {
			cube([18,3,50]);
			cube([18,30,4]);
			translate([0,7,8])
				rotate([-90,0,0])
					fillet(8.1,18); 
		}
		//cable slot
		translate([7,-2,40.4])
			cube([4,10,2]);
		
		//servo hole
        translate([2.5+(offset/2),10,42])
            rotate([90,90,0])
                cube([22.5+offset,12.2+offset,22.5+offset]);
		//Round top corners
		translate([4,-5,46])
				rotate([90,0,90])
					fillet(8.1,10);	
		
		translate([14,-5,46])
				rotate([180,0,90])
					fillet(8.1,10);

		//Drill holes
		translate([9,-5,18])
			rotate([-90,0,0])
				cylinder(d=1,h=10,$fn=100);
		translate([9,-5,43])
			rotate([-90,0,0])
				cylinder(d=1,h=10,$fn=100);

		//Round back corners
		translate([14,26,5])
				rotate([90,90,0])
					fillet(8.1,10);	
					
		translate([4,26,5])
				rotate([0,90,0])
					fillet(8.1,10);	
		
		//Mounting hole
		translate([9,20,0])
			countersink(3);
    }
}

module fillet(filletsize,width)
{
    difference() {
    translate([0,-filletsize/2,-filletsize/2])
        cube([width,filletsize,filletsize]);
    translate([-1,0,0])
        rotate([0,90,0])
            cylinder(width+2,d=filletsize,$fn=100);
    translate([-1,-filletsize,-filletsize/2])
        cube([width+2,filletsize,filletsize]);
    translate([-1,-filletsize/2,0])
        cube([width+2,filletsize,filletsize]);     
    }
}

module countersink(drillsize)
    union() {
        cylinder(drillsize*3,0,drillsize*3,$fn=100);
        translate([0,0,-3])
        cylinder(10,d=drillsize,$fn=100);
    }

servoStand();


