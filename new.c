#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define function to clear console screen
void clearScreen() {
    #ifdef _WIN32 // For Windows
    system("cls");
    #else // For Linux and macOS
    system("clear");
    #endif
}
#define MAX_TIME_LENGTH 10
#define MAX_LOGIN_ATTEMPTS 3
#define MAX_SEATS 50
#define MAX_USERNAME_LENGTH 50
#define MAX_PASSWORD_LENGTH 50
#define MAX_ROUTES 100
#define MAX_ROUTE_NAME_LENGTH 50
#define MAX_ORIGIN_DEST_LENGTH 50
#define MAX_BUS_NAME_LENGTH 50

// Admin credentials
const char ADMIN_USERNAME[] = "admin";
char ADMIN_PASSWORD[MAX_PASSWORD_LENGTH] = "adminpass";
const char SECURITY_QUESTION[] = "What is the capital of France?";
const char SECURITY_ANSWER[] = "Paris";


// Structure to represent a bus route
typedef struct {
    char bus_name[MAX_BUS_NAME_LENGTH];
    char route_name[MAX_ROUTE_NAME_LENGTH];
    char origin[MAX_ORIGIN_DEST_LENGTH];
    char destination[MAX_ORIGIN_DEST_LENGTH];
    char departure_time[10]; // Assuming departure time format HH:MM
    char arrival_time[10];
    float ticket_price;
    int deleted_flag; // Flag to indicate if the route is deleted (0 for not deleted, 1 for deleted)
} BusRoute;

// Structure to represent a seat
typedef struct {
    int seat_number;
    float price;
    int is_available;
} Seat;

// Structure to represent a user
typedef struct {
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    char profile[MAX_USERNAME_LENGTH];
    char security_question[100];
    char security_answer[100];
} User;

// Function prototypes
void signoutTravelers();
void signoutAdmin();
void resetPassword(User *traveler);
void resetAdminPassword();
void writePasswordToFile(const char *password);
void readPasswordFromFile();
void adminMenu();
void travelerMenu(User *traveler);
void viewProfile(User user);
void updateProfile(User *user);
void bus();
void travelerRegister(User *traveler);
int adminLogin();
int travelerLogin(User *traveler);
void make_reservation(Seat seats[], int num_seats, User user);
void calculate_total_cost(Seat seats[], int num_seats, int selected_seats[]);
void save_seat_data(Seat seats[], int num_seats, char busname[], char routename[], char origin[], char destination[], char arrivaltime[]);
void busroutemanagement();
void initializeSeats(Seat seats[], int num_seats);
void displaySeatStatus(Seat seats[], int num_seats, char busname[], char routename[], char origin[], char destination[], char arrivaltime[]);
void updateSeatStatus(Seat seats[], int num_seats, char busname[], char routename[], char origin[], char destination[], char arrivaltime[]);
void updatePrices();
void browseSeats();
void makeReservation();
void viewTravelerBookings();
void cancelTravelerBooking();
void AddBusRoute();
void UpdateBusRoute();
void DeleteBusRoute();
void displayBusRoutes();
void AssignBusNameAndPrice();
void seatmanagement();
void displayRoutePrices();
void updatePrices();
void pricemanagement();
void viewBookings();
void cancelBooking();
void bookingmanagement();
void searchBusRoutes();
void clearInputBuffer();
    char ch[10][130]={"Cardiff Express","Belfast Express","Derby Express","Chester Express","Newport Express","Truro Expre"};
int main() {
    clearScreen(); // Clear screen when program starts
    User admin;
    int role;
    printf("\n\n                                  ================================================");
    printf("\n    **********************************  Welcome to Bus Ticket Reservation System  **********************************");
    printf("\n\n                                  =================================================");
    printf("\n\n                                                1. Administrator Login");
    printf("\n\n                                                2. Traveler login");
    printf("\n\n                                                Choose your Option: ");
    scanf("%d", &role);
    clearScreen(); // Clear screen after user input
    if (role == 1) {
        readPasswordFromFile(); // Read password from file
        if (adminLogin()) {
            adminMenu();
        }
    } else if (role == 2) {
        int loginChoice;
        User traveler;
        printf("\n\n                   **************************************************************");
        printf("\n\n\n                                             1. Login");
        printf("\n\n                                               Are you a new User?");
        printf("\n\n                                               2. Register");
        printf("\n\n\n                                             Enter your choice: ");
        scanf("%d", &loginChoice);
        if (loginChoice == 1) {
            if (travelerLogin(&traveler)) {
                travelerMenu(&traveler);
            }
        } else if (loginChoice == 2) {
            travelerRegister(&traveler);
            travelerMenu(&traveler);
        } else {
            printf("\n Invalid choice! Exiting...");
        }
    } else {
        printf("\n Invalid role! Exiting...");
    }
    return 0;
}
// Admin login function
int adminLogin() {
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    int attempts = 0;

    clearScreen();

    do {
        printf("\n                                             =============================================");
        printf("\n                           **********************************  Administrator Login  ************");
        printf("\n                                             =============================================");

        printf("\n\n                                                          Username: ");
        scanf("%s", username);
        printf("\n                                                            Password: ");
        scanf("%s", password);

        // Check if username and password are correct
        if (strcmp(username, ADMIN_USERNAME) == 0 && strcmp(password, ADMIN_PASSWORD) == 0) {
            printf("\n\n                                                      Login successful!");
            return 1;
        } else {
            attempts++;
            printf("\n\n\n                                                  * Incorrect username or password!");

            if (attempts < MAX_LOGIN_ATTEMPTS) {
                printf(" You have %d attempts left.", MAX_LOGIN_ATTEMPTS - attempts);
            } else {
                printf(" You have reached the maximum number of login attempts.");
                char choice;
                printf("\n\n\n                                                  * Do you want to reset your password? (Y/N): ");
                scanf(" %c", &choice);
                if (choice == 'Y' || choice == 'y') {
                    resetAdminPassword();
                    return 0;
                } else {
                    return 0;
                }
            }
        }
    } while (attempts < MAX_LOGIN_ATTEMPTS);

    return 0;
}

