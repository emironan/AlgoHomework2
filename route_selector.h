#ifndef ROUTE_SELECTOR_H
#define ROUTE_SELECTOR_H

#include "cities.h"

int next_city_selector_least_score(City* cities, int current_city_id, int city_count, int* current_time, int* length);

int next_city_selector_first_closing(City* cities, int current_city_id, int city_count, int* current_time, int* length);

// YENİ EKLENEN FONKSİYONUN TANITIMI:
int next_city_selector_grasp(City* cities, int current_city_id, int city_count, int* current_time, int* length);

int distance_calculator(City city_1, City city_2);

#endif