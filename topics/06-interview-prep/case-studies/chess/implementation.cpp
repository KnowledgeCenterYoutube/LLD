#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <mutex>
#include <stdexcept>

using namespace std;

// Enums
enum class Color { WHITE, BLACK };
enum class PieceType { PAWN, ROOK, KNIGHT, BISHOP, QUEEN, KING };
enum class GameState { ACTIVE, CHECK, CHECKMATE, STALEMATE, DRAW };

// Forward declarations
class Piece;
class Board;
class GameState;

// Position class implementation
class Position {
private:
    int row;
    int col;

public:
    Position(int row, int col) : row(row), col(col) {}
    
    Position(const string& notation) {
        if (notation.length() != 2) {
            throw invalid_argument("Invalid position notation");
        }
        col = notation[0] - 'a';
        row = 8 - (notation[1] - '0');
    }
    
    int getRow() const { return row; }
    int getCol() const { return col; }
    
    string getNotation() const {
        string notation;
        notation += 'a' + col;
        notation += '0' + (8 - row);
        return notation;
    }
    
    bool isValid() const {
        return row >= 0 && row < 8 && col >= 0 && col < 8;
    }
    
    bool operator==(const Position& other) const {
        return row == other.row && col == other.col;
    }
};

// Move class implementation
class Move {
private:
    Position from;
    Position to;
    Piece* piece;
    Piece* capturedPiece;
    bool isCastling;
    bool isEnPassant;
    bool isPromotion;
    PieceType promotionType;

public:
    Move(const Position& from, const Position& to, Piece* piece)
        : from(from), to(to), piece(piece), capturedPiece(nullptr),
          isCastling(false), isEnPassant(false), isPromotion(false) {}
    
    void setCapturedPiece(Piece* piece) { capturedPiece = piece; }
    void setCastling(bool value) { isCastling = value; }
    void setEnPassant(bool value) { isEnPassant = value; }
    void setPromotion(bool value, PieceType type) {
        isPromotion = value;
        promotionType = type;
    }
    
    Position getFrom() const { return from; }
    Position getTo() const { return to; }
    Piece* getPiece() const { return piece; }
    Piece* getCapturedPiece() const { return capturedPiece; }
    bool getIsCastling() const { return isCastling; }
    bool getIsEnPassant() const { return isEnPassant; }
    bool getIsPromotion() const { return isPromotion; }
    PieceType getPromotionType() const { return promotionType; }
};

// Board class implementation
class Board {
private:
    vector<vector<unique_ptr<Piece>>> squares;
    mutex boardMutex;

public:
    Board() {
        squares.resize(8, vector<unique_ptr<Piece>>(8));
    }
    
    Piece* getPiece(const Position& pos) const {
        if (!pos.isValid()) return nullptr;
        return squares[pos.getRow()][pos.getCol()].get();
    }
    
    void setPiece(const Position& pos, unique_ptr<Piece> piece) {
        if (!pos.isValid()) return;
        squares[pos.getRow()][pos.getCol()] = move(piece);
    }
    
    void movePiece(const Position& from, const Position& to) {
        if (!from.isValid() || !to.isValid()) return;
        squares[to.getRow()][to.getCol()] = move(squares[from.getRow()][from.getCol()]);
    }
    
    bool isPositionValid(const Position& pos) const {
        return pos.isValid();
    }
    
    void clear() {
        for (auto& row : squares) {
            for (auto& piece : row) {
                piece.reset();
            }
        }
    }
};

// Piece class implementation
class Piece {
protected:
    Color color;
    Position position;
    bool hasMoved;

public:
    Piece(Color color, const Position& position)
        : color(color), position(position), hasMoved(false) {}
    
    virtual ~Piece() = default;
    
    Color getColor() const { return color; }
    Position getPosition() const { return position; }
    void setPosition(const Position& pos) { position = pos; }
    bool getHasMoved() const { return hasMoved; }
    void setHasMoved(bool moved) { hasMoved = moved; }
    
