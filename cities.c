#include <stdio.h>
#include <stdlib.h>

#include "cities.h"

City *read_cities(const char *filename, int *city_count)
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        return NULL;
    }

    int capacity = 1024;
    int count = 0;

    City *cities = malloc(capacity * sizeof(City));
    if (cities == NULL) {
        fclose(fp);
        return NULL;
    }

    while (1) {
        City c;

        int result = fscanf(fp, "%d %d %d %d %d",
                            &c.id, &c.x, &c.y, &c.open, &c.close);

        if (result == EOF) {
            break;
        }

        if (result != 5) {
            free(cities);
            fclose(fp);
            return NULL;
        }

        c.visited = 0;

        if (count == capacity) {
            capacity *= 2;

            City *temp = realloc(cities, capacity * sizeof(City));
            if (temp == NULL) {
                free(cities);
                fclose(fp);
                return NULL;
            }

            cities = temp;
        }

        cities[count] = c;
        count++;
    }

    fclose(fp);

    *city_count = count;
    return cities;
}