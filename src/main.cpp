#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <cstdio>
#include <string>
#include <iostream>
#include <queue>

#include "ttt.cpp"


// used to make the graph to search for best move
class TreeNode
{
    private:
        std::vector<TreeNode*> children; // all child nodes of this one
        TTT* gameState; // the state of the game at this node
        int data; // the score of this current node
        int depth; // the depth of this node, useful for printing
    public:
        // init with default values
        TreeNode()
        {
           children = {};
           gameState = NULL;
           data = 0;
           depth = 0;
        }

        // copy constructor
        TreeNode(TreeNode& copyFrom)
        {
            children = copyFrom.children;
            gameState = copyFrom.gameState;
            data = copyFrom.data;
            depth = copyFrom.depth;
        }

        // destructor, very useful for TTT gamestate
        ~TreeNode()
        {
            // free the gamestate
            delete gameState;
        }

        int getData()
        {
            return data;
        }

        int getDepth()
        {
            return depth;
        }

        TTT* getGameState()
        {
            return gameState;
        }

        std::vector<TreeNode*> getChildren()
        {
            return children;
        }

        static TreeNode* createBFSMoveTree(TTT* gameState)
        {
            TreeNode* rootNode = new TreeNode();
            rootNode->gameState = gameState;

            // create a queue of nodes to create a big old tree
            std::queue<TreeNode*> nodeQueue;

            // add it to the queue for processing
            nodeQueue.push(rootNode);

            // how many nodes are we making?
            int nodesCreated = 0;

            // first check to see if there are any valid moves left
            while (nodeQueue.size() > 0)
            {
                TreeNode* currNode = nodeQueue.front(); // get the node at the front of the queue
                nodeQueue.pop(); // remove from queue

                TTT* currentGameState = currNode->gameState;

                // explore all valid moves we can make on the current game board  
                for (int i = 0; i < currentGameState->getBoard().size(); ++i)
                {
                    for (int j = 0; j < currentGameState->getBoard()[0].size(); ++j)
                    {
                        // check to see if this is a valid move
                        if (currentGameState->getValidMovesLeft() > 0 && !currentGameState->getIsGameOver() && currentGameState->isValidMove(i, j))
                        {
                            // create a new gamestate from the current one
                            TTT* newGameState = new TTT(*currentGameState);

                            // get the current depth
                            u_int32_t depth = newGameState->getMaxGameMoves() - newGameState->getValidMovesLeft();

                            // make that move
                            if(!newGameState->doTurn(i, j))
                            {
                                printf("move could not be made!\n");
                                printf("row choice: %d, col choice: %d\n");
                            }

                            // get a new node to add to the children of the current node
                            TreeNode* childNode = new TreeNode();
                            childNode->gameState = newGameState;
                            childNode->data = newGameState->getWinningPlayer();
                            childNode->depth = newGameState->getMaxGameMoves() - newGameState->getValidMovesLeft();
                            ++nodesCreated;
                            
                            // append it to the current node children
                            currNode->children.push_back(childNode);

                            // if the game is over after the last move, then we don't go any further with this board
                            if (!newGameState->getIsGameOver())
                            {
                                // add child node to the queue to be explored
                                nodeQueue.push(childNode);
                            }
                        }
                    }
                }
            }

            // how many nodes did we create?
            printf("generated game tree with %d nodes\n", nodesCreated);

            // once that's all done, return this current node
            return rootNode;
        }

        static void printTreeDFS(TreeNode* root)
        {
            // this is a DFS search
            // if this is the last node in the tree
            if (root->children.size() == 0)
            {
                printf("End node: %d\n", root->data); 
            }
            else
            {
                printf("Parent Node: %d , num children: %ld\n", root->data, root->children.size()); 
                for (TreeNode* child: root->children)
                {
                    printTreeDFS(child);
                }
            }
        }

        // this does the job, but it needs depth information in order to be able to print out lines
        static void printTreeBFS(TreeNode* root)
        {
            // create the queue that we want to use to search all the child nodes, and add root to queue
            std::queue<TreeNode*> queue;
            queue.push(root);

            // iterate through all children and print everything
            int currDepth = 0;
            while (queue.size() > 0)
            {
                // take the next element off the queue 
                TreeNode* curr = queue.front();
                queue.pop();
                
                // add all children to queue
                for (TreeNode* child: curr->children)
                {
                    queue.push(child);
                }

                if (currDepth != curr->depth)
                {
                    printf("\n");
                    currDepth = curr->depth;
                }

                // print the value
                printf("%d ", curr->data);
            }
        }

        static void delTreeBFS(TreeNode* root)
        {
            // create the queue that we want to use to search all the child nodes, and add root to queue
            std::queue<TreeNode*> queue;
            queue.push(root);

            // iterate through all children and print everything
            while (queue.size() > 0)
            {
                // take the next element off the queue 
                TreeNode* curr = queue.front();
                queue.pop();
                
                // add all children to queue
                for (TreeNode* child: curr->children)
                {
                    queue.push(child);
                }

                // delete current node
                delete curr; 
            }
        }
};

int main()
{
    // testing the node stuff
    TTT* gameState = new TTT(3,3);
    TreeNode* gameTree = TreeNode::createBFSMoveTree(gameState);

    TreeNode::printTreeDFS(gameTree);
    TreeNode::delTreeBFS(gameTree);

    // // The game state of TTT
    // TTT gameState(3,3);

    // while(!gameState.getIsGameOver())
    // {

    //     gameState.draw();

    //     // update the board with the choices
    //     int row, col;
    //     do
    //     {
    //         // do we use the classic input method?
    //         std::cout << "Player " << gameState.getPlayer() << ", please enter row choice: \n";
    //         std::cin >> row;

    //         std::cout << "Player " << gameState.getPlayer() << ", please enter col choice: \n";
    //         std::cin >> col;

    //         if (!gameState.isValidMove(row, col))
    //         {
    //             std::cout << "That's not a valid move. Please try again.\n";
    //         }
    //     } while (!gameState.isValidMove(row, col));

    //     // update the game with the move
    //     gameState.doTurn(row, col);

    //     if (gameState.getIsGameOver())
    //     {
    //         gameState.draw();
    //         std::cout << "GAME OVER! Winning player: Player " << gameState.getPlayer() << "\n";
    //         return 0;
    //     }

    // }

    // return 0;
}