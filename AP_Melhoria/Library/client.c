#include "Headers/client.h"

// extern void *ec_malloc(unsigned int size);
// extern void logging(char *datafile, const char *funcname, char *info);
// extern char *logging_file;


person *create_person(const char *_name, const char *_address, const char *_postal_code, const char *_locality, const char *_NIF) {
    person *new_person = (person *)ec_malloc(sizeof(person));
    if (!new_person)
    {
        fatal("in create_person(), allocation for person failed... ");
        return NULL;
    }
    if (!isValidString(_name) || !isValidString(_address) || 
        !isValidLength(_postal_code, 8) || !isValidString(_locality) /* || 
        !isValidNIF(_NIF)*/ ) {

        printf("Invalid input data for creating a person.\n");
        return NULL;
    }

    //name
    new_person->name = (char *)ec_malloc(strlen(_name) + 1);  //could jusrt be done with strdup()
    if (!new_person->name)
    {
        fatal("in create_person(), allocation for name in person failed... ");
        free(new_person);
        return NULL;
    }
    new_person->name = strcpy(new_person->name ,_name); 
    
    //address
    new_person->address = (char *)ec_malloc(strlen(_address) + 1);
    if (!new_person->address)
    {
        fatal("in create_person(), allocation for address in person failed... ");
        free(new_person);
        return NULL;
    }
    new_person->address = strcpy(new_person->address ,_address); 
    
    //locality
    new_person->locality = (char *)ec_malloc(strlen(_locality) + 1);
    if (!new_person->locality)
    {
        fatal("in create_person(), allocation for address in person failed... ");
        free(new_person);
        return NULL;
    }
    new_person->locality = strcpy(new_person->locality ,_locality); 

    //nif
    new_person->NIF = (char *)ec_malloc(strlen(_NIF) + 1);
    if (!new_person->NIF)
    {
        fatal("in create_person(), allocation for address in person failed... ");
        free(new_person);
        return NULL;
    }
    new_person->NIF = strcpy(new_person->NIF ,_NIF); 
    
    // postal code and nif dont need to allocate, size already defined in struct
    strncpy(new_person->postal_code, _postal_code, sizeof(new_person->postal_code) - 1); 
    strncpy(new_person->NIF, _NIF, sizeof(new_person->NIF) - 1); 

    return new_person;
}

void DestroyPerson(void *p) {
    person *P = (person *)p;
    if (P != NULL) {
        free(P->name);
        free(P->address);
        free(P->locality);
        // No need to free postal_code and NIF as they are not dynamically allocated
        free(P);
        P=NULL;
    }
}




CLIENT *create_client(person *_client, char _client_type, const char *_card) {
    static int _cod =0;
    CLIENT *new_client = (CLIENT *)ec_malloc(sizeof(CLIENT));
    if (!new_client) {
        fatal("in create_client(), allocation for CLIENT failed...");
        return NULL;
    }
    if (!isValidClientType(_client_type) || 
        !isValidCard(_card)){

        printf("Invalid input data for creating a client.\n");
        return NULL;
    }

    // Assigning the person pointer directly, assuming the person has already been created
    new_client->client = _client;

    // Other attributes are simply assigned since they are not dynamically allocated
    new_client->cod = _cod++;
    new_client->client_type = _client_type;

    // Card
    new_client->card = (char *)ec_malloc(strlen(_card) + 1);
    if (!new_client->card) {
        fatal("in create_client(), allocation for card in CLIENT failed...");
        free(new_client);
        return NULL;
    }
    strcpy(new_client->card, _card);

    return new_client;
}

void DestroyClient(void *c) {
    CLIENT *C = (CLIENT *)c;
    if (C != NULL) {
        free(C->card);
        DestroyPerson(C->client);
        free(C);
    }
}


subclient *create_subclient(person *_client, CLIENT *_prime, int _prime_room_code) {
    subclient *new_subclient = (subclient *)ec_malloc(sizeof(subclient));
    if (!new_subclient) {
        fatal("in create_subclient(), allocation for subclient failed...");
        return NULL;
    }

    // Assigning the person pointer directly, assuming the person has already been created
    new_subclient->client = _client;
    int _prime_code = _prime->cod;
    // Other attributes are simply assigned
    char str[256];
    snprintf(str, sizeof(str), "%d0000", _prime_code);


    int subcode = atoi(str)+1; 
    new_subclient->cod = subcode;
    new_subclient->prime = _prime;
    new_subclient->prime_code = _prime_code;
    new_subclient->prime_room_code = _prime_room_code;

    return new_subclient;
}

