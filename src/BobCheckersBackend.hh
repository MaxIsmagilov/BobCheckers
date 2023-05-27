#ifndef BobChkrsBknd
#define BobChkrsBknd


#include <array>
#include <vector>
#include <algorithm>
#include <string>
#include <stack>
#include <iostream>
#include <random>

/// @brief the main checkers namespace
namespace Bob_checkers
{

/*      board types & squares   */

/// @brief define the bitboard type
typedef unsigned long long u64;

/// @brief squares of pieces
enum square 
{
    _A8, _B8, _C8, _D8, _E8, _F8, _G8, _H8, 
    _A7, _B7, _C7, _D7, _E7, _F7, _G7, _H7,
    _A6, _B6, _C6, _D6, _E6, _F6, _G6, _H6,
    _A5, _B5, _C5, _D5, _E5, _F5, _G5, _H5,
    _A4, _B4, _C4, _D4, _E4, _F4, _G4, _H4,
    _A3, _B3, _C3, _D3, _E3, _F3, _G3, _H3,
    _A2, _B2, _C2, _D2, _E2, _F2, _G2, _H2,
    _A1, _B1, _C1, _D1, _E1, _F1, _G1, _H1,
};

/// @brief coordinate strings
const std::string coordinates[64] = 
{
    "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8", 
    "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7", 
    "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6", 
    "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5", 
    "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
    "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
    "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
    "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1"
};

/*      bitboard utils      */

/// @brief finds whether a bit exists at an index
/// @param board 
/// @param square 
/// @return the bit (1 or 0 boolean)
static inline bool get_bit(u64 board, u64 square) { return (board & (1ULL << square));}

/// @brief adds a bit to the bitboard
/// @param board 
/// @param square 
/// @return an updated bitboard
static inline u64 push_bit(u64 board, int square) { return (board | (1ULL << square));}

/// @brief removes a bit from the bitboard
/// @param board 
/// @param square 
/// @return an updated bitboard
static inline u64 pop_bit(u64 board, int square) { return (board & ~(1ULL << square));}

/// @brief prints a bitboard
/// @param bb 
/// @return a string
std::string print_bitboard(const uint64_t& bb)
{
    std::string str = "";
    for (int i = 0; i < 64; i++)
    {
        str += (get_bit(bb,i)) ? '1' : '0';
        if (i%8 == 7) {str += '\n';}
    }
    return str;
}

/*      counting and finding bits       */

/// @brief counts the bits in a bitboard
/// @param bitboard 
/// @return the number of bits
static inline unsigned int count_bits(const uint64_t& bitboard) 
{
    uint64_t test = bitboard;
    unsigned int bitnumber = 0;
    while (test)
    {
        test &= test - 1;
        bitnumber++;
    }
    return bitnumber;
}

/// @brief  finds the index of the least significant bit
/// @param test 
/// @return the index
static inline int LSB_index(const uint64_t& test) 
{
    return (test) ? count_bits((test &  -test) - 1) : -1;
}

/*      main classes    */

/// @brief contains a move via a vector of the squares to move to
class Move_Wrapper
{
public:
    /// @brief the vector containing Move_Wrapper moves
    std::vector<int> _move;

    /// @brief default constructor for Move_Wrapper
    Move_Wrapper() {}

    /// @brief copy constructor for Move_Wrapper
    /// @param other 
    Move_Wrapper(const Move_Wrapper& other) : _move{std::move(other._move)} {}  

    /// @brief varaidic template constructor for Move_Wrapper
    /// @tparam ...params 
    /// @param ...squares a list of squares, should be an int type
        template <typename... params>
    Move_Wrapper(params... squares) : _move{squares...} {}

    /// @brief formats the move for output
    /// @return a std::string coordinate move, eg. e3d4
    std::string print_move() 
    {
        std::string result = "";
        for (int i : _move)
        {
            result += coordinates[i];
        }
        return std::forward<std::string>(result);
    }
};

/// @brief main board class
class Board
{
private: 

    /// @brief std::array of the bitboards,
    /// is formatted as {white men, white kings, black men, black kings}
    std::array<u64, 4> bitboards; 

    /// @brief contains the side to move. 1 and 0 for white and black, respectively.
    unsigned int side_to_move;

