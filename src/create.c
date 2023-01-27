#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "solve.h"
#include "create.h"
#include "extern.h"

char **GetCrossword(char *argv)
{
    int i, j, end = 0;
    char **crossword;
    FILE *fcrossword;
    
    const char empty = '0', black = '#';

    fcrossword = fopen(argv, "r");
    if(fcrossword == NULL){
        printf("Please select a valid crossword!\n");
        exit(-1);
    }
    fscanf(fcrossword, "%d", &cwsize);
    
    if((crossword = malloc(cwsize * sizeof(char*))) == NULL){
        printf("Memory problem, please try again!");
        exit(-1);
    }
    for(i = 0; i < cwsize; i++)
        if((crossword[i] = malloc(cwsize * sizeof(char))) == NULL){
            printf("Memory problem, please try again!");
            exit(-1);
        }    

    for(i = 0; i < cwsize; i++)
        for(j = 0; j < cwsize; j++)  
            crossword[i][j] = empty;

    while((fscanf(fcrossword, "%d", &i)) != EOF){
        fscanf(fcrossword, "%d", &j);
        if(end != EOF)crossword[i-1][j-1] = black;
        //-1 cause 1,1 position in file is 0,0 in our array.
    }
    fclose(fcrossword);
    return crossword;
}



FILE *GetDictionary(char **argv)
{
    int i = 0;
    FILE *fdictionary;

    while(argv[++i] != NULL)
        if(!(strcmp(argv[i],"-dict"))){
            if(argv[i+1] == NULL){ //Expects dictionary file after -dict.
                printf("Please select a dictionary.");
                exit(-1);
            }
            fdictionary = fopen(argv[i+1], "r");
            if(fdictionary ==  NULL){
                printf("Please select a valid dictionary!\n");
                exit(-1);
            }
            return fdictionary;
        }
    fdictionary = fopen("./input/Words.txt", "r");
    if(fdictionary ==  NULL){
                printf("Default dictionary 'Words.txt' is missing!\n");
                exit(-1);
            }
    return fdictionary;
}



int *CrosswordInfo(char **crossword)
{
    int *wordlen, *new_wordlen, len, i, j;
    int **laywords, **vertwords, tempsize; 
    /*Mark the starting positions of laying - vertical words*/
    const char layingword ='>',  verticalword = 'v', bothwords = '2', empty = '0';
    const int maxlen  = 0;     //Keep maxlen in wordlen[0].
                               //for wordlen[1+i] , if len exists value is 1  else -1.
    len = acount = dcount = 0;
    /*Create arrays to keep laying-vertical words start position(x,y) and length*/
    tempsize  = (cwsize * cwsize)/2;
    wordlen   = calloc(cwsize + 1, sizeof(int));
    laywords  = malloc(tempsize * sizeof(int*));
    vertwords = malloc(tempsize * sizeof(int*));
    if(laywords == NULL || vertwords == NULL || wordlen == NULL){
        printf("Memory problem, please try again!");
        exit(-1);
    }
    for(i = 0; i < tempsize; i++){
        laywords [i] = calloc(3, sizeof(int));
        vertwords[i] = calloc(3, sizeof(int));
        if(laywords == NULL || vertwords == NULL){
            printf("Memory problem, please try again!");
            exit(-1);
        }
    }
    /*Find laying words*/
    for (i = 0; i < cwsize; i++){ 
        for(j = 0; j < cwsize; j++){
            if(crossword[i][j] == empty)len++;
            else{
                if(len > 1){
                    laywords [acount][0]   = i;
                    laywords [acount][1]   = j-len;
                    laywords [acount++][2] = len;
                    wordlen  [len]         = 1;
                    crossword[i][j-len]    = layingword;
                }
                len = 0;
            }
        }
        /*row change*/
        if(len > 1){
            laywords [acount][0]   = i;
            laywords [acount][1]   = j-len;
            laywords [acount++][2] = len;
            wordlen  [len]         = 1;
            crossword[i][j-len]    = layingword;
        }
        len = 0;
    }
    /*Find vertical words*/
    for (i = 0; i < cwsize; i++){ 
        for(j = 0; j < cwsize; j++){
            if(crossword[j][i] == empty || crossword[j][i] == layingword)len++;
            else{
                if(len > 1){
                    vertwords[dcount][0]   = i;
                    vertwords[dcount][1]   = j-len;
                    vertwords[dcount++][2] = len;
                    wordlen  [len]         = 1;
                    if(crossword[j-len][i] == layingword)
                        crossword[j-len][i] = bothwords;
                    else 
                        crossword[j-len][i] = verticalword;
                }
                len = 0;
            }
        }
        /*column change*/
        if(len > 1){
            vertwords[dcount][0]   = i;
            vertwords[dcount][1]   = j-len;
            vertwords[dcount++][2] = len;
            wordlen  [len]         = 1;
            if(crossword[j-len][i] == layingword)
                crossword[j-len][i] = bothwords;
            else 
                crossword[j-len][i] = verticalword;
        }
        len = 0;
    }
    i = cwsize;                   
    while(!wordlen[i])i--;
    /*keep largest word length at 0 position.*/
    wordlen[maxlen] = i;
    /*Store data and free unnecessary memory*/
    new_wordlen = malloc((i+1) * sizeof(int));
    if((new_wordlen = malloc((i+1) * sizeof(int)))== NULL){
        printf("Memory problem, please try again!");    
        exit(-1);
    }
    across = malloc(acount * sizeof(struct words));
    down   = malloc(dcount * sizeof(struct words));
    if(across == NULL || down == NULL){
        printf("Memory problem, please try again!"); 
        exit(-1);
    }
    for(i = 0; i < dcount; i++){
        down[i].y     = vertwords[i][0];
        down[i].x     = vertwords[i][1];
        down[i].len   = vertwords[i][2];
        down[i].empty = 0;
    }
    for(i = 0; i < acount; i++){
        across[i].x     = laywords[i][0];
        across[i].y     = laywords[i][1];
        across[i].len   = laywords[i][2];
        across[i].empty = 0;
    }
    
    for(i = 0; i <= wordlen[0]; i++)new_wordlen[i] = wordlen[i];
    
    for(i = 0; i < tempsize; i++){
        free(laywords[i]);
        free(vertwords[i]);
    }
    free(laywords);
    free(vertwords);
    return new_wordlen;
}



