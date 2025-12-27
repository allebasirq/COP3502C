// Name: Isabella Rodrigues Quesada
// From Nov 3rd to Nov 15th

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUMGAMES 6
#define MAXSIZE 19
#define MAXNODES 200000

typedef struct NYT_String {
    char* str;
    int allowed[NUMGAMES];
} NYT_String;

typedef struct BST_Node {
    NYT_String* ptr;
    struct BST_Node* left;
    struct BST_Node* right;
} BST_Node;

char** allStringsInGame(BST_Node* root, int gameNo, int* arrSize);
BST_Node* createNode(char word[], int numGame);
BST_Node* getInput(int numOp);
void setPointers(BST_Node* firstNode, BST_Node* secondNode);
int lookForWordRec(BST_Node* root, char word[]);
void insertWord(BST_Node* root, char word[], int numGame);
void deleteGame(BST_Node* root, char word[], int gameNum, int* flag);
BST_Node* getParent(BST_Node* cur, BST_Node* child);
BST_Node* deleteNode(BST_Node* root, char word[]);
int isLeaf(BST_Node* node);
int hasOnlyRightChild(BST_Node* node);
int hasOnlyLeftChild(BST_Node* node);
BST_Node* getMinNodeRight(BST_Node* node);
void checkAvailableGames(BST_Node* root, char word[]);
void printAvailableGames(BST_Node* node);
void getStrings(BST_Node* node, int gameNum, char** arr, int* count);
int particularLenString(BST_Node* root, int gameNum, int length);
void swap(char* low, char* high);
void nextAlphaNode(BST_Node* root, char word[]);
BST_Node* getPtr(BST_Node* myPtr, char word[]);
void freeTree(BST_Node* tree);

int main() {

    int numOp;
    scanf("%d", &numOp);

    BST_Node* myRoot = getInput(numOp);

    freeTree(myRoot);
    
}

// Function to properly allocate space for BST and all other inner data (including NYT_String* and char* str)
BST_Node* createNode(char word[], int numGame) {

    BST_Node* res = malloc(sizeof(BST_Node));

    if(res == NULL) {
        printf("Failed to allocate res!\n");
        exit(1);
    }

    res->ptr = malloc(sizeof(NYT_String));

    res->ptr->str = malloc(sizeof(char)*(MAXSIZE+1));                        // +1 for null character
    strcpy(res->ptr->str, word);                                             // Copy word into the new node

    for(int i=0; i<NUMGAMES; i++) {                                          // Initialize allowed to zeros since we are only creating the node
        res->ptr->allowed[i] = 0;
    }

    // Now set that the word is allowed for the game
    res->ptr->allowed[numGame] = 1;

    res->left = NULL;
    res->right = NULL;

    return res;

}

// Funtion to alphabetically compare the node created accondingly to the words entered by user and reassign next pointers to finally set the tree.
// It will then return the root of the tree.
void setPointers(BST_Node* root, BST_Node* nodeToInsert) {

    int compare = strcmp(nodeToInsert->ptr->str, root->ptr->str);

    if(root == NULL || nodeToInsert == NULL) {
        return;
    }
    
    if(root->left == NULL && (compare < 0)) {
        root->left = nodeToInsert;
    }

    if(root->right == NULL && (compare > 0)) {
        root->right = nodeToInsert;
    }

    if(compare < 0) setPointers(root->left, nodeToInsert);                  // If the word comes before the root word, go to the left
    if(compare > 0) setPointers(root->right, nodeToInsert);                 // If the word comes after the root word, go to the right

    return;
}

