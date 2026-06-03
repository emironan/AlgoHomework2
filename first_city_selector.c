#include "first_city_selector.h"

int select_first_city(City* cities, int cities_count, int* open_time)
{
    int smallest_open_time = cities[0].open;
    int selected_id = cities[0].id; 

    for(int i = 1; i < cities_count; i++)
    {
        if(cities[i].open < smallest_open_time)
        {
            smallest_open_time = cities[i].open;
            selected_id = cities[i].id; 
        }
    }

    if(selected_id != -1)
    {
        cities[selected_id].visited = 1;
        *open_time = cities[selected_id].open;
    }

    return selected_id;
}

