// Author: Isabella Rodrigues Quesada
// Assignment: P2
// From Sept 15th 2025 to Sept 20th 2025
// Notes: Since the professor approved global variables for this assingment, I decided to use a couple counters for a better track. Since one
// of the counters was inside a recursive function, initializing the variable inside de function would affect the original count.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LETTERS 7

void sortLetters(char theLetters[], int length);
char** allocateArray(int userNumWords);
void fillArray(char** thisArray, int n);
int numOfPossiblePerm(int num);
void permFunction(char* alphaWords, char* perms, int used[], int index, int length, char** permArray);
void copyContent(char** originalArr, char** copyArr, int length);
int search(char* arr, int initial, int final, char theChar);
int checkLetters(char* word, char* mainWord);
void confirmPermutations(char** perms, char** validWords, int permArraySize, char** finalArray);
void print(char** arr, int length);
void freeFunction(char** arr, int n);

int counterPerm = 0;                                                                       // Counter for my permutation function
int counterValidWords = 0;                                                                 // Counter to valid words 2D array

int main(void) {

    char beeLetters[MAX_LETTERS+1];
    int numWords;
    int usedLetters[MAX_LETTERS] = {0};                                                 // Used array for permutations
    char thePerm[MAX_LETTERS+1];                                                        // The permutations itself

    scanf("%s", beeLetters);
    scanf("%d", &numWords);

    char mainChar = beeLetters[3];                                                      // Store main char

    sortLetters(beeLetters, strlen(beeLetters));

    char** dictionaryArray = allocateArray(numWords);
    fillArray(dictionaryArray, numWords);

    int permArrayLength = numOfPossiblePerm(strlen(beeLetters));                             // Num of possible permutations
    char** permArray = allocateArray(permArrayLength);

    permFunction(beeLetters, thePerm, usedLetters, 0, strlen(beeLetters), permArray);

    char** sortDictionary = allocateArray(numWords);
    copyContent(dictionaryArray, sortDictionary, numWords);                            // Making a copy of the dictionary so I can sort the letters

    char** validWordsArray = allocateArray(numWords); 

    for(int i=0; i< numWords; i++) {                                                   // Sorting each word lexicographically
        sortLetters(sortDictionary[i], strlen(sortDictionary[i]));  
    }

    for(int i=0; i<numWords; i++) {                                                     // Verifies if char is found and characters are allowed

        int charFound = search(sortDictionary[i], 0, strlen(sortDictionary[i])-1, mainChar);

        if(charFound==1) {
            int valid = checkLetters(sortDictionary[i], beeLetters);
            if(valid) {
                strcpy(validWordsArray[counterValidWords], dictionaryArray[i]);                 // If all true, copy to validWordsArray
                counterValidWords++;
            }
        }
    }

    validWordsArray = realloc(validWordsArray, counterValidWords*sizeof(char*));                      // Realloc the array                                                     
    char** finalWords = allocateArray(counterValidWords);

    confirmPermutations(permArray, validWordsArray, permArrayLength, finalWords);                   // finalWords is now populated

    print(finalWords, counterValidWords);

    freeFunction(dictionaryArray, numWords);
    freeFunction(permArray, permArrayLength);
    freeFunction(sortDictionary, numWords);
    freeFunction(validWordsArray, counterValidWords);
    freeFunction(finalWords, counterValidWords);

    return 0;
}

// This function will sort letters is alphabetical order using bubble sort
void sortLetters(char theLetters[], int length) {

    for(int i=0; i<length-1; i++) {
        for(int j=0; j<length - i - 1; j++) {
            if(theLetters[j] > theLetters[j+1]) {
                char temp;
                temp = theLetters[j];
                theLetters[j] = theLetters[j+1];
                theLetters[j+1] = temp;
                }
            }
        }

}

// This function dynamically allocates space for the 2D alphabet array
char** allocateArray(int userNumWords) {

    char** myArray = malloc(userNumWords*sizeof(char*));
    if(myArray==NULL) {
        exit(1);
    }

    for(int i=0; i<userNumWords; i++) {
        myArray[i] = malloc((MAX_LETTERS+1)*sizeof(char));

        if(myArray[i]==NULL) {
            exit(1);
    }
    }

    return myArray;

}

