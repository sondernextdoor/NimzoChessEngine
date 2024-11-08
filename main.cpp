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

std::string square_id[64]
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
	bool short_castle{};
	bool long_castle{};

	bool operator == ( move m )
	{
		return ( m.from & from && m.to & to && m.type == type && m.capture == capture && m.captured_type == captured_type );
	}
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

uint64_t short_castle_white
{
	0b0000000000000000000000000000000000000000000000000000000001110000
};

uint64_t long_castle_white
{
	0b0000000000000000000000000000000000000000000000000000000000011100
};

uint64_t short_castle_black
{
	0b0000111000000000000000000000000000000000000000000000000000000000
};

uint64_t long_castle_black
{
	0b0011100000000000000000000000000000000000000000000000000000000000
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

void print_board( bitboard board )
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

	bool white_short_castling_rights{ true };
	bool white_long_castling_rights{ true };
	bool black_short_castling_rights{ true };
	bool black_long_castling_rights{ true };

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

	int checks{};
	int captures{};
	std::vector<move> move_list{};

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
		bitboard pieces{ boards[( to_move == e_white ? e_white_pawn : e_black_pawn )] };
		pieces |= boards[( to_move == e_white ? e_white_knight : e_black_knight )];
		pieces |= boards[( to_move == e_white ? e_white_bishop : e_black_bishop )];
		pieces |= boards[( to_move == e_white ? e_white_rook : e_black_rook )];
		pieces |= boards[( to_move == e_white ? e_white_queen : e_black_queen )];
		pieces |= boards[( to_move == e_white ? e_white_king : e_black_king )];

		return pieces;
	}

	bitboard get_all_enemy_pieces() const
	{
		bitboard pieces{ boards[( to_move == e_white ? e_black_pawn : e_white_pawn )] };
		pieces |= boards[( to_move == e_white ? e_black_knight : e_white_knight )];
		pieces |= boards[( to_move == e_white ? e_black_bishop : e_white_bishop )];
		pieces |= boards[( to_move == e_white ? e_black_rook : e_white_rook )];
		pieces |= boards[( to_move == e_white ? e_black_queen : e_white_queen )];
		pieces |= boards[( to_move == e_white ? e_black_king : e_white_king )];

		return pieces;
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

				if ( pos & mask_rank[e_rank_2] )
				{
					board_moves.push_back( board_moves.back() << 8 & ~friendly_pieces & ~enemy_pieces );
				}

				board_moves.push_back( ( pos & clear_file[e_file_H] ) << 9 & enemy_pieces );
				board_moves.push_back( ( pos & clear_file[e_file_A] ) << 7 & enemy_pieces );
			}
			else
			{
				board_moves.push_back( pos >> 8 & ~friendly_pieces & ~enemy_pieces );

				if ( pos & mask_rank[e_rank_7] )
				{
					board_moves.push_back( board_moves.back() >> 8 & ~friendly_pieces & ~enemy_pieces );
				}

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

		bitboard friendly_pieces{ get_all_friendly_pieces() };

		bitboard spot_1_clip = clear_file[e_file_A] & clear_file[e_file_B];
		bitboard spot_2_clip = clear_file[e_file_A];
		bitboard spot_3_clip = clear_file[e_file_H];
		bitboard spot_4_clip = clear_file[e_file_H] & clear_file[e_file_G];

		bitboard spot_5_clip = clear_file[e_file_H] & clear_file[e_file_G];
		bitboard spot_6_clip = clear_file[e_file_H];
		bitboard spot_7_clip = clear_file[e_file_A];
		bitboard spot_8_clip = clear_file[e_file_A] & clear_file[e_file_B];

		bitboard spot_1 = ( pos & spot_1_clip ) << 6;
		bitboard spot_2 = ( pos & spot_2_clip ) << 15;
		bitboard spot_3 = ( pos & spot_3_clip ) << 17;
		bitboard spot_4 = ( pos & spot_4_clip ) << 10;

		bitboard spot_5 = ( pos & spot_5_clip ) >> 6;
		bitboard spot_6 = ( pos & spot_6_clip ) >> 15;
		bitboard spot_7 = ( pos & spot_7_clip ) >> 17;
		bitboard spot_8 = ( pos & spot_8_clip ) >> 10;

		bitboard knight_moves = spot_1 | spot_2 | spot_3 | spot_4 | spot_5 | spot_6 | spot_7 | spot_8;
		knight_moves &= ~friendly_pieces;

		if ( enemy == true )
		{
			to_move = to_move == e_white ? e_black : e_white;
		}

		return knight_moves;
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

		bitboard friendly_pieces{ get_all_friendly_pieces() };

		bitboard king_clip_file_h = pos & clear_file[e_file_H];
		bitboard king_clip_file_a = pos & clear_file[e_file_A];

		bitboard spot_1 = king_clip_file_h << 7;
		bitboard spot_2 = pos << 8;
		bitboard spot_3 = king_clip_file_h << 9;
		bitboard spot_4 = king_clip_file_h << 1;

		bitboard spot_5 = king_clip_file_a >> 7;
		bitboard spot_6 = pos >> 8;
		bitboard spot_7 = king_clip_file_a >> 9;
		bitboard spot_8 = king_clip_file_a >> 1;

		bitboard attacks{ spot_1 | spot_2 | spot_3 | spot_4 | spot_5 | spot_6 | spot_7 | spot_8 };

		if ( enemy == true )
		{
			to_move = to_move == e_white ? e_black : e_white;
		}

		return attacks &= ~friendly_pieces;
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

	bitboard get_sliding_attacks( bitboard pos = 0, bool enemy = false )
	{
		bitboard attacks{ get_bishop_attacks( pos, get_all_pieces(), enemy ) };
		attacks |= get_rook_attacks( pos, get_all_pieces(), enemy );
		attacks |= get_queen_attacks( pos, get_all_pieces(), enemy );

		return attacks;
	}

	bool can_castle_short()
	{
		bool castling_rights{ to_move == e_white ? white_short_castling_rights : black_short_castling_rights };
		bitboard castle_mask{ to_move == e_white ? short_castle_white : short_castle_black };

		if ( castling_rights == false || get_attacks( true ) & castle_mask || ( get_all_pieces() & ~get_king_board() ) & castle_mask )
		{
			return false;
		}

		return true;
	}

	bool can_castle_long()
	{
		bool castling_rights{ to_move == e_white ? white_long_castling_rights : black_long_castling_rights };
		bitboard castle_mask{ to_move == e_white ? long_castle_white : long_castle_black };

		if ( castling_rights == false || get_attacks( true ) & castle_mask || ( get_all_pieces() & ~get_king_board() ) & castle_mask )
		{
			return false;
		}

		return true;
	}

	bitboard king_short_castle_board()
	{
		return 1ull << ( _bitscanf( get_king_board() ) + 2 );
	}

	bitboard king_long_castle_board()
	{
		return 1ull << ( _bitscanf( get_king_board() ) - 2 );
	}

	bitboard rook_short_castle_board()
	{
		return king_short_castle_board() >> 1;
	}

	bitboard rook_long_castle_board()
	{
		return king_short_castle_board() << 1;
	}

	std::vector<move> get_king_moves( bitboard pos )
	{
		std::vector<move> moves{};
		bitboard friendly_pieces{ get_all_friendly_pieces() };

		bitboard king_clip_file_h = pos & clear_file[e_file_H];
		bitboard king_clip_file_a = pos & clear_file[e_file_A];

		bitboard spot_1 = king_clip_file_h << 7;
		bitboard spot_2 = pos << 8;
		bitboard spot_3 = king_clip_file_h << 9;
		bitboard spot_4 = king_clip_file_h << 1;

		bitboard spot_5 = king_clip_file_a >> 7;
		bitboard spot_6 = pos >> 8;
		bitboard spot_7 = king_clip_file_a >> 9;
		bitboard spot_8 = king_clip_file_a >> 1;

		bitboard king_moves{ spot_1 | spot_2 | spot_3 | spot_4 | spot_5 | spot_6 | spot_7 | spot_8 };

		king_moves &= ~friendly_pieces;
		
		if ( can_castle_short() )
		{
			move m{};
			m.from |= ( 1ull << _bitscanf( pos ) );
			m.to = king_short_castle_board();
			m.type = e_king;
			m.short_castle = true;

			moves.push_back( m );
		}

		if ( can_castle_long() )
		{
			move m{};
			m.from |= ( 1ull << _bitscanf( pos ) );
			m.to = king_long_castle_board();
			m.type = e_king;
			m.long_castle = true;

			moves.push_back( m );
		}

		while ( king_moves )
		{
			move m{};
			m.from |= ( 1ull << _bitscanf( pos ) );
			m.to |= ( 1ull << _bitscanf( king_moves ) );
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
			king_moves = BitOperations::PopLSB( king_moves );
		}

		return moves;
	}

	std::vector<move> get_white_pawn_moves( bitboard pos )
	{
		std::vector<bitboard> board_moves{};
		std::vector<move> moves{};
		bitboard friendly_pieces{ get_all_friendly_pieces() };
		bitboard enemy_pieces{ get_all_enemy_pieces() };

		board_moves.push_back( ( pos << 8 & ~friendly_pieces ) & ~enemy_pieces );

		if ( pos & mask_rank[e_rank_2] )
		{
			board_moves.push_back( ( board_moves.back() << 8 & ~friendly_pieces ) & ~enemy_pieces );
		}

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

			if ( m.to & get_all_enemy_pieces() )
			{
				if ( m.from << 8 == m.to || m.from << 16 == m.to )
				{
					continue;
				}

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

		board_moves.push_back( ( pos >> 8 & ~friendly_pieces ) & ~enemy_pieces );

		if ( pos & mask_rank[e_rank_7] )
		{
			board_moves.push_back( ( board_moves.back() >> 8 & ~friendly_pieces ) & ~enemy_pieces );
		}

		board_moves.push_back( ( pos & clear_file[e_file_A] ) >> 9 & enemy_pieces );
		board_moves.push_back( ( pos & clear_file[e_file_H] ) >> 7 & enemy_pieces );

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
				if ( m.from >> 8 == m.to || m.from >> 16 == m.to )
				{
					continue;
				}

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
		std::vector<move> moves{};
		bitboard friendly_pieces{ get_all_friendly_pieces() };

		bitboard spot_1_clip = clear_file[e_file_A] & clear_file[e_file_B];
		bitboard spot_2_clip = clear_file[e_file_A];
		bitboard spot_3_clip = clear_file[e_file_H];
		bitboard spot_4_clip = clear_file[e_file_H] & clear_file[e_file_G];

		bitboard spot_5_clip = clear_file[e_file_H] & clear_file[e_file_G];
		bitboard spot_6_clip = clear_file[e_file_H];
		bitboard spot_7_clip = clear_file[e_file_A];
		bitboard spot_8_clip = clear_file[e_file_A] & clear_file[e_file_B];

		bitboard spot_1 = ( pos & spot_1_clip ) << 6;
		bitboard spot_2 = ( pos & spot_2_clip ) << 15;
		bitboard spot_3 = ( pos & spot_3_clip ) << 17;
		bitboard spot_4 = ( pos & spot_4_clip ) << 10;

		bitboard spot_5 = ( pos & spot_5_clip ) >> 6;
		bitboard spot_6 = ( pos & spot_6_clip ) >> 15;
		bitboard spot_7 = ( pos & spot_7_clip ) >> 17;
		bitboard spot_8 = ( pos & spot_8_clip ) >> 10;

		bitboard knight_moves = spot_1 | spot_2 | spot_3 | spot_4 | spot_5 | spot_6 | spot_7 | spot_8;
		knight_moves &= ~friendly_pieces;

		while ( knight_moves )
		{
			move m{};
			m.from |= ( 1ull << _bitscanf( pos ) );
			m.to |= ( 1ull << _bitscanf( knight_moves ) );
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
			knight_moves = BitOperations::PopLSB( knight_moves );
		}

		return moves;
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

	bool is_king_in_check( bool enemy = false, bitboard pos = 0 )
	{
		if ( enemy == true )
		{
			to_move = to_move == e_white ? e_black : e_white;
		}

		if ( pos == 0 )
		{
			pos = get_king_board();
		}

		bitboard attacks{ get_attacks( true ) };

		if ( enemy == true )
		{
			to_move = to_move == e_white ? e_black : e_white;
		}

		return attacks & pos;
	}

	bitboard get_pinned_board( bool enemy = false )
	{
		if ( enemy == true )
		{
			to_move = to_move == e_white ? e_black : e_white;
		}

		bitboard enemy_attacks{ get_sliding_attacks( 0, true ) };
		bitboard king_rays{ get_sliding_attacks( get_king_board() ) };
		bitboard pinned{ enemy_attacks & king_rays };
		pinned &= get_all_pieces();

		if ( enemy == true )
		{
			to_move = to_move == e_white ? e_black : e_white;
		}

		return pinned;
	}

	std::vector<move> get_all_moves( bool enemy = false )
	{
		if ( magic_initialized == false )
		{
			magic_init();
			std::cout << "Magic Initialized\n\n";
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
			bitboard board{ ( 1ull << _bitscanf( pawn_board ) ) };
			auto v{ get_pawn_moves( board ) };
			moves.insert( moves.end(), v.begin(), v.end() );
			pawn_board = BitOperations::PopLSB( pawn_board );
		}

		while ( knight_board )
		{
			bitboard board{ ( 1ull << _bitscanf( knight_board ) ) };
			auto v{ get_knight_moves( board ) };
			moves.insert( moves.end(), v.begin(), v.end() );
			knight_board = BitOperations::PopLSB( knight_board );
		}

		while ( bishop_board )
		{
			bitboard board{ ( 1ull << _bitscanf( bishop_board ) ) };
			auto v{ get_bishop_moves_magic( _bitscanf( board ), get_all_pieces() ) };
			moves.insert( moves.end(), v.begin(), v.end() );
			bishop_board = BitOperations::PopLSB( bishop_board );
		}

		while ( rook_board )
		{
			bitboard board{ ( 1ull << _bitscanf( rook_board ) ) };
			auto v{ get_rook_moves_magic( _bitscanf( board ), get_all_pieces() ) };
			moves.insert( moves.end(), v.begin(), v.end() );
			rook_board = BitOperations::PopLSB( rook_board );
		}

		while ( queen_board )
		{
			bitboard board{ ( 1ull << _bitscanf( queen_board ) ) };
			auto v{ get_queen_moves_magic( _bitscanf( board ), get_all_pieces() ) };
			moves.insert( moves.end(), v.begin(), v.end() );
			queen_board = BitOperations::PopLSB( queen_board );
		}

		while ( king_board )
		{
			bitboard board{ ( 1ull << _bitscanf( king_board ) ) };
			auto v{ get_king_moves( board ) };
			moves.insert( moves.end(), v.begin(), v.end() );
			king_board = BitOperations::PopLSB( king_board );
		}

		if ( enemy ) { to_move = to_move == e_white ? e_black : e_white; };
		return moves;
	}

	std::vector<move> get_all_legal_moves()
	{
		auto moves{ get_all_moves() };

		for ( int i = 0; i < moves.size(); i++ )
		{
			if ( get_pinned_board() & moves[i].from )
			{
				make_move( moves[i] );
				bitboard pinned{ get_pinned_board() };
				unmake_move();

				if ( ( pinned & moves[i].to ) == false )
				{
					moves.erase( moves.begin() + i-- );
					continue;
				}
			}

			std::vector<move> white_moves{};

			make_move( moves[i] );
			bool check{ is_king_in_check( true ) };
			bool enemy_check{ is_king_in_check() };
			unmake_move();

			if ( enemy_check && to_move == e_black )
			{
				const auto& m{ moves[i] };
				make_move( moves[i] );

				if ( get_all_legal_moves().empty() )
				{
					++checkmates;
				}

				unmake_move();
				++checks;
			}

			if ( check == true )
			{
				moves.erase( moves.begin() + i-- );
			}
		}

		return moves;
	}

	int checkmates{};

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
			} break;

			case e_knight:
			{
				bitboard& knight_board{ get_knight_board() };
				knight_board &= ~_move.from;
				knight_board |= _move.to;
			} break;

			case e_bishop:
			{
				bitboard& bishop_board{ get_bishop_board() };
				bishop_board &= ~_move.from;
				bishop_board |= _move.to;
			} break;

			case e_rook:
			{
				if ( to_move == e_white && ( white_short_castling_rights || white_long_castling_rights ) )
				{
					if ( _move.from & 0b0000000000000000000000000000000000000000000000000000000000000001 )
					{
						white_short_castling_rights = false;
					}
					else if ( _move.from & 0b0000000000000000000000000000000000000000000000000000000010000000 )
					{
						white_long_castling_rights = false;
					}
				}
				else if ( to_move == e_black && ( black_short_castling_rights || black_long_castling_rights ) )
				{
					if ( _move.from & 0b0000000100000000000000000000000000000000000000000000000000000000 )
					{
						black_short_castling_rights = false;
					}
					else if ( _move.from & 0b1000000000000000000000000000000000000000000000000000000000000000 )
					{
						black_long_castling_rights = false;
					}
				}

				bitboard& rook_board{ get_rook_board() };
				rook_board &= ~_move.from;
				rook_board |= _move.to;
			} break;

			case e_queen:
			{
				bitboard& queen_board{ get_queen_board() };
				queen_board &= ~_move.from;
				queen_board |= _move.to;
			} break;

			case e_king:
			{
				bitboard& king_board{ get_king_board() };
				king_board &= ~_move.from;
				king_board |= _move.to;
			
				if ( to_move == e_white && ( white_short_castling_rights || white_long_castling_rights ) )
				{
					white_short_castling_rights = false;
					white_long_castling_rights = false;
				}
				else if ( to_move == e_black && ( black_short_castling_rights || black_long_castling_rights ) )
				{
					black_short_castling_rights = false;
					black_long_castling_rights = false;
				}

				if ( _move.short_castle == true )
				{
					if ( to_move == e_white )
					{
						get_rook_board() &= ~( 1ull << 7 );
						get_rook_board() |= ( 1ull << 5 );
					}
					else
					{
						get_rook_board() &= ~( 1ull << 63 );
						get_rook_board() |= ( 1ull << 61 );
					}
				}
				else if ( _move.long_castle == true )
				{
					if ( to_move == e_white )
					{
						get_rook_board() &= ~( 1ull << 0 );
						get_rook_board() |= ( 1ull << 3 );
					}
					else
					{
						get_rook_board() &= ~( 1ull << 56 );
						get_rook_board() |= ( 1ull << 59 );
					}
				}
			} break;
		}

		if ( _move.capture == true )
		{
			switch ( _move.captured_type )
			{
			case e_pawn:
			{
				get_enemy_pawn_board() &= ~_move.to;
			} break;

			case e_knight:
			{
				get_enemy_knight_board() &= ~_move.to;
			} break;

			case e_bishop:
			{
				get_enemy_bishop_board() &= ~_move.to;
			} break;

			case e_rook:
			{
				get_enemy_rook_board() &= ~_move.to;
			} break;

			case e_queen:
			{
				get_enemy_queen_board() &= ~_move.to;
			} break;

			case e_king:
			{
				get_enemy_king_board() &= ~_move.to;
			} break;
			}
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

	int64_t perft( uint8_t depth )
	{
		if ( depth == 0 )
		{
			return 1;
		}

		int64_t count{ 0 };
		auto moves{ get_all_legal_moves() };

		for ( move m : moves )
		{
			move_list.push_back( m );
			make_move( m );

			if ( m.capture )
			{
				++captures;
			}

			count += perft( depth - 1 );
			unmake_move();
		}

		return count;
	}

	void perftdiv( uint8_t depth )
	{
		uint64_t pf{};

		auto moves{ get_all_legal_moves() };

		for ( move m : moves )
		{
			std::cout << square_id[_bitscanf( m.from )] << square_id[_bitscanf( m.to )];

			make_move( m );
			pf = perft( depth - 1 );
			std::cout << ": " << pf << " moves\n";
			unmake_move();
		}
	}
};

position p{};

bool make_user_move( bitboard from, bitboard to, uint32_t type, bool capture, uint32_t captured_type, bool short_castle, bool long_castle )
{
	move m{};

	m.from = from;
	m.to = to;
	m.type = (piece_type)type;
	m.capture = capture;
	m.captured_type = (piece_type)captured_type;
	m.short_castle = short_castle;
	m.long_castle = long_castle;
	
	auto moves{ p.get_all_legal_moves() };
	bool found{};

	for ( const auto& _m : moves )
	{
		if ( m == _m )
		{
			found = true;
			break;
		}
	}

	if ( found == false )
	{
		return false;
	}

	p.make_move( m );
	return true;
}

int main()
{
	position p{};
}