# Compilador y opciones
CC = gcc

# Archivos
TARGET = mutex
SRC = mutex.c

# Regla principal
all: $(TARGET)

# Cómo crear el ejecutable
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

run: $(TARGET)
	@./$(TARGET)