void resetAdminPassword() {
    char answer[100];
    char new_password[MAX_PASSWORD_LENGTH];

    clearScreen();

    printf("\n\n\n                                                  * To reset your password, please answer the following security question:");
    printf("\n                                                  * %s\n", SECURITY_QUESTION);
    printf("\n\n\n                                                  * Your Answer: ");
    scanf("%s", answer);

    if (strcmp(answer, SECURITY_ANSWER) == 0) {
        printf("\n\n\n                                                  * Security question verified! Please enter your new password: ");
        scanf("%s", new_password);

        // Update the password
        strcpy(ADMIN_PASSWORD, new_password);
        writePasswordToFile(new_password);
        printf("\n\n\n                                                  * Password updated successfully!");
    } else {
        printf("\n\n\n                                                  * Incorrect answer to security question. Password reset failed.");
    }

    printf("\n\n\n                                                  * Press Enter to continue...");
    while (getchar() != '\n');
    getchar();

    // Go back to login phase
    adminLogin();
    clearScreen();
    adminMenu();
}
// Function to write password to file
void writePasswordToFile(const char *password) {
    FILE *file = fopen("admin_password.txt", "w");
    if (file != NULL) {
        fputs(password, file);
        fclose(file);
    } else {
        printf("\n\n\n                                                  * Error writing password to file.");
    }
}

// Function to read password from file
void readPasswordFromFile() {
    FILE *file = fopen("admin_password.txt", "r");
    if (file != NULL) {
        fgets(ADMIN_PASSWORD, MAX_PASSWORD_LENGTH, file);
        fclose(file);
    } else {
        printf("\n\n\n                                                  * Error reading password from file.");
    }
}

// Traveler login function
int travelerLogin(User *traveler) {
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    int attempts = 0;
    printf("\n=====================================");
    printf("\n Traveler Login");
    printf("\n=====================================");
    printf("\n Enter your username: ");
    scanf("%s", username);
    printf(" Enter your password: ");
    scanf("%s", password);

    // Check if username and password match with registered traveler
    FILE *file = fopen("travelers.txt", "r");
    if (file != NULL) {
        char tempUsername[MAX_USERNAME_LENGTH], tempPassword[MAX_PASSWORD_LENGTH];
        char tempSecurityQuestion[100], tempSecurityAnswer[100];
        while (fscanf(file, "%s %s %s %s\n", tempUsername, tempPassword, tempSecurityQuestion, tempSecurityAnswer) != EOF) {
            if (strcmp(username, tempUsername) == 0 && strcmp(password, tempPassword) == 0) {
                fclose(file);
                printf("\n Login successful!");
                strcpy(traveler->username, tempUsername);
                strcpy(traveler->password, tempPassword);
                strcpy(traveler->security_question, tempSecurityQuestion);
                strcpy(traveler->security_answer, tempSecurityAnswer);
                strcpy(traveler->profile, "traveler");
                return 1;
            }
        }
        fclose(file);
    }

    do {
        attempts++;
        printf("\n Incorrect username or password! Please try again.");
        if (attempts < MAX_LOGIN_ATTEMPTS) {
            printf("\n Remaining attempts: %d", MAX_LOGIN_ATTEMPTS - attempts);
            printf("\n Enter your username: ");
            scanf("%s", username);
            printf(" Enter your password: ");
            scanf("%s", password);
            file = fopen("travelers.txt", "r");
            if (file != NULL) {
                char tempUsername[MAX_USERNAME_LENGTH], tempPassword[MAX_PASSWORD_LENGTH];
                char tempSecurityQuestion[100], tempSecurityAnswer[100];
                while (fscanf(file, "%s %s %s %s\n", tempUsername, tempPassword, tempSecurityQuestion, tempSecurityAnswer) != EOF) {
                    if (strcmp(username, tempUsername) == 0 && strcmp(password, tempPassword) == 0) {
                        fclose(file);
                        printf("\n Login successful!");
                        strcpy(traveler->username, tempUsername);
                        strcpy(traveler->password, tempPassword);
                        strcpy(traveler->security_question, tempSecurityQuestion);
                        strcpy(traveler->security_answer, tempSecurityAnswer);
                        strcpy(traveler->profile, "traveler");
                        return 1;
                    }
                }
                fclose(file);
            }
        } else {
            printf("\n Maximum login attempts reached. Do you want to reset your password? (Y/N): ");
            char choice;
            scanf(" %c", &choice);
            if (choice == 'Y' || choice == 'y') {
                resetPassword(traveler);
            } else {
                printf("\n Exiting...");
            }
            return 0;
        }
    } while (attempts < MAX_LOGIN_ATTEMPTS);

    return 0;
}

