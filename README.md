# Banking System Management using Server/Client Architecture
<p align="center">
  <img src="https://github.com/Dris7/Banking_Server_Client/blob/main/Screenshot%202023-03-31%20005300.png" alt="Logo">
   <img src="https://github.com/Dris7/Banking_Server_Client/blob/main/Screenshot%202023-03-31%20005348.png" alt="Logo">
</p>

This is a banking system management project built using C programming language and utilizing sockets for server/client communication. The project is designed to handle multiple clients simultaneously and offers various functionalities including login to account, creating an account, checking account balance, transferring money, and deleting an account.

## Features

- User authentication: users can log in to their accounts securely using a unique username and password combination.
- Account creation: new users can create an account with ease and start using the banking system.
- Balance inquiry: users can check their account balance at any time.
- Money transfer: users can transfer money from their account to another account within the system.
- Account deletion: users can delete their account from the system.

## Installation

1. Clone the repository
2. Compile the `server.c` file using the command `gcc server.c -o server -lpthread`
3. Compile the `client.c` file using the command `gcc client.c -o client`

## Usage

1. Start the server by running `./server` in the terminal
2. Start the client by running `./client` in another terminal
3. Follow the on-screen instructions to navigate through the various functionalities of the system.

## Contributors

- [ATKOU DRISS](https://github.com/Dris7)
- [EL KANTOUR YOUNESS](https://github.com/younessX)
- [ABDELLAH SOUSAID](https://github.com/sousaid)
## License

This project is licensed under the [MIT License](https://opensource.org/licenses/MIT).
