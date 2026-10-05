# Address Book Management System (C)

A command-line contact manager written in C using structures, pointers, arrays and file I/O. Contacts are saved to a text file and reloaded on the next run.

## Features
- Add, view, edit and delete contacts
- Search by name, phone number or email (handles multiple matches)
- Input validation for phone numbers and email IDs
- Sort contacts
- Persistent storage using `fprintf()` / `fscanf()`

## Project Structure
| File | Purpose |
|------|---------|
| `main.c` | Menu and program flow |
| `file.c` / `file.h` | Saving and loading contacts from file |
| `contact.c` / `contact.h` | Add, search, edit, delete, sort logic |
| `contacts.txt` | Sample data file |

## Build & Run
```bash
gcc *.c -o addressbook
./addressbook
```

## Concepts Used
Structures, pointers, arrays, modular programming, file handling, input validation

## Author
N.M. Gayathri — [LinkedIn](https://linkedin.com/in/nmgayathri)
