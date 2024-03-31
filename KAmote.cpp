#include <iostream>
#include <vector>
#include <fstream>
#include <unistd.h>
#include <cstdio>
#include <string>
#include <ctime>
#include <iomanip>
#include <conio.h>
#include <algorithm>
#include <limits>

using namespace std;

struct ParkedVehicle {
    char row;
    int Slot;
    string name;
    time_t entryTime;
    time_t exitTime;
    double receiptAmount;

    ParkedVehicle() : exitTime(0), receiptAmount(20.0) {}

    double calculateDuration() const {
        return difftime(exitTime, entryTime);
    }

    string getFormattedEntryTime() const {
        char buffer[80];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtime(&entryTime));
        return buffer;
    }

    string getFormattedExitTime() const {
        if (exitTime == 0) {
            return "Not yet removed";
        }

        char buffer[80];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtime(&exitTime));
        return buffer;
    }
};

string getPassword(char sp) {
    enum IN {
        IN_BACK = 8,
        IN_RET = 13
    };
    string passwd = "";
    char ch_ipt;

    while (true) {
        ch_ipt = _getch();

        if (ch_ipt == IN::IN_RET) {
            cout << endl;
            return passwd;
        } else if (ch_ipt == IN::IN_BACK && passwd.length() != 0) {
            passwd.pop_back();
            cout << "\b \b";
            continue;
        } else if (ch_ipt == IN::IN_BACK && passwd.length() == 0) {
            continue;
        }

        passwd.push_back(ch_ipt);
        cout << sp;
    }
}

class ParkingLot {
private:
    vector<vector<bool>> slots;
    const string defaultSaveFilePath = R"(C:\Users\myPC\Documents\Parking_Lot_end.cpp)";
    string loadFilePath;
    string username;
    string password;
    vector<ParkedVehicle> parkedVehicles;

public:
    ParkingLot() : slots(4, vector<bool>(20, false)), loadFilePath(defaultSaveFilePath) {
        username = "Parking";
        password = "NEUST12345";
    }

    void clearScreen() {
        system("cls");
    }

    void displayParkingSlots() {
        clearScreen();
        cout << "Parking Slots:" << endl;

        for (int Slot = 1; Slot <= 20; ++Slot) {
            cout << "A-" << Slot << "\t\tB-" << Slot << "\t\tC-" << Slot << "\t\tD-" << Slot << "\t\t";
            cout << endl;
        }
    }

    void checkOccupancy() {
        clearScreen();
        cout << "Occupancy Status:" << endl;

        int parkedInA = 0, parkedInB = 0, parkedInC = 0, parkedInD = 0;

        for (int Slot = 1; Slot <= 20; ++Slot) {
            if (slots[0][Slot - 1]) {
                ++parkedInA;
            }
            if (slots[1][Slot - 1]) {
                ++parkedInB;
            }
            if (slots[2][Slot - 1]) {
                ++parkedInC;
            }
            if (slots[3][Slot - 1]) {
                ++parkedInD;
            }

            cout << Slot << "-A " << (slots[0][Slot - 1] ? "X" : "O") << "\t\t";
            cout << Slot << "-B " << (slots[1][Slot - 1] ? "X" : "O") << "\t\t";
            cout << Slot << "-C " << (slots[2][Slot - 1] ? "X" : "O") << "\t\t";
            cout << Slot << "-D " << (slots[3][Slot - 1] ? "X" : "O") << "\t\t";
            cout << endl;
        }

        cout << "Total vehicles parked in A: " << parkedInA << endl;
        cout << "Total vehicles parked in B: " << parkedInB << endl;
        cout << "Total vehicles parked in C: " << parkedInC << endl;
        cout << "Total vehicles parked in D: " << parkedInD << endl;
    }

    void parkVehicle() {
        char row;
        int Slot;
        string name;

        clearScreen();

        displayParkingSlots();
        checkOccupancy();

        cout << "Enter the row slot (A, B, C, or D): ";
        cin >> row;

        if (row == 'E' || row == 'e') {
            handleExit();
            return;
        }

        cout << "Enter the slot number (1-20): ";
        cin >> Slot;

        row = toupper(row);

        while (cin.fail() || Slot < 1 || Slot > 20) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a valid slot number (1-20): ";
            cin >> Slot;
        }

