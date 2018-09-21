#ifndef PROJECT_1_WORDLIST_H
#define PROJECT_1_WORDLIST_H

typedef struct _word_node
{
    char word[32];
    unsigned count;
    struct _word_node *next;
} WordList;

WordList *create_word_node( char const *const new_word );
void destroy_wordlist( WordList *word_list );

void add_word( WordList **word_list, char const *const new_word );
WordList *find_word( WordList *word_list, char const *const word );

void sort_wordlist( WordList **word_list );
unsigned get_list_size( WordList *word_list );

#endif
