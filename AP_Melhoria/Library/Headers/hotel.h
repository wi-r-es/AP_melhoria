#ifndef HOTEL_H_INCLUDED
#define HOTEL_H_INCLUDED

/* -- Includes -- */
/* libc includes. */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
/* client header file*/
#include "client.h"
#include "reservations.h"
#include "utils.h"



// Definição de Constantes
#define MAX_QUARTOS 400
#define ROOMS_PER_FLOOR 20
#define FLOORS_PER_TOWER 10
#define TOWERS_IN_HOTEL 2
#define MAX_SERVICES 100
#define MAX_CLIENTES 200
// #define MAX_RESERVAS 500

typedef struct{
    
    char *code; 
    char view; //M-mar, P-piscina
    char quality; //B-base, S-superior
    char capacity; // S-single, D-duplo, T-triplo, F-familiar (5max)
    int availability ; //0 used, 1 free 
}ROOM;

ROOM *create_room(const char *code, char view, char quality, char capacity);
void destroy_room(ROOM *room);
void show_room(ROOM *room);
void alter_room(ROOM *room, char view, char quality, char capacity, int availability);

typedef struct {
    ROOM *rooms[ROOMS_PER_FLOOR];
} FLOOR;

typedef struct {
    FLOOR floors[FLOORS_PER_TOWER];
} TOWER;


typedef struct {
    TOWER towers[TOWERS_IN_HOTEL];
    char *name; //dynamically allocated
    int rooms_occupied;
    int num_reservations; // Keep track of the number of reservations 
    int capacity_reservations;
    int num_services; // Keep track of the number of services done
    int num_fichas; // Keep track of the number of fichas 
    service_done *services[MAX_SERVICES];
    FICHA *fichas[MAX_CLIENTES];
    RESERVATION **reservations; // Flexible array member for reservations
    
} HOTEL;

typedef struct {
    CLIENT *list;
}clients_struct;

HOTEL *initialize_hotel(char *_name);
void Load_reservations(HOTEL *hotel);
void show_hotel(HOTEL *hotel);
void destroy_hotel(HOTEL *hotel);
void delete_provisional_client(HOTEL *hotel, int client_cod) ;
void modify_provisional_client(HOTEL *hotel, int client_cod, 
                               const char *new_name, const char *new_address, 
                               const char *new_postal_code, const char *new_locality, const char *new_NIF);
// tipoficha needs to be \0 to be not modified.
void modify_reservation(HOTEL *hotel, int num_reservation, int new_client_cod, char new_tipoFicha, 
                        char *new_room_reserved, const char *new_dataCheckIn, const char *new_dataCheckOut, 
                        int new_numPessoas, int new_payment_number, short int new_payment_mode, 
                        char new_situation[2], const char *new_observations, double new_valorCheckOut);

void delete_reservation(HOTEL *hotel, int num_reservation);
void mark_room_as_reserved(HOTEL *hotel, char *room_code);
void mark_room_as_available(HOTEL *hotel, char *room_code);
void free_reservation_rooms(HOTEL *hotel, RESERVATION *res);

void register_subclient(HOTEL *hotel, int main_reservation_num, CLIENT *subclient, char *subclient_room);
void update_room_states_on_checkin(HOTEL *hotel, RESERVATION *reservation);
void register_payment_on_checkin(HOTEL *hotel, RESERVATION *reservation, int opt, 
                                 int payment_number, short int payment_mode, double amount);

void add_reservation(HOTEL *hotel, RESERVATION *reservation);
int addClient(clients_struct *cs, CLIENT *newClientPtr);

void register_card_return(RESERVATION *reservation, int card_number, int is_returned);
void verify_services_and_calculate_bill(RESERVATION *reservation);
void update_room_states_on_checkout(HOTEL *hotel, RESERVATION *reservation);

void calculate_room_availability(HOTEL *hotel, const char* start_date, const char* end_date);

bool is_room_reserved_on_date(HOTEL* hotel, char *room_code, const char* date);
bool additional_rooms_contains(char** extra_rooms, char *room_code);

void show_occupancy_map_for_room(HOTEL* hotel, char *room_code, const char* start_date, const char* end_date);

void sort_reservations_by_room_code(RESERVATION **reservations, int count) ;

void list_reservations_by_checkin(HOTEL *hotel, const char *start_date, const char *end_date);
void list_reservations_by_checkout(HOTEL *hotel, const char *start_date, const char *end_date);

void generate_payments_file(HOTEL *hotel, const char *start_date, const char *end_date);

ROOM *find_room_by_code(HOTEL *hotel, char *roomCode);



#endif // HOTEL_H_INCLUDED