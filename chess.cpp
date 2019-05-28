#include "chess.h"
#include <iostream>
#include <typeinfo>
#include <sstream>
#include <random>
#include <ctime>


ChessBoard::ChessBoard(){
    this->state = Matris<ChessPiece*>(8);
}

ChessBoard::ChessBoard(bool createBoard){

    this->state = Matris<ChessPiece*>(8);

    Rook *rook = new Rook(0,0,false,this); state(0,0) = rook; this->black.push_back(rook);
    Knight *knight = new Knight(1,0,false,this); state(0,1) = knight; this->black.push_back(knight);
    Bishop *bishop = new Bishop(2,0,false,this); state(0,2) = bishop; this->black.push_back(bishop);
    Queen *queen = new Queen(3,0,false,this); state(0,3) = queen; this->black.push_back(queen);
    King *king = new King(4,0,false,this); state(0,4) = king; this->black.push_back(king);
    Bishop *bishop2 = new Bishop(5,0,false,this); state(0,5) = bishop2; this->black.push_back(bishop2);
    Knight *knight2 = new Knight(6,0,false,this); state(0,6) = knight2; this->black.push_back(knight2);
    Rook *rook2 = new Rook(7,0,false,this); state(0,7) = rook2; this->black.push_back(rook2);

    rook = new Rook(0,7,true,this); state(7,0) = rook; this->white.push_back(rook);
    knight = new Knight(1,7,true,this); state(7,1) = knight; this->white.push_back(knight);
    bishop = new Bishop(2,7,true,this); state(7,2) = bishop; this->white.push_back(bishop);
    queen = new Queen(3,7,true,this); state(7,3) = queen; this->white.push_back(queen);
    king = new King(4,7,true,this); state(7,4) = king; this->white.push_back(king);
    bishop2 = new Bishop(5,7,true,this); state(7,5) = bishop2; this->white.push_back(bishop2);
    knight2 = new Knight(6,7,true,this); state(7,6) = knight2; this->white.push_back(knight2);
    rook2 = new Rook(7,7,true,this); state(7,7) = rook2; this->white.push_back(rook2);


    for (int i = 0; i < 8; i++) {
       Pawn *pawn = new Pawn(i,1,false,this); state(1,i) = pawn; this->black.push_back(pawn);
       pawn = new Pawn(i,6,true,this); state(6,i) = pawn; this->white.push_back(pawn);
    }
}

void ChessBoard::move_piece(ChessMove chessMove){

    if (this->state(chessMove.to_y,chessMove.to_x) != nullptr) {

        if (this->state(chessMove.to_y,chessMove.to_x)->isWhite) {

            for (unsigned int i = 0; i < this->white.size(); i++) {
                 if (white[i] == this->state(chessMove.to_y,chessMove.to_x)) {
                     white.erase(white.begin()+i);
                     break;
                 }
            }
        }else{
            for (unsigned int i = 0; i < this->black.size(); i++) {
                 if (black[i] == this->state(chessMove.to_y,chessMove.to_x)) {
                     black.erase(black.begin()+i);
                     break;
                 }
            }
        }
    }

    this->state(chessMove.to_y,chessMove.to_x) = chessMove.piece;
    this->state(chessMove.from_y,chessMove.from_x) = nullptr;

    chessMove.piece->x = chessMove.to_x;
    chessMove.piece->y = chessMove.to_y;

}

vector<ChessMove> ChessBoard::capturingMoves(bool isWhite){
    vector<ChessMove> vec;
    if (isWhite) {
        for (ChessPiece* piece : this->white){
            vector<ChessMove> temp = piece->capturingMoves();
            vec.insert(vec.end(), temp.begin(),temp.end());
        }
    }else{
        for (ChessPiece* piece : this->black){
            vector<ChessMove> temp = piece->capturingMoves();
            vec.insert(vec.end(), temp.begin(),temp.end());
        }
    }
    return vec;
}
vector<ChessMove> ChessBoard::nonCapturingMoves(bool isWhite){

    vector<ChessMove> vec;
    if (isWhite) {
        for (ChessPiece* piece : this->white){
            vector<ChessMove> temp = piece->nonCapturingMoves();
            vec.insert(vec.end(), temp.begin(),temp.end());
        }
    }else{
        for (ChessPiece* piece : this->black){
            vector<ChessMove> temp = piece->nonCapturingMoves();
            vec.insert(vec.end(), temp.begin(),temp.end());
        }
    }
    return vec;
}

