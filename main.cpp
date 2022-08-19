#include <iostream>
#include <vector>
#include <string>
#include <bitset>
#include <iostream>
#include <chrono>
#include <string>
#include <unordered_map>
#include <FastMagicBitboards.hpp>

typedef uint64_t bitboard;
typedef uint64_t square;

enum board_type
{
	e_white_pawn,
	e_black_pawn,
	e_white_knight,
	e_black_knight,
	e_white_bishop,
	e_black_bishop,
	e_white_rook,
	e_black_rook,
	e_white_queen,
	e_black_queen,
	e_white_king,
	e_black_king
};

enum piece_type
{
	e_pawn,
	e_knight,
	e_bishop,
	e_rook,
	e_queen,
	e_king
};

enum piece_score
{
	e_pawn_score = 1,
	e_knight_score = 3,
	e_bishop_score = 3,
	e_rook_score = 5,
	e_queen_score = 9,
	e_king_score = 100
};

enum color
{
	e_white,
	e_black
};

enum
{
	e_rank_8,
	e_rank_7,
	e_rank_6,
	e_rank_5,
	e_rank_4,
	e_rank_3,
	e_rank_2,
	e_rank_1
};

enum
{
	e_file_H,
	e_file_G,
	e_file_F,
	e_file_E,
	e_file_D,
	e_file_C,
	e_file_B,
	e_file_A
};

enum
{
	A1, B1, C1, D1, E1, F1, G1, H1,
	A2, B2, C2, D2, E2, F2, G2, H2,
	A3, B3, C3, D3, E3, F3, G3, H3,
	A4, B4, C4, D4, E4, F4, G4, H4,
	A5, B5, C5, D5, E5, F5, G5, H5,
	A6, B6, C6, D6, E6, F6, G6, H6,
	A7, B7, C7, D7, E7, F7, G7, H7,
	A8, B8, C8, D8, E8, F8, G8, H8
};

std::string square_id[]
{
	"A1", "B1", "C1", "D1", "E1", "F1", "G1", "H1",
	"A2", "B2", "C2", "D2", "E2", "F2", "G2", "H2",
	"A3", "B3", "C3", "D3", "E3", "F3", "G3", "H3",
	"A4", "B4", "C4", "D4", "E4", "F4", "G4", "H4",
	"A5", "B5", "C5", "D5", "E5", "F5", "G5", "H5",
	"A6", "B6", "C6", "D6", "E6", "F6", "G6", "H6",
	"A7", "B7", "C7", "D7", "E7", "F7", "G7", "H7",
	"A8", "B8", "C8", "D8", "E8", "F8", "G8", "H8"
};

struct move
{
	square from{};
	square to{};
	piece_type type{};
	bool capture{};
	piece_type captured_type{};
};

bitboard h_file_board
{
	0b1000000010000000100000001000000010000000100000001000000010000000
};

bitboard a_file_board
{
	0b0000000100000001000000010000000100000001000000010000000100000001
};

bitboard white_board
{
	0b0000000000000000000000000000000000000000000000001111111111111111
};

bitboard black_board
{
	0b1111111111111111000000000000000000000000000000000000000000000000
};

uint64_t mask_rank[8]
{
	0b1111111100000000000000000000000000000000000000000000000000000000,
	0b0000000011111111000000000000000000000000000000000000000000000000,
	0b0000000000000000111111110000000000000000000000000000000000000000,
	0b0000000000000000000000001111111100000000000000000000000000000000,
	0b0000000000000000000000000000000011111111000000000000000000000000,
	0b0000000000000000000000000000000000000000111111110000000000000000,
	0b0000000000000000000000000000000000000000000000001111111100000000,
	0b0000000000000000000000000000000000000000000000000000000011111111
};

uint64_t clear_rank[8]
{
	0b0000000011111111111111111111111111111111111111111111111111111111,
	0b1111111100000000111111111111111111111111111111111111111111111111,
	0b1111111111111111000000001111111111111111111111111111111111111111,
	0b1111111111111111111111110000000011111111111111111111111111111111,
	0b1111111111111111111111111111111100000000111111111111111111111111,
	0b1111111111111111111111111111111111111111000000001111111111111111,
	0b1111111111111111111111111111111111111111111111110000000011111111,
	0b1111111111111111111111111111111111111111111111111111111100000000
};

uint64_t mask_file[8]
{
	0b1000000010000000100000001000000010000000100000001000000010000000,
	0b0100000001000000010000000100000001000000010000000100000001000000,
	0b0010000000100000001000000010000000100000001000000010000000100000,
	0b0001000000010000000100000001000000010000000100000001000000010000,
	0b0000100000001000000010000000100000001000000010000000100000001000,
	0b0000010000000100000001000000010000000100000001000000010000000100,
	0b0000001000000010000000100000001000000010000000100000001000000010,
	0b0000000100000001000000010000000100000001000000010000000100000001
};

uint64_t clear_file[8]
{
	0b0111111101111111011111110111111101111111011111110111111101111111,
	0b1011111110111111101111111011111110111111101111111011111110111111,
	0b1101111111011111110111111101111111011111110111111101111111011111,
	0b1110111111101111111011111110111111101111111011111110111111101111,
	0b1111011111110111111101111111011111110111111101111111011111110111,
	0b1111101111111011111110111111101111111011111110111111101111111011,
	0b1111110111111101111111011111110111111101111111011111110111111101,
	0b1111111011111110111111101111111011111110111111101111111011111110
};

