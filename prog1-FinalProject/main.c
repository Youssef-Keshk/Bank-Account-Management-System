#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <time.h>

typedef struct
{
    char month[3];
    char year[5];
} date_opened;

typedef struct
{
    char acc_num[11];
    char name[25];
    char mobile[12];
    char email[30];
    char balance[11];
    date_opened date;
} account;

char username[25];
char password[20];

account *Accounts;

//number of accounts in (accounts.txt)
int NUM_ofaccounts;

//used in withdraw,deposit,transfer,add_account and delete
int indexofaccount;
int indexofreceiver;

//helping functions
int initial_Menu();
int num_validation(char*);
int name_validation(char*);
char* month_Name(account*);
int email_validation(char*);
void edit_fileforaccount(double, int);
void sortbyname(account* );
void sortbybalance(account*);
void sortbydate(account* );

//main functions
int LOGIN();
void LOAD();
void Search();
void advanced_Search();
void addAccount();
void MODIFY();
void WITHDRAW();
void DEPOSIT();
void deleteAccount();
int save_Data();
void TRANSFER();
void REPORT();
void PRINT();
void MENU(int);
void QUIT();



int main()
{
    if (initial_Menu())
    {
        system("cls");

        MENU(1);

        free(Accounts);
    }

    else
        QUIT();

    return 0;
}

//Login or Quit
int initial_Menu()
{
    int flag = 0;
    char v[20];
    do
    {
        printf("Enter L to LOGIN\nEnter Q to QUIT\n");
        scanf("%s", v);

        int len = strlen(v);

        if(len == 1 && toupper(*v) == 'Q')
            return 0;

        else if (len == 1 && toupper(*v) == 'L')
            flag = LOGIN();

        getchar();

    }
    while(!flag);
    return 1;
}

//part(1)validate username and password
int LOGIN()
{
    FILE *ptr;
    ptr = fopen("accounts/users.txt", "r");
    char hold_username[15];
    char hold_password[15];

    if(ptr != NULL)
    {
        printf("Username: ");
        scanf("%s", username);
        printf("Password: ");
        scanf("%s", password);


        while(!feof(ptr))
        {
            fscanf(ptr, " %s ", hold_username);
            fscanf(ptr, " %s ", hold_password);

            if(!strcmp(hold_username, username) && !strcmp(hold_password, password))
            {
                fclose(ptr);
                return 1;
            }

        }
        printf("INvalid username and password\n");
        fclose(ptr);
    }
    else
        printf("error opening users file!\n");


    return 0;
}

//part(2)load data from accounts.txt into struct
void LOAD()
{
    char holdstring[100];

    NUM_ofaccounts = 0;
    int j = 0;
    FILE *load_account;
    load_account = fopen("accounts/accounts.txt", "r");

    if(load_account != NULL)
    {

        while (!feof(load_account))
        {
            if(fgetc(load_account) == '\n')
                NUM_ofaccounts++;
        }
        NUM_ofaccounts++; //adding last account in file

        rewind(load_account);//resets pointer position to start of file

        Accounts = malloc(NUM_ofaccounts * sizeof(account));//dynamically allocating a specific number of bytes of memory


        for(int i = 0; i < NUM_ofaccounts; i++)
        {
            fgets(holdstring, 99, load_account);//reads form accounts from file line by line

            char *tok = strtok(holdstring, ",");// , is the delimiter for separating tokens
            strcpy(Accounts[i].acc_num, tok);

            tok = strtok(NULL, ",");// NUL used to continue tokenizing the same string
            strcpy(Accounts[i].name, tok);

            tok = strtok(NULL, ",");
            strcpy(Accounts[i].email, tok);

            tok = strtok(NULL, ",");
            strcpy(Accounts[i].balance, tok);

            tok = strtok(NULL, ",");
            strcpy(Accounts[i].mobile, tok);

            tok = strtok(NULL, "-");
            strcpy(Accounts[i].date.month, tok);

            tok = strtok(NULL, "\n");
            strcpy(Accounts[i].date.year, tok);
        }

    }
    else printf("error in loading\n");

    fclose(load_account);
}

