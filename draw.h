#ifndef DRAW__H
#define DRAW__H

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

void draw(int grid_side_length, vector<BuildingObject> &buildings, vector<StreetObject> &streets, int fill, int inception);
void drawGrid(int grid_side_length);
void drawStreets(int grid_side_length, vector<StreetObject> &streets);

#endif