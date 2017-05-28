#include "draw.h"
#include <GL/glui.h>

#define ARRAY_TO_GRID 4.0

using namespace std;

void drawBuildings(int grid_side_length, vector<BuildingObject> &buildings, int fill);
void drawBuildingQuads();
void drawBuildingQuadsWireframe();

void drawB(int grid_side_length, BuildingObject building, int fill);
void drawC(int grid_side_length, BuildingObject building, int fill);
void drawL(int grid_side_length, BuildingObject building, int fill);
void drawBuildingL();
void drawBuildingLWireframe();
void drawPyramid();
void drawPyramidWireframe();

void draw(int grid_side_length, vector<BuildingObject> &buildings, vector<StreetObject> &streets, int fill, int inception) {
	if (inception) {
		glPushMatrix();
		glTranslatef(0, grid_side_length / (2.0 * ARRAY_TO_GRID), -grid_side_length / (2.0 * ARRAY_TO_GRID));
		glRotatef(90, 1, 0, 0);
		drawGrid(grid_side_length);
		drawStreets(grid_side_length, streets);
		drawBuildings(grid_side_length, buildings, fill);
		glPopMatrix();
	}
	drawGrid(grid_side_length);
	drawStreets(grid_side_length, streets);
	drawBuildings(grid_side_length, buildings, fill);
}

void drawGrid(int grid_side_length) {
	float half_gsl = grid_side_length / (2.0 * ARRAY_TO_GRID);

	/* Grid Cells */
	for (float i = -half_gsl; i <= half_gsl; i+=(1.0/ARRAY_TO_GRID)) {
		glBegin(GL_LINES);
		glColor3f(1, 1, 1);

		/* Lines along z axis, x varying */
		glVertex3f(i, 0.01, -half_gsl);
		glVertex3f(i, 0.01, half_gsl);

		/* Lines along x axis, z varying */
		glVertex3f(-half_gsl, 0.01, i);
		glVertex3f(half_gsl, 0.01, i);

		glEnd();
	}

	/* Grid base */
	glBegin(GL_QUADS);
		glColor3f(0.271, 0.271, 0.271);
		glVertex3f(-half_gsl, 0, half_gsl);
		glVertex3f(half_gsl, 0, half_gsl);
		glVertex3f(half_gsl, 0, -half_gsl);
		glVertex3f(-half_gsl, 0, -half_gsl);
	glEnd();
}

void drawStreets(int grid_side_length, vector<StreetObject> &streets) {
	float half_gsl = grid_side_length / (2.0 * ARRAY_TO_GRID);

	for (size_t i = 0; i < streets.size(); i++) {
		StreetObject street = streets.at(i);

		float lx1 = (street.x1 / (ARRAY_TO_GRID) - half_gsl);
		float ly1 = street.y1 / (ARRAY_TO_GRID) - half_gsl;
		float lx2 = street.x2 / (ARRAY_TO_GRID) - half_gsl;
		float ly2 = street.y2 / (ARRAY_TO_GRID) - half_gsl;

		float rx1;
		float ry1;
		float rx2;
		float ry2;

		float width = street.width;
		float width_diff = ((1.0 - width) * (1 / ARRAY_TO_GRID))/ 2.0;

		/* Vertical Street */
		if (lx1 == lx2) {

			rx1 = (street.x1 + 1) / (ARRAY_TO_GRID) - half_gsl;
			ry1 = (street.y1) / (ARRAY_TO_GRID) - half_gsl;
			rx2 = (street.x2 + 1) / (ARRAY_TO_GRID) - half_gsl;
			ry2 = (street.y2) / (ARRAY_TO_GRID) - half_gsl;

			/* Adjusting for width of streets */
			lx1 += width_diff;
			lx2 += width_diff;
			rx1 -= width_diff;
			rx2 -= width_diff;

		} else { /* Horizontal Street*/
			rx1 = (street.x1) / (ARRAY_TO_GRID) - half_gsl;
			ry1 = (street.y1 + 1) / (ARRAY_TO_GRID) - half_gsl;
			rx2 = (street.x2) / (ARRAY_TO_GRID) - half_gsl;
			ry2 = (street.y2 + 1) / (ARRAY_TO_GRID) - half_gsl;

			/* Adjusting for width of streets */
			ly1 += width_diff;
			ly2 += width_diff;
			ry1 -= width_diff;
			ry2 -= width_diff;
		}

		glBegin(GL_QUADS);
			glColor3f(0.639, 0.639, 0.639);
			glVertex3f(rx2, 0.01, ry2);
			glVertex3f(lx2, 0.01, ly2);
			glVertex3f(lx1, 0.01, ly1);
			glVertex3f(rx1, 0.01, ry1);
		glEnd();

	}

}