// Register new travelerwer
void travelerRegister(User *traveler) {
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    char security_question[100];
    char security_answer[100];

    printf("\n                                =====================================");
    printf("\n                                      Traveler Registration");
    printf("\n                                =====================================");

    printf("\n                                       Enter your Username: ");
    scanf("%s", username);

    printf("\n                                       Enter your password: ");
    scanf("%s", password);

    printf("\n                                        Enter security question: ");
    clearInputBuffer(); // Clear input buffer before reading the question
    fgets(security_question, sizeof(security_question), stdin);
    security_question[strcspn(security_question, "\n")] = '\0'; // Remove newline character

    printf("\n                                        Enter security answer: ");
    fgets(security_answer, sizeof(security_answer), stdin);
    security_answer[strcspn(security_answer, "\n")] = '\0'; // Remove newline character

    // Write new traveler details to file
    FILE *file = fopen("travelers.txt", "a");
    if (file != NULL) {
        fprintf(file, "%s %s %s %s %s\n", username, password, "traveler", security_question, security_answer);
        fclose(file);
        printf("\n Registration successful! Please login now.");
        strcpy(traveler->username, username);
        strcpy(traveler->password, password);
        strcpy(traveler->profile, "traveler");
        strcpy(traveler->security_question, security_question);
        strcpy(traveler->security_answer, security_answer);
    } else {
        printf("\n Error registering traveler.");
    }
}
void resetPassword(User *traveler) {
    char security_question[100];
    char security_answer[100];

    // Display the security question
    printf("\n Please answer the security question to reset your password: %s", traveler->security_question);

    printf("\n Your Answer: ");
    clearInputBuffer(); // Clear input buffer before reading the answer
    fgets(security_answer, sizeof(security_answer), stdin);
    security_answer[strcspn(security_answer, "\n")] = '\0'; // Remove newline character

    // Check if the security answer matches
    if (strcmp(security_answer, traveler->security_answer) == 0) {
        char new_password[MAX_PASSWORD_LENGTH];
        printf("\n Security question verified. Please enter your new password: ");
        scanf("%s", new_password);

        // Update password in the file
        FILE *file = fopen("travelers.txt", "r+");
        if (file != NULL) {
            char tempUsername[MAX_USERNAME_LENGTH], tempPassword[MAX_PASSWORD_LENGTH], tempProfile[20], tempSecurityQuestion[100], tempSecurityAnswer[100];
            while (fscanf(file, "%s %s %s %s %s\n", tempUsername, tempPassword, tempProfile, tempSecurityQuestion, tempSecurityAnswer) != EOF) {
                if (strcmp(tempUsername, traveler->username) == 0) {
                    fprintf(file, "%s %s %s %s %s\n", tempUsername, new_password, tempProfile, traveler->security_question, traveler->security_answer);
                }
            }
            fclose(file);
            printf("\n Password reset successfully!");
            return; // Exit function after password reset
        } else {
            printf("\n Error resetting password. Please try again later.");
        }
    } else {
        printf("\n Incorrect security answer. Password reset failed.");
    }
}

void signoutTravelers(){
     
     clearScreen();
           int loginChoice;
      User traveler;
      clearScreen();
      printf("\n\n                   **************************************************************");
      printf("\n\n\n                                             1. Login");
      printf("\n\n                                               Are you a new User?");
      printf("\n\n                                               2. Register");
      printf("\n\n\n                                             Enter your choice: ");
      scanf("%d", &loginChoice);
      if (loginChoice == 1) {
          if (travelerLogin(&traveler)) {
              travelerMenu(&traveler);
          }
      } else if (loginChoice == 2) {
          travelerRegister(&traveler);
          travelerMenu(&traveler);
      } else  {
          printf("\n Invalid choice! Exiting...");
      }
 }
void signoutAdmin(){
    clearScreen();
 adminLogin();
 clearScreen();
 adminMenu();
}
// Admin menu function
void adminMenu() {
    User admin;
    int choice;
    do {
        clearScreen(); // Clear screen before displaying menu
        printf("\n\n                      ================================================");
        printf("\n  ********************************  Welcome to the Admin Panel  *********************************");
        printf("\n                      ================================================");
        printf("\n\n                                   1. Bus Information Management");
        printf("\n\n                                   2. Seats management");
        printf("\n\n                                   3. Booking Management");
        printf("\n\n                                   4. View Profile");
        printf("\n\n                                   5. Update Profile");
        printf("\n\n                                   6. Sign out and Exit");
        printf("\n\n                                What do you want to manage: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
              busroutemanagement(); // Implement bus route management
                break;
            case 2:
               seatmanagement();// Implement seats management
                break;
            case 3:
              bookingmanagement(); // Implement booking management
                break;
            case 4:
                viewProfile((User){.username = "admin", .profile = "admin"});
                break;
            case 5:
               updateProfile(&admin); // Implement update profile
                break;   
            case 6:
             printf("\n                            --------------------------- Signing out... --------------------------");
             clearScreen();
                signoutAdmin();
                return;
            default:
                printf("\n                       * Invalid choice! Please enter a valid option.");
        }
        printf("\n\n Press Enter to continue...");
        while (getchar() != '\n'); // Clear input buffer
        getchar(); // Wait for Enter key
        clearScreen(); // Clear screen before next iteration
    } while (1);
}

// Traveler menu function
void travelerMenu(User *traveler) {
    int choice;
    do {
        clearScreen(); // Clear screen before displaying menu
        printf("\n\n=====================================");
        printf("\n Welcome, Traveler");
        printf("\n=====================================");
        printf("\n 1. View Profile");
        printf("\n 2. Update Profile");
        printf("\n 3. Search Buses");
        printf("\n 4. View Seats");
        printf("\n 5. Make Reservation");
        printf("\n 6. View Booking Details");
        printf("\n 7. Process Payment");
        printf("\n 8. Sign out and Exit");
        printf("\n\n Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                viewProfile(*traveler);
                break;
            case 2:
                // Implement update profile
                break;
            case 3:
            printf("\n     --------------------------- Signing out... --------------------------");
                clearScreen();
                signoutTravelers();
                return;
            default:
                printf("\n                         * Invalid choice! Please enter a valid option.");
        }
        printf("\n\n Press Enter to continue...");
        while (getchar() != '\n'); // Clear input buffer
        getchar(); // Wait for Enter key
        clearScreen(); // Clear screen before next iteration
    } while (1);
}
// Function to view user profile
void viewProfile(User user) {
    printf("\n\n=======================================================");
    printf("\n                       User Profile                      ");
    printf("\n=======================================================");
    printf("\n Username: %s", user.username);
    printf("\n Profile: %s", user.profile);
    // printf("\n\n Press Enter to continue...");
    getchar(); // Wait for Enter key
}

