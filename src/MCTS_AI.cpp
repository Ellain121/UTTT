#include "MCTS_AI.hpp"

namespace debug
{
    void printBoard(int board[10])
    {
        char boardChar[3][3];
        for (int h = 0; h < 3; ++h)
        for (int w = 0; w < 3; ++w)
            boardChar[h][w] = '-';

        for (int i = 0; i < 9; ++i)
        {
            if ((board[9] & (1 << i)) == (1 << i))
            {
                int tmpI = 8-i;
                boardChar[tmpI/3][tmpI%3] = 'O';
            }
        }

        for (int i = 9; i < 18; ++i)
        {
            if ((board[9] & (1 << i)) == (1 << i))
            {
                int tmpI = 17-i;
                boardChar[tmpI/3][tmpI%3] = 'X';
            }
        }

        for (int i = 18; i < 27; ++i)
        {
            if ((board[9] & (1 << i)) == (1 << i))
            {
                int tmpI = 26-i;
                boardChar[tmpI/3][tmpI%3] = 'T';
            }
        }

        for (int h = 0; h < 3; ++h)
        {
            for (int w = 0; w < 3; ++w)
            {
                cerr << boardChar[h][w];
                if (w+1 < 3)
                {
                    cerr << " | ";
                }
            }
            cerr << endl;
            if (h+1 < 3)
                cerr << "---------- " << endl;
            
        }



        for (int h = 0; h < 9; ++h)
        {
            for (int w = 0; w < 9; ++w)
            {
                int gIndx = h/3*3 + w/3;
                int lIndx = 8-w%3-h%3*3;
                auto& b = board[gIndx];

                auto& gB = board[9];
                if ((gB & (1 << (8-gIndx))) == (1 << (8-gIndx)))
                {
                    cerr << "O ";
                }
                else if ((gB & (1 << (17-gIndx))) == (1 << (17-gIndx)))
                {
                    cerr << "X ";
                }
                else if ((gB & (1 << (26-gIndx))) == (1 << (26-gIndx)))
                {
                    cerr << "T ";
                }
                else if ((b & (1 << lIndx)) == (1 << lIndx))
                {
                    cerr << "O ";
                }
                else if ((b & (1 << (lIndx+9))) == (1 << (lIndx+9)))
                {
                    cerr << "X ";
                }
                else
                {
                    cerr << "_ ";
                }

                if (w == 8) continue;
                if (w%3 == 2)
                {
                    cerr << "   |||   ";
                }
                else
                {
                    cerr << " | ";
                }
            }
            if (h == 8) continue;
            if (h%3 == 2)
            {
                cerr << "\n_________________________________________________________" << endl;
                cerr << endl;
            }
            else
            {
                cerr << endl << endl;
                // cout << "\n----------------" << endl;
            }
            
        }
    }
}
// using namespace debug;

Random::Random()
{
    time_t seconds;

    seconds = time (NULL);
    state = seconds;
}

uint32_t Random::getRandom()
{
    uint32_t x = state;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    state = x;
    return x;
}

int Random::getRandom(int bound)
{
    return getRandom() % bound;
}

Timeout::Timeout()
: mTimeMS(-1)
{
}

Timeout::Timeout(int ms)
: mTimeMS(ms)
{
    startTime = std::chrono::high_resolution_clock::now();
}

void Timeout::setTimeBound(int ms)
{
    mTimeMS = ms;
}

inline
bool Timeout::isTimeLeft()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::high_resolution_clock::now() - startTime).count() < mTimeMS;
}


// int(32bit)represent board
// 0-9 - o; 9-18 - x
int win[8] = {
    0b000000111,
    0b000111000,
    0b111000000,
    0b100100100,
    0b010010010,
    0b001001001,
    0b100010001,
    0b001010100,
};

MCTS_AI::MCTS_AI()
{
    for (int b = 0; b < 512; ++b)
    {
        auto& availMoves = availLocalMovesStore[b];
        for (int i = 0; i < 9; ++i)
        {
            // empty space place here
            if ( (b & (1 << i)) == 0)
            {
                // createChild(node, g, i + (isMyMove? 9 : 0));
                // availMoves.push_back(make_pair(g, i + (isMyMove? 9 : 0)));
                availMoves.push_back(i);
            }
        }
    }

    board[10] = {0};
    root = new Node;
    root->parent = nullptr;
    gIndx = -1;
    isMyMove = true;

    gameFirstMove = true;
}

