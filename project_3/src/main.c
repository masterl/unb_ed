#include <stdio.h>

#include "tictactoe.h"

void print_all( TicTacToePossibility *possibility );

int main( void )
{
    TicTacToePossibility *possibilities_tree = NULL;

    generate_possibilities_tree( HUMAN, &possibilities_tree );

    printf( "Printing possibilities tree...\n" );
    fflush( stdout );
    print_all( possibilities_tree );

    printf( "Destroying possibilities tree...\n" );
    fflush( stdout );
    destroy_possibilities_tree( possibilities_tree );
    return 0;
}

void print_all( TicTacToePossibility *possibility )
{
    if( !possibility )
    {
        return;
    }

    for( int line = 0; line < 3; ++line )
    {
        printf( " %c \u23a2 %c \u23a2 %c",
                possibility->board[line][0],
                possibility->board[line][1],
                possibility->board[line][2] );

        if( line != 2 )
        {
            printf( "\n\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500" );
        }

        puts( "" );
    }
    puts( "\n" );

    print_all( possibility->next_possibility );
    print_all( possibility->next_play );
}