// Function to update user profile
void updateProfile(User *user) {
    printf("\n\n=======================================================");
    printf("\n                       Update Profile                      ");
    printf("\n=======================================================");
    printf("\n Enter new username: ");
    scanf("%s", user->username);
    printf(" Enter new password: ");
    scanf("%s", user->password);
    printf("\n Profile updated successfully!");
    // printf("\n\n Press Enter to continue...");
    getchar(); // Wait for Enter key
}

void bus() {
    system("cls");
    printf("\n\n\n");
    printf("=========================================== BUS RESERVATION SYSTEM =========================================\n");
    for(int i = 0; i < 5; i++) {
        printf("\t\t\t\t\t[%d]  =>  %s\n", i + 1, ch[i]);
        printf("\n");
    }
}
void seatmanagement() {
    Seat seats[MAX_SEATS];
    int num_seats = MAX_SEATS;

    // Initialize seats with default values
    initializeSeats(seats, num_seats);

    int choice;
    do {
        printf("\n=====================================");
        printf("\n Seat Management");
        printf("\n=====================================");
        printf("\n 1. Display Seat Status" );
        printf("\n 2. Update Seat Status");
        printf("\n 3. Exit");
        printf("\n Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: {
                char busname[MAX_BUS_NAME_LENGTH];
                char routename[MAX_ROUTE_NAME_LENGTH];
                char origin[MAX_ORIGIN_DEST_LENGTH];
                char destination[MAX_ORIGIN_DEST_LENGTH];
                char arrivaltime[MAX_TIME_LENGTH];
                printf("Enter bus name: ");
                scanf("%s", busname);
                printf("Enter route name: ");
                scanf("%s", routename);
                printf("Enter origin: ");
                scanf("%s", origin);
                printf("Enter destination: ");
                scanf("%s", destination);
                printf("Enter arrival time (HH:MM): ");
                scanf("%s", arrivaltime);
                clearScreen();
                displaySeatStatus(seats, num_seats, busname, routename, origin, destination, arrivaltime);
                break;
            }
            case 2: {
                char busname[MAX_BUS_NAME_LENGTH];
                char routename[MAX_ROUTE_NAME_LENGTH];
                char origin[MAX_ORIGIN_DEST_LENGTH];
                char destination[MAX_ORIGIN_DEST_LENGTH];
                char arrivaltime[MAX_TIME_LENGTH];
                printf("Enter bus name: ");
                scanf("%s", busname);
                printf("Enter route name: ");
                scanf("%s", routename);
                printf("Enter origin: ");
                scanf("%s", origin);
                printf("Enter destination: ");
                scanf("%s", destination);
                printf("Enter arrival time (HH:MM): ");
                scanf("%s", arrivaltime);
                clearScreen();
                updateSeatStatus(seats, num_seats, busname, routename, origin, destination, arrivaltime);
                clearScreen();
                break;
            }
            case 3:
                printf("\nExiting...\n");
                break;
            default:
                printf("\nInvalid choice! Please enter a valid option.\n");
        }
    } while (choice != 3);
}

void initializeSeats(Seat seats[], int num_seats) {
    // Initialize seats with default values
    for (int i = 0; i < num_seats; i++) {
        seats[i].seat_number = i + 1;
        seats[i].is_available = 1; // Seat is available initially
    }
}
void displaySeatStatus(Seat seats[], int num_seats, char busname[], char routename[], char origin[], char destination[], char arrivaltime[]) {
    // Read seat status from the file
    FILE *file = fopen("seat_data.txt", "r");
    if (file == NULL) {
        printf("Error opening file for reading.\n");
        return;
    }

    printf("\n=============================================");
    printf("\n               Seat Status");
    printf("\n=============================================\n");

    char temp_busname[MAX_BUS_NAME_LENGTH], temp_routename[MAX_ROUTE_NAME_LENGTH], temp_origin[MAX_ORIGIN_DEST_LENGTH], temp_destination[MAX_ORIGIN_DEST_LENGTH], temp_arrivaltime[MAX_TIME_LENGTH];
    int seat_status;
    for (int i = 0; i < num_seats; i++) {
        if (fscanf(file, "%s %s %s %s %s %d", temp_busname, temp_routename, temp_origin, temp_destination, temp_arrivaltime, &seat_status) != 6) {
            break; // Break if fscanf fails to read all expected values
        }
        if (strcmp(temp_busname, busname) == 0 && strcmp(temp_routename, routename) == 0 && strcmp(temp_origin, origin) == 0 && strcmp(temp_destination, destination) == 0 && strcmp(temp_arrivaltime, arrivaltime) == 0) {
            printf("Seat %d - %s\n", seats[i].seat_number, seat_status ? "Available" : "Booked");
        }
    }

printf("=============================================\n");
fclose(file);
 printf("Press Enter to return to the seat management menu...");
    getchar(); // Wait for the user to press Enter
// printf("Press Enter to return to the seat management menu...");
// getchar(); // Wait for the user to press Enter
    
    
    
}

