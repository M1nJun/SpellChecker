#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    int N; // Number of words in the the dictionary
    int* index;
    FILE* words;
} dictionary;

dictionary* loadDictionary(const char* indexName,const char* dictName) {
    FILE* f = fopen(indexName,"rb");
    dictionary* d = (dictionary*) malloc(sizeof(dictionary));
    fread(&(d->N),sizeof(int),1,f);
    d->index = (int*) malloc((d->N + 1) * sizeof(int));
    fread(d->index,sizeof(int),d->N+1,f);
    fclose(f);
    d->words = fopen(dictName,"r");
    return d;
}

void freeDictionary(dictionary* d) {
    fclose(d->words);
    free(d->index);
    free(d);
}

int searchDictionary(dictionary* d,const char* word) {
    int start = 0;
    int end = d->N-1;
    char fileWord[64];
    while(start <= end) {
        int mid = (start+end)/2;
        fseek(d->words,d->index[mid],SEEK_SET);
        size_t bytesRead = fread(fileWord,sizeof(char),d->index[mid+1]-d->index[mid]-1,d->words);
        fileWord[bytesRead] = '\0';
        int comp = strcmp(word,fileWord);
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
    dictionary* d = loadDictionary("index.idx","words.txt");
    if(d == NULL) {
        printf("Can not load dictionary.\n");
        return 1;
    }

    checkWords(d,"text.txt");

    freeDictionary(d);
    return 0;
} 