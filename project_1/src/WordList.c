#include "WordList.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

WordNode **get_list_as_array( WordNode *word_list, unsigned *list_size );

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

void sort_wordlist( WordNode **word_list )
{
    if( !( *word_list ) )
    {
        return;
    }

    unsigned list_size;
    WordNode **nodes = get_list_as_array( *word_list, &list_size );

    bool changed = true;
    unsigned last = list_size;

    while( changed )
    {
        changed = false;

        for( unsigned i = 0; i < last - 1; ++i )
        {
            if( strcmp( nodes[i]->word, nodes[i + 1]->word ) > 0 )
            {
                WordNode *tmp = nodes[i];
                nodes[i] = nodes[i + 1];
                nodes[i + 1] = tmp;

                changed = true;
            }
        }
    }

    for( unsigned i = 0; i < list_size - 1; ++i )
    {
        nodes[i]->next = nodes[i + 1];
    }
    nodes[list_size - 1]->next = NULL;

    *word_list = nodes[0];

    free( nodes );
}

WordNode **get_list_as_array( WordNode *word_list, unsigned *list_size )
{
    WordNode **nodes;

    *list_size = get_list_size( word_list );

    nodes = malloc( sizeof( WordNode * ) * ( *list_size ) );

    for( unsigned i = 0; i < *list_size; ++i )
    {
        nodes[i] = word_list;
        word_list = word_list->next;
    }

    return nodes;
}

unsigned get_list_size( WordNode *word_list )
{
    unsigned size = 0;

    for( ; word_list != NULL; word_list = word_list->next )
    {
        ++size;
    }

    return size;
}