int   CreateMyDict(int *wordlen, FILE *fdictionary)
{
    char word[100];
    int len, count, i, j, *size, maxdict, maxlen, pos;
    maxlen = wordlen[0];
    size = calloc(maxlen+1 , sizeof(int));
    /*tranfer data from wordlen to dict*/
    dict = malloc((maxlen+1) * sizeof(struct dictionary));
    if(dict == NULL || size == NULL){
        printf("Memory problem, please try again!");    
        exit(-1);
    }
    for(i = 0; i <= maxlen; i++){
        if(wordlen[i] > 0 || i > 1)dict[i].len = i;
        else  dict[i].len = 0;
    }
    /*Count the dictionary words by length*/
    while((fscanf(fdictionary, "%s", word))!=EOF){ 
        len = strlen(word);
        if(len <= maxlen)
            if(dict[len].len)size[len]++;
    }
    /*And malloc the needed space*/
    for(i = 0; i <= maxlen; i++){
        if(dict[i].len){
            dict[i].words = malloc(size[i] * sizeof(char*));
            if(dict[i].words == NULL){
                printf("Memory problem , please try again!");
                exit(-1);
            }
            dict[i].dictsize = size[i];
            for(j = 0; j < size[i]; j++){
                dict[i].words[j] = malloc((dict[i].len+1) * sizeof(char));
                if(dict[i].words[j] == NULL){
                    printf("Memory problem , please try again!");
                    exit(-1);
                }
            }                
        }
        else dict[i].dictsize = 0;
    }
    /*Read the file again and save the words in dict*/
    rewind(fdictionary);
    while((fscanf(fdictionary, "%s", word))!=EOF){ 
        len = strlen(word);
        if(len <= maxlen)
            if(dict[len].len)
                strcpy(dict[len].words[--size[len]],word);  
    }

    for(i = 2; i <= maxlen; i++)
        if(wordlen[i] >=0)
            qsort(dict[i].words, dict[i].dictsize, sizeof(char*), strcompare);
    free(size);
   
    for(i = 2; i <= maxlen; i++)
        if(wordlen[i] >=0){
            dict[i].letter[0] = 0;
            for(j = 1; j < 26; j++)
                /*-1 value for first letters that doesn't exist in the current dict.
                 *i.e If there is no word starting with z , letter[25] = -1.*/
                dict[i].letter[j] = -1;
            for(j = 1; j < dict[i].dictsize; j++)
                /*When first letter of word changes , save the position.*/
                if(dict[i].words[j][0] != dict[i].words[j-1][0]){
                    pos = dict[i].words[j][0] - 'a';
                    dict[i].letter[pos] = j;
                }        
        }
    return 0;
}



