#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <mutex>
#include <chrono>
#include <stdexcept>
#include <algorithm>

using namespace std;
using namespace chrono;

// Enums
enum class UserType { STUDENT, FACULTY, STAFF };
enum class ReportType { AVAILABILITY, POPULAR, BORROWING, FINE };

// Forward declarations
class Book;
class User;
class BorrowRecord;
class Report;

// Book class implementation
class Book {
private:
    string isbn;
    string title;
    string author;
    string category;
    int totalCopies;
    int availableCopies;
    mutex bookMutex;

public:
    Book(const string& isbn, const string& title, const string& author)
        : isbn(isbn), title(title), author(author), totalCopies(1), availableCopies(1) {}
    
    string getIsbn() const { return isbn; }
    string getTitle() const { return title; }
    string getAuthor() const { return author; }
    int getAvailableCopies() const { return availableCopies; }
    
    bool isAvailable() const {
        lock_guard<mutex> lock(bookMutex);
        return availableCopies > 0;
    }
    
    void incrementCopies() {
        lock_guard<mutex> lock(bookMutex);
        totalCopies++;
        availableCopies++;
    }
    
    void decrementCopies() {
        lock_guard<mutex> lock(bookMutex);
        if (availableCopies > 0) {
            availableCopies--;
        }
    }
};

// User class implementation
class User {
private:
    string id;
    string name;
    string email;
    UserType type;
    double fineAmount;
    vector<BorrowRecord> borrowHistory;
    mutex userMutex;

public:
    User(const string& name, const string& email, UserType type)
        : id(generateId()), name(name), email(email), type(type), fineAmount(0.0) {}
    
    string getId() const { return id; }
    string getName() const { return name; }
    UserType getType() const { return type; }
    double getFineAmount() const { return fineAmount; }
    
    void addFine(double amount) {
        lock_guard<mutex> lock(userMutex);
        fineAmount += amount;
    }
    
    void payFine(double amount) {
        lock_guard<mutex> lock(userMutex);
        fineAmount = max(0.0, fineAmount - amount);
    }
    
    bool canBorrow() const {
        lock_guard<mutex> lock(userMutex);
        return fineAmount < 50.0 && borrowHistory.size() < 5;
    }
    
    void addBorrowRecord(const BorrowRecord& record) {
        lock_guard<mutex> lock(userMutex);
        borrowHistory.push_back(record);
    }

private:
    string generateId() {
        static int counter = 0;
        return "USER" + to_string(++counter);
    }
};

// BorrowRecord class implementation
class BorrowRecord {
private:
    string userId;
    string bookIsbn;
    system_clock::time_point borrowDate;
    system_clock::time_point dueDate;
    int renewalCount;
    bool isReturned;

public:
    BorrowRecord(const string& userId, const string& bookIsbn)
        : userId(userId), bookIsbn(bookIsbn), borrowDate(system_clock::now()),
          renewalCount(0), isReturned(false) {
        dueDate = borrowDate + hours(24 * 14); // 14 days
    }
    
    bool isOverdue() const {
        return !isReturned && system_clock::now() > dueDate;
    }
    
    double calculateFine() const {
        if (!isOverdue()) return 0.0;
        
        auto now = system_clock::now();
        auto duration = duration_cast<hours>(now - dueDate);
        return duration.count() / 24.0; // $1 per day
    }
    
    bool canRenew() const {
        return !isReturned && renewalCount < 2;
    }
    
    void renew() {
        if (canRenew()) {
            dueDate += hours(24 * 14); // Add 14 days
            renewalCount++;
        }
    }
    
    void markAsReturned() {
        isReturned = true;
    }
};

// BookCatalog class implementation
class BookCatalog {
private:
    unordered_map<string, Book> booksByIsbn;
    unordered_multimap<string, string> booksByTitle;
    unordered_multimap<string, string> booksByAuthor;
    mutex catalogMutex;

public:
    void addBook(const Book& book) {
        lock_guard<mutex> lock(catalogMutex);
        booksByIsbn[book.getIsbn()] = book;
        booksByTitle.insert({book.getTitle(), book.getIsbn()});
        booksByAuthor.insert({book.getAuthor(), book.getIsbn()});
    }
    
