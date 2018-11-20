#Edit this for different files!
SOURCE=slave.cpp

#Don't edit the rest
EXECUTABLE=output
CC=g++

all: $(EXECUTABLE)

g++ -std=c++11 –lpigpio –lpthread –lrt –o hello_pi hello_pi.cpp

$(EXECUTABLE): $(SOURCE)

	$(CC) -std=c++11 –lpigpio –lpthread –lrt $(SOURCE) -o$(EXECUTABLE)

clean:

	rm -f $(EXECUTABLE)