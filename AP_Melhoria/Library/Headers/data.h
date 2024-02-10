#ifndef DATA_H_INCLUDED
#define DATA_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024

typedef struct {
    char view;
    char quality;
    char capacity;
    int price;
    int cancellation_fee;
    char promotion_start[6];
    char promotion_end[6];
    int promotion_value;
} RoomData;

RoomData *read_room_data_from_file(const char *filename, int *num_entries);

typedef struct {
    char season_code; // 'B' for baixa, 'I' for intermédia, 'M' for média, 'A' for alta
    char season_name[20]; // "baixa", "intermédia", "média", "alta"
    char start_date[6]; // Start date in MM/DD format
    char end_date[6]; // End date in MM/DD format
    int price_increase; // Percentage of the price increase
} SeasonPricing;
int ReadSeasonPricingFromFile(const char *filename, SeasonPricing *season_pricing, int max_entries);

typedef struct {
    char service_code[10]; // For example, "PA", "A", "PA+A", etc.
    char description[50];  // Description of the service
    int value;             // Value in euros
} ServiceType;

int ReadServiceTypesFromFile(const char *filename, ServiceType *services, int max_services);





#endif // DATA_H_INCLUDED