//part(3)Searching for account number and printing account details
void Search()
{
    char num[15];

    int flag;
    do
    {
        printf("Enter account number: ");
        scanf("%s", num);
        getchar();
        if(strlen(num) != 10)
        {
            printf("INvalid account number\n");
            flag = 1;
        }
        else flag = 0;

    }
    while(flag);


    flag = num_validation(num);//returns account index or -1 if account is not found

    if(flag == -1)
        printf("Account is not found!!");

    else
    {
        system("cls");
        printf("\nAccount Number: %s\n", Accounts[flag].acc_num);
        printf("Name: %s\n", Accounts[flag].name);
        printf("E-mail: %s\n", Accounts[flag].email);
        printf("Balance: %s\n", Accounts[flag].balance);
        printf("Mobile: %s\n", Accounts[flag].mobile);
        printf("Date Opened: %s %s\n", month_Name(&Accounts[flag]), Accounts[flag].date.year);


    }

}

//validating account-number and returning its index if valid
int num_validation(char *x)
{
    int flag;
    for(int i = 0; i < NUM_ofaccounts; i++)
    {
        flag = strcmp(x, Accounts[i].acc_num);
        if(flag == 0)
            return i;
    }
    return -1;
}

//validating account name and converting each letter to the right case
int name_validation(char *x)
{
    int space = 0;

    for(int i = 0; i < strlen(x); i++)
    {
        if( x[i] == ' ')
        {
            space++;
            if( isalpha( x[i + 1]) )
            {
                x[i + 1] = toupper( x[i + 1] );
                i++;
            }
        }
        //check that all characters are letters
        else if( !isalpha(x[i]) )
        {
            printf("Error account name should consist of only letters\n");
            return 0;
        }
        else
            x[i] = tolower(x[i]);
     }

     x[0] = toupper(x[0]);

    if (space == 1)
    {
        return 1;
    }
    else
    {
        printf ("Account name should consist of 2 names\n");
        return 0;
    }
}

//returns month as a name
char* month_Name(account *m)
{
    if (atoi(m->date.month)  == 1)
        return "January";
    else if (atoi(m->date.month) == 2)
        return "February";
    else if (atoi(m->date.month)== 3)
        return "March";
    else if (atoi(m->date.month)  == 4 )
        return "April";
    else if (atoi(m->date.month)  == 5)
        return "May";
    else if (atoi(m->date.month)  == 6)
        return "June";
    else if (atoi(m->date.month)  == 7)
        return "July";
    else if (atoi(m->date.month)  == 8)
        return "August";
    else if (atoi(m->date.month)  == 9)
        return "September";
    else if (atoi(m->date.month)  == 10)
        return "October";
    else if (atoi(m->date.month)  == 11)
        return "November";
    else if (atoi(m->date.month)  == 12)
        return "December";
}

//part(4)Searching for account-name containing a certain keyword and printing account details
void advanced_Search()
{
    char keyword[20];
    int flag = 0;

    printf("Enter Keyword: ");
    scanf("%s", keyword);
    getchar();

    int key_len = strlen(keyword);
    char keyword_lowercase[20];
    char name_lowercase[20];

    for (int j = 0; j < key_len; j++)
    {
        keyword_lowercase[j] = tolower(keyword[j]);
    }
    keyword_lowercase[key_len] = '\0';


    for(int i = 0; i < NUM_ofaccounts; i++)
    {


        //handling lower and upper cases
        for (int j = 0; j < strlen(Accounts[i].name); j++)
        {
            name_lowercase[j] = tolower(Accounts[i].name[j]);
        }
        name_lowercase[strlen(Accounts[i].name)] = '\0';



//strstr function returns pointer to the first occurrence of keyword_lowercase in name_lowercase and returns NULL is not found.
        if (strstr(name_lowercase, keyword_lowercase) != NULL)
        {
            printf("\nAccount Number: %s\n", Accounts[i].acc_num);
            printf("Name: %s\n", Accounts[i].name);
            printf("E-mail: %s\n", Accounts[i].email);
            printf("Balance: %s\n", Accounts[i].balance);
            printf("Mobile: %s\n", Accounts[i].mobile);
            printf("Date Opened: %s %s\n\n", month_Name(&Accounts[i]), Accounts[i].date.year);
            flag = 1;

        }
    }
    if(flag == 0)
        printf("No matches are found");

}

