
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <ctime>
using namespace std;

// Transaction Class
class Transaction {
public:
    string type;
    double amount;
    string details;
    string dateTime;

    Transaction(string t, double a, string d) {
        type = t;
        amount = a;
        details = d;

        time_t now = time(0);
        dateTime = ctime(&now);
        if (!dateTime.empty() && dateTime.back() == '\n')
            dateTime.pop_back();
    }

    void display() const {
        cout << left << setw(15) << type
             << setw(12) << fixed << setprecision(2) << amount
             << details << "\n";
        cout << "  Date: " << dateTime << "\n";
    }
};

// Account Class
class Account {
public:
    int accountNumber;
    double balance;
    vector<Transaction> transactions;

    Account(int accNo, double initialBalance = 0) {
        accountNumber = accNo;
        balance = initialBalance;
    }

    void deposit(double amount) {
        if (amount <= 0) {
            cout << "Invalid deposit amount!\n";
            return;
        }

        balance += amount;
        transactions.emplace_back(
            "Deposit", amount, "Money deposited"
        );
        cout << "Deposit successful!\n";
    }

    bool withdraw(double amount) {
        if (amount <= 0) {
            cout << "Invalid withdrawal amount!\n";
            return false;
        }

        if (amount > balance) {
            cout << "Insufficient balance!\n";
            return false;
        }

        balance -= amount;
        transactions.emplace_back(
            "Withdrawal", amount, "Money withdrawn"
        );
        cout << "Withdrawal successful!\n";
        return true;
    }

    void addTransferRecord(double amount, const string& details) {
        transactions.emplace_back("Transfer", amount, details);
    }

    void displayAccount() const {
        cout << "\nAccount Number: " << accountNumber << "\n";
        cout << "Current Balance: Rs. "
             << fixed << setprecision(2) << balance << "\n";
    }

    void showTransactions(int limit = 5) const {
        cout << "\n----- Recent Transactions -----\n";

        if (transactions.empty()) {
            cout << "No transactions found.\n";
            return;
        }

        int start = (transactions.size() > limit)
                    ? transactions.size() - limit : 0;

        for (int i = static_cast<int>(transactions.size()) - 1;
             i >= start; i--) {
            transactions[i].display();
            cout << "------------------------------\n";
        }
    }
};

// Customer Class
class Customer {
public:
    int customerId;
    string name;
    string phone;
    vector<Account> accounts;

    Customer(int id, string n, string p) {
        customerId = id;
        name = n;
        phone = p;
    }

    void addAccount(int accNo) {
        accounts.emplace_back(accNo);
        cout << "Account created successfully!\n";
    }

    Account* findAccount(int accNo) {
        for (auto& acc : accounts) {
            if (acc.accountNumber == accNo)
                return &acc;
        }
        return nullptr;
    }

    void displayCustomer() const {
        cout << "\nCustomer ID: " << customerId << "\n";
        cout << "Name: " << name << "\n";
        cout << "Phone: " << phone << "\n";

        for (const auto& acc : accounts)
            acc.displayAccount();
    }
};

// Banking Management System
class Bank {
private:
    vector<Customer> customers;
    int nextCustomerId = 1001;
    int nextAccountNumber = 5001;

public:
    void createCustomer() {
        string name, phone;

        cout << "Enter customer name: ";
        getline(cin >> ws, name);

        cout << "Enter phone number: ";
        getline(cin >> ws, phone);

        customers.emplace_back(nextCustomerId++, name, phone);
        cout << "Customer created! Customer ID: "
             << customers.back().customerId << "\n";
    }

    Customer* findCustomer(int id) {
        for (auto& customer : customers) {
            if (customer.customerId == id)
                return &customer;
        }
        return nullptr;
    }

    Account* findAccount(int accNo) {
        for (auto& customer : customers) {
            Account* acc = customer.findAccount(accNo);
            if (acc != nullptr)
                return acc;
        }
        return nullptr;
    }

    void createAccount() {
        int id;
        cout << "Enter customer ID: ";
        cin >> id;

        Customer* customer = findCustomer(id);

        if (customer == nullptr) {
            cout << "Customer not found!\n";
            return;
        }

        customer->addAccount(nextAccountNumber++);
        cout << "Account Number: "
             << nextAccountNumber - 1 << "\n";
    }

    void depositMoney() {
        int accNo;
        double amount;

        cout << "Enter account number: ";
        cin >> accNo;

        Account* acc = findAccount(accNo);
        if (acc == nullptr) {
            cout << "Account not found!\n";
            return;
        }

        cout << "Enter deposit amount: Rs. ";
        cin >> amount;
        acc->deposit(amount);
    }

    void withdrawMoney() {
        int accNo;
        double amount;

        cout << "Enter account number: ";
        cin >> accNo;

        Account* acc = findAccount(accNo);
        if (acc == nullptr) {
            cout << "Account not found!\n";
            return;
        }

        cout << "Enter withdrawal amount: Rs. ";
        cin >> amount;
        acc->withdraw(amount);
    }

    void transferMoney() {
        int fromNo, toNo;
        double amount;

        cout << "Enter sender account number: ";
        cin >> fromNo;
        cout << "Enter receiver account number: ";
        cin >> toNo;
        cout << "Enter transfer amount: Rs. ";
        cin >> amount;

        Account* sender = findAccount(fromNo);
        Account* receiver = findAccount(toNo);

        if (!sender || !receiver) {
            cout << "One or both accounts not found!\n";
            return;
        }

        if (sender == receiver) {
            cout << "Cannot transfer to the same account!\n";
            return;
        }

        if (amount <= 0 || amount > sender->balance) {
            cout << "Invalid amount or insufficient balance!\n";
            return;
        }

        sender->balance -= amount;
        receiver->balance += amount;

        sender->addTransferRecord(
            amount, "Transferred to account " + to_string(toNo)
        );
        receiver->addTransferRecord(
            amount, "Received from account " + to_string(fromNo)
        );

        cout << "Transfer successful!\n";
    }

    void viewAccount() {
        int accNo;
        cout << "Enter account number: ";
        cin >> accNo;

        Account* acc = findAccount(accNo);
        if (acc == nullptr) {
            cout << "Account not found!\n";
            return;
        }

        acc->displayAccount();
    }

    void viewTransactions() {
        int accNo, limit;
        cout << "Enter account number: ";
        cin >> accNo;

        Account* acc = findAccount(accNo);
        if (acc == nullptr) {
            cout << "Account not found!\n";
            return;
        }

        cout << "How many recent transactions? ";
        cin >> limit;

        if (limit <= 0) {
            cout << "Enter a positive number!\n";
            return;
        }
        do {
            cout << "\n====== BANKING MANAGEMENT SYSTEM ======\n";
            cout << "1. Create Customer\n";
            cout << "2. Create Account\n";
            cout << "3. Deposit Money\n";
            cout << "4. Withdraw Money\n";
            cout << "5. Transfer Funds\n";
            cout << "6. View Account Information\n";
            cout << "7. View Recent Transactions\n";
            cout << "8. View All Customers\n";
            cout << "9. Exit\n";
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1: createCustomer(); break;
                case 2: createAccount(); break;
                case 3: depositMoney(); break;
                case 4: withdrawMoney(); break;
                case 5: transferMoney(); break;
                case 6: viewAccount(); break;
                case 7: viewTransactions(); break;
                case 8: viewCustomers(); break;
                case 9: cout << "Thank you for using our bank!\n"; break;
                default: cout << "Invalid choice! Try again.\n";
            }
        } while (choice != 9);
    }
};

int main() {
    Bank bank;
    bank.menu();
    return 0;
}
