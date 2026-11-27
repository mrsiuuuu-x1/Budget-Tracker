#include <stdio.h>
#include <string.h>
#include "budget.h"

int main(void) {
    BudgetTracker tracker;
    int choice;

    init_tracker(&tracker);
    if (load_tracker_data(&tracker, DATA_FILE)) {
        printf("Loaded existing records from %s\n", DATA_FILE);
    }

    do {
        printf("\n==============================\n");
        printf("        BUDGET TRACKER        \n");
        printf("==============================\n");
        printf("1. Set Monthly Budget\n");
        printf("2. Add Income\n");
        printf("3. Add Expense\n");
        printf("4. View Financial Records\n");
        printf("5. Monthly Summary Report\n");
        printf("6. Save Data & Export Report\n");
        printf("0. Save & Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            clear_input_buffer();
            choice = -1;
            continue;
        }
        clear_input_buffer();

        switch (choice) {
            case 1:
                set_monthly_budget(&tracker);
                save_tracker_data(&tracker, DATA_FILE);
                break;
            case 2:
                add_income(&tracker);
                save_tracker_data(&tracker, DATA_FILE);
                break;
            case 3:
                add_expense(&tracker);
                save_tracker_data(&tracker, DATA_FILE);
                break;
            case 4:
                view_records(&tracker);
                break;
            case 5:
                print_monthly_report(&tracker);
                break;
            case 6:
                if (save_tracker_data(&tracker, DATA_FILE)) {
                    printf("\nSaved records to %s\n", DATA_FILE);
                } else {
                    printf("\nCould not save records to %s\n", DATA_FILE);
                }
                if (export_monthly_report(&tracker, REPORT_FILE)) {
                    printf("Exported monthly report to %s\n", REPORT_FILE);
                } else {
                    printf("Could not export report to %s\n", REPORT_FILE);
                }
                break;
            case 0:
                save_tracker_data(&tracker, DATA_FILE);
                printf("\nData saved. Exiting Budget Tracker. Goodbye!\n");
                break;
            default:
                printf("\nInvalid choice. Please select 0-6.\n");
        }
    } while (choice != 0);

    return 0;
}

void init_tracker(BudgetTracker *tracker) {
    tracker->monthly_budget = 0.0;
    tracker->income_count = 0;
    tracker->expense_count = 0;
}

const char *get_category_name(int category) {
    const char *names[NUM_CATEGORIES] = {
        "Food",
        "Transport",
        "Education",
        "Shopping",
        "Entertainment",
        "Other"
    };

    if (category >= 1 && category <= NUM_CATEGORIES) {
        return names[category - 1];
    }
    return "Unknown";
}

void clear_input_buffer(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
        /* Clear leftover characters from stdin */
    }
}

void read_line(char *buffer, int size) {
    if (fgets(buffer, size, stdin) != NULL) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
    }
}

void set_monthly_budget(BudgetTracker *tracker) {
    double budget;
    printf("\nEnter monthly budget limit (Rs.): ");
    while (scanf("%lf", &budget) != 1 || budget < 0) {
        printf("Invalid amount. Enter a non-negative budget: ");
        clear_input_buffer();
    }
    clear_input_buffer();

    tracker->monthly_budget = budget;
    printf("Monthly budget set to Rs. %.2f\n", tracker->monthly_budget);
    check_budget_alerts(tracker);
}

void add_income(BudgetTracker *tracker) {
    if (tracker->income_count >= MAX_INCOME) {
        printf("\nIncome record limit reached!\n");
        return;
    }

    Income new_income;
    printf("\nEnter income amount (Rs.): ");
    while (scanf("%lf", &new_income.amount) != 1 || new_income.amount <= 0) {
        printf("Invalid amount. Enter a positive number: ");
        clear_input_buffer();
    }
    clear_input_buffer();

    printf("Enter income source (e.g., Salary, Allowance): ");
    read_line(new_income.source, MAX_TEXT_LEN);

    tracker->incomes[tracker->income_count] = new_income;
    tracker->income_count++;

    printf("Income recorded! (Total entries: %d)\n", tracker->income_count);
}