void print_board( bitboard board = black_board | white_board )
{
	for ( int64_t i = 63; i >= 0; i-- )
	{
		std::cout << ( ( board >> i ) & 1 ? 'P' : '.' );

		if ( i % 8 == 0 )
		{
			std::cout << std::endl;
		}
	}

	std::cout << std::endl;
}

struct position
{
private:

	color to_move{ e_white };
	std::vector<std::tuple<std::vector<bitboard>, move>> cache{};
	bool magic_initialized{};

	bitboard boards[12]
	{
		0b0000000000000000000000000000000000000000000000001111111100000000, // white pawns		- 0
		0b0000000011111111000000000000000000000000000000000000000000000000, // black pawns		- 1
		0b0000000000000000000000000000000000000000000000000000000001000010, // white knights	- 2
		0b0100001000000000000000000000000000000000000000000000000000000000,	// black knights	- 3
		0b0000000000000000000000000000000000000000000000000000000000100100, // white bishops	- 4
		0b0010010000000000000000000000000000000000000000000000000000000000, // black bishops	- 5
		0b0000000000000000000000000000000000000000000000000000000010000001, // white rooks		- 6
		0b1000000100000000000000000000000000000000000000000000000000000000,	// black rooks		- 7
		0b0000000000000000000000000000000000000000000000000000000000001000, // white queens		- 8
		0b0000100000000000000000000000000000000000000000000000000000000000, // black queens		- 9
		0b0000000000000000000000000000000000000000000000000000000000010000,	// white king		- 10
		0b0001000000000000000000000000000000000000000000000000000000000000	// black king		- 11
	};

	FastMagicBitboards magic;
	move alpha_move{};

public:

	bitboard& get_pawn_board()
	{
		return to_move == e_white ? boards[e_white_pawn] : boards[e_black_pawn];
	}
	
	bitboard& get_enemy_pawn_board()
	{
		return to_move == e_white ? boards[e_black_pawn] : boards[e_white_pawn];
	}

	bitboard& get_knight_board()
	{
		return to_move == e_white ? boards[e_white_knight] : boards[e_black_knight];
	}
	
	bitboard& get_enemy_knight_board()
	{
		return to_move == e_white ? boards[e_black_knight] : boards[e_white_knight];
	}

	bitboard& get_bishop_board()
	{
		return to_move == e_white ? boards[e_white_bishop] : boards[e_black_bishop];
	}
	
	bitboard& get_enemy_bishop_board()
	{
		return to_move == e_white ? boards[e_black_bishop] : boards[e_white_bishop];
	}

	bitboard& get_rook_board()
	{
		return to_move == e_white ? boards[e_white_rook] : boards[e_black_rook];
	}
	
	bitboard& get_enemy_rook_board()
	{
		return to_move == e_white ? boards[e_black_rook] : boards[e_white_rook];
	}

	bitboard& get_queen_board()
	{
		return to_move == e_white ? boards[e_white_queen] : boards[e_black_queen];
	}
	
	bitboard& get_enemy_queen_board()
	{
		return to_move == e_white ? boards[e_black_queen] : boards[e_white_queen];
	}

	bitboard& get_king_board()
	{
		return to_move == e_white ? boards[e_white_king] : boards[e_black_king];
	}
	
	bitboard& get_enemy_king_board()
	{
		return to_move == e_white ? boards[e_black_king] : boards[e_white_king];
	}

	bitboard get_all_friendly_pieces() const
	{
		return ( boards[ ( to_move == e_white ? e_white_pawn : e_black_pawn ) ] 
			| boards[ ( to_move == e_white ? e_white_knight : e_black_knight ) ] 
			| boards[ ( to_move == e_white ? e_white_bishop : e_black_bishop ) ] 
			| boards[ ( to_move == e_white ? e_white_rook : e_black_rook ) ] 
			| boards[ ( to_move == e_white ? e_white_queen : e_black_queen ) ] 
			| boards[ ( to_move == e_white ? e_white_king : e_black_king ) ] 
		);
	}

	bitboard get_all_enemy_pieces() const
	{
		return ( boards[ ( to_move == e_black ? e_white_pawn : e_black_pawn ) ] 
			| boards[ ( to_move == e_black ? e_white_knight : e_black_knight ) ] 
			| boards[ ( to_move == e_black ? e_white_bishop : e_black_bishop ) ] 
			| boards[ ( to_move == e_black ? e_white_rook : e_black_rook ) ] 
			| boards[ ( to_move == e_black ? e_white_queen : e_black_queen ) ] 
			| boards[ ( to_move == e_black ? e_white_king : e_black_king ) ] 
		);
	}

	bitboard get_all_pieces() const
	{
		return get_all_friendly_pieces() | get_all_enemy_pieces();
	}

