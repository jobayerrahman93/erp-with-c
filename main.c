#include "erp.h"

int main()
{
    int choice;
    int exitFlag = 0;

    // Loop to show the main menu again after logout
    while (!exitFlag)
    {
        printf("_______________ Welcome to MENU OF ERP Management System _______________\n\n");
        printf("1. ADMIN \n");
        printf("2. Employee\n");
        printf("3. Exit\n\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        printf("\n\n");

        switch (choice)
        {
            case 1:
                if (adminLogin())
                {
                    adminPanel();
                }
                else
                {
                    printf("Email or password is wrong. Exiting...\n");
                }
            break;

            case 2:
                if (employeeLogin())
                {
                    employeePanel();
                }
                else
                {
                    printf("Email or password is wrong. Exiting...\n");
                }
            break;

            case 3:
                printf("Exiting...\n");
            exitFlag = 1;  // Exit the loop and terminate the program
            break;

            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
