#include "erp.h"
char line[MAX_LINE_LENGTH];



void insertPayroll() {
    printf("Calculate Payroll Functionality\n");

    FILE *file = fopen("employees.csv", "r"); // Open the employee file to read base salary
    if (file == NULL) {
        printf("Unable to open employees file.\n");
        return;
    }

    FILE *salaryFile = fopen("emp_salary.csv", "a+"); // Open salary file in append mode
    if (salaryFile == NULL) {
        printf("Unable to open salary file.\n");
        fclose(file);
        return;
    }

    // Check if the salary file is empty, if so, add column headers
    fseek(salaryFile, 0, SEEK_END);
    if (ftell(salaryFile) == 0) {
        fprintf(salaryFile, "id,name,base_salary,additions,deductions,total_salary,month,paid_date\n");
    }

    rewind(file); // Reset the file pointer to the start
    fgets(line, sizeof(line), file); // Skip the header line in the employee file

    char line[MAX_LINE_LENGTH];
    char month[20];
    char buffer[20];
    time_t t;

    // Get the current time for paid date
    time(&t);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", localtime(&t));

    // Ask for month input (e.g., '2024-11' format)
    printf("Enter the month to pay salary (YYYY-MM format): ");
    scanf("%s", month);

    // Ask if the user wants to pay a single employee or all employees
    printf("Do you want to pay a single employee or all employees? (1 for single, 2 for all): ");
    int choice;
    scanf("%d", &choice);

    if (choice == 1) {
        // Process payroll for a single employee
        printf("Select an employee to pay salary:\n");
        int employeeCount = 0;
        Employee employees[100];

        // Read all employees from the file
        while (fgets(line, sizeof(line), file)) {
            Employee emp;
            if (sscanf(line, "%d,\"%99[^\"]\",\"%99[^\"]\",%f,\"%99[^\"]\",\"%29[^\"]\",%d,\"%99[^\"]\"",
                       &emp.id, emp.name, emp.email, &emp.salary, emp.designation, emp.password, &emp.status, emp.created_at)) {
                employees[employeeCount++] = emp;
                printf("%d. %s - Base Salary: %.2f\n", emp.id, emp.name, emp.salary);
            }
        }

        printf("Enter employee ID to pay: ");
        int empId;
        scanf("%d", &empId);

        Employee selectedEmp;
        int employeeFound = 0;
        for (int i = 0; i < employeeCount; i++) {
            if (employees[i].id == empId) {
                selectedEmp = employees[i];
                employeeFound = 1;
                break;
            }
        }

        if (!employeeFound) {
            printf("Employee not found.\n");
            fclose(file);
            fclose(salaryFile);
            return;
        }

        // Check if the employee has already been paid for this month
        rewind(salaryFile); // Reset salary file pointer
        int alreadyPaid = 0;
        while (fgets(line, sizeof(line), salaryFile)) {
            SalaryRecord salaryRecord;
            if (sscanf(line, "%d,%49[^,],%lf,%lf,%lf,%lf,%19[^,],%19[^\n]",
                       &salaryRecord.id, salaryRecord.name, &salaryRecord.base_salary,
                       &salaryRecord.additions, &salaryRecord.deductions, &salaryRecord.total_salary,
                       salaryRecord.month, salaryRecord.paid_date)) {
                if (salaryRecord.id == selectedEmp.id && strcmp(salaryRecord.month, month) == 0) {
                    printf("Employee %s has already been paid for the month %s.\n", salaryRecord.name, month);
                    alreadyPaid = 1;
                    break;
                }
            }
        }

        if (alreadyPaid) {
            fclose(file);
            fclose(salaryFile);
            return;
        }

        SalaryRecord salaryRecord;
        salaryRecord.id = selectedEmp.id;
        strcpy(salaryRecord.name, selectedEmp.name);
        salaryRecord.base_salary = selectedEmp.salary;
        strcpy(salaryRecord.month, month);

        // Get additions (bonuses, overtime, etc.)
        printf("Enter additions for employee %s: ", selectedEmp.name);
        scanf("%lf", &salaryRecord.additions);

        // Get deductions (taxes, leave, etc.)
        printf("Enter deductions for employee %s: ", selectedEmp.name);
        scanf("%lf", &salaryRecord.deductions);

        // Calculate total salary
        salaryRecord.total_salary = salaryRecord.base_salary + salaryRecord.additions - salaryRecord.deductions;

        // Record the salary payment details
        fprintf(salaryFile, "%d,%s,%.2f,%.2f,%.2f,%.2f,%s,%s\n",
                salaryRecord.id, salaryRecord.name, salaryRecord.base_salary,
                salaryRecord.additions, salaryRecord.deductions, salaryRecord.total_salary,
                salaryRecord.month, buffer);

        printf("Salary processed for %s: %.2f\n", selectedEmp.name, salaryRecord.total_salary);

    } else if (choice == 2) {
        // Process payroll for all employees
        rewind(file); // Reset file pointer
        fgets(line, sizeof(line), file); // Skip the header line
        while (fgets(line, sizeof(line), file)) {
            Employee emp;
            if (sscanf(line, "%d,\"%99[^\"]\",\"%99[^\"]\",%f,\"%99[^\"]\",\"%29[^\"]\",%d,\"%99[^\"]\"",
                       &emp.id, emp.name, emp.email, &emp.salary, emp.designation, emp.password, &emp.status, emp.created_at)) {
                // Check if the employee has already been paid for this month
                rewind(salaryFile); // Reset salary file pointer
                int alreadyPaid = 0;
                while (fgets(line, sizeof(line), salaryFile)) {
                    SalaryRecord salaryRecord;
                    if (sscanf(line, "%d,%49[^,],%lf,%lf,%lf,%lf,%19[^,],%19[^\n]",
                               &salaryRecord.id, salaryRecord.name, &salaryRecord.base_salary,
                               &salaryRecord.additions, &salaryRecord.deductions, &salaryRecord.total_salary,
                               salaryRecord.month, salaryRecord.paid_date)) {
                        if (salaryRecord.id == emp.id && strcmp(salaryRecord.month, month) == 0) {
                            printf("Employee %s has already been paid for the month %s.\n", salaryRecord.name, month);
                            alreadyPaid = 1;
                            break;
                        }
                    }
                }

                if (alreadyPaid) {
                    continue; // Skip this employee
                }

                SalaryRecord salaryRecord;
                salaryRecord.id = emp.id;
                strcpy(salaryRecord.name, emp.name);
                salaryRecord.base_salary = emp.salary;
                strcpy(salaryRecord.month, month);

                // Get additions (bonuses, overtime, etc.)
                printf("Enter additions for employee %s: ", emp.name);
                scanf("%lf", &salaryRecord.additions);

                // Get deductions (taxes, leave, etc.)
                printf("Enter deductions for employee %s: ", emp.name);
                scanf("%lf", &salaryRecord.deductions);

                // Calculate total salary
                salaryRecord.total_salary = salaryRecord.base_salary + salaryRecord.additions - salaryRecord.deductions;

                // Record the salary payment details
                fprintf(salaryFile, "%d,%s,%.2f,%.2f,%.2f,%.2f,%s,%s\n",
                        salaryRecord.id, salaryRecord.name, salaryRecord.base_salary,
                        salaryRecord.additions, salaryRecord.deductions, salaryRecord.total_salary,
                        salaryRecord.month, buffer);

                printf("Salary processed for %s: %.2f\n", emp.name, salaryRecord.total_salary);
            }
        }
    } else {
        printf("Invalid choice.\n");
    }

    fclose(file);
    fclose(salaryFile);

    printf("Payroll processed successfully!\n");
}




