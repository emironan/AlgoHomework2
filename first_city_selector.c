#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "first_city_selector.h"
#include "route_selector.h"

typedef struct st_map_specs_t
{
    int max_x;
    int max_y;
    int min_x;
    int min_y;
    int max_close_time;
} map_specs_t;

static map_specs_t* get_map_specs(City* cities, int cities_count);

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

int select_first_city_algo2(City* cities, int cities_count, int* open_time)
{
    int selected_id = -1;
    map_specs_t* map_specs = get_map_specs(cities, cities_count);

    long int map_area =
        (long int)(map_specs->max_x - map_specs->min_x) *
        (long int)(map_specs->max_y - map_specs->min_y);

    int r_value = ((int)sqrt(((double)map_area / (double)cities_count))) * 2;

    double score = -1000000;
    double score_calculated = 0;
    int neigbours_count = 0;

    for(int i = 0; i < cities_count; i++)
    {
        for(int j = 0; j < cities_count; j++)
        {
            if(i != j && distance_calculator(cities[i], cities[j]) < r_value)
            {
                neigbours_count++;
            }
        }

        double density_score = ((double)neigbours_count / 500.0) * 100.0;
        double time_penalty = ((double)cities[i].open / (double)map_specs->max_close_time) * 100.0;

        score_calculated = (density_score * 0.8) - (time_penalty * 0.2);

        neigbours_count = 0;

        if(score_calculated > score)
        {
            score = score_calculated;
            selected_id = i;
        }
    }

    if(selected_id != -1)
    {
        cities[selected_id].visited = 1;
        *open_time = cities[selected_id].open;
    }

    free(map_specs);
    return selected_id;
}

static map_specs_t* get_map_specs(City* cities, int cities_count)
{
    map_specs_t* map_specs = malloc(sizeof(map_specs_t));

    map_specs->max_x = 0;
    map_specs->max_y = 0;
    map_specs->min_x = 1000000;
    map_specs->min_y = 1000000;
    map_specs->max_close_time = 0;

    for(int i = 0; i < cities_count; i++)
    {
        if(cities[i].x > map_specs->max_x)
            map_specs->max_x = cities[i].x;

        if(cities[i].y > map_specs->max_y)
            map_specs->max_y = cities[i].y;

        if(cities[i].x < map_specs->min_x)
            map_specs->min_x = cities[i].x;

        if(cities[i].y < map_specs->min_y)
            map_specs->min_y = cities[i].y;

        if(cities[i].close > map_specs->max_close_time)
            map_specs->max_close_time = cities[i].close;
    }

    return map_specs;
}