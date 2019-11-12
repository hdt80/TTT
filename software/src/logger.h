#pragma once

#include <stdio.h>
#include <stdarg.h>

#define CLR_RED		"\x1b[31m"
#define CLR_GREEN	"\x1b[32m"
#define CLR_YELLOW	"\x1b[33m"
#define CLR_BLUE	"\x1b[34m"
#define CLR_PURPLE	"\x1b[35m"
#define CLR_CYAN	"\x1b[36m"
#define CLR_RESET	"\x1b[0m"

#if DEBUG
#pragma message "Building debug version"
#endif

#define infof(fmt, ...) \
	do { msgf(stdout, "I", CLR_BLUE, fmt, ##__VA_ARGS__); } while (0)

#define warnf(fmt, ...) \
	do { msgf(stdout, "W", CLR_YELLOW, fmt, ##__VA_ARGS__); } while (0)

#define errorf(fmt, ...) \
	do { msgf(stderr, "E", CLR_RED, fmt, ##__VA_ARGS__); } while (0)

#if DEBUG
#define debugf(fmt, ...) \
	do { msgf(stdout, "D", CLR_PURPLE, fmt, ##__VA_ARGS__); } while(0)
#else
#define debugf(fmt, ...) (void)0
#endif


void msgf(FILE* file, const char* tag, const char* color, const char* fmt, ...) {
	fprintf(file, "%s[%s%s%s] ", CLR_RESET, color, tag, CLR_RESET);

	va_list args;
	va_start(args, fmt);
	vfprintf(file, fmt, args);
	va_end(args);
}

