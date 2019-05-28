#ifndef CHESS_H

#include <iostream>
#include <math.h>
#include <vector>
#include "matrix.h"
#include <sstream>


class ChessPiece;

struct ChessMove {
    int from_x;
    int from_y;
    int to_x;
    int to_y;

    ChessPiece* piece;   // you can change the position of the chess piece with this pointer.

    ChessMove(int fromX, int fromY, int toX, int toY, ChessPiece * c){
        this->from_x = fromX;
        this->from_y = fromY;
        this->to_x = toX;
        this->to_y = toY;
        this->piece = c;
    }
};

class ChessBoard;

struct AI1{

    bool isWhite;

    AI1(){}

    AI1(bool is_white){
        isWhite = is_white;
    }

    void move(ChessBoard &board);

    void promotion(ChessBoard &board,ChessPiece * chess_piece);
};

struct AI2{

    bool isWhite;

    AI2(){}

    AI2(bool is_white){
        isWhite = is_white;
    }

    void move(ChessBoard &board);

    void promotion(ChessBoard &board, ChessPiece * chess_piece);

};

class ChessBoard {
  // add additional members or functions of your choice
  friend ChessBoard & operator>>(istream & , ChessBoard &);
  friend void AI1::promotion(ChessBoard &,ChessPiece * chess_piece);
  friend void AI2::promotion(ChessBoard &,ChessPiece * chess_piece);
   private:

       Matris<ChessPiece*> state; // Matris from lab 4  Matrix

   public:

       vector<ChessPiece*> black;
       vector<ChessPiece*> white;

       ChessBoard();
       ChessBoard(bool);
       ChessBoard(ChessBoard &board);
       void move_piece(ChessMove chessMove);
       vector<ChessMove> capturingMoves(bool isWhite);
       vector<ChessMove> nonCapturingMoves(bool isWhite);

       ~ChessBoard(){}

       ChessPiece * get(int x, int y){
           return this->state(y,x);
       }

};

using std::vector;
using std::istream;
using std::string;

ChessBoard & operator>>(istream & , ChessBoard &);

ostream & operator<<(ostream & , ChessBoard &);



class ChessPiece {
  friend void ChessBoard::move_piece(ChessMove p);
  friend ostream & operator<<(ostream & , ChessBoard &);
  friend void AI1::promotion(ChessBoard &,ChessPiece * chess_piece);
  friend void AI2::promotion(ChessBoard &,ChessPiece * chess_piece);

  protected:
      int x,y;
      bool isWhite;
      ChessBoard* board;
      /**
       * Returns 0 if target square is unreachable.
       * Returns 1 if target square is reachable and empty.
       * Returns 2 if move captures a piece.
       */
      virtual int validMove(int to_x, int to_y){return 0;}
      virtual std::string utfRepresentation(){return ".";}
      virtual char latin1Representation(){return '.';}

      virtual bool isPawn(){return false;}
  public:
      ChessPiece(int x_pos, int y_pos, bool is_white, ChessBoard *chess_board){
          this->x = x_pos;
          this->y = y_pos;
          this->isWhite = is_white;
          this->board = chess_board;
      }



      /**
       * Checks if this move is valid for this piece and captures
       * a piece of the opposite color.
       */
      bool capturingMove(int to_x, int to_y){
          if (validMove(to_x, to_y) == 2 && (this->board->get(to_x,to_y)->isWhite != this->isWhite)) {
              return true;
          }else{
              return false;
          }
      }
      /**
       * Checks if this move is valid but does not capture a piece.
       */
      bool nonCapturingMove(int to_x, int to_y){
          if (validMove(to_x, to_y) == 1) {
              return true;
          }else{
              return false;
          }
      }
      virtual vector<ChessMove> capturingMoves(){return {};}
      virtual vector<ChessMove> nonCapturingMoves(){return {};}

      /**
      * For testing multiple inheritance
      */
      int unnecessaryInt;
  };

