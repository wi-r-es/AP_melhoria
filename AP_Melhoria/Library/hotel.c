#include "Headers/hotel.h"

extern void *ec_malloc(unsigned int size);
extern void fatal(char *message);
extern int getRandomInt(int min, int max);

ROOM *create_room(const char *code, char view, char quality, char capacity) {
    ROOM *new_room = (ROOM *)ec_malloc(sizeof(ROOM));
    if (new_room == NULL) {
        fatal("in create_room(), allocation for ROOM failed...");
        return NULL;
    }

    new_room->code = strdup(code);
    //new_room->code[sizeof(new_room->code) - 1] = '\0'; // Ensure null-termination
    new_room->view = view;
    new_room->quality = quality;
    new_room->capacity = capacity;
    new_room->availability = 1;

    return new_room;
}

void alter_room(ROOM *room, char view, char quality, char capacity, int availability) {
    if (room != NULL) {
        room->view = view;
        room->quality = quality;
        room->capacity = capacity;
        room->availability = availability;
    }
}

void destroy_room(ROOM *room) {
    if (room != NULL) {
        free(room->code);
        free(room);
        room = NULL;
    }
}


void show_room(ROOM *room) {
    if (room != NULL) {
        printf("\n     [*]<%s>[*]\n", __FUNCTION__);
        printf("\t[ ]Code: [%s]\n", room->code);
        printf("\t[ ]View: [%c]\n", room->view); // 'M' for Mar, 'P' for Piscina
        printf("\t[ ]Quality: [%c]\n", room->quality); // 'B' for Base, 'S' for Superior
        printf("\t[ ]Capacity: [%c]\n", room->capacity); // Assumes single character representation
        printf("\t[ ]Availability: [%s]\n", room->availability==1 ? "AVAILABLE" : "OCCUPIED" ); // 0 Used, 1 Free

        printf("\n\n\t[i] VALUE IN AVAILABILITY: [%d]\n\n", room->availability);
    }
}

HOTEL *initialize_hotel(char *_name) {
    size_t size = sizeof(HOTEL);
    HOTEL *hotel = (HOTEL *)ec_malloc(size);
    
    if (!hotel)
    {
        fatal("in initialize_hotel(), allocation for hotel failed... ");
        return NULL;
    }
    //hotel->name = "MY HOTEL BAY HOTEL";
    
    hotel->name = (char *)ec_malloc(strlen(_name) + 1);  //could jusrt be done with strdup()
    if (!hotel->name)
    {
        fatal("in initialize_hotel(), allocation for name in hotel failed... ");
        free(hotel);
        return NULL;
    }
    hotel->name = strcpy(hotel->name ,_name); 
    //rooms occupied plus reservations
    hotel->rooms_occupied =0;
    hotel->num_reservations=1;
    hotel->num_services=0;
    hotel->num_fichas=0;
    hotel->capacity_reservations = 1;
    
    char capacities[] = {'S', 'D', 'T', 'F'}; // Array to map capacity
    
    for (int t = 0; t < TOWERS_IN_HOTEL; ++t) {
        for (int f = 0; f < FLOORS_PER_TOWER; ++f) {
            for (int r = 0; r < ROOMS_PER_FLOOR; ++r) {
                char room_code[10];
                sprintf(room_code, "%02d%02d%02d", t+1, f+1, r+1);
                char v = (r % 2 == 0) ? 'M' : 'P'; // Alternating views
                char q = (r % 2 == 0) ? 'B' : 'S'; // Alternating quality
                int capIndex = getRandomInt(1, 4) - 1; //  0-based for array indexing
                hotel->towers[t].floors[f].rooms[r] = create_room(room_code,v,q,capacities[capIndex]);
            }
        }
    }
    hotel->reservations = (RESERVATION **)malloc(hotel->capacity_reservations * sizeof(RESERVATION *));
    if (!hotel->reservations) {
        perror("Failed to allocate reservations");
        free(hotel);
        return NULL;
    }
    return hotel;
}


