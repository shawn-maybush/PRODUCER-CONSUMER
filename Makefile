# Compiler and flags
CC = g++
CFLAGS = -pthread -Wall -Wextra -pedantic

# Source and object files
SOURCES = main.cpp buffer.cpp producer.cpp consumer.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = producer_consumer

# Build rule
all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

# Rule to compile .cpp files into .o files
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
