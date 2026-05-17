# Blockchain Attendance Tracking System

A decentralized, immutable attendance tracking system implemented entirely in C. This project demonstrates core blockchain mechanics by linking attendance records via SHA-256 cryptographic hashes and authenticating entries using ECDSA digital signatures.

## Features
- **Custom Blockchain Data Structure:** Fully functional linked-list blockchain with Genesis block generation.
- **Cryptographic Hashing:** Utilizes OpenSSL `SHA-256` to ensure data immutability.
- **Digital Signatures:** Implements `secp256r1` Elliptic Curve Cryptography (ECDSA) to authenticate system administrator actions.
- **Data Persistence:** Binary file I/O operations for seamless session restoration across reboots.
- **Tamper Detection:** Built-in auditing algorithms to detect manual memory/file alterations.

## Required Libraries and Dependencies
This project relies on the **OpenSSL** library for all cryptographic functions. You must have the OpenSSL development headers installed on your system.

**For Linux (Ubuntu/Debian/WSL):**
```bash
sudo apt-get update
sudo apt-get install libssl-dev
```

**For macOS (via Homebrew):**
```bash
brew install openssl
```

## Compilation Instructions
To build the application, navigate to the project directory in your terminal and compile the source files while linking the OpenSSL crypto library (`-lcrypto`).
```bash
gcc main.c blockchain.c -o attendance_system -lcrypto
```

## How to Build and Run the Application

1. **Ensure Registry Exists:** Before running, verify that `students.txt` is in the same directory as your compiled executable. It should contain comma-separated student data (e.g., `ALU001,John Doe,BLK101`).
2. **Execute the Program:**
```bash
./attendance_system
```
3. **First Boot:** On the very first run, the system will automatically generate a new ECDSA keypair (`admin_key.pem`), create a Genesis Block, and create a local database file (`blockchain.dat`).
4. **Using the CLI:** Follow the interactive terminal menu to mark attendance, view the chain, validate integrity, and detect tampering.
