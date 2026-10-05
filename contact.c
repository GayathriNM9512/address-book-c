#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "contact.h"
#include "file.h"

int get_valid_choice(void) {
    int choice;
    while (scanf("%d", &choice) != 1) {
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF);
        printf("Invalid input! Please enter a number: ");
    }
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
    return choice;
}

void listContacts(AddressBook *addressBook) 
{
    if (addressBook->contactCount == 0) {
        printf("\nNo contacts found in the Address Book.\n\n");
        return;
    }

    for (int i = 0; i < addressBook->contactCount - 1; i++) {
        for (int j = 0; j < addressBook->contactCount - i - 1; j++) {
            if (strcasecmp(addressBook->contacts[j].name, addressBook->contacts[j + 1].name) > 0) {
                Contact temp = addressBook->contacts[j];
                addressBook->contacts[j] = addressBook->contacts[j + 1];
                addressBook->contacts[j + 1] = temp;
            }
        }
    }

    printf("\n" GREEN "-------------------------------- LIST OF CONTACTS --------------------------------" RESET "\n");

    printf("====================================================================================\n");
    printf("| " YELLOW "%-5s" RESET " | " YELLOW "%-20s" RESET " | " YELLOW "%-15s" RESET " | " YELLOW "%-31s" RESET " |\n", "S.No", "Name", "Phone", "Email");
    printf("====================================================================================\n");

    for (int i = 0; i < addressBook->contactCount; i++) {
        printf("| " YELLOW "%-5d" RESET " | " YELLOW "%-20s" RESET " | " YELLOW "%-15s" RESET " | " YELLOW "%-31s" RESET " |\n",
               i + 1, 
               addressBook->contacts[i].name, 
               addressBook->contacts[i].phone, 
               addressBook->contacts[i].email);
    }

    printf("------------------------------------------------------------------------------------\n");
}

void initialize(AddressBook *addressBook) {
    addressBook->contactCount = 0;
    loadContactsFromFile(addressBook);
}

void saveAndExit(AddressBook *addressBook) {
    saveContactsToFile(addressBook); 
    exit(EXIT_SUCCESS); 
}

int validate_name(const char *name) {
    if (strlen(name) == 0) return 0;
    for (int i = 0; name[i] != '\0'; i++) {
        if (!isalpha(name[i]) && name[i] != ' ' && name[i] != '.') {
            return 0; 
        }
    }
    return 1;
}

int validate_phone(const char *phone) {
    if (strlen(phone) != 10) return 0; 
    for (int i = 0; phone[i] != '\0'; i++) {
        if (!isdigit(phone[i])) {
            return 0; 
        }
    }
    return 1;
}

int is_phone_unique(AddressBook *addressBook, const char *phone, int current_idx) {
    for (int i = 0; i < addressBook->contactCount; i++) {
        if (i == current_idx) continue;
        if (strcmp(addressBook->contacts[i].phone, phone) == 0) {
            return 0;
        }
    }
    return 1;
}

int validate_email(const char *email) {
    int len = strlen(email);
    if (isdigit(email[0])) return 0;
    if (len < 4 || strcmp(email + len - 4, ".com") != 0) return 0;

    int at_count = 0;
    int dot_count = 0;
    for (int i = 0; email[i] != '\0'; i++) {
        if (!isdigit(email[i]) && !islower(email[i]) && email[i] != '@' && email[i] != '.' && email[i] != '_') {
            return 0;
        }
        if (email[i] == '@') {
            at_count++;
            if (i == 0 || i == len - 1) return 0;
        }
        if (email[i] == '.') {
            dot_count++;
            if (i == 0 || i == len - 1) return 0;
            if (i > 0 && email[i - 1] == '@') return 0;
        }
    }
    return (at_count == 1);
}

int is_email_unique(AddressBook *addressBook, const char *email, int current_idx) {
    for (int i = 0; i < addressBook->contactCount; i++) {
        if (i == current_idx) continue;
        if (strcmp(addressBook->contacts[i].email, email) == 0) {
            return 0;
        }
    }
    return 1;
}

