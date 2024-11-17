


// Employee Login
#include "erp.h"


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
    int choice,employeeId;
    char employeeName[100];
    int logoutFlag = 0;  // Flag to control when to log out


    // Assuming you have logic to get employee ID and name based on email
    employeeId = getEmployeeIdByEmail(myEmail);



    do
    {
        printf("\n______________ERP Management System  - Employee Panel______________\n\n");
        printf("1. Add Requisition\n");
        printf("2. View Requisition\n");
        printf("3. My Payroll\n");
        printf("4. Add Activities\n");
        printf("5. View Activities\n");
        printf("6. Logout\n\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        printf("\n\n");

        switch (choice)
        {
            case 1:
                addRequisition(employeeId);
            break;
            case 2:
                 viewRequisition(employeeId);;
            break;
            case 3:
                viewMyPayroll(myEmail);
            break;
            case 4:
                addActivity(employeeId);;
            break;
            case 5:
                viewActivities(employeeId);
            break;
            case 6:
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
    char line[MAX_LINE_LENGTH]; // Buffer to hold each line

    // Skip the header line
    fgets(line, sizeof(line), file);

    // Read each line in the file
    while (fgets(line, sizeof(line), file))
    {
        // Use sscanf to parse the line without prefixes like "id:", "name:", etc.
        if (sscanf(line, "%d,%99[^,],%99[^,],%f,%99[^,],%99[^,],%d,%99[^\n]",
                   &emp.id, emp.name, emp.email, &emp.salary, emp.designation, emp.password, &emp.status, emp.created_at) == 8)
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
    insertPayroll();
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


// Get employee ID by email
int getEmployeeIdByEmail(char *email) {
    FILE *file = fopen("employees.csv", "r");
    if (file == NULL) {
        printf("Unable to open employees file.\n");
        return -1;
    }

    char line[MAX_LINE_LENGTH];
    int id;
    char empEmail[100];

    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "%d,\"%*99[^\"]\",\"%99[^\"]\"", &id, empEmail)) {
            if (strcmp(empEmail, email) == 0) {
                fclose(file);
                return id;
            }
        }
    }

    fclose(file);
    return -1; // Return -1 if employee is not found
}

const char *getEmployeeNameById(int id) {
    static char empName[100]; // Static to allow returning a pointer
    FILE *file = fopen("employees.csv", "r");
    if (file == NULL) {
        printf("Unable to open employees file.\n");
        return "Unknown";
    }

    char line[500];
    int empId;

    while (fgets(line, sizeof(line), file)) {
        // Parse the CSV line
        if (sscanf(line, "%d,\"%99[^\"]\"", &empId, empName)) {
            if (empId == id) {
                fclose(file);
                return empName;
            }
        }
    }

    fclose(file);
    return "Unknown"; // If no matching ID is found
}
