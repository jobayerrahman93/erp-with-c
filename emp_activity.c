#include "erp.h"
#define MAX_ACTIVITY_LENGTH 700


void addActivity(int employeeId) {
    const char *employeeName = getEmployeeNameById(employeeId);

    // Open the activities file in append mode
    FILE *file = fopen("emp_activities.csv", "a+");
    if (file == NULL) {
        printf("Unable to open activities file.\n");
        return;
    }

    // Check if the file is empty and add headers if necessary
    fseek(file, 0, SEEK_END);
    if (ftell(file) == 0) {
        fprintf(file, "id,name,date,activity\n");
    }

    char activity[MAX_ACTIVITY_LENGTH];
    char date[20];
    time_t t;

    // Get current date
    time(&t);
    strftime(date, sizeof(date), "%Y-%m-%d", localtime(&t));

    printf("Enter your daily activity: ");
    getchar(); // Clear newline left by the previous scanf
    fgets(activity, MAX_ACTIVITY_LENGTH, stdin);
    activity[strcspn(activity, "\n")] = '\0'; // Remove trailing newline

    // Write the activity details to the CSV file
    fprintf(file, "%d,%s,%s,\"%s\"\n", employeeId, employeeName, date, activity);

    printf("Your activity has been recorded successfully!\n");

    fclose(file);
}

void viewActivities(int employeeId) {
    FILE *file = fopen("emp_activities.csv", "r");
    if (file == NULL) {
        printf("Unable to open activities file.\n");
        return;
    }

    char line[MAX_LINE_LENGTH];
    char filterDate[20];
    int filterChoice;
    int recordFound = 0;

    // Ask the employee if they want to filter by a specific date
    printf("Do you want to filter records by a specific date? (1 for Yes, 2 for No): ");
    scanf("%d", &filterChoice);

    if (filterChoice == 1) {
        printf("Enter the date to filter records (YYYY-MM-DD): ");
        scanf("%s", filterDate);
    }

    // Print the table header
    printf("\n%-15s %-10s %-35s %-50s\n", "ID", "Name", "Date", "Activity");
    printf("-------------------------------------------------------------\n");

    // Read and display all activities for the employee
    while (fgets(line, sizeof(line), file)) {
        int id;
        char date[20];
        char name[50];  // Store employee name
        char activity[MAX_ACTIVITY_LENGTH];

        // Parse the line: Modify sscanf to capture name as well
        if (sscanf(line, "%d,%49[^,],%19[^,],\"%[^\"]\"", &id, name, date, activity) == 4) {
            // Check if the activity belongs to the logged-in employee
            if (id != employeeId) {
                continue; // Skip activities of other employees
            }

            // Apply the date filter if chosen
            if (filterChoice == 1 && strcmp(date, filterDate) != 0) {
                continue; // Skip records that do not match the filter
            }

            // Display the record
            printf("%-15d %-10s %-35s %-50s\n", id, name, date, activity);
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



void viewActivitiesForAdmin() {
    FILE *file = fopen("emp_activities.csv", "r");
    if (file == NULL) {
        printf("Unable to open activities file.\n");
        return;
    }

    char line[MAX_LINE_LENGTH];
    char filterDate[20];
    int filterChoice, employeeChoice, recordFound = 0;
    int employeeId = -1; // Initialize to -1 to track employee ID only when needed

    // Ask the admin if they want to view activities for all employees or a specific employee
    printf("Do you want to view activities for all employees or a specific employee? (1 for All, 2 for Specific Employee): ");
    scanf("%d", &employeeChoice);

    // Ask if they want to filter by date
    printf("Do you want to filter records by a specific date? (1 for Yes, 2 for No): ");
    scanf("%d", &filterChoice);

    if (filterChoice == 1) {
        printf("Enter the date to filter records (YYYY-MM-DD): ");
        scanf("%s", filterDate);
    }

    // If the admin chooses to view activities for a specific employee, ask for employee ID once
    if (employeeChoice == 2) {
        printf("Enter employee ID to filter activities for: ");
        scanf("%d", &employeeId);
    }

    // Print the table header
    printf("\n%-15s %-10s %-35s %-50s\n", "ID", "Name", "Date", "Activity");
    printf("-------------------------------------------------------------\n");

    // Read and display activities based on the filter choices
    while (fgets(line, sizeof(line), file)) {
        int id;
        char date[20];
        char name[50];  // Store employee name
        char activity[MAX_ACTIVITY_LENGTH];

        // Parse the line: Modify sscanf to capture name as well
        if (sscanf(line, "%d,%49[^,],%19[^,],\"%[^\"]\"", &id, name, date, activity) == 4) {
            // If employeeChoice is 2, only show activities for the specific employee
            if (employeeChoice == 2 && id != employeeId) {
                continue; // Skip activities that don't match the selected employee
            }

            // Apply the date filter if chosen
            if (filterChoice == 1 && strcmp(date, filterDate) != 0) {
                continue; // Skip records that do not match the filter
            }

            // Display the record
            printf("%-15d %-10s %-35s %-50s\n", id, name, date, activity);
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