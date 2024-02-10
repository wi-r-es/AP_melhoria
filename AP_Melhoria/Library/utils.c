#include "Headers/utils.h"

/*
#ifdef _WIN32
#else
#include <sys/ioctl.h>
#include <termios.h>
#endif
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
*/

#ifdef _WIN32
char *logging_file = "Files\\O\\logs.csv";
char *table1_file = "Files\\I\\table1.txt";
char *table2_file = "Files\\I\\table2.txt";
char *table3_file = "Files\\I\\table3.txt";
#else
char *logging_file = "Files/O/logs.csv";
char *table1_file = "Files/I/table1.txt";
char *table2_file = "Files/I/table2.txt";
char *table3_file = "Files/I/table3.txt";
#endif

void WaitSegundos(int s)
{
    clock_t T0 = clock();
    clock_t T1 = T0 + s;
    while (clock() < T1);
}
void Wait(int s)
{
    WaitSegundos(s * CLOCKS_PER_SEC);
}

// #################################################################

// Logging function
void logging(char *datafile, const char *funcname, char *info)
{
    if (!funcname || !info)
        return;
    if (datafile == NULL)
        datafile = "default.csv";
    // printf("%s",datafile);
    FILE *F = fopen(datafile, "a");
    if (!F)
        return;
    fprintf(F, "%s,%s,[EXECUTING_FUNCTION]: %s,[SHORT-BRIEF]: %s\n", __DATE__, __TIME__, funcname, info);
    fclose(F);
}


// A function to display an error message and then exit
void fatal(char *message)
{
    char error_message[100];

    strcpy(error_message, "[!!!] Fatal Error ");
    strncat(error_message, message, 83);
    perror(error_message);
    // abort();
    // exit(-1);
}
void *ec_malloc(unsigned int size)
{
    void *ptr;
    ptr = malloc(size);
    // ptr=NULL;
    if (ptr == NULL)
    {
        fatal("in ec_malloc() on memory allocation");
        return NULL;
    }

    return ptr;
}

// Devolve um numero aleatorio entre [min, max]
// Gera um numero aleatorio
int ger_rand(int min, int max)
{
    return min + rand() % (max - min + 1);
}
// new ger random since other was was bias
int getRandomInt(int min, int max)
{
    // Seed the random number generator with the current time
    // Calculate the range size and generate a random offset
    int range = max - min + 1;
    int offset = rand() % range;

    // Return the random number within the range
    return min + offset;
}

void beautify(const char *str)
{
    int length = strlen(str);
    int padding = (50 - length) / 2;
    printf("##################################################\n");
    for (int i = 0; i < padding; i++)
        printf("#");
    printf("%s", str);
    // printf("%*s%s%*s\n", padding, "", str,padding, "" );
    for (int i = 0; i < padding; i++)
        printf("#");
    printf("\n");
    printf("##################################################\n");
}


void WriteTable1ToFile(){
    const char *dados[] = {
        "M;B;S;50;10;03/01;05/31;5",
        "M;B;D;70;15;;;",
        "M;B;T;80;20;03/01;05/31;5",
        "M;B;F;100;50;03/01;05/31;10",
        "M;S;S;70;20;02/01;05/31;15",
        "M;S;D;90;30;;;",
        "M;S;T;100;50;03/01;05/31;5",
        "M;S;F;120;60;03/01;05/31;10",
        "P;B;S;40;10;01/01;05/31;10",
        "P;B;D;60;15;01/01;05/31;5",
        "P;B;T;70;20;03/01;05/31;10",
        "P;B;F;90;40;03/01;05/31;15",
        "P;S;S;50;10;01/01;05/31;10",
        "P;S;D;70;30;01/01;05/31;5",
        "P;S;T;80;40;03/01;05/31;10",
        "P;S;F;100;50;03/01;05/31;15",
        NULL // marks end of data
    };
    FILE *F = fopen(table1_file, "w");
    if (!F){
        perror("Error opening file");
        return;
    }
    for (int i = 0; dados[i] != NULL; ++i) {
        fprintf(F, "%s\n", dados[i]);
    }
    fclose(F);
    printf("Arquivo 'table1.txt' criado com sucesso.\n");
}


void WriteTable2ToFile() {
    const char *season_pricing[] = {
        "B;baixa;11/01;03/31;0",
        "I;intermédia;04/01;05/31;50",
        "M;média;06/01;06/30;60",
        "A;alta;06/01;08/31;100",
        NULL
    };
    //int num_entries = sizeof(season_pricing) / sizeof(season_pricing[0]);

    FILE *F = fopen(table2_file, "w");
    if (!F) {
        perror("Error opening file");
        return;
    }
    printf("\n\ntest\n\n");
    for (int i = 0; season_pricing[i] != NULL; ++i) {
       fprintf(F, "%s\n", season_pricing[i]);
    }

    fclose(F);
    printf("Season pricing data written to '%s'.\n", table2_file);
}

void WriteTable3ToFile() {
    const char *services[] = {
        "PA;P. Almoço;10",
        "A;Almoço;20",
        "PA+A;P. Almoço + Almoço;25",
        "J;Jantar;20",
        "PA+J;P. Almoço + Jantar;20",
        "PA+A+J;P. Almoço + Almoço + Jantar;30",
        "B&C;Bebidas e Cafetaria;10",
        "PC;Pensão Completa;50",
        "O;Outros;10",
        NULL
    };

    FILE *F = fopen(table3_file, "w");
    if (!F) {
        perror("Error opening file");
        return;
    }

    for (int i = 0; services[i] != NULL; ++i) {
       fprintf(F, "%s\n", services[i]);
    }

    fclose(F);
    printf("Service type data written to '%s'.\n", table3_file);
}




