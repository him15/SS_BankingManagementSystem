struct trans_histroy
{
    int acc_no;
    char hist[1024];
};

struct FeedBack
{
    char feedback[1024];
};

struct Employee{
    int empID;
    char firstName[20];
    char lastName[20];
    char password[256];
    int role; // 0 -> Manager, 1 -> Employee     // changed
};

struct LoanDetails{
    int empID;
    int accountNumber;
    int loanID;
    int loanAmount;
    int status; // 0 -> requested, 1 -> pending, 2 -> approved, 3 -> rejected
};

struct Customer {
    int accountNumber;
    float balance;
    char customerName[20];
    char password[256];
    int activeStatus; // 0 -> deactivate, 1 -> activate

    // Write transcation history also
    // char transactionHistory[10][1024];
};

struct Counter{
    int count;
};