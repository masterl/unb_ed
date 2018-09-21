#include "similarity.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

long dot_product( WordNode *first_file_words, WordNode *second_file_words );
double word_list_magnitude( WordNode *word_list );

long dot_product( WordNode *first_file_words, WordNode *second_file_words )
{
    long product = 0;

    while( first_file_words && second_file_words )
    {
        int result = strcmp( first_file_words->word, second_file_words->word );

        if( result < 0 )
        {
            first_file_words = first_file_words->next;
            continue;
        }
        else if( result > 0 )
        {
            second_file_words = second_file_words->next;
            continue;
        }

        product += first_file_words->count * second_file_words->count;

        first_file_words = first_file_words->next;
        second_file_words = second_file_words->next;
    }

    return product;
}

double word_list_magnitude( WordNode *word_list )
{
    long sum = 0;

    for( ; word_list; word_list = word_list->next )
    {
        sum += word_list->count * word_list->count;
    }

    return sqrt( sum );
}

double calculate_similarity( WordNode *first_file_words, WordNode *second_file_words )
{
    long const product = dot_product( first_file_words, second_file_words );
    double const first_magnitude = word_list_magnitude( first_file_words );
    double const second_magnitude = word_list_magnitude( second_file_words );

    double const PI = 3.14159265;
    double const angle = acos( product / ( first_magnitude * second_magnitude ) );

    double const angle_in_degrees = angle * 180 / PI;

    return 100.0 - ( angle_in_degrees * 100 / 90 );
}
