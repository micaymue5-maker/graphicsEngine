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

# Headless checks replace raylib's window/pixel functions with capture stubs.
.PHONY: test
test: build/test_engine
	./build/test_engine

build/main_test.o: src/main.c Makefile | build
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -MP -Dmain=demo_main -c $< -o $@

build/test_engine.o: tests/test_engine.c Makefile | build
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -MP -c $< -o $@

build/test_engine: build/test_engine.o build/main_test.o build/raster.o build/shapes.o build/vectors.o
	$(CC) $(LDFLAGS) $^ -lm -o $@

-include build/main_test.d build/test_engine.d
