#include "erp.h"

//insert requisition for employee
void addRequisition(int employeeId)
{
    // Get the employee name
    const char *employeeName = getEmployeeNameById(employeeId);

    // Open the requisition items file to display available items
    FILE *reqFile = fopen("requisitions.csv", "r");
    if (reqFile == NULL)
    {
        printf("Unable to open requisitions file.\n");
        return;
    }

    // Display available requisition items
    printf("\nAvailable Requisition Items:\n");
    printf("ID\tItem Name\tQuantity\tDescription\n");
    printf("----------------------------------------------------------\n");

    char line[512];
    int id, availableQuantity;
    char itemName[100], description[255];

    // Read and display all requisition items
    while (fgets(line, sizeof(line), reqFile))
    {
        if (sscanf(line, "%d,%99[^,],%d,%254[^\n]", &id, itemName, &availableQuantity, description) == 4)
        {
            printf("%d\t%s\t\t%d\t\t%s\n", id, itemName, availableQuantity, description);
        }
    }
    fclose(reqFile);

    // Ask the employee for the item selection and requested quantity
    int selectedId, requestedQuantity;
    printf("\nEnter the ID of the requisition item you want to add: ");
    scanf("%d", &selectedId);
    printf("Enter the quantity you need: ");
    scanf("%d", &requestedQuantity);

    // Reopen the requisition file to find the selected item and update its quantity
    reqFile = fopen("requisitions.csv", "r+");  // Open in read/write mode
    if (reqFile == NULL)
    {
        printf("Unable to open requisitions file.\n");
        return;
    }

    int itemFound = 0;
    char updatedLines[512][512];  // To store updated lines
    int lineIndex = 0;
    char selectedItemName[100];   // Variable to store the selected item's name

    // Read all lines from the file to find and update the selected item
    while (fgets(line, sizeof(line), reqFile))
    {
        if (sscanf(line, "%d,%99[^,],%d,%254[^\n]", &id, itemName, &availableQuantity, description) == 4)
        {
            if (id == selectedId)
            {
                itemFound = 1;

                // Check if the requested quantity is available
                if (requestedQuantity > availableQuantity)
                {
                    printf("Requested quantity exceeds available stock. Try again with a lower quantity.\n");
                    fclose(reqFile);
                    return;
                }

                // Store the name of the selected item for the employee requisition
                strcpy(selectedItemName, itemName);

                // Update the available quantity
                availableQuantity -= requestedQuantity;

                // Store the updated line with the reduced quantity
                snprintf(updatedLines[lineIndex++], sizeof(updatedLines[lineIndex]), "%d,%s,%d,%s\n", id, itemName, availableQuantity, description);
                printf("Your requisition for '%s' (Qty: %d) has been submitted successfully!\n", itemName, requestedQuantity);
            }
            else
            {
                // If the item does not match, store the line as is
                snprintf(updatedLines[lineIndex++], sizeof(updatedLines[lineIndex]), "%s", line);
            }
        }
    }

    fclose(reqFile);

    // If the item was not found, inform the user and exit
    if (!itemFound)
    {
        printf("Invalid requisition item ID. Please try again.\n");
        return;
    }

    // Reopen the requisition file in write mode to update the stock
    reqFile = fopen("requisitions.csv", "w");  // Open in write mode to overwrite the file
    if (reqFile == NULL)
    {
        printf("Unable to open requisitions file to update stock.\n");
        return;
    }

    // Write the header back to the file
    fprintf(reqFile, "id,item_name,quantity,description\n");

    // Write the updated lines back to the file
    for (int i = 0; i < lineIndex; i++)
    {
        fprintf(reqFile, "%s", updatedLines[i]);
    }

    fclose(reqFile);

    // Now, add the requisition to the employee requisition file
    FILE *empReqFile = fopen("emp_requisition.csv", "a+");
    if (empReqFile == NULL)
    {
        printf("Unable to open employee requisition file.\n");
        return;
    }

    // Check if the file is empty, and add headers if needed
    fseek(empReqFile, 0, SEEK_END);
    if (ftell(empReqFile) == 0)
    {
        fprintf(empReqFile, "id,name,date,requisition,quantity\n");
    }

    // Get current date
    char date[20];
    time_t t;
    time(&t);
    strftime(date, sizeof(date), "%Y-%m-%d", localtime(&t));

    // Add the requisition details to the employee requisition file
    fprintf(empReqFile, "%d,%s,%s,%s,%d\n", employeeId, employeeName, date, selectedItemName, requestedQuantity);
    fclose(empReqFile);
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
    printf("%-10s  %-10s  %-50s\n", "Date", "Quantity", "Requisition");
    printf("--------------------------------------------------------------------------------\n");

    // Read the file line by line
    while (fgets(line, sizeof(line), file)) {
        int id, quantity;
        char name[100], date[20], requisition[MAX_LINE_LENGTH];

        // Parse the line considering the correct CSV structure (id,name,date,requisition,quantity)
        if (sscanf(line, "%d,%99[^,],%19[^,],%255[^,],%d", &id, name, date, requisition, &quantity) == 5) {
            if (id == employeeId) { // Match the employee ID
                printf("%-10s  %-10d  %-50s\n", date, quantity, requisition);
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

    // Skip the CSV header line
    fgets(line, sizeof(line), file);

    // Print the table header
    printf("\n%-15s %-15s %-20s %-50s %-10s\n", "ID", "NAME", "Date", "Requisition", "Quantity");
    printf("------------------------------------------------------------------------------------------\n");

    // Read and display all requisitions
    while (fgets(line, sizeof(line), file)) {
        int id, quantity;
        char name[50], date[20], requisition[255];

        // Parse the line: Extract id, name, date, requisition, and quantity from the file
        if (sscanf(line, "%d,%49[^,],%19[^,],%254[^,],%d", &id, name, date, requisition, &quantity) == 5) {
            // Apply the date filter if chosen
            if (filterChoice == 1 && strcmp(date, filterDate) != 0) {
                continue; // Skip records that do not match the filter
            }

            // Display the record
            printf("%-15d %-15s %-20s %-50s %-10d\n", id, name, date, requisition, quantity);
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