ChessBoard & operator>>(istream & in, ChessBoard & board){

    std::string str;

    for (int i = 0; i < 8; i++) {
        in >> str;
        for (int j = 0; j < 8; j++) {

            if (str[j] == 'Q') {
                Queen *queen = new Queen(j,i,true,&board); board.state(i,j) = queen; board.white.push_back(queen);
            }else if (str[j] == 'q'){
                Queen *queen = new Queen(j,i,false,&board); board.state(i,j) = queen; board.black.push_back(queen);
            }else if (str[j] == 'R'){
                Rook *rook = new Rook(j,i,true,&board); board.state(i,j) = rook; board.white.push_back(rook);
            }else if (str[j] == 'r'){
                Rook *rook = new Rook(j,i,false,&board); board.state(i,j) = rook; board.black.push_back(rook);
            }else if (str[j] == 'N'){
                Knight *knight = new Knight(j,i,true,&board); board.state(i,j) = knight; board.white.push_back(knight);
            }else if (str[j] == 'n'){
                Knight *knight = new Knight(j,i,false,&board); board.state(i,j) = knight; board.black.push_back(knight);
            }else if (str[j] == 'K'){
                King *king = new King(j,i,true,&board); board.state(i,j) = king; board.white.push_back(king);
            }else if (str[j] == 'k'){
                King *king = new King(j,i,false,&board); board.state(i,j) = king; board.black.push_back(king);
            }else if (str[j] == 'B'){
                Bishop *bishop = new Bishop(j,i,true,&board); board.state(i,j) = bishop; board.white.push_back(bishop);
            }else if (str[j] == 'b'){
                Bishop *bishop = new Bishop(j,i,false,&board); board.state(i,j) = bishop; board.black.push_back(bishop);
            }else if (str[j] == 'P'){
                Pawn *pawn = new Pawn(j,i,true,&board); board.state(i,j) = pawn; board.white.push_back(pawn);
            }else if (str[j] == 'p'){
                Pawn *pawn = new Pawn(j,i,false,&board); board.state(i,j) = pawn; board.black.push_back(pawn);
            }
        }
    }
    return board;
}
ostream & operator<<(ostream & out, ChessBoard & board){
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            if (board.get(x,y) != nullptr) {
                out << board.get(x,y)->utfRepresentation();
            }else{
                out << ".";
            }
        }
        out <<'\n';
    }
    return out;
}

void AI1::promotion(ChessBoard & board, ChessPiece * chess_piece){

    vector<ChessPiece*> pieces = {new Queen(chess_piece->x,chess_piece->y,isWhite,&board),
     new King(chess_piece->x,chess_piece->y,isWhite,&board),
     new Rook(chess_piece->x,chess_piece->y,isWhite,&board),
     new Bishop(chess_piece->x,chess_piece->y,isWhite,&board),
     new Knight(chess_piece->x,chess_piece->y,isWhite,&board)};

     ChessPiece * piece = pieces[rand() % pieces.size()];

     if (!isWhite && chess_piece->isPawn() && chess_piece->y == 6) {

         for (unsigned int i = 0; i < board.black.size(); i++) {
             if (board.black[i] == chess_piece) {
                 board.black[i] = piece;
                 board.state(chess_piece->y,chess_piece->x) = piece;
                 break;
             }
         }

     }else if (isWhite && chess_piece->isPawn() && chess_piece->y == 1){
         for (unsigned int i = 0; i < board.white.size(); i++) {
             if (board.white[i] == chess_piece) {
                 board.white[i] = piece;
                 board.state(chess_piece->y,chess_piece->x) = piece;
                 break;
             }
         }
     }

}

void AI1::move(ChessBoard & board){

    if (board.capturingMoves(isWhite).size() == 0 && board.nonCapturingMoves(isWhite).size() != 0) {

        vector<ChessMove> nonC = board.nonCapturingMoves(isWhite);
        int ran = rand() % nonC.size();
        board.move_piece(nonC[ran]);
        promotion(board,nonC[ran].piece);

    }else if (board.nonCapturingMoves(isWhite).size() != 0){
        vector<ChessMove> c = board.capturingMoves(isWhite);
        int ran = rand() % c.size();
        board.move_piece(c[ran]);
        promotion(board,c[ran].piece);
    }
}

