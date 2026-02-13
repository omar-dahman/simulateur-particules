OPTIONS = -Wall -Wextra -std=c99
CC = gcc
EXECUTABLE = simparticles
SRCDIR = src
OBJDIR = obj
BINDIR = bin
HEADERS = headers

# Variable to modify to enable or disable debug : leave empty or add -g
DEBUG = -g
# Variable holding the needed objects for linking
OBJECTS_NEEDED = #TO ADD HERE, name is to be $(OBJDIR)/myfile.o

.PHONY: all setup_dirs clean mrproper valgrind

all: build run

setup_dirs:
	mkdir -p $(OBJDIR) $(BINDIR)

clean:
	@rm -rf $(OBJDIR)/*

mrproper:
	@rm -rf $(BINDIR)/*

build: setup_dirs
	@echo "Compiling..."
	@make $(BINDIR)/$(EXECUTABLE)

run: $(BINDIR)/$(EXECUTABLE)
	@./$(BINDIR)/$(EXECUTABLE)

valgrind: $(BINDIR)/$(EXECUTABLE)
	valgrind --leak-check=full --track-origins=yes ./$(BINDIR)/$(EXECUTABLE)

$(BINDIR)/$(EXECUTABLE) : $(OBJECTS_NEEDED)
	@mkdir -p $(BINDIR)
	$(CC) $(OPTIONS) $(DEBUG) -o $@ $^

$(OBJDIR)/%.o : $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(OPTIONS) $(DEBUG) -c -o $@ $<

# TODO : add header tracking