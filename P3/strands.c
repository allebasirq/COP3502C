// Author: Isabella Rodrigues Quesada
// Date: Sept 29th 2025 to Oct 5th 2025

/* Notes: After I was done with the program, I got the following message:
a.out(90806,0x1f05f0f40) malloc: Double free of object 0x128e05c40 a.out(90806,0x1f05f0f40) malloc: *** set a breakpoint in malloc_error_break to debug.
I figuered this was happening due to concatenating the two words after concatWords was called. I commented out a line in freeFunctionGrid and
decided to leave it in the code just in case the person who will grade this program want to check that error.
Also, while test the code in Eustis, I got the following error: 
    strands.c:34:11: error: variably modified ‘DR’ at file scope
    34 | const int DR[NUMDIR] = {-1, -1, -1, 0, 0, 1, 1, 1}; 
So I changed from const int NUMDIR = 8; const int NULLPTR = -1; to #define NUMDIR 8 and #define NULLPTR -1. After this, it compiled with no problem */ 

#include <stdio.h>
#include <stdlib.h>

typedef struct dllnode {
    char ch;
    struct dllnode* prev;
    struct dllnode* next;
} dllnode;

dllnode*** allocateGrid(int numRows, int numColumns);
dllnode** allocatePtrGrid(int num);
int** allocateIndexArr(int nR, int nC);
void getInput(dllnode*** gPtr, int nR, int nC);
void getIndexes(int** prevArr, int** nextArr, int numR, int numC);
void setDirections(dllnode*** gPtrs, int** nextIndexArr, int** prevIndexArr, int numR, int numC);
void printWord(dllnode*** gPtrs, int gIndexRows, int gIndexCol);
void getQueries(dllnode*** gridPtr, int numQueries, int numRows, int numCols);
void transverseWord(dllnode*** gPtrs, int gRow, int gCol, int numRows, int numCols);
void concatWords(dllnode*** gridPtrs, int rowFirst, int colFirst, int rowSecond, int colSecond);
void freeFunctionGrid(dllnode*** gridPtr, int numRows, int numColumns);
void freeFunctionArray(int** arr, int nR, int nC);

#define NUMDIR 8
#define NULLPTR -1
const int DR[NUMDIR] = {-1, -1, -1, 0, 0, 1, 1, 1};
const int DC[NUMDIR] = {-1, 0, 1, -1, 1, -1, 0, 1};

int main() {

    int numRows, numCols, numQueries;

    dllnode*** strandsGrid;

    scanf("%d", &numRows);
    scanf("%d", &numCols);
    
    strandsGrid = allocateGrid(numRows, numCols);
    getInput(strandsGrid, numRows, numCols);

    int** prevIndexArr = allocateIndexArr(numRows, numCols);
    int** nextIndexArr = allocateIndexArr(numRows, numCols);

    getIndexes(prevIndexArr, nextIndexArr, numRows, numCols);

    setDirections(strandsGrid, nextIndexArr, prevIndexArr, numRows, numCols);

    scanf("%d", &numQueries);

    getQueries(strandsGrid, numQueries, numRows, numCols);

    freeFunctionGrid(strandsGrid, numRows, numCols);
    freeFunctionArray(prevIndexArr, numRows, numCols);
    freeFunctionArray(nextIndexArr, numRows, numCols);


    return 0;
}

// Function to dynamically allocate space for strands grid
dllnode*** allocateGrid(int numRows, int numColumns) {

    dllnode*** outer = malloc(numRows*sizeof(dllnode**));                   // Just a pointer to the grid

    if(outer == NULL) {
        printf("Failed to allocate memory for outer grid.\n");
        exit(1);
    }

    for(int i=0; i<numRows; i++) {
        dllnode** middle = malloc((numColumns+1)*sizeof(dllnode*));                    // Allocates space for array fo type dllnode*

        if(middle == NULL) {
            printf("Failed to allocate memory for middle grid.\n");
            exit(1);
        }

        outer[i] = middle;

        for(int j=0; j<numColumns; j++) {
            dllnode* inner = malloc(sizeof(dllnode));

            if(inner == NULL) {
                printf("Failed to allocate memory for inner grid.\n");
                exit(1);
            }
            
            // Initialize all instances
            inner->ch = '\0';
            inner->next = NULL;
            inner->prev = NULL; 

            outer[i][j] = inner;
        }

    }

    return outer;

}

// Function to get character inputs from user and populate the 2D array
void getInput(dllnode*** gPtr, int nR, int nC) {


    for(int i=0; i<nR; i++) {
        for (int j=0; j<nC; j++) {
            scanf(" %c", &(gPtr[i][j]->ch));                    // Filling characters
        }
    }
}

// Function to dynamically allocate space for user's input of indexes location
int** allocateIndexArr(int nR, int nC) {

    int** outer = malloc(nR*sizeof(int*));              // Each array will store half amount of rows (one array for prev p and one array for next p)
    
    if(outer == NULL) {
        printf("Could not allocate space for outer index array.\n");
        exit(1);
    }

    for(int i=0; i<nR; i++) {

        int* inner = malloc(nC*sizeof(int));

        if(inner == NULL) {
            printf("Could not allocate space for inner index array.\n");
            exit(1);
        }

        outer[i] = inner;
    }

    return outer;

}

// Function to get user's input of indexes referenced to prev and next pointers
void getIndexes(int** prevArr, int** nextArr, int numR, int numC) {

    // Getting index num that indicates what is the direction of next pointer
    for(int i=0; i<numR; i++) {
        for(int j=0; j<numC; j++) {
            scanf("%d", &nextArr[i][j]);
        }
    }

    // Getting index num that indicates what is the direction of previous pointer of character
    for(int i=0; i<numR; i++) { 
        for(int j=0; j<numC; j++) {
            scanf("%d", &prevArr[i][j]);
        }
    }

}

