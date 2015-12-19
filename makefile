CC = gcc

LaTeX_Generator: ./object/LaTeX_Generator.o
	${CC} -o ./LaTeX_Generator ./object/LaTeX_Generator.o
./object/LaTeX_Generator.o: ./LaTeX_Generator.c
	${CC} -c -o ./object/LaTeX_Generator.o ./LaTeX_Generator.c