    /// @brief executes one jump of the move
    /// @param square1 the origin square
    /// @param square2 the destination square
    inline void single_move(int square1, int square2)
    {
        // identify if the move captures
        bool capture = (abs(square1 - square2) > 9);

        // find the starting and ending bits
        const u64 startbit = 1ULL << square1;
        const u64 endbit = 1ULL << square2;

        // init piece variable
        int piece{-1};

        // init promotion variable
        bool promote{false};

        // find piece and promotion status
        // for white
        if (side_to_move)
        {
            if (startbit & bitboards[0])
                piece = 0;
            else
                piece = 1;
            promote = (piece == 0 && (square2 / 8 == 0));
        }
        // for black
        else 
        {
            if (startbit & bitboards[2])
                piece = 2;
            else
                piece = 3;
            promote = (piece == 2 && (square2 / 8 == 7));
        }

        // remove the piece on the origin square
        bitboards[piece] &= ~startbit;

        // promote if needed
        if (promote) bitboards[piece+1] |= endbit;

        // else dont promote
        else bitboards[piece] |= endbit;

        // remove any possible captured pieces if relevant
        if (capture) 
        {
            bitboards[0] &= ~(1ULL << ((square1 + square2)/2));
            bitboards[1] &= ~(1ULL << ((square1 + square2)/2));
            bitboards[2] &= ~(1ULL << ((square1 + square2)/2));
            bitboards[3] &= ~(1ULL << ((square1 + square2)/2));
        }
    }

public:

    /// @brief default constructor for Board
    explicit Board() noexcept : bitboards{{0ULL, 0ULL, 0ULL, 0ULL}}, side_to_move{1}
     {  }

    /// @brief const copy constructor for Board
    /// @param other
    explicit Board(const Board& other) : bitboards{{other[0], other[1], other[2], other[3]}} , side_to_move{other.get_side()}
     {  }

    /// @brief default constructor for Board
    /// @param other
    explicit Board(Board& other) : bitboards{{other[0], other[1], other[2], other[3]}} , side_to_move{other.get_side()}
     {  }

    /// @brief assignment copy operator
    /// @param other 
    inline void operator=(const Board& other) 
     {  bitboards = {other[0], other[1], other[2], other[3]}; 
        side_to_move = other.get_side(); }

    /// @brief [] (bracket) operator for board, allows for array-stule access
    /// @param index 
    /// @return a modifiable reference to the bitboard
    inline u64& operator[](int index)
     {  return bitboards[index]; }
    
    /// @brief [] (bracket) operator for const board, allows for array-stule access
    /// @param index 
    /// @return a value of the bitboard 
    inline u64 operator[](int index) const
     {  return bitboards[index]; }

    /// @brief gets the side to move
    /// @return 1 or 0 for white or black, respectively.
    inline unsigned int get_side() const
     {  return side_to_move; }

    /// @brief flips the side to move
    inline void flip_side()
     {  side_to_move ^= 1; }

    /// @brief executes a move
    /// @tparam T should be an int type
    /// @tparam ...params should be an int type
    /// @param square1 the origin square
    /// @param square2 the second square
    /// @param ...other_squares any other squares
        template <typename T, typename... params>
    inline void move(T square1, T square2, params... other_squares)
    {  
        // just executes single moves one by one 
        std::vector<int> moves{(int) square1, (int) square2, other_squares...};
        for (int i = 0; i < moves.size() - 1; i++)
        {
            single_move(moves[i], moves[i+1]);
        }
        flip_side();
    }

    /// @brief executes a move
    /// @param mv 
    inline void move(Move_Wrapper& mv)
    {   
        // just executes single moves one by one
        std::vector<int> moves{std::move(mv._move)};
        for (int i = 0; i < moves.size() - 1; i++)
        {
            single_move(moves[i], moves[i+1]);
        }
        flip_side();
    }

    /// @brief executes a move
    /// @param mv 
    inline void move(Move_Wrapper&& mv)
    {   
        // just executes single moves one by one
        std::vector<int> moves{std::move(mv._move)};
        for (int i = 0; i < moves.size() - 1; i++)
        {
            single_move(moves[i], moves[i+1]);
        }
        flip_side();
    }
    
    /// @brief loads a position 
    /// @param position 
    /// @note string is fromatted like a chess FEN, "1b1b1b1b/b1b1b1b1/1b1b1b1b/8/8/w1w1w1w1/1w1w1w1w/w1w1w1w1 w"
    /// for the starting position
    inline void load_pos(std::string position)
    {   
        bitboards = {0Ull, 0ULL, 0ULL, 0ULL};
        int index{0};
        int strpos{0};
        while (position[strpos] != ' ')
        {
            char token = position[strpos];
            if (token > '0' && token <= '9') index += token - '0';
            else if (token == '/') ;
            else
            {
                u64& bitboard = [&]() -> u64&
                    {
                        switch (token)
                        {
                        case 'w': return bitboards[0];
                        case 'W': return bitboards[1];
                        case 'b': return bitboards[2];
                        case 'B': return bitboards[3];
                        }
                        return bitboards[0];
                    }();
                bitboard |= 1ULL << index;
                index++;
            }
            strpos++;
        }
        side_to_move = (position[++strpos] == 'w');
    }

