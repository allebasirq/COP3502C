// Name: Isabella Rodrigues Quesada
// From Oct 19th to Oct 27th 2025

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXSIZE 12
#define NUMGAMES 7
#define TOTAL 6
#define BASECASESIZE 30

const char GAMES[NUMGAMES][MAXSIZE+1] = {"Wordle", "Spelling Bee", "Crossword", "Connections", "Strands", "Letter Boxed", "Total"};

typedef struct Player {
    char* name;
    int scores[NUMGAMES];
} Player;

Player** allocateArray(int num);
void getInput(Player** arr, int num);
int compare(Player* ptrP1, Player* ptrP2, int key);
void mergeSort(Player** list, int n, int key);
void merge(Player** list, int low, int high, int key);
void mergeSortRec(Player** list, int low, int high, int key);
void swap(Player** first, Player** second);
void insertionSort(Player** list, int num, int key);
void freeArray(Player** arr, int num);
void print(Player** arr, int n, int query);

int main() {

    int numPlayers, numQuery;
    scanf("%d", &numPlayers);

    Player** myArr = allocateArray(numPlayers);

    getInput(myArr, numPlayers);

    scanf("%d", &numQuery);
    mergeSort(myArr, numPlayers, numQuery);

    print(myArr, numPlayers, numQuery);

    freeArray(myArr, numPlayers);

    return 0;
}

// Funtion to allocate array of Players
Player** allocateArray(int num) {

    Player** outer = malloc(num*sizeof(Player*));

    if(outer == NULL) {
        printf("Failed to allocate outer.\n");
        exit(1);
    }

    for(int i=0; i<num; i++) {
        Player* inner  = malloc(sizeof(Player));

        if(inner == NULL) {
            printf("Failed to allocate inner.\n");
            exit(1);
        }

        inner->name = malloc((MAXSIZE+1)*sizeof(char));                             // Allocates space for each name + 1 for null character

        outer[i] = inner;
    }

    return outer;
}

// Funtion to read each name and scores from each player. It also sets the total points of each players
void getInput(Player** arr, int num) {

    for(int i=0; i<num; i++) {
        int sum=0, curScore=0;

        scanf("%s", arr[i]->name);

        for(int j=0; j<NUMGAMES-1; j++) {                                           // -1 to not get input for the total index
            scanf("%d", &curScore);
            arr[i]->scores[j] = curScore;
            sum = sum + curScore;
        }

        arr[i]->scores[TOTAL] = sum;

    }

}

// Wrapper function for recursive merge function call
void mergeSort(Player** list, int n, int key) {

    mergeSortRec(list, 0, n-1, key);

}

// If it returns -1, ptrP1 name should come before ptrP2 and it is in order.
// It it returns 0, names are the same
// Othewise, they are out of order
int compare(Player* ptrP1, Player* ptrP2, int key) {

    if(strcmp(ptrP1->name, ptrP2->name) < 0) {
        return -1;
    } else if (strcmp(ptrP1->name, ptrP2->name) > 0) {
        return 1;
    } else {
        return 0;
    }

}

// Performs a Merge Sort on list[low..high] according to the game indicated by the integer key.
void mergeSortRec(Player** list, int low, int high, int key) {

    if(high-low < BASECASESIZE) {
        insertionSort(&list[low], high-low + 1, key);                       // high-low + 1 because the last player was not being accounted for
        return;
    }
  
    int mid = (low+high)/2;
    
    mergeSortRec(list, low, mid, key);
    
    mergeSortRec(list, mid+1, high, key);
    
    merge(list, low, high, key);
}

// Merge Sorts the array list of size n according to the game indicated by the integer key.
void merge(Player** list, int low, int high, int key) {

    int length = high-low +1;
    int middle = (high+low)/2;
    int counter = 0;

    Player** temp = malloc(sizeof(Player*)*length);

    if(temp == NULL) {
        printf("Failed to allocate temp!\n");
        exit(1);
    }

    int tempIndex1 = low;
    int tempIndex2 = middle+1;

    // I will sort the list in descending order
    while(tempIndex1 <= middle && tempIndex2 <= high) {

        // Write explanation for this code. Don't forget
        if(list[tempIndex1]->scores[key] > list[tempIndex2]->scores[key]) {
            temp[counter] = list[tempIndex1];
            tempIndex1++;
            counter++; 

        } else if(list[tempIndex1]->scores[key] == list[tempIndex2]->scores[key]) {

            int cmp = compare(list[tempIndex1], list[tempIndex2], key);

            if(cmp < 0) {
                temp[counter] = list[tempIndex1];
                counter++;
                tempIndex1++;
            } else {
                temp[counter] = list[tempIndex2];
                counter++;
                tempIndex2++;
            }

        }else {
            temp[counter] = list[tempIndex2];
            tempIndex2++;
            counter++;
        }
    }

     while (tempIndex1 <= middle) {
        temp[counter] = list[tempIndex1];
        counter++;
        tempIndex1++;
    }

    // Copy remaining elements from right half
    while (tempIndex2 <= high) {
        temp[counter] = list[tempIndex2];
        counter++;
        tempIndex2++;
    }

    // Copy back from temp to our list
    for(int i=low; i<=high; i++) {
        list[i] = temp[i-low];
    }

    // Bye temp!
    free(temp);

}

// O(n^2) funtion to sort Player** list when players are less than base case
void insertionSort(Player** list, int num, int key) {

    for(int i=1; i<num; i++) {          // Index 0 is already sorted
        for(int j=i; j>0; j--) {

            if(list[j]->scores[key] > list[j-1]->scores[key]) {
                swap(&list[j], &list[j-1]);

            } else if(list[j]->scores[key] < list[j-1]->scores[key]) {
                break;

            } else {

                int cmp = compare(list[j], list[j-1], key);

                if (cmp<0) {
                    swap(&list[j], &list[j-1]);
                }
            }

        }

    }

}

// Swap function for insertion sort function
void swap(Player** first, Player** second) {

    Player* temp = *first;
    *first = *second;
    *second = temp;

}

// Function to free Player** array
void freeArray(Player** arr, int num) {

    for(int i=0; i<num; i++) {
        free(arr[i]->name);
        free(arr[i]);
    }

    free(arr);

}

// Print sorted array
void print(Player** arr, int n, int query) {

    printf("%s Ranklist\n", GAMES[query]);

    for(int i=0; i<n; i++) {
        printf("%d. %-15s %d\n", i+1, arr[i]->name, arr[i]->scores[query]);
    }

}
