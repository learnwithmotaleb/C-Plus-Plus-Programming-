#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;

struct Customer {
    int roomNumber;
    string name;
    string phone;
    int days;
    float bill;
};

struct FoodItem {
    int id;
    string name;
    float price;
};

vector<FoodItem> menu = {
    {1, "Burger", 5.99},
    {2, "Pizza", 8.99},
    {3, "Pasta", 7.49},
    {4, "Salad", 4.99},
    {5, "Drink", 1.99},
    {6, "Coffee", 2.99}
};

void roomSystem();
void acRoom();
void nonAcRoom();
void displayCustomers();
void checkout();
void foodOption();
void foodMenu();
float placeOrder();

int main() {
    cout << "\n=====Hotel Management System=====\n";

    while (true) {
        cout << "\n1: Room System";
        cout << "\n2: View Customer Records";
        cout << "\n3: Food Item";
        cout << "\n4: Check Out";
        cout << "\n5: Exit";
        
        int option;
        cout << "\n\nChoose Your Option: ";
        cin >> option;

        switch (option) {
            case 1: roomSystem(); break;
            case 2: displayCustomers(); break;
            case 3: foodOption(); break;
            case 4: checkout(); break;
            case 5: exit(0); break;
            default: cout << "Please Enter a Correct Choice\n"; break;
        }
    }
    return 0;
}

void roomSystem() {
    cout << "\n1: AC ROOM (per day $100.0)";
    cout << "\n2: NON-AC ROOM (per day $65.0)";
    cout << "\n3: Exit";

    int select;
    cout << "\n\nSelected Room: ";
    cin >> select;

    switch (select) {
        case 1: acRoom(); break;
        case 2: nonAcRoom(); break;
        case 3: exit(0); break;
        default: cout << "Please select a valid option\n"; break;
    }
}

void acRoom() {
    Customer customer;
    cout << "\nWelcome to AC Room Booking System\n";

    cout << "Enter Room Number: ";
    cin >> customer.roomNumber;
    cin.ignore();
    cout << "Enter Your Name: ";
    getline(cin, customer.name);
    cout << "Enter Your Phone Number: ";
    cin >> customer.phone;
    cout << "Enter Number of Days: ";
    cin >> customer.days;

    customer.bill = customer.days * 100.0;

    ofstream outFile("customers.dat", ios::app | ios::binary);
    if (!outFile) {
        cout << "Error opening file!\n";
        return;
    }
    outFile.write(reinterpret_cast<char*>(&customer), sizeof(Customer));
    outFile.close();

    cout << "AC Room booked successfully! Bill: $" << fixed << setprecision(2) << customer.bill << "\n";
}

void nonAcRoom() {
    Customer customer;
    cout << "\nWelcome to Non-AC Room Booking System\n";

    cout << "Enter Room Number: ";
    cin >> customer.roomNumber;
    cin.ignore();
    cout << "Enter Your Name: ";
    getline(cin, customer.name);
    cout << "Enter Your Phone Number: ";
    cin >> customer.phone;
    cout << "Enter Number of Days: ";
    cin >> customer.days;

    customer.bill = customer.days * 65.0;

    ofstream outFile("customers.dat", ios::app | ios::binary);
    if (!outFile) {
        cout << "Error opening file!\n";
        return;
    }
    outFile.write(reinterpret_cast<char*>(&customer), sizeof(Customer));
    outFile.close();

    cout << "Non-AC Room booked successfully! Bill: $" << fixed << setprecision(2) << customer.bill << "\n";
}

void displayCustomers() {
    Customer customer;
    ifstream inFile("customers.dat", ios::binary);
    if (!inFile) {
        cout << "No records found!\n";
        return;
    }

    cout << "\n=== Customer Records ===\n";
    while (inFile.read(reinterpret_cast<char*>(&customer), sizeof(Customer))) {
        cout << "Room Number: " << customer.roomNumber << "\n";
        cout << "Name: " << customer.name << "\n";
        cout << "Phone: " << customer.phone << "\n";
        cout << "Days: " << customer.days << "\n";
        cout << "Bill: $" << fixed << setprecision(2) << customer.bill << "\n\n";
    }
    inFile.close();
}

void checkout() {
    int roomNumber;
    bool found = false;
    cout << "\nEnter Room Number for Checkout: ";
    cin >> roomNumber;

    ifstream inFile("customers.dat", ios::binary);
    ofstream outFile("temp.dat", ios::binary);
    if (!inFile || !outFile) {
        cout << "Error processing records!\n";
        return;
    }

    Customer customer;
    while (inFile.read(reinterpret_cast<char*>(&customer), sizeof(Customer))) {
        if (customer.roomNumber == roomNumber) {
            found = true;
            cout << "Checkout successful for Room " << customer.roomNumber
                 << ". Total Bill: $" << fixed << setprecision(2) << customer.bill << "\n";
        } else {
            outFile.write(reinterpret_cast<char*>(&customer), sizeof(Customer));
        }
    }

    inFile.close();
    outFile.close();
    remove("customers.dat");
    rename("temp.dat", "customers.dat");

    if (!found) {
        cout << "Room not found!\n";
    }
}

void foodOption() {
    while (true) {
        cout << "\n=== Food Option ===\n";
        cout << "1. View Menu\n";
        cout << "2. Place Order\n";
        cout << "3. Exit\n";

        int choice;
        cout << "Enter Your Choice: ";
        cin >> choice;

        switch (choice) {
            case 1: foodMenu(); break;
            case 2: {
                float total = placeOrder();
                cout << "Total Bill: $" << fixed << setprecision(2) << total << "\n";
                break;
            }
            case 3: return;
            default: cout << "Enter a valid option\n"; break;
        }
    }
}

void foodMenu() {
    cout << "\n=== Food Menu ===\n";
    for (const auto& item : menu) {
        cout << item.id << ". " << item.name << " - $" << fixed << setprecision(2) << item.price << "\n";
    }
}

float placeOrder() {
    int itemId, quantity;
    float total = 0;
    char more;

    do {
        foodMenu();
        cout << "Enter the item ID to order: ";
        cin >> itemId;

        if (itemId < 1 || itemId > menu.size()) {
            cout << "Invalid item ID! Please try again.\n";
            continue;
        }

        cout << "Enter quantity for " << menu[itemId - 1].name << ": ";
        cin >> quantity;

        total += menu[itemId - 1].price * quantity;
        cout << quantity << " x " << menu[itemId - 1].name << " added to your order. Subtotal: $" << total << "\n";

        cout << "Do you want to order more items? (y/n): ";
        cin >> more;
    } while (more == 'y' || more == 'Y');

    return total;
}
