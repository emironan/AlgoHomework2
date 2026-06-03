#include "route_selector.h"
#include <math.h>

#include <stdio.h>


int distance_calculator(City city_1, City city_2);


int next_city_selector_least_score(City* cities, int current_city_id, int city_count, int* current_time, int* length)
{
    int selected_id = -1;
    int time_calculated;
    double score = 1000000;
    double score_calculated;
    double wait_score;
    double remaining_time_score;

    for(int i = 0 ; i < city_count; i++)
    {
        if((cities[i].id != current_city_id) && (cities[i].visited == 0))
        {
            time_calculated = distance_calculator(cities[current_city_id], cities[i]);
            if((*current_time + time_calculated) < cities[i].close)
            {
                if (*current_time + time_calculated > cities[current_city_id].open)
                {
                    wait_score = 0;
                }
                else{
                    wait_score = cities[current_city_id].open - (*current_time + time_calculated);
                }

                remaining_time_score = cities[current_city_id].close - (*current_time + time_calculated);

                score_calculated = time_calculated + (0.2 * wait_score) + (0.5 * remaining_time_score);

                if(score_calculated < score)
                {
                    score = score_calculated;
                    selected_id = cities[i].id;
                }
            }
            else{
                // printf("Time Calc= %d\n", *current_time + time_calculated);
                // printf("Close Time = %d\n", cities[i].close);
                
            }
        }
    }

    if(selected_id != -1)
    {
        cities[selected_id].visited = 1;
        if(cities[selected_id].open > *current_time + time_calculated)
        {
            *current_time = cities[selected_id].open;
        }
        else
        {
            time_calculated = distance_calculator(cities[current_city_id], cities[selected_id]);
            *length = time_calculated;
            *current_time = *current_time + time_calculated;
        }
    }

    return selected_id;
}


int next_city_selector_first_closing(City* cities, int current_city_id, int city_count, int* current_time, int* length)
{
    int close_time_selected = 1000000;
    int selected_id = -1;
    int time_calculated;

    for(int i = 0 ; i < city_count; i++)
    {
        if((cities[i].id != current_city_id) && (cities[i].visited == 0))
        {
            if(cities[i].close < close_time_selected)
            {
                time_calculated = distance_calculator(cities[current_city_id], cities[i]);
                if((*current_time + time_calculated) < cities[i].close)
                {
                    selected_id = cities[i].id;
                    close_time_selected = cities[i].close;
                }
                else{
                    // printf("Time Calc= %d\n", *current_time + time_calculated);
                    // printf("Close Time = %d\n", cities[i].close);
                    
                }
            }
        }
    }
    

    if(selected_id != -1)
    {
        cities[selected_id].visited = 1;
        if(cities[selected_id].open > *current_time + time_calculated)
        {
            *current_time = cities[selected_id].open;
        }
        else
        {
            time_calculated = distance_calculator(cities[current_city_id], cities[selected_id]);
            *length = time_calculated;
            *current_time = *current_time + time_calculated;
        }
    }

    return selected_id;
}

int distance_calculator(City city_1, City city_2)
{
    double x = (double) city_1.x - (double)city_2.x;
    double y = (double) city_1.y - (double)city_2.y;

    x = x * x;
    y = y * y;

    double root_val = sqrt(x + y);
    
    return (int)floor(root_val + 0.5);
} 
