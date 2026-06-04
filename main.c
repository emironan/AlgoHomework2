#include <stdio.h>
#include <stdlib.h>
#include "cities.h"
#include "first_city_selector.h"
#include "route_selector.h"
#include <time.h>

#define RANDOM_TRIALS_PER_START 100

typedef struct
{
    int route[55000];
    int visited_count;
    int total_distance;
    int total_time;
} RouteResult;


RouteResult run_route_from_start(City* cities, int city_count, int first_city_id, int randomized);
void reset_visited(City* cities, int city_count);
int write_output_file(const char* filename, int* route, int visited_count, int total_distance, int total_time);

int cities_visited_id[55000];
int visited_city_count = 0;
int total_time = 0;
int total_distance = 0;

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s input_file_name output_file_name\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *input_file = argv[1];
    char *output_file = argv[2];

    printf("Algo Project 2\n");

    srand((unsigned int)time(NULL));

    int city_count; 
    City* cities = read_cities(input_file, &city_count);

    if(cities == NULL)
    {
        printf("Cities read error\n");
        return -1;
    }

    printf("City Count = %d\n", city_count);
    
    int start_candidates[20];
    int candidate_count = 20;

    get_best_20_start_candidates(
        cities,
        city_count,
        start_candidates,
        candidate_count
    );

    RouteResult best_result;
    best_result.visited_count = -1;
    best_result.total_distance = 0;
    best_result.total_time = 0;

    for(int i = 0; i < candidate_count; i++)
    {
        int first_city_id = start_candidates[i];

        if(first_city_id == -1)
            continue;

        RouteResult normal_result = run_route_from_start(
            cities,
            city_count,
            first_city_id,
            0
        );

        printf("Start %d normal -> %d %d %d\n",
            first_city_id,
            normal_result.visited_count,
            normal_result.total_distance,
            normal_result.total_time);

        if(normal_result.visited_count > best_result.visited_count ||
        (normal_result.visited_count == best_result.visited_count &&
            normal_result.total_distance < best_result.total_distance))
        {
            best_result = normal_result;
        }

        int best_random_count = -1;

        for(int trial = 0; trial < RANDOM_TRIALS_PER_START; trial++)
        {
            RouteResult random_result = run_route_from_start(
                cities,
                city_count,
                first_city_id,
                1
            );

            if(random_result.visited_count > best_random_count)
                best_random_count = random_result.visited_count;

            if(random_result.visited_count > best_result.visited_count ||
            (random_result.visited_count == best_result.visited_count &&
                random_result.total_distance < best_result.total_distance))
            {
                best_result = random_result;

                printf("  NEW GLOBAL BEST trial %d -> %d %d %d\n",
                    trial,
                    best_result.visited_count,
                    best_result.total_distance,
                    best_result.total_time);
            }
        }

        printf("Start %d random best -> %d\n",
            first_city_id,
            best_random_count);
    }

    printf("BEST: %d %d %d\n",
        best_result.visited_count,
        best_result.total_distance,
        best_result.total_time);

    write_output_file(
        output_file,
        best_result.route,
        best_result.visited_count,
        best_result.total_distance,
        best_result.total_time
    );

    free(cities);

    return 0;
}

RouteResult run_route_from_start(City* cities, int city_count, int first_city_id, int randomized)
{
    RouteResult result;

    result.visited_count = 0;
    result.total_distance = 0;
    result.total_time = 0;

    reset_visited(cities, city_count);

    int current_time = cities[first_city_id].open;
    int distance = 0;
    int current_id = first_city_id;

    cities[first_city_id].visited = 1;
    result.route[result.visited_count] = first_city_id;

    while(current_id != -1)
    {
        result.visited_count++;

        if(randomized)
        {
            current_id = next_city_selector_randomized_top3(
                cities,
                current_id,
                city_count,
                &current_time,
                &distance
            );
        }
        else
        {
            current_id = next_city_selector_least_score(
                cities,
                current_id,
                city_count,
                &current_time,
                &distance
            );
        }

        if(current_id != -1)
        {
            result.total_distance += distance;
            result.route[result.visited_count] = current_id;
        }
    }

    int return_distance = distance_calculator(
        cities[result.route[result.visited_count - 1]],
        cities[first_city_id]
    );

    result.total_distance += return_distance;
    result.total_time = current_time + return_distance;

    return result;
}

void reset_visited(City* cities, int city_count)
{
    for(int i = 0; i < city_count; i++)
    {
        cities[i].visited = 0;
    }
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