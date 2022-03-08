#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

// Creates an array of words separated by the escape character
// @param redacted_words char[] of the array to put the words into
// @param redact_words_filename char[] of the string name of the file to import redacted words from
int get_redacted_words(char redacted_words[], const char* redact_words_filename) {
    FILE* redacted_words_stream = fopen(redact_words_filename, "r");
    int number_of_bytes_read;
    char buffer[1024];
    int cursor = 0;
    int total_bytes_read = 0;

    do {
        number_of_bytes_read = fread(buffer, 1, 1024, redacted_words_stream);
        total_bytes_read += number_of_bytes_read;

        for (int i = 0; i < number_of_bytes_read; i++) {
            char character = buffer[i];

            if (character == ',') {
                character = '\0';
            } else if (!isalpha(character)) {
                continue;
            }

            // Increment cursor in the case that the do-while loop iterates more than once
            // we can continue writing to the array contiguously
            redacted_words[cursor] = tolower(character);
            cursor++;
        }

    } while (number_of_bytes_read > 0);

    // Close final word with escape character for ease of iteration in the future
    redacted_words[cursor] = '\0';

    fclose(redacted_words_stream);

    return total_bytes_read;
}

// Iterates through the redacted words array and compares each against the word given to see whether
// the word should be redacted or not
// @param word char[] of the word to redact
// @param word_length int of how long the word is
// @param redacted_words char[] the array of redacted words
// @param redacted_word_size int of how many bytes are in the redacted words
bool is_word_redacted(char word[], int word_length, char redacted_words[], int redacted_word_size) {
    char new_word[1024];
    int cursor = 0;

    for (int i = 0; i < redacted_word_size; i++) {
        char character = redacted_words[i];
        new_word[cursor] = character;

        if (character == '\0') {
            if (cursor != 0 && memcmp(new_word, word, cursor) == 0) {
                // We need to make sure the redacted word doesn't form the first
                // part of the word by checking whether the word has been completed or
                // not (ie the next letter is the escape character)
                // For example, "password" isn't redacted by the word "pass"
                if (cursor != word_length - 1 && new_word[cursor + 1] == '\0') {
                    return true;
                }
            }

            // Reset the cursor to restart writing to the next word
            cursor = 0;
        } else {
            cursor++;
        }
    }

    return false;
}

// If the word should be redacted, replaces the word with * in the original char-array.
// @param buffer char* of the original char-array to redact from
// @param word char* of the word to potentially redact
// @param word_length int the length of the word to potentially redact
// @param redacted_words char* the array of words that should be redacted
// @param redacted_word_size int of how many bytes redacted_words is
// @param starting_index int of whereabouts in buffer the word starts
void redact(char* buffer, char* word, int word_length, char* redacted_words, int redacted_word_size, int starting_index) {
    if (is_word_redacted(word, word_length, redacted_words, redacted_word_size)) {
        for (int i = 0; i < word_length; i++) {
            buffer[starting_index + i] = '*';
        }
    };
}

// Takes a line that has been read from the file and scans all words for any that should be redacted.
// Redacts them as necessary.
// @param buffer char[] the line that has been read from the file
// @param buffer_size int of how many bytes the buffer is
// @param redacted_words char[] the array of redacted words
// @param redacted_words_size int of how many bytes redacted_words is
void redact_line(char buffer[], int buffer_size, char redacted_words[], int redacted_words_size) {
    int starting_index = 0;
    int ending_index = -1;
    char substring[1024];

    for (int i = 0; i < buffer_size; i++) {
        char character = buffer[i];

        if (character == ' ') {
            ending_index = i;
            redact(buffer, substring, ending_index - starting_index, redacted_words, redacted_words_size, starting_index);
            starting_index = ending_index + 1;
        } else {
            substring[i - ending_index - 1] = tolower(character);
        }
    }

    // Try to redact the final part of the line
    redact(buffer, substring, buffer_size - starting_index, redacted_words, redacted_words_size, starting_index);
}

// Given a file of text, and a file of words to redact, amends the text file with * instead of words marked as redacted.
// @param text_filename char* the name of the textfile to amend
// @param redact_words_filename char* the name of the textfile containing the redacted words, must be in CSV format
void redact_words(const char *text_filename, const char *redact_words_filename){
    // Create once, use many times
    char redacted_words[1024];
    int redacted_word_size = get_redacted_words(redacted_words, redact_words_filename);

    FILE *input_stream, *output_stream;

    input_stream = fopen(text_filename, "r");
    output_stream = fopen("result.txt", "w");

    char buffer[1024];
    int number_of_read_bytes;

    // Use do-while loop to ensure the contents run at least once
    do {
        number_of_read_bytes = fread(buffer, 1, 1024, input_stream);
        redact_line(buffer, number_of_read_bytes, redacted_words, redacted_word_size);
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