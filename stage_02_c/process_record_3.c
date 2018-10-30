#include <math.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <stdbool.h>
#include "helper/record.h"

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

void remove_trailing_comma(char * location);
int transform_record(int first_record_id, int last_record_id);
void sort_records(int first_record_id, int last_record_id);
int compareByName(const void *a, const void *b);
int compareByLocation(const void *a, const void *b);
int compare_by_date(const void *a, const void *b);
int compare_by_time(const void *a, const void *b);
bool inArray(int val, int * arr, int size);

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
	int num_msg_files = transform_record(first_record_id, last_record_id);
    sort_records(first_record_id, last_record_id);
	
	struct timeval time_start, time_end;
    gettimeofday(&time_start, NULL);
	
	/* =========== start of data processing code ================ */
	char path[TEXT_SHORT];
	char date[TEXT_SHORT];
    FILE *message_fp;
	int counter = 0;
	int low = 0;
	int high = num_msg_files;
	int mid;
	int save = -1;
	int user_id, hour, minutes;
	int arr[4000];
	int flag = 0, flagA = 0, flagB = 0;
	
	while((low <= high) && (flag == 0)) {
		mid = low + (high - low) / 2;
		sprintf(path, "data/message_%06d.dat", mid);
		message_fp = fopen(path, "r");
		
		while (fscanf(message_fp, "%d\t%[^\t]\t%*[^\n]\n", &user_id, date) != EOF) {
			sscanf(date, "%*d/%*d/%*d %d:%d", &hour, &minutes);
			
			if(hour == 8 || (hour == 9 && minutes == 0)) {
				//if equal set flag
				//if flag is zero, there is nothing in the array to check for duplicates
				//if flag is nonzero, check the array for duplicates
				if((flag == 0) || (!inArray(user_id, arr, counter))) {
					arr[counter] = user_id;
					counter++;
					//printf("1st user ids %d  %d:%d\n", user_id, hour, minutes);
				}
				save = mid;
				//flag will cause outer loop to exit when we are done checking the file
				flag = 1;
			} else if (hour < 8) {
				//if smaller then check upper list
				low = mid + 1;
			} else {
				//if larger then check lower list
				high = mid - 1;
			}
		}
		fclose(message_fp);
	}
	
	if(save != -1) {
		//Check below
		while((mid >= low) && (flagA == 0)) {
			mid--;
			sprintf(path, "data/message_%06d.dat", mid);
			message_fp = fopen(path, "r");
			
			while (fscanf(message_fp, "%d\t%[^\t]\t%*[^\n]\n", &user_id, date) != EOF) {
				sscanf(date, "%*d/%*d/%*d %d:%d", &hour, &minutes);
				
				if(hour == 8 || (hour == 9 && minutes == 0)) {
					if(!inArray(user_id, arr, counter)) {
						arr[counter] = user_id;
						counter++;
						//printf("2nd user ids %d  %d:%d\n", user_id, hour, minutes);
					}
				} else {
					//flag will cause outer loop to exit when we are done checking the file
					flagA = 1;
				}
			}
			fclose(message_fp);
		}
		
		//Check above
		while((save <= high) && (flagB == 0)) {
			save++;
			sprintf(path, "data/message_%06d.dat", save);
			message_fp = fopen(path, "r");
			
			while (fscanf(message_fp, "%d\t%[^\t]\t%*[^\n]\n", &user_id, date) != EOF) {
				sscanf(date, "%*d/%*d/%*d %d:%d", &hour, &minutes);
				
				if(hour == 8 || (hour == 9 && minutes == 0)) {
					if(!inArray(user_id, arr, counter)) {
						arr[counter] = user_id;
						counter++;
						//printf("3rd user ids %d  %d:%d\n", user_id, hour, minutes);
					}
				} else {
					//flag will cause outer loop to exit when we are done checking the file
					flagB = 1;
				}
			}
			fclose(message_fp);
		}
	}
	int arrSize = counter;
	
	char location[TEXT_SHORT];
    FILE *user_fp;
	counter = 0;
	low = first_record_id;
	high = last_record_id;
	save = -1;
	char comma[2] = ",";
	char * token1;
	char * token2;
	
	while(low <= high) {
		mid = low + (high - low) / 2;
		sprintf(path, "data/user_%06d.dat", mid);
		user_fp = fopen(path, "r");
		
		fscanf(user_fp, "%d\t%*[^\t]\t%[^\n]\n", &user_id, location);
		fclose(user_fp);
		
		token1 = strtok(location, comma);
		token2 = strtok(NULL, comma);
		if(token2 == NULL) {
			token2 = token1;
		}
		//printf("1st token2 %s\n", token2);
		
		if(strcmp(token2, "Nebraska") == 0) {
			//if equal break loop
			//printf("1st user_id %d\n", user_id);
			if(inArray(user_id, arr, arrSize)) {
				counter++;
			}
			save = mid;
			break;
		} else if (strcmp(token2, "Nebraska") < 0) {
			//if smaller then check upper list
			low = mid + 1;
		} else {
			//if larger then check lower list
			high = mid - 1;
		}
	}
	
	if(save != -1) {
		//Check above and below
		while(mid >= low) {
			mid--;
			sprintf(path, "data/user_%06d.dat", mid);
			user_fp = fopen(path, "r");
			
			fscanf(user_fp, "%d\t%*[^\t]\t%[^\n]\n", &user_id, location);
			fclose(user_fp);
			
			token1 = strtok(location, comma);
			token2 = strtok(NULL, comma);
			if(token2 == NULL) {
				token2 = token1;
			}
			//printf("2nd token2 %s\n", token2);
			
			if(strcmp(token2, "Nebraska") == 0) {
				//printf("2nd user_id %d\n", user_id);
				if(inArray(user_id, arr, arrSize)) {
					counter++;
				}
			} else {
				break;
			}
		}
		while(save <= high) {
			save++;
			sprintf(path, "data/user_%06d.dat", save);
			user_fp = fopen(path, "r");
			
			fscanf(user_fp, "%d\t%*[^\t]\t%[^\n]\n", &user_id, location);
			fclose(user_fp);
			
			token1 = strtok(location, comma);
			token2 = strtok(NULL, comma);
			if(token2 == NULL) {
				token2 = token1;
			}
			//printf("3rd token2 %s\n", token2);
			
			if(strcmp(token2, "Nebraska") == 0) {
				//printf("3rd user_id %d\n", user_id);
				if(inArray(user_id, arr, arrSize)) {
					counter++;
				}
			} else {
				break;
			}
		}
	}
	
	printf("Number of Nebraska users sending messages between 8am and 9am: %d\n", counter);
	
	/* =========== end of data processing code ================ */
	gettimeofday(&time_end, NULL);
    
    float totaltime = (time_end.tv_sec - time_start.tv_sec)
                    + (time_end.tv_usec - time_start.tv_usec) / 1000000.0f;
                    
    printf("\n\nProcess time %f seconds\n", totaltime);
	
    return 0;
}

