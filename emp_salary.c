#include "erp.h"


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
    fclose(employeeFile);
}
