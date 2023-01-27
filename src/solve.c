                                                                               
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "solve.h"
#include "create.h"
#include "extern.h"

void   Solve(char **crossword, int pathsize)
{
    int *word, step, count, len;
    char *letters, direction;
    ptrwords wordpath;
    step  = count = 0;
    
    while(step < pathsize){
        //PrintCrossword(crossword);
        wordpath  = path[step].position;
        letters   = path[step].position->letters;
        word      = &path[step].position->word;
        len       = path[step].position->len;
        direction = path[step].direction;
        /*Search for the current wordpattern missing letters*/
        MissingLetters(wordpath, direction, crossword);
        /*Find a word that fits*/
        *word = FindWord(*word, len, letters);
        if(*word >= 0 && *word < dict[len].dictsize){
            putword(wordpath, direction, dict[len].words[*word], crossword);
            path[step].position->empty = 0;
            /*Check if there are words that can intersect that word pattern*/ 
            if(ForwardCheck(crossword,step)){
                putword(wordpath, direction, letters, crossword);
                path[step].position->empty = 1;
                continue;//If not "delete" that word and search another
            }
        }
        else{//If no words remain, go back and restore crossword's previous state
            if((step = GoBack(step, crossword)) < 0){
                StartAgain(crossword, step, pathsize);
                step = 0;
            }
             continue;
        }
        step++;
    }


    return;
    
}



void  MissingLetters(ptrwords path, char direction, char **crossword)
{
    int i, last, len, x, y;
    char *letters;
    last = 0;
    len = path->len;
    x = path->x;
    y = path->y;
    letters = path->letters;
    /*Find if there is any letter in wordpattern and save them*/
    if(direction == '>'){
        for(i = 0; i < len; i++){
            letters[i] = crossword[x][y+i];
            if(letters[i]!= '0')last = i;
        }
    }
    if(direction == 'v'){
        for(i = 0; i < len; i++){ 
            letters[i] = crossword[x+i][y];
            if(letters[i]!= '0')last = i;
        }
    }
    /*"mark" with '1' every empty slot before last letter*/
    for(i = 0; i < last; i++){
        if(letters[i]=='0')letters[i]='1';
    }
    letters[len] = '\0';
    return;
}



int    FindWord(int prevw, int wlen, char *letters)
{   
    int nextw, first, i = 0, j = 0, a_z,a_zword;
    /*Search for next word in dict*/
    if((nextw = ++prevw) >= dict[wlen].dictsize){
    return -1;
    } 
    if(*letters != '0'){
        /*Save position of first needed letter*/
        while(letters[i] == '1')i++;
        first = i;
        if(!nextw){
            if(!first){
                nextw = dict[wlen].letter[letters[0]-'a'];
                if(nextw < 0)//there is no word starting with letter[0];
                    return -1;
            }
        }   
        else{
            if(!first && letters[0] < dict[wlen].words[nextw][0])
                return -1;
            else if(!first && letters[0] > dict[wlen].words[nextw][0]){
                a_z = letters[0] - 'a';
                while(dict[wlen].letter[a_z] < 0 && a_z < 26)a_z++;
                if(a_z == 26)return -1;
                else nextw = dict[wlen].letter[a_z];
            }
            else if(first == 1 && letters[1] < dict[wlen].words[nextw][1]){
                a_z = dict[wlen].words[nextw][0] - 'a' + 1;
                while(a_z < 26){
                    a_zword = dict[wlen].letter[a_z];
                    if(a_zword < 0)a_z++;
                    else if(letters[1] < dict[wlen].words[a_zword][1])a_z++;
                    else break;
                }
                if(a_z == 26) return -1;
                else nextw = dict[wlen].letter[a_z];
            }
        }

        while(nextw < dict[wlen].dictsize){
            /*Find word match with first letter*/
            while(dict[wlen].words[nextw][first] != letters[first]){
                if((!first || first == 1) && letters[first] < dict[wlen].words[nextw][first]){
                    a_z = dict[wlen].words[nextw][0] - 'a' + 1;
                    while(a_z < 26){
                        a_zword = dict[wlen].letter[a_z];
                        if(a_zword < 0)a_z++;
                        else if(letters[first] < dict[wlen].words[a_zword][first])a_z++;
                        else break;
                    }
                    if(a_z == 26) return -1;
                    else nextw = dict[wlen].letter[a_z];
                }
                if(++nextw >= dict[wlen].dictsize)return -1;
            }
            if(dict[wlen].words[nextw][first] == letters[first] && letters[first+1] == '\0')
                return nextw;//word found(after letter[first] word ends)
            /*Word match found, now search for the other letters(if needed)*/
            while(letters[++i] != '0'){
                if(letters[i] == dict[wlen].words[nextw][i] && i == wlen)
                    return nextw;//i == wlen , so no need to search for other letters.
                if(letters[i] != '1' && letters[i] != dict[wlen].words[nextw][i]){
                    i = first;
                    nextw++; //There is no match with next letters. 
                    break;   //Search again for next word.
                }
            }
            if( i != first) return nextw;  //word is found
        }
        return -1; //word not found
    }
    /*If 1st slot == '0' return a word with length needed (wlen)*/
    else return nextw;
}



