    #include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Trie structure
struct Trie
{	
    struct Trie *children[26];
    int count;
};
struct Trie *createTrie();
// Inserts the word to the trie structure
void insert(struct Trie *pTrie, char *word)
{
     // If the trie doesn't exist, create a new node
    if (pTrie == NULL) {
        printf("Trie is not initialized.\n");
        return;
    }

    struct Trie *node = pTrie;
    // Traverse the trie to the appropriate node for the word
    for (int i = 0; i < strlen(word); i++) {
        int index = word[i] - 'a';
        if (node->children[index] == NULL) {
            node->children[index] = createTrie();
        }
        node = node->children[index];
    }
    // Increment the count for the final node of the word
    node->count++;
}

// computes the number of occurances of the word
int numberOfOccurances(struct Trie *pTrie, char *word)
{
     // Traverse the trie to the appropriate node for the word
    struct Trie *node = pTrie;
    for (int i = 0; i < strlen(word); i++) {
        int index = word[i] - 'a';
        if (node->children[index] == NULL) {
            return 0;
        }
        node = node->children[index];
    }
    // Return the count for the final node of the word
    return node->count;
}

// deallocate the trie structure
struct Trie *deallocateTrie(struct Trie *pTrie)
{
        if (pTrie != NULL) {
        for (int i = 0; i < 26; i++) {
            if (pTrie->children[i] != NULL) {
                deallocateTrie(pTrie->children[i]);
            }
        }
        free(pTrie);
    }
    return NULL;
}

// Initializes a trie structure
struct Trie *createTrie()
{
    // Allocate memory for the Trie structure
    struct Trie *newNode = (struct Trie *)malloc(sizeof(struct Trie));
    if (!newNode) {
        printf("Memory allocation failed for Trie.\n");
        return NULL; // Return NULL to indicate failure
    }

    // Initialize the Trie structure
    for (int i = 0; i < 26; i++) {
        newNode->children[i] = NULL;
    }
    newNode->count = 0;

    return newNode;
}

// this function will return number of words in the dictionary,
// and read all the words in the dictionary to the structure words
int readDictionary(char *filename, char **pInWords)
{
           FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file: %s\n", filename);
        return -1; // Return -1 to indicate failure
    }

    int numWords = 0;
    char word[100];

    // Read words from the file until end of file is reached
    while (fscanf(file, "%s", word) != EOF) {
        // Allocate memory for each word
        pInWords[numWords] = malloc(strlen(word) + 1);
        if (!pInWords[numWords]) {
            printf("Memory allocation failed for word.\n");
            fclose(file);
            // Free previously allocated memory
            for (int i = 0; i < numWords; i++) {
                free(pInWords[i]);
            }
            return -1; // Return -1 to indicate failure
        }
        // Copy the word into allocated memory
        strcpy(pInWords[numWords], word);
        numWords++;
    }

    fclose(file);
    return numWords;
}

int main(void)
{
	char *inWords[256];
	
	//read the number of the words in the dictionary
	int numWords = readDictionary("dictionary.txt", inWords);
	for (int i=0;i<numWords;++i)
	{
		printf("%s\n",inWords[i]);
	}
	
	struct Trie *pTrie = createTrie();
	for (int i=0;i<numWords;i++)
	{
		insert(pTrie, inWords[i]);
	}
	// parse lineby line, and insert each word to the trie data structure
	char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
	for (int i=0;i<5;i++)
	{
		printf("\t%s : %d\n", pWords[i], numberOfOccurances(pTrie, pWords[i]));
	}
	pTrie = deallocateTrie(pTrie);
	if (pTrie != NULL)
		printf("There is an error in this program\n");
	return 0;
}