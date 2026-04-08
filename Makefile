CC = gcc
CFLAGS = -std=c23 -Wall -Wextra -O2 -D_POSIX_C_SOURCE=200809L -Iinclude
LDFLAGS =

TARGET = rsvc
SRCDIR = src
INCDIR = include
OBJDIR = obj

SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SOURCES))

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $@

clean:
	rm -rf $(OBJDIR) $(TARGET)

install: $(TARGET)
	install -Dm755 $(TARGET) /usr/local/bin/rsvc

.PHONY: all clean install