void add_expense(BudgetTracker *tracker) {
    if (tracker->expense_count >= MAX_EXPENSES) {
        printf("\nExpense record limit reached!\n");
        return;
    }

    Expense exp;
    printf("\nEnter base expense amount (Rs.): ");
    while (scanf("%lf", &exp.base_amount) != 1 || exp.base_amount <= 0) {
        printf("Invalid amount. Enter a positive number: ");
        clear_input_buffer();
    }
    clear_input_buffer();

    printf("Enter tax rate percentage (0 if none): ");
    while (scanf("%lf", &exp.tax_rate) != 1 || exp.tax_rate < 0) {
        printf("Invalid tax rate. Enter 0 or a positive percentage: ");
        clear_input_buffer();
    }
    clear_input_buffer();

    exp.tax_amount = exp.base_amount * (exp.tax_rate / 100.0);
    exp.total_amount = exp.base_amount + exp.tax_amount;

    printf("Enter date (DD MM YYYY): ");
    while (scanf("%d %d %d", &exp.date.day, &exp.date.month, &exp.date.year) != 3 ||
           exp.date.day < 1 || exp.date.day > 31 ||
           exp.date.month < 1 || exp.date.month > 12 ||
           exp.date.year < 2000) {
        printf("Invalid date. Re-enter (DD MM YYYY): ");
        clear_input_buffer();
    }
    clear_input_buffer();

    printf("Select Category:\n");
    for (int i = 1; i <= NUM_CATEGORIES; i++) {
        printf("  %d. %s\n", i, get_category_name(i));
    }
    printf("Choice (1-%d): ", NUM_CATEGORIES);
    while (scanf("%d", &exp.category) != 1 || exp.category < 1 || exp.category > NUM_CATEGORIES) {
        printf("Invalid category. Choose 1-%d: ", NUM_CATEGORIES);
        clear_input_buffer();
    }
    clear_input_buffer();

    printf("Enter expense description: ");
    read_line(exp.description, MAX_TEXT_LEN);

    tracker->expenses[tracker->expense_count] = exp;
    tracker->expense_count++;

    printf("Expense recorded under %s! (Tax: Rs. %.2f | Total: Rs. %.2f)\n",
           get_category_name(exp.category), exp.tax_amount, exp.total_amount);

    check_budget_alerts(tracker);
}

double calculate_total_income(const BudgetTracker *tracker) {
    double sum = 0.0;
    for (int i = 0; i < tracker->income_count; i++) {
        sum += tracker->incomes[i].amount;
    }
    return sum;
}

double calculate_total_expenses(const BudgetTracker *tracker) {
    double sum = 0.0;
    for (int i = 0; i < tracker->expense_count; i++) {
        sum += tracker->expenses[i].total_amount;
    }
    return sum;
}

double calculate_total_tax(const BudgetTracker *tracker) {
    double sum = 0.0;
    for (int i = 0; i < tracker->expense_count; i++) {
        sum += tracker->expenses[i].tax_amount;
    }
    return sum;
}

double calculate_remaining_balance(const BudgetTracker *tracker) {
    return calculate_total_income(tracker) - calculate_total_expenses(tracker);
}

void calculate_category_totals(const BudgetTracker *tracker, double totals[NUM_CATEGORIES]) {
    for (int i = 0; i < NUM_CATEGORIES; i++) {
        totals[i] = 0.0;
    }
    for (int i = 0; i < tracker->expense_count; i++) {
        int cat = tracker->expenses[i].category;
        if (cat >= 1 && cat <= NUM_CATEGORIES) {
            totals[cat - 1] += tracker->expenses[i].total_amount;
        }
    }
}

void check_budget_alerts(const BudgetTracker *tracker) {
    if (tracker->monthly_budget <= 0.0) {
        return;
    }

    double spent = calculate_total_expenses(tracker);
    double budget = tracker->monthly_budget;
    double percent_used = (spent / budget) * 100.0;

    if (spent > budget) {
        printf("\nALERT!\n");
        printf("You have exceeded your monthly budget.\n");
        printf("Budget:       Rs. %.2f\n", budget);
        printf("Total Spent:  Rs. %.2f\n", spent);
        printf("Exceeded By:  Rs. %.2f\n", spent - budget);
    } else if (percent_used >= 80.0) {
        printf("\nWARNING!\n");
        printf("You have used %.0f%% of your monthly budget.\n", percent_used);
        printf("Remaining Budget: Rs. %.2f\n", budget - spent);
    }
}

