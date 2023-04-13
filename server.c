
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

typedef struct {
    char username[50];
    char password[50];
    double balance;
} Account;
Account MyAccount ;

void save_account(Account account) {
    FILE *file = fopen("account.txt", "a");
    fprintf(file, "%s %s %lf\n", account.username, account.password, account.balance);
    fclose(file);
}

int load_account(char *username, char *password, Account *account) {
    FILE *file = fopen("account.txt", "r");
    if (file == NULL) return 0;

    while (fscanf(file, "%s %s %lf", account->username, account->password, &(account->balance)) != EOF) {
        if (strcmp(username, account->username) == 0 && strcmp(password, account->password) == 0) {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}
int sreach_for_account(char *username, Account *account) {
    FILE *file = fopen("account.txt", "r");
    if (file == NULL) return 0;

    while (fscanf(file, "%s %s %lf", account->username, account->password, &(account->balance)) != EOF) {
        if (strcmp(username, account->username)==0) {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}
void delete_account(Account account) {
    FILE *file = fopen("account.txt", "r");
    FILE *temp_file = fopen("temp_account.txt", "w");
    Account temp_account;
    
    while (fscanf(file, "%s %s %lf", temp_account.username, temp_account.password, &temp_account.balance) != EOF) {
        if (strcmp(account.username, temp_account.username) != 0) {
            fprintf(temp_file, "%s %s %lf\n", temp_account.username, temp_account.password, temp_account.balance);
        }
    }

    fclose(file);
    fclose(temp_file);
    
    remove("account.txt");
    rename("temp_account.txt", "account.txt");
}
void create_account(int client_socket_fd) {
    Account new_account;

    char buffer[1024] = {0};

    send(client_socket_fd, "Enter username: ", strlen("Enter username: "), 0);
    read(client_socket_fd, buffer, 1024);
    //stocker username dans la structure
    sscanf(buffer, "%s", new_account.username);

    send(client_socket_fd, "Enter password: ", strlen("Enter password: "), 0);
    read(client_socket_fd, buffer, 1024);
    sscanf(buffer, "%s", new_account.password);

    new_account.balance = 0;

    save_account(new_account);
    send(client_socket_fd, "Account created successfully!\n", strlen("Account created successfully!\n"), 0);
}

void login(int client_socket_fd) {
    char username[50], password[50];
    Account account;

    char buffer[1024] = {0};

    send(client_socket_fd, "Enter username: ", strlen("Enter username: "), 0);
    read(client_socket_fd, buffer, 1024);
    sscanf(buffer, "%s", username);

    send(client_socket_fd, "Enter password: ", strlen("Enter password: "), 0);
    read(client_socket_fd, buffer, 1024);
    sscanf(buffer, "%s", password);

    if (load_account(username, password, &account)) {
        char message[1024];
        
        snprintf(message, sizeof(message), "Login successful! Current balance: %.2lf\n", account.balance);
        send(client_socket_fd, message, strlen(message), 0);
        MyAccount.balance=account.balance;
        strcpy(MyAccount.username, account.username);
        strcpy(MyAccount.password, account.password);
        
        
        
        
    } else {
        send(client_socket_fd, "Incorrect username or password.\n", strlen("Incorrect username or password.\n"), 0);
    }
}

void print_balance(int client_socket_fd, Account account) {
    char msg[1024];
    Account mine;
    sreach_for_account(account.username, &mine);
    snprintf(msg, 1024, "Your balance is: $%.2lf\n", mine.balance);
    send(client_socket_fd, msg, strlen(msg), 0);
}

void edit_account(int client_socket_fd, Account account) {
    char buffer[1024];
  delete_account(account);
    send(client_socket_fd, "Enter new username (leave blank for no change): ", strlen("Enter new username (leave blank for no change): "), 0);
    char new_username[50];
    read(client_socket_fd, buffer, 1024);
    sscanf(buffer, "%s", new_username);
    if (strlen(new_username) > 0) {
        strcpy(account.username, new_username);
    }

    send(client_socket_fd, "Enter new password (leave blank for no change): ", strlen("Enter new password (leave blank for no change): "), 0);
    char new_password[50];
    read(client_socket_fd, buffer, 1024);
    sscanf(buffer, "%s", new_password);
    if (strlen(new_password) > 0) {
        strcpy(account.password, new_password);
    }

    
    save_account(account);

    send(client_socket_fd, "Account updated successfully!\n", strlen("Account updated successfully!\n"), 0);
}

void transfer_money(int client_socket_fd, Account sender_account) {
    char buffer[1024];
    //demander au client de saisir le nom du destinataire
    send(client_socket_fd, "Enter recipient's username: ", strlen("Enter recipient's username: "), 0);
    char recipient_username[50];
    read(client_socket_fd, buffer, 1024);
    sscanf(buffer, "%s", recipient_username);

    Account recipient_account;
    //chercher le compte du destinataire
    if (!sreach_for_account(recipient_username, &recipient_account)) {
        send(client_socket_fd, "Recipient account not found.\n", strlen("Recipient account not found.\n"), 0);
        return;
    }
    //suprimer chaque compte
    delete_account(sender_account);
    delete_account(recipient_account);

    send(client_socket_fd, "Enter amount to transfer: ", strlen("Enter amount to transfer: "), 0);
    double transfer_amount;
    read(client_socket_fd, buffer, 1024);
    sscanf(buffer, "%lf", &transfer_amount);

    //vérification du solde
    if (sender_account.balance >= transfer_amount) {
        sender_account.balance -= transfer_amount;
        recipient_account.balance += transfer_amount;
        //sauvgarder les nouveaux comptes
        save_account(sender_account);
        save_account(recipient_account);
        send(client_socket_fd, "Transfer completed successfully!\n", strlen("Transfer completed successfully!\n"), 0);
    } else {
        send(client_socket_fd, "Insufficient balance.\n", strlen("Insufficient balance.\n"), 0);
    }
}


int main() {
    int server_fd, client_fd, addr_len;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFFER_SIZE];

     // Crier socket avec le protocole TCP/IP
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;//0.0.0.0 pour recevoir des connexions entrant de toutes les adresse IP
    server_addr.sin_port = htons(PORT);

    // lier le socket au adresse IP et au numéro de port
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    //Écouter les clients (1 client à la fois)
    if (listen(server_fd, 1) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    addr_len = sizeof(client_addr);

    // Accepter la connection de client
    if ((client_fd = accept(server_fd, (struct sockaddr *)&client_addr, (socklen_t *)&addr_len)) < 0) {
        perror("accept failed");
        exit(EXIT_FAILURE);
    }
    
    char choice[10];
    while (1){
    //lire le choix de client
     read( client_fd, choice, 10);
       int type , op;
       //le type représente si le client est connecté ou non
       type= atoi(choice)%10;
       //op le choix de client
       op=atoi(choice)/10;
       // type 3 means it's logged
      if(type==3){
      switch (op){
      case 1:
      create_account(client_fd);
      break;
      case 2:
      login(client_fd);
      break;
      case 3:
      exit(1);
      break;}

      }else{
      
      switch (op){
      
      case 1:
      print_balance(client_fd , MyAccount ) ;
      break;
      case 2:
      transfer_money(client_fd , MyAccount) ;
      break;
      case 3:
      edit_account(client_fd , MyAccount);
      break;
      case 4:
      delete_account(MyAccount);
      break;
   }
   }

    }
    
    // Close sockets
    close(client_fd);
    close(server_fd);
    return 0;
}