	bitboard get_pawn_attacks( bitboard pos, bool enemy = false )
	{
		if ( enemy == true )
		{
			to_move = to_move == e_white ? e_black : e_white;
		}

		if ( pos == 0 )
		{
			pos = get_pawn_board();
		}

		bitboard attacks{};
		std::vector<bitboard> board_moves{};
		bitboard friendly_pieces{ get_all_friendly_pieces() };
		bitboard enemy_pieces{ get_all_enemy_pieces() };

		while ( pos )
		{
			if ( to_move == e_white )
			{
				board_moves.push_back( pos << 8 & ~friendly_pieces & ~enemy_pieces );
				board_moves.push_back( board_moves.back() << 8 & ~friendly_pieces & ~enemy_pieces );
				board_moves.push_back( ( pos & clear_file[e_file_H] ) << 9 & enemy_pieces );
				board_moves.push_back( ( pos & clear_file[e_file_A] ) << 7 & enemy_pieces );
			}
			else
			{
				board_moves.push_back( pos >> 8 & ~friendly_pieces & ~enemy_pieces );
				board_moves.push_back( board_moves.back() >> 8 & ~friendly_pieces & ~enemy_pieces );
				board_moves.push_back( ( pos & clear_file[e_file_H] ) >> 9 & enemy_pieces );
				board_moves.push_back( ( pos & clear_file[e_file_A] ) >> 7 & enemy_pieces );
			}

			pos = BitOperations::PopLSB( pos );
		}

		for ( size_t i = 0; i < board_moves.size(); i++ )
		{
			if ( board_moves[i] == 0 )
			{
				board_moves.erase( board_moves.begin() + i-- );
			}
			else
			{
				attacks |= board_moves[i];
			}
		}

		if ( enemy == true )
		{
			to_move = to_move == e_white ? e_black : e_white;
		}

		return attacks;
	}

	bitboard get_knight_attacks( bitboard pos = 0, bool enemy = false )
	{
		if ( enemy == true )
		{
			to_move = to_move == e_white ? e_black : e_white;
		}

		if ( pos == 0 )
		{
			pos = get_knight_board();
		}

		bitboard attacks{};
		std::vector<bitboard> board_moves{};
		bitboard friendly_pieces{ get_all_friendly_pieces() };


		while ( pos )
		{
			board_moves.push_back( ( pos & clear_file[e_file_H] ) >> 17 & ~friendly_pieces & clear_file[e_file_H] );
			board_moves.push_back( ( pos & clear_file[e_file_H] & clear_file[e_file_G] ) >> 10 & ~friendly_pieces & clear_file[e_file_H] );
			board_moves.push_back( ( pos & clear_file[e_file_H] ) << 15 & ~friendly_pieces & clear_file[e_file_H] );
			board_moves.push_back( ( pos & clear_file[e_file_H] & clear_file[e_file_G] ) << 6 & ~friendly_pieces & clear_file[e_file_H] );
			board_moves.push_back( ( pos & clear_file[e_file_A] ) >> 15 & ~friendly_pieces & clear_file[e_file_A] );
			board_moves.push_back( ( pos & clear_file[e_file_A] & clear_file[e_file_B] ) >> 6 & ~friendly_pieces & clear_file[e_file_A] );
			board_moves.push_back( ( pos & clear_file[e_file_A] ) << 17 & ~friendly_pieces & clear_file[e_file_A] );
			board_moves.push_back( ( pos & clear_file[e_file_A] & clear_file[e_file_B] ) << 10 & ~friendly_pieces & clear_file[e_file_A] );

			pos = BitOperations::PopLSB( pos );
		}

		for ( size_t i = 0; i < board_moves.size(); i++ )
		{
			if ( board_moves[i] == 0 )
			{
				board_moves.erase( board_moves.begin() + i-- );
			}
			else
			{
				attacks |= board_moves[i];
			}
		}

		if ( enemy == true )
		{
			to_move = to_move == e_white ? e_black : e_white;
		}

		return attacks;
	}

	bitboard get_bishop_attacks( bitboard pos, bitboard occ, bool enemy = false )
	{
		if ( enemy == true )
		{
			to_move = to_move == e_white ? e_black : e_white;
		}

		if ( pos == 0 )
		{
			pos = get_bishop_board();
		}

		bitboard attacks{};

		while ( pos )
		{
			attacks |= ( magic.GetBishopAttacks( _bitscanf( pos ), occ ) & ~get_all_friendly_pieces() );
			pos = BitOperations::PopLSB( pos );
		}

		if ( enemy == true )
		{
			to_move = to_move == e_white ? e_black : e_white;
		}

		return attacks;
	}

	bitboard get_rook_attacks( bitboard pos, bitboard occ, bool enemy = false )
	{
		if ( enemy == true )
		{
			to_move = to_move == e_white ? e_black : e_white;
		}

		if ( pos == 0 )
		{
			pos = get_rook_board();
		}

		bitboard attacks{};

		while ( pos )
		{
			attacks |= ( magic.GetRookAttacks( _bitscanf( pos ), occ ) & ~get_all_friendly_pieces() );
			pos = BitOperations::PopLSB( pos );
		}

		if ( enemy == true )
		{
			to_move = to_move == e_white ? e_black : e_white;
		}

		return attacks;
	}

	bitboard get_queen_attacks( bitboard pos, bitboard occ, bool enemy = false )
	{
		if ( enemy == true )
		{
			to_move = to_move == e_white ? e_black : e_white;
		}

		if ( pos == 0 )
		{
			pos = get_queen_board();
		}

		bitboard attacks{};

		while ( pos )
		{
			attacks |= ( magic.GetBishopAttacks( _bitscanf( pos ), get_all_pieces() ) & ~get_all_friendly_pieces() );
			attacks |= ( magic.GetRookAttacks( _bitscanf( pos ), get_all_pieces() ) & ~get_all_friendly_pieces() );
			pos = BitOperations::PopLSB( pos );
		}

		if ( enemy == true )
		{
			to_move = to_move == e_white ? e_black : e_white;
		}

		return attacks;
	}

