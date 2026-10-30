#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Function to write a message to the log file
// Levels: DEBUG, INFO, WARNING, ERROR, CRITICAL/FATAL
void log_message(const char *level, const char *message) {
    // 1. Open the file in "a" (append) mode. 
    // This creates the file if it doesn't exist and adds text to the end.
    FILE *log_file = fopen("app.log", "w");
    if (log_file == NULL) {
        perror("Error opening log file");
        return;
    }

    // 2. Get current system timestamp
    time_t now;
    time(&now);
    struct tm *local = localtime(&now);

    char time_str[32];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", local);

    // 3. Write the formatted log entry
    fprintf(log_file, "[%s] [%s] %s\n", time_str, level, message);

    // 4. Close the file to flush the stream and save resources
    fclose(log_file);
}

void print_log_file() {
    // 1. Open the log file in "r" (read) mode
    FILE *file = fopen("app.log", "r");
    if (file == NULL) {
        perror("Could not open log file");
        return;
    }

    // 2. Create a buffer to hold one line of text at a time
    char buffer[256];

    printf("--- Printing contents of %s ---\n", "app.log");

    // 3. Read line-by-line until the end of the file (EOF)
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        printf("%s", buffer); // Print the line exactly as it is
    }

    printf("--- End of file ---\n");

    // 4. Always close the file handle
    fclose(file);
}