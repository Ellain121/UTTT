#ifndef MCTS_AI_HPP
#define MCTS_AI_HPP

#pragma GCC optimize ("Ofast,inline,omit-frame-pointer")

#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>

#define DEBUG
using namespace std;


struct Move
{
    int row;
    int col;
};

struct Random
{
    uint32_t state;
    Random();

    uint32_t getRandom();

    int getRandom(int bound);
};


struct Timeout
{
    public:
        Timeout();
        Timeout(int ms);

        void setTimeBound(int ms);

        bool isTimeLeft();

    public:
        std::chrono::_V2::system_clock::time_point  startTime;
        int                                         mTimeMS;
};

struct Node;
struct Node
{
    int                     gPos;
    int                     lPos;
    int                     N;
    int                     win;
    Node*                   parent;
    vector<Node*>           children;

    bool                    fullyExpanded = false;
    bool                    untouched = true;
};

class MCTS_AI
{
    public:
        MCTS_AI();

        std::pair<int, int>  think(int opRow, int opCol);

    private:
        void    recurseDelete(Node* node, int maxDepth, int depth = 0);
        float   UCB(Node* node, int totalN);
        Node*   bestUCT(Node* node, int totalN);
        Node*   createChild(Node* parent, int gIndx, int lIndx);
        bool    IsgBoardBusy(int gBoard, int gIndx);
        Node*   pick_unvisited(Node* root, int board[10], int gIndx, bool isMyMove);
        int     evaluate(int b);
        bool    isMovesLeft(int board);
        bool    isBoardTerminal(int board);
        void    updateState(int board[10], Node* node, int& gIndx, bool& isMyMove);
        Node*   traverse(Node* node, int board[10], int totalN, int& gIndx, bool& isMyMove);
        Node*   pickRandomChild(Node* node, int board[10]);
        Node*   rollout_policy(Node* node, int board[10], int& gIndx, bool& isMyMove);
        int     result(int board);
        int     rollout(Node*& node, int board[10], int gIndx, bool isMyMove);
        void    updateStats(Node* node, int res);
        bool    isRoot(Node* node);
        void    backpropogate(Node*& node, int res);
        void    mcts(Node* root, int board[10], int gIndx, bool isMyMove = true, bool gameFirstMove = false);
        Move    convertMove(int gPos, int lPos);
        void    convertFromMove(int& gPos, int& lPos, int row, int col);
        Move    findBestMove(int board[10], Node*& root, int gIndx, bool isMyMove, bool gameFirstMove);

    
    private:
        vector<int>     availLocalMovesStore[512];
        Random          rnd;
        int             nodesCnt = 0;
        int             board[10];
        Node*           root;
        int             gIndx;
        bool            isMyMove;
        bool            gameFirstMove;
};

#endif