void drawBuildings(int grid_side_length, vector<BuildingObject> &buildings, int fill) {
	for (size_t i = 0; i < buildings.size(); i++) {
		BuildingObject building = buildings.at(i);
		vector<Rule> bp = building.blueprint;
		if (bp[0].type == 'N') {
			continue;
		}

		else if (bp[0].type == 'B') {
			drawB(grid_side_length, building, fill);
		}

		else if (bp[0].type == 'L') {
			drawL(grid_side_length, building, fill);
		}

		else if (bp[0].type == 'C'){
			drawC(grid_side_length, building, fill);
		} 

		else {
			cout << "this shouldn't happen no!" << endl;
		}
	}
}

void drawB(int grid_side_length, BuildingObject building, int fill) {
	float half_gsl = grid_side_length / (2.0 * ARRAY_TO_GRID);
	int x1, x2, y1, y2;
	float x_center, y_center, x_offset, y_offset, x_scale, y_scale, height;

	x1 = building.x1;
	x2 = building.x2;
	y1 = building.y1;
	y2 = building.y2;
	height = building.height;

	float curr_height = 0.01;

	x_center = (x1 + x2) / 2.0;
	y_center = (y1 + y2) / 2.0;
	x_offset = x_center * (1.0 / ARRAY_TO_GRID) - half_gsl;
	y_offset = y_center * (1.0 / ARRAY_TO_GRID) - half_gsl;
	x_scale = (x2 - x1) * (1.0 / (ARRAY_TO_GRID * 2.0));
	y_scale = (y2 - y1) * (1.0 / (ARRAY_TO_GRID * 2.0));

	for (size_t i = 0; i < building.blueprint.size(); i++) {
		float scale_w = building.blueprint[i].scale_w;
		x_scale *= scale_w;
		y_scale *= scale_w;
		
		glColor3f((float) x_center / grid_side_length, (float) y_center / grid_side_length, 0.3);
		glPushMatrix();
		glTranslatef(x_offset, curr_height, y_offset);
		glScalef(x_scale, building.blueprint[i].scale_h * height, y_scale);
		if (building.blueprint[i].type == 'B') {
			if (fill) {
				drawBuildingQuads();
			}
			drawBuildingQuadsWireframe();
		} else if (building.blueprint[i].type == 'P') {

			if (fill) {
				drawPyramid();
			}
			drawPyramidWireframe();
		}

		curr_height += building.blueprint[i].scale_h * height;
		glPopMatrix();
	}
}

void drawPyramid() {

	glBegin(GL_TRIANGLES);
		glVertex3f(1.0, 0.01, 1.0);
		glVertex3f(1.0, 0.01, -1.0);
		glVertex3f(0.01, 1.0, 0.01);

		glVertex3f(1.0, 0.01, -1.0);
		glVertex3f(-1.0, 0.01, -1.0);
		glVertex3f(0.01, 1.0, 0.01);

		glVertex3f(-1.0, 0.01, -1.0);
		glVertex3f(-1.0, 0.01, 1.0);
		glVertex3f(0.01, 1.0, 0.01);

		glVertex3f(-1.0, 0.01, 1.0);
		glVertex3f(1.0, 0.01, 1.0);
		glVertex3f(0.01, 1.0, 0.01);

	glEnd();
}

void drawPyramidWireframe() {

	glDisable(GL_POLYGON_OFFSET_FILL);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(0.0, 0.0, 0.0);

	drawPyramid();
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}

