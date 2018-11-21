#Edit this for different files!
SOURCE=slave.cpp

#Don't edit the rest
EXECUTABLE=output
CC=g++

all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCE)

	$(CC) -std=c++11 -lpigpio -lpthread -lrt -o $(EXECUTABLE) $(SOURCE)

clean:

	rm -f $(EXECUTABLE)