// part(5)Function to add a new bank account
void addAccount()
{

    int flag;
    account newaccount;

    do
     {

        // Get user input for account details
        do
        {
            flag = 0;
            printf("Enter new account number: ");
            scanf("%s", newaccount.acc_num);
            getchar();


            //Validate account number length of 10 digits and all of them are digits
            if(strlen(newaccount.acc_num) != 10 || newaccount.acc_num[0]=='0')
            {
                flag = 1;
                printf("Error: Account number should consist of 10 digits.\n");

            }
            else
                for(int i = 0; i < 10; i++)
                    if(newaccount.acc_num[i] < '0' || newaccount.acc_num[i]  > '9')
                    {
                        flag = 1;
                        printf("Error: Account number should consist of 10 digits.\n");

                        break;
                    }

        }
        while(flag);


        // Check if the account number is unique
        if (num_validation(newaccount.acc_num) != -1)
        {
            printf("Error: Account number already exists.\n");
            flag = 1;

        }



    }
    while(flag);


    printf("Enter account holder's name: ");
    gets(newaccount.name);
    while(!name_validation(newaccount.name))
    {
        printf("Enter account holder's name: ");
        gets(newaccount.name);
    }
    do
    {
        flag = 0;
        printf("Enter mobile number: ");
        scanf("%s", newaccount.mobile);

        if(strlen(newaccount.mobile) != 11)
        {
            printf("INvalid mobile number.\n");
            flag = 1;
        }

    }
    while(flag);

    do
    {
        printf("Enter e-mail address: ");
        scanf("%s", newaccount.email);

        flag = email_validation(newaccount.email);

        if(!flag)
            printf("INvalid e-mail address.\n");

    }
    while(!flag);

    //new accounts's balance is automatically set to 0
    strcpy(newaccount.balance,"0.00");
    getchar();
    char SoD;
    do
    {
        printf("Enter (S) to save the changes and add the account or (D) to discard the changes: ");

        scanf("%c", &SoD);
        getchar();

        if(toupper(SoD) == 'S')
        {
            NUM_ofaccounts ++;
            Accounts = realloc(Accounts, NUM_ofaccounts * sizeof(account));//resizing the memory block previously allocated

            Accounts[NUM_ofaccounts - 1]= newaccount;


            // Get the system's current date
            time_t t = time(NULL);
            struct tm tm = *localtime(&t);

            sprintf(Accounts[NUM_ofaccounts - 1].date.month, "%02d", tm.tm_mon + 1);
            sprintf(Accounts[NUM_ofaccounts - 1].date.year, "%d", tm.tm_year + 1900);


            // Update the "accounts.txt" file
            if(save_Data())
            {
                printf("Account successfully added!\n");

                indexofaccount = NUM_ofaccounts - 1;
                edit_fileforaccount(0, 0);
            }

        }
        else if(toupper(SoD) == 'D')
            printf("Changes are canceled");

    }
    while(toupper(SoD) != 'S' && toupper(SoD) != 'D');


}

//validating email address entered by user
int email_validation(char* email)
{
    //check that the first char is not dot and last char is not @
    if(email[0] == '.' || email[strlen(email) - 1] == '@')
        return 0;


    int atcount = 0;
    int atindex;

    int dotcount = 0;
    int dotflag = 0;
    int dotindex;

    //counting no# of @s and no# of dots
    for(int i = 0; i < strlen(email); i++)
    {
        if(email[i] == '@')
        {
            atcount++;
            dotflag = 0;
            atindex = i;
        }
        else if(email[i] == '.')
        {
            dotcount++;
            dotindex = i;

            //email can not contain two consecutive dots
            if(dotflag == 1)
                return 0;

            dotflag = 1;
        }
    }

    //check email has exactly one @ , at least one dot and a dot after the @
    if(dotcount < 1 || atcount != 1 || atindex > dotindex)
        return 0;

    else return 1;
}

//part(6)Function to delete an account from accounts.txt
void deleteAccount()
{
    char AccNum[13];
    int index;

    do
    {

        printf("Enter the bank account number to be deleted: ");
        scanf("%s", AccNum);
        getchar();

        index = num_validation(AccNum);
        if(index == -1)
                 printf("Error: Account not found.\n");


    }
    while(index == -1);

    //(atof)converts the string into float
    if (atof(Accounts[index].balance) > 0)
        {
            printf("Deletion failed. Account has a balance greater than zero.\n");
             return;
        }



    char SoD;

    do
    {
        //save or discard
        printf("Enter (S) to save changes and delete account or (D) to discard the changes: ");
        scanf(" %c", &SoD);
        getchar();


        // Delete account
        if(toupper(SoD) == 'S')
        {
                indexofaccount = index;
                edit_fileforaccount(0, -1);

                //(memset) used to fill the memory with value '\0' to delete it
                memset(&Accounts[index], '\0', sizeof(account));
                NUM_ofaccounts--;

                //adjusting the account variable Accounts
                for(int i = index; i < NUM_ofaccounts; i++)
                {
                    Accounts[i]= Accounts[i + 1];
                }
                Accounts = realloc(Accounts, NUM_ofaccounts * sizeof(account));

                if(save_Data())
                    printf("Account deleted successfully.\n");
        }
        else if(toupper(SoD) == 'D')
            printf("Deletion is canceled\n");

    }
    while(toupper(SoD) != 'S' && toupper(SoD) != 'D');
}

