#include "Headers/IO.h"

#ifdef _WIN32
char *clients_file = "Files\\I\\clientes.txt";
char *reservations_file = "Files\\O\\funcionarios.txt";
char *hotel_config = "Files\\O\\hotel.bin";
#else
//char *clients_file = "Files/I/clientes.txt";
char *reservations_file = "Files/O/reservations.bin";
char *hotel_config = "Files/O/hotel.bin";
#endif

extern void logging(char *datafile, const char *funcname, char *info);
extern char *logging_file;
extern void fatal(char *message);

// for test purposes
/*
void writeToFile(char *b)
{
    char *datafile = "newTEST";
    int fd; //= create(datafile, mode_t mode)

    fd = open(datafile, O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
    char *s = strcat(b, "\n");
    // printf("%d", fd);
    if (fd == -1)
        fatal("in main() while opening file");
    // printf("[DEBUG] file descriptor is %d\n",fd);
    // writting file
    if (write(fd, s, strlen(s)) == -1)
        fatal("in main() while writting buffer to file");
    // closing file
    if (close(fd) == -1)
        fatal("in main() while closing file");
}
*/

void save_reservation_to_file(FILE *file, const RESERVATION *res) {
    // Write fixed-size fields
    fwrite(&res->num_reservation, sizeof(res->num_reservation), 1, file);
    fwrite(&res->client_cod, sizeof(res->client_cod), 1, file);
    fwrite(&res->tipoFicha, sizeof(res->tipoFicha), 1, file);
    fwrite(&res->room_reserved, sizeof(res->room_reserved), 1, file);
    fwrite(&res->dataCheckIn, sizeof(res->dataCheckIn), 1, file);
    fwrite(&res->dataCheckOut, sizeof(res->dataCheckOut), 1, file);
    fwrite(&res->numPessoas, sizeof(res->numPessoas), 1, file);
    fwrite(&res->payment_number, sizeof(res->payment_number), 1, file);
    fwrite(&res->payment_mode, sizeof(res->payment_mode), 1, file);
    fwrite(&res->situation[0], sizeof(res->situation), 1, file);
    fwrite(&res->valorCheckOut, sizeof(res->valorCheckOut), 1, file);
    // Write dynamic fields (like observations)
    // We need to write the length followed by the actual string
    size_t obs_length = (res->observations != NULL) ? strlen(res->observations) + 1 : 0;
    fwrite(&obs_length, sizeof(size_t), 1, file); // Write the length of observations
    if (obs_length > 0) {
        fwrite(res->observations, sizeof(char), obs_length, file); // Write the observations string
    }

}

void save_ficha_to_file(FILE *file, const FICHA *ficha) {
    fwrite(&ficha->cod, sizeof(ficha->cod), 1, file);
    save_client_info_in_ficha(file, ficha->client); // Save client identification
    fwrite(&ficha->data, sizeof(ficha->data), 1, file);
}

void save_service_done_to_file(FILE *file, const service_done *service) {
    fwrite(&service->service_code, sizeof(service->service_code), 1, file);
    fwrite(&service->room_code, sizeof(service->room_code), 1, file);
    fwrite(&service->funcionario, sizeof(service->funcionario), 1, file);
    fwrite(&service->date, sizeof(service->date), 1, file);
    
    fwrite(service->service->type_of_service, sizeof(service->service->type_of_service), 1, file);
    fwrite(service->service->special_service_code, sizeof(service->service->special_service_code), 1, file);
    fwrite(service->service->description, sizeof(service->service->description), 1, file);
    fwrite(&service->service->value, sizeof(service->service->value), 1, file);
}



void save_hotel_to_binary_file(const char *filename, const HOTEL *hotel) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("Error opening file for writing");
        return;
    }
     printf("Hotel");
    // Write the name length and name.
    size_t name_length = strlen(hotel->name) + 1; // +1 for null terminator
    fwrite(&name_length, sizeof(size_t), 1, file);
    fwrite(hotel->name, sizeof(char), name_length, file);
    // Write scalar fields.
    fwrite(&hotel->rooms_occupied, sizeof(int), 1, file);
    fwrite(&hotel->num_reservations, sizeof(int), 1, file);
    fwrite(&hotel->num_services, sizeof(int), 1, file);
    fwrite(&hotel->num_fichas, sizeof(int), 1, file);
    // Write room configurations for each room.
    for (int t = 0; t < TOWERS_IN_HOTEL; ++t) {
        for (int f = 0; f < FLOORS_PER_TOWER; ++f) {
            for (int r = 0; r < ROOMS_PER_FLOOR; ++r) {
                ROOM *room = hotel->towers[t].floors[f].rooms[r];
                fwrite(room, sizeof(ROOM), 1, file);
            }
        }
    }
    // Write reservations if there are any.
    for (int i = 0; i < hotel->num_reservations; ++i) {
        if(hotel->num_reservations ==1) break;
        // Assuming a function to write each reservation exists.
        //if()
        save_reservation_to_file(file, hotel->reservations[i]);
    }
    // Write services if there are any.
    for (int i = 0; i < hotel->num_services; ++i) {
        if(hotel->num_services ==0) break;
        // Assuming a function to write each reservation exists.
        save_service_done_to_file(file, hotel->services[i]);
    }
    // Write ficha if there are any.
    for (int i = 0; i < hotel->num_fichas; ++i) {
        if(hotel->num_fichas ==0) break;
        // Assuming a function to write each reservation exists.
        save_ficha_to_file(file, hotel->fichas[i]);
    }
    fclose(file);
    printf("Hotel configuration saved to '%s'.\n", filename);
}

