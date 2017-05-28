#ifndef GRID__H
#define GRID__H

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <cmath>
#include <iomanip>
#include "CityObject.h"
#include "BuildingObject.h"
#include "StreetObject.h"

using namespace std;

void generateGrid(int grid_side_length, vector<CityObject> &grid, vector<BuildingObject> &buildings, vector<StreetObject> &streets);
void printGrid(int grid_side_length, vector<CityObject> grid);

#endif