void createContact(AddressBook *addressBook) {
    char str1[50];
    int res;
    do {
        printf("Enter the name : ");
        scanf(" %49[^\n]", str1);
        res = validate_name(str1); 
        if (res == 0)
            printf(RED "Your name is invalid, please enter a valid name\n" RESET);
    } while (res == 0);
    strcpy(addressBook->contacts[addressBook->contactCount].name, str1);

    char str2[20];
    int phone_valid = 0, phone_unique = 0;
    do {
        printf("Enter the phone : ");
        scanf("%19s", str2);
        phone_valid = validate_phone(str2);
        if (!phone_valid) {
            printf("Your phone number is invalid, please enter a valid 10-digit phone number\n");
            continue;
        }
        phone_unique = is_phone_unique(addressBook, str2, -1);
        if (!phone_unique) {
            printf("Phone number already exists, please enter a unique phone number\n");
        }
    } while (!phone_valid || !phone_unique);
    strcpy(addressBook->contacts[addressBook->contactCount].phone, str2);

    char str3[50];
    int email_valid = 0, email_unique = 0;
    do {
        printf("Enter the email : ");
        scanf(" %49[^\n]", str3);
        email_valid = validate_email(str3);
        if (!email_valid) {
            printf("Invalid email!\n");
            continue;
        }
        email_unique = is_email_unique(addressBook, str3, -1);
        if (!email_unique) {
            printf("Email ID already exists! Please enter a different email.\n");
        }
    } while (!email_valid || !email_unique);
    strcpy(addressBook->contacts[addressBook->contactCount].email, str3);

    addressBook->contactCount++;
}

int search_by_name(AddressBook *addressBook) {
    char name[50];
    printf("Enter the name: ");
    scanf(" %49[^\n]", name);

    int matches[100];
    int match_count = 0;
    for (int i = 0; i < addressBook->contactCount; i++) {
        if (strcasecmp(addressBook->contacts[i].name, name) == 0) {
            matches[match_count++] = i;
        }
    }

    if (match_count == 0) {
        printf("Contact not found!\n");
        return -1;
    } else if (match_count == 1) {
        int idx = matches[0];
        printf("\nContact Found:\n");
        printf("Name  : %s\n", addressBook->contacts[idx].name);
        printf("Phone : %s\n", addressBook->contacts[idx].phone);
        printf("Email : %s\n", addressBook->contacts[idx].email);
        return idx;
    } else {
        printf("\nMultiple contacts found with name '%s':\n", name);
        printf("%-5s %-20s %-15s %-30s\n", "S.No", "Name", "Phone", "Email");
        for (int i = 0; i < match_count; i++) {
            int idx = matches[i];
            printf("%-5d %-20s %-15s %-30s\n", 
                   i + 1, 
                   addressBook->contacts[idx].name, 
                   addressBook->contacts[idx].phone, 
                   addressBook->contacts[idx].email);
        }
        int serial;
        printf("Enter the Serial number of the contact you want: ");
        scanf("%d", &serial);
        if (serial >= 1 && serial <= match_count) {
            int selected_idx = matches[serial - 1];
            printf("\nContact Found:\n");
            printf("Name  : %s\n", addressBook->contacts[selected_idx].name);
            printf("Phone : %s\n", addressBook->contacts[selected_idx].phone);
            printf("Email : %s\n", addressBook->contacts[selected_idx].email);
            return selected_idx;
        } else {
            printf("Invalid Serial Number!\n");
            return -1;
        }
    }
}

int search_by_phone(AddressBook *addressBook) {
    char phone[20];
    printf("Enter phone number: ");
    scanf("%19s", phone);
    for (int i = 0; i < addressBook->contactCount; i++) {
        if (strcmp(addressBook->contacts[i].phone, phone) == 0) {
            printf("\n" GREEN "Contact Found:" RESET "\n");
            printf("Name  : %s\n", addressBook->contacts[i].name);
            printf("Phone : %s\n", addressBook->contacts[i].phone);
            printf("Email : %s\n", addressBook->contacts[i].email);
            return i;
        }
    }
    printf("\n" RED "Error: Phone number not found!" RESET "\n");
    return -1;
}

int search_by_mail(AddressBook *addressBook) {
    char email[50];
    printf("Enter email: ");
    scanf(" %49[^\n]", email);
    for (int i = 0; i < addressBook->contactCount; i++) {
        if (strcmp(addressBook->contacts[i].email, email) == 0) {
            printf("\n" GREEN "Contact Found:" RESET "\n");
            printf("Name  : %s\n", addressBook->contacts[i].name);
            printf("Phone : %s\n", addressBook->contacts[i].phone);
            printf("Email : %s\n", addressBook->contacts[i].email);
            return i;
        }
    }
    printf("\n" RED "Error: Email not found!" RESET "\n");
    return -1;
}

