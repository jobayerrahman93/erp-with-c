// erp.h
#ifndef ERP_H
#define ERP_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    char created_at [100];
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

#endif // ERP_H
