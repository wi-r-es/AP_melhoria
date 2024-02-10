#include "Headers/data.h"

RoomData *read_room_data_from_file(const char *filename, int *num_entries) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        return NULL;
    }

    char line[MAX_LINE_LENGTH];
    int count = 0;
    RoomData *room_data_array = NULL;
    RoomData *temp;

    while (fgets(line, MAX_LINE_LENGTH, file)) {
        temp = realloc(room_data_array, (count + 1) * sizeof(RoomData));
        if (!temp) {
            perror("Erro ao realocar memória");
            free(room_data_array);
            fclose(file);
            return NULL;
        }
        room_data_array = temp;
        
        char *token = strtok(line, ";");
        if (token) { room_data_array[count].view = token[0]; }
        
        token = strtok(NULL, ";");
        if (token) { room_data_array[count].quality = token[0]; }
        
        token = strtok(NULL, ";");
        if (token) { room_data_array[count].capacity = token[0]; }
        
        token = strtok(NULL, ";");
        if (token) { room_data_array[count].price = atoi(token); }
        
        token = strtok(NULL, ";");
        if (token) { room_data_array[count].cancellation_fee = atoi(token); }
        
        token = strtok(NULL, ";");
        if (token && strlen(token) < sizeof(room_data_array[count].promotion_start)) {
            strncpy(room_data_array[count].promotion_start, token, sizeof(room_data_array[count].promotion_start) - 1);
        }
        
        token = strtok(NULL, ";");
        if (token && strlen(token) < sizeof(room_data_array[count].promotion_end)) {
            strncpy(room_data_array[count].promotion_end, token, sizeof(room_data_array[count].promotion_end) - 1);
        }
        
        token = strtok(NULL, ";");
        if (token) { room_data_array[count].promotion_value = atoi(token); }

        count++;
    }

    fclose(file);
    
    if (num_entries) {
        *num_entries = count;
    }
    
    return room_data_array;
}

int ReadSeasonPricingFromFile(const char *filename, SeasonPricing *season_pricing, int max_entries) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return -1;
    }

    char line[100]; // Assuming each line will not exceed 100 characters
    int i = 0;
    while (fgets(line, sizeof(line), file) && i < max_entries) {
        line[strcspn(line, "\r\n")] = 0; // Removing potential new-line characters
        if (sscanf(line, "%c;%[^;];%[^;];%[^;];%d",
                   &season_pricing[i].season_code,
                   season_pricing[i].season_name,
                   season_pricing[i].start_date,
                   season_pricing[i].end_date,
                   &season_pricing[i].price_increase) == 5) {
            i++;
        } else {
            fprintf(stderr, "Error parsing line: %s\n", line);
            fclose(file);
            return -1;
        }
    }

    fclose(file);
    return i; // Return the number of entries read
}


int ReadServiceTypesFromFile(const char *filename, ServiceType *services, int max_services) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return -1;
    }

    char line[100];
    int i = 0;
    while (fgets(line, sizeof(line), file) && i < max_services) {
        line[strcspn(line, "\r\n")] = 0;
        if (sscanf(line, "%[^;];%[^;];%d",
                   services[i].service_code,
                   services[i].description,
                   &services[i].value) == 3) {
            i++;
        } else {
            fprintf(stderr, "Error parsing line: %s\n", line);
            fclose(file);
            return -1;
        }
    }

    fclose(file);
    return i;
}