void view_records(const BudgetTracker *tracker) {
    printf("\n========================================\n");
    printf("           FINANCIAL RECORDS            \n");
    printf("========================================\n");
    printf("Monthly Budget Limit: Rs. %.2f\n\n", tracker->monthly_budget);

    printf("--- INCOME RECORDS (%d) ---\n", tracker->income_count);
    if (tracker->income_count == 0) {
        printf("No income entries recorded.\n");
    } else {
        for (int i = 0; i < tracker->income_count; i++) {
            printf("%d. %-25s | Rs. %.2f\n",
                   i + 1,
                   tracker->incomes[i].source,
                   tracker->incomes[i].amount);
        }
    }

    printf("\n--- EXPENSE RECORDS (%d) ---\n", tracker->expense_count);
    if (tracker->expense_count == 0) {
        printf("No expense entries recorded.\n");
    } else {
        for (int i = 0; i < tracker->expense_count; i++) {
            const Expense *e = &tracker->expenses[i];
            printf("%d. [%02d/%02d/%04d] %-14s | %-20s | Base: Rs. %.2f | Tax: Rs. %.2f | Total: Rs. %.2f\n",
                   i + 1,
                   e->date.day, e->date.month, e->date.year,
                   get_category_name(e->category),
                   e->description,
                   e->base_amount,
                   e->tax_amount,
                   e->total_amount);
        }
    }
    printf("========================================\n");
}

void print_monthly_report(const BudgetTracker *tracker) {
    double total_income = calculate_total_income(tracker);
    double total_expenses = calculate_total_expenses(tracker);
    double total_tax = calculate_total_tax(tracker);
    double remaining_balance = calculate_remaining_balance(tracker);

    double cat_totals[NUM_CATEGORIES];
    calculate_category_totals(tracker, cat_totals);

    int highest_idx = 0;
    for (int i = 1; i < NUM_CATEGORIES; i++) {
        if (cat_totals[i] > cat_totals[highest_idx]) {
            highest_idx = i;
        }
    }

    printf("\n========================================\n");
    printf("          MONTHLY BUDGET REPORT         \n");
    printf("========================================\n\n");
    printf("Total Income:       Rs. %.2f\n", total_income);
    printf("Total Expenses:     Rs. %.2f\n", total_expenses);
    printf("Total Tax Paid:     Rs. %.2f\n", total_tax);
    printf("Remaining Balance:  Rs. %.2f\n\n", remaining_balance);
    printf("----------------------------------------\n");
    printf("EXPENSE SUMMARY\n");
    printf("----------------------------------------\n\n");

    for (int i = 0; i < NUM_CATEGORIES; i++) {
        printf("%-15s Rs. %.2f\n", get_category_name(i + 1), cat_totals[i]);
    }

    printf("\n----------------------------------------\n");
    printf("Highest Spending:\n");
    if (total_expenses > 0.0) {
        printf("%s - Rs. %.2f\n\n", get_category_name(highest_idx + 1), cat_totals[highest_idx]);
    } else {
        printf("None - Rs. 0.00\n\n");
    }

    printf("Budget Status:\n");
    if (tracker->monthly_budget > 0.0 && total_expenses > tracker->monthly_budget) {
        printf("EXCEEDED BUDGET (Limit: Rs. %.2f)\n\n", tracker->monthly_budget);
    } else {
        printf("WITHIN BUDGET\n\n");
    }

    printf("========================================\n");
    printf("             END OF REPORT              \n");
    printf("========================================\n");
}