int transform_record(int first_record_id, int last_record_id) {
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
        fprintf(user_file, "%06d\t%d\t", rp->id, rp->message_num);
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
	
	return message_record_filenumber;
}

/* Assuming transform_record has already been run */
void sort_records(int first_record_id, int last_record_id) {
    char path[TEXT_SHORT];
    FILE *user_fp;
    struct user *users = malloc(sizeof(struct user) * (last_record_id-first_record_id+1));
    int i, msgs;
    int message_number = 0; 
    
    for (i = 0; i <= (last_record_id-first_record_id); i++) {
        sprintf(path, "data/user_%06d.dat", first_record_id+i);
        user_fp = fopen(path, "r");
        while(fscanf(user_fp, "%d\t%d\t%[^\t]\t%[^\n]\n", &users[i].user_id, &msgs, users[i].name, users[i].location) != EOF);
        message_number += msgs;
        fclose(user_fp);
    }
	
    /* enter how query should be filtered compareByName or compareByLocation*/
    qsort(users, last_record_id-first_record_id+1, sizeof(struct user), compareByLocation);
	
    /* Rewrite users from sorted array */
    for (i = 0; i <= (last_record_id-first_record_id); i++) {
        sprintf(path, "data/user_%06d.dat", first_record_id+i);
        user_fp = fopen(path, "w");
        fprintf(user_fp, "%06d\t%s\t%s\n", users[i].user_id, users[i].name, users[i].location);
        fclose(user_fp);
    }

    /* declare */
    struct message *messages = malloc(sizeof(struct message) * message_number);
    int messages_per_file = ceil(message_number/(last_record_id-first_record_id));
    int num_of_message_files = ceil(message_number/messages_per_file);

    /* Populate message array */ 
    int j = 0; 
    for (i = 0; i <= num_of_message_files; i++) {
        sprintf(path, "data/message_%06d.dat", i);
        FILE *message_fp = fopen(path, "r");
        while (fscanf(message_fp, "%d\t%[^\t]\t%[^\n]\n", &messages[j].user_id, messages[j].date, messages[j].text) != EOF) {
           j++;
           if (j >= message_number) break;
        }
        fclose(message_fp);
    }
	
    /* Sorting Messages by Date */
    qsort(messages, message_number, sizeof(struct message), compare_by_time);

    /* Rewrite messages from sorted array */ 
    j = 0; 
    int k = 0;
    //printf("%d\n", num_of_message_files);
    for (i = 0; i <= num_of_message_files; i++) {
        sprintf(path, "data/message_%06d.dat", i);
        FILE *message_fp = fopen(path, "w");
        while (j < messages_per_file) {
            if (k >= message_number) break;
            fprintf(message_fp, "%06d\t%s\t%s\n", messages[k].user_id, messages[k].date, messages[k].text);
            j++;
            k++;
        }
        j = 0; 
        fclose(message_fp);
    }

    /* Release Heap Memory Usage */
    free(users);
    free(messages);
}