    /// @brief formats a board for printing
    /// @return a std::string representation of the board
    inline std::string print_board()
    {
        // create pieces
        char pieces[4] = {'o', '0', 'a', '@'};

        // the string of the board
        std::string board{""};

        // go through all squares
        for (int i = 0; i < 64; i++)
        {
            int j = 0;
            for (j; j < 4; j++)
            {
                // find if there is a piece on the square
                if (bitboards[j] & (1ULL << i))
                {
                    board += pieces[j];
                    break;
                }   
            }
            
            // if there is not a piece, add a space
            if (j==4) board += ((i/8 + i) % 2) ? ' ' : ' ' ;

            // add a new line if needed
            if (i%8 == 7 && i < 60) board += "\n--|---|---|---|---|---|---|-- \n";

            // add a vertical bar otherwise
            else if (i != 63) board += " | ";
        }

        // add the side to move
        board += "\nside to move: ";
        board += ((side_to_move == 1) ? "white" : "black");

        return board;
    }
};

/// @brief manages a stack of boards, used for make/unmake move
class Board_Stack
{
private:

    /// @brief a std::stack of boards
    std::array<Board, 256> boards;
    Board* _top{&boards[0]};
    
public:

    /// @brief default constructor for Board_Stack
    explicit Board_Stack() noexcept {}

    /// @brief copy constructor for Board_Stack
    /// @param other 
    Board_Stack(const Board& other) 
     { boards[0] = other;}

    /// @brief gets the board at the top of the stack
    /// @return a reference to the board
    inline Board& top()
    {
        return std::forward<Board&>(*_top);
    }

    /// @brief move for the board stack 
    /// @param mv 
    inline void make_move(Move_Wrapper& mv)
    {
        memcpy(_top+1, _top, sizeof(Board));
        (++_top)->move(mv);
    }

    /// @brief move for the board stack 
    /// @param mv 
    inline void make_move(Move_Wrapper&& mv)
    {
        memcpy(_top+1, _top, sizeof(Board));
        (++_top)->move(mv);
    }

    /// @brief make a move without flipping sides
    /// @param mv 
    inline void make_partial_move(Move_Wrapper& mv)
    {
        memcpy(_top+1, _top, sizeof(Board));
        (++_top)->move(mv);
        _top->flip_side();
    }

    /// @brief make a move without flipping sides
    /// @param mv 
    inline void make_partial_move(Move_Wrapper&& mv)
    {
        memcpy(_top+1, _top, sizeof(Board));
        (++_top)->move(mv);
        _top->flip_side();
    }