void AI2::promotion(ChessBoard & board, ChessPiece * chess_piece){

    stringstream s;
    ChessBoard c;
    s << board;
    s >> c;
    bool hasPromoted = false;
    vector<ChessPiece*> pieces = {new Queen(chess_piece->x,chess_piece->y,isWhite,&c),
     new King(chess_piece->x,chess_piece->y,isWhite,&c),
     new Rook(chess_piece->x,chess_piece->y,isWhite,&c),
     new Bishop(chess_piece->x,chess_piece->y,isWhite,&c),
     new Knight(chess_piece->x,chess_piece->y,isWhite,&c)};

     ChessPiece * piece;



    if (!isWhite && chess_piece->isPawn() && chess_piece->y == 6) {

        std::cout << "/ message /" << '\n';


        for (ChessPiece * tmp : pieces) {
            c.state(chess_piece->y,chess_piece->x) = tmp;

            if (tmp->capturingMoves().size() == 0) {
                hasPromoted = true;
                piece = tmp;
                break;
            }
            s << board;
            s >> c;
        }

        if (!hasPromoted){
            piece = pieces[rand() % pieces.size()];
        }

        piece->board = &board;

        for (unsigned int i = 0; i < board.black.size(); i++) {
            if (board.black[i] == chess_piece) {
                board.black[i] = piece;
                board.state(chess_piece->y,chess_piece->x) = piece;
                break;
            }
        }

    }else if (isWhite && chess_piece->isPawn() && chess_piece->y == 1){

        for (ChessPiece * tmp : pieces) {
            c.state(chess_piece->y,chess_piece->x) = tmp;
            if (tmp->capturingMoves().size() == 0) {
                hasPromoted = true;
                piece = tmp;
                break;
            }
            s << board;
            s >> c;
        }

        if (!hasPromoted){
            piece = pieces[rand() % pieces.size()];
        }

        piece->board = &board;

        for (unsigned int i = 0; i < board.white.size(); i++) {
            if (board.white[i] == chess_piece) {
                board.white[i] = piece;
                board.state(chess_piece->y,chess_piece->x) = piece;
                break;
            }
        }

    }


}

void AI2::move(ChessBoard & board){

    if (board.black.size() > 0) {
        stringstream s;
        ChessBoard c;

        bool hasMoved = false;

        s << board;
        s >> c;

        if (board.capturingMoves(isWhite).size() == 0 && board.nonCapturingMoves(isWhite).size() != 0) {

            vector<ChessMove> moves = c.nonCapturingMoves(isWhite);

            for (unsigned int i = 0; i < moves.size(); i++) {

                c.move_piece(moves[i]);

                if (c.capturingMoves(!isWhite).size() != 0) {
                    ChessMove finalMove = board.nonCapturingMoves(isWhite)[i];
                    board.move_piece(finalMove);
                    promotion(board,finalMove.piece);
                    hasMoved = true;
                    break;
                }
                s << board;
                s >> c;
            }

            if (!hasMoved) {
                vector<ChessMove> nonC = board.nonCapturingMoves(isWhite);
                int ran = rand() % nonC.size();
                board.move_piece(nonC[ran]);
                promotion(board,nonC[ran].piece);
            }

        }else if (board.capturingMoves(isWhite).size() != 0){

            vector<ChessMove> moves = c.capturingMoves(isWhite);

            for (unsigned int i = 0; i < moves.size(); i++) {

                c.move_piece(moves[i]);

                if (c.capturingMoves(!isWhite).size() != 0) {
                    ChessMove finalMove = board.capturingMoves(isWhite)[i];
                    board.move_piece(finalMove);
                    promotion(board,finalMove.piece);
                    hasMoved = true;
                    break;
                }
                s << board;
                s >> c;
            }

            if (!hasMoved) {
                vector<ChessMove> c = board.capturingMoves(isWhite);
                int ran = rand() % c.size();
                board.move_piece(c[ran]);
                promotion(board,c[ran].piece);

            }

        }
    }

}


using std::vector; using std::stringstream; using std::cout; using std::endl;

int main(int argc, char const *argv[]) {

    bool starting;
    int ai;
    std::cout << "Is white starting?(0/1)" << '\n';
    std::cin >> starting;
    std::cout << "Which kind of AI is starting?(1/2)" << '\n';
    std::cin >> ai;

    AI1 a1;

    AI2 a2;

    if (ai == 1) {
        a1.isWhite = starting;
        a2.isWhite = !starting;
    }else{
        a1.isWhite = !starting;
        a2.isWhite = starting;
    }

    //for (int i = 0; i < 100; i++) {

            srand ( time(NULL) );//

            ChessBoard board;

            stringstream s;
            s << "rnbqkbnr" << endl;
            s << "pppppppp" << endl;
            s << "........" << endl;
            s << "........" << endl;
            s << "........" << endl;
            s << "........" << endl;
            s << "PPPPPPPP" << endl;
            s << "RNBQKBNR";
            s >> board;


            //b.move_piece(ChessMove(1,1,1,6,b.black[8]));

            //std::cout << board << '\n';

            while (board.black.size() != 0 && board.white.size() != 0) {

                if (ai == 1) {
                    a1.move(board);
                    std::cout << board << '\n';
                    a2.move(board);
                    std::cout << board << '\n';
                }else{
                    a2.move(board);
                    std::cout << board << '\n';
                    a1.move(board);
                    std::cout << board << '\n';
                }

            }

            if (board.black.size() == 0) {
                std::cout << "Black wins!" << '\n';
                //b++;
            }else{
                std::cout << "White wins!" << '\n';
                //w++;
            }
    //}

    //std::cout << b << '\n';
    //std::cout << w << '\n';


    return 0;
}
