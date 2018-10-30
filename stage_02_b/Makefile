transform_and_sort_record : transform_and_sort_record.o record.o
				gcc -lm -o transform_and_sort_record transform_and_sort_record.o record.o

transform_and_sort_record.o : transform_and_sort_record.c
				  gcc -c -w transform_and_sort_record.c
				  				  
record.o : ./helper/record.c
			gcc -c -w ./helper/record.c

clean :
			rm transform_and_sort_record transform_and_sort_record.o record.o
			rm -rf data