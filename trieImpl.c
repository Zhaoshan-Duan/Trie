/* 
Implementation of Trie Data Structure 
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define SIZE 26 // only supports lowercase a-z
#define MAX_WORD_LENGTH 100

/* Trie Node data structure */
struct TrieNode {
    struct TrieNode* chars[SIZE]; // each node has 26 children
    int isEnd; // 1 == leaf node
    char data; // for printing purpose
};

/* Function to initiate a trie.
Basically same as the create trie node function but it does not take input. */
struct TrieNode* initTrie() {
    // Allocate memoery for a node
    struct TrieNode* node = (struct TrieNode*) malloc(sizeof(struct TrieNode));
    node->isEnd = 0;
    node->data = ' '; // root does not store data
    // Initialize values of the node to null
    for (int i = 0; i < SIZE; i++) {
        node->chars[i] = NULL;
    }
    return node;
}

/* Function to create an empty Trie node */
struct TrieNode* createTrieNode(char c) {
    // Allocate memoery for a node
    struct TrieNode* node = (struct TrieNode*) malloc(sizeof(struct TrieNode));
    node->isEnd = 0;
    node->data = c;
    // Initialize values of the node to null
    for (int i = 0; i < SIZE; i++) {
        node->chars[i] = NULL;
    }
    return node;
}

/* Function to insert a string to a trie.
If some of the characters already exist, 
it will only insert the ones that are unmatched yet. */
void insertWord(struct TrieNode* head, char* word) {
    struct TrieNode* curr = head; // iterator for trie
    for (int i = 0; word[i] != '\0'; i++) { // iterate through the word char by char
        int index = word[i] - 'a'; // index of the character in the child array
        if (curr->chars[index] == NULL) { // if the char does not exist in current child nodes
            curr->chars[index] = createTrieNode(word[i]); // create a new node
        }        
        curr = curr->chars[index]; // move to the next char node
    }
    curr->isEnd = 1; // mark the current node as end of the word
}

/* Search for a string in a Trie */
int searchWord(struct TrieNode* head, char* word) {
    // Simplest case: trie is empty
    if (head == NULL) {
        printf("Trie is empty!");
        return 0;
    }
    struct TrieNode* curr = head; // iterator for trie
    for (int i = 0; word[i] != '\0'; i++) { // iterate over the word char by char
        int index = word[i] - 'a';
        curr = curr->chars[index]; // root is empty so move to next node
        if (curr == NULL) { // empty node: the char does not exist
            return 0;
        }
    }
    return curr != NULL && curr->isEnd == 1; // false if it's not the end of the word
}

/* Search for a prefix in a Trie */
int searchPrefix(struct TrieNode* head, char* prefix) {
    // Simplest case: trie is empty
    if (head == NULL) {
        printf("Trie is empty!");
        return 0;
    }
    struct TrieNode* curr = head; // iterator for trie
    for (int i = 0; prefix[i] != '\0'; i++) { // iterate over the prefix char by char
        int index = prefix[i] - 'a';
        curr = curr->chars[index]; // root is empty so move to next node
        if (curr == NULL) { // empty node: the char does not exist
            return 0;
        }
    }
    return 1; // no need to check whether it's the end
}

