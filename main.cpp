#define OLC_PGE_APPLICATION
#include"olcPixelGameEngine.h"
#include<vector>
class board;

enum class GameState {
	blackTurn, whiteTurn, Promotion, whiteWins, blackWins, normal
};
enum class pieceType {
	BlackQueen, BlackKing, BlackBishop, BlackKnight, BlackRook, BlackPawn,
	WhiteQueen, WhiteKing, WhiteBishop, WhiteKnight, WhiteRook, WhitePawn, Empty, NotValid
};

class ai {
public:
	int size;
	//std::vector<std::tuple<pieceType*, int,bool, int>> hueristic;
	std::map<int64_t, int> map;
	std::vector<std::tuple<int64_t, int, int>> vec;
	//board, vector , bool -> void
	// return one random valid move
	int valueOfRoute(board* b, bool aiTurn);
	int inHueristic(pieceType* p, int depth);
	int maxValue(pieceType* p, bool turn);
	 pieceType* pr(board* b);
	 int recursion(board* b, int currentDepth, int MaxDepth, bool turn, float alpha, float beta);
	 pieceType* promote(board *b, bool tur);
};
class board {
public:
	


public:
	std::vector<pieceType*> vec;
	olc::Sprite *s;
	olc::Sprite* sprSheet;
	bool aiEnabled = true;
	bool aiTurn = white;
	olc::Sprite* sprites[12];
	board() {
		s = new olc::Sprite();
		sprSheet = new olc::Sprite();
		for(int i = 0; i < 12; i++)
			sprites[i]= new olc::Sprite();
		if (s->LoadFromFile("H:\\dev\\chess 2.0\\chess 2.0\\chessboard.png")) {
			std::cout << "success";
		}
		else {
			std::cout << "fail";
		}
		if (sprSheet->LoadFromFile("H:\\dev\\chess 2.0\\chess 2.0\\spriteSheet.png")) {
			std::cout << "success";
		}
		else {
			std::cout << "fail";
		}





		pieceType* arr = new pieceType[8 * 8];
		for (int i = 0; i < 8 * 8; i++) {
			arr[i] = pieceType::Empty;

		}
		for (int i = 0; i < 8; i++) {
			arr[1 * 8 + i] = pieceType::BlackPawn;
			arr[6 * 8 + i] = pieceType::WhitePawn;
		}
		arr[4] = pieceType::BlackQueen;
		arr[4 + 7 * 8] = pieceType::WhiteQueen;

		arr[3] = pieceType::BlackKing; 
		arr[3 + 7 * 8] = pieceType::WhiteKing;

		arr[0] = arr[7] = pieceType::BlackRook;
		arr[0 + 7 * 8] = arr[7 + 7 * 8] = pieceType::WhiteRook;

		arr[2] = arr[5] = pieceType::BlackBishop;
		arr[2 + 7 * 8] = arr[5 + 7 * 8] = pieceType::WhiteBishop;

		
		arr[1] = arr[6] = pieceType::BlackKnight;
		arr[1 + 7 * 8] = arr[6 + 7 * 8] = pieceType::WhiteKnight;



		vec.push_back(arr);
	
	}
	bool black = false, white = true;
	bool Turn = white;
	olc::vi2d size = { 8,8 };
	olc::vi2d Update(olc::PixelGameEngine * p) {
		//take user input
		static olc::vi2d mousepos;
		if (p->GetMouse(olc::Mouse::LEFT).bPressed) {
			mousepos = p->GetMousePos();
			mousepos -= {5, 10};
			mousepos /= tileSize;
		}
		olc::vi2d j = { 5,10 };
//		DrawSprite(mousepos * 24 + j, { 24,24 }, sprites[(int)pieceType::BlackPawn], p);

		return mousepos;
	}

	//vi2d, spr, pixelgameengine
	//Draw at location pos from offset to finish with size Size 
	void DrawSprite(const olc::vi2d pos,const olc::vi2d offset, const olc::vi2d finish
		,const  olc::vi2d size,olc::Sprite* spr,olc::PixelGameEngine *p) {
		

		p->SetPixelMode(olc::Pixel::ALPHA);
		for (int i = 0; i < size.y; i++) {
			for (int j = 0; j < size.x; j++) {
				olc::vi2d newPos = { j, i };
				olc::vi2d pixelPosition;
				
				pixelPosition.x = newPos.x / (float)size.x * (float)finish.x ;
				pixelPosition.y = newPos.y / (float)size.y * (float)finish.y ;
				
				p->Draw(pos + newPos, spr->GetPixel(pixelPosition + offset));
			}
		}
		p->SetPixelMode(olc::Pixel::NORMAL);
	}



	olc::vi2d newP;


	//promote pawn to king or bishop, or knight, or rook

	void promotePawn(olc::vi2d pos, std::vector<pieceType*>& val) {
		if ((int)getPieceType(pos) < 6 && pos.y == 7) {
			pieceType* t = CreateCopy();
			t[pos.x + 7 * 8] = pieceType::BlackBishop;
			val.push_back(t);

			 t = CreateCopy();
			t[pos.x + 7 * 8] = pieceType::BlackKing;
			val.push_back(t);

			 t = CreateCopy();
			t[pos.x + 7 * 8] = pieceType::BlackKnight;
			val.push_back(t);

			 t = CreateCopy();
			t[pos.x + 7 * 8] = pieceType::BlackRook;
			val.push_back(t);
		}

		if ((int)getPieceType(pos) >= 6 && (int)getPieceType(pos) < 12 && pos.y == 0) {
			pieceType* t = CreateCopy();
			t[pos.x + 0 * 8] = pieceType::WhiteBishop;
			val.push_back(t);

			 t = CreateCopy();
			t[pos.x + 0 * 8] = pieceType::WhiteKing;
			val.push_back(t);

			 t = CreateCopy();
			t[pos.x + 0 * 8] = pieceType::WhiteKnight;
			val.push_back(t);

			t = CreateCopy();
			t[pos.x + 0 * 8] = pieceType::WhiteRook;
			val.push_back(t);
		}

	}



	//pos -> bool
	//return valid pos
	//return true if x, y must be grater or equal zero but less tham 8 
	bool  inBound(olc::vi2d pos) {
		return (pos.x >= 0 && pos.y >= 0 && pos.x < 8 && pos.y < 8);
	}
	//vector, piece -> vector 
	//return valid moves of pawn
	//use embeessant, regular vertical movement, 2 move if in org place, right ot left in eating, promoting

