
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024
char menu[BUFFER_SIZE] = "\n"
             "  \033[1;35m _/_/_/_/   _/_/_/  _/_/_/_/_/    _/_/_/   \033[0m\n"
             " \033[1m\033[31m _/        _/           _/      _/           \033[0m\n"
             "\033[1m\033[33m _/_/_/      _/_/       _/        _/_/        \033[0m\n"
             "\033[1m\033[32m_/             _/      _/           _/      \033[0m\n"
             "\033[1m\033[36m/        _/_/_/       _/      _/_/_/ \033[0m""   \033[1m\033[31mB\033[0m\033[1m\033[33mA\033[0m\033[1m\033[32mN\033[0m\033[1m\033[36mK\033[0m\n"
             "\n\033[1;35mBank Management System\033[0m\n"
             "\033[1m1. \033[0mCreate account\n"
             "\033[1m2. \033[0mLogin\n"
             "\033[1m0. \033[0mExit\n"
             "\033[1mEnter your choice: \033[0m";
int islogged=0;

void login_client(int server_socket_fd) {
    char buffer[1024] = {0};
    char username[100], password[100];

 
    read(server_socket_fd, buffer, 1024);
    printf("%s", buffer);
    scanf("%s",username);
    send(server_socket_fd, username, strlen(username), 0);
    

    read(server_socket_fd, buffer, 1024);
    printf("%s", buffer);
    scanf("%s",password);
    send(server_socket_fd, password, strlen(password), 0);


    read(server_socket_fd, buffer, 1024);
    //if(strcmp(buffer, "login") == 0)
    printf("%s", buffer);
    char *result = strstr(buffer, "successful");

   if (result != NULL) {
     sleep(1);
     system("clear");
     char m[] =  "\n"
                  "  \033[1;35m _/_/_/_/   _/_/_/  _/_/_/_/_/    _/_/_/   \033[0m\n"
                  " \033[1m\033[31m _/        _/           _/      _/           \033[0m\n"
                  "\033[1m\033[33m _/_/_/      _/_/       _/        _/_/        \033[0m\n"
                  "\033[1m\033[32m_/             _/      _/           _/      \033[0m\n"
                  "\033[1m\033[36m/        _/_/_/       _/      _/_/_/ \033[0m""   \033[1m\033[31mB\033[0m\033[1m\033[33mA\033[0m\033[1m\033[32mN\033[0m\033[1m\033[36mK\033[0m\n"
                  "\033[1;34mWelcome to your Account !\033[0m\n"
                  "\033[1m1. \033[32mPrint Balance\033[0m\n"
                  "\033[1m2. \033[33mTransfer money\033[0m\n"
                  "\033[1m3. \033[36mEdit Account\033[0m\n"
                  "\033[1m4. \033[31mDelete Account\033[0m\n"
                  "\033[1m0. \033[35mExit\033[0m\n"
                  "Enter your choice: ";
     

	strcpy(menu, m);
     islogged=1;
   } else {
      printf("didn't log in !\n");
   }
}

void create_account_client(int server_socket_fd) {
    char buffer[1024] = {0};
    char username[100], password[100];


    read(server_socket_fd, buffer, 1024);
    printf("%s", buffer);
    scanf ("%s",username);
    send(server_socket_fd, username, strlen(username), 0);


    read(server_socket_fd, buffer, 1024);
    printf("%s", buffer);
     scanf("%s",password);
    send(server_socket_fd, password, strlen(password), 0);

    // Receive and display the "Accountcreated successfully!" message each send and receive is done to write or read in the server side
    read(server_socket_fd, buffer, 1024);
    printf("%s", buffer);
}
void edit_account_client(int server_socket_fd) {
    char buffer[1024] = {0};
    char new_username[50] = {0}, new_password[50] = {0};

    // Request and receive new username
    read(server_socket_fd, buffer, 1024);
    printf("%s", buffer);
    scanf("%s", new_username);
    send(server_socket_fd, new_username, strlen(new_username), 0);

    // Request and receive new password
    read(server_socket_fd, buffer, 1024);
    printf("%s", buffer);
    scanf("%s", new_password);
    send(server_socket_fd, new_password, strlen(new_password), 0);

    // Receive and display the "Account updated successfully!" message
    read(server_socket_fd, buffer, 1024);
    printf("%s", buffer);
}
void delete_account_client(int server_socket_fd) {

    // Receive and display the "Account deleted successfully!" message
    
    printf("Account deleted successfully!");
     char m[] =  "\nBank Management System\n1. Create account\n2. Login\n0. Exit\nEnter your choice: ";
  

	strcpy(menu, m);
      islogged=0;
    
}
void logout_account() {

    // Receive and display the "Account deleted successfully!" message
    
    printf("Logged out successfully!");
     char m[] =  "\nBank Management System\n1. Create account\n2. Login\n0. Exit\nEnter your choice: ";
  

	strcpy(menu, m);
      islogged=0;
    
}