// Function to read users input, queries and words. It will finally direct each call accordingly to what was entered.
BST_Node* getInput(int numOp) {

    int query;
    BST_Node* root = NULL;
    char word[MAXSIZE+1];
    int numGame, wordLength;

    for(int i=0; i<numOp; i++) {

        scanf("%d", &query);

        switch(query) {

            // Insert word
            case 1:

                scanf("%d", &numGame);
                scanf("%s", word);

                if(root == NULL) {
                    root = createNode(word, numGame);                        // Creating the root
                    break;
                }
                
                insertWord(root, word, numGame);                            // Since we now have the root, insert for other words
                break;

            // Delete word
            case 2:

                scanf("%d", &numGame);
                scanf("%s", word);

                // Flag is a variable to track if the node must be deleted or not.
                int* flag = malloc(sizeof(int));
                *flag = 0;

                deleteGame(root, word, numGame, flag);

                // If we get flagged, delete the node
                if(*flag == 1) {
                    root = deleteNode(root, word);
                }

                free(flag);

            break;
            
            // Given a string, determine which game(s), if any, it's allowed in.
            case 3:
  
                scanf("%s", word);
                checkAvailableGames(root, word);
                
            break;

            // Return an alphabetically sorted list of all strings allowed for a particular game.
            case 4:

                scanf("%d", &numGame);

                int* idx = malloc(sizeof(int));
                *idx = 0;

                char** res = allStringsInGame(root, numGame, idx);

                for(int i=0; i<(*idx); i++) {
                    printf("%s\n", res[i]);
                }

                free(idx);

            break;

            // Return the number of strings of a particular length allowed in a particular game.
            case 5:

                scanf("%d", &numGame);
                scanf("%d", &wordLength);

                int stringLen = particularLenString(root, numGame, wordLength);
                printf("%d\n", stringLen);

            break;
            
            // Given a string, return the next string alphabetically, that appears in any game.
            case 6:

                scanf("%s", word);
                nextAlphaNode(root, word);
                
            break;

            default:
            printf("Invalid query number\n");
        
        }

    }

    return root;

}

// This function will read each word and game number entered by user. It will then check if the word is already in the tree or not. If not, it will create a new
// node, rearrange the pointers of the tree and return the root. Otherwise, it will only set allowed of the game of that "word" node to 1 and return the root.
void insertWord(BST_Node* root, char word[], int gameNum) {

    int found = lookForWordRec(root, word);

    if(!found) {
        BST_Node* res = createNode(word, gameNum);
        setPointers(root, res);
        return;
    }

    BST_Node* nodePtr = getPtr(root, word);

    nodePtr->ptr->allowed[gameNum] = 1;

}

// Function to delete a word for a specific game. It does NOT free the node, since other games may still have the same word allowed.
void deleteGame(BST_Node* root, char word[], int gameNum, int* flag) {

    if(root == NULL) {
        return;
    }

    int compare = strcmp(word, root->ptr->str);

    if(compare == 0) {
        root->ptr->allowed[gameNum] = 0;                            // Not allowed anymore
        int available = 0;

        // Now let's check if we should delete this node or not
        for(int i=0; i<NUMGAMES; i++) {

            if(root->ptr->allowed[i] == 1) {
                available = 1;

                // Break since I don't need to keep checking
                break;
            }

        }

        // If the node does NOT have any games available, flag for deletion
        if(!available) *flag = 1;

        return;

    } else if(compare < 0) {
        return deleteGame(root->left, word, gameNum, flag);

    } else {
        return deleteGame(root->right, word, gameNum, flag);
    }

}

// Function to delete the node if it doesn't have any games available
BST_Node* deleteNode(BST_Node* root, char word[]) {

    // I don't want to loose the root in case I need it
    BST_Node* curNode = root;
    BST_Node* tmp;
    BST_Node* newDelNode;
    BST_Node* delNode = getPtr(curNode, word);
    BST_Node* parent = getParent(curNode, delNode);

    if(isLeaf(delNode)) {

        // If the root is all we have
        if(parent == NULL) {
            free(delNode);
            return NULL;
        }

        int compare = strcmp(parent->ptr->str, delNode->ptr->str);

        if(compare > 0) {
            free(parent->left);
            parent->left = NULL;
        }

        if(compare < 0) {
            free(parent->right);
            parent->right = NULL;
        }

        return root;

    } else if(hasOnlyLeftChild(delNode)) {

        if(parent == NULL) {
            tmp = delNode->left;
            free(delNode);
            return tmp;
        }

        int compare = strcmp(parent->ptr->str, delNode->ptr->str);

        if(compare < 0) {
            tmp = parent->right;
            parent->right = delNode->left;
            free(tmp);
        }

        if(compare > 0) {
            tmp = parent->left;
            parent->left = delNode->left;
            free(tmp);
        }

        return root;

    } else if(hasOnlyRightChild(delNode)) {

        if(parent == NULL) {
            tmp = delNode->right;
            free(delNode);
            return tmp;
        }

        int compare = strcmp(parent->ptr->str, delNode->ptr->str);

        if(compare < 0) {
            tmp = parent->right;
            parent->right = delNode->right;
            free(tmp);
        }

        if(compare > 0) {
            tmp = parent->left;
            parent->left = delNode->right;
            free(tmp);
        }

        return root;

    } else {                        // If it has two children

    BST_Node* minNode = getMinNodeRight(curNode->right);

    // Initialize temp
    tmp = createNode("initialize", 0);

    // Copy the min node into temp (using it like swap)
    *(tmp->ptr) = *(minNode->ptr);

    // Delete the min node
    deleteNode(root, minNode->ptr->str);

    // Copy back into the node that we were suppose to delete (but we are not because it has two children)
    *(delNode->ptr) = *(tmp->ptr);

    free(tmp);

    return delNode;
        
    }

}

