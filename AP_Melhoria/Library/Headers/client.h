#ifndef PESSOA_H_INCLUDED
#define PESSOA_H_INCLUDED

/* -- Includes -- */
/* libc includes. */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"


//person
typedef struct {
    char *name;
    char *address;
    char postal_code[9];
    char *locality;
    char *NIF;
} person;

person *create_person(const char *_name, const char *address, const char *postal_code, const char *localidade, const char *NIF);
void DestroyPerson(void *p);



//client
typedef struct {
    person *client;
    int cod; //codigo cliente
    char client_type; //N-normal , V-VIP, E-excelencia
    char *card;
    int type; //0 temp, 1 def
    //subclient *subordinates[];
} CLIENT;

CLIENT *create_client(person *_client, char _client_type, const char *_card);
void DestroyClient(void *c);
void ShowClient(void *c);

//sublcient
typedef struct{
    person *client;
    int cod;
    CLIENT *prime;
    int prime_code; 
    int prime_room_code;

}subclient;

subclient *create_subclient(person *_client, CLIENT *_prime, int _prime_room_code);
void DestroySubclient(void *sc);
void ShowSubclient(void *sc);
void update_person_details(CLIENT *p, const char *new_name, const char *new_address, 
                            const char *new_postal_code, const char *new_locality, 
                            const char *new_NIF);

CLIENT *create_random_clients();




#endif // PESSOA_H_INCLUDED