    void removeBook(const string& isbn) {
        lock_guard<mutex> lock(catalogMutex);
        auto it = booksByIsbn.find(isbn);
        if (it != booksByIsbn.end()) {
            booksByTitle.erase(it->second.getTitle());
            booksByAuthor.erase(it->second.getAuthor());
            booksByIsbn.erase(it);
        }
    }
    
    Book* getBook(const string& isbn) {
        lock_guard<mutex> lock(catalogMutex);
        auto it = booksByIsbn.find(isbn);
        return it != booksByIsbn.end() ? &it->second : nullptr;
    }
    
    vector<Book> searchByTitle(const string& title) {
        lock_guard<mutex> lock(catalogMutex);
        vector<Book> results;
        auto range = booksByTitle.equal_range(title);
        for (auto it = range.first; it != range.second; ++it) {
            results.push_back(booksByIsbn[it->second]);
        }
        return results;
    }
    
    vector<Book> searchByAuthor(const string& author) {
        lock_guard<mutex> lock(catalogMutex);
        vector<Book> results;
        auto range = booksByAuthor.equal_range(author);
        for (auto it = range.first; it != range.second; ++it) {
            results.push_back(booksByIsbn[it->second]);
        }
        return results;
    }
};

// UserManager class implementation
class UserManager {
private:
    unordered_map<string, User> users;
    mutex userManagerMutex;

public:
    string registerUser(const string& name, const string& email, UserType type) {
        lock_guard<mutex> lock(userManagerMutex);
        User user(name, email, type);
        string id = user.getId();
        users[id] = user;
        return id;
    }
    
    User* getUser(const string& id) {
        lock_guard<mutex> lock(userManagerMutex);
        auto it = users.find(id);
        return it != users.end() ? &it->second : nullptr;
    }
    
    void updateUser(const User& user) {
        lock_guard<mutex> lock(userManagerMutex);
        users[user.getId()] = user;
    }
    
    void removeUser(const string& id) {
        lock_guard<mutex> lock(userManagerMutex);
        users.erase(id);
    }
};

// BorrowingManager class implementation
class BorrowingManager {
private:
    unordered_map<string, vector<BorrowRecord>> userBorrowRecords;
    unordered_map<string, vector<string>> bookBorrowers;
    mutex borrowingMutex;

public:
    bool borrowBook(const string& userId, const string& isbn) {
        lock_guard<mutex> lock(borrowingMutex);
        
        // Check if user has reached borrowing limit
        if (userBorrowRecords[userId].size() >= 5) {
            return false;
        }
        
        // Create borrow record
        BorrowRecord record(userId, isbn);
        userBorrowRecords[userId].push_back(record);
        bookBorrowers[isbn].push_back(userId);
        
        return true;
    }
    
    bool returnBook(const string& userId, const string& isbn) {
        lock_guard<mutex> lock(borrowingMutex);
        
        auto& records = userBorrowRecords[userId];
        auto it = find_if(records.begin(), records.end(),
            [&](const BorrowRecord& record) {
                return record.getBookIsbn() == isbn && !record.isReturned();
            });
        
        if (it != records.end()) {
            it->markAsReturned();
            return true;
        }
        
        return false;
    }
    
    bool renewBook(const string& userId, const string& isbn) {
        lock_guard<mutex> lock(borrowingMutex);
        
        auto& records = userBorrowRecords[userId];
        auto it = find_if(records.begin(), records.end(),
            [&](const BorrowRecord& record) {
                return record.getBookIsbn() == isbn && !record.isReturned();
            });
        
        if (it != records.end() && it->canRenew()) {
            it->renew();
            return true;
        }
        
        return false;
    }
    
    vector<BorrowRecord> getUserBorrowHistory(const string& userId) {
        lock_guard<mutex> lock(borrowingMutex);
        return userBorrowRecords[userId];
    }
    
    vector<string> getBookBorrowers(const string& isbn) {
        lock_guard<mutex> lock(borrowingMutex);
        return bookBorrowers[isbn];
    }
};

// FineManager class implementation
class FineManager {
private:
    unordered_map<string, double> userFines;
    mutex fineMutex;

public:
    void addFine(const string& userId, double amount) {
        lock_guard<mutex> lock(fineMutex);
        userFines[userId] += amount;
    }
    
    void payFine(const string& userId, double amount) {
        lock_guard<mutex> lock(fineMutex);
        userFines[userId] = max(0.0, userFines[userId] - amount);
    }
    
