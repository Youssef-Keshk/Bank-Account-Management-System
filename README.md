# Bank Management System

## Overview
This **Bank Management System** is a console-based application designed to manage customer accounts within a bank. It allows employees to perform various operations, such as adding, modifying, searching, and handling transactions for bank accounts.

## Features

### Authentication
- **Login**: Users must log in using credentials stored in `users.txt`.

### Account Operations
- **Load Data**: Load account data from `accounts.txt`.
- **Search**:
  - **Query**: Search for an account by account number.
  - **Advanced Search**: Search for accounts by name keyword.
- **Account Management**:
  - **Add**: Create a new account (ensuring a unique account number).
  - **Modify**: Update name, email, and mobile number.
  - **Delete**: Remove an account if balance is zero.

### Transactions
- **Withdraw**: Withdraw money (*limit: $10,000 per transaction*).
- **Deposit**: Deposit money (*limit: $10,000 per transaction*).
- **Transfer**: Transfer money between accounts.

### Reports
- **View Transactions**: View the last five transactions of an account.
- **Print Accounts**: Print all accounts sorted by **name**, **balance**, or **date opened**.

### Data Management
- **Save**: Save all changes to `accounts.txt`.
- **Quit**: Exit the application.

## Usage
1. Start the program and log in.
2. The system automatically loads account data.
3. Choose from the menu options to perform actions.
4. After modifications, confirm whether to **save or discard changes**.
5. Exit the program using the **QUIT** option.

## Data Storage
- **`users.txt`**: Stores usernames and passwords.
- **`accounts.txt`**: Stores account details.
- **`accnumber.txt`** (e.g., `9124123456.txt`): Stores transaction history for each account.
