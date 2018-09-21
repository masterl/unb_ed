#include "WordList.h"

#include <stdlib.h>
#include <string.h>

WordNode *create_word_node( char const *const new_word )
{
    WordNode *new_node = calloc( 1, sizeof( WordNode ) );

    if( !new_node )
    {
        return NULL;
    }

    if( new_word )
    {
        strcpy( new_node->word, new_word );
        new_node->count = 1;
    }

    return new_node;
}

void destroy_wordlist( WordNode *word_list )
{
    WordNode *to_remove;

    while( word_list != NULL )
    {
        to_remove = word_list;
        word_list = word_list->next;

        free( to_remove );
    }
}

void add_word( WordNode **word_list, char const *const new_word )
{
    WordNode *found_word = find_word( *word_list, new_word );

    if( found_word )
    {
        ++found_word->count;

        return;
    }

    WordNode *new_word_node = create_word_node( new_word );

    if( !new_word_node )
    {
        return;
    }

    if( !( *word_list ) )
    {
        *word_list = new_word_node;

        return;
    }

    WordNode *last_node;

    for( last_node = *word_list; last_node->next != NULL; last_node = last_node->next )
        ;

    last_node->next = new_word_node;
}

WordNode *find_word( WordNode *word_list, char const *const word )
{
    for( ; word_list != NULL; word_list = word_list->next )
    {
        if( strcmp( word_list->word, word ) == 0 )
        {
            break;
        }
    }

    return word_list;
}
