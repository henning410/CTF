#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 1234
#define MAX_BUFFER_SIZE 64
#define BUFF_SIZE 12
#define MY_SECRET_WORD "Flag6:YouSolvedThisTaskCorrectly:)ThisIsOneOfTheLastFlags"
#define MAX_USERS 10

// Define a structure to represent a user
struct User
{
    char username[50];
    char password[50];
};

void removeSpacesAndNewlines(char *input)
{
    int i, j = 0;
    size_t length = strlen(input);

    for (i = 0; i < length; ++i)
    {
        if (input[i] != ' ' && input[i] != '\n')
        {
            input[j++] = input[i];
        }
    }

    // Null-terminate the modified string
    input[j] = '\0';
}

void sendMessage(int new_socket, char *string)
{
    if (send(new_socket, string, strlen(string), 0) < 0)
    {
        perror("Send failed");
        exit(EXIT_FAILURE);
    }
}

void printWelcomeMessage(int new_socket)
{
    sendMessage(new_socket, "_________ _______  _______  _______  _______ \n");
    sendMessage(new_socket, "\\__   __/(  ___  )(  ___  )/ ___   )(  ___  )\n");
    sendMessage(new_socket, "   ) (   | (   ) || (   ) |\\/   )  || (   ) |\n");
    sendMessage(new_socket, "   | |   | (___) || (___) |    /   )| (___) |\n");
    sendMessage(new_socket, "   | |   |  ___  ||  ___  |   /   / |  ___  |\n");
    sendMessage(new_socket, "   | |   | (   ) || (   ) |  /   /  | (   ) |\n");
    sendMessage(new_socket, "   | |   | )   ( || )   ( | /   (_/\\| )   ( |\n");
    sendMessage(new_socket, "   )_(   |/     \\||/     \\|(_______/|/     \\|\n");
    sendMessage(new_socket, "\nWillkommen beim Taaza Nutzermanagement. Zugriff ist nur Admins gewährt! \n \nBitte Passwort eingeben: \n");
}

void printMenu(int new_socket)
{
    sendMessage(new_socket, "\n\nMenü\n");
    sendMessage(new_socket, "-----------------------------------------------\n");
    sendMessage(new_socket, "1. Benutzer anlegen\n");
    sendMessage(new_socket, "2. Benutzer löschen\n");
    sendMessage(new_socket, "3. Alle Benutzer ausgeben\n");
    sendMessage(new_socket, "4. Beenden\n");
    sendMessage(new_socket, "\n\nGib eine Option ein: ");
}

// Function to add a user to the array
void addUser(struct User *users, int *userCount, const char *username, const char *password)
{
    if (*userCount < MAX_USERS)
    {
        // Copy the username and password to the new user
        strncpy(users[*userCount].username, username, sizeof(users[*userCount].username) - 1);
        strncpy(users[*userCount].password, password, sizeof(users[*userCount].password) - 1);

        // Null-terminate the strings
        users[*userCount].username[sizeof(users[*userCount].username) - 1] = '\0';
        users[*userCount].password[sizeof(users[*userCount].password) - 1] = '\0';

        // Increment the user count
        (*userCount)++;
        printf("User added successfully.\n");
    }
    else
    {
        printf("Error: Maximum number of users reached.\n");
    }
}

// Function to delete a user by username
void deleteUser(int new_socket, struct User *users, int *userCount, const char *username)
{
    int i, found = 0;

    for (i = 0; i < *userCount; ++i)
    {
        if (strcmp(users[i].username, username) == 0)
        {
            // User found, remove by shifting elements
            found = 1;
            for (int j = i; j < *userCount - 1; ++j)
            {
                strcpy(users[j].username, users[j + 1].username);
                strcpy(users[j].password, users[j + 1].password);
            }
            (*userCount)--;
            printf("User '%s' deleted successfully.\n", username);
            sendMessage(new_socket, "\nBenutzer erfolgreich gelöscht");
            break;
        }
    }

    if (!found)
    {
        printf("User '%s' does not exist.\n", username);
        sendMessage(new_socket, "\nUser existiert nicht");
    }
}

