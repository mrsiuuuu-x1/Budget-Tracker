
# Budget Tracker (Programming Fundamentals — C)

A console-based **Budget Tracker** written in C for **Programming Fundamentals (BSCS-1D)** at the **National University of Computer & Emerging Sciences (Karachi Campus)**. It helps users manage a monthly budget, record income and categorized expenses, calculate taxes, receive spending threshold warnings/alerts, view stored records, generate a monthly summary report, and persist data using C file handling.

## Group Members

- **26k-0655** Abdul Rafay
- **26k-0667** Syed Azfar Ahmed

## Features

- **Monthly Budget Management:** Set and update a monthly spending budget limit.
- **Income Recording:** Add income entries with amount and source.
- **Expense Recording & Categories:** Record expenses with base amount, tax rate, date (`DD MM YYYY`), description, and one of six categories:
  - Food
  - Transport
  - Education
  - Shopping
  - Entertainment
  - Other
- **Tax Tracking & Totals:** Compute per-expense tax and total amount, total income, total expenses, total tax paid, remaining balance, and category-wise totals.
- **Budget Warning & Alert System:**
  - **WARNING!** when spending reaches at least **80%** of the monthly budget (shows percentage used and remaining budget).
  - **ALERT!** when spending exceeds **100%** of the monthly budget (shows budget, total spent, and exceeded amount).
- **Financial Records Viewer & Monthly Summary Report:** View all recorded entries or display a formatted **Monthly Budget Report** with category totals, highest spending category, and budget status.
- **File Persistence & Report Export:** Automatically saves and reloads records via `records.txt` and exports the formatted summary to `monthly_report.txt`.

## Project Structure

```text
budget-tracker/
├── budget.h              # Constants, structs (Date, Income, Expense, BudgetTracker),
│                         # and function prototypes
├── main.c                # Menu loop, input helpers, calculations, alerts,
│                         # reports, and file I/O
├── README.md             # Project overview and usage guide
├── records.txt           # Auto-generated saved budget, income, and expense data
└── monthly_report.txt    # Generated when exporting the monthly report (Menu Option 6)
```

## How to Compile and Run

Open a terminal in the `budget-tracker/` folder and compile with `gcc`.

### Step 1: Compile

```bash
gcc -Wall -Wextra main.c -o budget_tracker.exe
```

### Step 2: Run

```powershell
.\budget_tracker.exe
```

## Menu Options

1. **Set Monthly Budget** — Set or update the monthly spending limit.
2. **Add Income** — Record a new income entry and source.
3. **Add Expense** — Record an expense with tax rate, date, category, and description, then check budget alerts.
4. **View Financial Records** — Display the budget limit along with all saved income and expense entries.
5. **Monthly Summary Report** — Print the full monthly financial summary in the console.
6. **Save Data & Export Report** — Save records to `records.txt` and export the report to `monthly_report.txt`.
7. **Save & Exit** — Save records and exit the program.
