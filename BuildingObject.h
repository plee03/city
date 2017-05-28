#ifndef BUILDING_OBJECT__H
#define BUILDING_OBJECT__H

#include <vector>

using namespace std;

struct Rule {
	char type;
	float scale_w;
	float scale_h;
	int rotation;
};

class BuildingObject {
	
	public:
		BuildingObject();
		int  x1, x2, y1, y2;
		float height;
		vector<Rule> blueprint;

		void generateBlueprint();
		void set_xy_width();

	private:
		int x_width;
		int y_width;

};

#endif