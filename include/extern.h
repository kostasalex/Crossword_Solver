struct dictionary
{
    int letter[26];  //"id"Of every diffrent 1st letter.
    int len;         
    int dictsize;
    char **words;    //Sorted word array with current len.
};
typedef struct words
{
    int x;
    int y;
    int len;
    int word;        //Keeps the id of word from dict.
    int empty;       //Has value 1 if word pattern is empty,else 0.
    char *letters;   //Word pattern state ie: "00a00".
}*ptrwords;
extern ptrwords across, down; 
/*Struct path is mostly strategically aranged pointers to struct words.*/
typedef struct path 
{   
    char direction;  
    int crossedsize;
    ptrwords *crossed;
    ptrwords position;
}*ptrpath;
extern struct dictionary *dict;
extern ptrpath path ;
extern int cwsize, dcount, acount;