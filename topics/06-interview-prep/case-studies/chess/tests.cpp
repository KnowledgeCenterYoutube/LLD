#include <iostream>
#include <cassert>
#include <vector>
#include "implementation.cpp"

using namespace std;

// Test helper functions
void assertTrue(bool condition, const string& message) {
    if (!condition) {
        throw runtime_error("Test failed: " + message);
    }
}

void assertFalse(bool condition, const string& message) {
    assertTrue(!condition, message);
}

void assertEqual(int expected, int actual, const string& message) {
    if (expected != actual) {
        throw runtime_error("Test failed: " + message + 
                          " (Expected: " + to_string(expected) + 
                          ", Got: " + to_string(actual) + ")");
    }
}

// Test cases
void testBasicMoves() {
    cout << "Running basic moves tests..." << endl;
    
    GameState game;
    
    // Test pawn moves
    assertTrue(game.makeMove(Position("e2"), Position("e4")), 
              "Should be able to move pawn two squares");
    assertTrue(game.makeMove(Position("e7"), Position("e5")), 
              "Should be able to move black pawn two squares");
    
    // Test knight moves
    assertTrue(game.makeMove(Position("g1"), Position("f3")), 
              "Should be able to move knight");
    assertTrue(game.makeMove(Position("b8"), Position("c6")), 
              "Should be able to move black knight");
    
    cout << "Basic moves tests passed!" << endl;
}

void testInvalidMoves() {
    cout << "Running invalid moves tests..." << endl;
    
    GameState game;
    
    // Test invalid pawn moves
    assertFalse(game.makeMove(Position("e2"), Position("e5")), 
               "Should not be able to move pawn three squares");
    assertFalse(game.makeMove(Position("e2"), Position("d3")), 
               "Should not be able to move pawn diagonally without capture");
    
    // Test moving opponent's piece
    assertFalse(game.makeMove(Position("e7"), Position("e5")), 
               "Should not be able to move opponent's piece");
    
    cout << "Invalid moves tests passed!" << endl;
}

void testCheckDetection() {
    cout << "Running check detection tests..." << endl;
    
    GameState game;
    
    // Set up a position where black is in check
    game.makeMove(Position("e2"), Position("e4"));  // White pawn
    game.makeMove(Position("e7"), Position("e5"));  // Black pawn
    game.makeMove(Position("f1"), Position("c4"));  // White bishop
    game.makeMove(Position("b8"), Position("c6"));  // Black knight
    game.makeMove(Position("d1"), Position("h5"));  // White queen
    
    assertTrue(game.isCheck(Color::BLACK), "Black should be in check");
    assertFalse(game.isCheck(Color::WHITE), "White should not be in check");
    
    cout << "Check detection tests passed!" << endl;
}

void testCheckmateDetection() {
    cout << "Running checkmate detection tests..." << endl;
    
    GameState game;
    
    // Set up a position where black is checkmated (Fool's Mate)
    game.makeMove(Position("f2"), Position("f4"));  // White pawn
    game.makeMove(Position("e7"), Position("e6"));  // Black pawn
    game.makeMove(Position("g2"), Position("g4"));  // White pawn
    game.makeMove(Position("d8"), Position("h4"));  // Black queen
    
    assertTrue(game.isCheckmate(Color::WHITE), "White should be checkmated");
    assertFalse(game.isCheckmate(Color::BLACK), "Black should not be checkmated");
    
    cout << "Checkmate detection tests passed!" << endl;
}

void testStalemateDetection() {
    cout << "Running stalemate detection tests..." << endl;
    
    GameState game;
    
    // Set up a position where black is stalemated
    game.makeMove(Position("e2"), Position("e4"));  // White pawn
    game.makeMove(Position("a7"), Position("a5"));  // Black pawn
    game.makeMove(Position("d1"), Position("h5"));  // White queen
    game.makeMove(Position("a8"), Position("a6"));  // Black rook
    game.makeMove(Position("h5"), Position("a5"));  // White queen
    game.makeMove(Position("h7"), Position("h5"));  // Black pawn
    game.makeMove(Position("h2"), Position("h4"));  // White pawn
    game.makeMove(Position("a6"), Position("h6"));  // Black rook
    game.makeMove(Position("a5"), Position("c7"));  // White queen
    game.makeMove(Position("f7"), Position("f6"));  // Black pawn
    game.makeMove(Position("c7"), Position("d7"));  // White queen
    
    assertTrue(game.isStalemate(Color::BLACK), "Black should be stalemated");
    assertFalse(game.isStalemate(Color::WHITE), "White should not be stalemated");
    
    cout << "Stalemate detection tests passed!" << endl;
}

void testValidMoves() {
    cout << "Running valid moves tests..." << endl;
    
    GameState game;
    
    // Get valid moves for a pawn
    vector<Position> moves = game.getValidMoves(Position("e2"));
    assertEqual(2, moves.size(), "Pawn should have 2 valid moves");
    
    // Get valid moves for a knight
    game.makeMove(Position("e2"), Position("e4"));  // White pawn
    moves = game.getValidMoves(Position("g1"));
    assertEqual(2, moves.size(), "Knight should have 2 valid moves");
    
    cout << "Valid moves tests passed!" << endl;
}

void testMoveUndo() {
    cout << "Running move undo tests..." << endl;
    
    GameState game;
    
    // Make a move
    game.makeMove(Position("e2"), Position("e4"));  // White pawn
    
    // Undo the move
    game.undoMove();
    
    // Try to make the same move again
    assertTrue(game.makeMove(Position("e2"), Position("e4")), 
              "Should be able to make the same move after undo");
    
    cout << "Move undo tests passed!" << endl;
}

int main() {
    try {
        testBasicMoves();
        testInvalidMoves();
        testCheckDetection();
        testCheckmateDetection();
        testStalemateDetection();
        testValidMoves();
        testMoveUndo();
        
        cout << "All tests passed!" << endl;
        return 0;
    } catch (const exception& e) {
        cerr << "Test failed: " << e.what() << endl;
        return 1;
    }
} 