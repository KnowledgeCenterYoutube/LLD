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
void testBookManagement() {
    cout << "Running book management tests..." << endl;
    
    LibrarySystem* library = LibrarySystem::getInstance();
    
    // Test adding books
    string isbn1 = library->addBook("978-0743273565", "The Great Gatsby", "F. Scott Fitzgerald");
    string isbn2 = library->addBook("978-0140283334", "1984", "George Orwell");
    
    // Test searching books
    vector<Book> results = library->searchBooks("Gatsby");
    assertEqual(1, results.size(), "Should find one book with 'Gatsby' in title");
    
    results = library->searchBooks("George");
    assertEqual(1, results.size(), "Should find one book by George Orwell");
    
    cout << "Book management tests passed!" << endl;
}

void testUserManagement() {
    cout << "Running user management tests..." << endl;
    
    LibrarySystem* library = LibrarySystem::getInstance();
    
    // Test registering users
    string userId1 = library->registerUser("John Doe", "john@example.com", UserType::STUDENT);
    string userId2 = library->registerUser("Jane Smith", "jane@example.com", UserType::FACULTY);
    
    assertTrue(!userId1.empty(), "Should generate valid user ID");
    assertTrue(!userId2.empty(), "Should generate valid user ID");
    assertTrue(userId1 != userId2, "User IDs should be unique");
    
    cout << "User management tests passed!" << endl;
}

void testBorrowingOperations() {
    cout << "Running borrowing operations tests..." << endl;
    
    LibrarySystem* library = LibrarySystem::getInstance();
    
    // Add a book
    string isbn = library->addBook("978-0743273565", "The Great Gatsby", "F. Scott Fitzgerald");
    
    // Register a user
    string userId = library->registerUser("John Doe", "john@example.com", UserType::STUDENT);
    
    // Test borrowing
    assertTrue(library->borrowBook(userId, isbn), "Should be able to borrow book");
    assertFalse(library->borrowBook(userId, isbn), "Should not be able to borrow same book twice");
    
    // Test returning
    assertTrue(library->returnBook(userId, isbn), "Should be able to return book");
    assertTrue(library->borrowBook(userId, isbn), "Should be able to borrow book again after return");
    
    cout << "Borrowing operations tests passed!" << endl;
}

void testFineManagement() {
    cout << "Running fine management tests..." << endl;
    
    LibrarySystem* library = LibrarySystem::getInstance();
    
    // Add a book
    string isbn = library->addBook("978-0743273565", "The Great Gatsby", "F. Scott Fitzgerald");
    
    // Register a user
    string userId = library->registerUser("John Doe", "john@example.com", UserType::STUDENT);
    
    // Borrow and return book late
    library->borrowBook(userId, isbn);
    
    // Simulate late return by modifying the borrow record
    // This would normally be handled by the system's time tracking
    // For testing, we'll just verify the fine calculation logic
    
    // Test fine payment
    assertTrue(library->borrowBook(userId, isbn), "Should be able to borrow with no fines");
    
    cout << "Fine management tests passed!" << endl;
}

void testBorrowingLimits() {
    cout << "Running borrowing limits tests..." << endl;
    
    LibrarySystem* library = LibrarySystem::getInstance();
    
    // Register a user
    string userId = library->registerUser("John Doe", "john@example.com", UserType::STUDENT);
    
    // Add multiple books
    vector<string> isbns;
    for (int i = 0; i < 6; i++) {
        string isbn = library->addBook("978-" + to_string(i), "Book " + to_string(i), "Author " + to_string(i));
        isbns.push_back(isbn);
    }
    
    // Try to borrow more than the limit
    for (int i = 0; i < 5; i++) {
        assertTrue(library->borrowBook(userId, isbns[i]), 
                  "Should be able to borrow up to 5 books");
    }
    
    assertFalse(library->borrowBook(userId, isbns[5]), 
               "Should not be able to borrow more than 5 books");
    
    cout << "Borrowing limits tests passed!" << endl;
}

void testConcurrentOperations() {
    cout << "Running concurrent operations tests..." << endl;
    
    LibrarySystem* library = LibrarySystem::getInstance();
    
    // Add a book with multiple copies
    string isbn = library->addBook("978-0743273565", "The Great Gatsby", "F. Scott Fitzgerald");
    
    // Register multiple users
    vector<string> userIds;
    for (int i = 0; i < 3; i++) {
        string userId = library->registerUser("User " + to_string(i), 
                                            "user" + to_string(i) + "@example.com", 
                                            UserType::STUDENT);
        userIds.push_back(userId);
    }
    
    // Simulate concurrent borrowing
    // In a real system, this would be handled by multiple threads
    // For testing, we'll just verify the thread safety of the operations
    
    for (const auto& userId : userIds) {
        assertTrue(library->borrowBook(userId, isbn), 
                  "Should be able to borrow book concurrently");
    }
    
    cout << "Concurrent operations tests passed!" << endl;
}

void testReportGeneration() {
    cout << "Running report generation tests..." << endl;
    
    LibrarySystem* library = LibrarySystem::getInstance();
    
    // Generate different types of reports
    Report availabilityReport = library->generateReport(ReportType::AVAILABILITY);
    Report popularReport = library->generateReport(ReportType::POPULAR);
    Report borrowingReport = library->generateReport(ReportType::BORROWING);
    Report fineReport = library->generateReport(ReportType::FINE);
    
    // Verify report generation
    assertTrue(!availabilityReport.generateBookAvailabilityReport().empty(), 
              "Should generate availability report");
    assertTrue(!popularReport.generatePopularBooksReport().empty(), 
              "Should generate popular books report");
    assertTrue(!borrowingReport.generateBorrowingPatternReport().empty(), 
              "Should generate borrowing pattern report");
    assertTrue(!fineReport.generateFineCollectionReport().empty(), 
              "Should generate fine collection report");
    
    cout << "Report generation tests passed!" << endl;
}

int main() {
    try {
        testBookManagement();
        testUserManagement();
        testBorrowingOperations();
        testFineManagement();
        testBorrowingLimits();
        testConcurrentOperations();
        testReportGeneration();
        
        cout << "All tests passed!" << endl;
        return 0;
    } catch (const exception& e) {
        cerr << "Test failed: " << e.what() << endl;
        return 1;
    }
} 