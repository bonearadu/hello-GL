IDIR   = -I include
LFLAGS = -ldl -lglfw

all: src/hello.cpp src/glad.c
	g++ -o out/hello $(IDIR) src/hello.cpp src/glad.c $(LFLAGS)

clean:
	rm out/*
	
.PHONY: all clean