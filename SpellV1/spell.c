#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    int N; // Number of words in the the dictionary
    char** words;
} dictionary;

dictionary* loadDictionary(const char* fileName) {
    FILE* f = fopen(fileName,"r");
    if(f == NULL)
        return NULL;

    // Count the words in the dictionary
    char ch;
    int lineCount = 0;
    while(!feof(f)) {
        ch = fgetc(f);
        if(ch == '\n') lineCount++;
    }
    lineCount++; // The number of words is one more than the number of line breaks.
    rewind(f);

    dictionary* d = (dictionary*) malloc(sizeof(dictionary));
    d->N = lineCount;
    if(lineCount == 0) {
        d->words = NULL;
    } else {
        d->words = (char**) malloc(lineCount * sizeof(char*));
        char buffer[64];
        int n = 0;
        while(!feof(f)) {
            fscanf(f ,"%s",buffer);
            char* word = (char*) malloc(strlen(buffer)+1);
            strcpy(word,buffer);
            d->words[n] = word;
            n++;
        }
    }
    fclose(f);

    return d;
}

void freeDictionary(dictionary* d) {
    for(int n = 0;n < d->N;n++)
        free(d->words[n]);
    free(d->words);
    free(d);
}

int searchDictionary(dictionary* d,const char* word) {
    int start = 0;
    int end = d->N-1;
    while(start <= end) {
        int mid = (start+end)/2;
        const char* lookup = d->words[mid];
        int comp = strcmp(word,d->words[mid]);
        if(comp == 0) return mid;
        else if(comp < 0) end = mid -1;
        else start = mid + 1;
    }
    return -1;
}

void checkWords(dictionary* d,const char* fileName) {
    FILE* f = fopen(fileName,"r");
    if(f == NULL) {
        printf("Could not open data file.\n");
        return;
    }
    char line[256];
    char* word = NULL;
    const char* delims = " 0123456789!@#$&?%*+-/<>,.;:(){}[]\"\'\n\r\t";
    while(!feof(f)) {
        fgets(line,255,f);
        word = strtok(line,delims);
        while(word != NULL) {
            int n = 0;
            while(word[n] != '\0') {
                word[n] = tolower(word[n]);
                n++;
            }
            int loc = searchDictionary(d,word);
            if(loc == -1) 
                printf("%s\n",word);
            word = strtok(NULL,delims);
        }
    }
    fclose(f);
}

int main() {
    dictionary* d = loadDictionary("words.txt");
    if(d == NULL) {
        printf("Can not load dictionary.\n");
        return 1;
    }

    checkWords(d,"text.txt");

    freeDictionary(d);
    return 0;
} 