void load_reservation_from_file(FILE *file, RESERVATION *res) {
    // Read fixed-size fields
    fread(&res->num_reservation, sizeof(res->num_reservation), 1, file);
    fread(&res->client_cod, sizeof(res->client_cod), 1, file);
    fread(&res->tipoFicha, sizeof(res->tipoFicha), 1, file);
    fread(&res->room_reserved, sizeof(res->room_reserved), 1, file);
    fread(&res->dataCheckIn, sizeof(res->dataCheckIn), 1, file);
    fread(&res->dataCheckOut, sizeof(res->dataCheckOut), 1, file);
    fread(&res->numPessoas, sizeof(res->numPessoas), 1, file);
    fread(&res->payment_number, sizeof(res->payment_number), 1, file);
    fread(&res->payment_mode, sizeof(res->payment_mode), 1, file);
    fread(&res->situation, sizeof(res->situation), 1, file);
    fread(&res->valorCheckOut, sizeof(res->valorCheckOut), 1, file);

    // Read dynamic fields (like observations)
    size_t obs_length = 0;
    fread(&obs_length, sizeof(size_t), 1, file); // Read the length of observations
    if (obs_length > 0) {
        res->observations = (char *)ec_malloc(obs_length);
        fread(res->observations, sizeof(char), obs_length, file); // Read the observations string
    } else {
        res->observations = NULL; // No observations
    }

    // Read any other dynamically sized fields here...
}


HOTEL *load_hotel_from_binary_file(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Error opening file for reading");
        return NULL;
    }

    // Read the name length and name.
    size_t name_length;
    fread(&name_length, sizeof(size_t), 1, file);
    char *name = (char *)ec_malloc(name_length * sizeof(char));
    fread(name, sizeof(char), name_length, file);

    // Read the number of reservations/fichas to determine how much memory to allocate for the hotel.
    
    

    HOTEL *hotel = NULL;
    hotel = initialize_hotel(name);
    // Read scalar fields.
    fread(&hotel->rooms_occupied, sizeof(int), 1, file);
    fread(&hotel->num_reservations, sizeof(int), 1, file);
    fread(&hotel->num_services, sizeof(int), 1, file);
    fread(&hotel->num_fichas, sizeof(int), 1, file);
    
    // Read room configurations for each room.
    for (int t = 0; t < TOWERS_IN_HOTEL; ++t) {
        for (int f = 0; f < FLOORS_PER_TOWER; ++f) {
            for (int r = 0; r < ROOMS_PER_FLOOR; ++r) {
                ROOM *room = hotel->towers[t].floors[f].rooms[r];
                fread(room, sizeof(ROOM), 1, file);
            }
        }
    }

     // Allocate and load reservations
    hotel->reservations = ec_malloc(hotel->num_reservations * sizeof(RESERVATION *));
    for (int i = 0; i < hotel->num_reservations; ++i) {
        hotel->reservations[i] = ec_malloc(sizeof(RESERVATION));
        memset(hotel->reservations[i], 0, sizeof(RESERVATION));
        load_reservation_from_file(file, hotel->reservations[i]);
    }
    // Allocate and load services
    //hotel->services = ec_malloc(hotel->num_services * sizeof(service_done *));
    for (int i = 0; i < hotel->num_services; ++i) {
        hotel->services[i] = ec_malloc(sizeof(service_done));
        memset(hotel->services[i], 0, sizeof(service_done));
        // Assume load_services_from_file exists and is implemented
        load_service_done_from_file(file, hotel->services[i], hotel->num_services);
    }
    // Allocate and load fichas
    //hotel->fichas = ec_malloc(hotel->num_fichas * sizeof(FICHA *));
    for (int i = 0; i < hotel->num_fichas; ++i) {
        hotel->fichas[i] = ec_malloc(sizeof(FICHA));
        hotel->fichas[i]->client = ec_malloc(sizeof(CLIENT));
        memset(hotel->fichas[i], 0, sizeof(FICHA));
        memset(hotel->fichas[i]->client, 0, sizeof(CLIENT));
        // This requires implementing load_fichas_from_file correctly
        load_ficha_from_file(file, hotel->fichas[i], hotel->fichas[i]->client);
    }

    fclose(file);
    printf("Hotel configuration loaded from '%s'.\n", filename);

    return hotel;
}