void MCTS_AI::recurseDelete(Node* node, int maxDepth, int depth)
{
    if (depth < maxDepth)
    {
        for (auto& child : node->children)
        {
            recurseDelete(child, maxDepth, depth+1);
        }
    }
    --nodesCnt;
    delete node;
}

inline
float MCTS_AI::UCB(Node* node, int totalN)
{
#ifdef DEBUG
    if (node->N < 1 || totalN < 1)
    {
        throw std::logic_error("UCB node->N less than 1 or totalN less than 1");
    }
#endif

    return (float)node->win / node->N + 1.3*(log(totalN)/(float)node->N);   
}

inline
Node* MCTS_AI::bestUCT(Node* node, int totalN)
{
    // return children[0];
    Node* bestChild = nullptr;
    float maxScore = -__FLT_MAX__;

#ifdef DEBUG
    if (node->children.size() == 0)
    {
        throw std::logic_error("bestUCT no children");
    }
#endif

    for (auto* child : node->children)
    {
        float score = UCB(child, totalN);
        if (score > maxScore)
        {
            maxScore = score;
            bestChild = child;
        }
    }
    return bestChild;
}

Node* MCTS_AI::createChild(Node* parent, int gIndx, int lIndx)
{
    Node* child = new Node;
    // gIndx can't be -1 in Node
    child->gPos = gIndx;
    child->lPos = lIndx;
    child->N = 0;
    child->win = 0;
    child->parent = parent;

    parent->children.push_back(child);

    ++nodesCnt;
    return child;
}

bool MCTS_AI::IsgBoardBusy(int gBoard, int gIndx)
{
    return (   
           (gBoard & (1 << (17-gIndx))) == (1 << (17-gIndx)) 
        || (gBoard & (1 << (8-gIndx)))  == (1 << (8-gIndx))
        || (gBoard & (1 << (26-gIndx))) == (1 << (26-gIndx)) 
        );
}

Node* MCTS_AI::pick_unvisited(Node* root, int board[10], int gIndx, bool isMyMove)
{
    int availLocalMove = -1;
    int childGIndx = gIndx;
    if (childGIndx == -1)
    {
        vector<pair<int, int>> listMoves;
        int gBoard = board[9];
        for (int g = 0; g < 9; ++g)
        {
            if (IsgBoardBusy(gBoard, g)) continue;

            int b = board[g];
            b = ( (b & 0b111111111) | (b >> 9));
            if (availLocalMovesStore[b].size() == 0) continue;

            for (int move : availLocalMovesStore[b])
            {
                listMoves.push_back(make_pair(g, move));
            }
        }

        vector<pair<int, int>> visitedMoves;
        
        for (auto& child : root->children)
        {
            visitedMoves.push_back(make_pair(child->gPos, isMyMove? (child->lPos - 9) : child->lPos ));
        }

        for (auto& lMove : listMoves)
        {
            bool found = false;
            for (auto& visMove: visitedMoves)
            {
                if (lMove.first == visMove.first && lMove.second == visMove.second)
                {
                    found = true;
                    break;
                }
            }
            if (!found)
            {
               availLocalMove = lMove.second;
               childGIndx = lMove.first;
               break;
            }
        }
    }
    else
    {
        int b = board[childGIndx];
        b = ( (b & 0b111111111) | (b >> 9));

        auto& listMoves = availLocalMovesStore[b];
        if (listMoves.size() == 0)
        {
            root->fullyExpanded = true;
            return nullptr;
        }

        vector<int> visitedMoves;
        for (auto& child : root->children)
        {
            visitedMoves.push_back(isMyMove? (child->lPos - 9) : child->lPos);
        }
        
        for (int lMove : listMoves)
        {
            bool found = false;
            for (int vMove : visitedMoves)
            {
                if (vMove == lMove)
                {
                    found = true;
                    break;
                }
            }
            if (!found)
            {
                availLocalMove = lMove;
                break;
            }
        }
    }

    if (availLocalMove == -1)
    {
        root->fullyExpanded = true;
        return nullptr;
    }

    // if (root->availMoves.size() == 0)
    // {
    //     findAvailMoves(root->availMoves, board, gIndx, isMyMove);
    // }

    // if (root->availMoves.size() == 0) return nullptr;
    Node* node = createChild(root, childGIndx, availLocalMove + (isMyMove? 9 : 0));
    return node;
}