//part(7) modify
void MODIFY()
{

    char num[15];
    char hold_char;
    int index;


    do
    {
        printf("Enter account number: ");
        scanf("%s", num);
        getchar();

        index = num_validation(num);
        if( index == -1)
          printf("Account is not found!!\n");

    }
    while(index == -1);

    int type;
    int flag;

    do
    {
        printf("Enter (1) to modify name\nEnter (2) to modify mobile number\nEnter (3) to modify email address\n");
        scanf ("%d", &type);
         getchar();

        while (type > 3 || type <  1 )
        {
            printf ("Error please enter correct number:");
            scanf ("%d", &type);
            getchar();
        }



        if (type == 1)
        {

            char newname[25];
            do
            {
                printf("Enter the new name: ");

                 gets(newname);

            }while(!name_validation(newname));

            //save or discard
            char SoD;
            do
            {
                printf("Enter (S) to save changes and confirm transactions or (D) to discard the changes: ");
                scanf(" %c", &SoD);
                getchar();


                if(toupper(SoD) == 'S')
                {
                    strcpy(Accounts[index].name, newname);

                    flag = save_Data();

                    if(flag)
                        printf("Modification was done successfully!\n");
                }

                else if(toupper(SoD) == 'D')
                    printf("Modification is canceled\n");

            }
            while(toupper(SoD) != 'S' && toupper(SoD) != 'D');

        }
        else if (type == 2)
        {
            char newmobile[12];
            do
            {
            printf("Enter the new mobile number: ");
            scanf("%s", newmobile);
            getchar();
            if (strlen(newmobile)!=11)
                printf("Error mobile number should consist of 11 numbers\n");
            }while (strlen(newmobile)!=11);
            //save or discard
            char SoD;
            do
            {
                printf("Enter (S) to save changes and confirm transactions or (D) to discard the changes: ");
                scanf(" %c", &SoD);
                getchar();


                if(toupper(SoD) == 'S')
                {
                    strcpy(Accounts[index].mobile, newmobile);

                    flag = save_Data();

                    if(flag)
                        printf("Modification was done successfully!\n");
                }
                else if(toupper(SoD) == 'D')
                    printf("Modification is canceled\n");

            }
            while(toupper(SoD) != 'S' && toupper(SoD) != 'D');

        }

        else if (type == 3)
        {
            char newemail[30];
            do
            {
                printf("Enter the new email address: ");
                scanf("%s", newemail);
                getchar();

                flag = email_validation(newemail);

                if(!flag)
                    printf("INvalid e-mail address!\n");

            }
            while(!flag);

            //save or discard
            char SoD;
            do
            {
                printf("Enter (S) to save changes and confirm transactions or (D) to discard the changes: ");
                scanf(" %c", &SoD);
                getchar();


                if(toupper(SoD) == 'S')
                {

                    strcpy(Accounts[index].email, newemail);

                    flag = save_Data();

                    if(flag)
                        printf("Modification was done successfully!\n");

                }
                else if(toupper(SoD) == 'D')
                    printf("Modification is canceled\n");

            }
            while(toupper(SoD) != 'S' && toupper(SoD) != 'D');
        }

        do
        {
            printf("Do you want to modify any thing else?\nEnter (Y) for yes or (N) for no: ");
            scanf("%c", &hold_char);

        }
        while(toupper(hold_char) != 'Y' && toupper(hold_char) != 'N');

    }
    while (toupper(hold_char) == 'Y');

}