void load_ficha_from_file(FILE *file, FICHA *ficha, CLIENT *clients) {
    fread(&ficha->cod, sizeof(ficha->cod), 1, file);
    load_client_info_in_ficha(file, clients); // Load and assign CLIENT pointer
    fread(&ficha->data, sizeof(ficha->data), 1, file);
}





  /*  // Write fichas data.
    for (int i = 0; i < hotel->num_fichas; ++i) {
        // Assume a function to write each FICHA exists.
        save_ficha_to_file(file, &hotel->fichas[i]);
    }
    void save_ficha_to_file(FILE *file, const FICHA *ficha) {
    fwrite(&ficha->cod, sizeof(ficha->cod), 1, file);
    //CLIENTS ARE IN SEPERATE FILE.

    // Saving time_t directly
    fwrite(&ficha->data, sizeof(ficha->data), 1, file);

    */



void save_client_info_in_ficha(FILE *file, const CLIENT *client) {
    // Save client code; use -1 to indicate a null client reference
    int client_cod = client ? client->cod : -1;
    fwrite(&client_cod, sizeof(client_cod), 1, file);

    if (client) {
        // For each string, save its length followed by its content
        // Name
        size_t name_length = strlen(client->client->name) + 1; // Include null terminator
        fwrite(&name_length, sizeof(name_length), 1, file);
        fwrite(client->client->name, name_length, 1, file);

        // Address
        size_t address_length = strlen(client->client->address) + 1; // Include null terminator
        fwrite(&address_length, sizeof(address_length), 1, file);
        fwrite(client->client->address, address_length, 1, file);

        // Locality
        size_t locality_length = strlen(client->client->locality) + 1; // Include null terminator
        fwrite(&locality_length, sizeof(locality_length), 1, file);
        fwrite(client->client->locality, locality_length, 1, file);

        // Postal Code and NIF can be directly written as their sizes are known and fixed
        fwrite(client->client->postal_code, sizeof(client->client->postal_code), 1, file);
        fwrite(client->client->NIF, sizeof(client->client->NIF), 1, file);
    }
}


void load_client_info_in_ficha(FILE *file, CLIENT *client_ptr) {
    int client_cod;
    fread(&client_cod, sizeof(client_cod), 1, file);

    // Allocate memory for the CLIENT structure
    CLIENT *client = (CLIENT *)ec_malloc(sizeof(CLIENT));
    client_ptr = client; // Assign the newly allocated CLIENT to the provided pointer

    if (client_cod != -1) { // Proceed only if a valid client code was saved
        client->cod = client_cod; // Assign the client code

        // Load and allocate memory for the dynamic string fields (name, address, locality)
        size_t length;
        // Name
        fread(&length, sizeof(length), 1, file);
        client->client->name = (char *)ec_malloc(length);
        fread(client->client->name, length, 1, file);

        // Address
        fread(&length, sizeof(length), 1, file);
        client->client->address = (char *)ec_malloc(length);
        fread(client->client->address, length, 1, file);

        // Locality
        fread(&length, sizeof(length), 1, file);
        client->client->locality = (char *)ec_malloc(length);
        fread(client->client->locality, length, 1, file);

        // For fixed-size fields like postal_code and NIF, you can read directly
        fread(client->client->postal_code, sizeof(client->client->postal_code), 1, file);
        fread(client->client->NIF, sizeof(client->client->NIF), 1, file);
    } else {
        // Handle case where no client was associated with the ficha
        client_ptr = NULL;
    }
}

void load_service_done_from_file(FILE *file, service_done *service, int num_services) {

    service_done *srv_done = (service_done *)ec_malloc(sizeof(service_done));
    service = srv_done; 


    fread(&service->service_code, sizeof(service->service_code), 1, file);
    fread(&service->room_code, sizeof(service->room_code), 1, file);
    fread(&service->funcionario, sizeof(service->funcionario), 1, file);
    fread(&service->date, sizeof(service->date), 1, file);

    fread(service->service->type_of_service, sizeof(service->service->type_of_service), 1, file);
    fread(service->service->special_service_code, sizeof(service->service->special_service_code), 1, file);
    fread(service->service->description, sizeof(service->service->description), 1, file);
    fread(&service->service->value, sizeof(service->service->value), 1, file);
}
