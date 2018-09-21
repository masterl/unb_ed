#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "WordList.h"
#include "file.h"

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
