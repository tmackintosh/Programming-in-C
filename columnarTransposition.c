#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

/* 
 *  A function which takes in a filename and returns the contents as a string, 
 *  with all non-alphanumeric (i.e., not A-Z, a-z, or 0-9) removed and characters
 *  converted to uppercase.
 */
char *read_string(const char *filename){
    char *final_string = (char *) malloc(1024 * sizeof(char));

    if (final_string == NULL) {
        printf("Dynamic memory allocation failed.");
        return '\0';
    }

    FILE *input_stream = fopen(filename, "r");
    char buffer[1024];
    int number_of_read_bytes;
    int total_number_of_read_bytes = 0;
    int cursor = 0;

    // Use do-while loop to ensure the contents run at least once
    do {
        number_of_read_bytes = fread(buffer, 1, 1024, input_stream);
        total_number_of_read_bytes += number_of_read_bytes;

        if (total_number_of_read_bytes != 1024) {
            final_string = realloc(final_string, total_number_of_read_bytes);
        }

        for (int i = 0; i < number_of_read_bytes; i++) {
            char character = buffer[i];

            if (isalpha(character)) {
                final_string[cursor] = toupper(character);
                cursor++;
            }
        }

    } while (number_of_read_bytes > 0);
    
    final_string[cursor] = '\0';

    fclose(input_stream);
    return final_string;
}

// Returns a formatted string in from:
// KEY\n
// ABC\n
// DXX\0
// @param *key the string of the key
// @param *string the string of the string to format
// @returns the encoded string
char* get_formatted_string(char *key, char *string) {
    int key_length = strlen(key);
    int string_length = strlen(string);
    
    // Treat the final output as a matrix with \n and \0 defining rows and columns
    float message_rows = string_length / (float)key_length;

    // Add 1 for the escape or new line character
    int rows = ceil(message_rows) + 1;
    // Add 1 for the key being put at the top
    int columns = key_length + 1;

    char* encrypted_string = (char *) malloc(rows * columns * sizeof(char));

    if (encrypted_string == NULL) {
        printf("Dynamic memory allocation failed.");
        return '\0';
    }

    // Add key onto the top row
    for (int i = 0; i < key_length; i++) {
        encrypted_string[i] = key[i];
    }

    int cursor = key_length;

    for (int i = 0; i < string_length; i++) {
        // Add new line characters where necessary
        if (i % (key_length) == 0) {
            encrypted_string[cursor] = '\n';
            cursor++;
        }

        encrypted_string[cursor] = string[i];
        cursor++;
    }

    // Fill in Xs where necessary
    for (int i = cursor; i < rows * columns - 1; i++) {
        encrypted_string[i] = 'X';
    }

    encrypted_string[rows * columns - 1] = '\0';
    return encrypted_string;
}

void swap_column(char *formatted_string, int key_length, int column1, int column2) {
    int columns = key_length + 1;

    for (int i = 0; i < columns - 1; i++) {
        int index1 = (i * columns) + column1;
        int index2 = (i * columns) + column2;

        char temp = formatted_string[index1];
        formatted_string[index1] = formatted_string[index2];
        formatted_string[index2] = temp;
    }
}

void columnar_transposition(char *formatted_string, char* key) {
    // Bubble sort the key, and swap the columns along with them
    bool swapped = true;
    int iterations = 0;

    while (swapped) {
        swapped = false;
        iterations++;

        for (int i = 0; i < strlen(key) - iterations; i++) {
            char character = key[i];
            char next_character = key[i + 1];

            if (character > next_character) {
                key[i] = next_character;
                key[i + 1] = character;
                swap_column(formatted_string, strlen(key), i, i + 1);
                swapped = true;
            }
        }
    }
}

char* get_encrypted_text(char *formatted_string, int key_length) {
    int columns = key_length + 1;
    int rows = strlen(formatted_string) / columns;

    char *new_string = (char *) malloc(columns * rows * sizeof(char));
    int cursor = 0;

    if (new_string == NULL) {
        printf("Dynamic memory allocation failed.");
        return '\0';
    }
    
    for (int i = key_length; i < strlen(formatted_string); i++) {
        char character = formatted_string[i];

        if (character != '\n' && character != '\0') {
            new_string[cursor] = character;
            cursor++;
        }
    }

    new_string[cursor] = '\0';

    free(formatted_string);
    return new_string;
}

/* 
 *  A function which takes in a filename and encrypts the contents using the
 *  key string which is obtained from the "key_filename" file. The encrypted 
 *  message should be returned using the "result" buffer, which you should 
 *  assume has not been initialised or any memory allocated to it.
 */
void encrypt_columnar(const char *message_filename, const char *key_filename, char **result){
    char *string = read_string(message_filename);
    char *key = read_string(key_filename);

    char *formatted_string = get_formatted_string(key, string);
    columnar_transposition(formatted_string, key);
    *result = get_encrypted_text(formatted_string, strlen(key));

    free(string);
    free(key);
}

/* 
 *  A function which takes in a string and decrypts the contents using the
 *  key string which is obtained from the "key_filename" file. The decrypted 
 *  message should be returned using the "result" buffer, which you should 
 *  assume has not been initialised or any memory allocated to it. The function 
 *  should return true if decryption was successful, false if not.
 */
int decrypt_columnar(const char *message_filename, const char *key_filename, char **result) {
    encrypt_columnar(message_filename, key_filename, result);
    return 0;
}

int main() {
    char **result;
    encrypt_columnar("input_file.txt", "redacted_words.txt", result);
    printf("%s\n", *result);

    FILE *output_stream = fopen("result.txt", "w");
    fwrite(*result, 1, strlen(*result), output_stream);
    fclose(output_stream);

    decrypt_columnar("result.txt", "redacted_words.txt", result);
    printf("%s\n", *result);
    free(*result);

    return EXIT_SUCCESS;
}