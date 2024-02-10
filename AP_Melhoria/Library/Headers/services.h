#ifndef SERVICES_H_INCLUDED
#define SERVICES_H_INCLUDED

/* -- Includes -- */
/* libc includes. */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "client.h"

// servico complementar
typedef struct {
    char type_of_service[5];
    char special_service_code[5];
    char description[100];
    double value;
    // Adicionar campos conforme necessário
} SERVICE;


SERVICE *create_service(const char *type_of_service, const char *special_service_code, const char *descricao, double _value);
void destroy_service(SERVICE *service);
void modify_service(SERVICE *srv, const char *type_srv, char *special_srv_code, const char *new_description, double _value);


// servico complementar efetuado
typedef struct{
    int service_code; //sequencial
    int room_code;
    int funcionario;
    time_t date;
    SERVICE *service;
    int flag_state; // 0=to be done, 1= done.
}service_done;

service_done *create_service_done(int service_code, int room_code, int funcionario, SERVICE *service);
void destroy_service_done(service_done *done_service);
void modify_service_done(service_done *sd, int new_service_code, int new_room_code, 
                               int new_funcionario, time_t new_date, 
                               const char *type_srv, char *special_srv_code, 
                               const char *new_description, double new_value);

void mark_service_done(service_done *svd);






#endif //SERVICES_H_INCLUDED