class King : public ChessPiece{
protected:
    int validMove(int to_x, int to_y){
        int delta_x = abs(to_x-this->x);
        int delta_y = abs(to_y-this->y);

        if (delta_x*delta_y == 1 || delta_x+delta_y == 1) {
            if (this->board->get(to_x,to_y) == nullptr) {
                return 1;
            }else{
                return 2;
            }
        }else{
            return 0;
        }
    }

public:

    King(int x_pos, int y_pos, bool is_white, ChessBoard *chess_board) :
    ChessPiece(x_pos, y_pos, is_white, chess_board) {}


    char latin1Representation(){
        if (this->isWhite) {
            return 'K';
        }else{
            return 'k';
        }
    }

    std::string utfRepresentation(){
        if (this->isWhite) {
            return "♔";
        }else{
            return "♚";
        }

    }

    vector<ChessMove> capturingMoves(){

        vector<ChessMove> vec;

        vector<int> moves = {0,-1,1};

        for (int x_move : moves) {
            for (int y_move : moves){
                if (this->x+x_move >= 0 && this->x+x_move < 8 && this->y+y_move >= 0 && this->y+y_move < 8) {
                    if (capturingMove(this->x+x_move,this->y+y_move)){
                        vec.push_back(ChessMove(this->x,this->y,this->x+x_move,this->y+y_move,this));
                    }
                }
            }
        }


        return vec;

    }

    vector<ChessMove> nonCapturingMoves(){

        vector<ChessMove> vec;

        vector<int> moves = {0,-1,1};

        for (int x_move : moves) {
            for (int y_move : moves){
                if (this->x+x_move >= 0 && this->x+x_move < 8 && this->y+y_move >= 0 && this->y+y_move < 8) {
                    if (nonCapturingMove(this->x+x_move,this->y+y_move)){
                        vec.push_back(ChessMove(this->x,this->y,this->x+x_move,this->y+y_move,this));
                    }
                }

            }
        }

        return vec;

    }


};

class Knight : public ChessPiece{

protected:

    int validMove(int to_x, int to_y){
        int delta_x = abs(to_x-this->x);
        int delta_y = abs(to_y-this->y);

        if (pow(delta_x,2)+pow(delta_y,2) == 5) {
            if (this->board->get(to_x,to_y) == nullptr) {
                return 1;
            }else{
                return 2;
            }
        }else{
            return 0;
        }
    }

public:

    Knight(int x_pos, int y_pos, bool is_white, ChessBoard *chess_board) :
    ChessPiece(x_pos, y_pos, is_white, chess_board) {}

    char latin1Representation(){
        if (this->isWhite) {
            return 'N';
        }else{
            return 'n';
        }
    }

    std::string utfRepresentation(){
        if (this->isWhite) {
            return "♘";
        }else{
            return "♞";
        }

    }

    vector<ChessMove> capturingMoves(){

        vector<ChessMove> vec;

        vector<int> moves = {1,2,-1,-2};

        for (int x_move : moves) {
            for (int y_move : moves){
                if (this->x+x_move >= 0 && this->x+x_move < 8 && this->y+y_move >= 0 && this->y+y_move < 8) {
                    if (capturingMove(this->x+x_move,this->y+y_move)){
                        vec.push_back(ChessMove(this->x,this->y,this->x+x_move,this->y+y_move,this));
                    }
                }
            }
        }


        return vec;

    }

    vector<ChessMove> nonCapturingMoves(){

        vector<ChessMove> vec;

        vector<int> moves = {1,2,-1,-2};

        for (int x_move : moves) {
            for (int y_move : moves){
                if (this->x+x_move >= 0 && this->x+x_move < 8 && this->y+y_move >= 0 && this->y+y_move < 8) {
                    if (nonCapturingMove(this->x+x_move,this->y+y_move)){
                        vec.push_back(ChessMove(this->x,this->y,this->x+x_move,this->y+y_move,this));
                    }
                }

            }
        }

        return vec;

    }

};

class Pawn : public ChessPiece{

protected:

    bool isPawn(){return true;}

