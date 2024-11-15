// erp.h
#ifndef ERP_H
#define ERP_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LINE_LENGTH 1024

// Employee Structure
typedef struct
{
    int id;
    char name[100];
    char email[100];
    float salary;
    char designation[100];
    char password[30];
    int status;
    char created_at[100];
    char last_paid[20]; // Add this field to track last salary paid date
} Employee;


// Admin Structure
typedef struct
{
    int id;
    char name[50];
    char email[50];
    char password[30];
    int status;
   char created_at [100];;
} Admin;


// Salary record structure for salary payment information
typedef struct {
    int id;
    char name[50];
    double base_salary;
    double additions;
    double deductions;
    double total_salary;
    char month[20];
    char paid_date[20];
} SalaryRecord;

// Function Declarations
int adminLogin();
void adminPanel();
void insertEmployee();
void viewAllEmployee();
void calculatePayroll();
void generateSalaryReport();
int getNextEmployeeID();
int getNextAdminID();
void insertAdmin();
void viewAllAdmin();
int employeeLogin();
void employeePanel();
void payroll();
void viewPayroll();

#endif // ERP_H
