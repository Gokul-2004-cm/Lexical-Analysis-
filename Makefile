CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
TARGET = lexical_analyzer
SRCDIR = src
BINDIR = bin
OBJDIR = obj

SOURCES = $(SRCDIR)/main.c $(SRCDIR)/lexer.c $(SRCDIR)/token.c $(SRCDIR)/symbolTable.c
OBJECTS = $(OBJDIR)/main.o $(OBJDIR)/lexer.o $(OBJDIR)/token.o $(OBJDIR)/symbolTable.o
HEADERS = $(SRCDIR)/*.h

all: $(BINDIR)/$(TARGET)

$(BINDIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^
	@echo "Build successful! Run with: ./bin/lexical_analyzer"

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(HEADERS)
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

run: $(BINDIR)/$(TARGET)
	./$(BINDIR)/$(TARGET)

clean:
	rm -rf $(OBJDIR) $(BINDIR)
	@echo "Cleanup complete!"

.PHONY: all run clean