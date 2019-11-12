# Compiler
CXX = gcc

# Directories used for input and output
SRCDIR = src
BUILDDIR = build
EXEDIR = bin

# Output name of the compiled library
OUTPUT_NAME = libttt
TEST_OUTPUT_NAME = ttt_test

# Where the sources are located
SRCS = $(wildcard $(SRCDIR)/*.c)

# Where the compiled objects are located
OBJS = $(patsubst $(SRCDIR)/%.c, $(BUILDDIR)/%.o, $(SRCS))

# Enable all warnings but format and unused variables
CXX_WARNS += -Wall -Wextra -Wno-unused-parameter -Wno-format \
			 -Wno-unused-variable -Wno-varargs

# Flags used to compile .cpp files
CXX_FLAGS += $(CXX_WARNS) -c -g -fPIC

# Compiling all the .cpp files into .o files
$(OBJS): $(BUILDDIR)/%.o : $(SRCDIR)/%.c
	$(CXX) $(CXX_FLAGS) -o "$@" "$<" -DDEBUG

# Building the library file
.PHONY: build
build: $(OBJS)
	ar rcs $(EXEDIR)/$(OUTPUT_NAME).a $^

.PHONY: test
test: build
	gcc ./src/test_program.c $(CXX_WARNS) -l ttt -L bin -pthread -lm -DDEBUG

.PHONY: clean
clean:
	rm ./a.out -f
	rm ./build/*