	//example
	pieceType getPieceType (const olc::vi2d ps) {
		return vec[vec.size() - 1][ps.x + ps.y * 8];
	};
	// validPawn()
	void validPawn(olc::vi2d pos, std::vector<pieceType*>& val, int direction) {
		auto getPiece = [&](const olc::vi2d ps) {
			return vec[vec.size() - 1][ps.x + ps.y * 8];
		};
		auto getPreviousPiece = [&](const olc::vi2d ps) {
			return vec[vec.size() - 2][ps.x + ps.y * 8];
		};
		auto createClone = [&]() {
			pieceType* copy = new pieceType[8 * 8];
			for (int i = 0; i < 8 * 8; i++) {
				copy[i] = vec[vec.size() - 1][i];
			}
			return copy;

		};
		

		//natural x, y -> bool
		//return true if white
		auto isWhite = [&](olc::vi2d ps) {
			if ((int)getPiece(ps) >= 6 && (int)getPiece(ps) < 12)
				return true;
			return false;

		};
		auto isBlack = [&](olc::vi2d ps) {
			if ((int)getPiece(ps) < 6)
				return true;
			return false;

		};
		//pos1, pos2-> bool
		//true means they are not same type
		auto notSameType = [&](olc::vi2d p1, olc::vi2d p2) {
			return (isWhite(p1) && isBlack(p2)) || (isBlack(p1) && isWhite(p2));
		};


		promotePawn(pos, val);

		olc::vi2d test = { pos.x, pos.y + direction };
		pieceType* t;
		if (inBound(test) && getPiece(test) == pieceType::Empty) {
			t = createClone();
			t[test.x + test.y * 8] = getPiece(pos);
			t[pos.x + pos.y * 8] = pieceType::Empty;
			val.push_back(t);
			test = { pos.x, pos.y + direction * 2};
			if (inBound(test) && getPiece(test) == pieceType::Empty) {
				if (getPiece(pos) == pieceType::BlackPawn && pos.y == 1) {
					t = createClone();
					t[test.x + test.y * 8] = getPiece(pos);
					t[pos.x + pos.y * 8] = pieceType::Empty;
					val.push_back(t);
				}

				if (getPiece(pos) == pieceType::WhitePawn && pos.y == 6) {
					t = createClone();
					t[test.x + test.y * 8] = getPiece(pos);
					t[pos.x + pos.y * 8] = pieceType::Empty;
					val.push_back(t);
				}

			}


		}//moving farward, 2 steps
		test = { pos.x + 1, pos.y + direction };
		if (inBound(test) && notSameType(test, pos)) {
			t = createClone();
			t[test.x + test.y * 8] = getPiece(pos);
			t[pos.x + pos.y * 8] = pieceType::Empty;
			val.push_back(t);
		}
		test = { pos.x - 1, pos.y + direction };
		if (inBound(test) && notSameType(test, pos)) {
			t = createClone();
			t[test.x + test.y * 8] = getPiece(pos);
			t[pos.x + pos.y * 8] = pieceType::Empty;
			val.push_back(t);
		}
		//finshed killing
		
		//
		auto embessant = [&]() {
			if (inBound(test) && notSameType(test, pos) && getPiece(test) == pieceType::BlackPawn && pos.y == 3 &&
				getPiece({ test.x, test.y + direction }) == pieceType::Empty &&
				getPiece({ test.x, test.y + direction * 2 }) == pieceType::Empty &&
				getPreviousPiece(test) == pieceType::Empty &&
				getPreviousPiece({ test.x, test.y + direction }) == pieceType::Empty &&
				getPreviousPiece({ test.x, test.y + direction * 2 }) == pieceType::BlackPawn
				)
			{
				t = createClone();
				t[test.x + (test.y + direction) * 8] = getPiece(pos);
				t[test.x + test.y * 8] = pieceType::Empty;
				t[pos.x + pos.y * 8] = pieceType::Empty;
				val.push_back(t);

			}


			if (inBound(test) && notSameType(test, pos) && getPiece(test) == pieceType::WhitePawn && pos.y == 4 &&
				getPiece({ test.x, test.y + direction }) == pieceType::Empty &&
				getPiece({ test.x, test.y + direction * 2 }) == pieceType::Empty &&
				getPreviousPiece(test) == pieceType::Empty &&
				getPreviousPiece({ test.x, test.y + direction }) == pieceType::Empty &&
				getPreviousPiece({ test.x, test.y + direction * 2 }) == pieceType::WhitePawn
				)
			{
				t = createClone();
				t[test.x + (test.y + direction) * 8] = getPiece(pos);
				t[test.x + test.y * 8] = pieceType::Empty;
				t[pos.x + pos.y * 8] = pieceType::Empty; 
				val.push_back(t);

			}

		};


		//white embessant
		// next piece is black and moved by 2 spaces
		test = { pos.x - 1, pos.y };
		embessant();
		test = { pos.x + 1, pos.y };
		embessant();

		






	}
	GameState currentState =GameState::normal, newstate = GameState::normal;





