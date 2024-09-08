OBJ = $(patsubst %.c, %.o, $(wildcard *.c))
result.exe : $(OBJ)
	gcc -o $@ $^
clean :
	rm *.o *.exe
