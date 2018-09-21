#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "WordList.h"
#include "file.h"
#include "similarity.h"

int main( int argc, char **argv )
{
    if( argc != 3 )
    {
        printf( "ERROR! You should provide two files.\n" );
        printf( "Usage:\n\t%s <file_1> <file_2>\n", argv[0] );

        return EXIT_FAILURE;
    }

    WordList *first_file_words = NULL;
    WordList *second_file_words = NULL;

    printf( "Reading [%s]...\n", argv[1] );
    get_words_from_file( &first_file_words, argv[1] );
    printf( "Reading [%s]...\n", argv[2] );
    get_words_from_file( &second_file_words, argv[2] );

    printf( "Sorting words...\n" );
    sort_wordlist( &first_file_words );
    sort_wordlist( &second_file_words );

    printf( "Calculating similarity...\n" );
    double const similarity = calculate_similarity( first_file_words, second_file_words );

    printf( "\nSimilarity between\n" );
    printf( "   %s\n", argv[1] );
    printf( "   %s\n", argv[2] );
    printf( "---------> %.2lf%%\n", similarity );

    destroy_wordlist( first_file_words );
    destroy_wordlist( second_file_words );

    return 0;
}
