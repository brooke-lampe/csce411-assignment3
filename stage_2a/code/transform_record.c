#include <math.h>
#include "record.h"

void remove_trailing_comma(char * location);

int main(int argc, char **argv)
{
    int i,j;
    /* print usage if needed */
    if (argc != 3) {
        fprintf(stderr, "Usage: %s first_record_id last_record_id\n", argv[0]);
        exit(0);
    }
    
    /* first and last record ids */
    int first_record_id = atoi(argv[1]);
    int last_record_id = atoi(argv[2]);
    int number_of_msgs = 0;
    char filename[1024];
    char path[TEXT_SHORT];
    char location_format[TEXT_SHORT];
    system("rm -rf ./data");
    system("mkdir ./data");

    for (i = first_record_id; i <= last_record_id; i++) {

        sprintf(filename, "record_%06d.dat", i);
        FILE *fp = fopen(filename,"rb");
        if (!fp) {
            fprintf(stderr, "Cannot open %s\n", filename);
            exit(0);
        }

        /* read the record from the file */
        record_t *rp = read_record(fp);
        
        sprintf(path, "data/user_%06d.dat", i);
        
        FILE *user_file = fopen(path, "w+");
        
        fprintf(user_file, "%06d\t", rp->id);
        fprintf(user_file, "%s\t", rp->name);
        remove_trailing_comma(rp->location);
        fprintf(user_file, "%s\n", rp->location);
        number_of_msgs += rp->message_num;
        
        fclose(user_file);
        fclose(fp);

        free_record(rp);
    }
    int messages_per_file = floor(number_of_msgs/(last_record_id-first_record_id));
    
    int message_record_filenumber = 0;
    int messages_in_file = 0;

    sprintf(path, "data/message_%06d.dat", message_record_filenumber);
    FILE *message_file = fopen(path, "w+");

    for (i = first_record_id; i <= last_record_id; i++) {
        sprintf(filename, "record_%06d.dat", i);
        FILE *fp = fopen(filename,"rb");
        if (!fp) {
            fprintf(stderr, "Cannot open %s\n", filename);
            exit(0);
        }

        /* read the record from the file */
        record_t *rp = read_record(fp);
        for (j = 0; j < rp->message_num; j++) {
                    
            if (messages_in_file == messages_per_file) {
                fclose(message_file);
                message_record_filenumber++; 
                messages_in_file = 0;
                sprintf(path, "data/message_%06d.dat", message_record_filenumber);
                fopen(path, "w+");                
            }

            fprintf(message_file, "%d/%d/%d %d:%d\t", rp->messages[j].month, rp->messages[j].day, rp->messages[j].year, rp->messages[j].hour, rp->messages[j].minute);
            fprintf(message_file, "%s\n", rp->messages[j].text);
            messages_in_file++;
        }


    }
    fclose(message_file);

    return 0;
}

void remove_trailing_comma(char * location) {
    int i;
    int found = 0;
    for (i = 0; i < TEXT_SHORT; i++) {
        if (location[i] == ',' && !found) {
            found = 1;
        } else if (location[i] == ',') {
            location[i] = '\0';
        }
    }

}