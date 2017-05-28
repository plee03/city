#include "grid.h"

#define NUM_X_PARTITION 2 // number of building partitions along x
#define NUM_Y_PARTITION 2 // number of building partitions along y
#define MIN_SUBGRID_LENGTH 5

using namespace std;

/* Private functions */
void generateStreets(int grid_side_length, vector<CityObject> &grid, vector<BuildingObject> &buildings, vector<StreetObject> &streets);
void generateStreets(int grid_side_length, vector<CityObject> &grid, vector<BuildingObject> &buildings, vector<StreetObject> &streets, int x_min, int x_max, int y_min, int y_max, float width);
void buildingPartition(vector<BuildingObject> &buildings, int x_min, int x_max, int y_min, int y_max);
void yPartitions(vector<BuildingObject> &buildings, int xp_min, int xp_max, int y_min, int y_max);
void mark_buildings(vector<CityObject> &grid, vector<BuildingObject> &buildings);

void generateGrid(int grid_side_length, vector<CityObject> &grid, vector<BuildingObject> &buildings, vector<StreetObject> &streets) {
	CityObject cityObjFalse;
	cityObjFalse.isStreet = false;
	grid.assign(grid_side_length * grid_side_length, cityObjFalse);
	generateStreets(grid_side_length, grid, buildings, streets);
	mark_buildings(grid, buildings);
}

void generateStreets(int grid_side_length, vector<CityObject> &grid, vector<BuildingObject> &buildings, vector<StreetObject> &streets) {
	generateStreets(grid_side_length, grid, buildings, streets, 0, grid_side_length, 0, grid_side_length, 1.0);
}

void generateStreets(int grid_side_length, vector<CityObject> &grid, vector<BuildingObject> &buildings, vector<StreetObject> &streets, int x_min, int x_max, int y_min, int y_max, float width) {

	int x_range = x_max - x_min;
	int y_range = y_max - y_min;

	// Base Case: when recursive call on sub-grid is too small
	if (x_range <= sqrt(grid_side_length) || y_range <= sqrt(grid_side_length) ||  
		x_range <= MIN_SUBGRID_LENGTH || y_range <= MIN_SUBGRID_LENGTH) {
			buildingPartition(buildings, x_min, x_max, y_min, y_max);
			return;
	}

	random_device rd; 
	/* Streets are at least 2 cells apart */
	int x_coord = rd() % (x_range - 4) + x_min + 2;
	int y_coord = rd() % (y_range - 4) + y_min + 2;

	CityObject cityObjTrue;
	cityObjTrue.isStreet = true;
	cityObjTrue.width = width;

	/* Street1 = Vertical */
	StreetObject street1;
	street1.width = width;
	street1.x1 = x_coord;
	street1.x2 = x_coord;
	street1.y1 = y_min;
	street1.y2 = y_max;
	streets.push_back(street1);

	/* Street2 = Horizontal */
	StreetObject street2;
	street2.width = width;
	street2.x1 = x_coord;
	street2.x2 = x_coord;
	street2.y1 = y_min;
	street2.y2 = y_max;
	streets.push_back(street2);


	// setting horizontal street
	for (int x = x_min; x < x_max; x++) {
		grid.at(grid_side_length * y_coord + x) = cityObjTrue;

	}

	// setting vertical street
	for (int y = y_min; y < y_max; y++) {
		grid.at(grid_side_length * y + x_coord) = cityObjTrue;

	}

	// Recursive call on sub-grids
	generateStreets(grid_side_length, grid, buildings, streets, x_min, x_coord, y_min, y_coord, 0.8 * width); // upper left
	generateStreets(grid_side_length, grid, buildings, streets, x_coord + 1, x_max, y_coord + 1, y_max, 0.8 * width); // lower right
	generateStreets(grid_side_length, grid, buildings, streets, x_min, x_coord, y_coord + 1, y_max, 0.8 * width); // lower left
	generateStreets(grid_side_length, grid, buildings, streets, x_coord + 1, x_max, y_min, y_coord, 0.8 * width); // upper right

}

void buildingPartition(vector<BuildingObject> &buildings, int x_min, int x_max, int y_min, int y_max) {
	int x_range = x_max - x_min;
	int num_partitions = x_range / NUM_X_PARTITION;

	if (num_partitions < 2) {
		yPartitions(buildings, x_min, x_max, y_min, y_max);
		return;
	}

	/* Evenly dividing subgrid vertically (x) */
	for (int i = 0; i < num_partitions; i++) {
		int xp_min = i * (x_range/num_partitions) + x_min;
		int xp_max = (i + 1) * (x_range/num_partitions) + x_min;

		/* Corner case: last building in subgrid */
		if (i == num_partitions - 1) {
			xp_min = i * (x_range/num_partitions) + x_min;
			xp_max = x_max;
		}
		yPartitions(buildings, xp_min, xp_max, y_min, y_max);
	}
}

void yPartitions(vector<BuildingObject> &buildings, int xp_min, int xp_max, int y_min, int y_max) {
	random_device rd; 

	int y_range = y_max - y_min;
	int num_partitions = y_range / NUM_Y_PARTITION;

	/* Corner case: y axis grid is less than NUM_Y_PARTITIONS */
	if (num_partitions < 2) {
		num_partitions = 1;
	}

	int par_min = y_min; /* Setting for first building in subgrid */

	/* Evenly dividing subgrid horizontally (y) */
	for (int i = 0; i < num_partitions; i++) {
		BuildingObject bo;

		bo.height = ((rd() % 10) + 1.0) / 11.0;

		/* Corner case: last building in subgrid */
		if (i == num_partitions - 1) {
			bo.x1 = xp_min;
			bo.x2 = xp_max;
			bo.y1 = par_min;
			bo.y2 = y_max;
			bo.set_xy_width();
			bo.generateBlueprint();
			buildings.push_back(bo);
			continue;
		}

		int partYmin = i * (y_range/num_partitions) + y_min;
		int partYmax = (i + 1) * (y_range/num_partitions) + y_min;
		int par_max = rd() % (partYmax - partYmin) + partYmin + 1; 

		bo.x1 = xp_min;
		bo.x2 = xp_max;
		bo.y1 = par_min;
		bo.y2 = par_max;

		par_min = par_max; /* Setting next building's min value */
		
		bo.set_xy_width();
		bo.generateBlueprint();
		buildings.push_back(bo);
	}
}

void mark_buildings(vector<CityObject> &grid, vector<BuildingObject> &buildings) {
	int grid_side_length = sqrt(grid.size());

	cout << "# Buildings: " << buildings.size() << endl;
	for(size_t i = 0; i < buildings.size(); i++) {
		BuildingObject bo = buildings.at(i);
		for (int x = bo.x1; x < bo.x2; x++) {
			for (int y = bo.y1; y < bo.y2; y++) {
				int loc = y * grid_side_length + x;
				CityObject co = grid.at(loc);
				co.buildingIndex = i;
				grid.at(loc) = co;
			}
		}
	}
}

void printGrid(int grid_side_length, vector<CityObject> grid) {

	for (unsigned i = 0; i < grid.size(); i++) {

		if (i % (grid_side_length) == 0 && (i != 0))  {
			cout << "\n";
		}
		if (grid[i].isStreet == true) {
			cout << " " << "#" << "  ";
		} else {
			if (grid[i].buildingIndex > 9) {
				cout << " " << grid[i].buildingIndex << " ";
			} else {
				cout << " " << grid[i].buildingIndex << "  ";
			}	
		}
	}
	cout << "\n";
}