	bitboard get_king_attacks( bitboard pos = 0, bool enemy = false )
	{
		if ( enemy == true )
		{
			to_move = to_move == e_white ? e_black : e_white;
		}

		if ( pos == 0 )
		{
			pos = get_king_board();
		}

		bitboard attacks{};
		std::vector<bitboard> board_moves{};
		bitboard friendly_pieces{ get_all_friendly_pieces() };
		bitboard enemy_pieces{ get_all_enemy_pieces() };

		while ( pos )
		{
			if ( to_move == e_white )
			{
				board_moves.push_back( pos >> 8 & ~friendly_pieces );
				board_moves.push_back( pos << 8 & ~friendly_pieces );
				board_moves.push_back( ( pos & clear_file[e_file_H] ) >> 1 & ~friendly_pieces );
				board_moves.push_back( ( pos & clear_file[e_file_A] ) << 1 & ~friendly_pieces );
				board_moves.push_back( ( pos & clear_file[e_file_H] ) >> 9 & ~friendly_pieces );
				board_moves.push_back( ( pos & clear_file[e_file_A] ) >> 7 & ~friendly_pieces );
				board_moves.push_back( ( pos & clear_file[e_file_H] ) << 9 & ~friendly_pieces );
				board_moves.push_back( ( pos & clear_file[e_file_A] ) << 7 & ~friendly_pieces );
			}
			else
			{
				board_moves.push_back( pos << 8 & ~friendly_pieces );
				board_moves.push_back( pos >> 8 & ~friendly_pieces );
				board_moves.push_back( ( pos & clear_file[e_file_A] ) << 1 & ~friendly_pieces );
				board_moves.push_back( ( pos & clear_file[e_file_H] ) >> 1 & ~friendly_pieces );
				board_moves.push_back( ( pos & clear_file[e_file_A] ) << 9 & ~friendly_pieces );
				board_moves.push_back( ( pos & clear_file[e_file_H] ) << 7 & ~friendly_pieces );
				board_moves.push_back( ( pos & clear_file[e_file_A] ) >> 9 & ~friendly_pieces );
				board_moves.push_back( ( pos & clear_file[e_file_H] ) >> 7 & ~friendly_pieces );
			}

			pos = BitOperations::PopLSB( pos );
		}

		for ( size_t i = 0; i < board_moves.size(); i++ )
		{
			if ( board_moves[i] == 0 )
			{
				board_moves.erase( board_moves.begin() + i-- );
			}
			else
			{
				attacks |= board_moves[i];
			}
		}

		if ( enemy == true )
		{
			to_move = to_move == e_white ? e_black : e_white;
		}

		return attacks;
	}

	bitboard get_attacks( bool enemy = false )
	{
		bitboard attacks{ get_pawn_attacks( 0, enemy ) };
		attacks |= get_knight_attacks( 0, enemy );
		attacks |= get_bishop_attacks( 0, get_all_pieces(), enemy );
		attacks |= get_rook_attacks( 0, get_all_pieces(), enemy );
		attacks |= get_queen_attacks( 0, get_all_pieces(), enemy );
		attacks |= get_king_attacks( 0, enemy );

		return attacks;
	}

	std::vector<move> get_white_king_moves( bitboard pos )
	{
		std::vector<bitboard> board_moves{};
		std::vector<move> moves{};
		bitboard friendly_pieces{ get_all_friendly_pieces() };

		board_moves.push_back( pos >> 8 & ~friendly_pieces );
		board_moves.push_back( pos << 8 & ~friendly_pieces );
		board_moves.push_back( ( pos & clear_file[e_file_H] ) >> 1 & ~friendly_pieces );
		board_moves.push_back( ( pos & clear_file[e_file_A] ) << 1 & ~friendly_pieces );
		board_moves.push_back( ( pos & clear_file[e_file_H] ) >> 9 & ~friendly_pieces );
		board_moves.push_back( ( pos & clear_file[e_file_A] ) >> 7 & ~friendly_pieces );
		board_moves.push_back( ( pos & clear_file[e_file_H] ) << 9 & ~friendly_pieces );
		board_moves.push_back( ( pos & clear_file[e_file_A] ) << 7 & ~friendly_pieces );

		for ( size_t i = 0; i < board_moves.size(); i++ )
		{
			if ( board_moves[i] == 0 )
			{
				board_moves.erase( board_moves.begin() + i-- );
			}
		}

		for ( bitboard board : board_moves )
		{
			move m{};
			m.from |= ( 1ull << _bitscanf( pos ) );
			m.to |= ( 1ull << _bitscanf( board ) );
			m.type = e_king;

			if ( m.to & get_all_enemy_pieces() )
			{
				m.capture = true;

				if ( m.to & get_enemy_pawn_board() )
				{
					m.captured_type = e_pawn;
				}
				else if ( m.to & get_enemy_knight_board() )
				{
					m.captured_type = e_knight;
				}
				else if ( m.to & get_enemy_bishop_board() )
				{
					m.captured_type = e_bishop;
				}
				else if ( m.to & get_enemy_rook_board() )
				{
					m.captured_type = e_rook;
				}
				else if ( m.to & get_enemy_queen_board() )
				{
					m.captured_type = e_queen;
				}
				else if ( m.to & get_enemy_king_board() )
				{
					m.capture = false;
				}
			}

			moves.push_back( m );
		}

		return moves;
	}

