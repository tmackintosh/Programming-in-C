#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

void get_redacted_words(char redacted_words[], const char* redact_words_filename) {
    FILE* redacted_words_stream = fopen(redact_words_filename, "r");
    int number_of_bytes_read;
    char buffer[1024];
    int cursor = 0;

    do {
        number_of_bytes_read = fread(buffer, 1, 1024, redacted_words_stream);
        
        for (int i = 0; i < number_of_bytes_read; i++) {
            char character = buffer[i];

            if (character == ',') {
                character = '\0';
            } else if (!isalpha(character)) {
                continue;
            }

            redacted_words[cursor] = tolower(character);
            cursor++;
        }

    } while (number_of_bytes_read > 0);

    redacted_words[cursor] = '\0';

    fclose(redacted_words_stream);
}

bool redact_individual_word(char word[], int word_length, char redacted_words[], int number_of_bytes_read) {
    char new_word[1024];
    int cursor = 0;

    word[word_length] = '\0';

    for (int i = 0; i < number_of_bytes_read; i++) {
        char character = redacted_words[i];
        new_word[cursor] = character;

        if (character == '\0') {
            if (cursor != 0 && memcmp(new_word, word, cursor) == 0) {
                return true;
            }

            cursor = 0;
        } else {
            cursor++;
        }
    }

    return false;
}

void redact(char* buffer, char* word, int word_length, char* redacted_words, int number_of_read_bytes, int starting_index) {
    printf("%s %d\n", word, word_length);
    if (redact_individual_word(word, word_length, redacted_words, number_of_read_bytes)) {
        for (int j = 0; j < word_length; j++) {
            buffer[starting_index + j] = '*';
        }
    };
}

void redact_line(char buffer[], int number_of_read_bytes, char redacted_words[]) {
    int starting_index = 0;
    int ending_index = -1;
    char substring[1024];

    for (int i = 0; i < number_of_read_bytes; i++) {
        char character = buffer[i];

        if (character == ' ') {
            ending_index = i;
            redact(buffer, substring, ending_index - starting_index, redacted_words, number_of_read_bytes, starting_index);
            starting_index = ending_index + 1;
        } else {
            substring[i - ending_index - 1] = tolower(character);
        }
    }

    redact(buffer, substring, number_of_read_bytes - starting_index, redacted_words, number_of_read_bytes, starting_index);
}

void redact_words(const char *text_filename, const char *redact_words_filename){
    char redacted_words[1024];
    get_redacted_words(redacted_words, redact_words_filename);

    FILE *input_stream, *output_stream;

    input_stream = fopen(text_filename, "r");
    output_stream = fopen("result.txt", "w");

    char buffer[1024];
    int number_of_read_bytes;

    // Use do-while loop to ensure the contents run at least once
    do {
        number_of_read_bytes = fread(buffer, 1, 1024, input_stream);
        redact_line(buffer, number_of_read_bytes, redacted_words);
        fwrite(buffer, 1, number_of_read_bytes, output_stream);
    } while (number_of_read_bytes > 0);

    fclose(input_stream);
    fclose(output_stream);
}

int main() {
    const char *input_file = "input_file.txt";
    const char *redact_file = "redacted_words.txt";
    redact_words(input_file, redact_file);

    char line[1024];
    FILE* file = fopen("result.txt", "r");
    if(!file){
        printf("Unable to open result.txt\n");
    }
    else{
        while (fgets(line, sizeof(line), file)) {
            printf("%s", line);
        }
        fclose(file);
    }
}