    /// @brief undoes a move
    inline void unmake_move()
    {
        --_top;
    }
};

/*      drivers         */

/// @brief move generation namespace
namespace move_generator
{

/// @brief 'a' file bitboard constant
constexpr u64 A_FILE = 72340172838076673ULL;

/// @brief 'b' file bitboard constant
constexpr u64 B_FILE = 144680345676153346ULL;

/// @brief 'g' file bitboard constant
constexpr u64 G_FILE = 4629771061636907072ULL;

/// @brief 'h' file bitboard constant
constexpr u64 H_FILE = 9259542123273814144ULL;

/// @brief eighth rank bitboard constant
constexpr u64 RANK_8 = 255ULL;

/// @brief seventh rank bitboard constant
constexpr u64 RANK_7 = 65280ULL;

/// @brief second rank bitboard constant
constexpr u64 RANK_2 = 71776119061217280;

/// @brief first rank bitboard constant
constexpr u64 RANK_1 = 18374686479671623680ULL;

/// @brief silent moves table
/// @note formatted as {white, black, king}
u64 silent_table[3][64];

/// @brief capture table
/// @note formatted as {white, black, king}
u64 capture_table[3][64]; 

/// @brief initialize move table
void init_move_table()
{
    for (int i = 0; i < 64; i++)
    {
        u64 pos = 1ULL << i;
        u64 table = 0ULL;
        if (pos & ~A_FILE & ~RANK_8) table |= 1ULL << (i - 9);
        if (pos & ~H_FILE & ~RANK_8) table |= 1ULL << (i - 7);
        silent_table[0][i] = table;
    }
    for (int i = 0; i < 64; i++)
    {
        u64 pos = 1ULL << i;
        u64 table = 0ULL;
        if (pos & ~A_FILE & ~RANK_1) table |= 1ULL << (i + 7);
        if (pos & ~H_FILE & ~RANK_1) table |= 1ULL << (i + 9);
        silent_table[1][i] = table;
    }
    for (int i = 0; i < 64; i++)
    {
        silent_table[2][i] = silent_table[0][i] | silent_table[1][i];
    }

    for (int i = 0; i < 64; i++)
    {
        u64 pos = 1ULL << i;
        u64 table = 0ULL;
        if (pos & ~(A_FILE | B_FILE | RANK_8 | RANK_7)) table |= 1ULL << (i - 18);
        if (pos & ~(H_FILE | G_FILE | RANK_8 | RANK_7)) table |= 1ULL << (i - 14);
        capture_table[0][i] = table;
    }
    for (int i = 0; i < 64; i++)
    {
        u64 pos = 1ULL << i;
        u64 table = 0ULL;
        if (pos & ~(A_FILE | B_FILE | RANK_2 | RANK_1)) table |= 1ULL << (i + 14);
        if (pos & ~(H_FILE | G_FILE | RANK_2 | RANK_1)) table |= 1ULL << (i + 18);
        capture_table[1][i] = table;
    }
    for (int i = 0; i < 64; i++)
    {
        capture_table[2][i] = capture_table[0][i] | capture_table[1][i];
    }
}

/// @brief move generator function object
class Move_Generator
{
private:
    /// @brief the main boardstack
    Board_Stack _move_stack;

    /// @brief the board that is being investigated
    const Board _bd;

    bool _captures_available{false};

public:

    /// @brief constructor for move_generator
    /// @param bd 
    Move_Generator(Board&& bd): _move_stack{bd}, _bd{bd} {}

    /// @brief constructor for move_generator
    /// @param bd 
    Move_Generator(Board& bd): _move_stack{bd}, _bd{bd} {}

    /// @brief get captures on a square
    /// @param bd 
    /// @param square 
    /// @return a vector of move_wrappers
    inline std::vector<Move_Wrapper> get_captures(const int square)
    {
        // grab the board state 
        const Board& bd = _move_stack.top();

        // init move vector
        std::vector<Move_Wrapper> moves;

        // init constants
        const int aside = (bd.get_side()) ? 0 : 2;
        const int dside = (bd.get_side()) ? 2 : 0;
        const u64 defending_occ = bd[0 + dside] | bd[1 + dside];
        const u64 attacking_occ = bd[0 + aside] | bd[1 + aside];
        const u64 all_occ = defending_occ | attacking_occ;

        // create man moves
        u64 bb = (1ULL << square) & bd[aside];
        while (bb)
        {
            // lookup attack value
            const int start = square;
            bb = pop_bit(bb, start);
            u64 att = capture_table[!(bd.get_side())][start];

            // go through attacks and parse
            while (att)
            {
                // get a possible end square
                const int end = LSB_index(att);
                att = pop_bit(att, end);

                // parse move if it captures a piece and moves to an available square
                if (((1ULL << end) & ~all_occ) && ((1ULL << ((start+end) / 2)) & defending_occ)) 
                {
                    // make a partial move and recursively find next captures
                    _move_stack.make_partial_move(Move_Wrapper(start,end));
                    std::vector<Move_Wrapper> temp = get_captures(end);

                    // if there are no future captures, continue
                    if (temp.size() == 0 || end / 8 == 0 || end / 8 == 7) moves.push_back(Move_Wrapper(start,end));

                    // else concatenate the future moves to the current ones
                    else for (Move_Wrapper mw : temp)
                    {
                        moves.push_back(std::forward<Move_Wrapper>(Move_Wrapper(start,end)));
                        std::for_each(mw._move.begin() + 1, mw._move.end(), [&](int i) {moves.back()._move.push_back(i);});
                    }

                    // unmake the stack move
                    _move_stack.unmake_move();
                }
            }
        }

        // create king moves
        bb = (1ULL << square) & bd[aside+1];
        while (bb)
        {
            // lookup attack value
            const int start = square;
            bb = pop_bit(bb, start);
            u64 att = capture_table[2][start];
            
            // go through attacks and parse
            while (att)
            {
                // get a possible end square
                const int end = LSB_index(att);
                att = pop_bit(att, end);

                // parse move if it captures a piece and moves to an available square
                if (((1ULL << end) & ~all_occ) && ((1ULL << ((start+end) / 2)) & defending_occ)) 
                {
                    // make a partial move and recursively find next captures
                    _move_stack.make_partial_move(Move_Wrapper(start,end));
                    std::vector<Move_Wrapper> temp = get_captures(end);

                    // if there are no future captures, continue
                    if (temp.size() == 0) moves.push_back(Move_Wrapper(start,end));

                    // else concatenate the future moves to the current ones
                    else for (Move_Wrapper mw : temp)
                    {
                        moves.push_back(std::forward<Move_Wrapper>(Move_Wrapper(start,end)));
                        std::for_each(mw._move.begin() + 1, mw._move.end(), [&](int i) {moves.back()._move.push_back(i);});
                    }

                    // unmake the stack move
                    _move_stack.unmake_move();
                } 
            }
        }

        // return the resultant vector
        return std::forward<std::vector<Move_Wrapper>>(moves);
    }