void updateSeatStatus(Seat seats[], int num_seats, char busname[], char routename[], char origin[], char destination[], char arrivaltime[]) {
    // Prompt for the number of seats to update
    displaySeatStatus(seats, num_seats, busname, routename, origin, destination, arrivaltime);
    int num_updates;
    printf("Enter the number of seats to update: ");
    scanf("%d", &num_updates);

    if (num_updates <= 0 || num_updates > num_seats) {
        printf("Invalid number of seats to update!\n");
        return;
    }

    int seat_numbers[num_updates];
    char choice;

    // Prompt for seat numbers to update status
    printf("Enter seat numbers to update status (separated by spaces): ");
    for (int i = 0; i < num_updates; i++) {
        scanf("%d", &seat_numbers[i]);
    }

    // Update seat statuses
    for (int i = 0; i < num_updates; i++) {
        if (seat_numbers[i] < 1 || seat_numbers[i] > num_seats) {
            printf("Invalid seat number: %d\n", seat_numbers[i]);
            continue;
        }

        // Update seat status
        printf("Set seat %d as (A) Available or (B) Booked: ", seat_numbers[i]);
        scanf(" %c", &choice);
        if (choice == 'A' || choice == 'a') {
            seats[seat_numbers[i] - 1].is_available = 1;
            printf("Seat %d is now available.\n", seat_numbers[i]);
        } else if (choice == 'B' || choice == 'b') {
            seats[seat_numbers[i] - 1].is_available = 0;
            printf("Seat %d is now booked.\n", seat_numbers[i]);
        } else {
            printf("Invalid choice for seat %d!\n", seat_numbers[i]);
        }
    }

    // Save the updated data to file
    save_seat_data(seats, num_seats, busname, routename, origin, destination, arrivaltime);
}

void save_seat_data(Seat seats[], int num_seats, char busname[], char routename[], char origin[], char destination[], char arrivaltime[]) {
    // Save seat data to a file
    FILE *fp = fopen("seat_data.txt", "a"); // Append mode
    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }
    
    for (int i = 0; i < num_seats; i++) {
        fprintf(fp, "%s %s %s %s %s %d\n", busname, routename, origin, destination, arrivaltime, seats[i].is_available);
    }
    
    fclose(fp);
}
void calculate_total_cost(Seat seats[], int num_seats, int selected_seats[]) {
    float total_cost = 0.0;
    
    for (int i = 0; i < num_seats; i++) {
        for (int j = 0; j < num_seats; j++) {
            if (seats[i].seat_number == selected_seats[j]) {
                total_cost += seats[i].price;
                break;
            }
        }
    }
    
    printf("Total Cost: $%.2f\n", total_cost);
}
void busroutemanagement() {
    // Implement function logic here
    printf("\nBus Information Management");
    printf("\n1. Display Bus Routes");
    printf("\n2. Add Bus Route");
    printf("\n3. Update Bus Route");
    printf("\n4. Delete Bus Route");
    printf("\n5. Back to Admin Panel");
    printf("\nEnter your choice: ");
    
    int choice;
    scanf("%d", &choice);
    
    switch (choice) {
        case 1:
            displayBusRoutes();
            break;
        case 2:
            AddBusRoute();
            break;
        case 3:
            UpdateBusRoute();
            break;
        case 4:
            DeleteBusRoute();
            break;
        case 5:
            printf("Returning to Admin Panel...\n");
            break;
        default:
            printf("Invalid choice!\n");
    }
}
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void displayBusRoutes() {
    char route_name[MAX_ROUTE_NAME_LENGTH];
    char destination[MAX_ORIGIN_DEST_LENGTH];
    char travel_date[11]; // Assuming date format is "YYYY-MM-DD"
    
    printf("Enter the route name: ");
    scanf("%s", route_name);
    printf("Enter the destination: ");
    scanf("%s", destination);
    printf("Enter the travel date (YYYY-MM-DD): ");
    scanf("%s", travel_date);

    FILE *file = fopen("bus_routes.txt", "r");
    if (file == NULL) {
        printf("Error opening file for reading.\n");
        return;
    }

    int found = 0;
    BusRoute currentRoute;

    while (fscanf(file, "%s %s %s %s %s %s %f %d", currentRoute.bus_name, currentRoute.route_name, currentRoute.origin, currentRoute.destination, currentRoute.arrival_time, currentRoute.departure_time, &currentRoute.ticket_price, &currentRoute.deleted_flag) != EOF) {
        if (strcmp(currentRoute.route_name, route_name) == 0 && strcmp(currentRoute.destination, destination) == 0 && currentRoute.deleted_flag == 0) {
            printf("\n                Search Result for Route: %s, Destination: %s\n", route_name, destination);
            printf("                      =============================================\n");
            printf("Bus Name: %s\n", currentRoute.bus_name);
            printf("Route Name: %s\n", currentRoute.route_name);
            printf("Origin: %s\n", currentRoute.origin);
            printf("Destination: %s\n", currentRoute.destination);
            printf("Arrival Time: %s\n", currentRoute.arrival_time);
            printf("Departure Time: %s\n", currentRoute.departure_time);
            printf("Ticket Price: %.2f\n", currentRoute.ticket_price);
            printf("Travel Date: %s\n", travel_date); // Assuming travel date is printed
            printf("                  ---------------------------------------------\n");
            found = 1;
        }
    }

    if (!found) {
        printf("\n\n               No bus routes found for Route: %s, Destination: %s\n", route_name, destination);
    }

    fclose(file);
}

void AddBusRoute() {
    // Create a bus route object
    BusRoute newRoute;

    // Prompt for and collect route details
    printf("\nEnter bus name: ");
    scanf("%s", newRoute.bus_name);
    printf("Enter route name: ");
    scanf("%s", newRoute.route_name);
    printf("Enter origin: ");
    scanf("%s", newRoute.origin);
    printf("Enter destination: ");
    scanf("%s", newRoute.destination);
    printf("Enter Arrival time (HH:MM): ");
    scanf("%s", newRoute.arrival_time);
    printf("Enter departure time (HH:MM): ");
    scanf("%s", newRoute.departure_time);
    printf("Enter Ticket price: ");
    scanf("%f", &newRoute.ticket_price);

    // Open the file in append mode to add the new route
    FILE *file = fopen("bus_routes.txt", "a");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    // Write the new route details to the file
    fprintf(file, "%s %s %s %s %s %s %.2f %d\n", newRoute.bus_name, newRoute.route_name, newRoute.origin, newRoute.destination, newRoute.arrival_time, newRoute.departure_time, newRoute.ticket_price, 0);

    // Close the file
    fclose(file);
    printf("Bus route added successfully!\n");
}

