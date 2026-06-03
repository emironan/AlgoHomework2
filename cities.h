#ifndef CITIES_H
#define CITIES_H


typedef struct {
    int id;
    int x, y;
    int open, close;
    int visited;
} City;

City *read_cities(const char *filename, int *city_count);


#endif