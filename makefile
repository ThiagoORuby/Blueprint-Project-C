compile:
	@gcc -g *.c src/*.c src/*.h -o main -lGL -lGLU -lglut -lm

run:
	./main