void viewPayrollForAdmin() {
    printf("View Payroll - Admin Panel\n");

    FILE *salaryFile = fopen("emp_salary.csv", "r"); // Open the salary file to read salary records
    if (salaryFile == NULL) {
        printf("Unable to open salary file.\n");
        return;
    }

    char line[MAX_LINE_LENGTH];
    char filterMonth[20];
    int filterChoice;
    int recordFound = 0; // To check if any record exists

    // Ask the admin if they want to filter by month
    printf("Do you want to filter records by a specific month? (1 for Yes, 2 for No): ");
    scanf("%d", &filterChoice);

    if (filterChoice == 1) {
        printf("Enter the month to filter records (YYYY-MM format): ");
        scanf("%s", filterMonth);
    }

    // Display column headers
    printf("\n%-5s %-20s %-15s %-10s %-10s %-15s %-10s %-15s\n",
           "ID", "Name", "Base Salary", "Additions", "Deductions",
           "Total Salary", "Month", "Paid Date");
    printf("------------------------------------------------------------------------------------------\n");

    // Skip the header row
    if (fgets(line, sizeof(line), salaryFile) == NULL) {
        printf("No data found in the file.\n");
        fclose(salaryFile);
        return;
    }

    // Read and display salary records
    while (fgets(line, sizeof(line), salaryFile)) {
        // Remove trailing newline character
        line[strcspn(line, "\n")] = 0;

        // Skip empty lines
        if (line[0] == '\0') {
            continue;
        }

        SalaryRecord salaryRecord;
        // Parse the line and ensure all fields are valid
        int fieldsParsed = sscanf(line, "%d,%49[^,],%lf,%lf,%lf,%lf,%19[^,],%19[^\n]",
                                  &salaryRecord.id, salaryRecord.name, &salaryRecord.base_salary,
                                  &salaryRecord.additions, &salaryRecord.deductions, &salaryRecord.total_salary,
                                  salaryRecord.month, salaryRecord.paid_date);
        if (fieldsParsed != 8) {
            continue; // Skip invalid or incomplete lines
        }

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

    if (!recordFound) {
        if (filterChoice == 1) {
            printf("No records found for the month %s.\n", filterMonth);
        } else {
            printf("No records found.\n");
        }
    }

    fclose(salaryFile);
}


void viewMyPayroll(char myEmail[]) {
    printf("View My Payroll\n");

    FILE *salaryFile = fopen("emp_salary.csv", "r"); // Open the salary file to read salary records
    if (salaryFile == NULL) {
        printf("Unable to open salary file.\n");
        return;
    }

    // Open employees file to get employee ID from email
    FILE *employeeFile = fopen("employees.csv", "r");
    if (employeeFile == NULL) {
        printf("Unable to open employees file.\n");
        fclose(salaryFile);
        return;
    }

    char line[MAX_LINE_LENGTH];
    char filterMonth[20];
    int filterChoice;
    int employeeId = 0;
    int recordFound = 0; // To check if any record exists
    char shownMonths[100][20];  // Array to store months already shown
    int shownMonthsCount = 0;    // Counter for how many months we've already displayed

    // Find the employee ID by email
    while (fgets(line, sizeof(line), employeeFile)) {
        Employee emp;
        if (sscanf(line, "%d,\"%99[^\"]\",\"%99[^\"]\",%f,\"%99[^\"]\",\"%29[^\"]\",%d,\"%99[^\"]\"",
                   &emp.id, emp.name, emp.email, &emp.salary, emp.designation, emp.password, &emp.status, emp.created_at)) {
            if (strcmp(emp.email, myEmail) == 0) {
                employeeId = emp.id; // Found the employee, store their ID
                break;
            }
        }
    }

    if (employeeId == 0) {
        printf("Employee not found.\n");
        fclose(salaryFile);
        fclose(employeeFile);
        return;
    }

    // Ask the user if they want to filter by month
    printf("Do you want to filter records by a specific month? (1 for Yes, 2 for No): ");
    scanf("%d", &filterChoice);

    if (filterChoice == 1) {
        printf("Enter the month to filter records (YYYY-MM format): ");
        scanf("%s", filterMonth);
    }

    // Read and display salary records for the logged-in employee
    printf("\n%-5s %-20s %-15s %-10s %-10s %-15s %-10s %-15s\n",
           "ID", "Name", "Base Salary", "Additions", "Deductions",
           "Total Salary", "Month", "Paid Date");
    printf("------------------------------------------------------------------------------------------\n");

    // Read the salary records file
    while (fgets(line, sizeof(line), salaryFile)) {
        SalaryRecord salaryRecord;
        if (sscanf(line, "%d,%49[^,],%lf,%lf,%lf,%lf,%19[^,],%19[^\n]",
                   &salaryRecord.id, salaryRecord.name, &salaryRecord.base_salary,
                   &salaryRecord.additions, &salaryRecord.deductions, &salaryRecord.total_salary,
                   salaryRecord.month, salaryRecord.paid_date)) {

            // Check if the salary belongs to the logged-in employee
            if (salaryRecord.id != employeeId) {
                continue;
            }

            // Apply the filter if chosen
            if (filterChoice == 1 && strcmp(salaryRecord.month, filterMonth) != 0) {
                continue; // Skip records that do not match the filter
            }


            // Check if this month has already been shown for this employee
            int isDuplicateMonth = 0;
            for (int i = 0; i < shownMonthsCount; i++) {
                if (strcmp(shownMonths[i], salaryRecord.month) == 0) {
                    isDuplicateMonth = 1;
                    break;
                }
            }

            // Debugging: Output the result of checking if the month is duplicate
            if (isDuplicateMonth) {
                continue; // Skip displaying this record, since it's a duplicate month for the employee
            }

            // Display the record
            printf("%-5d %-20s %-15.2f %-10.2f %-10.2f %-15.2f %-10s %-15s\n",
                   salaryRecord.id, salaryRecord.name, salaryRecord.base_salary,
                   salaryRecord.additions, salaryRecord.deductions, salaryRecord.total_salary,
                   salaryRecord.month, salaryRecord.paid_date);

            // Mark this month as shown for this employee
            strcpy(shownMonths[shownMonthsCount], salaryRecord.month);
            shownMonthsCount++;

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
    fclose(employeeFile);
}



