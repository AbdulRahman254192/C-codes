#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <fstream>
#include <limits>
#include <algorithm>

using namespace std;

/*=================================================================
 1. Data structure
 =================================================================*/

struct Book {
    int id;
    string title;
    string author;
    double price;
    int quantity;
};

vector<Book> books;
vector<string> history;

const string DATA_FILE = "bookstore_data.txt";

const string ADMIN_USER = "admin";
const string ADMIN_PASS = "admin";


// =================================================================
// 2. File handling function (Data Persistence)
// =================================================================

void loadFromFile() {
    ifstream inputFile(DATA_FILE);
    if (!inputFile.is_open()) {
        cout << "Book data file not found or empty. Starting with an empty inventory.\n";
        return;
    }

    Book b;
    while (inputFile >> b.id) {
        inputFile.ignore(numeric_limits<streamsize>::max(), '\n');
        
        if (!getline(inputFile, b.title)) break;
        
        if (!getline(inputFile, b.author)) break;
        
        if (!(inputFile >> b.price)) break;
        
        if (!(inputFile >> b.quantity)) break;

        books.push_back(b);
    }

    inputFile.close();
    cout << "Book data loaded successfully! Total books: " << books.size() << endl;
}

void saveToFile() {
    ofstream outputFile(DATA_FILE);
    if (!outputFile.is_open()) {
        cerr << "Error: Could not open file for saving data.\n";
        return;
    }

    for (const auto& b : books) {
        outputFile << b.id << "\n";
        outputFile << b.title << "\n";
        outputFile << b.author << "\n";
        outputFile << fixed << setprecision(2) << b.price << "\n";
        outputFile << b.quantity << "\n";
    }

    outputFile.close();
    cout << "Book data saved successfully, preserving data to the next run of program.\n";
}

// =================================================================
// 3. core function & utilities
// =================================================================

