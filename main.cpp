#include <iostream>
#include <fstream>
#include <iomanip>
#include <conio.h>

using namespace std;

void viewUsers();
void createUser();
void updateUser();
void deleteUser();
bool isIdExists(int id);
bool login();

struct User {
    int id;
    string username;
    string position;
    string salary;
};

int main() {
    if (!login()) {
        cout << "Login failed. Exiting...\n";
        return 0;
    }

    int choice;
    do {
        cout << " Salary Management System\n";
        cout << " Created by: Thomas Emmanuel R. Pajarillaga\n";
        cout << "1. View Users\n";
        cout << "2. Create New User\n";
        cout << "3. Update User\n";
        cout << "4. Delete User\n";
        cout << "Enter your choice (1-4): ";
        cin >> choice;

        switch (choice) {
            case 1:
                viewUsers();
                break;
            case 2:
                createUser();
                break;
            case 3:
                updateUser();
                break;
            case 4:
                deleteUser();
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
                break;
        }
    } while (true);

    return 0;
}

bool login() {
    string username, password;
    cout << "Enter username: ";
    cin >> username;

    cout << "Enter password: ";
    char ch;
    password = "";
    while ((ch = _getch()) != '\r') {
        cout << '*';
        password += ch;
    }
    cout << endl;

    if (username == "admin" && password == "admin123") {
        cout << endl;
        cout << "Login successful!\n";
        cout << endl;
        return true;
    } else {
        cout << "Invalid username or password.\n";
        return false;
    }
}

void viewUsers() {
    ifstream file("contract.txt");
    if (!file.is_open()) {
        cout << "Database file not found.\n";
        return;
    }

    cout << left << setw(5) << "ID" << setw(15) << "Username" <<setw(15) << "Position"  <<  "Salary\n";
    cout << "-----------------------------------------\n";
    User user;
    while (file >> user.id >> user.username >> user.position>> user.salary) {
        cout << left << setw(5) << user.id << setw(15) << user.username <<setw(15) << user.position  << user.salary<< endl;
    }
    file.close();

    int choice;
    do {
        cout << "Enter 0 to go back to main menu or 1 to exit: ";
        cin >> choice;
    } while (choice != 0 && choice != 1);

    if (choice == 1)
        exit(0);
}

void createUser() {
    ofstream file("contract.txt", ios::app);
    if (!file.is_open()) {
        cout << "Database file not found.\n";
        return;
    }

    User newUser;
    cout << "Enter username: ";
    cin >> newUser.username;

    cout << "Enter position: ";
    cin >> newUser.position;

    cout << "Enter salary: ";
    cin >> newUser.salary;

    int maxId = 0;
    ifstream countFile("contract.txt");
    if (countFile.is_open()) {
        User user;
        while (countFile >> user.id >> user.username >> user.position >> user.salary) {
            if (user.id > maxId) {
                maxId = user.id;
            }
        }
        countFile.close();
    }
    newUser.id = maxId + 1;

    file << newUser.id << " " << newUser.username << " " << newUser.position << " " << newUser.salary << endl;
    file.close();


   cout << "User created successfully.\n";
}

void updateUser() {
    int id;
    cout << "Enter ID of user to update: ";
    cin >> id;

    if (!isIdExists(id)) {
        cout << "User with ID " << id << " doesn't exist.\n";
        int choice;
        do {
            cout << "Enter 0 to go back to main menu or 1 to exit: ";
            cin >> choice;
        } while (choice != 0 && choice != 1);

        if (choice == 1)
            exit(0);
        return;
    }

    ifstream inFile("contract.txt");
    ofstream tempFile("temp.txt");

    User user;
    while (inFile >> user.id >> user.username >> user.position >> user.salary) {
        if (user.id == id) {
            cout << "Enter new username: ";
            cin >> user.username;

            cout << "Enter new position: ";
            cin >> user.position;

            cout << "Enter new salary: ";
            cin >> user.salary;
        }
        tempFile << user.id << " " << user.username << " " << user.position << " " << user.salary<< endl;
    }
    inFile.close();
    tempFile.close();

    remove("contract.txt");
    rename("temp.txt", "contract.txt");

    int choice;
    do {
        cout << "User updated successfully.\n";
        cout << "Enter 0 to go back to main menu or 1 to exit: ";
        cin >> choice;
    } while (choice != 0 && choice != 1);

    if (choice == 1)
        exit(0);
}

void deleteUser() {
    int id;
    cout << "Enter ID of user to delete: ";
    cin >> id;

    if (!isIdExists(id)) {
        cout << "User with ID " << id << " doesn't exist.\n";
        int choice;
        do {
            cout << "Enter 0 to go back to main menu or 1 to exit: ";
            cin >> choice;
        } while (choice != 0 && choice != 1);

        if (choice == 1)
            exit(0);
        return;
    }

    ifstream inFile("contract.txt");
    ofstream tempFile("temp.txt");

    User user;
    while (inFile >> user.id >> user.username >> user.position>> user.salary) {
        if (user.id != id)
            tempFile << user.id << " " << user.username << " " << user.position << " " << user.salary << endl;
    }
    inFile.close();
    tempFile.close();

    remove("contract.txt");
    rename("temp.txt", "contract.txt");

    int choice;
    do {
        cout << "User deleted successfully.\n";
        cout << "Enter 0 to go back to main menu or 1 to exit: ";
        cin >> choice;
    } while (choice != 0 && choice != 1);

    if (choice == 1)
        exit(0);
}

bool isIdExists(int id) {
    ifstream file("contract.txt");
    if (!file.is_open()) {
        cout << "Database file not found.\n";
        return false;
    }

    User user;
    while (file >> user.id >> user.username >> user.position >> user.salary) {
        if (user.id == id) {
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}