	void Ontick(olc::vi2d current, std::vector<pieceType*>& validmoves) {


		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if ((i == 0 || i == 7) &&
					(getPieceType({ j,i }) == pieceType::BlackPawn || getPieceType({ j,i }) == pieceType::WhitePawn))
				{
					currentState = GameState::Promotion;
					newstate = GameState::Promotion;
				}
			}
		}

		auto GetPiece = [&](int x, int y) {
			if (y >= 0 && y < 8 && x >= 0 && x < 8)
				return vec[vec.size() - 1][y * 8 + x];
			else return pieceType::NotValid; };
		auto GetPieces = [&](olc::vi2d _pos) {
			if (_pos.y >= 0 && _pos.y < 8 && _pos.x >= 0 && _pos.x < 8)
				return vec[vec.size() - 1][_pos.y * 8 + _pos.x];
			else return pieceType::NotValid;
		};
		auto createCopy = [&](pieceType* org, pieceType* copy) {
			copy = new pieceType[8 * 8];
			for (int i = 0; i < 8 * 8; i++) {
				copy[i] = org[i];
			}
			return copy;

		};
		
			if (GetPieces(current) != pieceType::Empty) {
				//if (Turn == black) {
					if ((int)GetPieces(current) < 6) {
						ValidMovesOf(current, validmoves);


					}
				//}
				// (Turn == white) {
					if ((int)GetPieces(current) >= 6) {

						ValidMovesOf(current, validmoves);



					}
				//}




			}
		
			filter(validmoves);
			if (Lose(Turn) && Turn == black)
				currentState = GameState::whiteWins;
			if (Lose(Turn) && Turn == white)
				currentState = GameState::blackWins;

	}

	//bool -> bool
	// takes turn and return true if there is any valid Move and is in check
	bool Lose(bool turn) {
		std::vector<pieceType*> ptr;
		if (checkBoard(turn, vec[vec.size() - 1])) {
			for (int i = 0; i < 8; i++) {
				for (int j = 0; j < 8; j++) {
					if (turn == black) {
						if ((int)getPieceType({ j,i }) < 6)
							if (getPieceType({ j,i }) == pieceType::BlackQueen)
								queen({ j,i }, ptr);
							else
							ValidMovesOf({ j,i }, ptr);
					}
					else {
						if ((int)getPieceType({ j,i }) >= 6 && (int)getPieceType({ j,i }) < 12)
							if (getPieceType({ j,i }) == pieceType::WhiteQueen)
								queen({ j,i }, ptr);
							else
							ValidMovesOf({ j,i }, ptr);

					}
				}
			}
			filter(ptr);
			bool b = ptr.size() != 0;
				//oneValidMove(ptr);
			for (auto& a : ptr)
				delete[] a;
			ptr.clear();
			if (b)
				return false;
			else
				return true;
		}

		return false;
	}



	//vector -> void
	//takes validMoves and filter unWanted validMoves that cause queen to be killed
	void filter(std::vector<pieceType*>& validmoves) {

		bool t = !Turn;
		//if true remove piece
		auto checkBoards = [&](pieceType* arr) {
			bool result = checkBoard(Turn, arr);
			if (result)
				delete[] arr;
			return result;

					
		};
		
		auto i =  std::remove_if(validmoves.begin(), validmoves.end(), checkBoards);
		validmoves.erase(i, validmoves.end());


	}



	bool oneValidMove(std::vector<pieceType*>& validmoves) {

		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++) {
				if (!checkBoard(Turn,validmoves[i]))
					return true;
			}
		}
		return false;

	}



	void moveIndefinitely(olc::vi2d ps, olc::vi2d v, std::vector<pieceType*>& val) {
		auto createClone = [&]() {

			pieceType* copy = new pieceType [8 * 8];
			for (int i = 0; i < 8 * 8; i++) {
				copy[i] = vec[vec.size() - 1][i];
			}
			return copy;

		};
		auto isWhite = [&](olc::vi2d ps) {
			if ((int)getPieceType(ps) >= 6 && (int)getPieceType(ps) < 12)
				return true;
			return false;

		};
		auto isBlack = [&](olc::vi2d ps) {
			if ((int)getPieceType(ps) < 6)
				return true;
			return false;

		};
		auto notSameType = [&](olc::vi2d p1, olc::vi2d p2) {
			return (isWhite(p1) && isBlack(p2)) || (isBlack(p1) && isWhite(p2));
		};
		int x = 1; 
		pieceType* t;
		olc::vi2d test = ps + v * x;
		while (inBound(test) && getPieceType(test) == pieceType::Empty) {
			t = createClone();
			t[test.x + test.y * 8] = getPieceType(ps);
			t[ps.x + ps.y * 8] = pieceType::Empty;
			val.push_back(t);
			x++;
			test = ps + v * x;
		}
		test = ps + v * x;
		if (inBound(test)&&notSameType(test,ps)) {
			t = createClone();
			t[test.x + test.y * 8] = getPieceType(ps);
			t[ps.x + ps.y * 8] = pieceType::Empty;
			val.push_back(t);


		}


	}

	void Rook(olc::vi2d ps, std::vector<pieceType*>& val) {
		olc::vi2d v = {0,1};
		moveIndefinitely(ps, v, val);
		 v = { 0,-1 };
		moveIndefinitely(ps, v, val);
		 v = { 1,0 };
		moveIndefinitely(ps, v, val);
		 v = { -1,0 };
		moveIndefinitely(ps, v, val);
		
	}
	void bishop(olc::vi2d ps, std::vector<pieceType*>& val) {
		olc::vi2d v = { 1,1 };
		moveIndefinitely(ps, v, val);
		v = { 1,-1 };
		moveIndefinitely(ps, v, val);
		v = { -1,1 };
		moveIndefinitely(ps, v, val);
		v = { -1,-1 };
		moveIndefinitely(ps, v, val);

	}


	void king(olc::vi2d ps, std::vector<pieceType*>& val) {
		bishop(ps, val);
		Rook(ps, val);
	}
	//vi2d, vector -> vector
	//valid moves of knight 
	// example (3,1), (3,-1), (-3,1), (-3,-1), (1,3), (-1, 3), (1, -3), (-1, -3)
	void knight(olc::vi2d ps, std::vector<pieceType*>& val) {
		auto createClone = [&]() {
			pieceType* copy = new pieceType[8 * 8];
			for (int i = 0; i < 8 * 8; i++) {
				copy[i] = vec[vec.size() - 1][i];
			}
			return copy;

		};
		auto isWhite = [&](olc::vi2d ps) {
			if ((int)getPieceType(ps) >= 6 && (int)getPieceType(ps) < 12)
				return true;
			return false;

		};
		auto isBlack = [&](olc::vi2d ps) {
			if ((int)getPieceType(ps) < 6)
				return true;
			return false;

		};
		auto notSameType = [&](olc::vi2d p1, olc::vi2d p2) {
			return (isWhite(p1) && isBlack(p2)) || (isBlack(p1) && isWhite(p2));
		};
		auto checkMove = [&](int x, int y) {
			olc::vi2d test = {ps.x +  x, ps.y + y };
			pieceType* t;
			if (inBound(test)&&(getPieceType(test) == pieceType::Empty|| notSameType(ps, test))) {
				t = createClone();
				t[test.x + test.y * 8] = getPieceType(ps);
				t[ps.x + ps.y * 8] = pieceType::Empty;
				val.push_back(t);
			}


		}; 
		checkMove(2, 1);
		checkMove(2, -1);
		checkMove(-2, 1);
		checkMove(-2, -1);
		checkMove(1, 2);
		checkMove(-1, 2);
		checkMove(1, -2);
		checkMove(-1, -2);



	}







	void queen(olc::vi2d ps, std::vector<pieceType*>& val) {
		auto getPiece = [&](const olc::vi2d ps) {
			return vec[vec.size() - 1][ps.x + ps.y * 8];
		};

		auto isWhite = [&](olc::vi2d ps) {
			if ((int)getPiece(ps) >= 6 && (int)getPiece(ps) < 12)
				return true;
			return false;

		};
		auto isBlack = [&](olc::vi2d ps) {
			if ((int)getPiece(ps) < 6)
				return true;
			return false;

		};
		//pos1, pos2-> bool
		//true means they are not same type
		auto notSameType = [&](olc::vi2d p1, olc::vi2d p2) {
			return (isWhite(p1) && isBlack(p2)) || (isBlack(p1) && isWhite(p2));
		};
		
		auto createClone = [&]() {
			pieceType* copy = new pieceType[8 * 8];
			for (int i = 0; i < 8 * 8; i++) {
				copy[i] = vec[vec.size() - 1][i];
			}
			return copy;

		};
		
		auto test = [&](olc::vi2d ts) {
			ts = ts + ps;
			pieceType* t;
			if (inBound(ts) && (notSameType(ps, ts) || getPiece(ts) == pieceType::Empty)) {
				t = createClone();
				t[ts.x + ts.y * 8] = getPiece(ps);
				t[ps.x + ps.y * 8] = pieceType::Empty;
				val.push_back(t);
			}
			
		};
		olc::vi2d tst = { 0,1 };
		test(tst);
		tst = { 0,-1 };
		test(tst);
		tst = { 1,0 };
		test(tst);
		tst = { -1,0 };
		test(tst);
		tst = { 1,1 };
		test(tst);
		tst = { -1,1 };
		test(tst);
		tst = { 1,-1 };
		test(tst);
		tst = { -1,-1 };
		test(tst);









	}


	void ValidMovesOf(olc::vi2d p, std::vector<pieceType*>& val) {

		switch (vec[vec.size() - 1][p.x + p.y * 8]) {
		case pieceType::BlackPawn:
			validPawn(p, val, 1);
				break;
		case pieceType::WhitePawn:
			validPawn(p, val, -1);
				break;

		case pieceType::BlackQueen:
			canTabeet(Turn, p, val);
			queen(p, val);
			break;
		case pieceType::WhiteQueen:
			canTabeet(Turn, p, val);
			queen(p, val);
			break; 
		case pieceType::BlackRook:
			Rook(p, val);
			break;
		case pieceType::WhiteRook:
			Rook(p, val);
			break;
		case pieceType::BlackBishop:
			bishop(p, val);
			break;
		case pieceType::WhiteBishop:
			bishop(p, val);
			break;
		case pieceType::BlackKing:
			king(p, val);
			break;
		case pieceType::WhiteKing:
			king(p, val);
			break;
		case pieceType::BlackKnight:
			knight(p, val);
			break;
		case pieceType::WhiteKnight:
			knight(p, val);
			break;
			
		}
		
	}

	int promotiontileSize = 150/4;
	int offsetx = 50;

	bool canTabeet(bool tur, olc::vi2d pos, std::vector<pieceType*>& val) {
		auto tabinDir = [&](const olc::vi2d quee, const olc::vi2d rook) {
			if (didMove(quee))
				return false;
			if (didMove(rook))
				return false;
			int dir = rook.x - quee.x;
			dir /= abs(rook.x - quee.x);
			int x = quee.x + dir;
			while (x != rook.x) {
				if (getPieceType({ x, quee.y }) != pieceType::Empty)
					return false;


				x += dir;
			}
			pieceType* t;
			t = CreateCopy();
			t[pos.x + pos.y * 8] = pieceType::Empty;
			t[rook.x + rook.y * 8] = pieceType::Empty;
			if (dir > 0) {
				t[rook.y * 8 + 5] = getPieceType(rook);
				t[rook.y * 8 + 6] = getPieceType(quee);

			}

			if (dir < 0) {
				t[rook.y * 8 + 2] = getPieceType(rook);
				t[rook.y * 8 + 1] = getPieceType(quee);

			}
			val.push_back(t);


			return true;
				
		};

		
		if (tur == black) {
			if (getPieceType(pos) == pieceType::BlackQueen)
			{
				for (int i = 0; i < vec.size(); i++)
				{
					if (checkBoard(tur, vec[i]))
						return false;
				}
				tabinDir(pos, { 7, pos.y });
				tabinDir(pos, { 0, pos.y });





			}
		}
		if (tur == white) {
			if (getPieceType(pos) == pieceType::WhiteQueen)
			{
				for (int i = 0; i < vec.size(); i++)
				{
					if (checkBoard(tur, vec[i]))
						return false;
				}
				tabinDir(pos, { 7, pos.y });
				tabinDir(pos, { 0, pos.y });





			}
		}



	}

	//bool, pieceType* -> bool
	//return true if queen was endanger in this board 
	bool checkBoard(bool tur, pieceType * b)
	{
		vec.push_back(b);

		std::vector<pieceType*> valM;
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if (tur == black) {
					if (getPieceType({ j,i }) == pieceType::WhiteQueen) {
						queen({ j,i }, valM);
					}
					else if ((int)getPieceType({j,i}) >= 6 && (int)getPieceType({ j,i }) < 12) {
						ValidMovesOf({ j,i }, valM);
					
					
					}
				}
				if (tur == white) {
					if (getPieceType({ j,i }) == pieceType::BlackQueen) {
						queen({ j,i }, valM);
					}
					else if ((int)getPieceType({ j,i }) < 6) {
						ValidMovesOf({ j,i }, valM);


					}
				}


			}
		}
		
		vec.pop_back();
		bool result = false;
		for (int i = 0; i < valM.size(); i++) {
			pieceType* ar = valM[i];
			bool containQueen = false;
			for (int i = 0; i < 8; i++) {
				for (int j = 0; j < 8; j++) {
					if (tur == black && ar[j + i * 8] == pieceType::BlackQueen)
						containQueen = true;
					if (tur == white && ar[j + i * 8] == pieceType::WhiteQueen)
						containQueen = true;


				}
			}
			if (!containQueen)
				result = true;

		}
		for (int i = 0; i < valM.size(); i++)
			delete[] valM[i];
		valM.clear();

		return result;


	}



	//olc::vi2d -> bool
	//return true if piece at this position changed across all boards
	bool didMove(olc::vi2d pos)
	{
		pieceType test = getPieceType(pos);
		for (int i = 0; i < vec.size(); i++) {
			if (test != vec[i][pos.x + pos.y * 8])
				return true;

		}
		return false;
	}

	bool promote(olc::PixelGameEngine* p, olc::vi2d current, std::vector<pieceType*>& validMoves) {
		auto typeOfPromote = [&](olc::vi2d &ps) {
			for (int j = 0; j < 8; j++) {
				if (getPieceType({ j, 0 }) == pieceType::WhitePawn) {
					ps = { j,0 };
					return true;
				}
				if (getPieceType({ j, 7 }) == pieceType::BlackPawn) {
					ps = { j,7 };
					return false;
				}

			}
			std::cout << "error in promotion";
			return false;
		};



		if (current.y > p->ScreenHeight() / 2 - promotiontileSize / 2
			&& current.y < p->ScreenHeight() / 2 + promotiontileSize / 2) {
			int testPos = current.x - offsetx;
			if (testPos >= 0 && testPos < promotiontileSize * 4) {
				int piec = testPos / promotiontileSize;
				piec++;
				olc::vi2d ps;
				if (typeOfPromote(ps)) {
					piec += 6;
					pieceType* t = CreateCopy();
					t[ps.x + 0] = (pieceType)piec;
					if (oneMatc(validMoves, t)) {
						vec.push_back(t);
						return true;
					}
				}
				else {
					pieceType* t = CreateCopy();
					t[ps.x + 7 * 8] = (pieceType)piec;
					if (oneMatc(validMoves, t)) {
						vec.push_back(t);
						return true;
					}
				}

			}

		}
		return false;
	 }
	pieceType* CreateCopy () {
		pieceType* copy = new pieceType[8 * 8];
		for (int i = 0; i < 8 * 8; i++) {
			copy[i] = vec[vec.size() - 1][i];
		}
		return copy;

	};

	void bayet(std::vector<pieceType*>& validmoves, olc::vi2d newP, olc::vi2d current, olc::PixelGameEngine* p) {
		pieceType* t;
		
		if (newP.x == 6)
		{
			t = CreateCopy();
			t[current.x + current.y * 8] = pieceType::Empty;
			t[7 + current.y * 8] = pieceType::Empty;
			t[6 + current.y * 8] = getPieceType(current);
			t[5 + current.y * 8] = getPieceType({7,current.y});
			if (oneMatc(validmoves, t))
			{
				vec.push_back(t);
				Turn = !Turn;
			}
			else {
				delete[] t;
			}



		}
		else if (newP.x == 1) {
			t = CreateCopy();
			t[current.x + current.y * 8] = pieceType::Empty;
			t[0 + current.y * 8] = pieceType::Empty;
			t[1 + current.y * 8] = getPieceType(current);
			t[2 + current.y * 8] = getPieceType({ 0,current.y });
			if (oneMatc(validmoves, t))
			{
				vec.push_back(t);
				Turn = !Turn;
			}
			else {
				delete[] t;
			}
		}

	}

	ai Ai;
	olc::vi2d HandleKeyPress(olc::PixelGameEngine *p, std::vector<pieceType*>& validMoves) {
	aiTurn = white;
		static olc::vi2d mousePos = {-1,-1};
		olc::vi2d current = Update(p);
		
		olc::vi2d m2 = {0,0};
		switch (currentState)
		{
		case GameState::Promotion:
			if (aiTurn == !Turn && aiEnabled) {

				pieceType* z = Ai.promote(this, !Turn);
				
					vec.push_back(z);
					currentState = GameState::normal;

			}
			else {

				if (p->GetMouse(olc::Mouse::LEFT).bPressed)
					m2 = p->GetMousePos();
				if (promote(p, m2, validMoves))
					currentState = GameState::normal;
			}
			break;
		case GameState::normal:
			if (aiTurn == Turn && aiEnabled) {
				
				pieceType* z = Ai.pr(this);
				for (int i = 0; i < 8; i++) {
					for (int j = 0; j < 8; j++)
						ValidMovesOf({ j,i }, validMoves);
				}
				if (oneMatc(validMoves, z)) {
					vec.push_back(z);
					Turn = !Turn;
				}
			}
			else if((int )getPieceType(mousePos) < 6 && (int)getPieceType(mousePos) < 12){

				bayet(validMoves, current, mousePos, p);
				
				playerInput(validMoves, current, mousePos, p);
			}
			break;
		default:
			break;
		}
		mousePos = current;
		return mousePos;
	}

	bool oneMatc(std::vector<pieceType*>& validmoves, pieceType*ar) {
		for (int j = 0; j < validmoves.size(); j++) {
			bool areequal = true;
			for (int i = 0; i < 8 * 8; i++) {
				if (validmoves[j][i] != ar[i])
					areequal = false;
			}
			if (areequal == true) {
				return true;
				
			}
		}
		return false;
	}

	void playerInput(std::vector<pieceType*>& validmoves, olc::vi2d newP, olc::vi2d current, olc::PixelGameEngine* p) {
		auto GetPiece = [&](int x, int y) {
			if (y >= 0 && y < 8 && x >= 0 && x < 8)
				return vec[vec.size() - 1][y * 8 + x];
			else return pieceType::NotValid; };
		auto GetPieces = [&](olc::vi2d _pos) {
			if (_pos.y >= 0 && _pos.y < 8 && _pos.x >= 0 && _pos.x < 8)
				return vec[vec.size() - 1][_pos.y * 8 + _pos.x];
			else return pieceType::NotValid;
		};
		auto createCopy = [&](pieceType* org, pieceType* copy) {
			copy = new pieceType[8 * 8];
			for (int i = 0; i < 8 * 8; i++) {
				copy[i] = org[i];
			}
			return copy;

		};
		
		
		if (inBound(current) && inBound(newP) &&newP != current) {
			pieceType* bar ;
			bar = CreateCopy();

			if (GetPieces(current) == pieceType::BlackPawn && GetPieces(newP) == pieceType::Empty
				&& newP.x != current.x && newP.y != current.y) {

				bar[newP.x + newP.y * 8] = pieceType::BlackPawn;
				bar[newP.x + current.y * 8] = pieceType::Empty;
				bar[current.x + current.y * 8] = pieceType::Empty;


			}
			else if (GetPieces(current) == pieceType::WhitePawn && GetPieces(newP) == pieceType::Empty
				&& newP.x != current.x && newP.y != current.y) {
				bar[newP.x + newP.y * 8] = pieceType::WhitePawn;
				bar[newP.x + current.y * 8] = pieceType::Empty;
				bar[current.x + current.y * 8] = pieceType::Empty;

			}
			else {

				bar[newP.x + newP.y * 8] = bar[current.x + current.y * 8];
				bar[current.x + current.y * 8] = pieceType::Empty;
			}
			
			if (oneMatc(validmoves, bar)) {
				vec.push_back(bar);
				Turn = !Turn;

			}
			else if(bar != nullptr){
				delete[] bar;
			
			}

		
		}
		for (int i = 0; i < validmoves.size(); i++)
			delete[] validmoves[i];
		validmoves.clear();

	}

	int tileSize = 100/4 ;

	void DrawPiece(olc::vi2d pos, olc::Sprite *spf, olc::PixelGameEngine* p) {
		olc::vi2d off;
		olc::vi2d tileS = {spf->width / 6, spf->width / 6};
		off.y = (int)getPieceType(pos) / 6;
		off.x = (int)getPieceType(pos) - 6 * off.y;
		off.y = (off.y + 1) % 2;
		off *= tileS.x;
		DrawSprite({ 5 + pos.x * tileSize, 10 + pos.y * tileSize }, off, tileS, { tileSize,tileSize }, spf, p);


	}
	void DrawPromote(olc::PixelGameEngine *p) {
		auto typeOfPromote = [&](olc::vi2d& ps) {
			for (int j = 0; j < 8; j++) {
				if (getPieceType({ j, 0 }) == pieceType::WhitePawn) {
					ps = { j,0 };
					return true;
				}
				if (getPieceType({ j, 7 }) == pieceType::BlackPawn) {
					ps = { j,7 };
					return false;
				}

			}
			std::cout << "error in promotion";
			return false;
		};

		olc::vi2d ps;
		if (typeOfPromote(ps)) {
			int smallh = sprSheet->height / 2;

			DrawSprite({ offsetx,p->ScreenHeight() / 2 - promotiontileSize / 2 }, { smallh,0 }, { smallh * 4, smallh },
				{promotiontileSize * 4, promotiontileSize},
				sprSheet, p);
		}
		else {
			int smallh = sprSheet->height / 2;

			DrawSprite({ offsetx,p->ScreenHeight() / 2 - promotiontileSize / 2 }, { smallh,smallh }, { smallh * 4, smallh },
				{ promotiontileSize * 4, promotiontileSize },
				sprSheet, p);
		}


	}


	void Draw(olc::PixelGameEngine* p) {
		static olc::vi2d current = { 0,0 };
		std::vector<pieceType*> validmoves;
		Ontick(current, validmoves);

		current = HandleKeyPress(p, validmoves);
		//draw sprites
		auto GetPiece = [&](int x, int y) {
			if (y >= 0 && y < 8 && x >= 0 && x < 8)
			return vec[vec.size() - 1][y * 8 + x];
			else return pieceType::NotValid; };
		auto GetPieces = [&](olc::vi2d _pos) {
			if (_pos.y >= 0 && _pos.y < 8 && _pos.x >= 0 && _pos.x < 8)
			return vec[vec.size()-1][_pos.y * 8 + _pos.x]; 
			else return pieceType::NotValid;
		};
		auto createCopy = [&](pieceType* org, pieceType* copy) {
			copy = new pieceType[8 * 8];
			for (int i = 0; i < 8 * 8; i++) {
				copy[i] = org[i];
			}
			return copy;

		};



		auto isBlack = [&](int x, int y) {
			if ((int)GetPiece(x, y) < 6 &&
				(int)GetPiece(x, y) < 12)
				return true;
			else return false;

		};
		auto isWhite = [&](int x, int y) {
			if ((int)GetPiece(x, y) >= 6 &&
				(int)GetPiece(x, y) < 12)
				return true;
			else return false;

		};


		pieceType* ar = new pieceType[8 * 8];
		DrawSprite({ 5,10 }, { 0,0 }, {s->width, s->height}, { 8 * tileSize,tileSize * 8 }, s, p);
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {

				olc::vi2d poss = { j,i };
				if ((int)getPieceType({j,i}) < 12) {
					DrawPiece(poss, sprSheet, p);
				}



			}
		}
		int textsize = 100;
		if (currentState == GameState::Promotion) {
			DrawPromote(p);
		}
		if (currentState == GameState::whiteWins) {
			p->DrawString({ 0, p->ScreenHeight() / 2  },"white wins", olc::WHITE,8);
		}
		if (currentState == GameState::blackWins) {
			p->DrawString({ 0, p->ScreenHeight() / 2 }, "black wins", olc::WHITE,8);
		}

		// decide which piece is chosen
		
		//visualize options
		
		
	}

};


