#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _word_node
{
    char *word;
    unsigned count;
    struct _word_node *next;
} WordNode;

WordNode *create_word_node( void );
void destroy_wordlist( WordNode *word_list );
void add_word( WordNode **word_list, char *new_word );
WordNode *find_word( WordNode *word_list, char const *const word );
void get_words_from_file( WordNode **word_list, char const *const filename );
char *next_word( FILE *file );

int main( void )
{
    WordNode *word_list = NULL;

    get_words_from_file( &word_list, "sonets-shakespeare.txt" );

    WordNode *reader;
    for( reader = word_list; reader != NULL; reader = reader->next )
    {
        if( reader->count > 2 )
        {
            printf( "%15s %4d\n", reader->word, reader->count );
        }
    }

    destroy_wordlist( word_list );

    return 0;
}

WordNode *create_word_node( void )
{
    return calloc( 1, sizeof( WordNode ) );
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

void add_word( WordNode **word_list, char *new_word )
{
    WordNode *found_word = find_word( *word_list, new_word );

    if( found_word )
    {
        ++found_word->count;
        free( new_word );

        return;
    }

    WordNode *new_word_node = create_word_node();

    if( !new_word_node )
    {
        free( new_word );
        return;
    }

    new_word_node->word = new_word;
    new_word_node->count = 1;

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

void get_words_from_file( WordNode **word_list, char const *const filename )
{
    FILE *file = NULL;

    if( !filename )
    {
        return;
    }

    file = fopen( filename, "r" );

    if( !file )
    {
        return;
    }

    char *new_word;

    while( !feof( file ) )
    {
        new_word = next_word( file );

        if( new_word )
        {
            add_word( word_list, new_word );
        }
    }
}

char *next_word( FILE *file )
{
    int character;
    char buffer[256];
    char *word = NULL;
    unsigned length = 0;

    while( ( character = fgetc( file ) ) != EOF )
    {
        if( !isalnum( character ) )
        {
            if( length == 0 )
            {
                continue;
            }

            break;
        }

        buffer[length] = character;
        ++length;
    }

    if( length != 0 )
    {
        buffer[length] = '\0';

        word = malloc( sizeof( char ) * length );
        strcpy( word, buffer );
    }

    return word;
}
