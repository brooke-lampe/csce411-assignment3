#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h>
#include <string.h>

#include "record.h"


int main(int argc, char **argv)
{
    int i;
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
    
    struct timeval time_start, time_end;
	
	int userId = 1;
	int max_messages = 0;
    
    /* start time */
    gettimeofday(&time_start, NULL);
    
    for (i = first_record_id; i <= last_record_id; i++) {
        /* open the corresponding file */  
        sprintf(filename, "record_%06d.dat", i);
    
        fp = fopen(filename,"rb");
    
        if (!fp) {
            fprintf(stderr, "Cannot open %s\n", filename);
            continue;
        }
        
        /* read the record from the file */
        record_t *rp = read_record(fp);
    
        /* =========== start of data processing code ================ */
		char * isNebraska = NULL;
		isNebraska = strstr(rp->location,"Nebraska");
		
		if(isNebraska != NULL) {
			int j;
			int timed = 0;
			for(j = 0; j < rp->message_num; j++) {
				message_t *message = &(rp->messages[j]);
				if (message->hour == 8 || (message->hour == 9 && message->minute == 0)){
					timed = 1;
					j = rp->message_num;
				}
			}
			if (timed == 1 && max_messages < rp->message_num){
				userId = rp->id;
			}
		}
        
        /* =========== end of data processing code ================ */    
    
        /* free memory */
        free_record(rp);
    
        /* close the file */
        fclose(fp);
    }    
        
    
    /* end time */
    gettimeofday(&time_end, NULL);
    
    float totaltime = (time_end.tv_sec - time_start.tv_sec)
                    + (time_end.tv_usec - time_start.tv_usec) / 1000000.0f;
                    
         
                    
    printf("\n\nProcess time %f seconds\n", totaltime);
	
	printf("User ID:  %d\n", userId);
    
    return 0;
}
