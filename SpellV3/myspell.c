#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h> //import or system calls function
#include <sys/stat.h> //import to use stat
#include <unistd.h>

typedef struct {
    int N; // Number of words in the the dictionary
    int* index;
    char* words;
} dictionary;

//created to find the size of the text file
long int findSize(const char* file_name)
{
    struct stat st; //declare stat variable

    if (stat(file_name, &st) == 0)
        return (st.st_size);
    else
        return -1;
}

dictionary* loadDictionary(const char* indexName,const char* dictName) {
    dictionary* d = (dictionary*)malloc(sizeof(dictionary)); //allocate memory for dictionary
    
    int index_fd = open(indexName, O_RDONLY);
    if (index_fd == -1){
        printf("error opening index file");
        return NULL;
    }
    read(index_fd, &(d->N), sizeof(int)); //read number of words

    d->index = (int*)malloc((d->N) * sizeof(int)); //allocate memory for index array in dictionary struct
    read(index_fd, d->index, (d->N) * sizeof(int)); //read in the index array
    close(index_fd);


    int words_fd = open(dictName, O_RDONLY);
    if (words_fd == -1){
        printf("error opening words file");
        return NULL;
    }

    //question: I figured out the size in the form of long int.
    //So if I do sizeof(size), now I know the size of "size" in bytes right?
    long int size = findSize(dictName);
    d->words = (char*)malloc(size+1); //allocate memory for words array in dictionary struct
    read(words_fd, d->words, size); //read in the words array
    close(words_fd);

    d->words[size] = '\0';
    for(int i=0;i<size;i++){
        if (d->words[i] == '\n'){
            d->words[i] = '\0';
        }
    }

    //question: Should I set up in case of an error when reading a file with and if statement?
    return d;
}

void freeDictionary(dictionary* d) {
    //question: is this the correct free function?
    free(d->words);
    free(d->index);
    free(d);
}

//search in the words array using index and binary search
int searchDictionary(dictionary* d,const char* word) {
    int start = 0;
    int end = d->N-1;
    char* fileWord;
    while(start <= end) {
        int mid = (start+end)/2;
        fileWord = &(d->words[d->index[mid]]);
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