    virtual vector<Position> getValidMoves(const Board& board) const = 0;
    virtual bool canMoveTo(const Position& pos, const Board& board) const = 0;
    virtual PieceType getType() const = 0;
};

// Pawn class implementation
class Pawn : public Piece {
public:
    Pawn(Color color, const Position& position) : Piece(color, position) {}
    
    vector<Position> getValidMoves(const Board& board) const override {
        vector<Position> moves;
        int direction = (color == Color::WHITE) ? -1 : 1;
        
        // Forward move
        Position forward(position.getRow() + direction, position.getCol());
        if (forward.isValid() && !board.getPiece(forward)) {
            moves.push_back(forward);
            
            // Double move from starting position
            if (!hasMoved) {
                Position doubleForward(position.getRow() + 2 * direction, position.getCol());
                if (!board.getPiece(doubleForward)) {
                    moves.push_back(doubleForward);
                }
            }
        }
        
        // Captures
        vector<Position> captures = {
            Position(position.getRow() + direction, position.getCol() - 1),
            Position(position.getRow() + direction, position.getCol() + 1)
        };
        
        for (const auto& capture : captures) {
            if (capture.isValid()) {
                Piece* target = board.getPiece(capture);
                if (target && target->getColor() != color) {
                    moves.push_back(capture);
                }
            }
        }
        
        return moves;
    }
    
    bool canMoveTo(const Position& pos, const Board& board) const override {
        vector<Position> validMoves = getValidMoves(board);
        return find(validMoves.begin(), validMoves.end(), pos) != validMoves.end();
    }
    
    PieceType getType() const override { return PieceType::PAWN; }
};

// Similar implementations for other pieces...

// GameState class implementation
class GameState {
private:
    Board board;
    Color currentPlayer;
    vector<Move> moveHistory;
    bool whiteKingMoved;
    bool blackKingMoved;
    bool whiteRookMoved[2];
    bool blackRookMoved[2];
    Position enPassantTarget;
    int halfMoveClock;
    int fullMoveNumber;
    unordered_map<string, int> positionHistory;
    mutex stateMutex;

public:
    GameState() : currentPlayer(Color::WHITE), whiteKingMoved(false),
                 blackKingMoved(false), halfMoveClock(0), fullMoveNumber(1) {
        initializeBoard();
    }
    
    bool makeMove(const Position& from, const Position& to) {
        lock_guard<mutex> lock(stateMutex);
        
        Piece* piece = board.getPiece(from);
        if (!piece || piece->getColor() != currentPlayer) {
            return false;
        }
        
        if (!piece->canMoveTo(to, board)) {
            return false;
        }
        
        // Create move
        Move move(from, to, piece);
        
        // Handle special cases
        if (piece->getType() == PieceType::KING) {
            handleCastling(move);
        } else if (piece->getType() == PieceType::PAWN) {
            handleEnPassant(move);
            handlePromotion(move);
        }
        
        // Execute move
        executeMove(move);
        
        // Update game state
        updateGameState();
        
        return true;
    }
    
    bool isCheck(Color color) const {
        // Find king
        Position kingPos;
        for (int row = 0; row < 8; row++) {
            for (int col = 0; col < 8; col++) {
                Piece* piece = board.getPiece(Position(row, col));
                if (piece && piece->getType() == PieceType::KING && 
                    piece->getColor() == color) {
                    kingPos = Position(row, col);
                    break;
                }
            }
        }
        
        // Check if any opponent piece can capture the king
        Color opponentColor = (color == Color::WHITE) ? Color::BLACK : Color::WHITE;
        for (int row = 0; row < 8; row++) {
            for (int col = 0; col < 8; col++) {
                Piece* piece = board.getPiece(Position(row, col));
                if (piece && piece->getColor() == opponentColor) {
                    if (piece->canMoveTo(kingPos, board)) {
                        return true;
                    }
                }
            }
        }
        
        return false;
    }
    