	std::vector<move> get_black_king_moves( bitboard pos )
	{
		std::vector<bitboard> board_moves{};
		std::vector<move> moves{};
		bitboard friendly_pieces{ get_all_friendly_pieces() };

		board_moves.push_back( pos << 8 & ~friendly_pieces );
		board_moves.push_back( pos >> 8 & ~friendly_pieces );
		board_moves.push_back( ( pos & clear_file[e_file_A] ) << 1 & ~friendly_pieces );
		board_moves.push_back( ( pos & clear_file[e_file_H] ) >> 1 & ~friendly_pieces );
		board_moves.push_back( ( pos & clear_file[e_file_A] ) << 9 & ~friendly_pieces );
		board_moves.push_back( ( pos & clear_file[e_file_H] ) << 7 & ~friendly_pieces );
		board_moves.push_back( ( pos & clear_file[e_file_A] ) >> 9 & ~friendly_pieces );
		board_moves.push_back( ( pos & clear_file[e_file_H] ) >> 7 & ~friendly_pieces );

		for ( size_t i = 0; i < board_moves.size(); i++ )
		{
			if ( board_moves[i] == 0 )
			{
				board_moves.erase( board_moves.begin() + i-- );
			}
		}

		for ( bitboard board : board_moves )
		{
			move m{};
			m.from |= ( 1ull << _bitscanf( pos ) );
			m.to |= ( 1ull << _bitscanf( board ) );
			m.type = e_king;

			if ( m.to & get_all_enemy_pieces() )
			{
				m.capture = true;

				if ( m.to & get_enemy_pawn_board() )
				{
					m.captured_type = e_pawn;
				}
				else if ( m.to & get_enemy_knight_board() )
				{
					m.captured_type = e_knight;
				}
				else if ( m.to & get_enemy_bishop_board() )
				{
					m.captured_type = e_bishop;
				}
				else if ( m.to & get_enemy_rook_board() )
				{
					m.captured_type = e_rook;
				}
				else if ( m.to & get_enemy_queen_board() )
				{
					m.captured_type = e_queen;
				}
				else if ( m.to & get_enemy_king_board() )
				{
					m.capture = false;
				}
			}

			moves.push_back( m );
		}

		return moves;
	}

	std::vector<move> get_white_pawn_moves( bitboard pos )
	{
		std::vector<bitboard> board_moves{};
		std::vector<move> moves{};
		bitboard friendly_pieces{ get_all_friendly_pieces() };
		bitboard enemy_pieces{ get_all_enemy_pieces() };

		board_moves.push_back( pos << 8 & ~friendly_pieces & ~enemy_pieces );
		board_moves.push_back( board_moves.back() << 8 & ~friendly_pieces & ~enemy_pieces );
		board_moves.push_back( ( pos & clear_file[e_file_H] ) << 9 & enemy_pieces );
		board_moves.push_back( ( pos & clear_file[e_file_A] ) << 7 & enemy_pieces );

		for ( size_t i = 0; i < board_moves.size(); i++ )
		{
			if ( board_moves[i] == 0 )
			{
				board_moves.erase( board_moves.begin() + i-- );
			}
		}

		for ( bitboard board : board_moves )
		{
			move m{};
			m.from |= ( 1ull << _bitscanf( pos ) );
			m.to |= ( 1ull << _bitscanf( board ) );
			m.type = e_pawn;

			if ( m.to & enemy_pieces )
			{
				m.capture = true;

				if ( m.to & get_enemy_pawn_board() )
				{
					m.captured_type = e_pawn;
				}
				else if ( m.to & get_enemy_knight_board() )
				{
					m.captured_type = e_knight;
				}
				else if ( m.to & get_enemy_bishop_board() )
				{
					m.captured_type = e_bishop;
				}
				else if ( m.to & get_enemy_rook_board() )
				{
					m.captured_type = e_rook;
				}
				else if ( m.to & get_enemy_queen_board() )
				{
					m.captured_type = e_queen;
				}
				else if ( m.to & get_enemy_king_board() )
				{
					m.capture = false;
				}
			}

			moves.push_back( m );
		}

		return moves;
	}

