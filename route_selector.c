#include "route_selector.h"
#include "cities.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// Temel Mesafe Hesaplayıcı
int distance_calculator(City city_1, City city_2)
{
    double x = (double)city_1.x - (double)city_2.x;
    double y = (double)city_1.y - (double)city_2.y;

    double root_val = sqrt(x * x + y * y);
    return (int)floor(root_val + 0.5);
}

// GRASP ve Fast-Bounding Optimizasyonlu Şehir Seçici
int next_city_selector_grasp(City* cities, int current_city_id, int city_count, int* current_time, int* length)
{
    typedef struct {
        int id;
        double score;
        int time_calc;
    } Candidate;

    Candidate top3[3];
    // Aday listesini başlangıçta garanti büyük değerlerle dolduruyoruz
    for(int i = 0; i < 3; i++) { 
        top3[i].id = -1; 
        top3[i].score = 999999999.0; 
    }

    for(int i = 0 ; i < city_count; i++)
    {
        if((cities[i].id != current_city_id) && (cities[i].visited == 0))
        {
            // 1. FAST BOUNDING: Zamanı çoktan geçtiyse hiç işlem yapma
            if (*current_time > cities[i].close) continue;

            // 2. FAST BOUNDING: Kuş uçuşu Manhattan mesafesi ile karekök öncesi eleme
            int dx = abs(cities[current_city_id].x - cities[i].x);
            int dy = abs(cities[current_city_id].y - cities[i].y);
            int min_possible_dist = (dx > dy) ? dx : dy; 

            if (*current_time + min_possible_dist > cities[i].close) {
                continue; // En iyimser ihtimalde bile yetişemiyoruz
            }

            // 3. Kesin Mesafe Hesaplaması
            int time_calculated = distance_calculator(cities[current_city_id], cities[i]);
            
            // Eğer kapanış saatinden önce varabiliyorsak
            if((*current_time + time_calculated) <= cities[i].close)
            {
                double wait_score = 0;
                if (cities[i].open > *current_time + time_calculated) {
                    wait_score = cities[i].open - (*current_time + time_calculated);
                }

                double remaining_time_score = cities[i].close - (*current_time + time_calculated);
                
                // DENGELİ SKORLAMA (Agresif ceza yok, sadece katsayı)
                // Mesafe: 1.0, Bekleme: 1.5 (Beklemeyi sevmeyiz), Kalan Zaman: 0.1
                double score_calculated = (1.0 * time_calculated) + (1.5 * wait_score) + (0.1 * remaining_time_score);

                // Top 3 listesini güncelle
                if(score_calculated < top3[2].score)
                {
                    top3[2].id = cities[i].id;
                    top3[2].score = score_calculated;
                    top3[2].time_calc = time_calculated;

                    // Bubble Sort
                    for(int j = 2; j > 0; j--) {
                        if(top3[j].score < top3[j-1].score) {
                            Candidate temp = top3[j];
                            top3[j] = top3[j-1];
                            top3[j-1] = temp;
                        }
                    }
                }
            }
        }
    }

    int valid_candidate_count = 0;
    for(int i = 0; i < 3; i++) {
        if(top3[i].id != -1) {
            valid_candidate_count++;
        }
    }

    if(valid_candidate_count == 0) {
        return -1; // Gidilecek hiçbir yer kalmadıysa döngüyü bitir
    }

    // 4. GRASP: Geçerli en iyi adaylar arasından rastgele birini seç
    int random_index = rand() % valid_candidate_count;
    int selected_id = top3[random_index].id;
    int time_calculated = top3[random_index].time_calc;

    // Değişkenleri güncelle
    cities[selected_id].visited = 1;
    *length = time_calculated;

    if(cities[selected_id].open > *current_time + time_calculated) {
        *current_time = cities[selected_id].open; 
    } else {
        *current_time = *current_time + time_calculated; 
    }

    return selected_id;
}