void drawC(int grid_side_length, BuildingObject building, int fill) {
	float half_gsl = grid_side_length / (2.0 * ARRAY_TO_GRID);
	int x1, x2, y1, y2;
	float x_center, y_center, x_offset, y_offset, x_scale, y_scale, height;

	float curr_height = 0.01;

	x1 = building.x1;
	x2 = building.x2;
	y1 = building.y1;
	y2 = building.y2;
	height = building.height;

	x_center = (x1 + x2) / 2.0;
	y_center = (y1 + y2) / 2.0;
	x_offset = x_center * (1.0 / ARRAY_TO_GRID) - half_gsl;
	y_offset = y_center * (1.0 / ARRAY_TO_GRID) - half_gsl;
	x_scale = (x2 - x1) * (1.0 / (ARRAY_TO_GRID * 2.0));
	y_scale = (y2 - y1) * (1.0 / (ARRAY_TO_GRID * 2.0));

	for (size_t i = 0; i < building.blueprint.size(); i++) {
		float scalar = building.blueprint[i].scale_w;
		x_scale *= scalar;
		y_scale *= scalar;

		glColor3f((float) x_center / grid_side_length, (float) y_center / grid_side_length, 0.3);
		glPushMatrix();
			glTranslatef(x_offset, curr_height, y_offset);
			glScalef(x_scale, building.blueprint[i].scale_h * height, y_scale);
			if (fill) {
				drawBuildingQuads();
			}
			drawBuildingQuadsWireframe();
		glPopMatrix();

		curr_height += building.blueprint[i].scale_h * height;
	}
}

void drawBuildingQuads() {

	glBegin(GL_QUADS);
	/* Base */
	glVertex3f(-1.0, 0.01, 1.0);
	glVertex3f(1.0, 0.01, 1.0);
	glVertex3f(1.0, 0.01, -1.0);
	glVertex3f(-1.0, 0.01, -1.0);

	/* Top */
	glVertex3f(-1.0, 1.0, 1.0);
	glVertex3f(1.0, 1.0, 1.0);
	glVertex3f(1.0, 1.0, -1.0);
	glVertex3f(-1.0, 1.0, -1.0);

	/* Body 1 */
	glVertex3f(-1.0, 0.01, -1.0);
	glVertex3f(-1.0, 0.01, 1.0);
	glVertex3f(-1.0, 1.0, 1.0);
	glVertex3f(-1.0, 1.0, -1.0);

	/* Body 2 */
	glVertex3f(-1.0, 0.01, 1.0);
	glVertex3f(1.0, 0.01, 1.0);
	glVertex3f(1.0, 1.0, 1.0);
	glVertex3f(-1.0, 1.0, 1.0);

	/* Body 3 */
	glVertex3f(1.0, 0.01, 1.0);
	glVertex3f(1.0, 0.01, -1.0);
	glVertex3f(1.0, 1.0, -1.0);
	glVertex3f(1.0, 1.0, 1.0);

	/* Body 4 */
	glVertex3f(1.0, 0.01, -1.0);
	glVertex3f(-1.0, 0.01, -1.0);
	glVertex3f(-1.0, 1.0, -1.0);
	glVertex3f(1.0, 1.0, -1.0);
	glEnd();
}

