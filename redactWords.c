#include <stdio.h>

void print_word(char substring[], int word_length) {
    for (int i = 0; i < word_length; i++) {
        printf("%c", substring[i]);
    }
    printf("\n");
}

void redact_line(char buffer[], int number_of_read_bytes) {
    int starting_index = 0;
    int ending_index = -1;
    char substring[1024];

    for (int i = 0; i < number_of_read_bytes; i++) {
        char character = buffer[i];

        if (character == ' ') {
            ending_index = i;
            print_word(substring, ending_index - starting_index);
            starting_index = ending_index + 1;
        } else {
            substring[i - ending_index - 1] = character;
        }
    }

    print_word(substring, number_of_read_bytes - starting_index);
}

void redact_words(const char *text_filename, const char *redact_words_filename){
    FILE *input_stream, *redacted_words_stream, *output_stream;

    input_stream = fopen(text_filename, "r");
    redacted_words_stream = fopen(redact_words_filename, "r");
    output_stream = fopen("result.txt", "w");

    char buffer[1024];
    int number_of_read_bytes;

    // Use do-while loop to ensure the contents run at least once
    do {
        number_of_read_bytes = fread(buffer, 1, 1024, input_stream);
        redact_line(buffer, number_of_read_bytes);
        fwrite(buffer, 1, number_of_read_bytes, output_stream);
    } while (number_of_read_bytes > 0);

    fclose(input_stream);
    fclose(redacted_words_stream);
    fclose(output_stream);
}

int main() {
    char* input_name = "input_file.txt";
    char* redact_words_filename = "redacted_words.txt";

    redact_words(input_name, redact_words_filename);

    return 0;
}