objects = src/main.o src/system.o src/auth.o src/validation.o  # Added validation.o

atm : $(objects)
	cc -o atm $(objects)

# Rules for compiling .c files into .o files
src/%.o: src/%.c src/header.h
	cc -c $< -o $@

clean :
	rm -f $(objects) atm