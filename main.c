#include <stdio.h>
#include <stdlib.h>
#include "cities.h"
#include "first_city_selector.h"
#include "route_selector.h"

int cities_visited_id[100];
int visited_city_count = 0;
int total_time = 0;
int total_distance = 0;

int main(void)
{
    printf("Algo Project 2\n");

    int city_count; 
    City* cities = read_cities("files/example-input-1.txt", &city_count);

    if(cities == NULL)
    {
        printf("Cities read error\n");
        return -1;
    }

    printf("City Count = %d\n", city_count);
    
    int time;

    int first_city_id = select_first_city(cities, city_count, &time);
    cities_visited_id[visited_city_count] = first_city_id; 
    int current_id = first_city_id;

    printf("Current Time = %d\t", time);
    printf("%d\t %d\t %d\t %d\t %d\n", cities[current_id].id, cities[current_id].x, cities[current_id].y, cities[current_id].open, cities[current_id].close);
    
    int distance;

    while(current_id != -1)
    {
        visited_city_count++;
        current_id = next_city_selector_first_closing(cities, current_id, city_count, &time, &distance);
        if(current_id != -1)
        {
            total_distance += distance;
            cities_visited_id[visited_city_count] = current_id;
            printf("Current Time = %d\t", time);
            printf("%d\t %d\t %d\t %d\t %d\n", cities[current_id].id, cities[current_id].x, cities[current_id].y, cities[current_id].open, cities[current_id].close);
        }
    }

    int return_distance = distance_calculator(cities[cities_visited_id[visited_city_count - 1]], cities[first_city_id]);
    total_distance += return_distance;
    total_time = time + return_distance;
    
    printf("%d %d %d\n", visited_city_count, total_distance, total_time);

    for(int i = 0; i < visited_city_count; i++)
    {
        printf("%d\n", cities_visited_id[i]);
    }

    free(cities);

    return 0;
}