	std::vector<move> get_black_pawn_moves( bitboard pos )
	{
		std::vector<bitboard> board_moves{};
		std::vector<move> moves{};
		bitboard friendly_pieces{ get_all_friendly_pieces() };
		bitboard enemy_pieces{ get_all_enemy_pieces() };

		board_moves.push_back( pos >> 8 & ~friendly_pieces & ~enemy_pieces );
		board_moves.push_back( board_moves.back() >> 8 & ~friendly_pieces & ~enemy_pieces );
		board_moves.push_back( ( pos & clear_file[e_file_H] ) >> 9 & enemy_pieces );
		board_moves.push_back( ( pos & clear_file[e_file_A] ) >> 7 & enemy_pieces );

		for ( size_t i = 0; i < board_moves.size(); i++ )
		{
			if ( board_moves[i] == 0 )
			{
				board_moves.erase( board_moves.begin() + i-- );
			}
		}

		for ( bitboard board : board_moves )
		{
			move m{};
			m.from |= ( 1ull << _bitscanf( pos ) );
			m.to |= ( 1ull << _bitscanf( board ) );
			m.type = e_pawn;

			if ( m.to & get_all_enemy_pieces() )
			{
				//std::cout << "piece: " << m.type << std::endl;
				//print_board( m.from );
				//print_board( m.to );
				//print_board( get_all_enemy_pieces() );
				m.capture = true;

				if ( m.to & get_enemy_pawn_board() )
				{
					m.captured_type = e_pawn;
				}
				else if ( m.to & get_enemy_knight_board() )
				{
					m.captured_type = e_knight;
				}
				else if ( m.to & get_enemy_bishop_board() )
				{
					m.captured_type = e_bishop;
				}
				else if ( m.to & get_enemy_rook_board() )
				{
					m.captured_type = e_rook;
				}
				else if ( m.to & get_enemy_queen_board() )
				{
					m.captured_type = e_queen;
				}
				else if ( m.to & get_enemy_king_board() )
				{
					m.capture = false;
				}
			}

			moves.push_back( m );
		}

		return moves;
	}

	std::vector<move> get_knight_moves( bitboard pos )
	{
		std::vector<bitboard> board_moves{};
		std::vector<move> moves{};
		bitboard friendly_pieces{ get_all_friendly_pieces() };

		board_moves.push_back( ( pos & clear_file[e_file_H] ) >> 17 & ~friendly_pieces & clear_file[e_file_H] );
		board_moves.push_back( ( pos & clear_file[e_file_H] & clear_file[e_file_G] ) >> 10 & ~friendly_pieces & clear_file[e_file_H] );
		board_moves.push_back( ( pos & clear_file[e_file_H] ) << 15 & ~friendly_pieces & clear_file[e_file_H] );
		board_moves.push_back( ( pos & clear_file[e_file_H] & clear_file[e_file_G] ) << 6 & ~friendly_pieces & clear_file[e_file_H] );
		board_moves.push_back( ( pos & clear_file[e_file_A] ) >> 15 & ~friendly_pieces & clear_file[e_file_A] );
		board_moves.push_back( ( pos & clear_file[e_file_A] & clear_file[e_file_B] ) >> 6 & ~friendly_pieces & clear_file[e_file_A] );
		board_moves.push_back( ( pos & clear_file[e_file_A] ) << 17 & ~friendly_pieces & clear_file[e_file_A] );
		board_moves.push_back( ( pos & clear_file[e_file_A] & clear_file[e_file_B] ) << 10 & ~friendly_pieces & clear_file[e_file_A] );

		for ( size_t i = 0; i < board_moves.size(); i++ )
		{
			if ( board_moves[i] == 0 )
			{
				board_moves.erase( board_moves.begin() + i-- );
			}
		}

		for ( bitboard board : board_moves )
		{
			move m{};
			m.from |= ( 1ull << _bitscanf( pos ) );
			m.to |= ( 1ull << _bitscanf( board ) );
			m.type = e_knight;

			if ( m.to & get_all_enemy_pieces() )
			{
				m.capture = true;

				if ( m.to & get_enemy_pawn_board() )
				{
					m.captured_type = e_pawn;
				}
				else if ( m.to & get_enemy_knight_board() )
				{
					m.captured_type = e_knight;
				}
				else if ( m.to & get_enemy_bishop_board() )
				{
					m.captured_type = e_bishop;
				}
				else if ( m.to & get_enemy_rook_board() )
				{
					m.captured_type = e_rook;
				}
				else if ( m.to & get_enemy_queen_board() )
				{
					m.captured_type = e_queen;
				}
				else if ( m.to & get_enemy_king_board() )
				{
					m.capture = false;
				}
			}

			moves.push_back( m );
		}

		return moves;
	}

	std::vector<move> get_king_moves( bitboard pos )
	{
		return to_move == e_white ? get_white_king_moves( pos ) : get_black_king_moves( pos );
	}

	std::vector<move> get_pawn_moves( bitboard pos )
	{
		return to_move == e_white ? get_white_pawn_moves( pos ) : get_black_pawn_moves( pos );
	}

	uint8_t _bitscanf( uint64_t bits )
	{
		return BitOperations::BitScan( bits );
	}

	void magic_init()
	{
		magic.GenerateForRook();
		magic.GenerateForBishop();
		magic_initialized = true;
	}

	std::vector<move> get_rook_moves_magic( uint8_t index, bitboard occ )
	{
		std::vector<move> moves{};
		bitboard result = magic.GetRookAttacks( index, occ ) & ~get_all_friendly_pieces();

		while ( result )
		{
			move m{};
			m.from |= ( 1ull << index );
			m.to |= ( 1ull << _bitscanf( result ) );
			m.type = e_rook;

			if ( m.to & get_all_enemy_pieces() )
			{
				m.capture = true;

				if ( m.to & get_enemy_pawn_board() )
				{
					m.captured_type = e_pawn;
				}
				else if ( m.to & get_enemy_knight_board() )
				{
					m.captured_type = e_knight;
				}
				else if ( m.to & get_enemy_bishop_board() )
				{
					m.captured_type = e_bishop;
				}
				else if ( m.to & get_enemy_rook_board() )
				{
					m.captured_type = e_rook;
				}
				else if ( m.to & get_enemy_queen_board() )
				{
					m.captured_type = e_queen;
				}
				else if ( m.to & get_enemy_king_board() )
				{
					m.capture = false;
				}
			}

			moves.push_back( m );
			result = BitOperations::PopLSB( result );
		}

		return moves;
	}

