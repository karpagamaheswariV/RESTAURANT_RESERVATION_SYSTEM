#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

struct User {
    string name;
    string email;
    string password;
};

struct Restaurant {
    int id;
    string name;
    string location;
    string cuisine;
};

struct Reservation {
    int reservationId;
    string userEmail;
    int restaurantId;
    string date;
    string time;
    int guests;
};

vector<User> users;
vector<Restaurant> restaurants = {
    {1, "The Gourmet Kitchen", "Downtown", "Italian"},
    {2, "Sushi World", "Uptown", "Japanese"},
    {3, "Taco Town", "Midtown", "Mexican"}
};
vector<Reservation> reservations;

void loadUsers() {
    ifstream file("users.txt");
    if (file.is_open()) {
        User user;
        while (file >> user.name >> user.email >> user.password) {
            users.push_back(user);
        }
        file.close();
    }
}

void saveUser(const User& user) {
    ofstream file("users.txt", ios::app);
    if (file.is_open()) {
        file << user.name << " " << user.email << " " << user.password << endl;
        file.close();
    }
}

bool registerUser(const string& name, const string& email, const string& password) {
    for (const auto& user : users) {
        if (user.email == email) {
            return false; // User already exists
        }
    }
    User newUser = {name, email, password};
    users.push_back(newUser);
    saveUser(newUser);
    return true;
}

bool loginUser(const string& email, const string& password) {
    for (const auto& user : users) {
        if (user.email == email && user.password == password) {
            return true;
        }
    }
    return false;
}

void displayRestaurants() {
    cout << "Available Restaurants:\n";
    for (const auto& restaurant : restaurants) {
        cout << restaurant.id << ". " << restaurant.name << " (" << restaurant.cuisine << ") - " << restaurant.location << endl;
    }
}

void loadReservations() {
    ifstream file("reservations.txt");
    if (file.is_open()) {
        Reservation reservation;
        while (file >> reservation.reservationId >> reservation.userEmail >> reservation.restaurantId >> reservation.date >> reservation.time >> reservation.guests) {
            reservations.push_back(reservation);
        }
        file.close();
    }
}

void saveReservation(const Reservation& reservation) {
    ofstream file("reservations.txt", ios::app);
    if (file.is_open()) {
        file << reservation.reservationId << " " << reservation.userEmail << " " << reservation.restaurantId << " " << reservation.date << " " << reservation.time << " " << reservation.guests << endl;
        file.close();
    }
}

int getNextReservationId() {
    if (reservations.empty()) return 1;
    return reservations.back().reservationId + 1;
}

void makeReservation(const string& userEmail) {
    displayRestaurants();
    int restaurantId;
    cout << "Enter Restaurant ID: ";
    cin >> restaurantId;

    string date, time;
    int guests;
    cout << "Enter Date (YYYY-MM-DD): ";
    cin >> date;
    cout << "Enter Time (HH:MM): ";
    cin >> time;
    cout << "Enter Number of Guests: ";
    cin >> guests;

    Reservation reservation = {getNextReservationId(), userEmail, restaurantId, date, time, guests};
    reservations.push_back(reservation);
    saveReservation(reservation);

    cout << "Reservation made successfully!\n";
}

void viewReservations(const string& userEmail) {
    cout << "Your Reservations:\n";
    for (const auto& reservation : reservations) {
        if (reservation.userEmail == userEmail) {
            cout << "ID: " << reservation.reservationId << ", Restaurant: " << reservation.restaurantId 
                 << ", Date: " << reservation.date << ", Time: " << reservation.time << ", Guests: " << reservation.guests << endl;
        }
    }
}

void cancelReservation(const string& userEmail) {
    int reservationId;
    cout << "Enter Reservation ID to cancel: ";
    cin >> reservationId;

    auto it = remove_if(reservations.begin(), reservations.end(), [&](const Reservation& reservation) {
        return reservation.reservationId == reservationId && reservation.userEmail == userEmail;
    });

    if (it != reservations.end()) {
        reservations.erase(it, reservations.end());
        cout << "Reservation canceled successfully!\n";

        ofstream file("reservations.txt");
        if (file.is_open()) {
            for (const auto& reservation : reservations) {
                file << reservation.reservationId << " " << reservation.userEmail << " " << reservation.restaurantId << " " << reservation.date << " " << reservation.time << " " << reservation.guests << endl;
            }
            file.close();
        }
    } else {
        cout << "Reservation not found or you don't have permission to cancel this reservation.\n";
    }
}

void menu() {
    cout << "1. Register\n";
    cout << "2. Login\n";
    cout << "3. Exit\n";
    cout << "Choose an option: ";
}

void userMenu() {
    cout << "1. View Restaurants\n";
    cout << "2. Make a Reservation\n";
    cout << "3. View Your Reservations\n";
    cout << "4. Cancel a Reservation\n";
    cout << "5. Logout\n";
    cout << "Choose an option: ";
}

int main() {
    loadUsers();
    loadReservations();

    int choice;
    string name, email, password;
    bool loggedIn = false;
    string loggedInUserEmail;

    while (true) {
        menu();
        cin >> choice;

        if (choice == 1) {
            cout << "Enter Name: ";
            cin >> name;
            cout << "Enter Email: ";
            cin >> email;
            cout << "Enter Password: ";
            cin >> password;
            if (registerUser(name, email, password)) {
                cout << "User registered successfully!\n";
            } else {
                cout << "User with this email already exists.\n";
            }
        } else if (choice == 2) {
            cout << "Enter Email: ";
            cin >> email;
            cout << "Enter Password: ";
            cin >> password;
            if (loginUser(email, password)) {
                cout << "Login successful!\n";
                loggedIn = true;
                loggedInUserEmail = email;

                while (loggedIn) {
                    userMenu();
                    cin >> choice;

                    if (choice == 1) {
                        displayRestaurants();
                    } else if (choice == 2) {
                        makeReservation(loggedInUserEmail);
                    } else if (choice == 3) {
                        viewReservations(loggedInUserEmail);
                    } else if (choice == 4) {
                        cancelReservation(loggedInUserEmail);
                    } else if (choice == 5) {
                        loggedIn = false;
                        cout << "Logged out successfully!\n";
                    } else {
                        cout << "Invalid option. Please try again.\n";
                    }
                }
            } else {
                cout << "Invalid email or password.\n";
            }
        } else if (choice == 3) {
            break;
        } else {
            cout << "Invalid option. Please try again.\n";
        }
    }

    return 0;
}
