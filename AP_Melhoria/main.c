#include "Library/Headers/hotel.h"
#include "Library/Headers/data.h"
#include "Library/Headers/utils.h"
#include "Library/Headers/test.h"
#include <stdlib.h>
#include <stdio.h>

extern char *logging_file;
extern char *table1_file;   
extern char *table2_file;
extern char *table3_file;
extern char *reservations_file;
extern char *hotel_config;
extern void save_hotel_to_binary_file(const char *filename, const HOTEL *hotel);
extern HOTEL *load_hotel_from_binary_file(const char *filename);
void menu(HOTEL *ht);
#define MAX_LINHAS 100 // Defina o máximo de linhas que você espera no arquivo

void __INNIT_TABLES__(RoomData *data);

int main()
{
    srand(time(NULL)); // Seed the random number generator
    
    
    
    //HOTEL *bin_test = load_hotel_from_binary_file(hotel_config);
    //show_hotel(bin_test);

    HOTEL *test_hotel = initialize_hotel("MY HOTEL BAY HOTEL");
    //show_hotel(test_hotel);
    RoomData *data=NULL;
    //printf("pointing to -> %p\n\n", data);
    __INNIT_TABLES__(data);
    // */
    
    
    CLIENT *clientArray[200];
    for(int i = 0; i < 200; i++){
        clientArray[i] = create_random_clients();
    }
    

    
    

    while(1){
        static int runs =0;
        printf("\n\n\tVALUE AT RUN: %d\n\n", runs );
    //ShowClient(C);
    int clientIndex = getRandomInt(0, 10 - 1);
    //show_hotel(test_hotel);
    CLIENT *C = clientArray[clientIndex];

    for( int i = 0; i<5; i++){
        simulate_room_booking(test_hotel, C);
        simulate_service_request(test_hotel);
    }
   
    simulate_check_in(test_hotel);
    simulate_check_out(test_hotel);
    simulate_payment_processing(test_hotel);
    runs++;

   }

    /*
    FICHA* ficha = (FICHA*)malloc( 5 * sizeof(FICHA));
    if (ficha == NULL) {
        perror("Initial malloc failed");
        return -1;
    }

    FICHA* temp = (FICHA*)realloc(ficha, 10 * sizeof(FICHA));
    if (temp == NULL) {
        perror("Realloc failed");
        free(ficha); // It's important to free the original array if realloc fails
        return EXIT_FAILURE;
    }
    ficha = temp; // Step 4: Update pointer

    */
   //printf("whre does it happen"); 

    //save_hotel_to_binary_file(hotel_config, test_hotel);


    destroy_hotel(test_hotel);
    //destroy_hotel(bin_test);






    return 0;
}

//
//void save_reservation_to_file(FILE *file, const reservation *res);
//void load_reservation_from_file(FILE *file, reservation *res);
//


void __INNIT_TABLES__(RoomData *data){
    WriteTable1ToFile();
    WriteTable2ToFile();
    WriteTable3ToFile();
    int num_entries;
    data = read_room_data_from_file(table1_file, &num_entries);

    if (data) {
        for (int i = 0; i < num_entries; i++) {
            // Aqui você pode imprimir ou manipular os dados lidos
            printf("View: %c, Quality: %c, Capacity: %c, Price: %d\n",
                   data[i].view, data[i].quality, data[i].capacity, data[i].price);
            // ... e assim por diante para os outros campos
        }
    }

    
    SeasonPricing season_pricing[10]; // Assuming we won't have more than 10 entries
    int num_read = ReadSeasonPricingFromFile(table2_file, season_pricing, 10);

    if (num_read >= 0) {
        printf("Read %d entries from the file.\n", num_read);
        // Optionally, print the data to verify
        for (int i = 0; i < num_read; ++i) {
            printf("%c - %s (%s to %s): %d%%\n",
                   season_pricing[i].season_code,
                   season_pricing[i].season_name,
                   season_pricing[i].start_date,
                   season_pricing[i].end_date,
                   season_pricing[i].price_increase);
        }
    } else {
        printf("Failed to read entries from the file.\n");
    }


     // Read data from file
    ServiceType services[10]; // Assuming we won't have more than 10 service types
    int num_services_read = ReadServiceTypesFromFile(table3_file, services, 10);

    if (num_services_read >= 0) {
        printf("Read %d service types from the file.\n", num_services_read);
        // Optionally, print the data to verify
        for (int i = 0; i < num_services_read; ++i) {
            printf("%s - %s: %d€\n",
                   services[i].service_code,
                   services[i].description,
                   services[i].value);
        }
    } else {
        printf("Failed to read service types from the file.\n");
    }

}




void menu(HOTEL *ht) {
    int choice;

    do {
        beautify("<HOTEL MANAGEMENT MENU>");
        printf("###   $<1> Show Hotel Information\n");
        printf("###   $<2> Manage Rooms\n");
        printf("###   $<3> Manage Services\n");
        printf("###   $<4> Manage Reservations\n");
        printf("###   $<5> Check-in a Reservation\n");
        printf("###   $<6> Check-out a Reservation\n");
        printf("###   $<7> List Occupied and Available Rooms\n");
        printf("###   $<8> Show Occupancy Map for a Room\n");
        printf("###   $<9> Generate Payment Report\n");
        printf("###   $<10> Load Hotel Data\n");
        printf("###   $<11> Save Hotel Data\n");
        printf("###   $<0> Exit and Save Data\n");
        printf("\n\n\t <Enter your choice>: ");
        scanf("%d", &choice);
        while (getchar() != '\n'); // Clear the input buffer

        switch (choice) {
            case 1:
                //show_hotel(ht);
                break;
            case 2:
                // Invoke room management function
                //manage_rooms(ht);
                break;
            case 3:
                // Invoke service management function
               // manage_services(ht);
                break;
            case 4:
                // Invoke reservation management function
                //manage_reservations(ht);
                break;
            case 5:
                // Invoke check-in function
                //check_in_reservation(ht);
                break;
            case 6:
                // Invoke check-out function
               // check_out_reservation(ht);
                break;
            case 7:
                // Invoke function to list occupied and available rooms
                //list_room_availability(ht, "2023-01-01", "2023-12-31"); // Example dates
                break;
            case 8:
                // Invoke function to show occupancy map for a specific room
                //show_occupancy_map(ht);
                break;
            case 9:
                // Invoke function to generate payment report
                //generate_payments_report(ht, "2023-01-01", "2023-12-31"); // Example dates
                break;
            case 10:
                // Load hotel data from a binary file
                //ht = load_hotel_from_binary_file(hotel_config);
                break;
            case 11:
                // Save current hotel data to a binary file
                //save_hotel_to_binary_file(hotel_config, ht);
                break;
            case 0:
                // Save data and exit
                printf("Saving hotel configurations and data before exiting...\n");
                //save_hotel_to_binary_file(hotel_config, ht);
                //destroy_hotel(ht);
                exit(0);
                break;
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    } while (choice != 0);
}