    int validMove(int to_x, int to_y){
        int delta_x = abs(to_x-this->x);
        int delta_y;

        if (this->isWhite) {
            delta_y = this->y-to_y;
        }else{
            delta_y = to_y-this->y;
        }

        if (delta_y == 1 && delta_x == 1) {
            if (this->board->get(to_x,to_y) != nullptr) {
                return 2;
            }
            else{
                return 0;
            }
        }

        if (((this->isWhite && this->y == 6) || (!this->isWhite && this->y == 1)) && delta_x == 0) {
            if ((delta_y == 2 || delta_y == 1) && this->board->get(to_x,to_y) == nullptr) {
                return 1;
            }else{
                return 0;
            }
        }else{
            if (delta_y == 1 && delta_x == 0 && this->board->get(to_x,to_y) == nullptr) {
                return 1;
            }else{
                return 0;
            }
        }
    }

public:

    Pawn(int x_pos, int y_pos, bool is_white, ChessBoard *chess_board) :
    ChessPiece(x_pos, y_pos, is_white, chess_board) {}


    char latin1Representation(){
        if (this->isWhite) {
            return 'P';
        }else{
            return 'p';
        }
    }

    std::string utfRepresentation(){
        if (this->isWhite) {
            return "♙";
        }else{
            return "♟";
        }

    }

    vector<ChessMove> capturingMoves(){

        vector<ChessMove> vec;
        vector<int> moves;

        if (!this->isWhite) {
            moves = {-1,1};
        }
        else{
            moves = {1,-1};
        }

        if (this->x+moves[0] >= 0 && this->x+moves[0] < 8 && this->y+moves[1] >= 0 && this->y+moves[1] < 8) {
            if (capturingMove(this->x+moves[0],this->y+moves[1])){
                vec.push_back(ChessMove(this->x,this->y,this->x+moves[0],this->y+moves[1],this));

            }
        }

        if (this->x+moves[1] >= 0 && this->x+moves[1] < 8 && this->y+moves[1] >= 0 && this->y+moves[1] < 8) {
            if (capturingMove(this->x+moves[1],this->y+moves[1])){
                vec.push_back(ChessMove(this->x,this->y,this->x+moves[1],this->y+moves[1],this));

            }
        }

        return vec;

    }

    vector<ChessMove> nonCapturingMoves(){

        vector<ChessMove> vec;

        vector<int> moves;

        if (this->isWhite) {
            moves = {-1,-2};
        }
        else{
            moves = {1,2};
        }

        if (this->y+moves[0] >= 0 && this->y+moves[0] < 8) {
            if (nonCapturingMove(this->x,this->y+moves[0])){
                vec.push_back(ChessMove(this->x,this->y,this->x,this->y+moves[0],this));

            }
        }

        if (this->y+moves[1] >= 0 && this->y+moves[1] < 8) {
            if (nonCapturingMove(this->x,this->y+moves[1])){
                vec.push_back(ChessMove(this->x,this->y,this->x,this->y+moves[1],this));

            }
        }

        return vec;

    }

};

class Bishop : virtual public ChessPiece{

protected:

    int validMove(int to_x, int to_y){
        int delta_x = abs(to_x-this->x);
        int delta_y = abs(to_y-this->y);

        if (delta_x == delta_y) {
            if (this->board->get(to_x,to_y) == nullptr) {
                return 1;
            }else{
                return 2;
            }
        }else{
            return 0;
        }
    }

public:

    Bishop(int x_pos, int y_pos, bool is_white, ChessBoard *chess_board) :
    ChessPiece(x_pos, y_pos, is_white, chess_board) {}


    char latin1Representation(){
        if (this->isWhite) {
            return 'B';
        }else{
            return 'b';
        }
    }

    std::string utfRepresentation(){
        if (this->isWhite) {
            return "♗";
        }else{
            return "♝";
        }

    }