char *generate_random_postal_code() {
    char *postal_code = (char *)malloc(9 * sizeof(char)); // 8 characters + null terminator
    if (postal_code == NULL) {
        perror("Memory allocation failed");
        return NULL; // Return NULL in case of allocation failure
    }
    sprintf(postal_code, "%04d-%03d", rand() % 10000, rand() % 1000);
    return postal_code;
}

char *generate_random_nif() {
    char *nif = malloc(10 * sizeof(char)); // Dynamically allocate space for 9 digits + null terminator
    if (nif == NULL) {
        perror("Memory allocation failed");
        return NULL; // Return NULL in case of allocation failure
    }

    // Generate first digit to be between 1 and 9
    nif[0] = '1' + getRandomInt(0,9); // Converts int to char directly

    // Generate subsequent digits to be between 0 and 9
    for (int i = 1; i < 9; i++) {
        nif[i] = '0' + getRandomInt(0,9); // Converts int to char directly
    }

    // Null-terminate the string
    nif[9] = '\0';
    printf("\n\tgenerated nif ---> %s\n", nif);

    return nif;
}


// Utility function to check if a string matches basic criteria: not NULL and not empty
bool isValidString(const char *str) {
    return str != NULL && str[0] != '\0';
}

// Checks if the client type is valid
bool isValidClientType(char type) {
    return type == 'N' || type == 'V' || type == 'E';
}

// Simple length check for postal code and NIF, not format specific
bool isValidLength(const char *str, size_t expectedLength) {
    return strlen(str) == expectedLength;
}

bool isValidNIF(const char *nif) {
    
     if (nif == NULL || nif[0] == '\0' || nif[0] == '0') {
        return false; // Ensure NIF is not NULL and not an empty string, and NIF cannot start with '0'
    }

    return strlen(nif) == 10; // Ensure NIF is exactly 9 digits long
}

// Validate card number (basic check, not format specific)
bool isValidCard(const char *card) {
    // Example: Basic validation could be just checking if it's a valid string
    return isValidString(card);
    // For more specific validation (like format), additional checks would be required
}




// Checks if a given date falls within the start and end dates
bool is_date_within_range(const char* date, const char* start_date, const char* end_date) {
    return strcmp(date, start_date) >= 0 && strcmp(date, end_date) <= 0;
}

// Function to manually parse a date string in "YYYY-MM-DD" format
void parse_date(const char* date_str, struct tm* date_tm) {
    int year, month, day;
    sscanf(date_str, "%d-%d-%d", &year, &month, &day);
    date_tm->tm_year = year - 1900; // tm_year is years since 1900
    date_tm->tm_mon = month - 1;    // tm_mon is 0-based
    date_tm->tm_mday = day;
    date_tm->tm_hour = 0;
    date_tm->tm_min = 0;
    date_tm->tm_sec = 0;
    date_tm->tm_isdst = -1;         // Not dealing with DST (daylight saving time)
}

char** get_days_range(const char* start_date, const char* end_date, int* size) {
    struct tm start_tm = {0}, end_tm = {0};
    parse_date(start_date, &start_tm);
    parse_date(end_date, &end_tm);

    // Calculate the difference in days
    time_t start_time = mktime(&start_tm);
    time_t end_time = mktime(&end_tm);
    double seconds = difftime(end_time, start_time);
    *size = seconds / (3600 * 24) + 1; // Include both start and end dates

    // Allocate memory for date strings
    char** dates = (char**)malloc(*size * sizeof(char*));
    for (int i = 0; i < *size; i++) {
        dates[i] = (char*)malloc(11 * sizeof(char)); // YYYY-MM-DD + null terminator
        struct tm tmp_tm = start_tm;
        tmp_tm.tm_mday += i; // Increment day
        mktime(&tmp_tm); // Normalize the tm structure
        strftime(dates[i], 11, "%Y-%m-%d", &tmp_tm);
    }

    return dates;
}




































#ifdef _WIN32
#include <stdio.h>
#include <windows.h>

int kbhit()
{
    static HANDLE input_handle = NULL;
    INPUT_RECORD input_record;
    DWORD events_read;

    // Initialize the input handle on the first call
    if (input_handle == NULL)
    {
        input_handle = GetStdHandle(STD_INPUT_HANDLE);
        SetConsoleMode(input_handle, ENABLE_PROCESSED_INPUT);
    }

    // Check if there is any input available
    PeekConsoleInput(input_handle, &input_record, 1, &events_read);
    if (events_read != 0 && input_record.EventType == KEY_EVENT && input_record.Event.KeyEvent.bKeyDown)
    {
        return 1;
    }

    return 0;
}
#else
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

int kbhit()
{
    struct termios oldt, newt;
    int oldf, newf;
    int ch;

    // Save the current terminal settings
    tcgetattr(STDIN_FILENO, &oldt);
    // Set the file status flags for stdin to non-blocking
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    // Set the terminal to raw mode
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    // Read one character from stdin
    ch = getchar();

    // Restore the terminal settings
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    // If a character was read, put it back on the input stream
    if (ch != EOF)
    {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}
#endif