char **DublicateCrossword(char **across_cw)
{
    int i, j, startpos;
    char **down_cw;
    /*Create a 2nd crossword for vertical words*/
    if((down_cw = malloc(cwsize * sizeof(char*))) == NULL){
        printf("Memory problem, please try again!");
        exit(-1);
    }
    for(i = 0; i < cwsize; i++){
        if((down_cw[i] = malloc(cwsize * sizeof(char))) == NULL){
            printf("Memory problem, please try again!");
            exit(-1);
        }
    }
    /*Copy data from crossword to the new crossword  (down_cw)*/
    for(i = 0; i < cwsize; i++)
        for(j = 0; j < cwsize; j++)
            down_cw[i][j] = across_cw[i][j];
    /*Scan horizontal and mark every slot with word's 
     *starting position for faster search*/
    for(i = 0; i < cwsize; i++)
        for(j = 0; j < cwsize; j++){
            if(across_cw[i][j] == '2' || across_cw[i][j] == '>'){
                startpos = j;
                while(j < cwsize){
                    if(across_cw[i][j]!='#')across_cw[i][j++] = startpos;
                    else break;
                }
            }
        }
    /*Do the same  vertical*/
    for(i = 0; i < cwsize; i++)
        for(j = 0; j < cwsize; j++){
            if(down_cw[j][i] == '2' ||down_cw[j][i] == 'v'){
                startpos = j;
                while(j < cwsize){
                    if(down_cw[j][i]!='#')down_cw[j++][i] = startpos;
                    else break;
                }
            }
        }
    return down_cw;
}



int   CreatePath(int pathsize, char **across_cw, char **down_cw)
{
    int i, a, d, j, x, y, len, crossed, *x_crossed, *y_crossed, search,
    step, finalstep, maxacross, maxdown, max, countletters, pos;
    char moveright = 1, direction, testletters[cwsize];
    ptrpath temppath;
    pos = i = a = d = 0;
    temppath = malloc((pathsize) * sizeof(struct path)); 
    /*Create a temp array to store all "crossed" words*/ 
    x_crossed = malloc(cwsize * sizeof(int)); 
    y_crossed = malloc(cwsize * sizeof(int));
    if(x_crossed == NULL || y_crossed == NULL || temppath == NULL){
        printf("Memory problem, please try again!");
        exit (-1);
    }
    /*Save alternately vertical - horizontal word patterns (across-down)
     *and all words beeing crossed to that word, for each "step"*/
    while(i < pathsize){
        if(a < acount && i < pathsize){
            temppath[i].position = &across[a];
            temppath[i].position->word = -1;//Function findword() starts searching from ++word.
            temppath[i].direction = '>';
            temppath[i].crossedsize = 0;
            len = across[a].len;
            x = across[a].x;
            y = across[a].y;
            for(j = 0, crossed = 0; j < len; j++)
                if(down_cw[x][y+j] != '>' && down_cw[x][y+j] !='#' && down_cw[x][y+j] !='0'){
                /*Slot is marked with word's starting position (from func DublicateCrossword)
                 *An integer with vertical words start position (x)*/
                   x_crossed[crossed] = down_cw[x][y+j];
                   y_crossed[crossed++] = y+j;
                }
            temppath[i].crossedsize = crossed;
            temppath[i].crossed = malloc(crossed * sizeof(ptrwords));
            if(temppath[i].crossed == NULL){
                printf("Memory problem, please try again!");
                exit(-1);
            }
            for(j = 0; j < crossed; j++){
                search = 0;
                while((x_crossed[j] != down[search].x) || (y_crossed[j] != down[search].y))search++;
                temppath[i].crossed[j] = &down[search];
            }
            a++;
            i++;
        } 
        if(d < dcount && i < pathsize){
            temppath[i].position = &down[d];
            temppath[i].position->word = -1;
            temppath[i].direction = 'v';
            temppath[i].crossedsize = 0;
            len = down[d].len;
            x  = down[d].x;
            y  = down[d].y;              
            for(j = 0, crossed = 0; j < len; j++)
                if(across_cw[x+j][y] != 'v' && across_cw[x+j][y] !='#' && across_cw[x+j][y] !='0'){
                    x_crossed[crossed] = x+j;
                    y_crossed[crossed++] = across_cw[x+j][y];
                } 
            temppath[i].crossedsize = crossed;
            temppath[i].crossed = malloc(crossed * sizeof(ptrwords));
            if(temppath[i].crossed == NULL){
                printf("Memory problem, please try again!");
                exit(-1);
            }
            for(j = 0; j < crossed; j++){
                search = 0;
                while((x_crossed[j] != across[search].x) || (y_crossed[j] != across[search].y))search++;
                temppath[i].crossed[j] = &across[search];
            }                  
            d++;
            i++;
        }
    }
    /*save all words intersect in path*/
    for(i = 0; i < pathsize; i++){
        temppath[i].position->letters = malloc(temppath[i].position->len * sizeof(char));
        if(temppath[i].position->letters == NULL){
                printf("Memory problem, please try again!");
                exit(-1);
        }
    }
    /*Now create the "final" path. */
    ResetCrossword(across_cw);
    ResetCrossword(down_cw);
    path = malloc((pathsize) * sizeof(struct path));
   
    for(i = 0; i < cwsize; i++)testletters[i] = 'a';
    maxacross = maxdown = finalstep = i = j = d = 0;
    /*First word pattern to be filled, is the 1st horizontal word with max length.*/
    direction = '>';
    for(i = 0, max = 0; i < acount; i++)
        if(across[i].len > max){
            max = across[i].len;
            pos = i;
        }
    /*In every step fill word pattern with letters,
     *to find the most "crossed" pattern for every next step*/
    putword(&across[pos], direction, testletters, across_cw);
    across[pos].empty = 1;
    while(&across[pos] != temppath[d].position)d++;
    maxacross++;
    path[finalstep] = temppath[d];
    finalstep++;

    while(finalstep < pathsize){
        max = 0; 
        if (direction == '>' && maxdown < dcount)direction = 'v';
        else if(direction == 'v' && maxacross < acount) direction = '>';
        if(direction == '>'){
            for(i = 0; i < acount && max <= maxdown; i++){
                if(!across[i].empty){
                    MissingLetters(&across[i], direction, across_cw);
                    d = countletters = 0;
                    for(j = 0; j < across[i].len; j++){
                        if(across[i].letters[j] != '0' && across[i].letters[j] != '1')
                            countletters++;
                    }
                     /*Every "step" (path index), is the most "Crossed" word pattern*/
                    if(countletters > max || maxdown == 0){
                        max = countletters;
                        while(&across[i] != temppath[d].position)d++;
                        path[finalstep] = temppath[d];
                    }
                } 
            }
            if(max){
                putword(path[finalstep].position, direction, testletters, across_cw);
                path[finalstep].position ->empty =1;
                maxacross++;
                finalstep ++;
            }
        }
        else{
            for(i = 0; i < dcount && max <= maxacross; i++){
               if(!down[i].empty){
                    MissingLetters(&down[i], direction, across_cw);
                    d = countletters = 0;
                    for(j = 0; j < down[i].len; j++){
                        if(down[i].letters[j] != '0' && down[i].letters[j] != '1')
                            countletters++;
                    }
                    if(cwsize > 13){
                        if(countletters >= max || maxacross == 0){
                           max = countletters;
                           while(&down[i] != temppath[d].position)d++;            
                           path[finalstep] = temppath[d];
                         }
                    }
                    else{
                        if(countletters > max || maxacross == 0){
                            max = countletters;
                            while(&down[i] != temppath[d].position)d++;
                            path[finalstep] = temppath[d];
                        }
                    }
                }
            }
            if(max){
                putword(path[finalstep].position, direction, testletters, across_cw);
                path[finalstep].position ->empty =1;
                maxdown++;
                finalstep++;
            }
        }
    }
    return 0;
}



