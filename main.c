#include <stdio.h>
#include <stdlib.h>
#include "cities.h"
#include "first_city_selector.h"
#include "route_selector.h"

int write_output_file(const char* filename, int* route, int visited_count, int total_distance, int total_time);

int cities_visited_id[55000];
int visited_city_count = 0;
int total_time = 0;
int total_distance = 0;

int main(void)
{
    printf("Algo Project 2\n");

    int city_count; 
    // City* cities = read_cities("files/example-input-3.txt", &city_count);
    City* cities = read_cities("files/test-input-4-tsptw.txt", &city_count);

    if(cities == NULL)
    {
        printf("Cities read error\n");
        return -1;
    }

    printf("City Count = %d\n", city_count);
    
    int time;

    // int first_city_id = select_first_city(cities, city_count, &time);
    int first_city_id = select_first_city_algo2(cities, city_count, &time);
    cities_visited_id[visited_city_count] = first_city_id; 
    int current_id = first_city_id;

    int distance;

    while(current_id != -1)
    {
        visited_city_count++;
        // current_id = next_city_selector_first_closing(cities, current_id, city_count, &time, &distance);
        current_id = next_city_selector_least_score(cities, current_id, city_count, &time, &distance);
        if(current_id != -1)
        {
            total_distance += distance;
            cities_visited_id[visited_city_count] = current_id;
        }
    }

    int return_distance = distance_calculator(cities[cities_visited_id[visited_city_count - 1]], cities[first_city_id]);
    total_distance += return_distance;
    total_time = time + return_distance;
    
    printf("%d %d %d\n", visited_city_count, total_distance, total_time);

    // for(int i = 0; i < visited_city_count; i++)
    // {
    //     printf("%d\n", cities_visited_id[i]);
    // }

    write_output_file(
    "files/output-3.txt",
    cities_visited_id,
    visited_city_count,
    total_distance,
    total_time
    );

    free(cities);

    return 0;
}


int write_output_file(const char* filename, int* route, int visited_count, int total_distance, int total_time)
{
    FILE* fp = fopen(filename, "w");

    if(fp == NULL)
    {
        printf("Output file open error\n");
        return -1;
    }

    fprintf(fp, "%d %d %d\n", visited_count, total_distance, total_time);

    for(int i = 0; i < visited_count; i++)
    {
        fprintf(fp, "%d\n", route[i]);
    }

    fprintf(fp, "\n");
    fclose(fp);

    return 0;
}