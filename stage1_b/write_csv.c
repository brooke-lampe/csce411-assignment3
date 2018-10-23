#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h>
#include <string.h>

#include "helper/record.h"


int main(int argc, char **argv)
{
    int i, j;
    /* print usage if needed */
    if (argc != 3) {
        fprintf(stderr, "Usage: %s first_record_id last_record_id\n", argv[0]);
        exit(0);
    }
    
    /* first and last record ids */
    int first_record_id = atoi(argv[1]);
    int last_record_id = atoi(argv[2]);
    

    char filename[1024];
    FILE *fp = NULL;
	
	FILE *fp_users;
	FILE *fp_messages;
	
	fp_users=fopen("Users.csv","w+");
	fp_messages=fopen("Messages.csv","w+");
	
	fprintf(fp_users,"id,name,location\n");
	fprintf(fp_messages,"id,user_id,text,time_stamp\n");    
	int message_id = 0;
    
    for (i = first_record_id; i <= last_record_id; i++) {
        sprintf(filename, "../data/record_%06d.dat", i);
    
        fp = fopen(filename,"rb");
    
        if (!fp) {
            fprintf(stderr, "Cannot open %s\n", filename);
            continue;
        }
		record_t *rp = read_record(fp);
		char *location = rp->location;
		location[strlen(location)-1] = 0;
		location[strlen(location)-2] = 0;
		fprintf(fp_users,"%d,%s,\"%s\"\n", rp->id, rp->name, rp->location);

		for(j = 0; j < rp->message_num; j++) {
			message_t *message = &(rp->messages[j]);
			char datetime[19];
			sprintf(datetime, "%04d-%02d-%02d %02d:%02d:%02d", message->year, message->month,  message->day,  message->hour,  message->minute, 0);
			char *text = message->text;
			text[strlen(text)-1] = 0;
			fprintf(fp_messages,"%d,%d,%s,%s\n", message_id, rp->id, message->text, datetime);
			message_id++;
		}    
    
        /* free memory */
        free_record(rp);
    
        /* close the file */
        fclose(fp);
    }
	fclose(fp_users);
	fclose(fp_messages);
             
                    
    printf("CSV files created\n");
    
    return 0;
}