// Function that returns 1 if a node is a leaf and 0 if not
int isLeaf(BST_Node* node) {

    if(node->left == NULL && node->right == NULL) {
        return 1;

    } else {
        return 0;
    }

}

// Function that returns 1 if a node has only right child
int hasOnlyRightChild(BST_Node* node) {

    if(node->left == NULL && node->right != NULL) {
        return 1;
    
    } else {
        return 0;
    }

}

// Function that returns 1 if a node has only left child
int hasOnlyLeftChild(BST_Node* node) {

    if(node->right == NULL && node->left != NULL) {
        return 1;
    
    } else {
        return 0;
    }

}

// Function to return a pointer to the left most node of the right side of the root. This will be the node to replace the root node iff the root is deleted
BST_Node* getMinNodeRight(BST_Node* node) {

    if(node == NULL) return NULL;

    // Now go to the left most node on the right side
    while(node->left != NULL) node = node->left; 

    return node;

}

// Function to return a pointer of the parent of the delNode
BST_Node* getParent(BST_Node* cur, BST_Node* child) {

    if(cur == NULL || child == NULL) return NULL;

    if(cur->left == child || cur->right == child) return cur;           // Found the parent

    int compare = strcmp(child->ptr->str, cur->ptr->str);

    if(compare < 0) {
        return getParent(cur->left, child);

    } else if (compare > 0) {
        return getParent(cur->right, child);

    } else {

        // It is the root
        return NULL;
    }


}

// Function to verify what games have "word" allowed.
void checkAvailableGames(BST_Node* root, char word[]) {

    if(root == NULL) {
        int notFound = -1;
        printf("%d\n", notFound);
        return;
    }

    int compare = strcmp(word, root->ptr->str);

    if(compare == 0) {
        printAvailableGames(root);

    } else if(compare < 0) {
        checkAvailableGames(root->left, word);

    } else {
        checkAvailableGames(root->right, word);
    }

}

// Function to print indexes of available games for specific word. This function will only be called once we found the node with the word. 
void printAvailableGames(BST_Node* node) {

    for(int i=0; i<NUMGAMES; i++) {
        
        if(node->ptr->allowed[i] == 1) {
            printf("%d ", i);
        } 
    }

    printf("\n");
    
}

// Function to return an alphabetically sorted list of all strings allowed for a particular game.
char** allStringsInGame(BST_Node* root, int gameNo, int* arrSize) {

    char** res = malloc(sizeof(char*)*MAXNODES);

    if(res == NULL) {
        printf("Failed to allocate for res!\n");
        exit(1);
    }

    for(int i=0; i<MAXNODES; i++) {
        res[i] = malloc(MAXSIZE+1);
    }

    getStrings(root, gameNo, res, arrSize);

    // I needed to catch this in the case we tried to print words for a game that DIDN'T have any games
    if(*arrSize != 0) {

        char** temp = realloc(res, sizeof(char*)*(*arrSize));

        // Catch it if fails
        if(temp == NULL) {
            printf("Failed realloc!\n");

            for(int i=0; i<MAXNODES; i++) {
                free(res[i]);
            }

            free(res);
            exit(1);
        }

    res = temp;

    }

    // Bubble sort
    for(int i= *arrSize-2; i>=0; i--) {
        for(int j=0; j <= i; j++) {
            if(strcmp(res[j], res[j+1]) > 0) {
                swap(res[j], res[j+1]);
            }
        }
    }

    return res;

    }