//part(8)Whithdraw
void WITHDRAW()
{
    char num[15];
    int index;


    //user is prompted for account number
    do
    {
        printf("Enter account number: ");
        scanf("%s", num);
        getchar();

        //num_validation function returns the (index) if number is found and returns (-1) if number is not found
        index = num_validation(num);

        //validate account number is found
        if(index == -1)
          printf("Account is not found!\n");


    }
    while (index == -1);


    double amount;

    //user is prompted for the amount needs to be withdrawn from that account
 int flag = 0;

    do
    {
        flag = 0;
        printf("Enter an amount to be withdrawn from that account: \n");
        scanf("%lf", &amount);
        getchar();

        if(amount > 10000)
        {
            printf("Error: maximum withdrawal limit is 10,000$ per transaction\n");
            flag = 1;
        }


    }
    while(flag);

    //make sure account has enough balance for the withdrawal
    if(amount > atof(Accounts[index].balance))
    {
        printf("Insufficient funds. Your account balance is below the withdrawal amount.\n");
        return;
    }


    double newbalance;

    char SoD;
    do
    {
        //save or discard
        printf("Enter (S) to save changes and confirm transactions or (D) to discard the changes: ");
        scanf(" %c", &SoD);
        getchar();


        if(toupper(SoD) == 'S')
        {

            //atof() function converts string into float
            newbalance = atof(Accounts[index].balance) - amount;


            //calculate number of digits in newbalance
            double hold_newbalance = newbalance;
            int no_ofdigits_innewbalance;

            for(no_ofdigits_innewbalance = 0; floor(hold_newbalance) > 1; no_ofdigits_innewbalance++)
                hold_newbalance /= 10;

            //gctv() function converts float into string
            gcvt(newbalance, no_ofdigits_innewbalance + 2, Accounts[index].balance );

            flag = save_Data();


            if(flag)
            {
                printf("Transaction was done successfully!\n");

                indexofaccount = index;

                edit_fileforaccount(amount, 1);
            }
        }
        else if(toupper(SoD) == 'D')
            printf("Transaction is canceled\n");

    }
    while(toupper(SoD) != 'S' && toupper(SoD) != 'D');

}

//part(9)Deposit
void DEPOSIT()
{
    char num[15];
    int index;

    //user is prompted for account number
    do
    {
        printf("Enter account number: ");
        scanf("%s", num);
        getchar();

        //num_validation function returns the (index) if number is found and returns (-1) if number is not found
        index = num_validation(num);

        //validate account number is found
        if(index == -1)
          printf("Account is not found!\n");


    }
    while (index == -1);


    double amount;

    //user is prompted for the amount needs to be deposited from that account
    int flag;
    do
    {
        flag = 0;
        printf("Enter an amount to be deposited to that account: \n");
        scanf("%lf", &amount);
        getchar();

        if(amount > 10000)
        {
            printf("Error: maximum deposit limit is 10,000$ per transaction\n");
            flag = 1;
        }

    }
    while(flag);

    double newbalance;

    char SoD;
    do
    {
        //save or discard
        printf("Enter (S) to save changes and confirm transactions or (D) to discard the changes: ");
        scanf(" %c", &SoD);
        getchar();


        if(toupper(SoD) == 'S')
        {
            //atof() function converts string into float
            newbalance = atof(Accounts[index].balance) + amount;


            //calculate number of digits in newbalance
            double hold_newbalance = newbalance;
            int no_ofdigits_innewbalance;

            for(no_ofdigits_innewbalance = 0; floor(hold_newbalance) > 1; no_ofdigits_innewbalance++)
                hold_newbalance /= 10;


            //gctv() function converts float into string
            gcvt(newbalance, no_ofdigits_innewbalance + 2, Accounts[index].balance );

            flag = save_Data();

            if(flag)
            {
                printf("Transaction was done successfully!\n");

                indexofaccount = index;

                edit_fileforaccount(amount, 2);
            }
        }
        else if(toupper(SoD) == 'D')
            printf("Transaction is canceled\n");

    }
    while(toupper(SoD) != 'S' && toupper(SoD) != 'D');

}

