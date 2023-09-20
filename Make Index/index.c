#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE* f1 = fopen("words.txt","r");
    int wordCount = 1;
    char ch;
    while(!feof(f1)) {
        ch = fgetc(f1);
        if(ch == '\n') wordCount++;
    }

    FILE* f2 = fopen("index.idx","wb");
    fwrite(&wordCount,sizeof(int),1,f2);

    int* positions = (int*) malloc((wordCount+1)*sizeof(int));

    rewind(f1);

    wordCount = 0;
    positions[wordCount] = (int) ftell(f1);
    while(!feof(f1)) {
        ch = fgetc(f1);
        if(ch == '\n') {
            wordCount++;
            positions[wordCount] = (int) ftell(f1);
        }
    }
    wordCount++;
    positions[wordCount] = (int) ftell(f1) + 1;
    fclose(f1);

    fwrite(positions,sizeof(int),wordCount+1,f2);
    fclose(f2);
    return 0;
}
