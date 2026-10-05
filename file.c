#include <stdio.h>
#include <stdlib.h>
#include "contact.h"
#include "file.h"

void saveContactsToFile(AddressBook *addressBook)
{
    FILE *fp = fopen("contact.txt", "w");
    if (fp == NULL)
    {
        printf("\n" RED "Error opening file for writing!" RESET "\n");
        return;
    }
    for (int i = 0; i < addressBook->contactCount; i++)
    {
        fprintf(fp, "%s,%s,%s\n",
                addressBook->contacts[i].name,
                addressBook->contacts[i].phone,
                addressBook->contacts[i].email);
    }
    fclose(fp);
    printf("\n" GREEN "Contacts saved successfully!" RESET "\n");
}

void loadContactsFromFile(AddressBook *addressBook)
{
    FILE *fp = fopen("contact.txt", "r");
    if (fp == NULL)
    {
        addressBook->contactCount = 0;
        return;
    }

    addressBook->contactCount = 0;

    while (addressBook->contactCount < MAX_CONTACTS &&
           fscanf(fp, " %49[^,],%19[^,],%49[^\n]\n",
                  addressBook->contacts[addressBook->contactCount].name,
                  addressBook->contacts[addressBook->contactCount].phone,
                  addressBook->contacts[addressBook->contactCount].email) == 3)
    {
        addressBook->contactCount++; 
    }

    fclose(fp);
    printf("\n" GREEN "%d contact(s) loaded successfully!" RESET "\n", addressBook->contactCount);
}