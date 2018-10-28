write_csv : write_csv.o record.o
				gcc -o write_csv write_csv.o record.o

write_csv.o : write_csv.c
				  gcc -c -w write_csv.c
				  				  
record.o : ./helper/record.c
			gcc -c -w ./helper/record.c

clean :
			rm write_csv write_csv.o record.o Users.csv Messages.csv