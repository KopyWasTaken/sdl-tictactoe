#include <vector>
#include <cstdio>

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
        int playerTurn; // 1 for X, 0 for O
        bool isGameOver;
        int winningPlayer; // 1 for X, 0 for 0, -1 for tie

    public:
        // Init everything
        TTT(int rows, int cols)
        {
            init(rows, cols);
        }

        // copy constructor
        TTT(const TTT& copyFrom)
        {
            board = copyFrom.board;
            nRows = copyFrom.nRows;
            nCols = copyFrom.nCols;
            size = copyFrom.size;
            validMovesLeft = copyFrom.validMovesLeft;
            playerTurn = copyFrom.playerTurn;
            isGameOver = copyFrom.isGameOver;
            winningPlayer = copyFrom.winningPlayer;
        }

        // Destructor if needed (might hold game on stack)
        ~TTT() {}

        // (re)initializes game board
        void init(int rows, int cols)
        {
            // clear the board first 
            board.clear(); 

            // Init the game board with all blanks
            for (auto i = 0; i < rows; ++i)
            {
                board.push_back({});
                for (auto j = 0; j < cols; ++j)
                {
                    board[i].push_back('-');
                }
            }
            nRows = rows;
            nCols = cols;
            size = rows * cols;
            validMovesLeft = size;
            playerTurn = false;
            isGameOver = false;
            winningPlayer = -1; // start with a tie, and only update if there is a winner
        }

        // returns the game board
        std::vector<std::vector<char>> getBoard()
        {
            return board;
        }

        // check to see if a row, col combo is legal
        bool isValidPos(int row, int col)
        {
            if (row >= 0 && row < nRows)
            {
                if (col >= 0 && col < nCols)
                {
                    return true;
                }
            }

            return false;
        }

        // get the value at a particular square
        char getValueAt(int row, int col)
        {
            if (isValidPos(row, col))
            {
                return board.at(row).at(col);
            }
            return 'e';
        }

        // is it valid to make a move at the specified combo
        bool isValidMove(int row, int col)
        {
            if (!isValidPos(row, col))
            {
                return false;
            }
            else 
            {
                if (getValueAt(row, col) == '-')
                {
                    return true;
                }
            }

            return false;
        }
        
        // check to see if the game is over (tie or win)
        bool getIsGameOver()
        {
            if (validMovesLeft == 0)
            {
                isGameOver = true;
            }
            else
            {

                // check all rows
                for (auto i = 0; i < nRows; ++i)
                {
                    if (isRowWinner(i))
                    {
                        isGameOver = true;
                        winningPlayer = playerTurn;
                        break;
                    }
                }

                // check all cols
                for (auto i = 0; i < nCols; ++i)
                {
                    if (isColWinner(i))
                    {
                        isGameOver = true;
                        winningPlayer = playerTurn;
                        break;
                    }
                }

                // check diagonals
                if (isDiagonalLeftWinner() || isDiagonalRightWinner())
                {
                    winningPlayer = playerTurn;
                    isGameOver = true;
                }
            }
            return isGameOver;
        }

        bool isRowWinner(int row)
        {
            if (isValidPos(row, 0))
            {
                // check that row
                char checkSymbol = playerTurn ? 'X' : 'O';
                for (char c: board.at(row))
                {
                    if (c != checkSymbol)
                    {
                        return false;
                    }
                }
                return true;
            }
            return false;
        }

        bool isColWinner(int col)
        {
            if (isValidPos(0, col))
            {
                // check that row
                char checkSymbol = playerTurn ? 'X' : 'O';
                for (std::vector row: board)
                {
                    if (row.at(col) != checkSymbol)
                    {
                        return false;
                    }
                }
                return true;
            }
            return false;
        }

        bool isDiagonalLeftWinner()
        {
            bool isWinner = true;
            char checkSymbol = playerTurn ? 'X' : 'O';
            // check each of the two diagonals to see if a player won
            for (auto i = 0; i < nRows; ++i)
            {
                if (getValueAt(i, i) != checkSymbol)
                {
                    isWinner = false;
                }
            }

            return isWinner;
        }

        bool isDiagonalRightWinner()
        {
            bool isWinner = true;
            char checkSymbol = playerTurn ? 'X' : 'O';
            // check the other diagonal
            for (auto i = nRows - 1; i >= 0; --i)
            {
                if (getValueAt(i, i) != checkSymbol)
                {
                    isWinner = false;
                }
            }

            return isWinner;
        }

        // change the player who's turn it is
        void changePlayerTurn()
        {
            playerTurn = !playerTurn;
        }

        // Makes a move, and switches the player turn
        bool doTurn(int row, int col)
        {
            if (!getIsGameOver() && validMovesLeft > 0)
            {
                char symbol = playerTurn ? 'X' : 'O';
                board.at(row).at(col) = symbol;
                --validMovesLeft;
                if (!getIsGameOver())
                {
                    changePlayerTurn();
                }
                return true;
            }
            return false;
        }

        // gets the index of the player who's turn it currently is
        int getPlayer()
        {
            return playerTurn;
        }

        // draw the board
        void draw()
        {
            for (std::vector row: board)
            {
                printf("\n");
                for (char c: row)
                {
                   printf("%c\t", c);
                }
            }
            printf("\n");
        }

        int getWinningPlayer()
        {
            return winningPlayer;
        }        

        bool isTie()
        {
            // if the game is over, and there are no moves left to make, and there is no winner, then we tie
            if (getIsGameOver() && getWinningPlayer() == -1)
            {
                return true;
            }

            return false;
        }

        // get the number of valid moves left
        int getValidMovesLeft()
        {
            return validMovesLeft;
        }

        // this returns the score of the board
        // 1 for X win, 0 for O win, -1 for tie
        int getGameScore()
        {
            return winningPlayer;
        }

        int getMaxGameMoves()
        {
            // return the total possible number of moves for a board of this size
            return nRows * nCols;
        }
};