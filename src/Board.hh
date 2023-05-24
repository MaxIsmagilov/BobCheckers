#include <array>
#include <vector>
#include <algorithm>
#include <string>
#include <stack>
#include <iostream>


namespace Bob_checkers
{

#define u64 unsigned long long 

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

#define get_bit(board, square) (board & (1ULL << square))

#define push_bit(board, square) (board | (1ULL << square))

#define pop_bit(board, square) (board & ~(1ULL << square))

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

class move_wrapper
{
public:
    std::vector<int> _move;
    move_wrapper() {}
    move_wrapper(const move_wrapper& other) : _move{std::move(other._move)} {}  
        template <typename... params>
    move_wrapper(params... squares) : _move{squares...} {}
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

class Board
{
private: 
    std::array<u64, 4> bitboards; // w, W, b, B
    unsigned int side_to_move;

    inline void single_move(int square1, int square2)
    {
        bool capture = (abs(square1 - square2) > 9);
        if (capture) std::cout<< "esdfa\n";
        const u64 startbit = 1ULL << square1;
        const u64 endbit = 1ULL << square2;
        int piece = -1;
        bool promote = false;
        if (side_to_move)
        {
            if (startbit & bitboards[0])
                piece = 0;
            else
                piece = 1;
            promote = (piece == 0 && (endbit / 8 == 0));
        }
        else 
        {
            if (startbit & bitboards[2])
                piece = 2;
            else
                piece = 3;
                
            promote = (piece == 2 && (endbit / 8 == 7));
        }
        bitboards[piece] ^= startbit;
        if (!promote) bitboards[piece] ^= endbit;
        else bitboards[piece+1] ^= endbit;

        if (capture) 
        {
            bitboards[0] &= ~(1ULL << ((square1 + square2)/2));
            bitboards[1] &= ~(1ULL << ((square1 + square2)/2));
            bitboards[2] &= ~(1ULL << ((square1 + square2)/2));
            bitboards[3] &= ~(1ULL << ((square1 + square2)/2));
        }
    }

public:
    explicit Board() : bitboards{{0ULL, 0ULL, 0ULL, 0ULL}}, side_to_move{1}
     {  }

    explicit Board(const Board& other) : bitboards{{other[0], other[1], other[2], other[3]}} , side_to_move{1}
     {  }

    explicit Board(Board& other) : bitboards{{other[0], other[1], other[2], other[3]}} , side_to_move{1}
     {  }

    inline void operator=(const Board& other) 
     {  bitboards = {other[0], other[1], other[2], other[3]}; 
        side_to_move = other.get_side(); }

    inline u64& operator[](int index)
     {  return bitboards[index]; }
    
    inline u64 operator[](int index) const
     {  return bitboards[index]; }

    inline unsigned int get_side() const
     {  return side_to_move; }

    inline void flip_side()
     {  side_to_move ^= 1; }

        template <typename T, typename... params>
    inline void move(T square1, T square2, params... other_squares)
    {   
        std::vector<int> moves{(int) square1, (int) square2, other_squares...};
        for (int i = 0; i < moves.size() - 1; i++)
        {
            single_move(moves[i], moves[i+1]);
        }
        flip_side();
    }

    inline void move(move_wrapper& mv)
    {   
        std::vector<int> moves{std::move(mv._move)};
        for (int i = 0; i < moves.size() - 1; i++)
        {
            single_move(moves[i], moves[i+1]);
        }
        flip_side();
    }
    
    /// @brief loads a position 
    /// @param position 
    /// @note string is fromatted like a chess FEN, 1b1b1b1b/b1b1b1b1/1b1b1b1b/8/8/w1w1w1w1/1w1w1w1w/w1w1w1w1 w
    inline void load_pos(std::string position)
    {   
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

    inline std::string print_board()
    {
        char pieces[4] = {'o', '0', 'a', '@'};
        std::string board{""};
        for (int i = 0; i < 64; i++)
        {
            int j = 0;
            for (j; j < 4; j++)
            {
                if (bitboards[j] & (1ULL << i))
                {
                    board += pieces[j];
                    break;
                }   
            }
            
            if (j==4) board += ((i/8 + i) % 2) ? ' ' : ' ' ;
            if (i%8 == 7 && i < 60) board += "\n--|---|---|---|---|---|---|-- \n";
            else if (i != 63) board += " | ";
        }

        board += "\nside to move: ";
        board += ((side_to_move == 1) ? "white" : "black");
        return board;
    }
};

class BoardStack
{
private:
    std::stack<Board> boards;
public:
    BoardStack() {}
    BoardStack(const Board& other) 
     {boards.push(other);}
     