    /// @brief gets all captures
    /// @param bd 
    /// @return a vector of move_wrappers
    inline std::vector<Move_Wrapper> get_all_captures()
    {
        // create final and piece-wise move vectors
        std::vector<Move_Wrapper> moves;
        std::vector<Move_Wrapper> newmoves;

        // go through squares and find captures
        for (int i = 0; i < 64; i++)
        {
            // get captures on that square
            newmoves = get_captures(i);

            // add all new moves to the final vector
            for (Move_Wrapper mw: newmoves)
             {  moves.push_back(std::forward<Move_Wrapper>(mw)); }
        }

        // return the vector
        return std::forward<std::vector<Move_Wrapper>>(moves);
    }

    /// @brief gets all silent moves
    /// @param bd 
    /// @return a vector of move_wrappers
    inline std::vector<Move_Wrapper> get_silents()
    {
        // create blank move vector
        std::vector<Move_Wrapper> moves;

        // create helpful constants
        const int aside = (_bd.get_side()) ? 0 : 2;
        const int dside = (_bd.get_side()) ? 2 : 0;
        const u64 defending_occ = _bd[0 + dside] | _bd[1 + dside];
        const u64 attacking_occ = _bd[0 + aside] | _bd[1 + aside];
        const u64 all_occ = defending_occ | attacking_occ;

        // check man bitboard
        u64 bb = _bd[aside];
        while (bb)
        {
            // pop the least significant bit and use it
            // to look up the move value
            int start = LSB_index(bb);
            bb = pop_bit(bb, start);
            u64 att = silent_table[!(_bd.get_side())][start];

            // parse the moves
            while (att)
            {
                // find the end square
                int end = LSB_index(att);
                att = pop_bit(att, end);

                // if the end square is not occupied, consider it valid
                if ((1ULL << end) & ~all_occ) moves.push_back(Move_Wrapper(start,end));
            }
        }
        
        // check king bitboard
        bb = _bd[aside+1];
        while (bb)
        {
            // pop the least significant bit and use it
            // to look up the move value
            int start = LSB_index(bb);
            bb = pop_bit(bb, start);
            u64 att = silent_table[2][start];

            // parse the moves
            while (att)
            {
                // find the end square
                int end = LSB_index(att);
                att = pop_bit(att, end);

                // if the end square is not occupied, consider it valid
                if ((1ULL << end) & ~all_occ) moves.push_back(Move_Wrapper(start,end));
            }
        }

        // return the resultant vector
        return std::forward<std::vector<Move_Wrapper>>(moves);
    }

    /// @brief checks whether captures are available
    /// @return true of false
    inline bool captures_available()
    {
        return _captures_available;
    }

    /// @brief gets all the moves available
    /// @return a vector of moves
    inline std::vector<Move_Wrapper> operator()() 
    {
        std::vector<Move_Wrapper> moves = get_all_captures();

        _captures_available = true;

        // if there are no captures, set the moves as the silent moves
        // this ensures that if a capture is available, only captures will be considered
        if (moves.size() == 0) {moves = get_silents(); _captures_available = false;}

        // return the resultant vector
        return std::forward<std::vector<Move_Wrapper>>(moves);

    }

};

} // end of move generation namespace

/// @brief transposition table namespace
namespace tt_util
{

/// @brief hash keys for table
u64 hash_keys[4][64];

/// @brief initializing method to generate hash keys
void generate_hash_keys()
{
    std::default_random_engine generator;
    std::uniform_int_distribution<u64> distribution(0ULL, 0xFFFFFFFFFFFFFFFFULL);
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 64; j++)
        {
            hash_keys[i][j] = distribution(generator);
        }
    }
}

