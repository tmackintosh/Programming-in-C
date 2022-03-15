#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

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
    if (strlen(newObj) > 64 / sizeof(char)) {
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
}

// If possible, inserts before the string "before" and returns true. Returns false if not possible (e.g., the before string is not in the list).
int insert_before(Node** head, const char* before, const char* newObj) {
    Node *previousNode = NULL;
    Node *currentNode = *head;
    Node *nextNode;

    while (currentNode != NULL) {
        nextNode = calculate_xor_value(previousNode, currentNode -> xor_value);

        if (currentNode -> name == before) {
            if (currentNode == *head) {
                insert_string(head, newObj);
                return true;
            }

            Node *newNode = make_new_node(newObj);
            newNode -> xor_value = calculate_xor_value(previousNode, currentNode);

            if (previousNode != NULL) {
                Node *previousPreviousNode = calculate_xor_value(previousNode -> xor_value, currentNode);
                previousNode -> xor_value = calculate_xor_value(previousPreviousNode, newNode);
            }
            
            currentNode -> xor_value = calculate_xor_value(newNode, nextNode);

            return true;
        }

        previousNode = currentNode;
        currentNode = nextNode;
    }

    return false;
}

// If possible, inserts after the string "after" and returns true. Returns false if not possible (e.g., the after string is not in the list).
int insert_after(Node** head, const char* after, const char* newObj) {
    Node *previousNode = NULL;
    Node *currentNode = *head;
    Node *nextNode;

    while (currentNode != NULL) {
        nextNode = calculate_xor_value(previousNode, currentNode -> xor_value);

        if (currentNode -> name == after) {
            Node *newNode = make_new_node(newObj);
            currentNode -> xor_value = calculate_xor_value(previousNode, newNode);
            newNode -> xor_value = calculate_xor_value(currentNode, nextNode);

            if (nextNode != NULL) {
                Node *nextNextNode = calculate_xor_value(currentNode, nextNode -> xor_value);
                nextNode -> xor_value = calculate_xor_value(newNode, nextNextNode);
            }

            return true;
        }

        previousNode = currentNode;
        currentNode = nextNode;
    }

    return false;
}

// If possible removes the string at the beginning of the XOR Linked list and returns its value in result. If successful return true, otherwise returns false
int remove_string(Node** head, char* result) {
    if (*head == NULL) {
        return false;
    }

    strcpy(result, (*head) -> name);
    
    Node *newHeadNode = calculate_xor_value(NULL, (*head) -> xor_value);
    Node *nextNode = calculate_xor_value(*head, newHeadNode -> xor_value);
    
    newHeadNode -> xor_value = calculate_xor_value(NULL, nextNode);
    *head = newHeadNode;

    return true;
}

// If possible, removes the string after the string "after" and fills in the result buffer with its value. If successful return true, otherwise returns false
int remove_after(Node** head, const char *after, char *result) {
    Node *previousPreviousNode = NULL;
    Node *previousNode = NULL;
    Node *currentNode = *head;
    Node *nextNode = calculate_xor_value(NULL, currentNode -> xor_value);

    while (currentNode != NULL) {
        nextNode = calculate_xor_value(previousNode, currentNode -> xor_value);
        previousPreviousNode = previousNode;
        previousNode = currentNode;
        currentNode = nextNode;

        if (currentNode == NULL) {
            return false;
        }

        if (previousNode -> name == after) {
            strcpy(result, currentNode -> name);
            nextNode = calculate_xor_value(previousNode, currentNode -> xor_value);
            previousNode -> xor_value = calculate_xor_value(previousPreviousNode, nextNode);
            
            if (nextNode != NULL) {
                Node *nextNextNode = calculate_xor_value(currentNode, nextNode -> xor_value);
                nextNode -> xor_value = calculate_xor_value(previousNode, nextNextNode);
            }

            free(currentNode);
            return true;
        }
    }

    return false;
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
    insert_string(&head, "Charlie");
    insert_string(&head, "Alpha");
    insert_after(&head, "Charlie", "Delta");
    insert_after(&head, "Alpha", "Bravo");
    print_list(head);

    char result[1024];
    remove_string(&head, result);
    remove_after(&head, "Charlie", result);
    insert_before(&head, "Bravo", "Alpha");
    remove_after(&head, "Bravo", result);

    printf("%s\n", result);
    print_list(head);

    return 0;
}