void   PrintCrossword(char **crossword)
{
    int i,j;
    
    printf("\n    ");
    for(i = 1; i <= cwsize; i++)printf("%2d ",i);
    printf("\n\n 1: ");
    for(i = 0; i < cwsize; i++){
        for(j = 0; j < cwsize; j++){
            if(crossword[i][j]=='#')printf("@@@");
            else printf(" %c ",crossword[i][j]);
        }
        if(i+2 <= cwsize)printf("\n\n%2d: ",i+2);
    }
    printf("\n");
    return;
}



void   ResetCrossword(char **crossword)
{
    int i,j;
    
    for(i = 0; i < cwsize; i++)
        for(j = 0; j < cwsize; j++)
            if(crossword[i][j] != '#')
                crossword[i][j] = '0';
    return;
}



static int strcompare (const void * a, const void * b)
{   
 
    return strcmp(*(const char **) a, *(const char **) b);
}



void FreeDict(int maxlen)
{
    int i, j;
    for(i = 2; i < maxlen; i++){
        if(dict[i].len){
            for(j = 0; j < dict[i].dictsize; j++)
                free(dict[i].words[j]);
            free(dict[i].words);
        }
    }
    free(dict);
}



void FreeWords(void)
{
    int i, j;
    for(i = 0; i < acount; i++)
        free(across[i].letters);
    free(across);
    for(i = 0; i < dcount; i++)
        free(down[i].letters);
    free(down);
    return;
}