#ifndef FIRST_CITY_SELECTOR_H
#define FIRST_CITY_SELECTOR_H

#include "cities.h"

// Returns selected city id
int select_first_city(City* cities, int cities_count, int* open_time);

int select_first_city_algo2(City* cities, int cities_count, int* open_time);

/* returns best 20 start candidates according to algo2 */
void get_best_20_start_candidates_algo2(City* cities, int cities_count, int* candidates, int candidate_count);






#endif