void Load_reservations(HOTEL *hotel) {
    for (int i = 0; i < hotel->num_reservations; ++i) {
        // Allocate memory for each reservation if not already allocated
        if (hotel->reservations[i] == NULL) {
            hotel->reservations[i] = (RESERVATION *)ec_malloc(sizeof(RESERVATION));
            if (hotel->reservations[i] == NULL) {
                perror("Failed to allocate memory for reservation");
                //exit(EXIT_FAILURE); // Or handle the error as appropriate
                return;
            }
            memset(hotel->reservations[i], 0, sizeof(RESERVATION)); // Zero-initialize the new reservation
        }

        RESERVATION *res = hotel->reservations[i]; // Directly use the pointer
        res->num_reservation = i; // Example sequential initialization
        res->client_cod = 0; // Assuming uninitialized
        res->tipoFicha = 'P'; // Default to 'P' - Provisória
        res->room_reserved = NULL; // Assuming uninitialized
        // Ensure these are properly initialized as strings, even if just empty
        strcpy(res->dataCheckIn, ""); // Zeroed dates
        strcpy(res->dataCheckOut, "");
        res->numPessoas = 1; // Default to 1 person
        // Initialize services and extra_rooms as needed
        // Assuming services and extra_rooms are properly defined in your RESERVATION structure
        // memset(res->services, 0, sizeof(res->services)); // Example zero-initialization
        // memset(res->extra_rooms, 0, sizeof(res->extra_rooms));
        res->payment_number = 0; // Assuming uninitialized
        res->payment_mode = 0; // Default to 0 - dinheiro
        res->situation[0] = 'P'; // Default to 'P' - provisoria
        // For dynamically allocated fields like observations, ensure they're properly handled
        res->observations = NULL; // No observations initially
        res->valorCheckOut = 0.0; // Assuming uninitialized
    }
}

// Helper function to check if a pointer is not NULL
int is_not_null(const void *ptr) {
    return ptr != NULL;
}
void show_hotel(HOTEL *hotel) {
    if (!hotel) {
        printf("Hotel data is not available.\n");
        return;
    }

    printf("Hotel Name: %s\n", hotel->name);
    printf("Rooms Occupied: %d\n", hotel->rooms_occupied);
    printf("Number of Reservations: %d\n", hotel->num_reservations);

    // Check if the towers field is not NULL
    if (hotel->towers == NULL) {
        printf("No towers found in the hotel.\n");
        return;
    }
    // Iterate through each tower
    for (int t = 0; t < TOWERS_IN_HOTEL; ++t) {
        printf("\nTower %d:\n", t + 1);
        // Check if the floors field is not NULL
        if (hotel->towers[t].floors == NULL) {
            printf("No floors found in tower %d.\n", t + 1);
            continue;
        }

        // Iterate through each floor
        for (int f = 0; f < FLOORS_PER_TOWER; ++f) {
            int floorOccupied = 0; // To count occupied rooms per floor
            // Check if the rooms array is not NULL
            if (hotel->towers[t].floors[f].rooms == NULL) {
                printf("No rooms found on floor %d of tower %d.\n", f + 1, t + 1);
                continue;
            }

            // Iterate through rooms on the floor
            for (int r = 0; r < ROOMS_PER_FLOOR; ++r) {
                // Use a helper function to check if a pointer is not NULL
                if (is_not_null(&hotel->towers[t].floors[f].rooms[r])) {
                    if (hotel->towers[t].floors[f].rooms[r]->availability == 1) {
                        floorOccupied++;
                    }
                    show_room(hotel->towers[t].floors[f].rooms[r]);
                }
            }
            printf("Floor %d of tower %d: %d rooms occupied\n", f + 1, t + 1, floorOccupied);
        }
    }

    // Optionally, summarize reservations
    if (hotel->num_reservations != 1) {
        printf("\nReservations Summary:\n");
        for (int i = 0; i < hotel->num_reservations; ++i) {
            const RESERVATION *res = hotel->reservations[i];
            if (is_not_null(res)) {
                printf("Reservation %d: Room %d, Check-in: %s, Check-out: %s, Status: %c\n",
                       res->num_reservation, res->room_reserved, res->dataCheckIn, res->dataCheckOut, res->situation);
                // Add more details as necessary
            }
        }
    }
}




