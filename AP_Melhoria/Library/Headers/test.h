#ifndef TEST_H_INCLUDED
#define TEST_H_INCLUDED

#include "services.h" // Assuming this file includes the definition of SERVICE
#include "utils.h"
#include "hotel.h"
#include "reservations.h"

ROOM* getRandomRoom(HOTEL *ht);
SERVICE *generate_random_service(); 
void simulate_room_booking(HOTEL *hotel, CLIENT* c ) ;
void simulate_service_request(HOTEL *hotel);
void simulate_check_in(HOTEL *hotel);
void simulate_check_out(HOTEL *hotel);
void simulate_payment_processing(HOTEL *hotel);



#endif // TEST_H_INCLUDED