int MCTS_AI::evaluate(int b)
{
    int bSecond = (b >> 9);
    for (int i = 0; i < 8; ++i)
    {
        if ((b & win[i]) == win[i])
        {
            return -10;
        }

        if ((bSecond & win[i]) == win[i])
        {
            return 10;
        }
    }

    // If no winner/tie return zero;
    return 0;
}

bool MCTS_AI::isMovesLeft(int board)
{
    int uniteB = (board) | (board >> 9) | (board >> 18);
    return (uniteB & 0b111111111) != 0b111111111;
}

bool MCTS_AI::isBoardTerminal(int board)
{
    int score = evaluate(board);
    return (score == 10 || score == -10 || !isMovesLeft(board));
}

void MCTS_AI::updateState(int board[10], Node* node, int& gIndx, bool& isMyMove)
{   
    // node cannot have gPos that equals -1 or larger than 8
    int& b = board[node->gPos];
    // lPos 0-9 or 9-18 already counted in node
    b |= (1 << node->lPos);

    int& gBoard = board[9];
    if (isBoardTerminal(b))
    {
        int score = evaluate(b);
        if (score == 10)
        {
            // x 9-18
            gBoard |= (1 << (17-node->gPos));
        }
        else if (score == -10)
        {
            // 0 0-9
            gBoard |= (1 << (8-node->gPos));
        }
        else
        {
            // tie 18-27
            gBoard |= (1 << (26-node->gPos));
        }
    }

    // node->lPos can be 0-18
    int new_gIndx = node->lPos >= 9 ? 17-node->lPos : 8-node->lPos;
    if (IsgBoardBusy(board[9], new_gIndx))
    {
        new_gIndx = -1;
    }

    gIndx = new_gIndx;
    isMyMove = !isMyMove;
}

Node* MCTS_AI::traverse(Node* node, int board[10], int totalN, int& gIndx, bool& isMyMove)
{
    // while (fullyExpanded(node))
    while(node->fullyExpanded && !node->children.empty())
    {
        node = bestUCT(node, totalN);
        updateState(board, node, gIndx, isMyMove);
    }
    // return pick_univisted(node.children) or node
    Node* unvisited = pick_unvisited(node, board, gIndx, isMyMove);

    if (unvisited == nullptr)
    {
        return node;
    }
    else
    {
        updateState(board, unvisited, gIndx, isMyMove);
        return unvisited;
    }
}

Node* MCTS_AI::pickRandomChild(Node* node, int board[10])
{
    if (node->children.size() == 0)
    {
        debug::printBoard(board);
        throw std::logic_error("Must be a child in pickRandomChild");
    }
    int rndNum = rnd.getRandom(node->children.size());
    Node* child = node->children[rndNum];

    return child;
}

Node* MCTS_AI::rollout_policy(Node* node, int board[10], int& gIndx, bool& isMyMove)
{   
    int childGIndx = gIndx;
    if (childGIndx == -1)
    {
        while (true)
        {
            childGIndx = rnd.getRandom(9);
            int b = board[childGIndx];
            b = ( (b & 0b111111111) | (b >> 9));
            if (availLocalMovesStore[b].size() > 0 && !IsgBoardBusy(board[9], childGIndx)) break;
        }
        
    }
    int b = board[childGIndx];
    b = ( (b & 0b111111111) | (b >> 9));
#ifdef DEBUG
    if (availLocalMovesStore[b].size() == 0)
    {
        throw std::logic_error("rollout policy no avail moves");
    }
#endif
    int size = availLocalMovesStore[b].size();
    int localMove = availLocalMovesStore[b][(rnd.getRandom(size))];
    auto child = createChild(node, childGIndx, localMove + (isMyMove? 9 : 0));
    updateState(board, child, gIndx, isMyMove);
    // cout << child->gPos << " " << child->lPos << " nextgPos: " << gIndx << endl;
    // debug::printBoard(board);
    // cout << "\nprnt smth: ";
    // cin.get();
    return child;
}

int MCTS_AI::result(int board)
{
    int score = evaluate(board);
    return score;
}

