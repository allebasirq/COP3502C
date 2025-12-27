// Name: Isabella Rodrigues Quesada
// From October 27th to 29th 2025

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
void quickSort(Player** list, int n, int key);
void quickSortRec(Player** list, int low, int high, int key);
int partition(Player** list, int low, int high, int key);
void swap(Player** first, Player** second);
void selectionSort(Player** arr, int num, int key);
void freeArray(Player** arr, int num);
void print(Player** arr, int n, int query);

int main() {

    int numPlayers, numQuery;

    scanf("%d", &numPlayers);

    Player** myArr = allocateArray(numPlayers);

    getInput(myArr, numPlayers);
    scanf("%d", &numQuery);

    quickSort(myArr, numPlayers, numQuery);

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

// Quick Sorts the array list of size n according to the game indicated by the integer key.
void quickSort(Player** list, int n, int key){ 

    quickSortRec(list, 0, n, key);

}

// Performs a Quick Sort on list[low..high] according to the game indicated by the integer key.
void quickSortRec(Player** list, int low, int high, int key) {

    if(high-low < BASECASESIZE) {
        selectionSort(&list[low], high-low, key);               // We want to start reading the list at index low
        return;
    }

    int split = partition(list, low, high-1, key);              // high-1 because high is the last index. If we are given the length of the array, the last element is at high-1

    quickSortRec(list, low, split-1, key);
    quickSortRec(list, split+1, high, key);

}

// Partition function for quick sort function
int partition(Player** list, int low, int high, int key) {
    
    int pivot;
    int first = list[low]->scores[key];                     // low
    int mid = list[(low+high)/2]->scores[key];            // middle
    int last = list[high]->scores[key];                    // high

    // Picking median
    if((mid<= first && first<=mid) || (last<= first && first<= mid)) {
        pivot = low;

    } else if ((first<= mid && mid<= last) || (last<= mid && mid <=first)) {
        pivot = (low+high)/2;

    } else {
        pivot = high;
    }

    // Move the partition to the first spot of the array
    swap(&list[low], &list[pivot]);

    // Store the index of the partition element.
	pivot = low;
    int pivotNum = list[pivot]->scores[key];

    // Left side walker(It will look for numbers less than element on pivot index to swap)
    low++;

    // Low is the left walker. High is the right walker
    while(low <= high) {

        // If the number is greater than pivotNum or the number is the same as pivotNum and is already in alpha order, move to the next index
        while((low <= high) && ((list[low]->scores[key] > pivotNum) ||
         ((list[low]->scores[key] == pivotNum) && (compare(list[low], list[pivot], key))<0))) {
            low++;
        }

        // If the number is less than pivotNum or the number is the same as pivotNum and is already in alpha order, move to the next index
        while(high >= low && (list[high]->scores[key] < pivotNum) ||
         ((list[high]->scores[key] == pivotNum) && (compare(list[high], list[pivot], key))>0)) {
            high--;
        }

        if(low < high) {
            swap(&list[low], &list[high]);
        }

    }

    swap(&list[pivot], &list[high]);

    // return the idex of the pivot
    return high;

}

// Swap function
void swap(Player** first, Player** second) {

    Player* temp = *first;
    *first = *second;
    *second = temp;

}

// // O(n^2) funtion to sort Player** list when players are less than base case
void selectionSort(Player** arr, int num, int key) {

    for(int i=num-1; i>0; i--) {
        int min = 0;
        
        for(int j=1; j<=i; j++) {
            if(arr[j]->scores[key] < arr[min]->scores[key]) {
                min = j;
            } 
            
            if (arr[j]->scores[key] == arr[min]->scores[key]) {

                if(compare(arr[j], arr[min], key) >0) {
                    min =j;
                }
            }
        }

        swap(&arr[min], &arr[i]);
    }
}

// Function to free Player array
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
