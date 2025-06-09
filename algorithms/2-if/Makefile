# Compiler
CC = clang
CFLAGS = -I. -I$(PATHU) -I$(PATHS) -g -O2 -ffast-math -march=native
LDFLAGS = -lm

# Directories
PATHU = unity/src/
PATHS = src/
PATHT = test/
PATHB = build/
PATHD = build/depends/
PATHO = build/objs/
PATHR = build/results/

# Create all necessary directories
BUILD_PATHS = $(PATHB) $(PATHD) $(PATHO) $(PATHR)

EXECS = main
EXECT = main_test 

# Source files
MAIN_SRCS = $(wildcard $(PATHS)*.c)  # Main source files
TEST_SRCS = $(wildcard $(PATHT)*.c)  # Test source files
UNITY_SRC = $(PATHU)unity.c           # Unity source file

# Object files
MAIN_OBJS = $(MAIN_SRCS:$(PATHS)%.c=$(PATHO)%.o)
TEST_OBJS = $(TEST_SRCS:$(PATHT)%.c=$(PATHO)%.o)
UNITY_OBJ = $(PATHO)unity.o

TARGET_EXTENSION = out

.PHONY: all clean test

all: $(BUILD_PATHS) $(PATHB)$(EXECS).$(TARGET_EXTENSION)

# Build main executable
$(PATHB)$(EXECS).$(TARGET_EXTENSION): $(MAIN_OBJS)
	@echo "Linking main executable..."
	$(CC) -o $@ $^ $(LDFLAGS)

# Build test executable
$(PATHB)$(EXECT).$(TARGET_EXTENSION): $(TEST_OBJS) $(UNITY_OBJ)
	@echo "Linking test executable..."
	$(CC) -o $@ $^ $(LDFLAGS)

# Compile main source files
$(PATHO)%.o: $(PATHS)%.c
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Compile test source files
$(PATHO)%.o: $(PATHT)%.c
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -DTEST -c $< -o $@  # Define TEST for test compilation

# Compile Unity
$(UNITY_OBJ): $(UNITY_SRC)
	@echo "Compiling Unity..."
	$(CC) $(CFLAGS) -c $< -o $@

# Create directories if they don't exist
$(BUILD_PATHS):
	@mkdir -p $@

# Clean build artifacts
clean:
	@echo "Cleaning up..."
	rm -f $(PATHO)*.o
	rm -f $(PATHB)*.$(TARGET_EXTENSION)
	rm -f $(PATHR)*.txt

# Run tests
test: $(PATHB)$(EXECT).$(TARGET_EXTENSION)
	@echo "Running tests..."
	./$(PATHB)$(EXECT).$(TARGET_EXTENSION)

