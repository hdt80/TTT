DEVICE_PORT=ttyUSB0

# Compiler
GCC = avr-gcc

# Directories used for input and output
SRCDIR = src
BUILDDIR = build
INCLUDEDIR = src

# Name to output the built file to
OUTPUT_NAME = ttt

# Where the sources are located
SRCS = $(wildcard $(SRCDIR)/*.c)
SRCS += $(wildcard $(SRCDIR)/*/*.c)

# Where the compiled objects are located
OBJS = $(patsubst $(SRCDIR)/%.c, $(BUILDDIR)/%.o, $(SRCS))

# Flags for the compiler
GCC_FLAGS += -Wall -Wextra -Wno-unused-variable -Wno-unused-parameter -mmcu=atmega328p -c -DF_CPU=16000000UL

# Minimake files used to dependency rebuilding
DEPS := $(OBJS:.o=.d)
-include $(DEPS)

# Linking all the .o files and with the libs
build: $(OBJS)
	$(GCC) $^ -mmcu=atmega328p -o ./bin/$(OUTPUT_NAME)
	avr-objcopy -O ihex -R .eeprom ./bin/$(OUTPUT_NAME) ./bin/$(OUTPUT_NAME).hex

# Compiling all the .c files into .o files
$(OBJS): $(BUILDDIR)/%.o : $(SRCDIR)/%.c
	$(GCC) $(GCC_FLAGS) -o "$@" "$<"

# Running the created exe
.PHONY: run
run: build
	avrdude -F -V -c arduino -p ATMEGA328P -P /dev/$(DEVICE_PORT) -b 115200 -U flash:w:./bin/$(OUTPUT_NAME).hex
	
# Cleaning everything up
.PHONY: clean
clean:
	rm -rf bin/*
	rm -rf $(BUILDDIR)/*
	
.PHONY: port
port: run
	dterm /dev/${DEVICE_PORT}