void UpdateBusRoute() {
    // Input the criteria for searching
    char busname[MAX_BUS_NAME_LENGTH];
 char busroute[MAX_ROUTE_NAME_LENGTH];
 char originname[MAX_ORIGIN_DEST_LENGTH];
 char destinationname[MAX_ORIGIN_DEST_LENGTH];
 char arrivaltime[MAX_TIME_LENGTH];
 printf("Enter the name of the bus you want to Update: ");
 scanf("%s", busname);
 printf("Enter the name of the bus route you want to Update: ");
 scanf("%s", busroute);
 printf("Enter the name of the bus Origin you want to Update: ");
 scanf("%s", originname);
 printf("Enter the name of the bus destination you want to Update: ");
 scanf("%s", destinationname);
 printf("Enter the bus arrival time you want to Update (HH:MM): ");
 scanf("%s", arrivaltime);
 clearInputBuffer(); // Clear the input buffer
    

    // Open the file in read mode
    FILE *file = fopen("bus_routes.txt", "r");
    if (file == NULL) {
        printf("Error opening file for reading.\n");
        return;
    }

    // Open a temporary file in write mode to store updated data
    FILE *tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL) {
        printf("Error opening temporary file for writing.\n");
        fclose(file);
        return;
    }

    // Search for the specified criteria in the file
    int found = 0;
    BusRoute currentRoute;

    while (fscanf(file, "%s %s %s %s %s %s %f %d", currentRoute.bus_name, currentRoute.route_name, currentRoute.origin, currentRoute.destination, currentRoute.arrival_time, currentRoute.departure_time, &currentRoute.ticket_price, &currentRoute.deleted_flag) != EOF) {
        // Check if any of the criteria match
        if (strcmp(currentRoute.bus_name, busname) == 0 && strcmp(currentRoute.route_name, busroute) == 0 && strcmp(currentRoute.origin, originname) == 0 && strcmp(currentRoute.destination, destinationname) == 0 && strcmp(currentRoute.arrival_time, arrivaltime) == 0) {
            found = 1;
            // Display bus route information
            printf("===============================================");
            printf("\n             Bus Route Information");
            printf("\n===============================================\n");
            printf("\n\n                       Bus Name: %s\n", currentRoute.bus_name);
            printf(" \n                       Route: %s\n", currentRoute.route_name);
            printf(" \n                       Origin: %s\n", currentRoute.origin);
            printf(" \n                       Destination: %s\n", currentRoute.destination);
            printf(" \n                       Arrival Time: %s\n", currentRoute.arrival_time);
            printf(" \n                       Departure Time: %s\n", currentRoute.departure_time);
            printf(" \n                       Ticket Price: %.2f\n", currentRoute.ticket_price);

            // Prompt for new details
            printf("\nEnter new bus name: ");
            scanf("%s", currentRoute.bus_name);
            printf("Enter new route name: ");
            scanf("%s", currentRoute.route_name);
            printf("Enter new origin: ");
            scanf("%s", currentRoute.origin);
            printf("Enter new destination: ");
            scanf("%s", currentRoute.destination);
            printf("Enter new arrival time (HH:MM): ");
            scanf("%s", currentRoute.arrival_time);
            printf("Enter new departure time (HH:MM): ");
            scanf("%s", currentRoute.departure_time);
            printf("Enter new ticket price: ");
            scanf("%f", &currentRoute.ticket_price);
        }
        // Write the current line (possibly updated) to the temporary file
        fprintf(tempFile, "%s %s %s %s %s %s %.2f %d\n", currentRoute.bus_name, currentRoute.route_name, currentRoute.origin, currentRoute.destination, currentRoute.arrival_time, currentRoute.departure_time, currentRoute.ticket_price, currentRoute.deleted_flag);
    }

    // Close both files
    fclose(file);
    fclose(tempFile);

    // Delete the original file
    remove("bus_routes.txt");

    // Rename the temporary file to the original filename
    rename("temp.txt", "bus_routes.txt");

    if (found) {
        printf("Bus route updated successfully!\n");
    } else {
        printf("No bus route found matching the criteria!\n");
    }
}
void DeleteBusRoute() {
    // Prompt for and collect the details of the route to be deleted
    char busname[MAX_BUS_NAME_LENGTH];
    char busroute[MAX_ROUTE_NAME_LENGTH];
    char originname[MAX_ORIGIN_DEST_LENGTH];
    char destinationname[MAX_ORIGIN_DEST_LENGTH];
    char arrivaltime[MAX_TIME_LENGTH];

    printf("Enter the name of the bus you want to delete: ");
    scanf("%s", busname);
    printf("Enter the name of the bus route you want to delete: ");
    scanf("%s", busroute);
    printf("Enter the name of the bus origin you want to delete: ");
    scanf("%s", originname);
    printf("Enter the name of the bus destination you want to delete: ");
    scanf("%s", destinationname);
    printf("Enter the bus arrival time you want to delete (HH:MM): ");
    scanf("%s", arrivaltime);
    clearInputBuffer(); // Clear the input buffer

    // Open the file in read mode
    FILE *file = fopen("bus_routes.txt", "r");
    if (file == NULL) {
        printf("Error opening file for reading.\n");
        return;
    }

    // Open a temporary file in write mode to store routes with the updated deleted flag
    FILE *tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL) {
        printf("Error opening temporary file for writing.\n");
        fclose(file);
        return;
    }

    // Create a bus route object
    BusRoute currentRoute;

    // Search for the specified route in the file and mark it as deleted
    int found = 0;
    while (fscanf(file, "%s %s %s %s %s %s %f %d", currentRoute.bus_name, currentRoute.route_name, currentRoute.origin, currentRoute.destination, currentRoute.arrival_time, currentRoute.departure_time, &currentRoute.ticket_price, &currentRoute.deleted_flag) != EOF) {
        // Check if the current route matches the one to be deleted
        if (strcmp(currentRoute.bus_name, busname) == 0 && strcmp(currentRoute.route_name, busroute) == 0 && strcmp(currentRoute.origin, originname) == 0 && strcmp(currentRoute.destination, destinationname) == 0 && strcmp(currentRoute.arrival_time, arrivaltime) == 0) {
            found = 1;
            currentRoute.deleted_flag = 1; // Mark the route as deleted
        }
        // Write the current route (possibly updated) to the temporary file
        fprintf(tempFile, "%s %s %s %s %s %s %.2f %d\n", currentRoute.bus_name, currentRoute.route_name, currentRoute.origin, currentRoute.destination, currentRoute.arrival_time, currentRoute.departure_time, currentRoute.ticket_price, currentRoute.deleted_flag);
    }

    // Close both files
    fclose(file);
    fclose(tempFile);

    if (!found) {
        printf("Route not found!\n");
    } else {
        // Delete the original file
        remove("bus_routes.txt");
        // Rename the temporary file to the original filename
        rename("temp.txt", "bus_routes.txt");
        printf("Route marked as deleted successfully!\n");
    }
}

