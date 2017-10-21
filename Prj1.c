// Oguzhan Yigit, CSE225
// Compares the given two input text files and produce common words and 2-grams

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct node {
    char st[255];
    struct node *next;
};
typedef struct node NodeText;

FILE *inpf1 = NULL;
FILE *inpf2 = NULL;
char *unwantedChars = ".,;:!?+-*/^%&{[]}\'\"";

void insertWithoutSort(NodeText **header, char st[255])
{
    NodeText *p, *temp;

    // create node to insert and assign values to its fields
    p = (NodeText *) malloc(sizeof(NodeText));
    strcpy(p->st,st);
    p->next=NULL;

    // if LL empty
    if (*header == NULL) {
        *header=p;
    } else {// if LL is not empty
        // assign temp to header to point same node
        temp = *header;
        // iterate it until the last node
        while (temp->next != NULL) {
            temp = temp->next;
        }
        // add new node into last
        temp->next = p;
    }
}

int insertWithSort(NodeText ** header, char st[255], int *count)
{
    NodeText *p,*q,*r;

    // create node to insert and assign values to its fields
    p = (NodeText *) malloc(sizeof(NodeText));
    strcpy(p->st,st);
    p->next=NULL;

    // if LL empty
    if (*header == NULL) {
        *header=p;
    } else {// if LL not empty
        q = *header;

        // sort alphabetically(strcmp)
        while (q != NULL && strcmp(q->st,p->st) < 0) {
            r = q;
            q = q->next;
        }

        // dont add existent node
        if (q != NULL && (!strcmp(q->st,p->st))) {
            return 0;
        }

        // insert node in a sorted fashion
        if (q != NULL)
            p->next = q;

        // if value to insert is the least in LL then have header point to node
        if (q == *header) {
            *header = p;
        } else { // otherwise insert node in correct position
            r->next = p;
        }

    }
    // increate the count here if the code flow reaches here
    (*count)++;
    return 1;
}

int toLowerCase(char *p) {
    int len = strlen(p);
    int i;
    for (i = 0; i < len; i++) {
        // if the char is upper replace it with his lower case clone
        if (isupper(p[i])) {
            p[i] = tolower(p[i]);
        }
    }
}

int isUnwantedChar(char ch){
    // if the given char(ch) is in the unwantedChars array return 1(true) else return false(0)
    int i;
    for (i = 0; i < strlen(unwantedChars); i++) {
        if (unwantedChars[i] == ch)
            return 1;
    }
    return 0;
}

void trimUnwantedChars(char *s) {
    int writer = 0, reader = 0;
    // iterate given string as per reader val
    while (s[reader]) {
        // if the char is not unwantedChars,
        // if it is normal char(a-z,A-Z,1-9), enter the condition
        if (!isUnwantedChar(s[reader])) {
            // assign the normal char into s[writer](s[0] in the first iteration)
            // and increase it by 1 after assigning it (difference between a++ and ++a)
            s[writer++] = s[reader];
        }
        reader++;
    }
    // assign ending char into last char position(s[writer])
    s[writer] = '\0';
}

int main() {
    NodeText *LL1 = NULL, *LL2 = NULL, *resultLL1Gram = NULL, *resultLL2Gram = NULL, *p,
            *p2, *test;
    char string1[255];
    char string2[255];
    int x = 0;
    int y = 0;

    // read files and construct LLs
    inpf1 = fopen("input1.txt","r");
    inpf2 = fopen("input2.txt","r");

    if (inpf1 == NULL || inpf2 == NULL) {
        printf("WARNING: Problem is detected while opening input files\n\n");
        printf("Make sure the input files are in the same directory as with executable\n");
        printf("and filenames are \"input1.txt\" and \"input2.txt\" (case-sensitive)\n");
        return -1;
    }

    printf("First Text Document: \n");

    // fill first LL from input1.txt
    while (fscanf(inpf1, " %255s", string1) == 1) {
        printf("%s ",string1);
        toLowerCase(string1);
        trimUnwantedChars(string1);
        insertWithoutSort(&LL1,string1);
    }

    printf("\n\n\nSecond Text Document: \n");

    // fill second LL from input2.txt
    while (fscanf(inpf2, " %255s", string2) == 1) {
        printf("%s ",string2);
        toLowerCase(string2);
        trimUnwantedChars(string2);
        insertWithoutSort(&LL2,string2);
    }

    fclose(inpf1);
    fclose(inpf2);

    // Assign iterator nodes to header nodes
    p = LL1;
    p2 = LL2;

    char tStr1[255];
    char tStr2[255];

    // start comparing (1-gram)
    while(p != NULL) {
        while(p2 != NULL) {
            if (!strcmp(p->st,p2->st)) { // if strings are same, insert it into result LL
                insertWithSort(&resultLL1Gram,p->st,&x);
            }
            p2=p2->next;
        }
        // set secondary iterator pointer to header pointer after every iteration
        // (because we dont want to stuck on last node)
        p2 = LL2;
        p=p->next;
    }

    printf("\n\n\nRESULT(1-gram): \n\n");

    // Print 1-gram results
    test = resultLL1Gram;
    printf("the number of common words: %d\n\n",x);
    printf("the common words are:\n\n");
    while (test != NULL) {
        printf("%s\n",test->st);
        test = test->next;
    }

    // Make sure the iterator pointers reference header ptrs
    p = LL1;
    p2 = LL2;


    // start comparing (2-gram)
    while(p != NULL) {
        // we are taking words as 2-group so dont take 2-group words at last node
        if (p->next == NULL) {
            break;
        }
        // take first word
        strcpy(tStr1,p->st);
        // append space
        strcat(tStr1," ");
        // append second word (p->next->st)
        // and store it on str1
        char *str1 = strcat(tStr1, p->next->st);
        while(p2 != NULL) {
            // we are taking words as 2-group so dont take 2-group words at last node
            if (p2->next == NULL) {
                break;
            }
            // take first word (from input2.txt)
            strcpy(tStr2,p2->st);
            // append space
            strcat(tStr2," ");
            // append second word (from input2.txt)
            // and store it on str2
            char *str2 = strcat(tStr2, p2->next->st);
            // compare str1 and str2 and insert it into result LL if both str1 and str2 are same
            if (!strcmp(str1,str2)) {
                insertWithSort(&resultLL2Gram,str1,&y);
                // optimization: dont compare same word with rest words(from input2.txt)
                // we already found it
                break;
            }
            // iterate it
            p2=p2->next;
        }
        // set secondary iterator pointer to header pointer after every iteration
        // (because we dont want to stuck on last node)
        p2 = LL2;
        // iterate it
        p=p->next;
    }

    // print 2-gram results
    printf("\n\n\nRESULT(2-gram): \n\n");
    test = resultLL2Gram;
    printf("the number of common 2-grams: %d\n\n",y);
    printf("the common 2-grams are:\n\n");
    while (test != NULL) {
        printf("%s\n",test->st);
        test = test->next;
    }

    printf("\n");
    return 0;

}
