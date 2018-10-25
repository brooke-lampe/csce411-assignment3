#include <math.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include "helper/record.h"

void remove_trailing_comma(char * location);
void transform_record(int first_record_id, int last_record_id);
void sort_records(int first_record_id, int last_record_id);
int compareByName(const void *a, const void *b);

struct message {
    int user_id;
    char text[TEXT_LONG];
    char date[TEXT_SHORT]; /* Sorting by date */
};

struct user {
    int user_id;
    char name[TEXT_SHORT];
    char location[TEXT_LONG]; /* Sorting by location */
};



int main(int argc, char **argv)
{

    /* print usage if needed */
    if (argc != 3) {
        fprintf(stderr, "Usage: %s first_record_id last_record_id\n", argv[0]);
        exit(0);
    }
    
    /* first and last record ids */
    int first_record_id = atoi(argv[1]);
    int last_record_id = atoi(argv[2]);
    transform_record(first_record_id, last_record_id);
    sort_records(first_record_id, last_record_id);
    return 0;
}

void transform_record(int first_record_id, int last_record_id) {
    int i,j;
    int number_of_msgs = 0;
    char filename[1024];
    char path[TEXT_SHORT];
    char location_format[TEXT_SHORT];
    system("rm -rf ./data");
    system("mkdir ./data");
    
    struct timeval time_start, time_end;
    gettimeofday(&time_start, NULL);
    
    for (i = first_record_id; i <= last_record_id; i++) {

        sprintf(filename, "../data/record_%06d.dat", i);
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
        sprintf(filename, "../data/record_%06d.dat", i);
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
                message_file = fopen(path, "w+");                
            }
            fprintf(message_file, "%d\t", rp->id);
            fprintf(message_file, "%d/%d/%d %d:%d\t", rp->messages[j].month, rp->messages[j].day, rp->messages[j].year, rp->messages[j].hour, rp->messages[j].minute);
            fprintf(message_file, "%s\n", rp->messages[j].text);
            messages_in_file++;
        }
        fclose(fp);

    }
    fclose(message_file);
    gettimeofday(&time_end, NULL);
    
    float totaltime = (time_end.tv_sec - time_start.tv_sec)
                    + (time_end.tv_usec - time_start.tv_usec) / 1000000.0f;
                    
         
                    
    printf("\n\nProcess time %f seconds\n", totaltime);
}

/* Assuming transform_record has already been run */
void sort_records(int first_record_id, int last_record_id) {
    char path[TEXT_SHORT];
    char buffer[160];
    char other[160];
    FILE *user_fp;
    struct user users[last_record_id-first_record_id+1];
    int i;
    for (i = 0; i <= (last_record_id-first_record_id); i++) {
        sprintf(path, "data/user_%06d.dat", first_record_id+i);
        user_fp = fopen(path, "r");
        while(fscanf(user_fp, "%d\t%[^\t]\t%[^\n]\n", &users[i].user_id, users[i].name, users[i].location) != EOF) ;
        fclose(user_fp);
    }

    /* enter how query should be filtered */
    qsort(users, last_record_id-first_record_id+1, sizeof(struct user), compareByName);
    
    /* FOR DEBUGGING PURPOSES */
    printf("MAYBE SORTED NAMES: \n");
    for (i = 0; i <= (last_record_id-first_record_id); i++) {
        printf("Reocrd: %d\t%s\t%s\n", users[i].user_id, users[i].name, users[i].location);
    }
    
    for (i = 0; i <= (last_record_id-first_record_id); i++) {
        sprintf(path, "data/user_%06d.dat", first_record_id+i);
        user_fp = fopen(path, "w");
        fprintf(user_fp, "%06d\t%s\t%s\n", users[i].user_id, users[i].name, users[i].location);
        fclose(user_fp);
    }


}

int compareByName(const void *a, const void *b) {
     struct user *user1 = (struct user *)a;
     struct user *user2 = (struct user *)b;
     return strcmp(user1->name, user2->name); 
}

int compareByLocation(const void *a, const void *b) {
     struct user *user1 = (struct user *)a;
     struct user *user2 = (struct user *)b;
     return strcmp(user1->location, user2->location); 
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