// void searchBusRoutes() {
    // char route_name[MAX_ROUTE_NAME_LENGTH];
    // char destination[MAX_ROUTE_NAME_LENGTH];
    // char travel_date[11]; // Assuming date format is "YYYY-MM-DD"
    // 
    // printf("Enter the route name: ");
    // scanf("%s", route_name);
    // printf("Enter the destination: ");
    // scanf("%s", destination);
    // printf("Enter the travel date (YYYY-MM-DD): ");
    // scanf("%s", travel_date);
// 
    // FILE *file = fopen("bus_routes.txt", "r");
    // if (file == NULL) {
        // printf("Error opening file for reading.\n");
        // return;
    // }
// 
    // int found = 0;
    // char bus_name[MAX_BUS_NAME_LENGTH];
    // char route[MAX_ROUTE_NAME_LENGTH];
    // char origin[MAX_ROUTE_NAME_LENGTH];
    // char dest[MAX_ROUTE_NAME_LENGTH];
    // char arrival_time[6];
    // char departure_time[6];
    // float ticket_price;
// 
    // while (fscanf(file, "%s %s %s %s %s %s %f", bus_name, route, origin, dest, arrival_time, departure_time, &ticket_price) != EOF) {
        // if (strcmp(route, route_name) == 0 && strcmp(dest, destination) == 0) {
            // printf("\n                Search Result for Route: %s, Destination: %s", route_name, destination);
            // Display all bus routes
// printf("\n\n\n                      =============================================");
// printf("\n                                   Desire Bus Routes Information");
// printf("\n                          =============================================\n");
            // printf("Bus Name: %s\n", bus_name);
            // printf("Route Name: %s\n", route);
            // printf("Origin: %s\n", origin);
            // printf("Destination: %s\n", dest);
            // printf("Arrival Time: %s\n", arrival_time);
            // printf("Departure Time: %s\n", departure_time);
            // printf("Ticket Price: %.2f\n", ticket_price);
            // printf("Travel Date: %s\n", travel_date); // Assuming travel date is printed
            // printf("                  ---------------------------------------------\n");
            // found = 1;
        // }
    // }
// 
    // if (!found) {
        // printf("\n\n\n               No bus routes found for Route: %s, Destination: %s\n", route_name, destination);
    // }
// 
    // fclose(file);
// 
// void AssignBusNameAndPrice() {
    // Create a bus route object
    // BusRoute route;
// 
    // Prompt for and collect the name of the route to assign bus name and price
    // char routeName[MAX_ROUTE_NAME_LENGTH];
    // printf("\nEnter the name of the route you want to assign bus name and price: ");
    // scanf("%s", routeName);
// 
    // Open the file in read mode
    // FILE *file = fopen("bus_routes.txt", "r");
    // if (file == NULL) {
        // printf("Error opening file for reading.\n");
        // return;
    // }
// 
    // Open a temporary file in write mode to store updated data
    // FILE *tempFile = fopen("temp.txt", "w");
    // if (tempFile == NULL) {
        // printf("Error opening temporary file for writing.\n");
        // fclose(file);
        // return;
    // }
// 
    // Search for the specified route in the file
    // int found = 0;
    // while (fscanf(file, "%s %s %s %s %s %s %f", route.route_name, route.origin, route.destination,route.arrival_time, route.departure_time, route.bus_name, &route.ticket_price) != 7) {
        // Check if the current line corresponds to the route to be updated
        // if (strcmp(route.route_name, routeName) == 0) {
            // found = 1;
            // Prompt for and update bus name and ticket price
            // printf("Enter bus name for the route: ");
            // scanf("%s", route.bus_name);
            // printf("Enter ticket price for the route: ");
            // scanf("%f", &route.ticket_price);
        // }
        // Write the current line (possibly updated) to the temporary file
        // fprintf(tempFile, "%s %s %s %s %s %s %.2f\n", route.route_name, route.origin, route.destination,route.arrival_time ,route.departure_time, route.bus_name, route.ticket_price);
    // }
// 
    // Close both files
    // fclose(file);
    // fclose(tempFile);
// 
    // Delete the original file
    // remove("bus_routes.txt");
// 
    // Rename the temporary file to the original filename
    // rename("temp.txt", "bus_routes.txt");