pieceType* ai::promote(board *b, bool tur) {

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if ((i == 0 || i == 7) && (b->getPieceType({ j, i }) == pieceType::BlackPawn ||
				b->getPieceType({ j, i }) == pieceType::WhitePawn)) {
				pieceType* t;
				t = b->CreateCopy();
				if (tur == b->black)
					t[j + i * 8] = pieceType::BlackKing;
				else
					t[j + i * 8] = pieceType::WhiteKing;

				return t;
			}
		}
	}
	return nullptr;
}


int ai::valueOfRoute(board* b, bool aiTurn) {
	int resultBlack = 0, resultWhite = 0;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if ((int)b->getPieceType({ j, i }) < 6) {
				switch (b->getPieceType({ j, i }))
				{
				case pieceType::BlackPawn:
					resultBlack += 1;
					break;

				case pieceType::BlackBishop:
					resultBlack += 3;
					break;
				case pieceType::BlackRook:
					resultBlack += 5;
					break;
				case pieceType::BlackKing:
					resultBlack += 9;
					break;
				case pieceType::BlackKnight:
					resultBlack += 3;
					break;

				}
			}
			else if ((int)b->getPieceType({ j, i }) >= 6 && (int)b->getPieceType({ j, i }) < 12) {
				switch (b->getPieceType({ j, i }))
				{
				case pieceType::WhitePawn:
					resultWhite += 1;
					break;

				case pieceType::WhiteBishop:
					resultWhite += 3;
					break;
				case pieceType::WhiteRook:
					resultWhite += 5;
					break;
				case pieceType::WhiteKing:
					resultWhite += 9;
					break;
				case pieceType::WhiteKnight:
					resultWhite += 3;
					break;

				}
			}

		}
	}

	if (aiTurn == b->black) {
		return resultBlack - resultWhite;
	}
	else {
		return resultWhite - resultBlack;
	}

}
int ai::maxValue(pieceType* p, bool turn) {
	int resultBlack = 0, resultWhite = 0;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if ((int)p[j + i * 8] < 6) {
				switch (p[j + i * 8])
				{
				case pieceType::BlackPawn:
					resultBlack += 1;
					break;

				case pieceType::BlackBishop:
					resultBlack += 3;
					break;
				case pieceType::BlackRook:
					resultBlack += 5;
					break;
				case pieceType::BlackKing:
					resultBlack += 9;
					break;
				case pieceType::BlackKnight:
					resultBlack += 3;
					break;

				}
			}
			else if ((int)p[j + i * 8] >= 6 && (int)p[j + i * 8] < 12) {
				switch (p[j + i * 8])
				{
				case pieceType::WhitePawn:
					resultWhite += 1;
					break;

				case pieceType::WhiteBishop:
					resultWhite += 3;
					break;
				case pieceType::WhiteRook:
					resultWhite += 5;
					break;
				case pieceType::WhiteKing:
					resultWhite += 9;
					break;
				case pieceType::WhiteKnight:
					resultWhite += 3;
					break;

				}
			}

		}
	}

	if (turn == false) {
		return resultBlack - resultWhite;
	}
	else {
		return resultWhite - resultBlack;
	}

}