void destroy_hotel(HOTEL *hotel) {
    if (hotel == NULL) {
        return; // No need to free if the hotel pointer is NULL
    }
    // Free the hotel's name
    free(hotel->name);
    hotel->name = NULL; // Prevent dangling pointer by setting to NULL after freeing
    // Free any dynamically allocated fields within each reservation

    // Free reservations
    if (hotel->reservations) {
        for (int i = 0; i < hotel->num_reservations; ++i) {
            if (hotel->reservations[i]) {
                free(hotel->reservations[i]->observations);
                free(hotel->reservations[i]->room_reserved);
               
                free(hotel->reservations[i]);
            }
        }
        free(hotel->reservations);
    }
    //free(hotel->reservations);
    for (int i = 0; i < hotel->num_services; ++i) {
        destroy_service_done(hotel->services[i]);
    }

    for (int i = 0; i < hotel->num_fichas; ++i) {
        free(hotel->fichas[i]);
    }
    //Free the
    // Iterate through each tower
    for (int t = 0; t < TOWERS_IN_HOTEL; ++t) {
        printf("\nTower %d:\n", t + 1);
        // Iterate through each floor
        for (int f = 0; f < FLOORS_PER_TOWER; ++f) {
            int floorOccupied = 0; // To count occupied rooms per floor
            // Iterate through rooms on the floor
            for (int r = 0; r < ROOMS_PER_FLOOR; ++r) {
                destroy_room(hotel->towers[t].floors[f].rooms[r]);
            }
            printf("Floor %d: %d rooms occupied\n", f + 1, floorOccupied);
        }
    }
    // Free the reservations array
    free(hotel);
    hotel=NULL;
}

void fix_fichas_array(HOTEL *hotel, int deletedIndex) {
    if (hotel == NULL || deletedIndex < 0 || deletedIndex >= hotel->num_fichas) {
        printf("Invalid parameters for fix_fichas_array.\n");
        return;
    }

    // Shift elements to "fill" the gap left by the deleted ficha
    for (int i = deletedIndex; i < hotel->num_fichas - 1; i++) {
        hotel->fichas[i] = hotel->fichas[i + 1];
    }

    // Set the last element to NULL as it's now a duplicate after the shift
    hotel->fichas[hotel->num_fichas - 1] = NULL;

    // Decrement the count of fichas
    hotel->num_fichas--;
}

void delete_provisional_client(HOTEL *hotel, int client_cod) {
    for (int i = 0; i < hotel->num_fichas; ++i) {
        if (hotel->fichas[i]->client->cod == client_cod && hotel->fichas[i]->state==0) {
            // Free the client and person data

            DestroyClient(hotel->fichas[i]->client);
            fix_fichas_array(hotel, i);
            printf("Provisional client with cod %d deleted successfully.\n", client_cod);
            
            return; 
        }
    }
    printf("Client with cod %d not found or not provisional.\n", client_cod);
}

void modify_provisional_client(HOTEL *hotel, int client_cod, const char *new_name, const char *new_address, const char *new_postal_code, const char *new_locality, const char *new_NIF) {
    for (int i = 0; i < hotel->num_fichas; ++i) {
        if (hotel->fichas[i]->client->cod == client_cod && hotel->fichas[i]->state==0) {
            // Use update_person_details to change client's personal information
            update_person_details(hotel->fichas[i]->client, new_name, new_address, new_postal_code, new_locality, new_NIF);
            
            
            printf("Client with cod %d updated successfully.\n", client_cod);
            return;
        }
    }
    printf("Client with cod %d not found or not provisional.\n", client_cod);
}


