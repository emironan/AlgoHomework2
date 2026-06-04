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

void get_best_20_start_candidates(City* cities, int cities_count, int* candidates, int candidate_count)
{
    map_specs_t* map_specs = get_map_specs(cities, cities_count);

    long int map_area =
        (long int)(map_specs->max_x - map_specs->min_x) *
        (long int)(map_specs->max_y - map_specs->min_y);

    int r_value = ((int)sqrt(((double)map_area / (double)cities_count))) * 2;

    double* best_scores = malloc(sizeof(double) * candidate_count);

    for(int i = 0; i < candidate_count; i++)
    {
        candidates[i] = -1;
        best_scores[i] = -1000000000.0;
    }

    for(int i = 0; i < cities_count; i++)
    {
        int neighbours_count = 0;

        for(int j = 0; j < cities_count; j++)
        {
            if(i != j && distance_calculator(cities[i], cities[j]) < r_value)
            {
                neighbours_count++;
            }
        }

        double density_score = ((double)neighbours_count / 500.0) * 100.0;
        double time_penalty = ((double)cities[i].open / (double)map_specs->max_close_time) * 100.0;

        double score_calculated = (density_score * 0.8) - (time_penalty * 0.2);

        for(int k = 0; k < candidate_count; k++)
        {
            if(score_calculated > best_scores[k])
            {
                for(int m = candidate_count - 1; m > k; m--)
                {
                    best_scores[m] = best_scores[m - 1];
                    candidates[m] = candidates[m - 1];
                }

                best_scores[k] = score_calculated;
                candidates[k] = cities[i].id;
                break;
            }
        }
    }

    free(best_scores);
    free(map_specs);
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