// Function to perform an autocomplete search based on a given prefix
void autocomplete(struct TrieNode* root, const char *prefix) {
    struct TrieNode *node = root; // Start from the root node
    for (int i = 0; i < strlen(prefix); i++) { // For each character in the prefix
        int index = prefix[i] - 'a'; // Convert the character to an index (0-25)
        if (!node->chars[index]) // If the child node for this character doesn't exist
            return; // The prefix doesn't exist in the Trie, so return
        node = node->chars[index]; // Move to the child node
    }
    if (node->isEnd) // If the current node marks the end of a word
        printf("%s\n", prefix); // Print the prefix because it's a word in the Trie
    for (int i = 0; i < SIZE; i++) { // For each child node
        if (node->chars[i]) { // If the child node exists
            char nextPrefix[MAX_WORD_LENGTH]; // Create a new string to hold the next prefix
            strcpy(nextPrefix, prefix); // Copy the current prefix to the next prefix
            int len = strlen(nextPrefix); // Get the length of the next prefix
            nextPrefix[len] = i + 'a'; // Add the current character to the next prefix
            nextPrefix[len + 1] = '\0'; // Null-terminate the next prefix
            autocomplete(root, nextPrefix); // Recursively call the function with the next prefix
        }
    }
}
// Function to check if a word exists in the Trie and print a message accordingly
void spellCheck(struct TrieNode *root, const char *word) {
    struct TrieNode *node = root; // Start from the root node
    for (int i = 0; i < strlen(word); i++) { // For each character in the word
        int index = word[i] - 'a'; // Convert the character to an index (0-25)
        if (!node->chars[index]) { // If the child node for this character doesn't exist
            printf("'%s' is not a valid word.\n", word); // The word doesn't exist in the Trie, so print a message
            return; // Return because the rest of the word won't exist in the Trie
        }
        node = node->chars[index]; // Move to the child node
    }
    if (node->isEnd) // If the current node marks the end of a word
        printf("'%s' is a valid word.\n", word); // The word exists in the Trie, so print a message
    else // If the current node doesn't mark the end of a word
        printf("'%s' is not a valid word.\n", word); // The word doesn't exist in the Trie, so print a message
}
// Function to insert a word into the Trie
void insert(struct TrieNode *root, const char *word) {
    struct TrieNode *node = root;
    for (int i = 0; i < strlen(word); i++) {
        int index = word[i] - 'a';
        if (!node->chars[index])
            node->chars[index] = initTrie();
        node = node->chars[index];
    }
    node->isEnd = 1;
}
/* Print Trie */
void printTrie(struct TrieNode* head) {
    if (head == NULL) {
        return;
    }
    struct TrieNode* curr = head;
    if (curr->data != ' ') { // don't print the root
        printf(" -> %c", curr->data);
    }
    for (int i = 0; i < SIZE; i++) {
        printTrie(curr->chars[i]);
    }
}

/* Helper function to print search result */
// function pointer type for the function that will be passed as an argument
typedef int (*SearchFunction)(struct TrieNode* node, char* string); 
void printSearch(SearchFunction func, struct TrieNode* node, char* string) {
    int result = func(node, string);
    char* typeOfSearch = func == searchWord ? "word" : "prefix";
    if (result == 1) {
        printf("The %s \"%s\" exists in the trie.", typeOfSearch, string);
    } else {
        printf("The %s \"%s\" does not exist in the trie.", typeOfSearch, string);
    }
    printTrie(node);
    printf("\n");
}

/* Free memory allocated for Trie nodes */
void freeTrieNode(struct TrieNode* head) {
    for (int i = 0; i < SIZE; i++) {
        if(head->chars[i] != NULL) {
            freeTrieNode(head->chars[i]);
        }
    }
    free(head);
}

int main() {
    struct TrieNode* head = initTrie();
    insertWord(head, "hello");
    printTrie(head);

    printf("%d\n", searchWord(head, "hello"));
    printf("%d\n", searchPrefix(head, "hel"));
    printf("%d\n", searchPrefix(head, "hellooo"));
    printf("%d\n", searchWord(head, "j"));
    
    printSearch(searchWord, head, "hello");
    printSearch(searchWord, head, "hel");
    printSearch(searchWord, head, "c");
    printSearch(searchPrefix, head, "hello");
    printSearch(searchPrefix, head, "hel");
    printSearch(searchPrefix, head, "a");
    
   struct TrieNode *root = initTrie();

    // Open the file containing the words
    FILE *file = fopen("words.txt", "r");
    if (file == NULL) {
        printf("Could not open file words.txt\n");
        return 1;
    }

    // Insert each word from the file into the Trie
    char word[MAX_WORD_LENGTH];
    while (fscanf(file, "%s", word) != EOF) {
        insert(root, word);
    }
    fclose(file);

    // Perform an autocomplete search based on user input
    printf("Enter a prefix for autocomplete: ");
    scanf("%s", word);
    autocomplete(root, word);

    freeTrieNode(head);
}