pieceType change(pieceType* p, board *b) {
	if (b->vec.size() <= 1)
		return pieceType::Empty;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (p[j + i * 8] != pieceType::Empty && p[j + i * 8] != b->vec[b->vec.size() - 2][j + i * 8])
				return p[j = i * 8];

		}
	}
	return pieceType::Empty;

}

int pieceValue(pieceType* p, board* b) {
	switch (change(p, b))
	{
	case pieceType::BlackPawn:
		return 1;
		break;

	case pieceType::BlackBishop:
		return 3;
		break;
	case pieceType::BlackRook:
		return 5;
		break;
	case pieceType::BlackKing:
		return 9;
		break;
	case pieceType::BlackKnight:
		return 3;
		break;




	case pieceType::WhitePawn:
		return 1;
		break;

	case pieceType::WhiteBishop:
		return 3;
		break;
	case pieceType::WhiteRook:
		return 5;
		break;
	case pieceType::WhiteKing:
		return 9;
		break;
	case pieceType::WhiteKnight:
		return 3;
		break;

	}
}
int64_t hash(pieceType* p) {
	int64_t pow = 1;
	int64_t result = 0;
	for (int i = 0; i < 32; i++) {
		result += (int64_t)p[i] * pow;
		pow *= 2;

	}
	pow = 1;
	for (int i = 32; i < 64; i++) {
		result += (int64_t)p[i] * pow;
		pow *= 2;
	}

	return result;

}

