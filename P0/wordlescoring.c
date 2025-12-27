// Assignment P0
// Author: Isabella Rodrigues Quesada
// Date: 08/21/25

#include <stdio.h>

#define MAXWORDS 1000
#define MAXLEN 5
#define ALPHASIZE 26

int userInput(char words[][MAXLEN+1]);                                               // function prototypes
void findIndex(int n, const char words[][MAXLEN+1], int alphabet[ALPHASIZE]);
void printPoints(int n, const char words[][MAXLEN+1], int alphabet[ALPHASIZE]);

int main() {

    char words[MAXWORDS][MAXLEN+1];             // defining max limits before user's decision. Char ends at /0 null thus add 1
    int alphabet[ALPHASIZE] = {0};              // fill alphabet array with zeros

    int n = userInput(words);
    findIndex(n, words, alphabet);
    printPoints(n, words, alphabet);

    return 0;
}

int userInput(char words[][MAXLEN+1]){
    
    int n;
    scanf("%d", &n);                              // first scan from Eustis

    for(int i=0; i<n; i++) {
        scanf("%s", words[i]);                    // second to n-th scan from Eustis
    }

    return n;
}

void findIndex(int n, const char words[][MAXLEN+1], int alphabet[ALPHASIZE]){

    for(int i=0; i<n; i++){
        for (int j=0; j<MAXLEN; j++){
           alphabet[words[i][j] - 'a']++;      // each index from alphabet array represents a letter
        }                                       // when finding the index, add one to count
    }
}

void printPoints(int n, const char words[][MAXLEN+1], int alphabet[ALPHASIZE]){
     for (int i=0; i<n; i++){
        int points = 0;                         // resets points for new words
        for (int j=0; j<MAXLEN; j++) {  
                points += alphabet[words[i][j] - 'a'];
            }
        printf("%s\t%d\n", words[i], points);
    }

}
