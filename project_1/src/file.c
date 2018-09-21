#include "file.h"

#include <ctype.h>
#include <stdio.h>

char *const next_word( FILE *file );

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

        if( new_word[0] != '\0' )
        {
            add_word( word_list, new_word );
        }
    }
}

char *const next_word( FILE *file )
{
    int character;
    static char buffer[256];
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

    buffer[length] = '\0';

    return buffer;
}
