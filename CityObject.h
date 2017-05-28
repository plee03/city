#ifndef CITY_OBJECT__H
#define CITY_OBJECT__H

struct CityObject {
	bool isStreet;
	float width; /* Street width percentage, between 0 and 1 */
	int buildingIndex;
};

#endif