void modify_reservation(HOTEL *hotel, int num_reservation, int new_client_cod, char new_tipoFicha, 
                        char *new_room_reserved, const char *new_dataCheckIn, const char *new_dataCheckOut, 
                        int new_numPessoas, int new_payment_number, short int new_payment_mode, 
                        char new_situation[2], const char *new_observations, double new_valorCheckOut) {

    for (int i = 0; i < hotel->num_reservations; ++i) {
        if (hotel->reservations[i] && hotel->reservations[i]->num_reservation == num_reservation) {
            RESERVATION *res = hotel->reservations[i];
            
            // Perform checks before updating each field
            if (new_client_cod != 0) res->client_cod = new_client_cod;
            if (new_tipoFicha=='P' && new_tipoFicha=='D') res->tipoFicha = new_tipoFicha; // Assuming '\0' is not a valid tipoFicha
            if (new_room_reserved != NULL ) res->room_reserved = new_room_reserved ? strdup(new_room_reserved) : NULL;
            
            if (new_dataCheckIn && new_dataCheckIn[0] != '\0') { // Check if not NULL and not empty string
                strncpy(res->dataCheckIn, new_dataCheckIn, sizeof(res->dataCheckIn) - 1);
                res->dataCheckIn[sizeof(res->dataCheckIn) - 1] = '\0';
            }
            
            if (new_dataCheckOut && new_dataCheckOut[0] != '\0') {
                strncpy(res->dataCheckOut, new_dataCheckOut, sizeof(res->dataCheckOut) - 1);
                res->dataCheckOut[sizeof(res->dataCheckOut) - 1] = '\0';
            }
            
            if (new_numPessoas != 0) res->numPessoas = new_numPessoas;
            if (new_payment_number != 0) res->payment_number = new_payment_number;
            if (new_payment_mode != 0) res->payment_mode = new_payment_mode;
            if (new_situation[0] != '\0') res->situation[0] = new_situation[0]; // Assuming '\0' is not a valid situation
            
            if (new_observations && new_observations[0] != '\0') { // Check if not NULL and not empty string
                if (res->observations) free(res->observations); // Free existing observations if any
                res->observations = strdup(new_observations);
            } else if (new_observations == NULL && res->observations != NULL) {
                // If explicitly set to NULL, clear existing observations
                free(res->observations);
                res->observations = NULL;
            }
            
            if (new_valorCheckOut != 0.0) res->valorCheckOut = new_valorCheckOut;
            
            printf("Reservation %d updated successfully.\n", num_reservation);
            return;
        }
    }
    printf("Reservation %d not found.\n", num_reservation);
}



void delete_reservation(HOTEL *hotel, int num_reservation) {
    for (int i = 0; i < hotel->num_reservations; i++) {
        if (hotel->reservations[i]->num_reservation == num_reservation) {
            // Free reservation
            destroy_reservation(hotel->reservations[i]);
            
            // Shift the rest of the reservations to fill the gap
            for (int j = i; j < hotel->num_reservations - 1; j++) {
                hotel->reservations[j] = hotel->reservations[j + 1];
            }
            hotel->reservations[hotel->num_reservations - 1] = NULL; // Avoid dangling pointer
            hotel->num_reservations--; // Decrement total number of reservations

            printf("Reservation %d deleted.\n", num_reservation);
            return;
        }
    }
    printf("Reservation %d not found.\n", num_reservation);
}

void mark_room_as_reserved(HOTEL *hotel, char *room_code) {
    if(room_code != NULL) {
        for (int t = 0; t < TOWERS_IN_HOTEL; t++) {
            for (int f = 0; f < FLOORS_PER_TOWER; f++) {
                for (int r = 0; r < ROOMS_PER_FLOOR; r++) {
                    if (strcmp(hotel->towers[t].floors[f].rooms[r]->code, room_code)==0) {
                        hotel->towers[t].floors[f].rooms[r]->availability = 0; // Mark as reserved
                        return;
                    }
                }
            }
        }
    }
    printf("Room %s not found.\n", room_code);
}

void mark_room_as_available(HOTEL *hotel, char *room_code) {
    for (int t = 0; t < TOWERS_IN_HOTEL; t++) {
        for (int f = 0; f < FLOORS_PER_TOWER; f++) {
            for (int r = 0; r < ROOMS_PER_FLOOR; r++) {
                if (strcmp(hotel->towers[t].floors[f].rooms[r]->code,room_code)==0) {
                    hotel->towers[t].floors[f].rooms[r]->availability = 1; // Mark as available
                    return;
                }
            }
        }
    }
    printf("Room %s not found.\n", room_code);
}

void update_reservation_rooms(HOTEL *hotel, RESERVATION *res) {
    mark_room_as_reserved(hotel, res->room_reserved);
    for (int i = 0; i < sizeof(res->extra_rooms) / sizeof(int); i++) {
        if (res->extra_rooms[i] != NULL) { 
            mark_room_as_reserved(hotel, res->extra_rooms[i]);
        }
    }
}