        template <typename T, typename... params>
    inline void make_move(T square1, T square2, params... other_squares)
    {
        boards.push(boards.top());
        boards.top().move(square1, square2, other_squares...);
    }

    inline void make_move(move_wrapper& mv)
    {
        boards.push(boards.top());
        boards.top().move(mv);
    }

    inline void unmake_move()
    {
        boards.pop();
    }
};

namespace move_generator
{

constexpr u64 A_FILE = 72340172838076673ULL;

constexpr u64 B_FILE = 144680345676153346ULL;

constexpr u64 G_FILE = 4629771061636907072ULL;

constexpr u64 H_FILE = 9259542123273814144ULL;

constexpr u64 RANK_8 = 255ULL;

constexpr u64 RANK_7 = 65280ULL;

constexpr u64 RANK_2 = 71776119061217280;

constexpr u64 RANK_1 = 18374686479671623680ULL;

u64 silent_table[3][64]; // white, black, king
u64 capture_table[3][64]; // white, black, king

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

inline std::vector<move_wrapper> get_captures(Board& bd)
{
    BoardStack move_stack(bd);
    std::vector<move_wrapper> moves;
    const int aside = (bd.get_side()) ? 0 : 2;
    const int dside = (bd.get_side()) ? 2 : 0;
    const u64 defending_occ = bd[0 + dside] | bd[1 + dside];
    const u64 attacking_occ = bd[0 + aside] | bd[1 + aside];
    const u64 all_occ = defending_occ | attacking_occ;

    u64 bb = bd[aside];
    while (bb)
    {
        int start = LSB_index(bb);
        bb = pop_bit(bb, start);
        u64 att = capture_table[!(bd.get_side())][start];
        while (att)
        {
            int end = LSB_index(att);
            att = pop_bit(att, end);
            if (((1ULL << end) & ~all_occ) && ((1ULL << ((start+end) / 2)) & defending_occ)) 
                moves.push_back(move_wrapper(start,end));
        }
    }
    bb = bd[aside+1];
    while (bb)
    {
        int start = LSB_index(bb);
        bb = pop_bit(bb, start);
        u64 att = capture_table[2][start];
        while (att)
        {
            int end = LSB_index(att);
            att = pop_bit(att, end);
            if (((1ULL << end) & ~all_occ) && ((1ULL << ((start+end) / 2)) & defending_occ)) 
                moves.push_back(move_wrapper(start,end));
        }
    }

    return std::forward<std::vector<move_wrapper>>(moves);
}

inline std::vector<move_wrapper> get_silents(Board& bd)
{
    std::vector<move_wrapper> moves;
    const int aside = (bd.get_side()) ? 0 : 2;
    const int dside = (bd.get_side()) ? 2 : 0;
    const u64 defending_occ = bd[0 + dside] | bd[1 + dside];
    const u64 attacking_occ = bd[0 + aside] | bd[1 + aside];
    const u64 all_occ = defending_occ | attacking_occ;

    u64 bb = bd[aside];
    while (bb)
    {
        int start = LSB_index(bb);
        bb = pop_bit(bb, start);
        u64 att = silent_table[!(bd.get_side())][start];
        while (att)
        {
            int end = LSB_index(att);
            if (att & ~all_occ) moves.push_back(move_wrapper(start,end));
            att = pop_bit(att, end);
        }
    }
    bb = bd[aside+1];
    while (bb)
    {
        int start = LSB_index(bb);
        bb = pop_bit(bb, start);
        u64 att = silent_table[2][start];
        while (att)
        {
            int end = LSB_index(att);
            if (att & ~all_occ) moves.push_back(move_wrapper(start,end));
            att = pop_bit(att, end);
        }
    }
    return std::forward<std::vector<move_wrapper>>(moves);
}

std::vector<move_wrapper> get_legal_moves(Board& bd)
{
    std::vector<move_wrapper> moves = get_captures(bd);
    if (moves.size() == 0) moves = get_silents(bd);

    return std::forward<std::vector<move_wrapper>>(moves);
}

   
}
}