// Function to print all strings available for a specific game
void getStrings(BST_Node* node, int gameNum, char** arr, int* index) {

    if(node == NULL) {
        return;
    }

    if(node->ptr->allowed[gameNum] == 1) {
        strcpy(arr[*index], node->ptr->str);

        // Increase index
        *index = (*index) + 1;
    }

    if(node->left != NULL) getStrings(node->left, gameNum, arr, index);
    if(node->right != NULL) getStrings(node->right, gameNum, arr, index);

    return;

}

// Recursive function to search words with specific length that is available for a specific game. It will return the total amount of words that fit in that requirement
int particularLenString(BST_Node* root, int gameNum, int length) {

    if(root == NULL) {
        return 0;
    }

    if(root->ptr->allowed[gameNum] == 1) {

        if(strlen(root->ptr->str) == length) {
            
            // Found one and keep looking
            return 1 + particularLenString(root->left, gameNum, length) + particularLenString(root->right, gameNum, length);;
        }

    }
    
    return particularLenString(root->left, gameNum, length) + particularLenString(root->right, gameNum, length);

}

// Iteratively function for query 6. When given a string, it will return the next string alphabetically, that appears in any game.
void nextAlphaNode(BST_Node* root, char word[]) {

    BST_Node* walker = root;
    BST_Node* result = NULL;

    while (walker != NULL) {
        int cmp = strcmp(word, walker->ptr->str);

        if (cmp < 0) {
            result = walker;
            walker = walker->left;

        }
        else if (cmp > 0) {
            walker = walker->right;

        }
        else {

            if (walker->right != NULL) {

                BST_Node* temp = walker->right;
                while (temp->left != NULL) temp = temp->left;
                printf("%s\n", temp->ptr->str);

                return;
            }

            if (result != NULL) {
                printf("%s\n", result->ptr->str);

            } else {
                printf("NO SUCCESSOR\n");
            }
            return;
        }
    }

    if (result != NULL) {
        printf("%s\n", result->ptr->str);

    } else {
        printf("NO SUCCESSOR\n");
    }
}

// Function to swap the words 
void swap(char* low, char* high) {

    char* tmp = malloc(sizeof(char)*(MAXSIZE+1));
    if(tmp == NULL) {
        printf("Failed to allocate tmp!\n");
        exit(1);
    }

    strcpy(tmp, low);
    strcpy(low, high);
    strcpy(high, tmp);

    // Don't need this anymore!
    free(tmp);

}

// Function to recursively check if the word entered by user is already on the tree or not. If found, return 1. Otherwise, return 0.
int lookForWordRec(BST_Node* root, char word[]) {

    if(root == NULL) {
        return 0;
    }

    int compare = strcmp(word, root->ptr->str);

    // We have it!
    if(compare == 0) {
        return 1;
    }

    // Since it is a BST, move accondingly
    if(compare < 0) return lookForWordRec(root->left, word);
    if(compare > 0) return lookForWordRec(root->right, word);

    // Not found. Just return
    return 0;

}

// Function to recursively get the pointer of a node that was created before. The main purpose of it is to use the pointer to assign the game numbers
BST_Node* getPtr(BST_Node* myPtr, char word[]) {

    if(myPtr == NULL) {          // It will never get here since I will only call it if found the word but I still need a base case :)
        return NULL;
    }

    int compare = strcmp(word, myPtr->ptr->str);

    if(compare == 0) {
        return myPtr;

    } else if(compare < 0) {
        return getPtr(myPtr->left, word);

    } else {
        return getPtr(myPtr->right, word);
    }

}

// Free function for all memory allocation
void freeTree(BST_Node* root) {

    if(root == NULL) {
        return;
    }

    if(root->left != NULL) freeTree(root->left);
    if(root->right != NULL) freeTree(root->right);

    free(root->ptr->str);
    free(root->ptr);
    free(root);
}
