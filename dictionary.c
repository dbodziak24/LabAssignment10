#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 26

// Trie structure
struct TrieNode
{
    struct TrieNode *children[ALPHABET_SIZE];
    int count; // To store the count of occurences
};

struct TrieNode *createNode() 
{
    struct TrieNode *node = (struct TrieNode *)malloc(sizeof(struct TrieNode));
    if (node) {
        node->count = 0;
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            node->children[i] = NULL;
        }
    }
    return node;
}

// Inserts the word to the trie structure
void insert(struct TrieNode *root, char *word)
{
    struct TrieNode *current = root;
    for (int i = 0; word[i] != '\0'; i++) {
        int index = word[i] - 'a';
        if (!current->children[index]) {
            current->children[index] = createNode();
        }
        current = current->children[index];
    }
    current->count++; // Increment occurence count
}

// Computes the number of occurances of the word
int numberOfOccurances(struct TrieNode *root, char *word)
{
    struct TrieNode *current = root;
    for (int i = 0; word[i] != '\0'; i++) {
        int index = word[i] - 'a';
        if (!current->children[index]) {
            return 0; // Word not found
        }
        current = current->children[index];
    }
    return current->count;
}

// Deallocate the trie structure
struct TrieNode *deallocateTrie(struct TrieNode *root)
{
    if (root) {
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            if (root->children[i]) {
                deallocateTrie(root->children[i]);
                root->children[i] = NULL; // Set children to NULL after deallocation
            }
        }
        free(root);
    }
    return NULL; // Return NULL after deallocating memory
}

// Initializes a trie structure
struct TrieNode *createTrie()
{
    return createNode();
}

// This function will return number of words in the dictionary,
// and read all the words in the dictionary to the structure words
int readDictionary(char *filename, char **pInWords)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    int numWords = 0;
    char word[100]; // Assuming max word length is 99 characters

    while (fscanf(file, "%s", word) != EOF) {
        pInWords[numWords] = strdup(word); // Copying word into pInWords
        numWords++;
    }

    fclose(file);
    return numWords;
}

int main(void)
{
    char *inWords[256];

    // Read the number of the words in the dictionary
    int numWords = readDictionary("dictionary.txt", inWords);
    for (int i = 0; i < numWords; ++i)
    {
        printf("%s\n", inWords[i]);
    }

    struct TrieNode *pTrie = createTrie();
    for (int i = 0; i < numWords; i++)
    {
        insert(pTrie, inWords[i]);
    }

    // Parse line by line, and insert each word to the trie data structure
    char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
    for (int i = 0; i < 5; i++)
    {
        printf("\t%s : %d\n", pWords[i], numberOfOccurances(pTrie, pWords[i]));
    }
    pTrie = deallocateTrie(pTrie);
    if (pTrie != NULL)
        printf("There is an error in this program\n");
    return 0;
}