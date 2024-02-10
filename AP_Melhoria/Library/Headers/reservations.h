#ifndef RESERVATIONS_H_INCLUDED
#define RESERVATIONS_H_INCLUDED

/* -- Includes -- */
/* libc includes. */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "client.h"
#include "services.h"
#include "payments.h"



// Definição de Constantes
#define MAX_QUARTOS 400

typedef struct{
    int cod;
    CLIENT *client;
    time_t data;
    int state; //0 prob - 1 def
}FICHA;

FICHA *initialize_ficha(int cod, CLIENT *client);


//reserva
// Estrutura para Reserva
typedef struct reservation{
    int num_reservation; //sequencial
    int client_cod;
    FICHA *client_ficha;
    char tipoFicha; // 'P' -  Provisória, 'D' - Definitiva
    char* room_reserved; // Códigos do quarto reservado
    char dataCheckIn[10];
    char dataCheckOut[10];
    int numPessoas; //a ocupar o quarto
    
    char *extra_rooms[5]; // Códigos dos quartos extra reservados
    
    //double valorAdiantado;
    int payment_number;
    short int payment_mode; //1-dinheiro, 2-MB, 3-cheque, 4-transferencia , 0-null
    char situation[2]; // 'P' - provisoria, 'C'-confirmada, 'O'-em operacao, 'F'-finalizada

    double valorCheckOut; //valor pago no checkout
   // double payment_amount;

    SERVICE *services[5]; //array para codigo dos servicos requesitados
    // Adicionar observações conforme necessário
    char *observations;
} RESERVATION;


RESERVATION *create_reservation(int client_cod, FICHA *fch, char tipoFicha, char *room_reserved, 
                                const char *dataCheckIn, const char *dataCheckOut, int numPessoas, int payment_num,/*double valorAdiantado, */
                                int payment_mode, char *situation, const char *observations, double valorCheckOut);

void destroy_reservation(RESERVATION *res);

void show_reservation(const RESERVATION *res); 

int add_extra_room(RESERVATION *res, char *room_code);
int remove_extra_room(RESERVATION *res, char *room_code);

void register_payment(RESERVATION *res, int payment_number, short int payment_mode, double amount);
void modify_payment(RESERVATION *res, int payment_number, short int new_payment_mode, double new_amount);
void cancel_payment(RESERVATION *res, int payment_number);





#endif // RESERVATIONS_H_INCLUDED