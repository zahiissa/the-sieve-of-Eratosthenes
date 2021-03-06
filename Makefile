CC = gcc

OBJS = sieve_of_eratosthenes.o


EXEC = main

COMP_FLAG = -lpthread

OUTPUT_FILE = -o

COMPILE_FILES = $(CC) -c $(COMP_FLAG)

BUILD_FILES= $(CC)  $(COMP_FLAG)
		
$(EXEC) : $(OBJS)
		$(BUILD_FILES) $(OBJS) $(OUTPUT_FILE) $@

sieve_of_eratosthenes.o: sieve_of_eratosthenes.c list.h
		$(COMPILE_FILES) $*.c


clean:
		rm -f $(OBJS) $(EXEC)



 
