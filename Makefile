INCLUDE = -I include
LDFLAGS = -ldl -lglfw

all: src/hello.c src/glad.c
	gcc -o out/hello $(INCLUDE) src/hello.c src/glad.c $(LDFLAGS)

clean:
	rm out/*
	
.PHONY: all clean
