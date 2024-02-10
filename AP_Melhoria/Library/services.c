#include "Headers/services.h"

SERVICE *create_service(const char *type_of_service, const char *special_service_code, 
                                    const char *_description, double _value) {

    SERVICE *service = (SERVICE *)ec_malloc(sizeof(SERVICE));
    strncpy(service->type_of_service, type_of_service, sizeof(service->type_of_service) - 1);
    service->type_of_service[sizeof(service->type_of_service) - 1]='\0';
    strncpy(service->special_service_code, special_service_code, sizeof(service->special_service_code) - 1);
    service->special_service_code[sizeof(service->special_service_code) - 1]='\0';
    strncpy(service->description, _description, sizeof(service->description) - 1);
    service->description[sizeof(service->description) - 1]='\0';

    service->value = _value;

    return service;
}

void destroy_service(SERVICE *service) {
    if (service != NULL) {
        free(service);
        service=NULL;
    }
}

void show_service(const SERVICE *service) {
    beautify(__FUNCTION__);
    printf("Service Type: %s \nCode: %s \nDescription: %s \nValue: %.2f\n\n",
           service->type_of_service, service->special_service_code, service->description, service->value);
}

void modify_service(SERVICE *srv, const char *type_srv, char *special_srv_code, const char *new_description, double _value) {
    if (srv != NULL) {
        strncpy(srv->type_of_service, type_srv, sizeof(srv->type_of_service) - 1);
        strncpy(srv->special_service_code, special_srv_code, sizeof(srv->special_service_code) - 1);
        strncpy(srv->description, new_description, sizeof(srv->description) - 1);
        unsigned index = (unsigned) strlen(srv->description);
        srv->description[index] = '\0';
        srv->value=_value;
    }
}







service_done *create_service_done(int service_code, int room_code, int funcionario, SERVICE *service) {
    service_done *done_service = (service_done *)ec_malloc(sizeof(service_done));
    done_service->service_code = service_code;
    done_service->room_code = room_code;
    done_service->funcionario = funcionario;
    done_service->date = time(NULL); //get current time
    *done_service->service = *service; //deep copy
    done_service->flag_state=0;

    return done_service;
}

void destroy_service_done(service_done *done_service) {
    if (done_service != NULL) {
        // If extra_service was dynamically allocated and stored in service_done, it needs deep cleaning.
        // Otherwise, simply free the service_done struct.
        free(done_service);
    }
}

void show_service_done(const service_done *done_service) {
    beautify(__FUNCTION__);
    printf("Service Code: %d \nRoom Code: %d \nEmployee: %d \nDate: %s \nService Value: %.2f\n\n",
           done_service->service_code, done_service->room_code, done_service->funcionario, 
                            ctime(&done_service->date), done_service->service->value);
}

void modify_service_done(service_done *sd, int new_service_code, int new_room_code, 
                               int new_funcionario, time_t new_date, 
                               const char *type_srv, char *special_srv_code, 
                               const char *new_description, double new_value) {                            
    if (sd != NULL) {
        if(sd->flag_state == 1){
            fatal("[ ]--->service as been already done, cannot be modified!!");
        }else {
            // Modify the direct fields of the service_done structure
            sd->service_code = new_service_code;
            sd->room_code = new_room_code;
            sd->funcionario = new_funcionario;
            sd->date = new_date;

            // If there's an existing SERVICE associated, modify it
            if (sd->service != NULL) {
                modify_service(sd->service, type_srv, special_srv_code, new_description, new_value);
            } else {
            fatal("Service detail not found....");
            }
        }
    } else fatal("Null pointer given....");
}


void mark_service_done(service_done *svd){
    svd->flag_state=1;

}
