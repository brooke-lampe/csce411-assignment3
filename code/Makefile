transform_record : transform_record.o record.o
					gcc -lm -o transform_record

transform_record.o : transform.c 
				  	gcc transform_record.c record.c 

clean :
					rm transform_record transform_record.o
