#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<fcntl.h>
#include<unistd.h>
#include<crypt.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/ip.h>
#include<sys/stat.h>
#include<sys/types.h>

// Determine the paths of the variours files
#define EMPPATH "../Database/Employe.txt"
#define CUSPATH "../Database/Customer.txt"
#define LOANPATH "../Database/LoanDetails.txt"
#define COUNTERPATH "../Database/LoanCounter.txt"
#define HISTORYPATH "../Database/TransactionHistory.txt"
#define FEEDPATH "../Database/Feedback.txt"
#define HASHKEY "$6$saltsalt$"
// Compile
// gcc server.c -o server -L/lib/x86_64-linux-gnu/libcrypt.so -lcrypt

#define MAINMENU "\n+++++++++++++++++ LOGIN AS +++++++++++++++++++++++++\n1. Customer\n2. Employee\n3. Manager\n4. Admin\n5. Exit\nEnter your choice: "
#define ADMINMENU "\nWelcome HIMANSHU, Your are currently in ADMIN Section! \n`+++++++++++++++++++++++ Admin ++++++++++++++++++++++++\n1. Add New Bank Employee\n2. Modify Customer/Employee Details\n3. Manage User Roles\n4. Change Password\n5. Logout\nEnter your choice: "
#define CUSMENU "\n++++++++++++++++++ CUSTOMER +++++++++++++++++\n1. Deposit\n2. Withdraw\n3. View Balance\n4. Apply for a loan\n5. Money Transfer\n6. Change Password\n7. View Transaction\n8. Add Feedback\n9. Logout\nEnter your choice: "
#define EMPMENU "\n++++++++++++++++++++++++++++++++ EMPLOYEE +++++++++++++++++++++++++\n1. Add New Customer\n2. Modify Customer Details\n3. Approve/Reject Loans\n4. View Assigned Loan Applications\n5. View Customer Transactions\n6. Change Password\n7. Logout\n8. Exit\nEnter your choice: "
#define MNGMENU "\n+++++++++++++++++++++++++++ MANAGER +++++++++++++++++++++++++++\n1. Activate/Deactivate Customer Accounts\n2. Assign Loan Application Processes to Employees\n3. Review Customer Feedback\n4. Change Password\n5. Logout\n6. Exit\nEnter your choice: "

// 
#include "../AllStructures/allStruct.h"
#include "../Modules/Customer.h"
#include "../Modules/Admin.h"
#include "../Modules/Employee.h"
#include "../Modules/Manager.h"




void connectionHandler(int client_socket);
void exitClient(int client_socketFD);


int main(){
    
    int server_socket_fd, client_socket_fd; // hold server and client socket descriptor
    int bindStatus;
    int listenStatus;
    int clientSize;


    struct sockaddr_in server_addr, client_addr;  // address-> store the server ip    client->used to store information of client once connection is accepted
    
    // 1, Creating server Socket
    server_socket_fd = socket(AF_INET, SOCK_STREAM, 0); // function create the socket  (ipv4, TCP, protocol(default 0))
    if(server_socket_fd == -1)
    {
        perror("Error");
        exit(-1);
    }
    printf("Server Created\n");

    // prepared server address
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); // Host To Network Long // This sets the server's IP address to 0.0.0.0 (using INADDR_ANY), which means it will listen for connections on all available network interfaces.
    // htonl()  - it ensures that the port number is in the correct format 
    server_addr.sin_family = AF_INET;   // line ensures that the socket is configured to use IPv4 addresses for communication.
    server_addr.sin_port = htons(8080); // This line sets the port number to 8080 and converts it to network byte order using htons(). The server will listen for connections on this port.

    // 2. Binding socket to address and port
    bindStatus = bind(server_socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (bindStatus == -1)
    {
        perror("Error");
        exit(-1);
    }
    printf("Binding to socket successful!\n");

    // 3. Listen for connection
    listenStatus = listen(server_socket_fd, 2); // Marks the socket as passive, waiting for incoming client connections. The second argument (2) specifies the maximum number of queued connections.
    if (listenStatus == -1)
    {
        perror("Error");
        exit(-1);
    }
    printf("Listening for connections!\n");

    // 4. Accepting Client Connection
    while(1)
    {
        clientSize = sizeof(client_addr);
        client_socket_fd = accept(server_socket_fd, (struct sockaddr *) &client_addr, &clientSize);
        // accept(): Blocks the server, waiting for a client to connect. When a client connects, it creates a new socket for that client (client_socket_fd).
        if (client_socket_fd == -1)
            perror("Error");
        else
        {
            if(fork() == 0)
            {
                // This is used to handle client connection..
                connectionHandler(client_socket_fd);
            }
        }
    }
    close(server_socket_fd);
    return 0;
}

// Handling client Connection
void connectionHandler(int client_socket){
    char readBuffer[4096], writeBuffer[4096];
    int readBytes, writeBytes, choice;

    while(1){
        writeBytes=write(client_socket,MAINMENU,sizeof(MAINMENU)); //sends the content of the MAINMENU buffer to the socket associated with the client
        if(writeBytes == -1){
            printf("Not able to send data to client! \n");
        }else{
            bzero(readBuffer , sizeof(readBuffer)); // clean the buffer
            readBytes = read(client_socket, readBuffer, sizeof(readBuffer));
            if(readBytes == -1)
            {
                printf("Unable to read data from client\n");
            }
            else if(readBytes == 0)
            {
                printf("No data was sent to the server\n");
            }else{
                // printf("%s\n" , readBuffer);
                choice = atoi(readBuffer);
                printf("Client has entered: %d\n", choice);
                switch (choice) 
                {
                    case 1:
                        printf("Client has chooses for : %dth coice i.e Customer\n", choice);
                        // 1. For Customer
                        customerMenu(client_socket);
                        break;
                    
                    case 2:
                        // 2. for Employee
                        printf("Client has chooses for : %dth coice i.e Employee\n", choice);
                        employeeMenu(client_socket);
                        break;
                            
                    case 3:
                        // 3. Manager
                        printf("Client has chooses for : %dth coice i.e Manager\n", choice);
                        managerMenu(client_socket);
                        break;

                    case 4:
                        // 4. Admin
                        printf("Client has chooses for : %dth coice i.e Admin\n", choice);
                        adminMenu(client_socket);
                        break;

                    case 5:
                    printf("Client has chooses for : %dth coice i.e Exit the Client!\n", choice);
                        exitClient(client_socket);
                        return;

                    default:
                        printf("ALERT!!!  You Have Entered The Wrong Choice!!!!!!!!!!!!!!!!!!\n");
                        break;
                }
                printf("\nChoice written by the server succesfully ! \n");
            }

        }
    }
}

void exitClient(int client_socketFD){
    bzero(writeBuffer, sizeof(writeBuffer));
    strcpy(writeBuffer, "---------You are been logged out-----------------\n");
    write(client_socketFD, writeBuffer, sizeof(writeBuffer));
    read(client_socketFD, readBuffer, sizeof(readBuffer));
}