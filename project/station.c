#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "station.h"


/* Creeaza o gara cu un numar fix de peroane.
 * 
 * platforms_no: numarul de peroane ale garii
 *
 * return: gara creata
 */
TrainStation* open_train_station(int platforms_no) {
    TrainStation *new_train_station = malloc(sizeof(TrainStation));
    new_train_station -> platforms_no = platforms_no;
    new_train_station -> platforms = malloc(platforms_no * sizeof(Train));
    return new_train_station;
}


/* Elibereaza memoria alocata pentru gara.
 *
 * station: gara existenta
 */
void close_train_station(TrainStation *station) {
    if (station != NULL)
        for(int i = 0; i < station -> platforms_no; ++i) {
            if(station -> platforms[i] != NULL) {
                while(station -> platforms[i] -> train_cars != NULL) {
                    TrainCar* curr = station -> platforms[i] -> train_cars;
                    station -> platforms[i] -> train_cars = station -> platforms[i] -> train_cars -> next;
                    free(curr);
                }
                free(station -> platforms[i]);
            }
        }
    else
        free(station);
}


/* Afiseaza trenurile stationate in gara.
 *
 * station: gara existenta
 * f: fisierul in care se face afisarea
 */
void show_existing_trains(TrainStation *station, FILE *f) {
    if (station != NULL)
        for(int i = 0; i < station -> platforms_no; ++i) {
            fprintf(f, "%d: ", i);
            if(station -> platforms[i] != NULL) {
                fprintf(f, "(%d)", station -> platforms[i] -> locomotive_power);
                TrainCar *curr = station -> platforms[i] -> train_cars;
                while(curr != NULL) {
                    fprintf(f, "-|%d|", curr -> weight);
                    curr = curr -> next;
                }
                free(curr);
            }
            fprintf(f, "\n");
        }
}


/* Adauga o locomotiva pe un peron.
 * 
 * station: gara existenta
 * platform: peronul pe care se adauga locomotiva
 * locomotive_power: puterea de tractiune a locomotivei
 */
void arrive_train(TrainStation *station, int platform, int locomotive_power) {
    for (int i = 0; i < station -> platforms_no; ++i) {
        if(i == platform) {
            if (station -> platforms[i] == NULL) {
                station -> platforms[i] = malloc(sizeof(Train));
                station -> platforms[i] -> train_cars = NULL;
                station -> platforms[i] -> locomotive_power = locomotive_power;
            }
        }
    }
}


/* Elibereaza un peron.
 * 
 * station: gara existenta
 * platform: peronul de pe care pleaca trenul
 */
void leave_train(TrainStation *station, int platform) {
    for (int i = 0; i < station -> platforms_no; ++i) {
        if(i == platform) {
            if (station -> platforms[i] != NULL) {
                free(station -> platforms[i]);
                station -> platforms[i] = NULL;
            }
        }
    }
}


/* Adauga un station -> (*platforms)[i] -> train_cars la capatul unui tren.
 * 
 * station: gara existenta
 * platform: peronul pe care se afla trenul
 * weight: greutatea station -> (*platforms)[i] -> train_carsului adaugat
 */
void add_train_car(TrainStation *station, int platform, int weight) {
    TrainCar *new_traincar = malloc(sizeof(TrainCar));
    new_traincar -> next  = NULL;
    new_traincar -> weight = weight;
    for (int i = 0; i < station -> platforms_no; ++i) {
        if(i == platform) {
            if (station -> platforms[i] != NULL) {
                if (station -> platforms[i] -> train_cars == NULL) {
                    station -> platforms[i] -> train_cars = new_traincar;
                }
                else {
                    TrainCar *curr = station -> platforms[i] -> train_cars;
                    while(curr -> next != NULL){
                        curr = curr -> next;
                    }
                    curr -> next = new_traincar;
                    free(curr);
                }
            }
        }
    }
}


/* Scoate vagoanele de o anumita greutate dintr-un tren.
 * 
 * station: gara existenta
 * platform: peronul pe care se afla trenul
 * weight: greutatea station -> (*platforms)[i] -> train_carsului scos
 */