// Function to print all users
void printUsers(const struct User *users, int userCount, int new_socket)
{
    sendMessage(new_socket, "\nAlle Benutzer\n");
    sendMessage(new_socket, "-----------------------------------------------\n");
    printf("User List:\n");
    for (int i = 0; i < userCount; ++i)
    {
        printf("Username: %s\n", users[i].username);
        sendMessage(new_socket, "Benutzername: ");
        sendMessage(new_socket, (char *)users[i].username);
        sendMessage(new_socket, "\nPasswort: ");
        sendMessage(new_socket, (char *)users[i].password);
    }
}

int main()
{
    int server_socket, new_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    char buffer[MAX_BUFFER_SIZE] = {0};
    char username[BUFF_SIZE];
    // Array to store users
    struct User users[MAX_USERS];
    // Variable to keep track of the number of users
    int userCount = 0;

    // Create socket
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set up server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind the socket to the specified port
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_socket, 3) < 0)
    {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    while (1)
    {
        // Accept incoming connection
        if ((new_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_len)) < 0)
        {
            perror("Accept failed");
            exit(EXIT_FAILURE);
        }

        printWelcomeMessage(new_socket);

        memset(buffer, 0, sizeof(buffer));
        if (recv(new_socket, buffer, sizeof(buffer), 0) <= 0)
        {
            perror("Receive failed");
            exit(EXIT_FAILURE);
        }

        // Check for password
        if (strncmp(buffer, MY_SECRET_WORD, strlen(MY_SECRET_WORD)) == 0)
        {
            sendMessage(new_socket, "\nErfolgreich eingeloggt. Du bist ein vertrauenswürdiger Admin!");
            while (1)
            {
                printMenu(new_socket);
                memset(buffer, 0, sizeof(buffer));
                if (recv(new_socket, buffer, sizeof(buffer), 0) <= 0)
                {
                    perror("Receive failed");
                    exit(EXIT_FAILURE);
                }

                removeSpacesAndNewlines(buffer);
                // Check if the received message is a specific number (e.g., "42")
                if (strncmp(buffer, "1", 2) == 0)
                {
                    sendMessage(new_socket, "\nBenutzer anlegen\n");
                    sendMessage(new_socket, "-----------------------------------------------\n");

                    sendMessage(new_socket, "Benutzername: ");
                    memset(buffer, 0, sizeof(buffer));
                    if (recv(new_socket, buffer, sizeof(buffer), 0) <= 0)
                    {
                        perror("Receive failed");
                        exit(EXIT_FAILURE);
                    }
                    removeSpacesAndNewlines(buffer);
                    strcpy(username, buffer);
                    printf("Received username: %s\n", buffer);

                    sendMessage(new_socket, "Passwort: ");
                    char *password = "";
                    memset(buffer, 0, sizeof(buffer));
                    if (recv(new_socket, buffer, sizeof(buffer), 0) <= 0)
                    {
                        perror("Receive failed");
                        exit(EXIT_FAILURE);
                    }
                    removeSpacesAndNewlines(buffer);
                    printf("Received password: %s\n", buffer);

                    sendMessage(new_socket, "\nNeuer Benutzer mit folgenden Daten wird angelegt:\n");
                    sendMessage(new_socket, "Username: ");
                    sendMessage(new_socket, username);
                    sendMessage(new_socket, "\tPasswort: ");
                    sendMessage(new_socket, buffer);
                    addUser(users, &userCount, username, buffer);
                }
                else if (strcmp(buffer, "2") == 0)
                {
                    sendMessage(new_socket, "\nBenutzer löschen\n");
                    sendMessage(new_socket, "-----------------------------------------------\n");

                    sendMessage(new_socket, "Benutzername: ");
                    memset(buffer, 0, sizeof(buffer));
                    if (recv(new_socket, buffer, sizeof(buffer), 0) <= 0)
                    {
                        perror("Receive failed");
                        exit(EXIT_FAILURE);
                    }
                    removeSpacesAndNewlines(buffer);
                    deleteUser(new_socket, users, &userCount, buffer);
                }
                else if (strcmp(buffer, "3") == 0)
                {
                    printUsers(users, userCount, new_socket);
                }
                else if (strcmp(buffer, "4") == 0)
                {
                    sendMessage(new_socket, "\nLogout");
                    close(new_socket);
                    break;
                }
                else
                {
                    sendMessage(new_socket, "\nFalscher Input");
                }
            }
        }
        else
        {
            sendMessage(new_socket, "\nUps, falsches Passwort. Sicher, dass du ein Admin bist?");
            printf("Wrong password\n");
            close(new_socket);
        }
    }

    close(server_socket);
    return 0;
}
