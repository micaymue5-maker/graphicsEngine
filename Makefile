CC = clang
PKG_CONFIG ?= pkg-config

TARGET := build/pixel_shapes
SOURCES := $(wildcard src/*.c)
OBJECTS := $(patsubst src/%.c,build/%.o,$(SOURCES))
DEPS := $(OBJECTS:.o=.d)

CPPFLAGS += -Iinclude $(shell $(PKG_CONFIG) --cflags raylib)
CFLAGS += -std=c17 -Wall -Wextra -Wpedantic -g -O0
LDLIBS += $(shell $(PKG_CONFIG) --libs raylib)

.PHONY: all run
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) $(LDLIBS) -o $@

build/%.o: src/%.c Makefile | build
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -MP -c $< -o $@

build:
	mkdir -p $@

run: $(TARGET)
	./$(TARGET)

-include $(DEPS)
