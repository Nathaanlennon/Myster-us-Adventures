# Nom du programme
TARGET = Mysterus_adventure

# Compilateur
CC = gcc

# Options de compilation
CFLAGS = -Wall -Wextra

# Liste des fichiers source
SOURCES =test.c usual.c game.c

# Liste des fichiers objets correspondant aux fichiers source
OBJECTS = $(SOURCES:.c=.o)

# Règle par défaut
all: $(TARGET)

external/rgr/lib/libGameRGR2.so:
	cd ./external/rgr && mkdir -p lib && $(MAKE) lib/libGameRGR2.so

# Règle de compilation des fichiers objets
%.o: %.c external/rgr/lib/libGameRGR2.so
	$(CC) $(CFLAGS) -Iinclude -Iexternal/rgr/inc -Lexternal/rgr/lib -lGameRGR2 -lncursesw  -c $< -o $@

# Règle de construction du programme
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -Iinclude -Iexternal/rgr/inc -Lexternal/rgr/lib $(OBJECTS) -o $@ -lGameRGR2 -lncursesw

# Règle de nettoyage des fichiers objets et du programme
clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: build
build: $(TARGET)

.PHONY: run
run: build
	LD_LIBRARY_PATH=external/rgr/lib ./$(TARGET)