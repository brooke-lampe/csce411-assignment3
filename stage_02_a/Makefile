transform_record : transform_record.o record.o
				gcc -lm -o transform_record transform_record.o record.o

transform_record.o : transform_record.c
				  gcc -c -w transform_record.c
				  				  
record.o : ./helper/record.c
			gcc -c -w ./helper/record.c

clean :
			rm transform_record transform_record.o record.o
			rm -rf data