void drawBuildingQuadsWireframe() {

	glDisable(GL_POLYGON_OFFSET_FILL);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(0.0, 0.0, 0.0);

	drawBuildingQuads();
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void drawL(int grid_side_length, BuildingObject building, int fill) {
	float half_gsl = grid_side_length / (2.0 * ARRAY_TO_GRID);
	int x1, x2, y1, y2;
	float x_center, y_center, x_offset, y_offset, x_scale, y_scale, height;
	float scalar = building.blueprint[0].scale_w;

	x1 = building.x1;
	x2 = building.x2;
	y1 = building.y1;
	y2 = building.y2;
	height = building.height;

	x_center = (x1 + x2) / 2.0;
	y_center = (y1 + y2) / 2.0;
	x_offset = x_center * (1.0 / ARRAY_TO_GRID) - half_gsl;
	y_offset = y_center * (1.0 / ARRAY_TO_GRID) - half_gsl;
	x_scale = (x2 - x1) * (1.0 / (ARRAY_TO_GRID * 2.0));
	y_scale = (y2 - y1) * (1.0 / (ARRAY_TO_GRID * 2.0));

	x_scale *= scalar;
	y_scale *= scalar;

	glColor3f((float) x_center / grid_side_length, (float) y_center / grid_side_length, 0.3);
	glPushMatrix();
		glTranslatef(x_offset, 0, y_offset);
		glScalef(x_scale, height, y_scale);
		glRotatef(building.blueprint[0].rotation, 0, 1, 0);
			if (fill) {
				drawBuildingL();
			}
			drawBuildingLWireframe();
	glPopMatrix();
}

void drawBuildingL() {

	glBegin(GL_QUADS);

	/* Building A */
	
	/* Base */
	glVertex3f(-1.0, 0.01, 1.0);
	glVertex3f(1.0, 0.01, 1.0);
	glVertex3f(1.0, 0.01, 0.01);
	glVertex3f(-1.0, 0.01, 0.01);

	/* Top */
	glVertex3f(-1.0, 1.0, 1.0);
	glVertex3f(1.0, 1.0, 1.0);
	glVertex3f(1.0, 1.0, 0.01);
	glVertex3f(-1.0, 1.0, 0.01);

	/* Body 1 */
	glVertex3f(-1.0, 0.01, 1.0);
	glVertex3f(1.0, 0.01, 1.0);
	glVertex3f(1.0, 1.0, 1.0);
	glVertex3f(-1.0, 1.0, 1.0);

	/* Body 2 */
	glVertex3f(1.0, 0.01, 1.0);
	glVertex3f(1.0, 0.01, 0.01);
	glVertex3f(1.0, 1.0, 0.01);
	glVertex3f(1.0, 1.0, 1.0);

	/* Body 3 */
	glVertex3f(1.0, 0.01, 0.01);
	glVertex3f(-1.0, 0.01, 0.01);
	glVertex3f(-1.0, 1.0, 0.01);
	glVertex3f(1.0, 1.0, 0.01);

	/* Body 4 */
	glVertex3f(-1.0, 0.01, 0.01);
	glVertex3f(-1.0, 0.01, 1.0);
	glVertex3f(-1.0, 1.0, 1.0);
	glVertex3f(-1.0, 1.0, 0.01);

	/* Building B */
	
	/* Base */
	glVertex3f(-1.0, 0.01, 0.01);
	glVertex3f(0.01, 0.01, 0.01);
	glVertex3f(0.01, 0.01, -1.0);
	glVertex3f(-1.0, 0.01, -1.0);

	/* Top */
	glVertex3f(-1.0, 1.0, 0.01);
	glVertex3f(0.01, 1.0, 0.01);
	glVertex3f(0.01, 1.0, -1.0);
	glVertex3f(-1.0, 1.0, -1.0);

	/* Body 1 */
	glVertex3f(0.01, 0.01, 0.01);
	glVertex3f(0.0, 0.01, -1.0);
	glVertex3f(0.01, 1.0, -1.0);
	glVertex3f(0.01, 1.0, 0.01);

	/* Body 2 */
	glVertex3f(0.01, 0.01, -1.0);
	glVertex3f(-1.0, 0.01, -1.0);
	glVertex3f(-1.0, 1.0, -1.0);
	glVertex3f(0.01, 1.0, -1.0);

	/* Body 3 */
	glVertex3f(-1.0, 0.01, -1.0);
	glVertex3f(-1.0, 0.01, 0.01);
	glVertex3f(-1.0, 1.0, 0.01);
	glVertex3f(-1.0, 1.0, -1.0);
	glEnd();
}

void drawBuildingLWireframe() {

	glColor3f(0.0, 0.0, 0.0);

	/* Base */
	glBegin(GL_LINE_STRIP);
	glVertex3f(-1.0, 0.01, 1.0);
	glVertex3f(1.0, 0.01, 1.0);
	glVertex3f(1.0, 0.01, 0.01);
	glVertex3f(0.01, 0.01, 0.01);
	glVertex3f(0.01, 0.01, -1.0);
	glVertex3f(-1.0, 0.01, -1.0);
	glVertex3f(-1.0, 0.01, 1.0);
	glEnd();

	/* Top */ 
	glBegin(GL_LINE_STRIP);
	glVertex3f(-1.0, 1.0, 1.0);
	glVertex3f(1.0, 1.0, 1.0);
	glVertex3f(1.0, 1.0, 0.01);
	glVertex3f(0.01, 1.0, 0.01);
	glVertex3f(0.01, 1.0, -1.0);
	glVertex3f(-1.0, 1.0, -1.0);
	glVertex3f(-1.0, 1.0, 1.0);
	glEnd();

	glBegin(GL_LINES);

	glVertex3f(-1.0, 0.01, 1.0);
	glVertex3f(-1.0, 1.0, 1.0);

	glVertex3f(1.0, 0.01, 1.0);
	glVertex3f(1.0, 1.0, 1.0);

	glVertex3f(1.0, 0.01, 0.01);
	glVertex3f(1.0, 1.0, 0.01);

	glVertex3f(0.01, 0.01, 0.01);
	glVertex3f(0.01, 1.0, 0.01);

	glVertex3f(0.01, 0.01, -1.0);
	glVertex3f(0.01, 1.0, -1.0);

	glVertex3f(-1.0, 0.01, -1.0);
	glVertex3f(-1.0, 1.0, -1.0);

	glEnd();
}
