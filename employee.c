// employee.c
#include "erp.h"


// Employee Login
#include "erp.h"

// Function that checks employee login credentials
// It returns the email of the logged-in employee or NULL if the login fails
char* employeeLogin() {
    static char loggedInEmail[100];  // Static so it persists after the function ends
    char inputEmail[100], inputPassword[100];
    FILE *employeeFile = fopen("employees.csv", "r");

    if (employeeFile == NULL) {
        printf("Unable to open employee file.\n");
        return NULL;
    }

    printf("Enter email: ");
    scanf("%s", inputEmail);

    printf("\nEnter password: ");
    scanf("%s", inputPassword);

    char line[MAX_LINE_LENGTH];

    while (fgets(line, sizeof(line), employeeFile)) {
        Employee emp;
        if (sscanf(line, "%d,\"%99[^\"]\",\"%99[^\"]\",%f,\"%99[^\"]\",\"%29[^\"]\",%d,\"%99[^\"]\"",
                   &emp.id, emp.name, emp.email, &emp.salary, emp.designation, emp.password, &emp.status, emp.created_at)) {
            if (strcmp(emp.email, inputEmail) == 0 && strcmp(emp.password, inputPassword) == 0) {
                strcpy(loggedInEmail, emp.email);  // Store the logged-in employee's email
                fclose(employeeFile);
                return loggedInEmail;  // Return the email of the logged-in employee
            }
                   }
    }

    fclose(employeeFile);
    printf("Invalid email or password.\n");
    return NULL;  // Return NULL if login fails
}

// employee panel
void employeePanel(char myEmail[])
{
    int choice;
    int logoutFlag = 0;  // Flag to control when to log out

    do
    {
        printf("\n______________ERP Management System  - Employee Panel______________\n\n");
        printf("1. Add Requisition\n");
        printf("2. View Requisition\n");
        printf("3. My Payroll\n");
        printf("4. Logout\n\n");
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
                viewMyPayroll(myEmail);
            break;
            case 4:
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
    FILE *file = fopen("employees.csv", "a+"); // Open file in append mode with read capability
    if (file == NULL)
    {
        printf("Unable to open file.\n");
        return;
    }

    // Check if the file is empty
    fseek(file, 0, SEEK_END);
    if (ftell(file) == 0)
    {
        // Write column headers
        fprintf(file, "id,name,email,salary,designation,password,status,created_at\n");

    }
    rewind(file); // Reset the file pointer to allow proper handling later

    Employee emp;
    char user_pass[30];

    // Generate a unique ID for the employee
    emp.id = getNextEmployeeID();
    printf("Generated Employee ID: %d\n", emp.id);

    // Get employee details
    printf("Enter Employee Name: ");
    scanf(" %[^\n]s", emp.name);

    printf("Enter Employee Email: ");
    scanf("%s", emp.email);

    printf("Enter Employee Designation: ");
    scanf(" %[^\n]s", emp.designation);

    printf("Enter Employee Salary: ");
    scanf("%f", &emp.salary);

    printf("Enter Employee Password: ");
    scanf("%s", user_pass);

    // Ensure password is valid
    if (strlen(user_pass) < 7)
    {
        printf("Error: Password must be at least 7 characters long.\n");
        fclose(file);
        return;
    }
    strcpy(emp.password, user_pass);

    // Get the current time
    time_t t = time(NULL);
    char buffer[11];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", localtime(&t));
    strcpy(emp.created_at, buffer);

    emp.status = 1; // Active status

    // Write employee data to the file in proper CSV format
    fprintf(file, "%d,\"%s\",\"%s\",%.2f,\"%s\",\"%s\",%d,\"%s\"\n",
            emp.id, emp.name, emp.email, emp.salary, emp.designation, emp.password, emp.status, emp.created_at);

    fclose(file);

    printf("Employee added successfully!\n");
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

//calculate payroll
void calculatePayroll()
{
    payroll();
}

void viewPayroll() {
    printf("------------------View Payroll Records------------------\n");

    FILE *salaryFile = fopen("emp_salary.csv", "r"); // Open the salary file to read salary records
    if (salaryFile == NULL) {
        printf("Unable to open salary file.\n");
        return;
    }

    char line[MAX_LINE_LENGTH];
    char filterMonth[20];
    int filterChoice;

    // Ask the user if they want to filter by month
    printf("Do you want to filter records by a specific month? (1 for Yes, 2 for No): ");
    scanf("%d", &filterChoice);

    if (filterChoice == 1) {
        printf("Enter the month to filter records (YYYY-MM format): ");
        scanf("%s", filterMonth);
    }

    // Read and display records
    int recordFound = 0; // To check if any record exists
    printf("\n%-5s %-20s %-15s %-10s %-10s %-15s %-10s %-15s\n",
           "ID", "Name", "Base Salary", "Additions", "Deductions",
           "Total Salary", "Month", "Paid Date");
    printf("------------------------------------------------------------------------------------------\n");

    while (fgets(line, sizeof(line), salaryFile)) {
        // Skip the header row
        if (strncmp(line, "id,", 3) == 0) {
            continue;
        }

        SalaryRecord salaryRecord;
        if (sscanf(line, "%d,%49[^,],%lf,%lf,%lf,%lf,%19[^,],%19[^\n]",
                   &salaryRecord.id, salaryRecord.name, &salaryRecord.base_salary,
                   &salaryRecord.additions, &salaryRecord.deductions, &salaryRecord.total_salary,
                   salaryRecord.month, salaryRecord.paid_date)) {

            // Apply the filter if chosen
            if (filterChoice == 1 && strcmp(salaryRecord.month, filterMonth) != 0) {
                continue; // Skip records that do not match the filter
            }

            // Display the record
            printf("%-5d %-20s %-15.2f %-10.2f %-10.2f %-15.2f %-10s %-15s\n",
                   salaryRecord.id, salaryRecord.name, salaryRecord.base_salary,
                   salaryRecord.additions, salaryRecord.deductions, salaryRecord.total_salary,
                   salaryRecord.month, salaryRecord.paid_date);
            recordFound = 1;
        }
    }

    if (!recordFound) {
        if (filterChoice == 1) {
            printf("No records found for the month %s.\n", filterMonth);
        } else {
            printf("No records found.\n");
        }
    }

    fclose(salaryFile);
}


void generateSalaryReport()
{
    printf("Generate and Send Salary Report Functionality\n");
}

int getNextEmployeeID() {
    FILE *file = fopen("employees.csv", "r");
    if (file == NULL) {
        return 1;  // If file doesn't exist, start with ID 1
    }

    int maxID = 0;
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        Employee emp;
        if (sscanf(line, "%d,\"%99[^\"]\",\"%99[^\"]\",%f,\"%99[^\"]\",\"%29[^\"]\",%d,\"%99[^\"]\"",
                   &emp.id, emp.name, emp.email, &emp.salary, emp.designation, emp.password, &emp.status, emp.created_at)) {
            if (emp.id > maxID) {
                maxID = emp.id;  // Find the maximum existing ID
            }
                   }
    }
    fclose(file);
    return maxID + 1;  // Return the next available ID
}
