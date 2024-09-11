CPPC=gcc
CODEDIRS= . ./src
INCDIRS= . /usr/include/GL

FLAGS=-Wall $(foreach D, $(INCDIRS), -I$D) -g
LIBFLAGS=-lGL -lGLEW -lglut
FILES=$(foreach D, $(CODEDIRS), $(wildcard $D/*.c))
BINARY=bin
OBJ=$(patsubst %.c,%.o,$(FILES))

all: $(BINARY) 

$(BINARY): $(OBJ)
	$(CPPC) $^ -o $@ $(LIBFLAGS)
%.o: %.c
	$(CPPC) -c $(FLAGS) -o $@ $^

clean:
	rm -r $(OBJ) $(BINARY)
run: $(BINARY)
	./$(BINARY)
stage: clean
	git status
	git add .