int getIntInput(const string& prompt) {
    int value;
    cout << prompt;
    while (!(cin >> value)) {
        cout << "Invalid input. Please enter a number: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return value;
}

double getDoubleInput(const string& prompt) {
    double value;
    cout << prompt;
    while (!(cin >> value)) {
        cout << "Invalid input. Please enter a number: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return value;
}

bool adminLogin() {
    string username, password;
    cout << "\n----------- ADMIN LOGIN -----------\n";
    
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter Username: ";
    getline(cin, username);

    cout << "Enter Password: ";
    getline(cin, password);

    if (username == ADMIN_USER && password == ADMIN_PASS) {
        cout << "\nAccess Granted. Welcome, Admin!\n";
        return true;
    } else {
        cout << "\nAccess Denied. Invalid username or password.\n";
        return false;
    }
}


void addBook() {
    Book newBook;
    cout << "\n----------- ADD NEW BOOK -----------\n";

    newBook.id = getIntInput("1. Enter Book ID: ");
    
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
    
    cout << "2. Enter Book Title: ";
    getline(cin, newBook.title);
    
    cout << "3. Enter Book Author: ";
    getline(cin, newBook.author);

    newBook.price = getDoubleInput("4. Enter Book Price: $");
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
    
    newBook.quantity = getIntInput("5. Enter Book Stock (Quantity): ");

    books.push_back(newBook);
    cout << "\nBook \"" << newBook.title << "\" added successfully!\n";
}

void deleteBook() {
    string titleToDelete;
    cout << "\n----------- DELETE BOOK -----------\n";
    cout << "Enter Book Title to delete: ";
    
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
    getline(cin, titleToDelete);

    auto it = books.begin();
    while (it != books.end()) {
        if (it->title == titleToDelete) {
            it = books.erase(it);
            cout << "Book \"" << titleToDelete << "\" deleted successfully!\n";
            return;
        } else {
            ++it;
        }
    }
    cout << "Book not found.\n";
}


void displayAllBooks() {
    if (books.empty()) {
        cout << "\nNo books currently in inventory.\n";
        return;
    }

    cout << "\n--------- ALL BOOKS IN INVENTORY ---------\n";
    cout << left << setw(5) << "ID"
              << setw(30) << "Title"
              << setw(20) << "Author"
              << right << setw(10) << "Price"
              << setw(10) << "Stock\n";
    cout << string(75, '-') << "\n";

    for (const auto& b : books) {
        cout << left << setw(5) << b.id
                  << setw(30) << b.title
                  << setw(20) << b.author
                  << right << setw(10) << fixed << setprecision(2) << b.price
                  << setw(10) << b.quantity << "\n";
    }
    cout << string(75, '-') << "\n";
}

void searchBook() {
    string searchTitle;
    cout << "\n----------- SEARCH BOOK -----------\n";
    cout << "Enter Book Title (or part of the title) to search: ";
    
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
    getline(cin, searchTitle);

    bool found = false;
    for (const auto& b : books) {
        string title_lower = b.title;
        string search_lower = searchTitle;
        
        transform(title_lower.begin(), title_lower.end(), title_lower.begin(), [](unsigned char c){ return std::tolower(c); });
        transform(search_lower.begin(), search_lower.end(), search_lower.begin(), [](unsigned char c){ return std::tolower(c); });

        if (title_lower.find(search_lower) != string::npos) {
            cout << "\nBook Found:\n";
            cout << "Title: " << b.title << "\n";
            cout << "Author: " << b.author << "\n";
            cout << "Price: $" << fixed << setprecision(2) << b.price << "\n";
            cout << "Stock: " << b.quantity << "\n";
            found = true;
            history.push_back(b.title);
            break;
        }
    }

    if (!found) {
        cout << "Book not found.\n";
    }
}

void sellBook() {
    string sellTitle;
    int quantityToSell;
    cout << "\n----------- SELL A BOOK -----------\n";
    cout << "1. Enter Book Title to sell: ";
    
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
    getline(cin, sellTitle);

    for (auto& b : books) {
        if (b.title == sellTitle) {
            cout << "2. Book Found: " << b.title << "\n";
            cout << "3. Price: $" << fixed << setprecision(2) << b.price << "\n";
            cout << "4. Stock: " << b.quantity << "\n";

            if (b.quantity == 0) {
                cout << "Sorry, this book is out of stock.\n";
                return;
            }

            cout << "5. Enter Quantity to sell: ";
            while (!(cin >> quantityToSell) || quantityToSell <= 0 || quantityToSell > b.quantity) {
                cout << "Invalid quantity. Enter a value between 1 and " << b.quantity << ": ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }

            b.quantity -= quantityToSell;
            double totalPrice = quantityToSell * b.price;
            cout << "\nSale successful! Total Price: $" << fixed << setprecision(2) << totalPrice << "\n";
            history.push_back(b.title);
            return;
        }
    }
    cout << "Book not found.\n";
}

void viewHistory() {
    cout << "\n----------- RECENTLY ACCESSED BOOKS -----------\n";
    if (history.empty()) {
        cout << "No recent search or purchase history.\n";
        return;
    }

    cout << "Recently viewed/purchased books:\n";
    for (const auto& h : history) {
        cout << "- " << h << "\n";
    }
}


// =================================================================
// 4. menu functions
// =================================================================

void adminMenu() {
    int choice;
    do {
        cout << "\n------------ ADMIN DASHBOARD ----\n";
        cout << "1. Add New Book.\n";
        cout << "2. Delete Book.\n";
        cout << "3. View All Books (Inventory).\n";
        cout << "4. Search for a Book.\n";
        cout << "5. View History.\n";
        cout << "6. Logout.\n";
        cout << "Enter your choice (1-6):- ";

        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
            case 1: addBook(); break;
            case 2: deleteBook(); break;
            case 3: displayAllBooks(); break;
            case 4: searchBook(); break;
            case 5: viewHistory(); break;
            case 6: cout << "Logging out of Admin Dashboard.\n"; break;
            default: cout << "Invalid option. Please try again.\n";
        }
    } while (choice != 6);
}

void customerMenu() {
    int choice;
    do {
        cout << "\n------------ CUSTOMER MENU ----\n";
        cout << "1. View All Books.\n";
        cout << "2. Search for a Book.\n";
        cout << "3. Buy a Book.\n";
        cout << "4. View History.\n";
        cout << "5. Exit to Main Menu.\n";
        cout << "Enter your choice (1-5):- ";

        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
            case 1: displayAllBooks(); break;
            case 2: searchBook(); break;
            case 3: sellBook(); break;
            case 4: viewHistory(); break;
            case 5: cout << "Returning to Main Menu.\n"; break;
            default: cout << "Invalid option. Please try again.\n";
        }
    } while (choice != 5);
}

void mainMenu() {
    int choice;
    do {
        cout << "\n-----WELCOME TO THE BOOK STORE SYSTEM-----\n";
        cout << "Please select your role:\n";
        cout << "1. Login as Admin.\n";
        cout << "2. Login as Customer.\n";
        cout << "3. Exit.\n";
        cout << "Enter your choice (1-3):- ";

        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
            case 1: 
                if (adminLogin()) {
                    adminMenu();
                }
                break;
            case 2: customerMenu(); break;
            case 3: cout << "Exiting the Book Store System. Goodbye!\n"; break;
            default: cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 3);
}

// =================================================================
// 5. MAIN PROGRAM ENTRY
// =================================================================

int main() {
    loadFromFile();

    mainMenu();

    saveToFile();

    return 0;
}