//part(10)Transfer
void TRANSFER()
{

    int flag;

    char numsofsender[15];
    int index_of_sender;

    char numofreceiver[15];
    int index_of_receiver;

    //user is prompted for sender account number
    do
    {
        printf("Enter the account number of the SENDER: ");
        scanf("%s", numsofsender);
        getchar();

        //num_validation function returns the (index) if number is found and returns (-1) if number is not found
        index_of_sender = num_validation(numsofsender);

        //validate sender account number is found
        if (index_of_sender == -1)
           printf("Account is not found!\n");

    }
    while (index_of_sender == -1);


    //user is prompted for receiver account number
    do
    {
        flag = 0;
        printf("Enter the account number of the RECEIVER: ");
        scanf("%s", numofreceiver);
        getchar();

        //num_validation function returns the (index) if number is found and returns (-1) if number is not found
        index_of_receiver = num_validation(numofreceiver);

        //validate account number is found
        if (index_of_receiver == -1)
        {
            printf("Account is not found!\n");
            flag = 1;
        }
        else if (index_of_receiver == index_of_sender)
        {
            printf("Can not use the same account number\n");
            flag = 1;
        }


    }
    while (flag);

    //user is prompted for the amount needs to be transfered from that account
    double amount;

    do
    {
        flag = 0;
        printf("Enter an amount to be transfered from %s to %s: ", Accounts[index_of_sender].name, Accounts[index_of_receiver].name);
        scanf("%lf", &amount);
        getchar();

        if (amount > 10000)
        {
            printf("Error: maximum transfer limit is 10,000$ per transaction\n");
            flag = 1;
        }

    }
    while (flag);

    //make sure account has enough balance for the withdrawal
    if(amount > atof(Accounts[index_of_sender].balance))
    {
        printf("Insufficient funds. Your account balance is below the tranfering amount.\n");
        return;
    }

    char SoD;
    do
    {
        printf("Enter (S) to save changes and confirm transactions or (D) to discard the changes: ");
        scanf(" %c", &SoD);
        getchar();

        if(toupper(SoD) == 'S')
        {

            double newbalanceofsender;
            //atof() function converts string into float
            newbalanceofsender = atof(Accounts[index_of_sender].balance) - amount;


            //calculate number of digits in newbalanceofsender
            double hold_newbalance1 = newbalanceofsender;
            int no_ofdigits_innewbalance;

            for (no_ofdigits_innewbalance = 0; floor(hold_newbalance1) > 1; no_ofdigits_innewbalance++)
                hold_newbalance1 /= 10;

            //gctv() function converts float into string
            gcvt(newbalanceofsender, no_ofdigits_innewbalance + 2, Accounts[index_of_sender].balance);


            double newbalanceofreciver;
            //atof() function converts string into float
            newbalanceofreciver = atof(Accounts[index_of_receiver].balance) + amount;


            //calculate number of digits in newbalanceofreciver
            double hold_newbalance2 = newbalanceofreciver;
            int no_ofdigits_innewbalance1;

            for (no_ofdigits_innewbalance1 = 0; floor(hold_newbalance2) > 1; no_ofdigits_innewbalance1++)
                hold_newbalance2 /= 10;


            //gctv() function converts float into string
            gcvt(newbalanceofreciver, no_ofdigits_innewbalance1 + 2, Accounts[index_of_receiver].balance);


            flag = save_Data();

            if (flag)
            {
                printf("Transaction was done successfully!\n");

                indexofaccount = index_of_sender;
                indexofreceiver = index_of_receiver;

                edit_fileforaccount(amount, 3);
            }

        }
        else if (toupper(SoD) == 'D')
            printf("Transaction is canceled\n");

    }
    while(toupper(SoD) != 'S' && toupper(SoD) != 'D');

}

//part(11)Report
void REPORT()
{

    char num[15];
    char line[100];
    int index;

    do
    {
        printf("Enter account number: ");
        scanf("%s", num);
        getchar();

        index = num_validation(num);

        if(index == -1)
           printf("Error: Account not found.\n");


    }
    while (index == -1);

    char string[15];
     sprintf(string, "%s.txt", Accounts[index].acc_num);

    FILE *ACCOUNT;

    ACCOUNT = fopen(string, "r");

    if(ACCOUNT != NULL)
    {
        if(fgetc(ACCOUNT) != EOF)
        {
            rewind(ACCOUNT);
            int n = 1;

            while (!feof(ACCOUNT))
            {
                if(fgetc(ACCOUNT) == '\n')
                    n++;
            }

            rewind(ACCOUNT);

            if(n <= 5)
            {
                printf("The %d transactions made on this account are:\n", n);
                while (!feof(ACCOUNT))
                {
                    fgets(line, 199, ACCOUNT);

                    printf("%s", line);
                }
            }
            else
            {
                printf("The last 5 transactions made on this account are:\n");
                for(int i = 0; i < n; i++)
                {
                    fgets(line, 99, ACCOUNT);
                    if(i >= n - 5)
                        printf("%s", line);
                }
            }

            fclose(ACCOUNT);

        }
        else
            printf("NO transactions were made on this account.\n");
    }
    else printf("Error: File not found!\n");

}

