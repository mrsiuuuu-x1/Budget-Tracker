#ifndef BUDGET_H
#define BUDGET_H

#define MAX_INCOME 100
#define MAX_EXPENSES 200
#define MAX_TEXT_LEN 100
#define NUM_CATEGORIES 6

#define DATA_FILE "records.txt"
#define REPORT_FILE "monthly_report.txt"

/* Categories:
   1 = Food, 2 = Transport, 3 = Education,
   4 = Shopping, 5 = Entertainment, 6 = Other */

typedef struct {
    int day;
    int month;
    int year;
} Date;

typedef struct {
    double amount;
    char source[MAX_TEXT_LEN];
} Income;

typedef struct {
    double base_amount;
    double tax_rate;
    double tax_amount;
    double total_amount;
    Date date;
    char description[MAX_TEXT_LEN];
    int category;
} Expense;

typedef struct {
    double monthly_budget;
    Income incomes[MAX_INCOME];
    int income_count;
    Expense expenses[MAX_EXPENSES];
    int expense_count;
} BudgetTracker;

/* Setup & input helpers */
void init_tracker(BudgetTracker *tracker);
const char *get_category_name(int category);
void clear_input_buffer(void);
void read_line(char *buffer, int size);

/* Core actions */
void set_monthly_budget(BudgetTracker *tracker);
void add_income(BudgetTracker *tracker);
void add_expense(BudgetTracker *tracker);

/* Calculations & alerts */
double calculate_total_income(const BudgetTracker *tracker);
double calculate_total_expenses(const BudgetTracker *tracker);
double calculate_total_tax(const BudgetTracker *tracker);
double calculate_remaining_balance(const BudgetTracker *tracker);
void calculate_category_totals(const BudgetTracker *tracker, double totals[NUM_CATEGORIES]);
void check_budget_alerts(const BudgetTracker *tracker);

/* Reports & records */
void view_records(const BudgetTracker *tracker);
void print_monthly_report(const BudgetTracker *tracker);
int export_monthly_report(const BudgetTracker *tracker, const char *filename);

/* File handling */
int save_tracker_data(const BudgetTracker *tracker, const char *filename);
int load_tracker_data(BudgetTracker *tracker, const char *filename);

#endif