int MCTS_AI::rollout(Node*& node, int board[10], int gIndx, bool isMyMove)
{
    while (!isBoardTerminal(board[9]))
    {
        node = rollout_policy(node, board, gIndx, isMyMove);
    }
    return result(board[9]);
}

void MCTS_AI::updateStats(Node* node, int res)
{
    int winRes = node->lPos < 9? -10 : 10;

    if (res == winRes)
        node->win += 2;
    else if (res == 0)
        node->win += 1;
    
    node->N += 2;
}

bool MCTS_AI::isRoot(Node* node)
{
    return node->parent == nullptr;
}

void MCTS_AI::backpropogate(Node*& node, int res)
{
    if (isRoot(node))
    {
        node->N += 2;
        return;
    }
    updateStats(node, res);

    backpropogate(node->parent, res);
}

void MCTS_AI::mcts(Node* root, int board[10], int gIndx, bool isMyMove, bool gameFirstMove)
{    
    Timeout timeout(85);
    // if (gameFirstMove)
    // {
    //     timeout.setTimeBound(990);
    // }
    int cnt = 0;
    while ((cnt & 127) != 127 || timeout.isTimeLeft())
    // while (cnt < 100)
    {
        ++cnt;
        int board_cp[10];
        for (int i = 0; i < 10; ++i)
            board_cp[i] = board[i];
        int cp_gIndx = gIndx;
        bool cpisMyMove = isMyMove;

        Node* leaf = traverse(root, board_cp, root->N, cp_gIndx, cpisMyMove);
        int sim_res = rollout(leaf, board_cp, cp_gIndx, cpisMyMove);
        backpropogate(leaf, sim_res);
    }
    cerr << std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::high_resolution_clock::now() - timeout.startTime).count() << " ms" << endl;
    cerr << cnt << endl;
}

Move MCTS_AI::convertMove(int gPos, int lPos)
{
    if (lPos >= 9) lPos -= 9;

    Move move;
    move.row = gPos/3*3 + (8-lPos)/3;
    move.col = gPos%3*3 + (8-lPos)%3;
    return move;
}

void MCTS_AI::convertFromMove(int& gPos, int& lPos, int row, int col)
{
    gPos = row/3*3 + col/3;
    lPos = 8 - (row%3*3 + col%3);
}

Move MCTS_AI::findBestMove(int board[10], Node*& root, int gIndx, bool isMyMove, bool gameFirstMove)
{
    mcts(root, board, gIndx, isMyMove, gameFirstMove);

    Node* bestChild = bestUCT(root, root->N);
    
    auto startTime = std::chrono::high_resolution_clock::now();
    for (auto& child : root->children)
    {
        if (child != bestChild)
        {
            recurseDelete(child, 10);
        }
    }
    cerr << std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::high_resolution_clock::now() - startTime).count() << " ms" << endl;

    cerr << bestChild->gPos << " " << bestChild->lPos << " " <<  ((float)bestChild->win / bestChild->N) << " " << bestChild->N << endl;

    root = bestChild;
    return convertMove(bestChild->gPos, bestChild->lPos);
}

std::pair<int, int> MCTS_AI::think(int opRow, int opCol)
{
    if (opRow != -1)
    {
        int gPos;
        int lPos;
        convertFromMove(gPos, lPos, opRow, opCol);
        bool found = false;
        for (auto* child : root->children)
        {
            if (child->gPos == gPos && child->lPos == lPos)
            {
                root = child;
                found = true;
                break;
            }
        }
        if (!found)
        {
            root = createChild(root, gPos, lPos);
        }
        bool mMove = false;
        updateState(board, root, gIndx, mMove);
    }

    // int validActionCount;
    // cin >> validActionCount;
    // cin.ignore();
    // for (int i = 0; i < validActionCount; i++)
    // {
    //     int row;
    //     int col;
    //     cin >> row >> col;
    //     cin.ignore();
    // }
    // debug::printBoard(board);
    Move bestMove = findBestMove(board, root, gIndx, true, gameFirstMove);
    cout << bestMove.row << " " << bestMove.col << " " << nodesCnt << endl;
    updateState(board, root, gIndx, isMyMove);
    // debug::printBoard(board);

    gameFirstMove = false;

    return std::make_pair(bestMove.row, bestMove.col);
}