    bool isCheckmate(Color color) const {
        if (!isCheck(color)) return false;
        
        // Try all possible moves for all pieces
        for (int row = 0; row < 8; row++) {
            for (int col = 0; col < 8; col++) {
                Piece* piece = board.getPiece(Position(row, col));
                if (piece && piece->getColor() == color) {
                    vector<Position> moves = piece->getValidMoves(board);
                    for (const auto& move : moves) {
                        // Try move
                        Board tempBoard = board;
                        tempBoard.movePiece(piece->getPosition(), move);
                        
                        // Check if still in check
                        if (!isCheck(color)) {
                            return false;
                        }
                    }
                }
            }
        }
        
        return true;
    }
    
    bool isStalemate(Color color) const {
        if (isCheck(color)) return false;
        
        // Check if any legal moves exist
        for (int row = 0; row < 8; row++) {
            for (int col = 0; col < 8; col++) {
                Piece* piece = board.getPiece(Position(row, col));
                if (piece && piece->getColor() == color) {
                    vector<Position> moves = piece->getValidMoves(board);
                    if (!moves.empty()) {
                        return false;
                    }
                }
            }
        }
        
        return true;
    }
    
    bool isDraw() const {
        // Check for insufficient material
        if (isInsufficientMaterial()) return true;
        
        // Check for threefold repetition
        if (isThreefoldRepetition()) return true;
        
        // Check for fifty-move rule
        if (halfMoveClock >= 50) return true;
        
        return false;
    }
    
    vector<Position> getValidMoves(const Position& pos) const {
        Piece* piece = board.getPiece(pos);
        if (!piece) return {};
        
        vector<Position> moves = piece->getValidMoves(board);
        vector<Position> validMoves;
        
        for (const auto& move : moves) {
            // Try move
            Board tempBoard = board;
            tempBoard.movePiece(pos, move);
            
            // Check if move is legal
            if (!isCheck(piece->getColor())) {
                validMoves.push_back(move);
            }
        }
        
        return validMoves;
    }
    
    void undoMove() {
        if (moveHistory.empty()) return;
        
        Move lastMove = moveHistory.back();
        moveHistory.pop_back();
        
        // Restore board state
        board.movePiece(lastMove.getTo(), lastMove.getFrom());
        if (lastMove.getCapturedPiece()) {
            board.setPiece(lastMove.getTo(), 
                         unique_ptr<Piece>(lastMove.getCapturedPiece()));
        }
        
        // Restore piece state
        lastMove.getPiece()->setHasMoved(!lastMove.getPiece()->getHasMoved());
        
        // Switch player
        currentPlayer = (currentPlayer == Color::WHITE) ? Color::BLACK : Color::WHITE;
    }
    
    string getFEN() const {
        // Implementation of FEN string generation
        // This is a simplified version
        string fen;
        
        // Board position
        for (int row = 0; row < 8; row++) {
            int emptyCount = 0;
            for (int col = 0; col < 8; col++) {
                Piece* piece = board.getPiece(Position(row, col));
                if (piece) {
                    if (emptyCount > 0) {
                        fen += to_string(emptyCount);
                        emptyCount = 0;
                    }
                    fen += getPieceChar(piece);
                } else {
                    emptyCount++;
                }
            }
            if (emptyCount > 0) {
                fen += to_string(emptyCount);
            }
            if (row < 7) fen += '/';
        }
        
        // Active color
        fen += ' ';
        fen += (currentPlayer == Color::WHITE) ? 'w' : 'b';
        
        // Castling availability
        fen += ' ';
        fen += getCastlingString();
        
        // En passant target
        fen += ' ';
        fen += enPassantTarget.getNotation();
        
        // Halfmove clock
        fen += ' ';
        fen += to_string(halfMoveClock);
        
        // Fullmove number
        fen += ' ';
        fen += to_string(fullMoveNumber);
        
        return fen;
    }

private:
    void initializeBoard() {
        // Initialize pawns
        for (int col = 0; col < 8; col++) {
            board.setPiece(Position(1, col), 
                         make_unique<Pawn>(Color::BLACK, Position(1, col)));
            board.setPiece(Position(6, col), 
                         make_unique<Pawn>(Color::WHITE, Position(6, col)));
        }
        
        // Initialize other pieces
        // ... (implementation for other pieces)
    }
    