int export_monthly_report(const BudgetTracker *tracker, const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        return 0;
    }

    double total_income = calculate_total_income(tracker);
    double total_expenses = calculate_total_expenses(tracker);
    double total_tax = calculate_total_tax(tracker);
    double remaining_balance = calculate_remaining_balance(tracker);

    double cat_totals[NUM_CATEGORIES];
    calculate_category_totals(tracker, cat_totals);

    int highest_idx = 0;
    for (int i = 1; i < NUM_CATEGORIES; i++) {
        if (cat_totals[i] > cat_totals[highest_idx]) {
            highest_idx = i;
        }
    }

    fprintf(fp, "========================================\n");
    fprintf(fp, "          MONTHLY BUDGET REPORT         \n");
    fprintf(fp, "========================================\n\n");
    fprintf(fp, "Total Income:       Rs. %.2f\n", total_income);
    fprintf(fp, "Total Expenses:     Rs. %.2f\n", total_expenses);
    fprintf(fp, "Total Tax Paid:     Rs. %.2f\n", total_tax);
    fprintf(fp, "Remaining Balance:  Rs. %.2f\n\n", remaining_balance);
    fprintf(fp, "----------------------------------------\n");
    fprintf(fp, "EXPENSE SUMMARY\n");
    fprintf(fp, "----------------------------------------\n\n");

    for (int i = 0; i < NUM_CATEGORIES; i++) {
        fprintf(fp, "%-15s Rs. %.2f\n", get_category_name(i + 1), cat_totals[i]);
    }

    fprintf(fp, "\n----------------------------------------\n");
    fprintf(fp, "Highest Spending:\n");
    if (total_expenses > 0.0) {
        fprintf(fp, "%s - Rs. %.2f\n\n", get_category_name(highest_idx + 1), cat_totals[highest_idx]);
    } else {
        fprintf(fp, "None - Rs. 0.00\n\n");
    }

    fprintf(fp, "Budget Status:\n");
    if (tracker->monthly_budget > 0.0 && total_expenses > tracker->monthly_budget) {
        fprintf(fp, "EXCEEDED BUDGET (Limit: Rs. %.2f)\n\n", tracker->monthly_budget);
    } else {
        fprintf(fp, "WITHIN BUDGET\n\n");
    }

    fprintf(fp, "========================================\n");
    fprintf(fp, "             END OF REPORT              \n");
    fprintf(fp, "========================================\n");

    fclose(fp);
    return 1;
}

int save_tracker_data(const BudgetTracker *tracker, const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        return 0;
    }

    fprintf(fp, "BUDGET %.2f\n", tracker->monthly_budget);
    fprintf(fp, "INCOMES %d\n", tracker->income_count);
    for (int i = 0; i < tracker->income_count; i++) {
        fprintf(fp, "%.2f|%s\n",
                tracker->incomes[i].amount,
                tracker->incomes[i].source);
    }

    fprintf(fp, "EXPENSES %d\n", tracker->expense_count);
    for (int i = 0; i < tracker->expense_count; i++) {
        const Expense *e = &tracker->expenses[i];
        fprintf(fp, "%.2f|%.2f|%.2f|%.2f|%d|%d|%d|%d|%s\n",
                e->base_amount,
                e->tax_rate,
                e->tax_amount,
                e->total_amount,
                e->date.day,
                e->date.month,
                e->date.year,
                e->category,
                e->description);
    }

    fclose(fp);
    return 1;
}

int load_tracker_data(BudgetTracker *tracker, const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        return 0;
    }

    char label[32];
    int count = 0;

    if (fscanf(fp, "%31s %lf", label, &tracker->monthly_budget) != 2) {
        fclose(fp);
        return 0;
    }

    tracker->income_count = 0;
    if (fscanf(fp, "%31s %d", label, &count) == 2) {
        int ch;
        while ((ch = fgetc(fp)) != '\n' && ch != EOF) {}
        for (int i = 0; i < count && i < MAX_INCOME; i++) {
            Income inc;
            if (fscanf(fp, "%lf|%99[^\n]", &inc.amount, inc.source) == 2) {
                tracker->incomes[tracker->income_count++] = inc;
            }
            while ((ch = fgetc(fp)) != '\n' && ch != EOF) {}
        }
    }

    tracker->expense_count = 0;
    if (fscanf(fp, "%31s %d", label, &count) == 2) {
        int ch;
        while ((ch = fgetc(fp)) != '\n' && ch != EOF) {}
        for (int i = 0; i < count && i < MAX_EXPENSES; i++) {
            Expense exp;
            if (fscanf(fp, "%lf|%lf|%lf|%lf|%d|%d|%d|%d|%99[^\n]",
                       &exp.base_amount,
                       &exp.tax_rate,
                       &exp.tax_amount,
                       &exp.total_amount,
                       &exp.date.day,
                       &exp.date.month,
                       &exp.date.year,
                       &exp.category,
                       exp.description) == 9) {
                tracker->expenses[tracker->expense_count++] = exp;
            }
            while ((ch = fgetc(fp)) != '\n' && ch != EOF) {}
        }
    }

    fclose(fp);
    return 1;
}