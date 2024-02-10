/*******************************************************************************
 *  @file IO.h                                                                 *
 *  @brief Function prototypes to load ADTs from files.                        *
 *                                                                             *
 *  This contains the prototypes for loading clients, employees and products   *
 *  from input files.                                                          *
 *                                                                             *
 *******************************************************************************/

#ifndef IO_H_INCLUDED
#define IO_H_INCLUDED

/* -- Includes -- */
/* libc includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
/* clients header file */
#include "hotel.h"



/**********************************************************************************************************************************
 * @brief For testing purposes only... Use Unix flags for file permissions/modes                                                  *
 **********************************************************************************************************************************/
void writeToFile(char *b);

/**********************************************************************************************************************************
 * @brief                                                                                      *
 **********************************************************************************************************************************/
void save_hotel_to_binary_file(const char *filename, const HOTEL *hotel);
void save_reservation_to_file(FILE *file, const RESERVATION *res);
void save_ficha_to_file(FILE *file, const FICHA *ficha); 
void save_client_info_in_ficha(FILE *file, const CLIENT *client);
void save_service_done_to_file(FILE *file, const service_done *service);


void load_ficha_from_file(FILE *file, FICHA *ficha, CLIENT *clients);
void load_reservation_from_file(FILE *file, RESERVATION *res);
HOTEL *load_hotel_from_binary_file(const char *filename);
void load_service_done_from_file(FILE *file, service_done *service, int num_services);
void load_client_info_in_ficha(FILE *file, CLIENT *client_ptr);

#endif // IO_H_INCLUDED
