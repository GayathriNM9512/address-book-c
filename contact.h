#ifndef CONTACT_H
#define CONTACT_H

#define MAX_CONTACTS 100

#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
#define RESET   "\033[0m"

typedef struct {
    char name[50];
    char phone[20];
    char email[50];
} Contact;
 
typedef struct {
    Contact contacts[MAX_CONTACTS];
    int contactCount;
} AddressBook;

void createContact(AddressBook *addressBook);
void searchContact(AddressBook *addressBook);
void editContact(AddressBook *addressBook);
void deleteContact(AddressBook *addressBook);
void listContacts(AddressBook *addressBook);
void initialize(AddressBook *addressBook);

int search_by_name(AddressBook *addressBook);
int search_by_phone(AddressBook *addressBook);
int search_by_mail(AddressBook *addressBook);

int get_valid_choice(void);

#endif