void free_reservation_rooms(HOTEL *hotel, RESERVATION *res) {
    mark_room_as_available(hotel, res->room_reserved);
    for (int i = 0; i < sizeof(res->extra_rooms) / sizeof(int); i++) {
        if (res->extra_rooms[i] != NULL) {
            mark_room_as_available(hotel, res->extra_rooms[i]);
        }
    }
}

void register_subclient(HOTEL *hotel, int main_reservation_num, CLIENT *subclient, char *subclient_room) {
    // Find the main reservation
    for (int i = 0; i < hotel->num_reservations; i++) {
        if (hotel->reservations[i]->num_reservation == main_reservation_num) {
            RESERVATION *res = hotel->reservations[i];
            CLIENT *prime_c= res->client_ficha->client;

            printf("Subclient %d registered to reservation %d.\n", subclient->cod, main_reservation_num);
            return;
        }
    }
    printf("Main reservation not found.\n");
}
subclient *create_subclient(person *_client, CLIENT *_prime, int _prime_room_code);

void update_room_states_on_checkin(HOTEL *hotel, RESERVATION *reservation) {
    mark_room_as_reserved(hotel, reservation->room_reserved);
    for (int i = 0; i < sizeof(reservation->extra_rooms) / sizeof(reservation->extra_rooms[0]); i++) {
        if (reservation->extra_rooms[i] != NULL) { // Assuming 0 indicates no room
            mark_room_as_reserved(hotel, reservation->extra_rooms[i]);
        }
    }
    // Update the reservation situation to 'O' for "in operation" or checked in
    reservation->situation[0] = 'O';
    printf("Rooms and reservation updated for check-in.\n");
}
// opt =0 register paymentm, =1 modify, =2 cancel
void register_payment_on_checkin(HOTEL *hotel, RESERVATION *reservation, int opt, 
                                 int payment_number, short int payment_mode, double amount) {
    switch (opt) {
        case 0: // Register a new payment
            register_payment(reservation, payment_number, payment_mode, amount);
            break;
        case 1: // Modify an existing payment
            modify_payment(reservation, payment_number, payment_mode, amount); 
            break;
        case 2: // Cancel an existing payment
            cancel_payment(reservation, payment_number);
            break;
        default:
            fatal("Invalid option");
            break;
    }
}


void add_reservation(HOTEL *hotel, RESERVATION *reservation) {
    if (hotel->num_reservations == hotel->capacity_reservations) {
        // Need to reallocate
        int new_capacity = hotel->capacity_reservations +2; // Double the capacity
        RESERVATION **new_reservations = (RESERVATION **)realloc(hotel->reservations, new_capacity * sizeof(RESERVATION *));
        if (!new_reservations) {
            perror("Failed to reallocate reservations");
            return;
        }

        hotel->reservations = new_reservations;
        hotel->capacity_reservations = new_capacity;
    }

    // Add the new reservation
    hotel->reservations[hotel->num_reservations] = reservation;
    hotel->num_reservations++;

    printf("Reservation added. Total reservations: %d.\n", hotel->num_reservations);
}

// Function to add a new client to the list
int addClient(clients_struct *cs, CLIENT *newClientPtr) {
    static int numberOfClients = 0; // Keeps track of the number of clients
    
    // Check if this is the first client being added
    if (cs->list == NULL) {
        // Allocate memory for one CLIENT
        cs->list = (CLIENT*)malloc(sizeof(CLIENT));
        if (cs->list == NULL) { // Check for allocation failure
            return -1; // Return error code
        }
        numberOfClients = 1;
    } else {
        // If not the first client, resize the list to accommodate one more CLIENT
        CLIENT *temp = (CLIENT*)realloc(cs->list, (numberOfClients + 1) * sizeof(CLIENT));
        if (temp == NULL) {
            return -1; // Return error code if reallocation failed
        }
        cs->list = temp;
        numberOfClients++;
    }

    // Copy the new client data into the list using the pointer
    cs->list[numberOfClients - 1] = *newClientPtr; // Dereference the pointer to copy the client

    return 0; // Return 0 on success
}


void register_card_return(RESERVATION *reservation, int card_number, int is_returned) {

    printf("Card %d return status updated for reservation %d.\n", card_number, reservation->num_reservation);

    if (!is_returned) {
        // Apply a charge for the unreturned card
        double charge_amount = 50.00; // Example charge amount
        reservation->valorCheckOut += charge_amount; // Add the charge to the total bill
        printf("Charge of %.2f applied for unreturned card %d.\n", charge_amount, card_number);
    }
}

