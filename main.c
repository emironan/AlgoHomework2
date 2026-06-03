#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "cities.h"
#include "route_selector.h"

// --- YENİ EKLENEN: Rota Geçerliliği ve Skor Hesaplama (Time Slack & Validation) ---
// Bir rotanın baştan sona zaman pencerelerini ihlal edip etmediğini kontrol eder
int evaluate_route(City* cities, int* route, int route_len, int* out_dist, int* out_time) {
    int current_time = cities[route[0]].open; 
    int total_dist = 0;

    for(int i = 1; i < route_len; i++) {
        int prev_city = route[i-1];
        int curr_city = route[i];
        int d = distance_calculator(cities[prev_city], cities[curr_city]);
        
        total_dist += d;
        current_time += d;

        if (current_time > cities[curr_city].close) {
            return 0; // GEÇERSİZ: Zaman kısıtlaması aşıldı
        }
        if (current_time < cities[curr_city].open) {
            current_time = cities[curr_city].open; // Bekleme yap
        }
    }
    
    // Başlangıç şehrine dönüş
    int return_d = distance_calculator(cities[route[route_len-1]], cities[route[0]]);
    total_dist += return_d;
    current_time += return_d;

    *out_dist = total_dist;
    *out_time = current_time;
    return 1; // GEÇERLİ
}

int main(int argc, char *argv[])
{
    if (argc != 3) {
        printf("Kullanim: %s <input_dosyasi.txt> <output_dosyasi.txt>\n", argv[0]);
        return -1;
    }

    srand((unsigned int)time(NULL));

    int city_count; 
    City* cities = read_cities(argv[1], &city_count);
    if(cities == NULL) return -1;

    int *cities_visited_id = malloc(city_count * sizeof(int));
    int *best_route_id = malloc(city_count * sizeof(int)); 
    int *temp_route = malloc((city_count + 1) * sizeof(int)); // Test rotaları için bellek

    int best_visited_count = -1;
    int best_total_distance = 2147483647;
    int best_total_time = 2147483647;

    int max_iterations = (city_count > 5000) ? 2 : 50; 
    int max_start_cities = (city_count > 1000) ? 10 : city_count;

    printf("Full Optimizasyon (GRASP + Insertion + Local Search) Basladi...\n");

    for(int iter = 0; iter < max_iterations; iter++) 
    {
        for (int start_index = 0; start_index < max_start_cities; start_index++) 
        {
            // 1. HAZIRLIK AŞAMASI
            for (int i = 0; i < city_count; i++) cities[i].visited = 0; 
            int visited_city_count = 0;
            int current_id = cities[start_index].id;
            int current_time = cities[start_index].open; 
            
            cities[start_index].visited = 1;
            cities_visited_id[visited_city_count++] = current_id; 
            int distance = 0;

            // 2. İNŞA AŞAMASI (GRASP ile açgözlü temel rota oluşturma)
            while(current_id != -1) {
                current_id = next_city_selector_grasp(cities, current_id, city_count, &current_time, &distance);
                if(current_id != -1) {
                    cities_visited_id[visited_city_count++] = current_id;
                }
            }

            int eval_dist, eval_time;
            evaluate_route(cities, cities_visited_id, visited_city_count, &eval_dist, &eval_time);

            // 3. POST-PROCESSING (A) : INSERTION (Araya Sıkıştırma)
            // Ziyaret edilmemiş şehirleri zorla aralara sıkıştırmayı dener
            int insertion_improved = 1;
            while(insertion_improved) {
                insertion_improved = 0;
                for(int c = 0; c < city_count; c++) {
                    if (cities[c].visited == 0) { 
                        for(int pos = 1; pos <= visited_city_count; pos++) {
                            // Yeni şehri 'pos' indeksine yerleştirerek geçici rota oluştur
                            for(int k = 0; k < pos; k++) temp_route[k] = cities_visited_id[k];
                            temp_route[pos] = cities[c].id;
                            for(int k = pos; k < visited_city_count; k++) temp_route[k+1] = cities_visited_id[k];

                            int test_dist, test_time;
                            if (evaluate_route(cities, temp_route, visited_city_count + 1, &test_dist, &test_time)) {
                                // Geçerli bir sıkıştırma bulduk! Rotayı kalıcı yap.
                                visited_city_count++;
                                for(int k = 0; k < visited_city_count; k++) cities_visited_id[k] = temp_route[k];
                                cities[c].visited = 1;
                                eval_dist = test_dist;
                                eval_time = test_time;
                                insertion_improved = 1;
                                break; // Bu şehri ekledik, diğerlerine geç
                            }
                        }
                    }
                }
            }

            // 4. POST-PROCESSING (B) : LOCAL SEARCH (Ardışık Swap ile Mesafe Tıraşlama)
            // Şehir sayısını bozmadan komşu şehirlerin yerini değiştirip mesafeyi kısaltır
            int swap_improved = 1;
            while(swap_improved) {
                swap_improved = 0;
                for(int i = 1; i < visited_city_count - 1; i++) {
                    // temp_route içine kopyala
                    for(int k = 0; k < visited_city_count; k++) temp_route[k] = cities_visited_id[k];
                    
                    // i ile i+1'i yer değiştir
                    int temp = temp_route[i];
                    temp_route[i] = temp_route[i+1];
                    temp_route[i+1] = temp;

                    int test_dist, test_time;
                    if (evaluate_route(cities, temp_route, visited_city_count, &test_dist, &test_time)) {
                        if (test_dist < eval_dist) {
                            // Hem geçerli hem de DAHA KISA bir rota bulduk!
                            cities_visited_id[i] = temp_route[i];
                            cities_visited_id[i+1] = temp_route[i+1];
                            eval_dist = test_dist;
                            eval_time = test_time;
                            swap_improved = 1;
                        }
                    }
                }
            }

            // 5. ŞAMPİYON KARŞILAŞTIRMASI (Lexicographical Kuralı [cite: 39])
            int is_better = 0;
            if (visited_city_count > best_visited_count) {
                is_better = 1;
            } else if (visited_city_count == best_visited_count) {
                if (eval_dist < best_total_distance) {
                    is_better = 1;
                } else if (eval_dist == best_total_distance) {
                    if (eval_time < best_total_time)  {
                        is_better = 1;
                    }
                }
            }

            if (is_better) {
                best_visited_count = visited_city_count;
                best_total_distance = eval_dist;
                best_total_time = eval_time;
                for (int i = 0; i < visited_city_count; i++) {
                    best_route_id[i] = cities_visited_id[i];
                }
            }
        }
    }

    printf("\n>>> SUPREME SONUC: %d Sehir | %d Mesafe | %d Zaman <<<\n", 
            best_visited_count, best_total_distance, best_total_time);

    // 6. ÇIKTIYI YAZDIR
    FILE *out_file = fopen(argv[2], "w");
    fprintf(out_file, "%d %d %d\n", best_visited_count, best_total_distance, best_total_time);
    for(int i = 0; i < best_visited_count; i++) fprintf(out_file, "%d\n", best_route_id[i]);
    fclose(out_file);

    free(cities_visited_id);
    free(best_route_id);
    free(temp_route);
    free(cities);

    return 0;
}