void DestroySubclient(void *sc) {
    subclient *SC = (subclient *)sc;
    if (SC != NULL) {
        DestroyPerson(SC->client);
        // Assuming the prime CLIENT is managed elsewhere, so not freeing SC->prime
        free(SC);
    }
}




void ShowClient(void *c) {
    CLIENT *C = (CLIENT *)c;
    printf("\n     [*]<%s>[*]\n", __FUNCTION__);
    printf("\t[ ]CODIGO: [%d]\n\t[ ]TIPO CLIENTE: [%c]\n", C->cod, C->client_type);
    if (C->client != NULL) {
        printf("\t[ ]NOME: [%s]\n\t[ ]NIF: [%s]\n", C->client->name, C->client->NIF);
    }
    printf("\t[ ]CARTAO: [%s]\n", C->card);
}

void ShowSubclient(void *sc) {
    subclient *SC = (subclient *)sc;
    printf("\n     [*]<%s>[*]\n", __FUNCTION__);
    if (SC->client != NULL) {
        printf("\t[ ]NOME: [%s]\n\t[ ]NIF: [%s]\n", SC->client->name, SC->client->NIF);
    }
    printf("\t[ ]CODIGO SUBCLIENTE: [%d]\n", SC->cod);
    if (SC->prime != NULL) {
        printf("\t[ ]CODIGO CLIENTE PRIME: [%d]\n", SC->prime->cod);
        printf("\t[ ]CODIGO QUARTO PRIME: [%d]\n", SC->prime_room_code);
    }
}


CLIENT *create_random_clients() {
    
    const char *names[] = {"Alice", "Bob", "Charlie", "Diana", "Evan",
                           "Fiona", "George", "Hannah", "Ian", "Jill"};
    const char *addresses[] = {"123 Elm St", "456 Pine St", "789 Oak St",
                               "101 Maple St", "202 Birch St"};
    const char client_types[] = {'N', 'V', 'E'}; // N-normal, V-VIP, E-excelencia
    const char *cards[] = {"1234-5678-9012-3456", "2345-6789-0123-4567",
                           "3456-7890-1234-5678"};
    const char *cities[] = {"City A", "City B", "City C", "City D"};
    const int num_cities = sizeof(cities) / sizeof(cities[0]);
    

    // Generate random indexes for arrays
    //int nameIndex = rand() % (sizeof(names) / sizeof(names[0]));
    int nameIndex = rand() % (sizeof(names) / sizeof(names[0]));
    int addressIndex = rand() % (sizeof(addresses) / sizeof(addresses[0]));
    int cityIndex = rand() % num_cities;
    int typeIndex = rand() % (sizeof(client_types) / sizeof(client_types[0]));
    int cardIndex = rand() % (sizeof(cards) / sizeof(cards[0]));
    char *postal = generate_random_postal_code();
    char *nif=generate_random_nif();

    // Creating a random person
    person *client_person = create_person(names[nameIndex], addresses[addressIndex], postal, cities[cityIndex], nif);

    // Creating a random client
    CLIENT *client = create_client(client_person, client_types[typeIndex], cards[cardIndex]);

    // Show the client to verify
    ShowClient(client);
    //printf("00000");
    return client;

}

void update_person_details(CLIENT *C, const char *new_name, const char *new_address, 
                            const char *new_postal_code, const char *new_locality, const char *new_NIF) {
    if (C == NULL) return;

    // Update name
    if (C->client->name != NULL) free(C->client->name);
    C->client->name = strdup(new_name);

    // Update address
    if (C->client->address != NULL) free(C->client->address);
    C->client->address = strdup(new_address);

    // Update postal code
    strncpy(C->client->postal_code, new_postal_code, sizeof(C->client->postal_code) - 1);
    C->client->postal_code[sizeof(C->client->postal_code) - 1] = '\0'; // Ensure null-termination

    // Update locality
    if (C->client->locality != NULL) free(C->client->locality);
    C->client->locality = strdup(new_locality);

    // Update NIF
    strncpy(C->client->NIF, new_NIF, sizeof(C->client->NIF) - 1);
    C->client->NIF[sizeof(C->client->NIF) - 1] = '\0'; // Ensure null-termination
}

void convert_provisional_to_definitive(CLIENT *client) {
    if (client == NULL) {
        printf("Client pointer is null.\n");
        return;
    }

    //client->client_type=

    printf("Client %d converted to definitive.\n", client->cod);
}