void remove_train_cars(TrainStation *station, int platform, int weight) {
    for (int i = 0; i < station -> platforms_no; ++i) {
        if(i == platform) {
            if (station -> platforms[i] != NULL) {
                if (station -> platforms[i] -> train_cars != NULL){
                    TrainCar *curr = station -> platforms[i] -> train_cars;
                    TrainCar *prev = station -> platforms[i] -> train_cars;
                    while(curr != NULL){
                        curr = curr -> next;
                        if (curr != NULL && curr -> weight == weight) {
                            prev -> next = curr -> next;  
                        }
                        else prev = curr;
                    }
                    if (station -> platforms[i] -> train_cars -> weight == weight) {
                        station -> platforms[i] -> train_cars = station -> platforms[i] -> train_cars -> next;
                    }
                }
            }
        }
    }
}


/* Muta o secventa de vagoane dintr-un tren in altul.
 * 
 * station: gara existenta
 * platform_a: peronul pe care se afla trenul de unde se scot vagoanele
 * pos_a: pozitia primului station -> (*platforms)[i] -> train_cars din secventa
 * cars_no: lungimea secventei
 * platform_b: peronul pe care se afla trenul unde se adauga vagoanele
 * pos_b: pozitia unde se adauga secventa de vagoane
 */
void move_train_cars(TrainStation *station, int platform_a, int pos_a, 
                                int cars_no, int platform_b, int pos_b) {
    
    if (cars_no <= 0 || pos_a <= 0 || pos_b <= 0) return;
    
    TrainCar *start_a = station -> platforms[platform_a] -> train_cars;
    TrainCar *prev_a = NULL; 
    int i = 0;                            
    for(i = 1; i < pos_a && start_a != NULL; ++i) {
        prev_a = start_a; //pointer la prev-ul primului vagon care trebuie mutat
        start_a = start_a -> next; //pointer la primul vagon care trebuie mutat
    }
    if (i != pos_a) return; //cazul in care primul vagon care trebuie mutat nu exista
    TrainCar *start_b = station -> platforms[platform_b] -> train_cars;  
    TrainCar *prev_b = NULL;                              
    for(i = 1; i < pos_b && start_b != NULL; ++i) {
        prev_b = start_b; //pointer la prev ul locului unde trebuie mutat primul vagon
        start_b = start_b -> next; //pointer la locul unde trebuie mutat primul vagon
    }
    if (i != pos_b) return; //cazul in care incearca sa adauge vagoane pe o pozitie inexistenta

    TrainCar *check = start_a;
    int count = 0;

    while(check) {
        check = check -> next;
        ++count;
    }

    TrainCar *ptr = start_a; //vagonul care trebuie mutat
    TrainCar *curr = NULL;
    if (count >= cars_no)
        for(int i = 0; i < cars_no && ptr != NULL; ++i) {
            if(prev_a != NULL) prev_a -> next = ptr -> next; //in caz ca primul vagon care trebuie mutat nu e primul din tren
                                                            //leaga vagonul precedent de urmatorul vagon(astfel eliminandu-l din tren)
            curr = ptr -> next; // curr tine urmatorul vagon care trebuie mutat
            ptr -> next = start_b; // next-ul vagonului care este mutat este pozitia pe care trebuie mutat, in felul asta luand el acea pozitie
            if(prev_b != NULL) prev_b -> next = ptr; //daca pozitia pe care vagonul trebuia mutat nu e prima, atunci leaga vagonul din spate de acesta
            if(prev_a == NULL) station -> platforms[platform_a] -> train_cars = curr; //daca pozitia vagonului care trebuie mutat e prima, atunci urmatorul vagon 
                                                                    //va deveni primul (asfel eliminandu-l din tren)
            if(prev_b == NULL) station -> platforms[platform_b] -> train_cars = ptr; // daca pozitia unde vagonul trebuie mutat este prima atunci ptr devine primul vagon
            if(prev_b != NULL) prev_b = prev_b -> next;
            else prev_b = ptr;
            ptr = curr; // se pregateste de urmatoarea iteratie
        }
}


/* Gaseste trenul cel mai rapid.
 * 
 * station: gara existenta
 *
 * return: peronul pe care se afla trenul
 */
int find_express_train(TrainStation *station) {
    int max = 0;
    int peronMax = 0;
    int totalWeight = 0;
    for (int i = 0; i < station -> platforms_no; ++i) {
        if (station -> platforms[i] != NULL) {
            totalWeight = station -> platforms[i] -> locomotive_power; 
            TrainCar *curr = station -> platforms[i] -> train_cars;
            while (curr != NULL) {
                totalWeight -= curr -> weight;
                curr = curr -> next;
            }
            if(totalWeight > max) {
                max = totalWeight;
                peronMax = i;
            }
        }
    }
    return peronMax;
}