//appending accnumber.txt file
void edit_fileforaccount(double amount, int type_oftransaction)
{

    char string[15];
    sprintf(string, "transaction history/%s.txt", Accounts[indexofaccount].acc_num);


    //called from withdraw function
    if (type_oftransaction == 1)
    {
        FILE *ACCOUNT = fopen(string, "a");
        fseek(ACCOUNT, 0, SEEK_END);

        //ftell function do determine whether file is empty or not
        if(ftell(ACCOUNT) == 0)
            fprintf(ACCOUNT, "Withdraw  by %.2lf$", amount);

        else
            fprintf(ACCOUNT, "\nWithdraw  by %.2lf$", amount);
        fclose(ACCOUNT);
    }

    //called from deposit function
    else if (type_oftransaction == 2)
    {
        FILE *ACCOUNT = fopen(string, "a");
        fseek(ACCOUNT, 0, SEEK_END);

        if(ftell(ACCOUNT) == 0)
            fprintf(ACCOUNT, "Deposit  by %.2lf$", amount);
        else
            fprintf(ACCOUNT, "\nDeposit  by %.2lf$", amount);
        fclose(ACCOUNT);
    }

    //called from transfer function
    else if (type_oftransaction == 3)
    {
        FILE *ACCOUNT_ofsender = fopen(string, "a");
        fseek(ACCOUNT_ofsender, 0, SEEK_END);

        if(ftell(ACCOUNT_ofsender) == 0)
            fprintf(ACCOUNT_ofsender, "Transfer  from account by %.2lf$", amount);
        else
            fprintf(ACCOUNT_ofsender, "\nTransfer  from account by %.2lf$", amount);

        fclose(ACCOUNT_ofsender);

        sprintf(string, "transaction history/%s.txt", Accounts[indexofreceiver].acc_num);


        FILE *ACCOUNT_ofreceiver = fopen(string, "a");

        fseek(ACCOUNT_ofreceiver, 0, SEEK_END);

        if(ftell(ACCOUNT_ofreceiver) == 0)
            fprintf(ACCOUNT_ofreceiver, "Transfer to account by %.2lf$", amount);
        else
            fprintf(ACCOUNT_ofreceiver, "\nTransfer to account by %.2lf$", amount);
        fclose(ACCOUNT_ofreceiver);
    }

    //called from addAccount function
    else if(type_oftransaction == 0)
    {
        //creating a file for new account
        FILE *ACCOUNT = fopen(string, "w");
        fclose(ACCOUNT);
    }

    //called from deleteAccount function
    else if(type_oftransaction == -1)
    {
        //deleting the file of the deleted account
        if(remove(string) != 0)
            printf("No history of transactions\n");
    }

}

//part(12)printing accounts in a sorted way
void PRINT()
{
    account *sort = malloc(NUM_ofaccounts * sizeof(account));
    int num;

    printf("Do you want the data sorted by:\n1.Name(enter 1)\n2.Balance(enter 2)\n3.Date(enter 3)\n");
    scanf("%d",&num);
    getchar();
    while (num > 3 || num < 1)
    {
        printf("\nINvalid please enter correct number: ");
        scanf("%d", &num);
    }

    if (num == 1)
        sortbyname(sort);

    if (num == 2)
        sortbybalance(sort);

    if (num == 3)
        sortbydate(sort);

    system("cls");

    //printing Accounts after sorting
    for (int i = 0; i < NUM_ofaccounts; i++)
    {
        printf("\nAccount Number: %s\n", sort[i].acc_num);
        printf("Name: %s\n", sort[i].name);
        printf("E-mail: %s\n", sort[i].email);
        printf("Balance: %s\n", sort[i].balance);
        printf("Mobile: %s\n", sort[i].mobile);
        printf("Date Opened: %s %s\n", month_Name(&sort[i]), sort[i].date.year);
    }

    free(sort);
}

//part(12.1)
void sortbyname(account * sort)
{

    // Copy accounts to sort array
    for (int i = 0; i < NUM_ofaccounts; i++)
        sort[i] = Accounts[i];



    account temp;
    int flag = 0;

    //bubble sort algorithm is used
    for (int pass = 1; pass < NUM_ofaccounts && !flag; pass++)
        {
            flag = 1;
            for (int j = 0; j < NUM_ofaccounts - pass; j++)
            {
                if (strcmp(sort[j].name, sort[j + 1].name) > 0 )
                    {

                    temp = sort[j];
                    sort[j] = sort[j + 1];
                    sort[j + 1] = temp;
                    flag = 0;
                    }
            }
        }


}

