/**********************************************************************************
 *  @file utils.h                                                                 *
 *  @brief Function prototypes of useful functions used across the program.       *
 *                                                                                *
 **********************************************************************************/


#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED
/* -- Includes -- */
/* libc includes. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <locale.h>
#include "data.h"



/*********************************************************************
 * @brief Pauses the execution for the specified number of seconds.  *
 *********************************************************************/
void Wait(int s);
/*********************************************************************
 * @brief Pauses the execution for the specified number of seconds.  *
 *********************************************************************/
void WaitSegundos(int s);

/*********************************************************************************
 * @brief Appends a log entry to a data file.                                    *
 *                                                                               *
 *       It takes the data file path (datafile),                                 *
 *       the name of the executing function (funcname),                          *
 *       and a brief description of the log message (info).                      *
 *       If the datafile is NULL, it uses a default file name "default.csv".     *
 *       The function opens the file in append mode using fopen(),               *
 *       writes the log entry in the format date,time,function,brief,            *
 *       and then closes the file using fclose().                                *
 *                                                                               *
 * @param datafile : String representing the datafile                            *
 * @param funcname : String represening the name of the funciton                 *
 * @param info : String representing the descriptions of the log message         *
 * @return None.                                                                 *
 *********************************************************************************/
void logging(char *datafile, const char *funcname, char *info);

// A function to display an error message and then exit
/****************************************************************************************
 * @brief Displays an error message and terminates the program.(NOT ANYMORE FOR NOW)    *
 *                                                                                      *
 * @param message : String representing the error message to be printed                 *
 * @see perror()                                                                        *
 * @return None.                                                                        *
 ****************************************************************************************/
void fatal(char *message);


// Error-check malloc
/********************************************************************************************************************
 * @brief An error-checking wrapper for the malloc() function.                                                      *
 *                                                                                                                  *
 *        It attempts to allocate memory of the specified size (size) using malloc().                               *
 *        If the memory allocation fails, it calls the fatal() function with an error message and returns NULL.     *
 *        If the allocation is successful, it returns a pointer to the allocated memory.                            *
 *                                                                                                                  *
 *                                                                                                                  *
 * @param size : size to be allocated                                                                               *
 * @see fatal()                                                                                                     *
 * @return Pointer to the allocated memory if allocation was successful, NULL otherwise.                            *
 ********************************************************************************************************************/
void *ec_malloc(unsigned int size);


// Devolve um numero aleatorio entre [min, max]
// Gera um numero aleatorio
/********************************************************************
 * @brief Generates a random integer within the range [min, max].   *
 *                                                                  *
 * @param min : minimum number to be generated                      *
 * @param max : maximum number to be generated                      *
 * @see rand()                                                      *
 * @return Random integer within the range.                         *
 ********************************************************************/
int ger_rand(int min, int max); /**< not random enough */

// new ger random since other was bias
/********************************************************************************************************************
 * @brief Generates a random integer within the range [min, max].                                                   *
 *                                                                                                                  *
 *        It calculates the range size and generates a random offset within that range using rand().                *
 *        The random offset is added to the minimum value to obtain a random number within the specified range.     *
 *        This function is an improved version that avoids bias compared to the previous implementation.            *
 *                                                                                                                  *
 * @param min : minimum number to be generated                                                                      *
 * @param max : maximum number to be generated                                                                      *
 * @see rand()                                                                                                      *
 * @return Random integer within the range.                                                                         *
 ********************************************************************************************************************/
int getRandomInt(int min, int max);

/******************************************************************************************************
 * @brief Prints a formatted string surrounded by a decorative frame.                                 *
 *                                                                                                    *
 *       It calculates the padding required to center the string in a frame of width 50 characters.   *
 *       It prints a top frame line consisting of '#' characters.                                     *
 *       It prints the padded string in the center of the frame.                                      *
 *       It prints a bottom frame line consisting of '#' characters.                                  *
 *       The resulting frame visually separates the string from the surrounding content.              *
 *                                                                                                    *
 * @param str : string to be printed withing the decorative frame                                     *
 * @return None.                                                                                      *
 ******************************************************************************************************/
void beautify(const char *str);


void WriteTable1ToFile();
void WriteTable2ToFile();
void WriteTable3ToFile();


char *generate_random_postal_code();
char *generate_random_nif(); 

bool isValidString(const char *str);
bool isValidClientType(char type);
bool isValidNIF(const char *nif); 
bool isValidLength(const char *str, size_t expectedLength);
bool isValidCard(const char *card);

bool is_date_within_range(const char* date, const char* start_date, const char* end_date);
void parse_date(const char* date_str, struct tm* date_tm);
char** get_days_range(const char* start_date, const char* end_date, int* size);




#endif // UTILS_H_INCLUDED
