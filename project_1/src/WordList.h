#ifndef PROJECT_1_WORDLIST_H
#define PROJECT_1_WORDLIST_H

typedef struct _word_node
{
    char word[32];
    unsigned count;
    struct _word_node *next;
} WordNode;

WordNode *create_word_node( char const *const new_word );
void destroy_wordlist( WordNode *word_list );

void add_word( WordNode **word_list, char const *const new_word );
WordNode *find_word( WordNode *word_list, char const *const word );

#endif