// 
    // if (found) {
        // printf("Bus name and ticket price assigned successfully!\n");
    // } else {
        // printf("Route not found!\n");
    // }
// }
void pricemanagement() {
    int choice;
    do {
        printf("\nPrice Management Menu:");
        printf("\n1. Display Route Prices");
        printf("\n2. Update Prices");
        printf("\n3. Exit");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                displayRoutePrices();
                break;
            case 2:
                updatePrices();
                break;
            case 3:
                printf("Exiting Price Management Menu...\n");
                break;
            default:
                printf("Invalid choice! Please enter a valid option.\n");
        }
    } while (choice != 3);
}
void displayRoutePrices() {
    // Open the file in read mode
    FILE *file = fopen("bus_routes.txt", "r");
    if (file == NULL) {
        printf("Error opening file for reading.\n");
        return;
    }

    // Read and print route-wise prices
    BusRoute route;
    printf("\nRoute-wise Prices:\n");
    while (fscanf(file, "%s %s %s %s %s %s %f", route.route_name,route.bus_name, route.origin, route.destination, route.departure_time, route.arrival_time, route.ticket_price) == 7) {
        printf("Route: %s, Price: %.2f\n", route.route_name, route.ticket_price);
    }

    // Close the file1
    fclose(file);
}

void updatePrices() {
    // Prompt for and collect the name of the route to update price
    char routeName[MAX_ROUTE_NAME_LENGTH];
    printf("\nEnter the name of the route you want to update price: ");
    scanf("%s", routeName);

    // Open the file in read mode
    FILE *file = fopen("bus_routes.txt", "r");
    if (file == NULL) {
        printf("Error opening file for reading.\n");
        return;
    }

    // Open a temporary file in write mode to store updated data
    FILE *tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL) {
        printf("Error opening temporary file for writing.\n");
        fclose(file);
        return;
    }

    // Search for the specified route in the file
    int found = 0;
    BusRoute route;
    while (fscanf(file, "%s %s %s %s %s %s %f", route.route_name,route.bus_name, route.origin, route.destination, route.departure_time, route.arrival_time, &route.ticket_price) == 6) {
        // Check if the current line corresponds to the route to be updated
        if (strcmp(route.route_name, routeName) == 0) {
            found = 1;
            // Prompt for and update ticket price
            printf("Enter new ticket price for the route: ");
            scanf("%f", &route.ticket_price);
        }
        // Write the current line (possibly updated) to the temporary file
        fprintf(tempFile, "%s %s %s %s %s %.2f\n", route.route_name, route.origin, route.destination, route.departure_time, route.arrival_time, route.ticket_price);
    }

    // Close both files
    fclose(file);
    fclose(tempFile);

    // Delete the original file
    remove("bus_routes.txt");

    // Rename the temporary file to the original filename
    rename("temp.txt", "bus_routes.txt");

    if (found) {
        printf("Ticket price updated successfully!\n");
    } else {
        printf("Route not found!\n");
    }
}
void bookingmanagement() {
    int choice;
    do {
        printf("\nBooking Management Menu:");
        printf("\n1. View Bookings");
        printf("\n2. Cancel Booking");
        printf("\n3. Exit");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                viewBookings();
                break;
            case 2:
                cancelBooking();
                break;
            case 3:
                printf("Exiting Booking Management Menu...\n");
                break;
            default:
                printf("Invalid choice! Please enter a valid option.\n");
        }
    } while (choice != 3);
}
void viewBookings() {
    // Open the file in read mode
    FILE *file = fopen("bookings.txt", "r");
    if (file == NULL) {
        printf("Error opening file for reading.\n");
        return;
    }

    // Read and print bookings
    char username[50], routeName[50];
    int seatNumber;
    printf("\nBookings:\n");
    while (fscanf(file, "%s %s %d", username, routeName, &seatNumber) != EOF) {
        printf("Username: %s, Route: %s, Seat Number: %d\n", username, routeName, seatNumber);
    }

    // Close the file
    fclose(file);
}

void cancelBooking() {
    // Prompt for and collect the username and route name for cancellation
    char username[50], routeName[50];
    int seatNumber;
    printf("\nEnter the username: ");
    scanf("%s", username);
    printf("Enter the route name: ");
    scanf("%s", routeName);
    printf("Enter the seat number: ");
    scanf("%d", &seatNumber);

    // Open the file in read mode
    FILE *file = fopen("bookings.txt", "r");
    if (file == NULL) {
        printf("Error opening file for reading.\n");
        return;
    }

    // Open a temporary file in write mode to store updated data
    FILE *tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL) {
        printf("Error opening temporary file for writing.\n");
        fclose(file);
        return;
    }

    // Search for the booking to be canceled in the file
    int found = 0;
    char tempUsername[50], tempRouteName[50];
    int tempSeatNumber;
    while (fscanf(file, "%s %s %d", tempUsername, tempRouteName, &tempSeatNumber) != EOF) {
        // Check if the current line corresponds to the booking to be canceled
        if (strcmp(tempUsername, username) == 0 && strcmp(tempRouteName, routeName) == 0 && tempSeatNumber == seatNumber) {
            found = 1;
            printf("Booking canceled for Username: %s, Route: %s, Seat Number: %d\n", username, routeName, seatNumber);
        } else {
            // Write the current line (not to be canceled) to the temporary file
            fprintf(tempFile, "%s %s %d\n", tempUsername, tempRouteName, tempSeatNumber);
        }
    }

    // Close both files
    fclose(file);
    fclose(tempFile);

    // Delete the original file
    remove("bookings.txt");

    // Rename the temporary file to the original filename
    rename("temp.txt", "bookings.txt");

    if (!found) {
        printf("Booking not found!\n");
    }
}




