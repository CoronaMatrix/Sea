run: ./main.c ./vm.c ./compiler.c ./scanner.c utils/int_array.c utils/value_array.c ./table.c ./object.c
	cc -o main -g ./main.c ./vm.c ./compiler.c ./scanner.c utils/int_array.c utils/value_array.c ./table.c ./object.c; ./main ./test1.sea
	
clean:
	rm -f main *.o