/// @brief calculates the key for a board
/// @param bd 
/// @return the hash key
static inline u64 get_key(Board& bd)
{
    u64 cpy[4] = {bd[0], bd[1], bd[2], bd[3]};
    u64 key = 0ULL;
    for (int i = 0; i < 4;  i++)
    {
        while (cpy[i])
        {
            int j = LSB_index(cpy[i]);
            cpy[i] = pop_bit(cpy[i], j);
            key ^= hash_keys[i][j];
        }
    }
    return key;
} 

/// @brief entry type enum
enum tt_entry_type {EXACT, LBOUND, UBOUND, FAIL};

/// @brief transposition table structure
struct TT_Entry
{
    tt_entry_type _type;
    u64 _key{0ULL};
    int _value{0};
};

/// @brief size of the transposition table, currently ~3.2 GB
constexpr size_t tablesize = sizeof(TT_Entry) * 131072;

/// @brief failed entry constant
constexpr TT_Entry failed_entry{FAIL, 0ULL, 0};

/// @brief transposition table class
class Transposition_Table
{
private:

    /// @brief the table, an array of the TT_Entry type
    std::array<TT_Entry, tablesize/sizeof(TT_Entry)> _table;

    /// @brief
    const u64 length{tablesize/sizeof(TT_Entry)};

    /// @brief helper function to find the index in which the entry is stored
    /// @param tte 
    /// @return the index
    inline u64 get_index(const u64 key)
     {  return key % length;  }

public:

    /// @brief constructor for transposition table
    explicit Transposition_Table() noexcept
     {  std::fill(_table.begin(), _table.end(), failed_entry);  }

    /// @brief finds the desired entry
    /// @param key
    /// @return the entry or the failed_entry constant
    inline TT_Entry find(const u64 key)
     {  return (key == _table[get_index(key)]._key) ? _table[get_index(key)] : failed_entry; }

    /// @brief adds an entry to the transposition table
    /// @param tte
    inline void add(TT_Entry& tte)
     {  _table[get_index(tte._key)] = std::forward<TT_Entry>(tte);  }

    /// @brief adds an entry to the transposition table
    /// @param tte 
    inline void add(TT_Entry&& tte)
     {  _table[get_index(tte._key)] = std::forward<TT_Entry>(tte); }
};

} // end of TT namespace

/// @brief evaluation namespace
namespace evaluation
{

/// @brief man value constants
constexpr int man_values[64] 
{
     0,  0,  0,  0,  0,  0,  0,  0,
    24,  0, 10,  0,  8,  0,  9,  0,
     0,  8,  0, 10,  0, 10,  0, 24,
     3,  0,  4,  0,  5,  0,  1,  0,
     0,  2,  0,  13,  0,  8,  0,  1,
     1,  0,  2,  0,  1,  0,  0,  0,
     0, -1,  0,  7,  0,  9,  0, 25,
    -5,  0, 23,  0, 10,  0, 32,  0
};

/// @brief king value constants
constexpr int king_values[64] 
{
     0, -7,  0, -1,  0, -3,  0,-10,
    -5,  0,  0,  0, -1,  0, -1,  0,
     0,  1,  0, 10,  0,  8,  0, -1,
    -1,  0, 10,  0, 11,  0,  1,  0,
     0,  2,  0, 11,  0, 10,  0, -1,
    -1,  0,  8,  0,  10, 0,  0,  0,
     0, -1,  0, -1,  0, -1,  0, -1,
   -10,  0,  0,  0, -1,  0, -7,  0
};

/// @brief piece value constants
constexpr int piece_values[4]
 {  100, 1000, -100, -1000  };

/// @brief flips the square
/// @param square 
/// @return a flipped square

#define flip_square(square) (63 - square)


/// @brief evaluates the position
/// @param bd 
/// @return the evaluation (an int)
static inline int eval(const Board& bd)
{
    // load bitboards into temp values
    u64 pieces[4] = {bd[0], bd[1], bd[2], bd[3]};

    // init value
    int val = 0;

    // go through all pieces
    for (int i = 0; i < 4; i++)
    {
        // calculate each piece from a set's value and add it to the value variable
        while (pieces[i])
        {
            int square = LSB_index(pieces[i]);
            pieces[i] = pop_bit(pieces[i], square);
            val += piece_values[i];
            if (i == 0)
                val += man_values[square];
            else if (i == 1)
                val += king_values[square];
            else if (i == 2)
                val -= man_values[flip_square(square)];
            else if (i == 3)
                val -= king_values[flip_square(square)];
        }
    }

    // return the value
    return val;
}

} // end of the evaluation namespace

