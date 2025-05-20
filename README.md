# ATM Management System

A terminal-based ATM system written in C that supports user registration, secure authentication, and basic banking operations. It offers an intuitive command-line interface for managing accounts, performing deposits, withdrawals, and updating user details.

---

## ✨ Features

- ✅ User registration and login
- 🔐 Secure user authentication using hashed passwords
- 🧾 Account creation with multiple types:
  - Saving
  - Current
  - Fixed deposit (1–3 years)
- 💸 Banking operations:
  - Deposit and withdraw money
  - Check account details
  - Transfer account ownership
- 📝 Update user details (country, phone number)
- ❌ Delete accounts
- 🔎 Input validation for:
  - Phone numbers
  - Account numbers
  - Dates
  - Usernames

---

## 🚀 Installation

1. **Clone the repository**  
```bash
   git clone https://github.com/your-username/atm-system.git
   cd atm-system
```

2. **Build the program using `make`**

```bash
   make
```

3. **Run the executable**

```bash
   ./atm
```

---

## 🧑‍💻 Usage

* Upon launching, users are prompted to register or log in.
* After login, users can:

  * Create new accounts
  * View or update existing accounts
  * Deposit/withdraw funds
  * Transfer account ownership
  * Delete accounts

> All data is stored in flat text files within the `data/` directory.

---

## 🔒 Validations

The program includes thorough input validation to ensure data integrity:

| Input Type     | Rules                                                                |
| -------------- | -------------------------------------------------------------------- |
| Username       | Must be alphanumeric; case-sensitive                                 |
| Phone Number   | Digits only, 8 to 15 characters                                      |
| Account Number | Digits only, must be unique to each user                             |
| Dates          | Validated for proper mm/dd/yyyy format and real calendar dates       |
| Account Type   | Must be one of: `saving`, `current`, `fixed01`, `fixed02`, `fixed03` |

---

## 📁 Project Structure

| File           | Description                                                                  |
| -------------- | ---------------------------------------------------------------------------- |
| `Makefile`     | Build script for compiling the program using `gcc`.                          |
| `main.c`       | Entry point: handles the main menu and user session flow.                    |
| `auth.c`       | User login, registration, and credential storage.                            |
| `system.c`     | Core banking logic: creating, updating, and deleting accounts.               |
| `validation.c` | Helper functions for validating inputs (e.g., dates, phone numbers).         |
| `header.h`     | Shared declarations: structs, constants, and function prototypes.            |
| `data/`        | Folder for storing user and account data (e.g., `users.txt`, `records.txt`). |

---

## 📌 Notes

* All data is stored in plain text files. No database setup required.
* The `Makefile` automatically creates the `data/` folder and necessary files if missing.
* Tested on Linux systems. For Windows, replace `clear` and `sleep` with appropriate equivalents.

---

## 🛠 Dependencies

* **Compiler**: GCC or Clang (C99 compatible)
* **Make**: For simplified build process
* No external libraries required.

---

## 📃 License

This project is licensed under the [MIT License](LICENSE).

---

## 🤝 Contributions

Contributions, bug reports, and suggestions are welcome!
To contribute:

1. Fork this repository
2. Create a new branch: `git checkout -b feature-name`
3. Commit your changes: `git commit -m "Add feature"`
4. Push and submit a pull request

---

## 📞 Contact

For feedback or inquiries, open an issue or contact the maintainer at \[[jesee-kuya](kuyajesee@proton.me)].

