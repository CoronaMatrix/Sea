run: ./main.c ./vm.c  ./compiler.c ./parser.c ./expr.c ./stmt.c ./scanner.c utils/int_array.c utils/value_array.c utils/sym_table.c ./table.c ./object.c
	cc -o main -g ./main.c ./compiler.c ./vm.c ./parser.c ./expr.c ./stmt.c ./scanner.c utils/int_array.c utils/value_array.c utils/sym_table.c ./table.c ./object.c; ./main ./test1.sea
	
clean:
	rm -f main *.o
