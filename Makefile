INCLUDE = -I include
LDFLAGS = -ldl -lglfw

all: src/hello.cpp src/glad.c
	g++ -o out/hello $(INCLUDE) src/hello.cpp src/glad.c $(LDFLAGS)

clean:
	rm out/*
	
.PHONY: all clean