/* Gaseste trenul supraincarcat.
 * 
 * station: gara existenta
 *
 * return: peronul pe care se afla trenul
 */
int find_overload_train(TrainStation *station) {
    for (int i = 0; i < station -> platforms_no; ++i) {
        if (station -> platforms[i] != NULL) {
            int totalWeight = 0;
            TrainCar *curr = station -> platforms[i] -> train_cars;
            while (curr != NULL) {
                totalWeight += curr -> weight;
                curr = curr -> next;
            }
            if(totalWeight > station -> platforms[i] -> locomotive_power) {
                return i;
            }
        }
    }
    return -1;
}


/* Gaseste trenul cu incarcatura optima.
 * 
 * station: gara existenta
 *
 * return: peronul pe care se afla trenul
 */
int find_optimal_train(TrainStation *station) {
    int min = 100;
    int peronMin = 0;
    int totalWeight = 0;
    for (int i = 0; i < station -> platforms_no; ++i) {
        if (station -> platforms[i] != NULL) {
            totalWeight = station -> platforms[i] -> locomotive_power; 
            TrainCar *curr = station -> platforms[i] -> train_cars;
            while (curr != NULL) {
                totalWeight -= curr -> weight;
                curr = curr -> next;
            }
            if(totalWeight < min) {
                min = totalWeight;
                peronMin = i;
            }
        }
    }
    return peronMin;
}


/* Gaseste trenul cu incarcatura nedistribuita bine.
 * 
 * station: gara existenta
 * cars_no: lungimea secventei de vagoane
 *
 * return: peronul pe care se afla trenul
 */
int find_heaviest_sequence_train(TrainStation *station, int cars_no, TrainCar **start_car) {
    TrainCar *curr = NULL;
    TrainCar *sequence = NULL;
    int totalWeight = 0;
    int max = 0;
    int nrPeron = -1;
    for (int i = 0; i < station -> platforms_no; ++i) {
        int count = 0;

        if (station -> platforms[i]) {
            TrainCar *check = station -> platforms[i] -> train_cars;
            while(check) {
                check = check -> next;
                ++count;
            }
        }
        if (count >= cars_no) {
            if (station -> platforms[i]) curr = station -> platforms[i] -> train_cars;
            while (curr != NULL) {
                if (count >= cars_no) {
                    sequence = curr;
                    totalWeight = 0;
                    for(int j = 0; j < cars_no && sequence != NULL; ++j) {
                        totalWeight += sequence -> weight;
                        sequence = sequence -> next;
                    }
                    if(totalWeight >= max) {
                        max = totalWeight;
                        *start_car = curr;
                        nrPeron = i;
                    }
                }
                curr = curr -> next;
                count --;
            }
        }
    }
    if(nrPeron > -1) return nrPeron;
    else *start_car = NULL;
    return -1;
}



/* Ordoneaza vagoanele dintr-un tren in ordinea descrescatoare a greutatilor.
 * 
 * station: gara existenta
 * platform: peronul pe care se afla trenul
 */

// TrainCar* swap(TrainCar* vagon1, TrainCar* vagon2)
// {
//     TrainCar* tmp = vagon2->next;
//     vagon2->next = vagon1;
//     vagon2->next = tmp;
//     return vagon2;
// }

void order_train(TrainStation *station, int platform) {
    // for (int i = 0; i < station -> platforms_no; ++i){
    //     if (i == platform)
    //         if (station -> platforms[i] != NULL) {
    //             TrainCar* curr = station -> platforms[i] -> train_cars;
    //             while (curr -> next != NULL) {
    //                 if (curr -> next -> weight > curr -> weight) {
    //                     TrainCar* temp = curr -> next;
    //                     if (curr == station -> platforms[i] -> train_cars) station -> platforms[i] -> train_cars = temp;
    //                     curr -> next = temp -> next;
    //                     temp -> next = curr;
    //                 }
    //                 if (curr -> next != NULL) curr = curr -> next;
    //                 printf("W@OP@O@@O");
    //             }
    //         }
    // }
}


/* Scoate un station -> (*platforms)[i] -> train_cars din trenul supraincarcat.
 * 
 * station: gara existenta
 */
void fix_overload_train(TrainStation *station) {
}
