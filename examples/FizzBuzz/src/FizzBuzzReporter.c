#include <stdio.h>

#include "FizzBuzzReporter.h"

/*
 * Written as an example of a C program we want to test.
 */

// printf included to provide and example of how to mock with cgreen
void fizzBuzzReporter(const unsigned int input, char* const reportBuffer, const size_t bufferSize) {
    if((input % 15) == 0) {
        snprintf(reportBuffer, bufferSize, "FIZZBUZZ");
    } else if((input % 3) == 0) {
        snprintf(reportBuffer, bufferSize, "FIZZ");
    } else if((input % 5) == 0) {
        snprintf(reportBuffer, bufferSize, "BUZZ");
    } else {
        snprintf(reportBuffer, bufferSize, "%u", input);
    }

    printf("%u reports %s\n", input, reportBuffer);
}
