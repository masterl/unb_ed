#include "tictactoe.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void copy_board( TicTacToePossibility *dest, TicTacToePossibility const *src );
static void generate_possibilities_for( char const current_player_token,
                                        TicTacToePossibility *possibilities_tree );
static GameToken tree_in_line( TicTacToePossibility *current_play, int line );
static GameToken tree_in_column( TicTacToePossibility *current_play, int column );
static GameToken tree_in_diagonal( TicTacToePossibility *current_play );

TicTacToePossibility *create_new_possibility( void )
{
    TicTacToePossibility *new_possibility = calloc( 1, sizeof( TicTacToePossibility ) );

    if( new_possibility )
    {
        memset( new_possibility->board, TOKEN_EMPTY, sizeof( new_possibility->board ) );
    }

    return new_possibility;
}

void destroy_possibilities_tree( TicTacToePossibility *possibilities_tree )
{
    if( possibilities_tree )
    {
        destroy_possibilities_tree( possibilities_tree->next_possibility );
        destroy_possibilities_tree( possibilities_tree->next_play );

        free( possibilities_tree );
    }
}

void generate_possibilities_tree( PlayerType current_player,
                                  TicTacToePossibility **possibilities_tree )
{
    char const current_player_token = current_player == AI ? TOKEN_AI : TOKEN_HUMAN;

    TicTacToePossibility *current_possibility = *possibilities_tree;

    if( !current_possibility )
    {
        current_possibility = create_new_possibility();

        *possibilities_tree = current_possibility;
    }

    generate_possibilities_for( current_player_token, *possibilities_tree );

    for( TicTacToePossibility *possibility = current_possibility; possibility != NULL;
         possibility = possibility->next_possibility )
    {
        if( game_ended( possibility ) == TOKEN_EMPTY )
        {
            printf( "generating next play" );
            fflush( stdout );
            TicTacToePossibility *next_play = create_new_possibility();

            copy_board( next_play, possibility );

            possibility->next_play = next_play;

            generate_possibilities_tree( current_player == AI ? HUMAN : AI, &next_play );
        }
    }
}

static void generate_possibilities_for( char const current_player_token,
                                        TicTacToePossibility *possibilities_tree )
{
    TicTacToePossibility *previous_board = NULL;
    TicTacToePossibility *current_board = possibilities_tree;

    for( int line = 0; line < 3; ++line )
    {
        for( int column = 0; column < 3; ++column )
        {
            if( current_board->board[line][column] == TOKEN_EMPTY )
            {
                TicTacToePossibility *new_possibility = create_new_possibility();

                copy_board( new_possibility, current_board );

                current_board->board[line][column] = current_player_token;

                previous_board = current_board;
                current_board->next_possibility = new_possibility;

                current_board = new_possibility;
            }
        }
    }

    if( current_board != possibilities_tree )
    {
        free( current_board );
        previous_board->next_possibility = NULL;
    }
}

static void copy_board( TicTacToePossibility *dest, TicTacToePossibility const *src )
{
    for( int line = 0; line < 3; ++line )
    {
        for( int column = 0; column < 3; ++column )
        {
            dest->board[line][column] = src->board[line][column];
        }
    }
}

GameToken game_ended( TicTacToePossibility *current_play )
{
    int const results_size = 7;
    GameToken results[results_size];

    results[0] = tree_in_line( current_play, 0 );
    results[1] = tree_in_line( current_play, 1 );
    results[2] = tree_in_line( current_play, 2 );
    results[3] = tree_in_column( current_play, 0 );
    results[4] = tree_in_column( current_play, 1 );
    results[5] = tree_in_column( current_play, 2 );
    results[7] = tree_in_diagonal( current_play );

    for( int i = 0; i < results_size; ++i )
    {
        if( results[i] != TOKEN_EMPTY )
        {
            return results[i];
        }
    }

    return TOKEN_EMPTY;
}

static GameToken tree_in_line( TicTacToePossibility *current_play, int line )
{
    GameToken const player_token = current_play->board[line][0];

    if( player_token == TOKEN_EMPTY )
    {
        return TOKEN_EMPTY;
    }

    for( int column = 1; column < 3; ++column )
    {
        if( player_token != current_play->board[line][column] )
        {
            return TOKEN_EMPTY;
        }
    }

    return player_token;
}

static GameToken tree_in_column( TicTacToePossibility *current_play, int column )
{
    GameToken const player_token = current_play->board[0][column];

    if( player_token == TOKEN_EMPTY )
    {
        return TOKEN_EMPTY;
    }

    for( int line = 1; line < 3; ++line )
    {
        if( player_token != current_play->board[line][column] )
        {
            return TOKEN_EMPTY;
        }
    }

    return player_token;
}

static GameToken tree_in_diagonal( TicTacToePossibility *current_play )
{
    GameToken player_token = current_play->board[0][0];

    for( int i = 1; i < 3; ++i )
    {
        if( player_token != current_play->board[i][i] || player_token == TOKEN_EMPTY )
        {
            break;
        }
    }

    player_token = current_play->board[2][0];

    for( int i = 1; i < 3; ++i )
    {
        if( player_token != current_play->board[2 - i][i] || player_token == TOKEN_EMPTY )
        {
            return TOKEN_EMPTY;
        }
    }

    return player_token;
}
