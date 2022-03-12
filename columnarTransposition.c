#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

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
    return final_string;
}

char* get_unsorted_string(char *key, char *string) {
    int key_length = strlen(key);
    int string_length = strlen(string);
    
    // Treat the final output as a matrix
    float message_rows = string_length / (float)key_length;

    int rows = ceil(message_rows) + 1;
    int columns = key_length + 1;

    char* encrypted_string = (char *) malloc(rows * columns * sizeof(char));

    for (int i = 0; i < key_length; i++) {
        encrypted_string[i] = key[i];
    }

    int cursor = key_length;

    for (int i = 0; i < string_length; i++) {
        if (i % (key_length) == 0) {
            encrypted_string[cursor] = '\n';
            cursor++;
        }

        encrypted_string[cursor] = string[i];
        cursor++;
    }

    for (int i = cursor; i < rows * columns - 1; i++) {
        encrypted_string[cursor] = 'X';
    }

    encrypted_string[rows * columns] = '\0';
    return encrypted_string;
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

    char *encrypted_string = get_unsorted_string(key, string);

    *result = encrypted_string;
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
int decrypt_columnar(const char *message_filename, const char *key_filename, char **result){
    return 1;   
}

int main() {
    char **result;
    encrypt_columnar("input_file.txt", "redacted_words.txt", result);
    printf("%s", *result);
    free(result);
    return EXIT_SUCCESS;
}