pieceType* ai::pr(board* b)
{
	auto compare = [&](pieceType* p1, pieceType* p2) {
		return (pieceValue(p1, b) < pieceValue(p2, b));
	};
	/*for (int i = 0; i < hueristic.size(); i++) {
		delete[] hueristic[i]._Myfirst._Val;

	}*/
	vec.clear();
	pieceType* result = promote(b, b->aiTurn);
	if (result == nullptr) {
	    size = 0;
		std::vector<pieceType*> ptBlack;
		std::vector<pieceType*> ptWhite;
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if ((int)b->getPieceType({ j, i }) < 6) {
					b->ValidMovesOf({ j,i }, ptBlack);
					b->filter(ptBlack);
				}
				else if ((int)b->getPieceType({ j, i }) >= 6 && (int)b->getPieceType({ j, i }) < 12) {
					b->ValidMovesOf({ j,i }, ptWhite);
					b->filter(ptWhite);
				}
			}
		}

		//std::sort(ptBlack.begin(), ptBlack.end(), compare);

		//std::sort(ptWhite.begin(), ptWhite.end(), compare);
		int value = -10000;
		if (b->aiTurn == b->black) {

			
			
			for (auto a : ptBlack) {
				b->vec.push_back(a);
				int rec;
				rec = recursion(b, 0, 3, !b->aiTurn, value, 10000);



				if (rec > value) {
					value = rec;
					result = a;

				}
				b->vec.pop_back();

			}
		

		}
		else {
			
			for (auto a : ptWhite) {
				b->vec.push_back(a);
				int rec;
				rec = recursion(b, 0 , 3, !b->aiTurn, value, 10000);
				
				if (rec > value) {
					value = rec;
					result = a;

				}
				b->vec.pop_back();

			}
			


		}
		for (int i = 0; i < ptWhite.size(); i++) {
			if (ptWhite[i] != result)
				delete[] ptWhite[i];
		}

		for (int i = 0; i < ptBlack.size(); i++) {
			if (ptBlack[i] != result)
				delete[] ptBlack[i];
		}
		ptBlack.clear();
		ptWhite.clear();
		//std::cout << hash(result);
		std::cout << size << std::endl;
		std::cout << value << "\n";
	}
	return result;
}