void searchContact(AddressBook *addressBook) {
    if (addressBook->contactCount == 0) {
        printf("\nAddress Book is empty! Please add contacts first.\n");
        return;
    }
    int choice;
    do {
        printf("\nSearch Contact By:\n");
        printf("1. Name\n2. Phone\n3. Email\n4. Exit to Main Menu\n");
        printf("Enter choice: ");
        choice = get_valid_choice();

        switch (choice) {
            case 1: search_by_name(addressBook); break;
            case 2: search_by_phone(addressBook); break;
            case 3: search_by_mail(addressBook); break;
            case 4: printf("Returning to main menu...\n"); break;
            default: printf("Invalid choice! Please try again.\n"); break;
        }
    } while (choice != 4);
}

void editContact(AddressBook *addressBook) {
    if (addressBook->contactCount == 0) {
        printf("\n" YELLOW "Address Book is empty!" RESET "\n");
        return;
    }

    int n, ind = -1;
    printf("\nSearch contact to edit by:\n");
    printf("1. Name\n2. Phone\n3. Email\n");
    printf("Enter choice: ");
    n = get_valid_choice();

    if (n == 1) ind = search_by_name(addressBook);
    else if (n == 2) ind = search_by_phone(addressBook);
    else if (n == 3) ind = search_by_mail(addressBook);
    else {
        printf("Invalid choice!\n");
        return;
    }

    if (ind == -1) {
        printf("\n" RED "Contact not found. Edit cancelled." RESET "\n");
        return; 
    }

    int field_choice;
    char ch[50], ch1[20], ch2[50];
    printf("\nEnter the field you want to edit:\n");
    printf("1. Name\n2. Phone\n3. Email\n");
    printf("Enter choice: ");
    field_choice = get_valid_choice();

    if (field_choice == 1) {
        printf("Enter the new name: ");
        scanf(" %49[^\n]", ch);
        if (validate_name(ch)) {
            strcpy(addressBook->contacts[ind].name, ch);
            printf("\n" GREEN "Name updated successfully!" RESET "\n");
        } else {
            printf("\nInvalid name!\n");
        }
    } else if (field_choice == 2) {
        printf("Enter the new phone: ");
        scanf("%19s", ch1);
        if (validate_phone(ch1) && is_phone_unique(addressBook, ch1, ind)) {
            strcpy(addressBook->contacts[ind].phone, ch1);
            printf("\n" GREEN "Phone updated successfully!" RESET "\n");
        } else {
            printf("\nInvalid or duplicate phone number!\n");
        }
    } else if (field_choice == 3) {
        printf("Enter the new email: ");
        scanf(" %49[^\n]", ch2);
        if (validate_email(ch2) && is_email_unique(addressBook, ch2, ind)) {
            strcpy(addressBook->contacts[ind].email, ch2);
            printf("\n" GREEN "Email updated successfully!" RESET "\n");
        } else {
            printf("\nInvalid or duplicate email!\n");
        }
    } else {
        printf("\n" RED "Invalid field option!" RESET "\n");
    }
}

void deleteContact(AddressBook *addressBook) {
    if (addressBook->contactCount == 0) {
        printf("\nAddress Book is empty! No contacts to delete.\n");
        return;
    }

    int n, index = -1;
    printf("\nSearch contact to delete by:\n");
    printf("1. Name\n2. Phone\n3. Email\n");
    printf("Enter choice: ");
    n = get_valid_choice();

    if (n == 1) index = search_by_name(addressBook);
    else if (n == 2) index = search_by_phone(addressBook);
    else if (n == 3) index = search_by_mail(addressBook);
    else {
        printf("Invalid choice!\n");
        return;
    }

    if (index == -1) {
        printf("Contact not found. Deletion cancelled.\n");
        return;
    }

    char confirm;
    printf("Are you sure you want to delete this contact? (y/n): ");
    scanf(" %c", &confirm);
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);

    if (confirm != 'y' && confirm != 'Y') {
        printf("Deletion cancelled.\n");
        return;
    }

    for (int i = index; i < addressBook->contactCount - 1; i++) {
        addressBook->contacts[i] = addressBook->contacts[i + 1];
    }
    addressBook->contactCount--;
    printf("Contact deleted successfully!\n");
}