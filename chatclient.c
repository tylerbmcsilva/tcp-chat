#include"stdio.h"  
#include"stdlib.h"  
#include"sys/types.h"  
#include"sys/socket.h"  
#include"string.h"  
#include"netinet/in.h"  
#include"netdb.h"
  
#define PORT 33450
#define BUF_SIZE 512 

int setupSocket();
void setChatHandle();
void getInput();
int sendMessage();
int receiveMessage();
int prepend();
void close();

int main(int argc, char**argv) {
    char buffer[BUF_SIZE];
    char chatHandle[13];
    char *host, *port;
    int sock;

    // Validate arguments
    if(argc < 3){
        printf("ERROR: Correct use: %s [hostname] [port]\n", argv[0]);
        exit(2);
    }
    
    // Get host and port
    host    = argv[1];
    port    = argv[2];
    // Setup socket to be used
    sock    = setupSocket(host, port);

    // Prompt user for Chat Handle
    setChatHandle(chatHandle);
    
    // Send initial message to server
    strncpy(buffer, argv[2], BUF_SIZE);
    prepend(&buffer, &chatHandle);
    sendMessage(sock, buffer);
    
    // While loop to receive and send
    int x = 0;
    while ( x == 0) {
        // Receive message
        x = receiveMessage(sock);
        if(x != 0){ break; }
        // Send Message
        memset(buffer, 0, BUF_SIZE);
        getInput(chatHandle, buffer, 500);
        prepend(&buffer, &chatHandle);
        x = sendMessage(sock, buffer);
        if(strstr(buffer, "\\quit") != NULL) {
            printf("Client has closed the connection\n");
            x = -1;
        }
    }
    // Once done, close the socket
    close(sock);
    // Exit successfully
    exit(0);
}

/*
 setupSocket - char *host, char *port
    - Sets up the connection with the server
    - Connects and returns the socket
*/
int setupSocket(char *host, char *port){
    struct addrinfo hints, *serverAddress;
    int s, sock;

    // Get information on server
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    s = getaddrinfo(host, port, &hints, &serverAddress);
    if(s != 0){
        printf("ERROR: Unable to getaddrinfo (%s)", gai_strerror(s));
        exit(1);
    } 
    
    // Create socket
    sock = socket(serverAddress->ai_family, serverAddress->ai_socktype, serverAddress->ai_protocol);
    if(sock < 0){
        printf("ERROR: Unable to create socket\n");
        exit(1);
    }
    
    // Connect to the socket
    if (connect(sock, serverAddress->ai_addr, serverAddress->ai_addrlen) == -1) {
        close(sock);
        perror("ERROR: Unable to connect to socket\n");
        exit(2);
    }
    
    // Return the socket
    return sock;
}

/*
 setChatHandle - char chat[]
    - Prompts user for their chat handle
    - Appends "> " to the handle
*/
void setChatHandle(char chat[]){
    getInput("What is your chat handle? (10 characters max)\n", chat, 13);
    chat[strlen(chat)] = '>';
    chat[strlen(chat)] = ' ';
}

/*
 getInput - char prompt[], char var[], int size
    - Prompts user for input
    - Sets var equal to the response
*/
void getInput(char prompt[], char var[], int size){
    // Clear variable completely
    memset(var, 0, BUF_SIZE);
    // Prompt message
    printf("%s", prompt);
    // Get stdin and store to variable
    if ( fgets(var, size, stdin) == NULL ){
        perror("ERROR: No input provided.\n");
        exit(1);
    }
    // Remove newline character
    var[strcspn(var, "\n")] = '\0';
}

/*
 sendMessage - int sock, char message[]
    - Sends message through the socket
*/
int sendMessage(int sock, char message[]){
    if(send(sock, message, BUF_SIZE, 0) < 0){
        perror("ERROR: Sending message");
        exit(1);
    } else {
        return 0;
    }
}

/*
 receiveMessage - int sock
    - Gets data from the socket
    - Checks if the server has closed the connection
*/
int receiveMessage(int sock) {
    char buffer[BUF_SIZE]; 
    memset(buffer, 0, BUF_SIZE);  
    if (recvfrom(sock, buffer, BUF_SIZE, 0, NULL, NULL) < 0) {  
        perror("ERROR: Receiving Data\n");
        exit(1);  
    } else {
        if(strstr(buffer, "\\quit") != NULL) {
            printf("Server has closed the connection\n");
            return -1;
        }
        printf("%s\n", buffer);
        return 0;
    }  
}

/*
 prepend - char *s, char *t
 - prepends t to s
 - Reference: https://stackoverflow.com/questions/2328182/prepending-to-a-string
*/
int prepend(char *str, char *pre) {
    int len = strlen(pre);
    int i;
    memmove(str + len, str, strlen(str) + 1);
    for(i = 0; i < len; ++i){
        str[i] = pre[i];
    }
    return 0;
}
