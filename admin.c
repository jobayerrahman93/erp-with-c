// admin.c
#include "erp.h"


int validateCredentials(const char *line, const char *userEmail, const char *password)
{
    int storedId, storedStatus;
    char storedName[50], storedUserEmail[50], storedPassword[50], storedCreatedAt[20];

    // Parse the CSV line for quoted fields
    if (sscanf(line, "%d,\"%49[^\"]\",\"%49[^\"]\",\"%49[^\"]\",%d,\"%19[^\"]\"",
               &storedId, storedName, storedUserEmail, storedPassword, &storedStatus, storedCreatedAt) == 6)
    {
        // Check if the parsed email and password match the input
        if (strcmp(userEmail, storedUserEmail) == 0 && strcmp(password, storedPassword) == 0)
        {
            return 1; // Login successful
        }
    }

    return 0; // Login failed
}

int adminLogin()
{
    char userEmail[50], password[50];
    char line[MAX_LINE_LENGTH];
    char storedName[50], storedUserEmail[50], storedPassword[50];
    int storedId;

    printf("Enter Admin Email: ");
    scanf("%s", userEmail);
    printf("\nEnter Admin Password: ");
    scanf("%s", password);

    FILE *file = fopen("user_admin.csv", "r");
    if (file == NULL)
    {
        printf("Unable to open users file.\n");
        return 0;
    }

    int loginSuccessful = 0;
    while (fgets(line, sizeof(line), file))
    {
        if (validateCredentials(line, userEmail, password))
        {

            loginSuccessful = 1;
            break;
        }
    }

    fclose(file);

    if (loginSuccessful)
    {
        printf("Login successful!\n");
        return 1;
    }
    else
    {

        return 0;
    }
}

// admin panel
void adminPanel()
{
    int choice;
    int logoutFlag = 0;

    do
    {
        printf("\n______________ERP Management System  - Admin Panel______________\n\n");
        printf("1. Add Employee\n");
        printf("2. View Employees\n");
        printf("3. Payroll\n");
        printf("4. View Payroll\n");
        printf("5. Generate and Send Salary Report\n");
        printf("6. Add Admin\n");
        printf("7. View Admin\n");
        printf("8. View Requistion\n");
        printf("8. View Activites\n");
        printf("10. Logout\n\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        printf("\n\n");

        switch (choice)
        {
            case 1:
                insertEmployee();
            break;
            case 2:
                viewAllEmployee();
            break;
            case 3:
                calculatePayroll();
            break;
            case 4:
                viewPayrollForAdmin();
            break;
            case 5:
                generateSalaryReport();
            break;
            case 6:
                insertAdmin();
            break;
            case 7:
                viewAllAdmin();
            break;
            case 8:
                viewAllRequisitionForAdmin();
            break;
            case 9:
                viewAllRequisitionForAdmin();
            break;
            case 10:
                printf("Logging out...\n");
            logoutFlag = 1;
            break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (!logoutFlag);
}

// get admin id for next admin
int getNextAdminID()
{
    int id, maxID = 0;
    FILE *file = fopen("user_admin.csv", "r");

    if (file)
    {
        char line[200];
        while (fgets(line, sizeof(line), file))
        {
            if (sscanf(line, "id:%d,", &id) == 1)
            {
                if (id > maxID)
                {
                    maxID = id;
                }
            }
        }
        fclose(file);
    }

    return maxID + 1;
}

// insert admin
void insertAdmin()
{
    FILE *file = fopen("user_admin.csv", "a+"); // Open file in append mode with read capability
    if (file == NULL)
    {
        printf("Unable to open file.\n");
        return;
    }

    // Check if the file is empty
    fseek(file, 0, SEEK_END);
    if (ftell(file) == 0)
    {
        // Write the header if the file is empty
        fprintf(file, "id,name,email,password,status,created_at\n");
        printf("Header added to the file.\n");
    }

    Admin adm;
    char user_pass[50];

    // Generate a unique ID for the admin
    adm.id = getNextAdminID();
    printf("Generated Admin ID: %d\n", adm.id);

    // Get admin details
    printf("Enter Admin Name: ");
    scanf(" %[^\n]s", adm.name);

    printf("Enter Admin Email: ");
    scanf("%s", adm.email);

    printf("Enter Admin Password: ");
    scanf("%s", user_pass);

    // Check password length
    if (strlen(user_pass) < 7)
    {
        printf("Error: Password cannot be less than 7 characters.\n\n");
        fclose(file);
        return;
    }

    strcpy(adm.password, user_pass);

    // Get the current time
    time_t t;
    char buffer[11];
    time(&t);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", localtime(&t));

    strcpy(adm.created_at, buffer);
    adm.status = 1; // Active status

    // Write admin data to the file
    fprintf(file, "%d,\"%s\",\"%s\",\"%s\",%d,\"%s\"\n",
            adm.id, adm.name, adm.email, adm.password, adm.status, adm.created_at);

    fclose(file);

    printf("Admin added successfully!\n");
}

// view all admin list
void viewAllAdmin()
{
    FILE *file = fopen("user_admin.csv", "r");
    if (file == NULL)
    {
        printf("Unable to open file.\n");
        return;
    }

    printf("========= Admin List =========\n");
    printf("ID, Name, Email, Status, Created At\n");

    Admin adm;
    char line[256]; // Buffer to hold each line

    // Read each line in the file
    while (fgets(line, sizeof(line), file))
    {
        // Adjusted sscanf format string with spaces after commas
        if (sscanf(line, "id:%d, name: %49[^,], email: %49[^,], password:%*[^,], status:%d, created_at:%49[^\n]",
                   &adm.id, adm.name, adm.email, &adm.status, adm.created_at) == 5)
        {
            // Print the admin information
            printf("%d, %s, %s, %d, %s\n", adm.id, adm.name, adm.email, adm.status, adm.created_at);
        }
    }

    fclose(file);
}