/// @brief algorithm namespace
namespace algo
{

/// @brief arbitrarily large number to use in place of infinity
const int infinity = 1000000;

/// @brief checkmate value
const int game_over = 100000;

/// @brief move information struct
struct Move_Info
{
    /// @brief a Move_Wrapper containing the move
    Move_Wrapper _mvwrpr;

    /// @brief the count of total nodes checked
    int _node_count;

    /// @brief the calculated value of the board + move
    int _value;

    /// @brief overriden > operator
    /// @param other 
    /// @return the value of the > operator on the `value` variables of each
    inline bool operator>(const Move_Info& other)
    {
        return this->_value > other._value;
    }

    /// @brief overriden < operator
    /// @param other 
    /// @return the value of the < operator on the `value` variables of each
    inline bool operator<(const Move_Info& other)
    {
        return this->_value < other._value;
    }

    /// @brief represents the Move_Wrapper as a string
    /// @return a std::string
    std::string to_string()
    {
        std::string s;
        s += _mvwrpr.print_move();
        s += ",\t";
        s += std::to_string(_value);
        s += ",\t";
        s += std::to_string(_node_count);
        return std::forward<std::string>(s);
    }
};

/// @brief function object for calculating values
class Move_Evaluator
{
private:
    /// @brief a board stack to keep track of plies
    Board_Stack _this_stack;

    /// @brief transposition table reference
    tt_util::Transposition_Table& _table;

    /// @brief a deepest search variable
    const int _depth{0};

    /// @brief the depth of quiescence
    static const int quiescence_depth{2};

    const int cut_depth{1000};

    /// @brief the total number of moves
    int* _moves{nullptr};

    /// @brief the move that is being checked
    const Move_Wrapper _mvwrpr;

    /// @brief the value of the node
    int _value;

    /// @brief waits for the search to quiet down
    /// @param depth 
    /// @param alpha 
    /// @param beta 
    /// @param color 
    /// @return the quiescence value
    inline int quiescence(int depth, int alpha, int beta, int color)
    {
        (*_moves)++;

        move_generator::Move_Generator mg(_this_stack.top());
        std::vector<Move_Wrapper> moveslist = mg();
        
        // return the value if node quiet or if depth = 0
        if (depth == 0 || mg.captures_available())
            return evaluation::eval(std::forward<Board&>(_this_stack.top())) * color; 
        
        // store the original alpha
        int original_alpha = alpha;

        // find a transpositon table entry
        tt_util::TT_Entry entry = _table.find(tt_util::get_key(_this_stack.top()));
        if (entry._type != tt_util::FAIL)
        {
            if (entry._type == tt_util::EXACT)
                return entry._value;
            if (entry._type == tt_util::LBOUND)
                alpha = std::max(alpha, entry._value);
            if (entry._type == tt_util::UBOUND)
                beta = std::min(beta, entry._value);
            if (alpha >= beta)
                return entry._value;
        }

        // create a move vector

        // if there are no moves, it is game over
        if (moveslist.size() == 0)
            return -(game_over + depth);

        // set an arbitrarily large negative number
        int value = -infinity;

        // go through the move list
        for (Move_Wrapper mw: moveslist)
        {
            _this_stack.make_move(mw);
            value = std::max(value, -quiescence(depth-1, -beta, -alpha, -color));
            alpha = std::max(value, alpha);
            _this_stack.unmake_move();
            if (alpha >= beta)
                break;
        }

        // create a new transposition table entry
        tt_util::TT_Entry newEntry;
        newEntry._value = value;
        if (value <= original_alpha)
            newEntry._type = tt_util::UBOUND;
        else if (value >= beta)
            newEntry._type = tt_util::LBOUND;
        else
            newEntry._type = tt_util::EXACT;
        _table.add(newEntry);

        return value;
    }