// This function will populate the dictionary array
void fillArray(char** thisArray, int n) {

    for(int i=0; i<n; i++) {
        scanf("%s", thisArray[i]);
    }

}

// This recursive function will return the amount of possible permutations (or simply n factorial)
int numOfPossiblePerm(int num) {

    // Base case
    if(num<=1) {
        return 1;
    }

    return num * numOfPossiblePerm(num-1);

}

// This recursive function will permutate the spelling bee letters and return a new allocated array populated with all different words.
void permFunction(char* possibleLetters, char* perms, int used[], int index, int length, char** permArray) {

    // Base case
    if(index==length && possibleLetters != NULL) {                                            // Stores all permutations in permArray
        strcpy(permArray[counterPerm], perms);
        counterPerm++;
    }

    for(int i=0; i<length; i++) {
        if(used[i] == 0) {
            perms[index] = possibleLetters[i];
            used[i] = 1;
            permFunction(possibleLetters, perms, used, index+1, length, permArray);                     // Move to next index
            used[i] = 0;
        }
    }

}

// This function will copy contents of an originial array to a copy array
void copyContent(char** originalArr, char** copyArr, int length) {

    for(int i=0; i<length; i++) {
        strcpy(copyArr[i], originalArr[i]);
    }

}

// This recursive function searches if "word" contains "theChar" (using binary search)
int search(char* word, int initial, int final, char theChar) {

    // Base case
    if(initial>final || word == NULL) {
        return -1;          // Invalid
    }

    int mid = (initial+final) / 2;

    if(word[mid] == theChar) {
        return 1;   // Found
    }

    if(word[mid] < theChar) {
        return search(word, mid+1, final, theChar);
    }

    if(word[mid] > theChar) {
        return search(word, initial, mid-1, theChar);
    } 

    return 0;   // Not found
    
}

// This function checks if the letters in the dictionary contains the same letters of the spelling bee word
// It returns 1 if all letters are found and 0 if not found
int checkLetters(char* wordToCheck, char* mainWord) {

    for(int i=0; wordToCheck[i] != '\0'; i++) {
        int found = 0;

        for(int j=0; j<strlen(mainWord); j++) {
            if(wordToCheck[i] == mainWord[j]) {
                found = 1;
                break;
            }
        }
        if(!found) {
            return 0;
        }
    }
    return 1;               // If it reaches here, then we matched all the letters!
}

// This function will double check if dictionary words can be found in any permutation possibility
// If found, it will store out final words to our final array
void confirmPermutations(char** perms, char** validWords, int permArraySize, char** finalArray) {
    
    for(int i=0; i<counterValidWords; i++) {
        for(int k=0; k<permArraySize; k++) {
            if(strncmp(perms[k], validWords[i], strlen(validWords[i])) == 0) {     
                strcpy(finalArray[i], validWords[i]);
                break;
            }
        }
    }

}

// Last and final function to sort ONLY valid words (with all checks!) and print
void print(char** arr, int length) {

    char** sortedFinalArray = allocateArray(length);
    copyContent(arr, sortedFinalArray, length);                                     // Make a copy to not mess up original array

    char* temp = malloc((MAX_LETTERS+1)*sizeof(char));

    for(int i=0; i<length-1; i++) {                                                 // Bubble sort to sort the 2d array
        for(int j=0; j<length-1-i; j++) {
            if(strcmp(sortedFinalArray[j], sortedFinalArray[j+1]) > 0) {
                temp = sortedFinalArray[j];
                sortedFinalArray[j] = sortedFinalArray[j+1];
                sortedFinalArray[j+1] = temp;
            }
        }
    }

    for(int i=0; i<length; i++) {
        printf("%s\n", sortedFinalArray[i]);
    }

    freeFunction(sortedFinalArray, length);

}

// This function will free all memory allocations
void freeFunction(char** arr, int n) {

    if(arr == NULL) {
        return;
    }

    for(int i=0; i<n; i++) {
        free(arr[i]);
    }
    free(arr);
}