void verify_services_and_calculate_bill(RESERVATION *reservation) {
    double total_amount = reservation->valorCheckOut; // Start with the current bill amount

    // Add costs of all registered services to the total bill
    for (int i = 0; i < sizeof(reservation->services) / sizeof(reservation->services[0]); i++) {
        if (reservation->services[i] != NULL ) { 
            total_amount += reservation->services[i]->value;
            printf("Service '%s' cost %.2f added to bill.\n", reservation->services[i]->description, reservation->services[i]->value);
        }
    }

    printf("Total bill for reservation %d is %.2f.\n", reservation->num_reservation, total_amount);
    reservation->valorCheckOut = total_amount; // Update the total bill amount
}

void update_room_states_on_checkout(HOTEL *hotel, RESERVATION *reservation) {
    mark_room_as_available(hotel, reservation->room_reserved);
    for (int i = 0; i < sizeof(reservation->extra_rooms) / sizeof(reservation->extra_rooms[0]); i++) {
        if (reservation->extra_rooms[i] != NULL) { 
            mark_room_as_available(hotel, reservation->extra_rooms[i]);
        }
    }
    // Update the reservation situation to 'F' for "finalizada" or checked out
    reservation->situation[0] = 'F';
    printf("Rooms updated for check-out; reservation %d finalized.\n", reservation->num_reservation);
}

void calculate_room_availability(HOTEL *hotel, const char* start_date, const char* end_date) {
    int total_rooms = 0, total_free_rooms = 0, total_occupied_rooms = 0;

    for (int t = 0; t < TOWERS_IN_HOTEL; t++) {
        for (int f = 0; f < FLOORS_PER_TOWER; f++) {
            int free_rooms = 0, occupied_rooms = 0;

            for (int r = 0; r < ROOMS_PER_FLOOR; r++) {
                total_rooms++;
                if (hotel->towers[t].floors[f].rooms[r]->availability == 1) {
                    free_rooms++;
                    total_free_rooms++;
                } else {
                    occupied_rooms++;
                    total_occupied_rooms++;
                }
            }

            printf("Tower %d, Floor %d: Free Rooms: %d, Occupied Rooms: %d\n", t+1, f+1, free_rooms, occupied_rooms);
        }
    }

    printf("Total Rooms: %d, Total Free Rooms: %d (%.2f%%), Total Occupied Rooms: %d (%.2f%%)\n",
           total_rooms, total_free_rooms, (total_free_rooms * 100.0) / total_rooms,
           total_occupied_rooms, (total_occupied_rooms * 100.0) / total_rooms);
}


bool is_room_reserved_on_date(HOTEL* hotel, char *room_code, const char* date) {
    for (int i = 0; i < hotel->num_reservations; i++) {
        RESERVATION* res = hotel->reservations[i];
        if ((res->room_reserved == room_code || additional_rooms_contains(res->extra_rooms, room_code)) &&
            is_date_within_range(date, res->dataCheckIn, res->dataCheckOut)) {
            return true; // Room is reserved on this date
        }
    }
    return false; // Room is not reserved on this date
}

// Helper function to check if the room_code is in the extra_rooms array
bool additional_rooms_contains(char** extra_rooms, char *room_code) {
    for (int i = 0; i < 5; i++) { // Assuming 5 extra rooms as per the RESERVATION struct
        if (strcmp(*extra_rooms , room_code)==0) {
            return true;
        }
        extra_rooms++;
    }
    return false;
}

void show_occupancy_map_for_room(HOTEL* hotel, char *room_code, const char* start_date, const char* end_date) {
    int size = 0;
    char** dates = get_days_range(start_date, end_date, &size);
    if (dates == NULL) {
        printf("Could not generate date range.\n");
        return;
    }

    printf("Occupancy Map for Room %d from %s to %s:\n", room_code, start_date, end_date);
    for (int i = 0; i < size; i++) {
        bool reserved = is_room_reserved_on_date(hotel, room_code, dates[i]);
        printf("%s: %s\n", dates[i], reserved ? "Occupied" : "Free");
    }

    // Cleanup dynamically allocated dates array
    for (int i = 0; i < size; i++) {
        free(dates[i]);
    }
    free(dates);
}