	std::vector<move> get_bishop_moves_magic( uint8_t index, bitboard occ )
	{
		std::vector<move> moves{};
		bitboard result = magic.GetBishopAttacks( index, occ ) & ~get_all_friendly_pieces();
		
		while ( result )
		{
			move m{};
			m.from |= ( 1ull << index );
			m.to |= ( 1ull << _bitscanf( result ) );
			m.type = e_bishop;

			if ( m.to & get_all_enemy_pieces() )
			{
				m.capture = true;

				if ( m.to & get_enemy_pawn_board() )
				{
					m.captured_type = e_pawn;
				}
				else if ( m.to & get_enemy_knight_board() )
				{
					m.captured_type = e_knight;
				}
				else if ( m.to & get_enemy_bishop_board() )
				{
					m.captured_type = e_bishop;
				}
				else if ( m.to & get_enemy_rook_board() )
				{
					m.captured_type = e_rook;
				}
				else if ( m.to & get_enemy_queen_board() )
				{
					m.captured_type = e_queen;
				}
				else if ( m.to & get_enemy_king_board() )
				{
					m.capture = false;
				}
			}

			moves.push_back( m );
			result = BitOperations::PopLSB( result );
		}

		return moves;
	}

	std::vector<move> get_queen_moves_magic( uint8_t index, bitboard occ )
	{
		std::vector<move> moves{};
		bitboard result = magic.GetBishopAttacks( index, occ ) & ~get_all_friendly_pieces();

		while ( result )
		{
			move m{};
			m.from |= ( 1ull << index );
			m.to |= ( 1ull << _bitscanf( result ) );
			m.type = e_queen;

			if ( m.to & get_all_enemy_pieces() )
			{
				m.capture = true;

				if ( m.to & get_enemy_pawn_board() )
				{
					m.captured_type = e_pawn;
				}
				else if ( m.to & get_enemy_knight_board() )
				{
					m.captured_type = e_knight;
				}
				else if ( m.to & get_enemy_bishop_board() )
				{
					m.captured_type = e_bishop;
				}
				else if ( m.to & get_enemy_rook_board() )
				{
					m.captured_type = e_rook;
				}
				else if ( m.to & get_enemy_queen_board() )
				{
					m.captured_type = e_queen;
				}
				else if ( m.to & get_enemy_king_board() )
				{
					m.capture = false;
				}
			}

			moves.push_back( m );
			result = BitOperations::PopLSB( result );
		}

		result = magic.GetRookAttacks( index, occ ) & ~get_all_friendly_pieces();

		while ( result )
		{
			move m{};
			m.from |= ( 1ull << index );
			m.to |= ( 1ull << _bitscanf( result ) );
			m.type = e_queen;
			moves.push_back( m );
			result = BitOperations::PopLSB( result );
		}

		return moves;
	}

	bool is_king_in_check( bitboard pos = 0 )
	{
		if ( pos == 0 )
		{
			pos = get_king_board();
		}

		if ( get_attacks( true ) & pos ) ++checks;
		return get_attacks( true ) & pos;
	}

	bitboard get_pinned_board( bitboard pos, bool enemy = false )
	{
		if ( enemy == true )
		{
			to_move = to_move == e_white ? e_black : e_white;
		}

		bitboard occ{ get_all_pieces() };
		bitboard sliders{ get_rook_board() & get_queen_board() };
		bitboard pinned{}; 
		bitboard possiblePinned{};
		bitboard pinners{};

		if ( sliders ) 
		{
			possiblePinned = get_rook_attacks( pos, occ ) & get_all_friendly_pieces();
			pinners = get_rook_attacks( pos, occ ^ possiblePinned ) & sliders;

			while ( pinners )
			{
				bitboard pinner{ _bitscanf( pinners ) };
				pinned |= get_rook_attacks( pinner, occ ) & possiblePinned;
				pinners &= ~( 1ull << pinner );
			}
		}

		return pinned;
	}

	std::vector<move> get_all_moves( bool enemy = false )
	{
		if ( magic_initialized == false )
		{
			magic_init();
		}

		if ( enemy ) { to_move = to_move == e_white ? e_black : e_white; };

		std::vector<move> moves{};
		bitboard pawn_board{ get_pawn_board() };
		bitboard knight_board{ get_knight_board() };
		bitboard bishop_board{ get_bishop_board() };
		bitboard rook_board{ get_rook_board() };
		bitboard queen_board{ get_queen_board() };
		bitboard king_board{ get_king_board() };

		while ( pawn_board )
		{
			auto v{ get_pawn_moves( pawn_board ) };
			moves.insert( moves.end(), v.begin(), v.end() );
			pawn_board = BitOperations::PopLSB( pawn_board );
		}

		while ( knight_board )
		{
			auto v{ get_knight_moves( knight_board ) };
			moves.insert( moves.end(), v.begin(), v.end() );
			knight_board = BitOperations::PopLSB( knight_board );
		}

		while ( bishop_board )
		{
			auto v{ get_bishop_moves_magic( _bitscanf( bishop_board ), get_all_pieces() ) };
			moves.insert( moves.end(), v.begin(), v.end() );
			bishop_board = BitOperations::PopLSB( bishop_board );
		}

		while ( rook_board )
		{
			auto v{ get_rook_moves_magic( _bitscanf( rook_board ), get_all_pieces() ) };
			moves.insert( moves.end(), v.begin(), v.end() );
			rook_board = BitOperations::PopLSB( rook_board );
		}

		while ( queen_board )
		{
			auto v{ get_queen_moves_magic( _bitscanf( queen_board ), get_all_pieces() ) };
			moves.insert( moves.end(), v.begin(), v.end() );
			queen_board = BitOperations::PopLSB( queen_board );
		}

		while ( king_board )
		{
			auto v{ get_king_moves( king_board ) };
			moves.insert( moves.end(), v.begin(), v.end() );
			king_board = BitOperations::PopLSB( king_board );
		}

		if ( enemy ) { to_move = to_move == e_white ? e_black : e_white; };
		return moves;
	}