    double getUserFine(const string& userId) {
        lock_guard<mutex> lock(fineMutex);
        return userFines[userId];
    }
    
    bool canBorrow(const string& userId) {
        lock_guard<mutex> lock(fineMutex);
        return userFines[userId] < 50.0;
    }
    
    void generateFineReport() {
        lock_guard<mutex> lock(fineMutex);
        // Implementation of fine report generation
    }
};

// Report class implementation
class Report {
private:
    system_clock::time_point startDate;
    system_clock::time_point endDate;
    ReportType type;

public:
    Report(ReportType type) : type(type) {
        startDate = system_clock::now() - hours(24 * 30); // Last 30 days
        endDate = system_clock::now();
    }
    
    void setDateRange(system_clock::time_point start,
                     system_clock::time_point end) {
        startDate = start;
        endDate = end;
    }
    
    string generateBookAvailabilityReport() {
        // Implementation of book availability report
        return "Book Availability Report";
    }
    
    string generatePopularBooksReport() {
        // Implementation of popular books report
        return "Popular Books Report";
    }
    
    string generateBorrowingPatternReport() {
        // Implementation of borrowing pattern report
        return "Borrowing Pattern Report";
    }
    
    string generateFineCollectionReport() {
        // Implementation of fine collection report
        return "Fine Collection Report";
    }
};

// LibrarySystem class implementation (Singleton)
class LibrarySystem {
private:
    static LibrarySystem* instance;
    static mutex instanceMutex;
    
    BookCatalog bookCatalog;
    UserManager userManager;
    BorrowingManager borrowingManager;
    FineManager fineManager;
    
    LibrarySystem() = default;
    
public:
    static LibrarySystem* getInstance() {
        lock_guard<mutex> lock(instanceMutex);
        if (!instance) {
            instance = new LibrarySystem();
        }
        return instance;
    }
    
    string addBook(const string& isbn, const string& title, const string& author) {
        Book book(isbn, title, author);
        bookCatalog.addBook(book);
        return isbn;
    }
    
    string registerUser(const string& name, const string& email, UserType type) {
        return userManager.registerUser(name, email, type);
    }
    
    bool borrowBook(const string& userId, const string& isbn) {
        User* user = userManager.getUser(userId);
        Book* book = bookCatalog.getBook(isbn);
        
        if (!user || !book || !user->canBorrow() || !book->isAvailable()) {
            return false;
        }
        
        if (borrowingManager.borrowBook(userId, isbn)) {
            book->decrementCopies();
            return true;
        }
        
        return false;
    }
    
    bool returnBook(const string& userId, const string& isbn) {
        if (borrowingManager.returnBook(userId, isbn)) {
            Book* book = bookCatalog.getBook(isbn);
            if (book) {
                book->incrementCopies();
            }
            return true;
        }
        return false;
    }
    
    vector<Book> searchBooks(const string& query) {
        vector<Book> results;
        vector<Book> titleResults = bookCatalog.searchByTitle(query);
        vector<Book> authorResults = bookCatalog.searchByAuthor(query);
        
        results.insert(results.end(), titleResults.begin(), titleResults.end());
        results.insert(results.end(), authorResults.begin(), authorResults.end());
        
        return results;
    }
    
    Report generateReport(ReportType type) {
        return Report(type);
    }
};

// Initialize static members
LibrarySystem* LibrarySystem::instance = nullptr;
mutex LibrarySystem::instanceMutex;

// Example usage
int main() {
    LibrarySystem* library = LibrarySystem::getInstance();
    
    // Add books
    library->addBook("978-0743273565", "The Great Gatsby", "F. Scott Fitzgerald");
    library->addBook("978-0140283334", "1984", "George Orwell");
    
    // Register users
    string userId = library->registerUser("John Doe", "john@example.com", UserType::STUDENT);
    
    // Borrow a book
    if (library->borrowBook(userId, "978-0743273565")) {
        cout << "Book borrowed successfully!" << endl;
    }
    
    // Search books
    vector<Book> results = library->searchBooks("Gatsby");
    cout << "Found " << results.size() << " books" << endl;
    
    // Generate report
    Report report = library->generateReport(ReportType::AVAILABILITY);
    cout << report.generateBookAvailabilityReport() << endl;
    
    return 0;
} 