//part(12.2)
void sortbybalance(account *sort)
{

    // Copy accounts to sort array
    for (int i = 0; i < NUM_ofaccounts; i++)
    {
        sort[i]= Accounts[i];
    }

    int flag = 0;
    account temp;


    //bubble sort algorithm is used

        for (int pass = 1; pass < NUM_ofaccounts && !flag; pass++)
        {
            flag = 1;
            for (int j = 0; j < NUM_ofaccounts - pass; j++)
            {
                if (atoi(sort[j].balance) < atoi(sort[j + 1].balance))
                {
                    temp = sort[j];
                    sort[j] = sort[j + 1];
                    sort[j + 1] = temp;
                    flag = 0;

                }
            }
        }



}

//part(12.3)
void sortbydate(account *sort )
{


    // Copy accounts to sort array
    for (int i = 0; i < NUM_ofaccounts; i++)
    {
        sort[i]= Accounts[i];
    }

    int flag = 0;
    account temp;

    for (int pass = 1; pass < NUM_ofaccounts && !flag; pass++)
        {
            flag = 1;
              for (int j = 0; j < NUM_ofaccounts - pass; j++)
            {

                if (atoi(sort[j].date.year) < atoi(sort[j + 1].date.year))
                {
                    temp = sort[j];
                    sort[j] = sort[j + 1];
                    sort[j + 1] = temp;
                    flag = 0;
                }
                else if (atoi(sort[j].date.year) == atoi(sort[j + 1].date.year))
                {
                    if (atoi(sort[j].date.month) < atoi(sort[j + 1].date.month))
                        {
                        temp = sort[j];
                        sort[j] = sort[j + 1];
                        sort[j + 1] = temp;
                        flag = 0;
                        }
                }
            }
        }

}


//part(13)save
int save_Data()
{
    FILE *saving;
    saving = fopen("accounts/accounts.txt", "w");

    if(saving != NULL)
    {
        for (int i = 0; i < NUM_ofaccounts; i++)
        {
            fprintf(saving,"%s,%s,%s,%s,%s,%s-%s",Accounts[i].acc_num, Accounts[i].name, Accounts[i].email,
                    Accounts[i].balance, Accounts[i].mobile,Accounts[i].date.month, Accounts[i].date.year);
            if(i != NUM_ofaccounts - 1)
                fprintf(saving, "\n");
        }

        fclose(saving);
        return 1;
    }
    else
    {
        printf("Error in saving.File is not found\n");
        fclose(saving);

        return 0;
    }
}

//part(14)Quit
void QUIT()
{
    system("cls");
    printf("Program closed. Have a great day!");
    return;
}

//part(15)Menu
void MENU(int p)
{

    //the number 1 indicates that it's the first time to call menu to prevent calling load each time menu is called
    if(p == 1)
        LOAD();
    int x;

    printf("1- ADD\n2- DELETE\n3- MODIFY\n4- SEARCH\n5- ADVANCED SEARCH\n6- WITHDRAW\n7- DEPOSIT\n8- TRANSFER\n9- REPORT\n10-PRINT\n11-LogOut\n");

    do
    {
        printf("Choose a function from 1 to 11: ");
        scanf("%d", &x);

    }
    while(x < 1 || x > 11);

    system("cls");
    if(x == 1) addAccount();
    else if(x == 2)  deleteAccount();
    else if(x == 3)  MODIFY();
    else if(x == 4)  Search();
    else if(x == 5) advanced_Search();
    else if(x == 6)  WITHDRAW();
    else if(x == 7)  DEPOSIT();
    else if(x == 8) TRANSFER();
    else if(x == 9) REPORT();
    else if(x == 10) PRINT();
    else if(x == 11)//logout
    {
        int flag;
        getchar();
        flag = initial_Menu();
        if(flag == 1)
        {
            system("cls");
            MENU(0);
        }
        else
         {
                QUIT();
               return;
         }
    }




    if(x != 11)
    {
        char m[20];
        printf("\nPress any key then enter to return to the menu.");
        scanf("%s", m);

        system("cls");
        MENU(0);

    }
    else return;
}



