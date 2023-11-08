#ifndef INCLUDE_TTT_H
#define INCLUDE_TTT_H
#include <vector>

class TTT
{
    private:
        // the game board
        std::vector<std::vector<char>> board;

        // book keeping variables
        int nRows; 
        int nCols;
        int size;
        int validMovesLeft;
        int playerTurn;
        bool isGameOver;

    public:
        // Init everything
        TTT();

        // Destructor if needed (might hold game on stack)
        ~TTT();

        // returns the game board
        std::vector<std::vector<char>> getBoard();

        // get the value at a particular square
        char getValueAt(int row, int col);

        // check to see if a move is legal
        bool isValidMove(int row, int col);
        
        // check to see if the game is over (tie or draw)
        bool isGameOver();

        // get the index of the winning player
        int getWinner();

        // change the player who's turn it is
        bool changePlayerTurn();

        // update the state of the game, given any events that have happened
        void update();
};
#endif