void   putword(ptrwords path, char direction, char *word, char **crossword)
{
    int i, x, y, len;
    len = path->len;
    x = path->x;
    y = path->y;

    if(direction =='>')
        for(i = 0; i < len; i++){
            if(word[i]=='1'){
                crossword[x][y+i] = '0';
                continue;
            }
            crossword[x][y+i] = word[i];
        }      
    else if(direction == 'v')
        for(i = 0; i < len; i++){
            if(word[i] =='1'){
                crossword[x+i][y] = '0';
                continue;
            }
            crossword[x+i][y] = word[i];
        }
    return;
}



int ForwardCheck(char **crossword, int step )
{
    int i;
    char direction, *letters;
    ptrwords crossed;
    /*Search for the "crossed" words*/
    if(path[step].direction == '>')direction = 'v';
    else direction = '>';
    for(i = 0; i < path[step].crossedsize; i++){
        crossed = path[step].crossed[i];
        if(crossed->empty){
            MissingLetters(crossed, direction, crossword);
            letters = crossed->letters;
            crossed->word = FindWord(crossed->word, crossed->len, letters);
            if(crossed->word < 0){//No word found
                if(i)
                    for(i; i >= 0; i--){
                        crossed = path[step].crossed[i];
                        if(crossed -> empty)crossed->word = -1;
                    }
                return -1;
            }
        /*Func FindWord will start from word ++;*/    
        crossed->word--;
        }   
    }
    return 0;
}




int    GoBack(int step, char  **crossword)
{
    int i , j = 0, *word, dictsize, len;
    const int outofwords = -1;
    
    /*Restore crossword's previous state*/
    path[step].position->word = -1;
    while(--step >= 0){
        putword(path[step].position,path[step].direction, path[step].position->letters, crossword);
        path[step].position->empty = 1;
        word = &path[step].position->word;
        len = path[step].position->len;
        dictsize = dict[len].dictsize;
        if((*word + 1) < dictsize)
            return step;
        else 
            if(!step) return outofwords;
        *word = -1; 
    }
}



int check(char **argv, char ** crossword)
{
    int i = 0, j, len, a = 0, ch;
    char  word[128];
    ptrwords check;
    while(argv[++a] != NULL)
        if(!(strcmp(argv[a],"-check")))
            break;
    if(argv[a] == NULL)return -1;
    for(i = 0; i < acount; i++){
        if(fgets(word, 128, stdin) == NULL){
            printf("\nMore words needed to fill the crossword\n");
            return 0;
        }
        len = strlen(word);
        if(word[len-1] == '\n'){
            --len;
            word[len] = '\0';
        } 
        if(word[len] == '\n') word[len] = '\0';
        if(len != across[i].len){
            printf("\nWord '%s' does not fit at (%d,%d) across \n", word, across[i].x+1, across[i].y+1);
            return 0;
        }
        for(j = 0; j < len; j++) {          
            if (word[j] >= 'A' && word[j] <= 'Z')                 
                word[j] = word[j] + ('a'-'A');                            
        }
        for(j = 0; j < dict[len].dictsize; j++){
            if(!(strcmp(word,dict[len].words[j])))break;
        }
        if(j == dict[len].dictsize){
            printf("\nWord '%s' not in dictionary\n",word);
            return 0;
        }
        for(j = 0; j < len; j++){
            crossword[across[i].x][across[i].y+j] = word[j];
        }
    }
    for(i = 0; i < dcount; i++){
        if(fgets(word, 128, stdin) == NULL){
            printf("\nMore words needed to fill the crossword\n");
            return 0;
        }
        len = strlen(word);
        if(word[len-1] == '\n'){
            --len;
            word[len] = '\0';
        } 
        if(word[len] == '\n') word[len] = '\0';
        if(len != down[i].len){
            printf("len = %d down i len = %d\n",len,down[i].len);
            printf("\nWord '%s' does not fit at (%d,%d) down\n", word, down[i].x+1, down[i].y+1);
            return 0;
        }
        for(j = 0; j < len; j++) {          
            if (word[j] >= 'A' && word[j] <= 'Z')                 
                word[j] = word[j] + ('a'-'A');                            
        }
        for(j = 0; j < dict[len].dictsize; j++){
            if(!(strcmp(word,dict[len].words[j])))break;
        }
        if(j == dict[len].dictsize){
            printf("\nWord '%s' not in dictionary\n",word);
            return 0;
        }
        check = &down[i];
        MissingLetters(check, 'v', crossword);
        for(j = 0; j < len; j++){
            if(down[i].letters[j] != '1' && down[i].letters[j] != '0')
                if(down[i].letters[j] != word[j]){
                    printf("\nWord '%s' does not fit at (%d,%d) down\n", word, down[i].x+1, down[i].y+1);
                    return 0;
                }
            crossword[down[i].x+j][down[i].y] = word[j];
        }
    }
    if((ch = getchar())!= EOF && ch != '\n'){
        printf("\nMore words than needed\n");
        return 0;
    }
    else{
        PrintCrossword(crossword);
        return 0;
    } 
}

int draw(char **argv ,char **crossword)
{
    int i = 0;
    while(argv[++i] != NULL)
        if(!(strcmp(argv[i],"-draw")))return 0;
    return -1;
}


int RandomWord(char **argv)
{
    int i = 0;
    while(argv[++i] != NULL)
        if(!(strcmp(argv[i],"-rand"))){
            srand(time(NULL));
            return rand()%30;
        }
    return -1;
}


int    StartAgain(char ** crossword, int step, int pathsize){
    int i = 0;
    static int nextw = 0;
    nextw++;
    path[0].position->word = nextw;
    for(i = 1; i <= step; i++)path[i].position->word = -1;
    ResetCrossword(crossword);
}