    void handleCastling(Move& move) {
        // Implementation of castling logic
    }
    
    void handleEnPassant(Move& move) {
        // Implementation of en passant logic
    }
    
    void handlePromotion(Move& move) {
        // Implementation of pawn promotion logic
    }
    
    void executeMove(Move& move) {
        // Store captured piece
        Piece* capturedPiece = board.getPiece(move.getTo());
        if (capturedPiece) {
            move.setCapturedPiece(capturedPiece);
        }
        
        // Move piece
        board.movePiece(move.getFrom(), move.getTo());
        move.getPiece()->setHasMoved(true);
        
        // Update move history
        moveHistory.push_back(move);
        
        // Switch player
        currentPlayer = (currentPlayer == Color::WHITE) ? Color::BLACK : Color::WHITE;
    }
    
    void updateGameState() {
        // Update halfmove clock
        if (moveHistory.back().getCapturedPiece() || 
            moveHistory.back().getPiece()->getType() == PieceType::PAWN) {
            halfMoveClock = 0;
        } else {
            halfMoveClock++;
        }
        
        // Update fullmove number
        if (currentPlayer == Color::WHITE) {
            fullMoveNumber++;
        }
        
        // Update position history
        string fen = getFEN();
        positionHistory[fen]++;
    }
    
    bool isInsufficientMaterial() const {
        // Implementation of insufficient material check
        return false;  // Simplified
    }
    
    bool isThreefoldRepetition() const {
        for (const auto& entry : positionHistory) {
            if (entry.second >= 3) return true;
        }
        return false;
    }
    
    char getPieceChar(Piece* piece) const {
        char c;
        switch (piece->getType()) {
            case PieceType::PAWN: c = 'p'; break;
            case PieceType::ROOK: c = 'r'; break;
            case PieceType::KNIGHT: c = 'n'; break;
            case PieceType::BISHOP: c = 'b'; break;
            case PieceType::QUEEN: c = 'q'; break;
            case PieceType::KING: c = 'k'; break;
        }
        return (piece->getColor() == Color::WHITE) ? toupper(c) : c;
    }
    
    string getCastlingString() const {
        string castling;
        if (!whiteKingMoved) {
            if (!whiteRookMoved[0]) castling += 'K';
            if (!whiteRookMoved[1]) castling += 'Q';
        }
        if (!blackKingMoved) {
            if (!blackRookMoved[0]) castling += 'k';
            if (!blackRookMoved[1]) castling += 'q';
        }
        return castling.empty() ? "-" : castling;
    }
};

// Example usage
int main() {
    GameState game;
    
    // Make some moves
    game.makeMove(Position("e2"), Position("e4"));  // White pawn
    game.makeMove(Position("e7"), Position("e5"));  // Black pawn
    game.makeMove(Position("g1"), Position("f3"));  // White knight
    
    // Check game state
    if (game.isCheck(Color::BLACK)) {
        cout << "Black is in check!" << endl;
    }
    
    // Get valid moves for a piece
    vector<Position> moves = game.getValidMoves(Position("f3"));
    cout << "Valid moves for knight: ";
    for (const auto& move : moves) {
        cout << move.getNotation() << " ";
    }
    cout << endl;
    
    // Get FEN string
    cout << "FEN: " << game.getFEN() << endl;
    
    return 0;
} 