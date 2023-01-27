#include <stdio.h>
#include <stdlib.h>
#include "solve.h"
#include "create.h"
#include "extern.h"
struct dictionary *dict;
ptrwords across, down;
ptrpath path;
int cwsize, dcount, acount;

int main(int argc, char *argv[])
{
    int i, j = 0, pathsize;    
    int *wordlen, maxlen;
    char **crossword, **down_cw, **across_cw,ch;
    FILE *fdictionary;
    /*The first argument should be a crossword file.*/
    if (argv[1] == NULL){
        printf("Please select a crossword!\n");
        return -1;
    }
    crossword = GetCrossword(argv[1]);
    /*If there is the argument "-dict", return the proper dict.
     *Else return the default dictionary (words.txt).*/
    fdictionary = GetDictionary(argv);
    /*Collects useful info to create the proper dictionary and "solving path":
     *Word lengths, num of vertical and laying words..*/
    wordlen = CrosswordInfo(crossword);
    maxlen  = wordlen[0];
    CreateMyDict(wordlen, fdictionary);

    free(wordlen);
    fclose(fdictionary);
    /*Make 2 crosswords with diffrent word path (down - across). 
     *And mark every slot with word's starting position*/
    across_cw = crossword;
    down_cw   = DublicateCrossword(across_cw);
    pathsize  = acount + dcount;
    /*Creates an array of structs . Each struct has the following data stored:
     *Direction of word. Word starting position (down - across);
     *The words that intersecting that pattern*/
    CreatePath(pathsize, across_cw, down_cw);

    for(i = 0; i < cwsize; i++)
        free(down_cw[i]);
    free(down_cw);
    ResetCrossword(crossword);
    /*If there is the argument "-check", no need to solve the crossword*/
    if(!(check(argv, crossword))){
        char input[130];
        for(i = 0; i < cwsize; i++)
            free(crossword[i]);
        free(crossword);
        FreeDict(maxlen);
        FreeWords();
        return 0;
    }
    /*If there is arg "-rand" , gives a random 1st word.*/
    path[0].position->word = RandomWord(argv);
    Solve(crossword, pathsize);
    if(!(draw(argv, crossword))){
        PrintCrossword(crossword);
         for(i = 0; i < cwsize; i++)
            free(crossword[i]);
        free(crossword);
        FreeDict(maxlen);
        FreeWords();
        return 0;
    }
    /*Print laying Words*/
    for(i = 0; i < acount; i++)
        printf("%s\n",dict[across[i].len].words[across[i].word]);
    /*Print vertival words*/
    for(i = 0; i < dcount; i++)
        printf("%s\n",dict[down[i].len].words[down[i].word]);
    for(i = 0; i < cwsize; i++)
        free(crossword[i]);
    free(crossword);
    FreeDict(maxlen);
    FreeWords();
    return 0;
}