    vector<ChessMove> capturingMoves(){

        vector<ChessMove> vec;

        int i = 1;

        while (this->x+i < 8 && this->y+i < 8){
            if (this->board->get(this->x+i,this->y+i) != nullptr) {
                if (capturingMove(this->x+i,this->y+i)){
                    vec.push_back(ChessMove(this->x,this->y,this->x+i,this->y+i,this));
                }
                break;
            }
            i++;
        }

        i = 1;

        while (this->x+i < 8 && this->y-i >= 0){
            if (this->board->get(this->x+i,this->y-i) != nullptr) {
                if (capturingMove(this->x+i,this->y-i)){
                    vec.push_back(ChessMove(this->x,this->y,this->x+i,this->y-i,this));
                }
                break;
            }

            i++;
        }

        i = 1;

        while (this->x-i >= 0 && this->y+i < 8){
            if (this->board->get(this->x-i,this->y+i) != nullptr) {
                if (capturingMove(this->x-i,this->y+i)){
                    vec.push_back(ChessMove(this->x,this->y,this->x-i,this->y+i,this));
                }
                break;
            }

            i++;
        }

        i = 1;

        while (this->x-i >= 0 && this->y-i >= 0){
            if (this->board->get(this->x-i,this->y-i) != nullptr) {
                if (capturingMove(this->x-i,this->y-i)){
                    vec.push_back(ChessMove(this->x,this->y,this->x-i,this->y-i,this));
                }
                break;
            }
            i++;
        }


        return vec;

    }

    vector<ChessMove> nonCapturingMoves(){

        vector<ChessMove> vec;

        int i = 1;

        while (this->board->get(this->x+i,this->y+i) == nullptr && (this->x+i < 8 && this->y+i < 8)){
            if (nonCapturingMove(this->x+i,this->y+i)){
                vec.push_back(ChessMove(this->x,this->y,this->x+i,this->y+i,this));
            }
            i++;
        }

        i = 1;

        while (this->board->get(this->x+i,this->y-i) == nullptr && (this->x+i < 8 && this->y-i >= 0)){
            if (nonCapturingMove(this->x+i,this->y-i)){
                vec.push_back(ChessMove(this->x,this->y,this->x+i,this->y-i,this));
            }
            i++;
        }

        i = 1;

        while (this->board->get(this->x-i,this->y+i) == nullptr && (this->x-i >= 0 && this->y+i < 8)){
            if (nonCapturingMove(this->x-i,this->y+i)){
                vec.push_back(ChessMove(this->x,this->y,this->x-i,this->y+i,this));
            }
            i++;
        }

        i = 1;

        while (this->board->get(this->x-i,this->y-i) == nullptr && (this->x-i >= 0 && this->y-i >= 0)){
            if (nonCapturingMove(this->x-i,this->y-i)){
                vec.push_back(ChessMove(this->x,this->y,this->x-i,this->y-i,this));
            }
            i++;
        }



        return vec;

    }

};

class Rook : virtual public ChessPiece{

protected:

    int validMove(int to_x, int to_y){
        int delta_x = abs(to_x-this->x);
        int delta_y = abs(to_y-this->y);

        if ((delta_x == 0 && delta_y != 0) || (delta_y == 0 && delta_x != 0)) {
            if (this->board->get(to_x,to_y) == nullptr) {
                return 1;
            }else{
                return 2;
            }
        }else{
            return 0;
        }
    }

public:

    Rook(int x_pos, int y_pos, bool is_white, ChessBoard *chess_board) :
    ChessPiece(x_pos, y_pos, is_white, chess_board) {}


    char latin1Representation(){
        if (this->isWhite) {
            return 'R';
        }else{
            return 'r';
        }
    }

    std::string utfRepresentation(){
        if (this->isWhite) {
            return "♖";
        }else{
            return "♜";
        }

    }

