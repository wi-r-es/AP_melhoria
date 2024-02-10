#include "Headers/test.h"


SERVICE *generate_random_service() {
    char *types[] = {"SPA", "GYM", "BRK"}; // Example service types
    char *descriptions[] = {
        "Spa session",
        "Gym access",
        "Breakfast included"
    };

    int typeIndex = rand() % (sizeof(types) / sizeof(types[0]));
    double value = (double)(rand() % 100) + 50; // Random value between 50 and 150
 
    return create_service(types[typeIndex], "CODE", descriptions[typeIndex], value);
}

ROOM* getRandomRoom(HOTEL *ht){
    int t = getRandomInt(0, 1);
    int f = getRandomInt(0, 9);
    int rIndex = getRandomInt(0, 19);
    ROOM *r=ht->towers[t].floors[f].rooms[rIndex];
    //show_room(r);
    return r;
}


void simulate_room_booking(HOTEL *hotel, CLIENT* c) {
    printf("in : %s     !\n", __FUNCTION__);
    ROOM *room = NULL;
    //printf("TEST");
    int attempts = 0, maxAttempts = 400;
    /*do*/ 
    
    while (attempts < maxAttempts) { // Check if the room is available; if not, find an available one
        room = getRandomRoom(hotel);
        //show_room(room);
        if(room != NULL && room->availability == 1) {
            break; // Found an available room
            
        }
        attempts++;
    } //while(room == NULL || room->availability != 1);
    if (attempts >= maxAttempts) {
        printf("No available rooms found.\n");
        return; // Exit the function early
    }
    
    if (attempts == maxAttempts )
    {
        fatal("no room available");
    }
     
    //printf("TEST");
    // Mark the room as booked (changing its availability to 0)
    //alter_room(room, room->view, room->quality, room->capacity, 0);
    mark_room_as_reserved(hotel, room->code);

    printf("Room %s booked successfully.\n", room->code);

    // Select a random client
   // int clientIndex = getRandomInt(0, sizeof(carr) - 1); 
    CLIENT *client = c; 
    FICHA *ficha = NULL;
    // Create a Ficha for the client

    ficha = initialize_ficha(client->cod, client); // Assuming initialize_ficha requires client code and client pointer
    
    // Create a reservation
    // Assuming default values for some fields for simplicity. You should adjust according to your application logic.
    char *checkInDate = "2023-01-01"; // Example check-in date
    char *checkOutDate = "2023-01-05"; // Example check-out date
    RESERVATION *reservation = create_reservation(client->cod, ficha, 'P', room->code, 
                                                  checkInDate, checkOutDate, 1, 0, 1, "P", NULL, 0.0);
    if(reservation) {
        printf("Reservation created for Client %d in Room %s.\n", client->cod, room->code);
         
        add_reservation(hotel, reservation);
    } else {
        printf("Failed to create reservation.\n");
    }
    
}

void simulate_service_request(HOTEL *hotel) {
    printf("in : %s     !\n", __FUNCTION__);
    if(hotel->num_reservations ==1 ) return;
    //printf("jjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjj");

    int reservationIndex = (getRandomInt(1, hotel->num_reservations-1));
    RESERVATION *reservation = hotel->reservations[reservationIndex];
    SERVICE *service = generate_random_service();
    //printf("ttttttttttttttttttttttttttttttttttttttt");
    for (int i = 0; i < 5; i++) { // Assuming a max of 5 services per reservation for simplicity
        if (reservation->services[i] == NULL) { // Spot available for a new service
            reservation->services[i] = service;
            printf("Service '%s' requested for reservation %d.\n", service->description, reservation->num_reservation);
            return; // Successfully added service, exit function
        }
    }

    printf("Service '%s' requested for reservation %d.\n", service->description, reservation->num_reservation);

    // In a real scenario, you would also deallocate the service if it's no longer needed outside of the reservation
}






void simulate_check_in(HOTEL *hotel) {
    printf("in : %s     !\n", __FUNCTION__);
    if (hotel->num_reservations <=1) {
        printf("No reservations available for check-in.\n");
        return;
    }
    
    
    // Select a reservation at random to check in
    int reservationIndex = (getRandomInt(0, hotel->num_reservations-1));
    RESERVATION *reservation = hotel->reservations[reservationIndex];
    
    if (!reservation) {
        printf("Selected reservation is NULL. Aborting check-in process.\n");
    return;
    }

    // Check for NULL pointers in reservation structure
    if (!reservation->client_ficha || !reservation->client_ficha->client) {
        printf("Critical data in reservation is missing.\n");
        return;
    }

    // Mark the reservation as in operation ('O')
    reservation->situation[0] = 'O';
    reservation->client_ficha->state=1;
    reservation->client_ficha->client->type=1;
   // bool flag = false
    for(int i=0; i<5; i++){
        if(reservation->extra_rooms[i] != NULL){
            mark_room_as_reserved(hotel, reservation->extra_rooms[i]);
            //flag=true;
        }
    }
   // if(flag) 

    // Ensure the room associated with the reservation is marked as occupied
    mark_room_as_reserved(hotel, reservation->room_reserved);
    
    printf("Check-in completed for Reservation %d, Room: %s\n", reservation->num_reservation, reservation->room_reserved);
    
    // Optional: Add initial services requested at check-in if applicable
}


void simulate_check_out(HOTEL *hotel) {
    printf("in : %s     !\n", __FUNCTION__);
    // Select a reservation at random to check out
    // Only considering reservations that are active
    int activeReservations = 0;
    RESERVATION *reservation = NULL;
    for (int i = 0; i < hotel->num_reservations; i++) {
        if(hotel->reservations[i] != NULL){
            if (hotel->reservations[i]->situation[0] == 'O') {  // 'O' for Operational
                activeReservations++;
                reservation = hotel->reservations[i];
            }
        }
    }

    if (activeReservations == 0) {
        printf("No active reservations to check out.\n");
        return;
    }

    // Process check-out: calculate final bill, register any last-minute services, etc.
    // For simplicity, we'll just mark the reservation as completed ('F' for "Finished")
    reservation->situation[0] = 'F';
    verify_services_and_calculate_bill(reservation);
    update_room_states_on_checkout(hotel, reservation);


    printf("Check-out completed for reservation %d.\n", reservation->num_reservation);
}

void simulate_payment_processing(HOTEL *hotel) {
    printf("in : %s     !\n", __FUNCTION__);
    if(hotel->num_reservations<=1) return;
    RESERVATION *reservation = NULL;
    for(int i=0; i<hotel->num_reservations; i++){
        if(hotel->reservations[i] && hotel->reservations[i]->situation[0] == 'O'){
            reservation = hotel->reservations[i];
            break;
        }
    }
    
    if (!reservation) {
        printf("No reservation in operation found.....\n");
        return; // No applicable reservation found
    }

    double paymentAmount = reservation->valorCheckOut;

    
    // Process the payment
    printf("Payment processed for reservation %d: %.2f, \tpayed using: &d\n", reservation->num_reservation, paymentAmount, reservation->payment_mode);
    return;
    // In a real scenario, you would update the reservation's financial details, mark payments as completed, etc.
}

