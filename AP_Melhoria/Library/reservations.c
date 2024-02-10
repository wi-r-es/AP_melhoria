#include "Headers/reservations.h"


RESERVATION *create_reservation(int client_cod, FICHA *fch, char tipoFicha, char *room_reserved, 
                                const char *dataCheckIn, const char *dataCheckOut, int numPessoas, int payment_num,/*double valorAdiantado, */
                                int payment_mode, char *situation, const char *observations, double valorCheckOut) {

    static int _num_reservation =0;
    RESERVATION *res = (RESERVATION *)ec_malloc(sizeof(RESERVATION));
    res->num_reservation = _num_reservation++;
    res->client_cod = client_cod;
    if(tipoFicha=='P' && tipoFicha=='D')
        res->client_ficha = fch;
    res->tipoFicha = tipoFicha;
    res->room_reserved = room_reserved ? strdup(room_reserved) : NULL;
    strncpy(res->dataCheckIn, dataCheckIn, sizeof(res->dataCheckIn) - 1);
    strncpy(res->dataCheckOut, dataCheckOut, sizeof(res->dataCheckOut) - 1);
    res->numPessoas = numPessoas;
    // Services array and extra_rooms array initialization should be handled separately
    //res->valorAdiantado = valorAdiantado;
    for (int i=0; i<5; i++){
        res->
        extra_rooms[i]=NULL; 
    }
    for (int i=0; i<5; i++){
        res->services[i]=NULL; 
    }
    res->payment_number=payment_num;
    res->payment_mode = payment_mode;
    res->situation[0] = situation[0];
    res->observations = observations ? strdup(observations) : NULL; // Dynamically allocating observations string
    res->valorCheckOut = valorCheckOut;

    return res;
}

void destroy_reservation(RESERVATION *res) {
    if (res != NULL) {
        if(res->observations) {free(res->observations);} // Free dynamically allocated observations string
        free(res->room_reserved);
        free(res);
        res=NULL;
    }
}

void show_reservation(const RESERVATION *res) {
    printf("Reservation Number: %d \nClient Code: %d \nType: %c \nRoom Reserved: %d \nCheck-In: %s \nCheck-Out: %s \nPeople: %d \nPayment Number: %d \nPayment Mode: %d \nSituation: %c \nObservations: %s \nCheck-Out Value: %.2f\n",
           res->num_reservation, res->client_cod, res->tipoFicha, res->room_reserved, res->dataCheckIn, res->dataCheckOut, res->numPessoas, res->payment_number, res->payment_mode, res->situation, res->observations, res->valorCheckOut);
}

// Function to initialize a FICHA structure
FICHA *initialize_ficha(int cod, CLIENT *client) {
    if (client == NULL) {
        printf("Client data is not available.\n");
        return NULL;
    }

    puts("test");

    FICHA *new_ficha = (FICHA *)ec_malloc(sizeof(FICHA));
    puts("aaaaaaa");
    if (new_ficha == NULL) {
        return NULL; // Return NULL if memory allocation fails
    }

    puts("test");

    new_ficha->cod = cod;
    new_ficha->client = client; // Assign the passed CLIENT pointer. Ensure it's properly allocated and initialized.
    new_ficha->data = time(NULL); // Set to current time
    new_ficha->state = 0; //provisional

    return new_ficha;
}


void set_ficha_definitiva(FICHA *fich){
    if (fich!=NULL){
        fich->state=1;
    }
    if (fich->client!=NULL){
        fich->client->type=1;
    }
}

void free_ficha(FICHA *ficha) {
    if (ficha == NULL) {
        return; // Nothing to free
    }

    if (ficha->client != NULL) {
        DestroyClient(ficha->client);
        ficha->client = NULL;
    } 

    // Finally, free the FICHA itself.
    free(ficha);
}

int add_extra_room(RESERVATION *res, char *room_code) {
    if (res == NULL) {
        printf("Reservation pointer is null.\n");
        return 0;
    }

    // Search for an empty slot in the extra_rooms array
    for (int i = 0; i < sizeof(res->extra_rooms) / sizeof(res->extra_rooms[0]); i++) {
        if (res->extra_rooms[i] == 0) { 
            res->extra_rooms[i] = room_code; // Add the room code
            return 1; // Success
        }
    }

    // If we reach this point, the array is full
    printf("Extra rooms array is full. Cannot add more rooms.\n");
    return 0; // Indicate failure
}

int remove_extra_room(RESERVATION *res, char *room_code) {
    if (res == NULL) {
        printf("Reservation pointer is null.\n");
        return 0;
    }

    int size = sizeof(res->extra_rooms) / sizeof(res->extra_rooms[0]);
    for (int i = 0; i < size; ++i) {
        if (res->extra_rooms[i] == room_code) { // Room code found
            // Shift subsequent entries to fill the gap
            for (int j = i; j < size - 1; ++j) {
                res->extra_rooms[j] = res->extra_rooms[j + 1];
            }
            res->extra_rooms[size - 1] = 0; // Set the last slot to 0 (unused)
            return 1; // Success
        }
    }

    // Room code not found
    printf("Room code %d not found in extra rooms.\n", room_code);
    return 0; // Indicate failure
}


void register_payment(RESERVATION *res, int payment_number, short int payment_mode, double amount) {
    if (res == NULL) {
        fatal("Invalid reservation reference.\n");
        return;
    }

    res->payment_number = payment_number;
    res->payment_mode = payment_mode;
    res->valorCheckOut = amount; // Assuming valorCheckOut represents the payment amount

    printf("Payment registered: Number %d, Mode %d, Amount %.2f\n", payment_number, payment_mode, amount);
}

void modify_payment(RESERVATION *res, int payment_number, short int new_payment_mode, double new_amount) {
    if (res == NULL || res->payment_number != payment_number) {
        printf("Invalid reservation or payment number.\n");
        return;
    }

    res->payment_mode = new_payment_mode;
    res->valorCheckOut = new_amount;

    printf("Payment modified: Number %d, New Mode %d, New Amount %.2f\n", payment_number, new_payment_mode, new_amount);
}

void cancel_payment(RESERVATION *res, int payment_number) {
    if (res == NULL || res->payment_number != payment_number) {
        printf("Invalid reservation or payment number.\n");
        return;
    }

    // Option 1: Set the amount to 0 to indicate cancellation
    res->valorCheckOut = 0;

    // Option 2: Introduce and use a status field (not present in the given structure)
    // res->payment_status = CANCELED; // Assuming such a field exists

    printf("Payment canceled: Number %d\n", payment_number);
}



