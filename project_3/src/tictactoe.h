#ifndef PROJECT_3_TICTACTOE_H
#define PROJECT_3_TICTACTOE_H

typedef struct _tic_tac_toe_possibility
{
    char board[3][3];
    struct _tic_tac_toe_possibility *next_possibility;
    struct _tic_tac_toe_possibility *next_play;
} TicTacToePossibility;

typedef enum
{
    HUMAN,
    AI
} PlayerType;

typedef enum
{
    PLAYER_WON,
    AI_WON,
    DRAW
} GameStatus;

typedef enum
{
    TOKEN_HUMAN = 'X',
    TOKEN_AI = 'O',
    TOKEN_EMPTY = ' '
} GameToken;

TicTacToePossibility *create_new_possibility( void );
void destroy_possibilities_tree( TicTacToePossibility *possibilities_tree );
void generate_possibilities_tree( PlayerType current_player,
                                  TicTacToePossibility **possibilities_tree );

GameToken game_ended( TicTacToePossibility *current_play );

#endif