        if (row >= 'A' && row <= 'D' && Slot >= 1 && Slot <= 20) {
            if (!slots[row - 'A'][Slot - 1]) {
                slots[row - 'A'][Slot - 1] = true;

                ParkedVehicle parkedVehicle;
                parkedVehicle.row = row;
                parkedVehicle.Slot = Slot;
                cout << "Enter the name of the vehicle: ";
                cin >> parkedVehicle.name;

                parkedVehicle.entryTime = time(0);

                parkedVehicles.push_back(parkedVehicle);

                cout << "Vehicle parked at " << row << "-" << Slot << " for " << parkedVehicle.name << endl;
                cout << "Entry time: " << put_time(localtime(&parkedVehicle.entryTime), "%Y-%m-%d %H:%M:%S") << endl;

                checkOccupancy();
            } else {
                cout << "Sorry, the slot " << row << "-" << Slot << " is already occupied. Please choose another slot." << endl;
                system("pause");
                parkVehicle();
            }
        } else {
            cout << "Invalid row or slot number. Please try again." << endl;
            parkVehicle();
        }
    }

    void removeVehicle() {
        char row;
        int Slot;

        do {
            clearScreen();

            displayParkingSlots();
            checkOccupancy();

            cout << "Enter the row (A, B, C, or D) of the vehicle to remove: ";
            cin >> row;

            if (row == 'E' || row == 'e') {
                handleExit();
                return;
            }

            cout << "Enter the slot number (1-20) of the vehicle to remove: ";
            cin >> Slot;

            row = toupper(row);

            while (cin.fail() || Slot < 1 || Slot > 20) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please enter a valid slot number (1-20): ";
                cin >> Slot;
            }

            auto it = find_if(parkedVehicles.begin(), parkedVehicles.end(),
                              [&](const ParkedVehicle &vehicle) {
                                  return vehicle.row == row && vehicle.Slot == Slot;
                              });

            if (it != parkedVehicles.end()) {
                if (it->exitTime == 0) {
                    it->exitTime = time(0);
                }

                double duration = it->calculateDuration();

                clearScreen();

                cout << "Vehicle at " << row << "-" << Slot << "  " << it->name << " has left." << endl;
                cout << "Parked for: " << fixed << setprecision(2) << duration / 3600.0 << " hours" << endl;
                cout << "Receipt Amount: " << it->receiptAmount << " pesos" << endl;
                cout << "Entry time: " << it->getFormattedEntryTime() << endl;
                cout << "Exit time: " << it->getFormattedExitTime() << endl;
                slots[row - 'A'][Slot - 1] = false;

                int penaltyAmount = 0;

                char confirmLostReceipt;
                cout << "Is the receipt lost? (Y/N): ";
                cin >> confirmLostReceipt;

                if (confirmLostReceipt == 'Y' || confirmLostReceipt == 'y') {
                    cout << "Lost receipt penalty: ";
                    cin >> penaltyAmount;

                    cout << "You must pay for the expenses resulting from the missed receipt. \nPenalty amount: " << penaltyAmount << " pesos." << endl;

                    it->receiptAmount += penaltyAmount;
                }

                cout << "Total Amount to be Paid: " << it->receiptAmount << " pesos" << endl;
                system("pause");
                checkOccupancy();
            } else {
                cout << "No vehicle found at " << row << "-" << Slot << ". Please enter a valid occupied slot." << endl;
            }

            cout << "Do you want to remove another vehicle? (Y/N): ";
            char anotherRemoval;
            cin >> anotherRemoval;

            if (anotherRemoval != 'Y' && anotherRemoval != 'y') {
                clearScreen();
                break;
            }

        } while (true);
    }

    void displayHistory() {
        clearScreen();

        cout << "Parking History:" << endl;

        for (const auto &vehicle : parkedVehicles) {
            cout << "Row: " << vehicle.row << "\tslot: " << vehicle.Slot << "\tName: " << vehicle.name << "\tEntry: " << vehicle.getFormattedEntryTime() << "\tExit: " << vehicle.getFormattedExitTime() << endl;
        }

        system("pause");
    }

    void resetSaveFile() {
        ofstream outFile(defaultSaveFilePath);

        if (outFile.is_open()) {
            for (auto &row : slots) {
                fill(row.begin(), row.end(), false);
            }

            outFile.close();
            cout << "Save file reset successfully." << endl;
        } else {
            cout << "Error resetting save file." << endl;
        }
    }

    void resetEverything() {
        for (auto &row : slots) {
            fill(row.begin(), row.end(), false);
        }

        resetSaveFile();

        cout << "Everything reset successfully." << endl;
    }

    void displayMenu() {
        cout << "Choose Option" << endl;
        cout << "A. Park a vehicle" << endl;
        cout << "B. Display parking slots" << endl;
        cout << "C. Display occupancy status" << endl;
        cout << "D. Remove a parked vehicle" << endl;
        cout << "E. Reset save file" << endl;
        cout << "F. Reset everything" << endl;
        cout << "G. Display Parking History" << endl;
        cout << "H. Exit" << endl;
    }

    void processChoice(char choice) {
        switch (choice) {
            case 'A':
                parkVehicle();
                break;
            case 'B':
                displayParkingSlots();
                break;
            case 'C':
                checkOccupancy();
                break;
            case 'D':
                removeVehicle();
                break;
            case 'E':
                clearScreen();
                resetSaveFile();
                break;
            case 'F':
                clearScreen();
                resetEverything();
                break;
            case 'G':
                displayHistory();
                break;
            case 'H':
                handleExit();
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    }

    void runParkingLot() {
        while (true) {
            if (authenticate()) {
                loadParkingLotState();

                char choice;

                while (true) {
                    displayMenu();
                    cout << "Enter your choice (A/B/C/D/E/F/G/H): ";
                    cin >> choice;

                    choice = toupper(choice);
                    processChoice(choice);
                }
            } else {
                cout << "Authentication failed. Please try again." << endl;
            }
        }
    }

    void handleExit() {
        char confirmExit;
        cout << "Do you want to save the parking lot state before exiting? if Yes Enter y/Y if No Enter n/N. (Y/N): ";
        cin >> confirmExit;

        if (confirmExit == 'Y' || confirmExit == 'y') {
            saveParkingLotState();
        }

        cout << "Exiting the parking lot application. Goodbye!" << endl;
        exit(0);
    }

    void saveParkingLotState() {
        ofstream outFile(defaultSaveFilePath);

        if (outFile.is_open()) {
            for (const auto &row : slots) {
                for (const bool &occupied : row) {
                    outFile << (occupied ? 1 : 0) << " ";
                }
                outFile << "\n";
            }

            outFile.close();
            cout << "Parking lot state saved successfully to " << defaultSaveFilePath << "." << endl;
        } else {
            cout << "Unable to save parking lot state to " << defaultSaveFilePath << ". Check file permissions or path." << endl;
        }
    }

    void loadParkingLotState() {
        ifstream inFile(defaultSaveFilePath);

        if (inFile.is_open() && inFile.peek() != std::ifstream::traits_type::eof()) {
            for (auto &row : slots) {
                for (size_t i = 0; i < row.size(); ++i) {
                    int value;
                    inFile >> value;
                    row[i] = (value == 1);
                }
            }

            inFile.close();
            cout << "Parking lot state loaded successfully." << endl;
        } else {
            cout << "No saved parking lot state found." << endl;
        }
    }

    bool authenticate() {
        string enteredUsername, enteredPassword;

        do {
            clearScreen();
            cout << "\t\t\t" << "Parking Lot System" << endl << endl;
            cout << " Log-in first" << endl << endl;
            cout << "Enter username: ";
            cin >> enteredUsername;
            cout << "Enter password: ";
            enteredPassword = getPassword('*');

            if(enteredUsername == username && enteredPassword == password) {
                clearScreen();
                return true;
            } else {
                cout << "Invalid username or password. Please try again." << endl;
                system("pause");
            }

        } while (true);

        return false;
    }
};

int main() {
   chdir(R"(C:\Users\myPC\Documents\)");
    ParkingLot parkingLot;
    parkingLot.runParkingLot();

    return 0;
}


