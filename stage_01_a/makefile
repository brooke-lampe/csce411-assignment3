process_record : process_record_1.o process_record_2.o process_record_3.o process_record_4.o record.o
				gcc -o process_record_1 process_record_1.o record.o
				gcc -o process_record_2 process_record_2.o record.o
				gcc -o process_record_3 process_record_3.o record.o
				gcc -o process_record_4 process_record_4.o record.o

process_record_1.o : process_record_1.c
				  gcc -c -w process_record_1.c
				  
process_record_2.o : process_record_2.c
				  gcc -c -w process_record_2.c
				  
process_record_3.o : process_record_3.c
				  gcc -c -w process_record_3.c
				  
process_record_4.o : process_record_4.c
				  gcc -c -w process_record_4.c
				  
record.o : ./helper/record.c
			gcc -c -w ./helper/record.c

clean :
			rm process_record_1 process_record_1.o process_record_2 process_record_2.o process_record_3 process_record_3.o process_record_4 process_record_4.o record.o