// employee.c
#include "erp.h"
#include <time.h>

// Employee Login
int employeeLogin()
{
    char userEmail[50], password[50];
    char line[MAX_LINE_LENGTH];
    char storedName[50], storedUserEmail[50], storedPassword[50], storedDesignation[50];
    int storedId, storedStatus;
    double storedSalary;
    char createdAt[20];

    // Prompt for email and password
    printf("Enter Employee Email: ");
    scanf("%s", userEmail);
    printf("\nEnter Employee Password: ");
    scanf("%s", password);

    // Open the employees.csv file for reading
    FILE *file = fopen("employees.csv", "r");
    if (file == NULL)
    {
        printf("Unable to open employees file.\n");
        return 0;
    }

    int loginSuccessful = 0;
    while (fgets(line, sizeof(line), file))
    {
        // Use sscanf to parse the line from the CSV
        if (sscanf(line, "id:%d, name:%[^,], email:%[^,], salary:%lf, designation:%[^,], password:%[^,], status:%d, created_at:%s",
                   &storedId, storedName, storedUserEmail, &storedSalary, storedDesignation, storedPassword, &storedStatus, createdAt))
        {
            // Validate credentials
            if (strcmp(userEmail, storedUserEmail) == 0 && strcmp(password, storedPassword) == 0)
            {
                loginSuccessful = 1;
                break;
            }
        }
    }

    fclose(file);

    if (loginSuccessful)
    {
        printf("Login successful!\n\n");

        printf("Welcome, %s (Designation: %s)\n", storedName, storedDesignation);
        return 1;
    }
    else
    {

        return 0;
    }
}

// employee panel
void employeePanel()
{
    int choice;
    int logoutFlag = 0;  // Flag to control when to log out

    do
    {
        printf("\n______________ERP Management System  - Employee Panel______________\n\n");
        printf("1. Add Requisition\n");
        printf("2. View Requisition\n");
        printf("3. Logout\n\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        printf("\n\n");

        switch (choice)
        {
            case 1:
                printf("This module is under development\n");
            break;
            case 2:
                printf("This module is under development\n");
            break;
            case 3:
                printf("Logging out...\n");
            logoutFlag = 1;
            break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (!logoutFlag);
}

// Insert employee
void insertEmployee()
{
    FILE *file = fopen("employees.csv", "a"); // Open file in append mode
    if (file == NULL)
    {
        printf("Unable to open file.\n");
        return;
    }

    Employee emp;
    char user_pass[30];

    // Generate a unique ID for the employee
    emp.id = getNextEmployeeID();
    printf("Generated Employee ID: %d\n", emp.id);

    // Get employee details
    printf("Enter Employee Name: ");
    scanf(" %[^\n]s", emp.name); // To read string with spaces

    printf("Enter Employee Email: ");
    scanf("%s", emp.email);

    printf("Enter Employee Designation: ");
    scanf(" %[^\n]s", emp.designation); // To read string with spaces

    printf("Enter Employee Salary: ");
    scanf("%f", &emp.salary); // Input salary as float

    printf("Enter Employee Password: ");
    scanf("%s", user_pass); // Input password

    // Ensure password has at least 7 characters
    if (strlen(user_pass) < 7)
    {
        printf("Error: Password must be at least 7 characters long.\n\n");
        return;
    }

    // Copy the password to the struct
    strcpy(emp.password, user_pass);

    // Get the current time
    time_t t;
    char buffer[11];

    // Get the current calendar time
    time(&t);

    // Format the time into a string (YYYY-MM-DD)
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", localtime(&t));

    // Copy formatted time to created_at field
    strcpy(emp.created_at, buffer);
    emp.status = 1; // Active status

    // Write employee data to the file
    fprintf(file, "id:%d, name:%s, email:%s, salary:%.2f, designation:%s, password:%s, status:%d, created_at:%s\n",
            emp.id, emp.name, emp.email, emp.salary, emp.designation, emp.password, emp.status, emp.created_at);

    fclose(file);

    printf("Employee added successfully!\n");
    return;
}

// View all employees
void viewAllEmployee()
{
    FILE *file = fopen("employees.csv", "r");
    if (file == NULL)
    {
        printf("Unable to open file.\n");
        return;
    }

    printf("========= Employee List =========\n");
    printf("ID, Name, Email, Salary, Designation, Status, Created At\n");

    Employee emp;
    char line[256]; // Buffer to hold each line

    // Read each line in the file
    while (fgets(line, sizeof(line), file))
    {
        // Use sscanf to parse the line with labels and spaces after commas
        if (sscanf(line, "id:%d, name: %99[^,], email: %99[^,], salary:%f, designation: %99[^,], password:%*[^,], status:%d, created_at:%99[^\n]",
                   &emp.id, emp.name, emp.email, &emp.salary, emp.designation, &emp.status, emp.created_at) == 7)
        {
            // Print the employee information
            printf("%d, %s, %s, %.2f, %s, %d, %s\n", emp.id, emp.name, emp.email, emp.salary, emp.designation, emp.status, emp.created_at);
        }
    }

    fclose(file);
}

void calculatePayroll()
{
    printf("Calculate Payroll Functionality\n");
}

void generateSalaryReport()
{
    printf("Generate and Send Salary Report Functionality\n");
}

int getNextEmployeeID()
{
    int id, maxID = 0;
    char line[MAX_LINE_LENGTH];

    FILE *file = fopen("employees.csv", "r");
    if (file == NULL)
    {
        printf("Unable to open employees file.\n");
        return 1;  // Return a default ID if the file can't be opened
    }

    while (fgets(line, sizeof(line), file))
    {
        // Use sscanf to extract the ID from the line
        if (sscanf(line, "id:%d, %*[^,]", &id) == 1)
        {
            if (id > maxID)
            {
                maxID = id;
            }
        }
    }

    fclose(file);

    return maxID + 1;  // Return the next available ID
}

