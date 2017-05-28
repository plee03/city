#include <cstdlib>
#include <cstdio>
#include <iostream>
#include "BuildingObject.h"
#include <random>

using namespace std;

BuildingObject::BuildingObject() {

	x1 = 0;
	y1 = 0;
	x2 = 0;
	y2 = 0;
	height = 0.0;

	Rule r;
	r.type = 'B';
	r.scale_h = 1;
	r.scale_w = 1;
	blueprint.push_back(r);
}

void BuildingObject::set_xy_width() {
	x_width = x2 - x1;
	y_width = y2 - y1;
}

void BuildingObject::generateBlueprint() {

	/* START: No transformations have been applied yet */
	if (blueprint[0].type == 'B') {

		/* If building dimensions are too small, forced to be not a building */
		if (x_width < 2 || y_width < 2) {
			blueprint[0].type = 'N';
		}

		random_device rd; 
		int bp_case = rd() % 100;
		/* Case 1 - not a building */
		if (bp_case > 0 && bp_case <= 5) {
			blueprint[0].type = 'N';
		} 
		/* Case 2 - L building */
		else if (bp_case > 10 && bp_case <= 30) {
			blueprint[0].type = 'L';
			blueprint[0].rotation = 90 * (rd() % 3);
			float scalar = (rd()% (500) + 500)/1000.0;
			blueprint[0].scale_w = scalar; 
		}

		/* Case 3 - Chunks */
		else if (bp_case > 30 && bp_case <= 35) {
			/* Set conditions for first chunk */
			blueprint[0].type = 'C';
			blueprint[0].scale_h = 2.0;
			blueprint[0].scale_w = ((rd() % 10) + 1.0) / 15.0;
			float total_scale_w = 1.0;
			float num_chunks = (rd() % (int) ((height * 10) + 1)) + 2; // range from 2 to height of building
			for (int i = 1; i < num_chunks; i++) {

				Rule r;
				r.type = 'C';
				r.scale_w = ((rd() % 10) + 1.0) / 15.0;
				total_scale_w *= r.scale_w; 
				r.scale_h = ((rd() % 10) + 1.0) / 15.0;
				if (total_scale_w < 0.05) {
					break;
				} 
				blueprint.push_back(r);
			}
		}
		/* Otherwise, stay a basic building */
		else {

			bool p_case = rd() % 2;

			float scalar = (rd()% (500) + 500)/1000.0;
			blueprint[0].scale_w = scalar; 

			/* Caps for basic buildings - Pyramid */
			if (p_case && height < 0.5 && height > 0.3) {
				Rule r;
				r.type = 'P';
				r.scale_w = 1;
				r.scale_h = ((rd() % 200) + 200) / 1000.0;
				
				blueprint.push_back(r);
			} 

		}
	}
}
