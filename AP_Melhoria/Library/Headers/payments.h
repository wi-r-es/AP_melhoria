#ifndef PAYMENTS_H_INCLUDED
#define PAYMENTS_H_INCLUDED

/* -- Includes -- */
/* libc includes. */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Estrutura para Pagamento
typedef struct {
    int reservation_code;
    double value;
    int payment_mode; //0-dinheiro, 1-MB, 2-cheque, 3-transferencia
    // Adicionar campos conforme necessário
} PAYMENT;


#endif //PAYMENTS_H_INCLUDED