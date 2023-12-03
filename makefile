# Define directories
BUILDDIR = build
SRCDIR = src
BINDIR = bin
INCDIR = include

# Define the compiler
CC = cc

# Define compilation flags
CFLAGS = -I $(INCDIR)

# List of source files
SRCFILES = $(SRCDIR)/main.c $(SRCDIR)/tarsau.c

# List of object files
OBJFILES = $(BUILDDIR)/main.o $(BUILDDIR)/tarsau.o

# Output executable
OUTPUT = $(BINDIR)/app

# Rule to build the executable
$(OUTPUT): $(OBJFILES)
	$(CC) -o $(OUTPUT) $(OBJFILES)

# Rule to build object files
$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@echo "Compiling $<..."
	$(CC) -c $(CFLAGS) $< -o $@

# Phony target to clean the project
clean:
	rm -rf $(BUILDDIR)/*.o $(BINDIR)/*

# Dependencies
$(BUILDDIR)/main.o: $(INCDIR)/tarsau.h
$(BUILDDIR)/tarsau.o: $(INCDIR)/tarsau.h