	void pinned_test()
	{

	}

	std::vector<move> get_all_legal_moves()
	{
		auto moves{ get_all_moves() };

		for ( int i = 0; i < moves.size(); i++ )
		{
			if ( get_pinned_board( get_king_board() ) & moves[i].from )
			{
				moves.erase( moves.begin() + i-- );
				continue;
			}

			make_move( moves[i] );
			bool check{ is_king_in_check( ) };
			unmake_move();

			if ( check == true )
			{
				moves.erase( moves.begin() + i-- );
			}
		}

		return moves;
	}

	void make_move( move _move )
	{
		std::vector<bitboard> board_copies{};

		for ( int i = 0; i < 12; i++ )
		{
			board_copies.push_back( boards[i] );
		}

		cache.push_back( std::tuple<std::vector<bitboard>, move>( board_copies, _move ) );

		switch ( _move.type )
		{
			case e_pawn:
			{
				bitboard& pawn_board{ get_pawn_board() };
				pawn_board &= ~_move.from;
				pawn_board |= _move.to;
				get_enemy_pawn_board() &= ~pawn_board;
			} break;

			case e_knight:
			{
				bitboard& knight_board{ get_knight_board() };
				knight_board &= ~_move.from;
				knight_board |= _move.to;
				get_enemy_knight_board() &= ~knight_board;
			} break;
				
			case e_bishop:
			{
				bitboard& bishop_board{ get_bishop_board() };
				bishop_board &= ~_move.from;
				bishop_board |= _move.to;
				get_enemy_bishop_board() &= ~bishop_board;
			} break;

			case e_rook:
			{
				bitboard& rook_board{ get_rook_board() };
				rook_board &= ~_move.from;
				rook_board |= _move.to;
				get_enemy_rook_board() &= ~rook_board;
			} break;

			case e_queen:
			{
				bitboard& queen_board{ get_queen_board() };
				queen_board &= ~_move.from;
				queen_board |= _move.to;
				get_enemy_queen_board() &= ~queen_board;
			} break;

			case e_king:
			{
				bitboard& king_board{ get_king_board() };
				king_board &= ~_move.from;
				king_board |= _move.to;
				get_enemy_king_board() &= ~king_board;
			} break;
		}

		to_move = to_move == e_white ? e_black : e_white;
	}

	void unmake_move()
	{
		for ( int i = 0; i < 12; i++ )
		{
			boards[i] = std::get<0>( cache.back() )[i];
		}

		to_move = to_move == e_white ? e_black : e_white;
		cache.pop_back();
	}

	int32_t search( uint16_t depth, int alpha, int beta )
	{
		if ( depth == 0 )
		{
			return alpha;
		}

		auto moves{ get_all_moves() };

		for ( auto m : moves ) 
		{
			make_move( m );

			int32_t score = -search( depth - 1, -beta, -alpha );

			if ( score >= beta )
			{
				return beta;
			}

			if ( score > alpha )
			{
				alpha = score;
				alpha_move = m;
			}

			unmake_move();
		}

		return alpha;
	}
	int checks{};
	int captures{};

	int64_t perft( uint8_t depth )
	{
		if ( depth == 0 )
		{
			return 1;
		}

		int64_t count{ 0 };
		auto moves{ get_all_legal_moves() };

		for( move m : moves )
		{
			make_move( m );
			if ( m.capture ) ++captures;
			count += perft( depth - 1 );
			unmake_move();
		}

		return count;
	}
};

std::string type_to_string( piece_type type )
{
	switch ( type )
	{
	case e_pawn:
		return "Pawn";
		break;
	case e_knight:
		return "Knight";
		break;
	case e_bishop:
		return "Bishop";
		break;
	case e_rook:
		return "Rook";
		break;
	case e_queen:
		return "Queen";
		break;
	case e_king:
		return "King";
		break;
	}

	return "";
}

int main()
{
	position p{};
	std::cout << p.perft( 3 ) << "\nchecks: " << p.checks << "\ncaptures: " << p.captures;

	//auto moves{ p.get_all_moves() }; //p.get_pawn_moves( p.get_pawn_board() ) };

	//for ( auto b : moves )
	//{
		//std::cout << type_to_string( b.type ) << ": " << square_id[b.from] << "->" << square_id[b.to] << std::endl;
		//print_board( squares[b.from] );
	//}

	getchar();
}