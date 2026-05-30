SRCDIR = src
OBJDIR = obj
BINDIR = bin
HEADERSDIR = headers
IMAGESDIR = output

FLAGS = -Wall -Wextra -std=c99 -I$(HEADERSDIR)
OPTIONS = $(FLAGS) $(GTK_FLAGS)
CC = gcc
EXECUTABLE = simparticles
GTK_EXEC = simparticles_gtk

HEADERS = $(wildcard $(HEADERSDIR)/*.h)
ALL_OBJECTS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(wildcard $(SRCDIR)/*.c))
OBJECTS_NEEDED = $(filter-out $(GTK_FILTEROUT_OBJECTS), $(ALL_OBJECTS))
GTK_OBJ = $(filter-out $(OBJDIR)/main.o, $(ALL_OBJECTS))
GTK_FILTEROUT_SRC := $(shell find src -type f -name "*.c" -exec grep -il "gtk" {} +)
GTK_FILTEROUT_OBJECTS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(GTK_FILTEROUT_SRC))

GTK_FLAGS = `pkg-config --cflags gtk+-3.0`
GTK_LIBS  = `pkg-config --libs gtk+-3.0`

# Variable to modify to enable or disable debug : leave empty or add -g
DEBUG = -g
# Variable to add library if needed, like maths with -lm
LIBRARIES = -lm

.PHONY: all clean mrproper build run valgrind docs build-gtk run-gtk

all: build run

clean:
	@rm -rf $(OBJDIR)/*
	@rm -rf $(IMAGESDIR)/*
	@find docs/ -mindepth 1 ! -name 'Doxyfile' -exec rm -rf {} +
	@echo "Cleaning done"

mrproper:
	@rm -rf $(BINDIR)/*

build:
	@echo "Compiling simulation..."
	@make $(BINDIR)/$(EXECUTABLE)

run: $(BINDIR)/$(EXECUTABLE)
	@mkdir -p $(IMAGESDIR)
	@./$(BINDIR)/$(EXECUTABLE)

valgrind: $(BINDIR)/$(EXECUTABLE)
	@echo "Memory check on simulation..."
	valgrind --leak-check=full --track-origins=yes ./$(BINDIR)/$(EXECUTABLE)

docs:
	cd docs && doxygen Doxyfile

build-gtk: $(BINDIR)/$(GTK_EXEC)

run-gtk: $(BINDIR)/$(GTK_EXEC)
	@mkdir -p $(BINDIR)
	@./$(BINDIR)/$(GTK_EXEC)

$(BINDIR)/$(GTK_EXEC): $(GTK_OBJ)
	@mkdir -p $(BINDIR)
	$(CC) $(OPTIONS) $(DEBUG) -o $@ $^ $(LIBRARIES) $(GTK_LIBS)

$(BINDIR)/$(EXECUTABLE) : $(OBJECTS_NEEDED)
	@mkdir -p $(BINDIR)
	$(CC) $(OPTIONS) $(DEBUG) -o $@ $^ $(LIBRARIES)

$(OBJDIR)/%.o : $(SRCDIR)/%.c $(HEADERS)
	@mkdir -p $(OBJDIR)
	$(CC) $(OPTIONS) $(DEBUG) -c -o $@ $<