    /// @brief the negamax algorithm
    /// @param depth 
    /// @param alpha 
    /// @param beta 
    /// @return the result of the step of negamax
    /// @note called recursively
    inline int negamax(int depth, int alpha, int beta, int color, bool can_cut)
    {
        /* pseudocode for  the negamax algorithm */

        /*
            function negamax(node, depth, α, β, color) is
                if depth = 0 or node is a terminal node then
                    return color × the heuristic value of node

                childNodes := generateMoves(node)
                childNodes := orderMoves(childNodes)
                value := −∞
                foreach child in childNodes do
                    value := max(value, −negamax(child, depth − 1, −β, −α, −color))
                    α := max(α, value)
                    if α ≥ β then
                        break (* cut-off *)
                return value

            (* Initial call for Player A's root node *)
            negamax(rootNode, depth, −∞, +∞, 1)
        */

       
        
        move_generator::Move_Generator mg(_this_stack.top());
        std::vector<Move_Wrapper> moveslist = mg();

        if (depth <= cut_depth && !mg.captures_available() && can_cut)
        {
            depth -= 2;
            alpha = beta-1;
        }

        can_cut = false;

        // increment total nodes
        (*_moves)++;

        // store the original alpha
        int original_alpha = alpha;

        // find a transpositon table entry
        tt_util::TT_Entry entry = _table.find(tt_util::get_key(_this_stack.top()));
        if (entry._type != tt_util::FAIL)
        {
            if (entry._type == tt_util::EXACT)
                return entry._value;
            if (entry._type == tt_util::LBOUND)
                alpha = std::max(alpha, entry._value);
            if (entry._type == tt_util::UBOUND)
                beta = std::min(beta, entry._value);
            if (alpha >= beta)
                return entry._value;
        }


        // return the value if depth cutoff
        if (depth <= 0)
        {
            if (mg.captures_available()) return -quiescence(quiescence_depth, -beta, -alpha, -color);
            else return evaluation::eval(std::forward<Board&>(_this_stack.top())) * color;
        }

        // if there are no moves, it is game over
        if (moveslist.size() == 0)
            return -(game_over + depth);

        // set an arbitrarily large negative number
        int value = -infinity;

        // negamax through the move list
        for (Move_Wrapper mw: moveslist)
        {
            _this_stack.make_move(mw);
            value = std::max(value, -negamax(depth-1, -beta, -alpha, -color, can_cut));
            alpha = std::max(value, alpha);
            _this_stack.unmake_move();
            can_cut = true;
            if (alpha >= beta)
                break;
        }
        
        // create a new transposition table entry
        tt_util::TT_Entry newEntry;
        newEntry._value = value;
        if (value <= original_alpha)
            newEntry._type = tt_util::UBOUND;
        else if (value >= beta)
            newEntry._type = tt_util::LBOUND;
        else
            newEntry._type = tt_util::EXACT;
        _table.add(newEntry);

        // return the value
        return value;
    }


public:

    /// @brief constructor for Move_Evaluator
    /// @param bd 
    /// @param depth 
    /// @param mw 
    /// @param table
    Move_Evaluator(Board& bd, const int depth, Move_Wrapper mw, tt_util::Transposition_Table& table) : 
        _this_stack{Board_Stack(bd)}, _depth{depth}, _moves{new int{0}}, _mvwrpr{std::move(mw)}, _table{table}, cut_depth{(depth-4)}
     {  _this_stack.make_move(mw); }

    /// @brief operator (), calculates value
    /// @return the value of the node
    inline Move_Info operator()() 
     {  _value = -negamax(_depth, -infinity, infinity, ((_this_stack.top().get_side()) ? 1 : -1), false);
        return {std::move(_mvwrpr), *_moves, _value};   }

    /// @brief destructor to free up pointer-bound memory
    ~Move_Evaluator() 
     {  delete _moves;  }
};

/// @brief gets the best move
/// @param depth 
/// @param bd 
/// @param transposition
/// @return the best (hopefully) move, a null move if no moves possible
inline Move_Info get_best_move(int depth, Board& bd, tt_util::Transposition_Table& transposition)
{
    // create move and final vectors
    move_generator::Move_Generator mg(bd);
    std::vector<Move_Wrapper> moveslist = mg();
    std::vector<Move_Info> calculated_list;

    // create node counter
    int totalnodes = 0;

    // calculate all moves and accumulate node count into `totalnodes`
    for (Move_Wrapper mw : moveslist)
    {
        Move_Evaluator me(bd, depth-1, std::move(mw), transposition);
        calculated_list.push_back(me());
        totalnodes += calculated_list.back()._node_count;
    }

    // sort the list, descending
    std::sort(calculated_list.begin(), calculated_list.end(), std::greater<>());

    // return a null move if there are no moves avalavble
    if (calculated_list.size() == 0) return {Move_Wrapper(0, 0), 0, 0};

    // return the best node
    return {calculated_list.front()._mvwrpr, totalnodes, calculated_list.front()._value};
}

} // end of algo namespace

} // end of main BobCheckers namespace

#endif // BobChkrsBknd