    vector<ChessMove> capturingMoves(){

        vector<ChessMove> vec;

        int i = 1;

        while (this->x+i < 8){
            if (this->board->get(this->x+i,this->y) != nullptr) {
                if (capturingMove(this->x+i,this->y)){
                    vec.push_back(ChessMove(this->x,this->y,this->x+i,this->y,this));
                }
                break;
            }
            i++;
        }

        i = 1;

        while (this->y-i >= 0){
            if (this->board->get(this->x,this->y-i) != nullptr) {
                if (capturingMove(this->x,this->y-i)){
                    vec.push_back(ChessMove(this->x,this->y,this->x,this->y-i,this));
                }
                break;
            }

            i++;
        }

        i = 1;

        while (this->x-i >= 0){
            if (this->board->get(this->x-i,this->y) != nullptr) {
                if (capturingMove(this->x-i,this->y)){
                    vec.push_back(ChessMove(this->x,this->y,this->x-i,this->y,this));
                }
                break;
            }

            i++;
        }

        i = 1;

        while (this->y+i < 8){
            if (this->board->get(this->x,this->y+i) != nullptr) {
                if (capturingMove(this->x,this->y+i)){
                    vec.push_back(ChessMove(this->x,this->y,this->x,this->y+i,this));
                }
                break;
            }
            i++;
        }

        return vec;

    }

    vector<ChessMove> nonCapturingMoves(){

        vector<ChessMove> vec;

        int i = 1;

        while (this->board->get(this->x+i,this->y) == nullptr && (this->x+i < 8)){
            if (nonCapturingMove(this->x+i,this->y)){
                vec.push_back(ChessMove(this->x,this->y,this->x+i,this->y,this));
            }
            i++;
        }

        i = 1;

        while (this->board->get(this->x,this->y-i) == nullptr && (this->y-i >= 0)){
            if (nonCapturingMove(this->x,this->y-i)){
                vec.push_back(ChessMove(this->x,this->y,this->x,this->y-i,this));
            }
            i++;
        }

        i = 1;

        while (this->board->get(this->x-i,this->y) == nullptr && (this->x-i >= 0)){
            if (nonCapturingMove(this->x-i,this->y)){
                vec.push_back(ChessMove(this->x,this->y,this->x-i,this->y,this));
            }
            i++;
        }

        i = 1;

        while (this->board->get(this->x,this->y+i) == nullptr && (this->y+i < 8)){
            if (nonCapturingMove(this->x,this->y+i)){
                vec.push_back(ChessMove(this->x,this->y,this->x,this->y+i,this));
            }
            i++;
        }

        return vec;

    }
};

class Queen : public Bishop, public Rook{

protected:

    int validMove(int to_x,int to_y){
        if (Bishop::validMove(to_x,to_y) == 2 || Rook::validMove(to_x,to_y) == 2) {
            return 2;
        }else if (Bishop::validMove(to_x,to_y) == 1 || Rook::validMove(to_x,to_y) == 1){
            return 1;
        }else{
            return 0;
        }
    }

public:

    Queen(int x_pos, int y_pos, bool is_white, ChessBoard *chess_board) :
    ChessPiece(x_pos, y_pos, is_white, chess_board),
    Bishop(x_pos, y_pos, is_white, chess_board),
    Rook(x_pos, y_pos, is_white, chess_board){}

    char latin1Representation(){
        if (this->isWhite) {
            return 'Q';
        }else{
            return 'q';
        }
    }

    std::string utfRepresentation(){
        if (this->isWhite) {
            return "♕";
        }else{
            return "♛";
        }

    }

    vector<ChessMove> capturingMoves(){

        vector<ChessMove> vec;

        vector<ChessMove> temp;

        temp = Bishop::capturingMoves();

        vec.insert(vec.end(), temp.begin(),temp.end());

        temp = Rook::capturingMoves();

        vec.insert(vec.end(), temp.begin(),temp.end());

        return vec;

    }

    vector<ChessMove> nonCapturingMoves(){

        vector<ChessMove> vec;

        vector<ChessMove> temp;

        temp = Bishop::nonCapturingMoves();

        vec.insert(vec.end(), temp.begin(),temp.end());

        temp = Rook::nonCapturingMoves();

        vec.insert(vec.end(), temp.begin(),temp.end());

        return vec;

    }

};





#endif
