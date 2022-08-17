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
typedef uint8_t square;
typedef uint8_t move_flags;

const bitboard null_board{ bitboard( 0 ) };
const bitboard one_board{ bitboard( 1 ) };

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
	square from;
	square to;
	piece_type type;
	move_flags flags;
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
	std::vector<std::tuple<bitboard, move>> cache{};

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

public:

	uint8_t get_rank_file( square sq )
	{
		return sq >> 3;
	}

	bitboard get_pawn_board() const
	{
		return to_move == e_white ? boards[e_white_pawn] : boards[e_black_pawn];
	}
	
	bitboard get_enemy_pawn_board() const
	{
		return to_move == e_white ? boards[e_black_pawn] : boards[e_white_pawn];
	}

	bitboard get_knight_board() const
	{
		return to_move == e_white ? boards[e_white_knight] : boards[e_black_knight];
	}
	
	bitboard get_enemy_knight_board() const
	{
		return to_move == e_white ? boards[e_black_knight] : boards[e_white_knight];
	}

	bitboard get_bishop_board() const
	{
		return to_move == e_white ? boards[e_white_bishop] : boards[e_black_bishop];
	}
	
	bitboard get_enemy_bishop_board() const
	{
		return to_move == e_white ? boards[e_black_bishop] : boards[e_white_bishop];
	}

	bitboard get_rook_board() const
	{
		return to_move == e_white ? boards[e_white_rook] : boards[e_black_rook];
	}
	
	bitboard get_enemy_rook_board() const
	{
		return to_move == e_white ? boards[e_black_rook] : boards[e_white_rook];
	}

	bitboard get_queen_board() const
	{
		return to_move == e_white ? boards[e_white_queen] : boards[e_black_queen];
	}
	
	bitboard get_enemy_queen_board() const
	{
		return to_move == e_white ? boards[e_black_queen] : boards[e_white_queen];
	}

	bitboard get_king_board() const
	{
		return to_move == e_white ? boards[e_white_king] : boards[e_black_king];
	}
	
	bitboard get_enemy_king_board() const
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

	bitboard get_all_pieces()
	{
		return get_all_friendly_pieces() | get_all_enemy_pieces();
	}

	bitboard* get_all_boards()
	{
		return boards;
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
			m.from = _bitscanf( pos );
			m.to = _bitscanf( board );
			m.type = e_king;
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
			m.from = _bitscanf( pos );
			m.to = _bitscanf( board );
			m.type = e_king;
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
			m.from = _bitscanf( pos );
			m.to = _bitscanf( board );
			m.type = e_pawn;
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
			m.from = _bitscanf( pos );
			m.to = _bitscanf( board );
			m.type = e_pawn;
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
			m.from = _bitscanf( pos );
			m.to = _bitscanf( board );
			m.type = e_knight;
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

	bitboard _rays[8][64];

	enum cardinal_directions
	{
		north,
		south,
		east,
		west,
		north_east,
		north_west,
		south_east,
		south_west
	};

	inline bitboard _eastN( bitboard board, int n ) 
	{
		bitboard new_board{ board };

		for ( int i = 0; i < n; i++ ) 
		{
			new_board = ( ( new_board << 1 ) & ( ~e_file_A ) );
		}

		return new_board;
	}

	inline bitboard _westN( bitboard board, int n ) 
	{
		bitboard new_board{ board };

		for ( int i = 0; i < n; i++ ) 
		{
			new_board = ( ( new_board >> 1 ) & ( ~e_file_H ) );
		}

		return new_board;
	}

	inline int _row( int square ) 
	{
		return square / 8;
	}

	inline int _col( int square ) 
	{
		return square % 8;
	}

	uint8_t _bitscanf( uint64_t bits )
	{
		return BitOperations::BitScan( bits );
	}

	void magic_init()
	{
		magic.GenerateForRook();
		magic.GenerateForBishop();
	}

	void init() 
	{
		for ( int square = 0; square < 64; square++ ) 
		{
			// North
			_rays[north][square] = 0x0101010101010100ULL << square;

			// South
			_rays[south][square] = 0x0080808080808080ULL >> ( 63 - square );

			// East
			_rays[east][square] = 2 * ( ( one_board << ( square | 7 ) ) - ( one_board << square ) );

			// West
			_rays[west][square] = ( one_board << square ) - ( one_board << ( square & 56 ) );

			// North West
			_rays[north_west][square] = _westN( 0x102040810204000ULL, 7 - _col( square ) ) << ( _row( square ) * 8 );

			// North East
			_rays[north_east][square] = _eastN( 0x8040201008040200ULL, _col( square ) ) << ( _row( square ) * 8 );

			// South West
			_rays[south_west][square] = _westN( 0x40201008040201ULL, 7 - _col( square ) ) >> ( ( 7 - _row( square ) ) * 8 );

			// South East
			_rays[south_east][square] = _eastN( 0x2040810204080ULL, _col( square ) ) >> ( ( 7 - _row( square ) ) * 8 );
		}
	}

	bitboard get_ray( cardinal_directions ray_direction, int square ) 
	{
		return _rays[ray_direction][square];
	}

	bitboard get_bishop_attacks( int square, bitboard blockers ) 
	{
		bitboard attacks{ null_board };

		// North West
		attacks |= get_ray( north_west, square );

		if ( get_ray( north_west, square ) & blockers ) 
		{
			attacks &= ~( get_ray( north_west, ( _bitscanf( get_ray( north_west, square ) & blockers ) ) ) );
		}

		// North East
		attacks |= get_ray( north_east, square );

		if ( get_ray( north_east, square ) & blockers ) 
		{
			attacks &= ~( get_ray( north_east, ( _bitscanf( get_ray( north_east, square ) & blockers ) ) ) );
		}

		// South East
		attacks |= get_ray( south_east, square );

		if ( get_ray( south_east, square ) & blockers ) 
		{
			attacks &= ~( get_ray( south_east, ( _bitscanf( get_ray( south_east, square ) & blockers ) ) ) );
		}

		// South West
		attacks |= get_ray( south_west, square );

		if ( get_ray( south_west, square ) & blockers ) 
		{
			attacks &= ~( get_ray( south_west, ( _bitscanf( get_ray( south_west, square ) & blockers ) ) ) );
		}

		return attacks;
	}

	bitboard get_rook_attacks( int square, bitboard blockers ) 
	{
		bitboard attacks{ null_board };

		// North
		attacks |= get_ray( north, square );

		if ( get_ray( north, square ) & blockers ) 
		{
			attacks &= ~( get_ray( north, _bitscanf( get_ray( north, square ) & blockers ) ) );
		}

		// South
		attacks |= get_ray( south, square );

		if ( get_ray( south, square ) & blockers )
		{
			attacks &= ~( get_ray( south, _bitscanf( get_ray( south, square ) & blockers ) ) );
		}

		// East
		attacks |= get_ray( east, square );

		if ( get_ray( east, square ) & blockers )
		{
			attacks &= ~( get_ray( east, _bitscanf( get_ray( east, square ) & blockers ) ) );
		}

		// West
		attacks |= get_ray( west, square );

		if ( get_ray( west, square ) & blockers )
		{
			attacks &= ~( get_ray( west, _bitscanf( get_ray( west, square ) & blockers ) ) );
		}

		return attacks;
	}

	std::vector<move> get_rook_moves_magic( uint8_t index, bitboard occ )
	{
		std::vector<move> moves{};
		bitboard result = magic.GetRookAttacks( index, occ ) & ~get_all_friendly_pieces();

		while ( result )
		{
			move m{};
			m.from = index;
			m.to = _bitscanf( result );
			m.type = e_rook;
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
			m.from = index;
			m.to = _bitscanf( result );
			m.type = e_bishop;
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
			m.from = index;
			m.to = _bitscanf( result );
			m.type = e_queen;
			moves.push_back( m );
			result = BitOperations::PopLSB( result );
		}

		result = magic.GetRookAttacks( index, occ ) & ~get_all_friendly_pieces();

		while ( result )
		{
			move m{};
			m.from = index;
			m.to = _bitscanf( result );
			m.type = e_queen;
			moves.push_back( m );
			result = BitOperations::PopLSB( result );
		}

		return moves;
	}

	bool is_king_in_check( bool enemy = false )
	{
		auto moves{ get_all_moves( !enemy ) };

		for ( auto m : moves )
		{
			if ( m.to == _bitscanf( enemy ? get_enemy_king_board() : get_king_board() ) )
			{
				return true;
			}
		}

		return false;
	}

	bitboard get_pinned_board( bool enemy = false )
	{
		bitboard pinned{};
		auto moves{ get_all_moves( enemy ) };

		for ( auto m : moves )
		{
			make_move( m );

			if ( is_king_in_check( true ) )
			{
				pinned |= ( 1ull << m.from );
			}

			unmake_move();
		}

		return pinned;
	}

	std::vector<move> get_all_moves( bool enemy = false )
	{
		magic_init();
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

	void make_move( move _move )
	{
		bitboard* board{};

		switch ( _move.type )
		{
			case e_pawn:
				board = to_move == e_white ? &boards[e_white_pawn] : &boards[e_black_pawn];
				cache.push_back( std::tuple<bitboard, move>( *board, _move  ) );
				*board &= ~_move.from;
				*board |= _move.to;
				break;

			case e_knight:
				board = to_move == e_white ? &boards[e_white_knight] : &boards[e_black_knight];
				cache.push_back( std::tuple<bitboard, move>( *board, _move ) );
				*board &= ~_move.from;
				*board |= _move.to;
				break;
				
			case e_bishop:
				board = to_move == e_white ? &boards[e_white_bishop] : &boards[e_black_bishop];
				cache.push_back( std::tuple<bitboard, move>( *board, _move ) );
				*board &= ~_move.from;
				*board |= _move.to;
				break;

			case e_rook:
				board = to_move == e_white ? &boards[e_white_rook] : &boards[e_black_rook];
				cache.push_back( std::tuple<bitboard, move>( *board, _move ) );
				*board &= ~_move.from;
				*board |= _move.to;
				break;

			case e_queen:
				board = to_move == e_white ? &boards[e_white_queen] : &boards[e_black_queen];
				cache.push_back( std::tuple<bitboard, move>( *board, _move ) );
				*board &= ~_move.from;
				*board |= _move.to;
				break;

			case e_king:
				board = to_move == e_white ? &boards[e_white_king] : &boards[e_black_king];
				cache.push_back( std::tuple<bitboard, move>( *board, _move ) );
				*board &= ~_move.from;
				*board |= _move.to;
				break;
		}

		to_move = to_move == e_white ? e_black : e_white;
	}

	void unmake_move()
	{
		bitboard* board{};

		switch ( std::get<1>( cache.back() ).type )
		{
		case e_pawn:
			board = to_move == e_white ? &boards[e_white_pawn] : &boards[e_black_pawn];
			*board = std::get<0>( cache.back() );
			break;

		case e_knight:
			board = to_move == e_white ? &boards[e_white_knight] : &boards[e_black_knight];
			*board = std::get<0>( cache.back() );
			break;

		case e_bishop:
			board = to_move == e_white ? &boards[e_white_bishop] : &boards[e_black_bishop];
			*board = std::get<0>( cache.back() );
			break;

		case e_rook:
			board = to_move == e_white ? &boards[e_white_rook] : &boards[e_black_rook];
			*board = std::get<0>( cache.back() );
			break;

		case e_queen:
			board = to_move == e_white ? &boards[e_white_queen] : &boards[e_black_queen];
			*board = std::get<0>( cache.back() );
			break;

		case e_king:
			board = to_move == e_white ? &boards[e_white_king] : &boards[e_black_king];
			*board = std::get<0>( cache.back() );
			break;
		}

		to_move = to_move == e_white ? e_black : e_white;
		cache.pop_back();
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
	auto moves{ p.get_all_moves() }; //p.get_pawn_moves( p.get_pawn_board() ) };

	for ( auto b : moves )
	{
		std::cout << type_to_string( b.type ) << ": " << square_id[b.from] << "->" << square_id[b.to] << std::endl;
		//print_board( squares[b.from] );
	}

	getchar();
}