bool error(board* b, bool turn){
	if (b->checkBoard(!turn, b->vec[b->vec.size() - 1]))
		return true;

}
// board int int -> int
// return max win from this route;
int ai::recursion(board* b, int currentDepth, int MaxDepth, bool turn, float alpha, float beta)
{
	size++;
	//std::cout << "1";
	bool err = error(b, turn);


	if (currentDepth > MaxDepth)
		std::cout << currentDepth<<"\n";
	if ((currentDepth >= MaxDepth)) {
		
		return valueOfRoute(b,b->aiTurn);
	}
	if (b->Lose(b->aiTurn)) {
		return -1000;
	}
	if (b->Lose(!b->aiTurn)) {
		return 1000;
	}

	auto compareMax = [&](pieceType* p1, pieceType* p2) {
		return (maxValue(p1, b->aiTurn) > maxValue(p2, b->aiTurn));
	};
	auto compareMin = [&](pieceType* p1, pieceType* p2) {
		return (maxValue(p1, b->aiTurn) < maxValue(p2, b->aiTurn));
	};

	pieceType* ptr;
	ptr = promote(b, !turn);
	if(ptr != nullptr){
		b->vec.push_back(ptr);
		int max =  recursion(b, currentDepth + 1, MaxDepth, turn, alpha, beta);
		b->vec.pop_back();
		delete[] ptr;
		return max;
	}
	else {
		delete[] ptr;

		std::vector<pieceType*> ptBlack;
		std::vector<pieceType*> ptWhite;
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if ((int)b->getPieceType({ j, i }) < 6) {
					b->ValidMovesOf({ j,i }, ptBlack);
					//b->filter(ptBlack);
				}
				else if ((int)b->getPieceType({ j, i }) >= 6 && (int)b->getPieceType({ j, i }) < 12) {
					b->ValidMovesOf({ j,i }, ptWhite);
					//b->filter(ptWhite);
				}
			}
		}

		


		
		if (turn == b->black) {
			if (b->black == b->aiTurn) {
				//std::sort(ptBlack.begin(), ptBlack.end(), compareMax);
				
				int value = -10000;//recursion(b, currentDepth + 1, MaxDepth, !turn, alpha, beta);

				for (auto a : ptBlack) {
					b->vec.push_back(a);

					int rec; 
					int h = inHueristic(a,currentDepth);
					if (h == -1000000) {
						rec = recursion(b, currentDepth + 1, MaxDepth, !turn, alpha, beta);
						//std::cout << rec << std::endl;
						vec.push_back(std::tuple<int64_t, int, int>(hash(a), rec, currentDepth));
						map.insert(std::pair<int64_t, int>(hash(a), rec));
					}
					else rec = h;

					//rec = recursion(b, currentDepth + 1, MaxDepth, !turn, alpha, beta);
					//	if (h != -1000000)
						//	std::cout << "h: " << h << " rec: " << rec << std::endl;


					value =std::max(value ,rec);
					alpha = std::max((float)value, alpha);
					b->vec.pop_back();
					if (alpha >= beta)
						break;

				}
				for (int i = 0; i < ptBlack.size(); i++) {
					delete[] ptBlack[i];
				}
				for (int i = 0; i < ptWhite.size(); i++) {
					delete[] ptWhite[i];
				}

				ptBlack.clear();
				
				return value;
			}
			else {
				//std::sort(ptBlack.begin(), ptBlack.end(), compareMin);
				//b->vec.push_back(ptBlack[0]);
				int value = 10000;//recursion(b, currentDepth + 1, MaxDepth, !turn, alpha, beta);
			//	b->vec.pop_back();

				for (auto a : ptBlack) {
					b->vec.push_back(a);

					int rec;
					int h = inHueristic(a,currentDepth);
					if (h == -1000000) {
						rec = recursion(b, currentDepth + 1, MaxDepth, !turn, alpha, beta);
						//std::cout << rec << std::endl;
						vec.push_back(std::tuple<int64_t, int, int>(hash(a), rec, currentDepth));
						map.insert(std::pair<int64_t, int>(hash(a), rec));
					}
					else rec = h;
					//rec = recursion(b, currentDepth + 1, MaxDepth, !turn, alpha, beta);
					//	if (h != -1000000)
						//	std::cout << "h: " << h << " rec: " << rec << std::endl;

					value = std::min(value, rec);
					beta = std::min((float)value, beta);
					b->vec.pop_back();
					if (alpha >= beta)
						break;

				}
				for (int i = 0; i < ptBlack.size(); i++) {
					delete[] ptBlack[i];
				}
				for (int i = 0; i < ptWhite.size(); i++) {
					delete[] ptWhite[i];
				}

				ptBlack.clear();
				
				return value;

			}

		}

		if (turn == b->white) {
			if (b->white == b->aiTurn) {
			//	std::sort(ptWhite.begin(), ptWhite.end(), compareMax);
				//b->vec.push_back(ptWhite[0]);
				int value = -10000; //recursion(b, currentDepth + 1, MaxDepth, !turn, alpha, beta);
				//b->vec.pop_back();

				for (auto &a : ptWhite) {
					b->vec.push_back(a);

					int rec; 
					int h = inHueristic(a,currentDepth);
					if (h == -1000000) {
						rec = recursion(b, currentDepth + 1, MaxDepth, !turn, alpha, beta);
						//std::cout << rec << std::endl;
						vec.push_back(std::tuple<int64_t, int, int>(hash(a), rec, currentDepth));
						map.insert(std::pair<int64_t, int>(hash(a), rec));
					}
					else rec = h;
					//rec = recursion(b, currentDepth + 1, MaxDepth, !turn, alpha, beta);

					value = std::max(value, rec);
					alpha = std::max((float)value, alpha);

					b->vec.pop_back();
					if (alpha >= beta)
						break;
				}
				for (int i = 0; i < ptBlack.size(); i++) {
					delete[] ptBlack[i];
				}
				for (int i = 0; i < ptWhite.size(); i++) {
					delete[] ptWhite[i];
				}
				
				ptWhite.clear();
				return value;
			}
			else {
				//std::sort(ptWhite.begin(), ptWhite.end(), compareMin);

				//b->vec.push_back(ptWhite[0]);
				int value = 10000;//recursion(b, currentDepth + 1, MaxDepth, !turn, alpha, beta);
				//b->vec.pop_back();

				for (auto a : ptWhite) {
					b->vec.push_back(a);

					int rec;
					int h = inHueristic(a,currentDepth);
					if (h == -1000000) {
						rec = recursion(b, currentDepth + 1, MaxDepth, !turn, alpha, beta);
						//std::cout << rec << std::endl;
						vec.push_back(std::tuple<int64_t, int, int>(hash(a), rec, currentDepth));
						map.insert(std::pair<int64_t, int>(hash(a), rec));
					}
					else rec = h;


					//rec = recursion(b, currentDepth + 1, MaxDepth, !turn, alpha, beta);


					value = std::min(value, rec);
					beta = std::min((float)value, beta);
					b->vec.pop_back();
					if (alpha >= beta)
						break;
				}
				for (int i = 0; i < ptBlack.size(); i++) {
					delete[] ptBlack[i];
				}
				for (int i = 0; i < ptWhite.size(); i++) {
					delete[] ptWhite[i];
				}
				
				ptWhite.clear();
				return value;

			}

		}





	}



}