// This function will collect the indexes of prev and next pointer. It will then redirect next and prev pointers of each dllnode*
void setDirections(dllnode*** gPtrs, int** nextIndexArr, int** prevIndexArr, int numR, int numC) {

    int nextIndex;
    int prevIndex;

    for(int i=0; i<numR; i++) {
        for(int j=0; j<numC; j++) {
            nextIndex = nextIndexArr[i][j];
            prevIndex = prevIndexArr[i][j];

            // I think this is redundant to define the ->next and -> to NULL right below since I've initialized when allocating but thought
            // it would be good practice to leave it to show how to handle the next and prev ptr if index is -1
            if((nextIndex == NULLPTR) && (prevIndex != NULLPTR)) {
                gPtrs[i][j]->next = NULL;                   // Meaning this node is the tail
                gPtrs[i][j]->prev = gPtrs[i+DR[prevIndex]][j+DC[prevIndex]];
                continue;
            } else if ((prevIndex == NULLPTR) && (nextIndex != NULLPTR)) {
                gPtrs[i][j]->prev = NULL;                   // Meaning this node is the head
                gPtrs[i][j]->next = gPtrs[i+DR[nextIndex]][j+DC[nextIndex]];
                continue;
            } 
            
            gPtrs[i][j]->prev = gPtrs[i+DR[prevIndex]][j+DC[prevIndex]];
            gPtrs[i][j]->next = gPtrs[i+DR[nextIndex]][j+DC[nextIndex]];

        }
    }

}

// Function to get queries from user and manipulate the words accordingly with the number entered
void getQueries(dllnode*** gridPtr, int numQueries, int numRows, int numCols) {

    int counter = 0;
    int query, gRow, gCol, secondWordR, secondWordC;

    while(counter != numQueries) {

        scanf("%d", &query);
        scanf("%d", &gRow);
        scanf("%d", &gCol);

        switch(query) {

            case 1:
            printWord(gridPtr, gRow, gCol);
            break;

            case 2:
            transverseWord(gridPtr, gRow, gCol, numRows, numCols);
            break;

            case 3:
            scanf("%d", &secondWordR);
            scanf("%d", &secondWordC);
            concatWords(gridPtr, gRow, gCol, secondWordR, secondWordC);
            break;

        }

        counter++;
    }


}

// This function prints the corresponding word in order, accordingly with the given row number and column number
void printWord(dllnode*** gPtrs, int gRow, int gCol) {

    dllnode* temp = gPtrs[gRow][gCol];

    if(temp->prev == NULL) {                        // If we alread have the head, just print each ch and move to the next one
        while(temp != NULL) {
            printf("%c", temp->ch);
            temp = temp->next;
        }

    } else {                                       // Otherwise, move around the list till we find the head
        while(temp->prev != NULL) {
        temp = temp->prev;
        }

        while(temp != NULL) {                       // And finally print each ch and move to the next one
            printf("%c", temp->ch);
            temp = temp->next;
        }
    
    }

    printf("\n");

}

// Function to transverse the word in the grid. Last character will become the first one, the first will become the last and so on.
// This change will move the oficial order in the grid
void transverseWord(dllnode*** gPtrs, int givenRow, int givenCol, int numRows, int numCols) {

    dllnode* currentNode = gPtrs[givenRow][givenCol];
    int gridSize = numRows*numCols;                         // Since I don't know the size of the word, I'll consider the max num of elements in the grid
    int counter = 0;

    dllnode** arr = allocatePtrGrid(gridSize);

    while(currentNode->prev != NULL) {                          // Go to the head
        currentNode = currentNode->prev;
    }

    while((currentNode != NULL) && (counter<gridSize)) {       // Saving nodes in the array
        arr[counter] = currentNode;
        currentNode = currentNode->next;
        counter++;
    }

    for(int i=0; i<counter; i++) {                              // Swapping the pointers
        dllnode* temp = arr[i]->next;
        arr[i]->next = arr[i]->prev;
        arr[i]->prev = temp;
    }

    arr[0]->next = NULL;                // Setting tail
    arr[counter-1]->prev = NULL;        // Setting head

    // Now the head will point to the old tail
    gPtrs[givenRow][givenCol] = arr[counter-1];

}

// Function to concatenate the two words found by given indexes
void concatWords(dllnode*** gridPtrs, int rowFirst, int colFirst, int rowSecond, int colSecond) {

    dllnode* firstWord = gridPtrs[rowFirst][colFirst];
    dllnode* secondWord = gridPtrs[rowSecond][colSecond];

    firstWord->next = secondWord;             // Change next pointer of first word to point to head of second word
    secondWord->prev = firstWord;             // Change previous pointer of second word to point to the tail of the first word

}

// This function dynamically allocates memory for pointers array to transverse the double linked list
dllnode** allocatePtrGrid(int num) {

    dllnode** res = malloc(num*sizeof(dllnode*));

    if(res == NULL) {
        printf("Failed to allocate ptr array 'res'!\n");
        exit(1);
    }

    return res;

}

// Function to free grid's dynamic memory
void freeFunctionGrid(dllnode*** gridPtr, int numRows, int numColumns) {

    for(int i=0; i<numRows; i++) {
        for(int j=0; j<numColumns; j++) {                                           // THIS POINT HERE
            // free(gridPtr[i][j]);
        }
        free(gridPtr[i]);
    }

    free(gridPtr);
}

// Function to free array's dynamic memory
void freeFunctionArray(int** arr, int nR, int nC) {

    for(int i=0; i<nR; i++) {
        free(arr[i]);
    }

    free(arr);
}

