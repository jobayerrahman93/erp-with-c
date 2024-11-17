#include "erp.h"

//insert requisition for employee
void addRequisition(int employeeId) {

    //getting employee name
    const char *employeeName = getEmployeeNameById(employeeId);

    FILE *file = fopen("emp_requisition.csv", "a+");
    if (file == NULL) {
        printf("Unable to open requisition file.\n");
        return;
    }

    // Check if the file is empty and add headers if necessary
    fseek(file, 0, SEEK_END);
    if (ftell(file) == 0) {
        fprintf(file, "id,name,date,requisition\n");
    }

    char requisition[MAX_LINE_LENGTH];
    char date[20];
    time_t t;

    // Get current date
    time(&t);
    strftime(date, sizeof(date), "%Y-%m-%d", localtime(&t));

    printf("Enter your requisition: ");
    getchar(); // Clear newline left by the previous scanf
    fgets(requisition, MAX_LINE_LENGTH, stdin);
    requisition[strcspn(requisition, "\n")] = '\0'; // Remove trailing newline

    // Write requisition details to the CSV file
    fprintf(file, "%d,%s,%s,\"%s\"\n", employeeId, employeeName, date, requisition);

    printf("Your requisition has been submitted successfully!\n");
    fclose(file);
}

//view requisition for employee
void viewRequisition(int employeeId) {
    FILE *file = fopen("emp_requisition.csv", "r");
    if (file == NULL) {
        printf("Unable to open requisition file.\n");
        return;
    }

    char line[MAX_LINE_LENGTH];
    int recordFound = 0; // Flag to check if any records exist

    printf("\nMy Requisitions:\n");
    printf("--------------------------------------------------------------------------------\n");
    printf("%-10s  %-50s\n", "Date",  "Requisition");
    printf("--------------------------------------------------------------------------------\n");

    // Read the file line by line
    while (fgets(line, sizeof(line), file)) {
        int id;
        char date[20];
        char requisition[MAX_LINE_LENGTH];

        // Parse the line
        if (sscanf(line, "%d,%*[^,],%19[^,],\"%[^\"]\"", &id, date, requisition) == 3) {
            if (id == employeeId) { // Match the employee ID
                printf("%-10s  %-50s\n", date, requisition);
                recordFound = 1;
            }
        }
    }

    if (!recordFound) {
        printf("No requisitions found.\n");
    }

    fclose(file);
}

//view requisition for admin
void viewAllRequisitionForAdmin() {
    FILE *file = fopen("emp_requisition.csv", "r");
    if (file == NULL) {
        printf("Unable to open requisition file.\n");
        return;
    }

    char line[MAX_LINE_LENGTH];
    char filterDate[20];
    int filterChoice;
    int recordFound = 0;

    // Ask the admin if they want to filter by a specific date
    printf("Do you want to filter records by a specific date? (1 for Yes, 2 for No): ");
    scanf("%d", &filterChoice);

    if (filterChoice == 1) {
        printf("Enter the date to filter records (YYYY-MM-DD): ");
        scanf("%s", filterDate);
    }

    // Print the table header
    printf("\n%-15s %-15s %-35s %-50s\n", "ID", "NAME", "Date", "Requisition");
    printf("-------------------------------------------------------------\n");

    // Read and display all requisitions
    while (fgets(line, sizeof(line), file)) {
        int id;
        char name[50];
        char date[20];
        char requisition[MAX_LINE_LENGTH];

        // Parse the line: Extract id, name, date, and requisition from the file
        if (sscanf(line, "%d,%49[^,],%19[^,],\"%[^\"]\"", &id, name, date, requisition) == 4) {
            // Apply the date filter if chosen
            if (filterChoice == 1 && strcmp(date, filterDate) != 0) {
                continue; // Skip records that do not match the filter
            }

            // Display the record
            printf("%-15d %-15s %-35s %-50s\n", id, name, date, requisition);
            recordFound = 1;
        }
    }

    if (!recordFound) {
        if (filterChoice == 1) {
            printf("No records found for the date %s.\n", filterDate);
        } else {
            printf("No records found.\n");
        }
    }

    fclose(file);
}