void sort_reservations_by_room_code(RESERVATION **reservations, int count) {
    bool swapped;
    do {
        swapped = false;
        for (int i = 0; i < count - 1; ++i) {
            if (reservations[i]->room_reserved > reservations[i + 1]->room_reserved) {
                RESERVATION *temp = reservations[i];
                reservations[i] = reservations[i + 1];
                reservations[i + 1] = temp;
                swapped = true;
            }
        }
    } while (swapped);
}


void list_reservations_by_checkin(HOTEL *hotel, const char *start_date, const char *end_date) {
    RESERVATION *filtered[hotel->num_reservations]; // Temporary array for filtered reservations
    int count = 0;

    for (int i = 0; i < hotel->num_reservations; ++i) {
        if (is_date_within_range(hotel->reservations[i]->dataCheckIn, start_date, end_date)) {
            filtered[count++] = hotel->reservations[i];
        }
    }

    // Sort the filtered reservations by room code
    sort_reservations_by_room_code(filtered, count);

    // Print sorted and filtered reservations
    for (int i = 0; i < count; ++i) {
        printf("Reservation %d for Room %d (Check-in: %s)\n",
               filtered[i]->num_reservation, filtered[i]->room_reserved, filtered[i]->dataCheckIn);
    }
}


void list_reservations_by_checkout(HOTEL *hotel, const char *start_date, const char *end_date) {
    RESERVATION *filtered[hotel->num_reservations]; // Temporary array for filtered reservations
    int count = 0;

    for (int i = 0; i < hotel->num_reservations; ++i) {
        if (is_date_within_range(hotel->reservations[i]->dataCheckOut, start_date, end_date)) {
            filtered[count++] = hotel->reservations[i];
        }
    }

    // Sort the filtered reservations by room code
    sort_reservations_by_room_code(filtered, count);

    // Print sorted and filtered reservations
    for (int i = 0; i < count; ++i) {
        printf("Reservation %d for Room %d (Check-in: %s)\n",
               filtered[i]->num_reservation, filtered[i]->room_reserved, filtered[i]->dataCheckOut);
    }
}



void generate_payments_file(HOTEL *hotel, const char *start_date, const char *end_date) {
    char filename[255];
    snprintf(filename, sizeof(filename), "payments_%s_to_%s.txt", start_date, end_date);

    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Failed to open file for writing");
        return;
    }

    // Write the header
    fprintf(file, "Reservation Number, Room Code, Payment Number, Payment Mode, Amount, Date\n");

        for (int i = 0; i < hotel->num_reservations; i++) {
        RESERVATION *res = hotel->reservations[i];
        // Assuming you have a way to convert the reservation's check-in or check-out date to a comparable format
        // and a function to check if the date is within the range.
        if (is_date_within_range(res->dataCheckIn, start_date, end_date) ||
            is_date_within_range(res->dataCheckOut, start_date, end_date)) {
            // Write the payment details to the file
            fprintf(file, "%d, %d, %d, %d, %.2f, %s\n",
                    res->num_reservation,
                    res->room_reserved,
                    res->payment_number,
                    res->payment_mode,
                    res->valorCheckOut,
                    res->dataCheckIn); // Assuming payment date is the same as check-in for simplicity
        }
    }

    fclose(file);
    printf("Payment report generated: %s\n", filename);
}



ROOM *find_room_by_code(HOTEL *hotel, char *roomCode) {
    if (!hotel || !roomCode) return NULL; // Safety check

    // Iterate through each tower
    for (int t = 0; t < TOWERS_IN_HOTEL; ++t) {
        // Iterate through each floor in the tower
        for (int f = 0; f < FLOORS_PER_TOWER; ++f) {
            // Iterate through each room on the floor
            for (int r = 0; r < ROOMS_PER_FLOOR; ++r) {
                ROOM *currentRoom = hotel->towers[t].floors[f].rooms[r];
                // Check if the current room's code matches the search code
                if (strcmp(currentRoom->code, roomCode) == 0) {
                    return currentRoom; // Room found, return a pointer to it
                }
            }
        }
    }
    
    return NULL; // Room not found
}