int compare_by_date(const void *a, const void *b) {
    struct message *message1 = (struct message*)a;
    struct message *message2 = (struct message*)b;
    int year1, year2, month1, month2, day1, day2, hour1, hour2, minutes1, minutes2;
    sscanf(message1->date, "%d/%d/%d %d:%d", &month1, &day1, &year1, &hour1, &minutes1); 
    sscanf(message2->date, "%d/%d/%d %d:%d", &month2, &day2, &year2, &hour2, &minutes2);

    if (year1 > year2) {
        return 1;
    } else if (year2 > year1) {
        return -1;
    } else {
        if (month1 > month2) {
            return 1;
        } else if (month2 > month1) {
            return -1;
        } else {
            if (day1 > day2) {
                return 1;
            } else if (day2 > day1) {
                return -1;
            } else {
                if (hour1 > hour2) {
                    return 1;
                } else if (hour2 > hour1) {
                    return -1;
                } else {
                    if (minutes1 > minutes2) {
                        return 1;
                    } else if (minutes2 > minutes1) {
                        return -1;
                    } else {
                        return 0;
                    }
                }
            }
        }
    }
}

int compare_by_time(const void *a, const void *b) {
    struct message *message1 = (struct message*)a;
    struct message *message2 = (struct message*)b;
    int hour1, hour2, minutes1, minutes2;
    sscanf(message1->date, "%*d/%*d/%*d %d:%d", &hour1, &minutes1); 
    sscanf(message2->date, "%*d/%*d/%*d %d:%d", &hour2, &minutes2);

	if (hour1 > hour2) {
		return 1;
	} else if (hour2 > hour1) {
		return -1;
	} else {
		if (minutes1 > minutes2) {
			return 1;
		} else if (minutes2 > minutes1) {
			return -1;
		} else {
			return 0;
		}
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
	 
	 char comma[2] = ",";
	 char location1[TEXT_SHORT], location2[TEXT_SHORT];
	 char * token1 = NULL;
	 char * token2 = NULL;
	 
	 if(user1->location != NULL) {
		 strcpy(location1, user1->location);
		 token1 = strtok(location1, comma);
		 token1 = strtok(NULL, comma);
		 if(token1 == NULL) {
			 token1 = location1;
		 }
	 }
	 
	 if(user2->location != NULL) {
		 strcpy(location2, user2->location);
		 token2 = strtok(location2, comma);
		 token2 = strtok(NULL, comma);
		 if(token2 == NULL) {
			token2 = location2;
		 }
	 }
     return strcmp(token1, token2); 
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

bool inArray(int val, int * arr, int size) {
	int i;
	for(i = 0; i < size; i++) {
		if(arr[i] == val) {
			return true;
		}
	}
	return false;
}