#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct node {
    char* name;
    struct node* xor_value;
} Node;

Node* calculate_xor_value(Node* before, Node* after) {
    return (Node*)((__intptr_t)before ^ (__intptr_t)after);
}

Node* make_new_node(const char *string) {
    Node *newNode = (Node*) malloc(sizeof(Node));
    newNode -> name = (char *)string;
    newNode -> xor_value = NULL;
    
    return newNode;
}

void print_list(Node* head) {
    Node *prev = NULL;
    Node *curr = head;
    Node* next;

    while (curr != NULL) {
        printf("%s -> ", curr -> name);
        next = calculate_xor_value(prev, curr -> xor_value);
        prev = curr;
        curr = next;
    }

    printf("NULL\n");
}

// Inserts the string at the beginning of the XOR linked list.
void insert_string(Node** head, const char* newObj) {
    if (strlen(newObj) > 64) {
        printf("Can't insert more than 64 characters in string");
    }

    Node *newNode = make_new_node(newObj);
    newNode -> xor_value = calculate_xor_value(NULL, *head);

    // We should only look for the next node if the list isn't empty
    if (*head != NULL) {
        Node *nextNode = calculate_xor_value(NULL, (*head) -> xor_value);
        (*head) -> xor_value = calculate_xor_value(newNode, nextNode);
    }

    *head = newNode;

// If possible, inserts before the string "before" and returns true. Returns false if not possible (e.g., the before string is not in the list).
int insert_before(Node** head, const char* before, const char* newObj) {
    return -1;
}

// If possible, inserts after the string "after" and returns true. Returns false if not possible (e.g., the after string is not in the list).
int insert_after(Node** head, const char* after, const char* newObj) {
    return -1;
}

// If possible removes the string at the beginning of the XOR Linked list and returns its value in result. If successful return true, otherwise returns false
int remove_string(Node** head, char* result) {
    return -1;
}

// If possible, removes the string after the string "after" and fills in the result buffer with its value. If successful return true, otherwise returns false
int remove_after(Node** head, const char *after, char *result) {
    return -1;
}

// If possible, removes the string before the string "before" and fills in the result buffer with its value. If successful return true, otherwise returns false.
int remove_before(Node** head, const char *before, char *result) {
    return -1;
}

// Removes all nodes and releases any memory allocated to the linked list
void free_all(Node** head) {
    
}

int main () {
    Node *head = NULL;
    insert_string(&head, "Alpha");
    insert_string(&head, "Bravo");
    print_list(head);
}