void transfer_money_client(int server_socket_fd) {
    char buffer[1024] = {0};
    char recipient_username[50] = {0};
    double transfer_amount;

    //lire la réponse de serveur
    read(server_socket_fd, buffer, 1024);
    printf("%s", buffer);
    //le client doit donner le nom de destinataire
    scanf("%s", recipient_username);
    //l'envoie du nom de destinataire au serveur
    send(server_socket_fd, recipient_username, strlen(recipient_username), 0);

    //recevoir la demande du montant à transférer
    read(server_socket_fd, buffer, 1024);
    printf("%s", buffer);
    scanf("%lf", &transfer_amount);
    sprintf(buffer, "%.2lf", transfer_amount);
    send(server_socket_fd, buffer, strlen(buffer), 0);

    //afficher le message de réussite ou bien d'échec
    read(server_socket_fd, buffer, 1024);
    printf("%s", buffer);
}

void print_balance_client(int server_socket_fd) {
    char buffer[1024] = {0};

    // Receive and display the balance message
    read(server_socket_fd, buffer, 1024);
    printf("%s", buffer);
}
int main() {
    int client_fd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    // Create socket
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Configurartion d'address de serveur
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    //inet_pton convertit une adresse IP du texte au type sockaddr_in
    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        perror("invalid address");
        exit(EXIT_FAILURE);
    }

    // Connect to server
    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("connection failed");
        exit(EXIT_FAILURE);
    }

    char  choice[10] ;

    //afficher le menu après la connection
  do{

     //affichage de menu
     printf("%s",menu);

     scanf("%s",choice);
        
     //if not logges
     if(islogged==0){
         char new_char = '3';
         int len = strlen(choice);
   
         char new_arr[len + 2];
         memcpy(new_arr, choice, len);
         new_arr[len] = new_char;
         new_arr[len + 1] = '\0';
         //le choix + 3 (concatination)
         send(client_fd,new_arr, strlen(new_arr), 0);

           switch (atoi(choice)){
              case 1:
              create_account_client(client_fd);
              break;
              case 2:
              login_client(client_fd);
              break;
              case 0:
              exit(1);
              break;}
   }
   //l'autentification est bien faite
   else{
    char new_char = '5';
    int len = strlen(choice);

    char new_arr[len + 2];
    memcpy(new_arr, choice, len);
    new_arr[len] = new_char;
    new_arr[len + 1] = '\0';

    //concatination du choix avec 5
    send(client_fd,new_arr, strlen(new_arr), 0);

    //atoi: converssion du chaine de character en entier
    //maintenant le client va attendre la réponse du serveur
    switch (atoi(choice)){
      case 1:
      print_balance_client(client_fd) ;
      break;
      case 2:
      transfer_money_client(client_fd) ;
      break;
      case 3:
      edit_account_client(client_fd);
      break;
      case 4:
      delete_account_client(client_fd);
      break;
      case 0:
      logout_account();
      break;
   
   }
   }

   //la boucle est répiter tand que l'utilisateur est choisie une proposition
  }while(atoi(choice)!=0);
   

    // Close socket
    close(client_fd);
    return 0;
}