bool areEqual(pieceType* p1, pieceType* p2) {
	//bool r = true;
	for (int i = 0; i < 64; i++) {
		int p = (int)p1[i];
		int c = (int)p2[i];
		if (c != p)
			return false;

	}
	return true;
}


int ai::inHueristic(pieceType* p, int depth) {

	//int64_t ha = hash(p);
	//if (map.count(ha))
	//{
	//	std::map<int64_t, int>::iterator val = map.find(hash(p));
	//	return val->second;
	//}
	//return -1000000;
	for (auto& a : vec) {
		if (a._Myfirst._Val == hash(p) && a._Get_rest()._Get_rest()._Myfirst._Val == depth) {
			int result;
			result = a._Get_rest()._Myfirst._Val;
		//	std::cout << result;
			return result;
			//std::cout << "resut: " << result << "\n" << "turn :" << a._Get_rest()._Get_rest()._Get_rest()._Myfirst._Val;
			//return a._Get_rest()._Get_rest()._Myfirst._Val;
		}


		//}
		//return result;
	}
	return -1000000;
}






class chess : public olc::PixelGameEngine {
	board b;
	virtual bool OnUserUpdate(float fElapsedTime) override {
		Clear({ 0, 0, 0 });
		b.Draw(this);
		return true;
	}
	virtual bool OnUserCreate() override {

		return true;
	}

};


int main() {
	chess c;
	c.Construct(256 * 1, 230 * 1, 4, 4);
	c.Start();



}