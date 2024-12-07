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
        printf("8. Add Requisition Item\n");
        printf("9. View Requisition Items\n");
        printf("10. View Requisitions\n");
        printf("11. View Activities\n");
        printf("12. Logout\n\n");
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
                addRequisitionItem();
            break;
            case 9:
                viewAllRequisitionItems();
            break;
            case 10:
                viewAllRequisitionForAdmin();
            break;
            case 11:
                viewActivitiesForAdmin();
            break;
            case 12:
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
        char line[MAX_LINE_LENGTH];

        // Skip the header line
        fgets(line, sizeof(line), file);

        // Read each line and extract the ID
        while (fgets(line, sizeof(line), file))
        {
            // Parse the line: format for parsing the CSV entry
            if (sscanf(line, "%d,\"%*[^,]\",\"%*[^,]\",%*[^,],%d,\"%*[^,]\"", &id) == 1)
            {
                if (id > maxID)
                {
                    maxID = id;
                }
            }
        }
        fclose(file);
    }

    return maxID + 1;  // The next available ID is the max ID + 1
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

    // Skip the header line
    fgets(line, sizeof(line), file);

    // Read each line in the file
    while (fgets(line, sizeof(line), file))
    {
        // Remove newline characters from the end of the line
        line[strcspn(line, "\n")] = '\0';

        // Use sscanf to parse the line properly
        if (sscanf(line, "%d,\"%49[^\"]\",\"%49[^\"]\",%*[^,],%d,\"%49[^\"]\"",
                   &adm.id, adm.name, adm.email, &adm.status, adm.created_at) == 5)
        {
            // Print the admin information
            printf("%d, %s, %s, %d, %s\n", adm.id, adm.name, adm.email, adm.status, adm.created_at);
        }
    }

    fclose(file);
}


// add requisition item
void addRequisitionItem()
{
    char itemName[100];
    int quantity;
    char description[255];
    FILE *file, *tempFile;
    int lastID = 0, newID;

    // Open the file in read mode to determine the last ID
    file = fopen("requisitions.csv", "r");
    if (file != NULL)
    {
        char line[512];
        while (fgets(line, sizeof(line), file))
        {
            // Extract the ID from the line (first value before the comma)
            int currentID;
            sscanf(line, "%d,", &currentID);
            lastID = currentID; // Update the last ID
        }
        fclose(file);
    }

    // Increment the last ID to generate a new ID
    newID = lastID + 1;

    // Open the file in append mode
    file = fopen("requisitions.csv", "a");
    if (file == NULL)
    {
        printf("Error opening requisitions file.\n");
        return;
    }

    // Collect requisition details
    printf("Enter Item Name: ");
    scanf(" %[^\n]", itemName); // Allows spaces in input
    printf("Enter Quantity: ");
    scanf("%d", &quantity);
    printf("Enter Description: ");
    scanf(" %[^\n]", description);

    // Save the details to the file
    fprintf(file, "%d,%s,%d,%s\n", newID, itemName, quantity, description);
    fclose(file);

    printf("Requisition item added successfully with ID: %d\n", newID);
}


// view requisition item
void viewAllRequisitionItems()
{
    FILE *file;
    char line[512];

    // Open the file in read mode
    file = fopen("requisitions.csv", "r");
    if (file == NULL)
    {
        printf("No requisitions found or error opening the file.\n");
        return;
    }

    printf("\n______________Requisition Items______________\n");
    printf("ID\tItem Name\tQuantity\tDescription\n");
    printf("----------------------------------------------------------\n");

    // Read each line and display it
    while (fgets(line, sizeof(line), file))
    {
        int id, quantity;
        char itemName[100], description[255];

        // Parse the line to extract data
        sscanf(line, "%d,%99[^,],%d,%254[^\n]", &id, itemName, &quantity, description);

        // Display the data
        printf("%d\t%s\t\t%d\t\t%s\n", id, itemName, quantity, description);
    }

    fclose(file);
}