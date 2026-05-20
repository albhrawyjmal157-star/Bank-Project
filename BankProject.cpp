#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;


//Methods of clients 

const string FileClientsName = "Clients.txt";
const string FileUserName = "Users.txt";
const string Separator = "#//#";

struct stClient
{
    string AccountNumber;
    string PinCode;
    string ClientName;
    string Phone;
    double Balance = 0;
};
struct stUser
{
    string UserName;
    string Password;
    int Permission=0;
};

enum enChoices
{
    ShowListClients = 1,
    AddNewClient = 2,
    DeleteClient = 3,
    UpdateClient = 4,
    FindClient = 5,
    Transactions = 6,
    ManageUser = 7,
    Exit = 8,

};
enum enpermissionOfMainMenu
{
    all = -1,
    PermissionShowListClients = 1,
    PermissionAddNewClient = 2,
    PermissionDeleteClient = 4,
    PermissionUpdateClient = 8,
    PermissionFindClient = 16,
    PermissionTransactions = 32,
    PermissionManageUser =64,

};
enum enTransactions
{
    Deposit = 1,
    Withdraw = 2,
    TotalBalance = 3,
    MainMenuTransactions = 4
};

enum enManageUser
{
    ListUsers = 1,
    AddNewUser = 2,
    DeleteUser = 3,
    UpdataUser = 4,
    FindUser = 5,
    MainMenuManageUser = 6,
};

vector<stClient> vClients;
vector<stUser> vUsers;
stUser CurrentUser;
vector<string> Split(string Statement, string Separator = "#//#")
{
    vector<string> vWords;
    size_t Position = 0;

    while ((Position = Statement.find(Separator)) != string::npos)
    {
        vWords.push_back(Statement.substr(0, Position));
        Statement.erase(0, Position + Separator.length());
    }

    if (!Statement.empty())
    {
        vWords.push_back(Statement);
    }

    return vWords;
}

stClient ConvertLineToStructure(const string& Line)
{
    stClient Client;

    vector<string> Words = Split(Line, Separator);

    if (Words.size() >= 5)
    {
        Client.AccountNumber = Words[0];
        Client.PinCode = Words[1];
        Client.ClientName = Words[2];
        Client.Phone = Words[3];
        Client.Balance = stod(Words[4]);
    }

    return Client;
}

string ConvertClientToLine(const stClient& Client)
{
    return Client.AccountNumber + Separator +
        Client.PinCode + Separator +
        Client.ClientName + Separator +
        Client.Phone + Separator +
        to_string(Client.Balance);
}

vector<stClient> LoadClientsDataFromFile()
{
    vector<stClient> vClients;

    fstream MyFile(FileClientsName, ios::in);

    if (MyFile.is_open())
    {
        string Line;

        while (getline(MyFile, Line))
        {
            vClients.push_back(ConvertLineToStructure(Line));
        }

        MyFile.close();
    }

    return vClients;
}

void SaveClientsDataToFile(const vector<stClient>& vClients)
{
    fstream MyFile(FileClientsName, ios::out);

    if (MyFile.is_open())
    {
        for (const stClient& Client : vClients)
        {
            MyFile << ConvertClientToLine(Client) << endl;
        }

        MyFile.close();
    }
}
void PrintDeniedMessage()
{
    cout << "\n-----------------------------------------\n";
    cout << "Access Denied, \nYou dont have permission to do this \nplease conact your admin . \n";
    cout << "\n-----------------------------------------\n";
    system("pause > 0");
}

bool CheckPermission(enpermissionOfMainMenu Permission)
{
    if (CurrentUser.Permission == -1)
    {
        return true;
    }
    if ((Permission & CurrentUser.Permission) == Permission)
    {
        return true;
    }
    else
    {
        return false;
    }
}

string ReadData(const string& Message)
{
    string Line;

    cout << Message;
    getline(cin >> ws, Line);

    return Line;
}

double ReadBalance()
{
    double Balance;

    cout << "Enter Balance ? ";
    cin >> Balance;

    return Balance;
}

void PrintClient(const stClient& Client)
{
    cout << "--------------------------------------------------\n";
    cout << "Client Info : \n";
    cout << "Account Number : " << Client.AccountNumber << endl;
    cout << "PinCode        : " << Client.PinCode << endl;
    cout << "Client Name    : " << Client.ClientName << endl;
    cout << "Phone          : " << Client.Phone << endl;
    cout << "Balance        : " << fixed << setprecision(2) << Client.Balance << endl;
    cout << "--------------------------------------------------\n";
}

bool FindClientByAccountNumber(const string& AccountNumber, stClient& Client)
{
    for (const stClient& C : vClients)
    {
        if (C.AccountNumber == AccountNumber)
        {
            Client = C;
            return true;
        }
    }

    return false;
}

void ReadClientData(stClient& Client)
{
    Client.PinCode = ReadData("Enter PinCode ? ");
    Client.ClientName = ReadData("Enter Client Name ? ");
    Client.Phone = ReadData("Enter Phone ? ");
    Client.Balance = ReadBalance();
}

int ReadChoiceNumber(int From, int To)
{
    int Number = 0;

    do
    {
        cout << "\nEnter Your Choice (" << From << "," << To << ") ? ";
        cin >> Number;

    } while (Number < From || Number > To);

    return Number;
}

void ShowClientsListScreen()
{
    system("cls");

    if (!CheckPermission(PermissionShowListClients))
    {
        PrintDeniedMessage();
        return;
    }

    cout << "\t\t==============================================\n";
    cout << "\t\t            Show Clients Screen\n";
    cout << "\t\t==============================================\n";

    if (vClients.empty())
    {
        cout << "\n\t\tNo Clients Available.\n";
        return;
    }

    cout << "\n\t\tClients Count : " << vClients.size() << "\n\n";

    cout << "\t\t"
        << setw(20) << left << "Account Number"
        << setw(15) << left << "PinCode"
        << setw(30) << left << "Client Name"
        << setw(20) << left << "Phone"
        << setw(12) << left << "Balance"
        << endl;

    cout << "\t\t---------------------------------------------------------------------------------------------\n";

    for (const stClient& Client : vClients)
    {
        cout << "\t\t"
            << setw(20) << left << Client.AccountNumber
            << setw(15) << left << Client.PinCode
            << setw(30) << left << Client.ClientName
            << setw(20) << left << Client.Phone
            << setw(12) << left << fixed << setprecision(2) << Client.Balance
            << endl;
    }
}

void AddClient()
{
    system("cls");
    if (!CheckPermission(PermissionAddNewClient))
    {
        PrintDeniedMessage();
        return;
    }

    cout << "\t\t==============================================\n";
    cout << "\t\t              Add Client Screen\n";
    cout << "\t\t==============================================\n\n";

    stClient Client;

    Client.AccountNumber = ReadData("Enter Account Number ? ");

    while (FindClientByAccountNumber(Client.AccountNumber, Client))
    {
        cout << "\nAccount Number Already Used!\n";

        Client.AccountNumber = ReadData("Enter Another Account Number ? ");

        if (Client.AccountNumber == "-1")
        {
            return;
        }
    }

    ReadClientData(Client);

    vClients.push_back(Client);

    SaveClientsDataToFile(vClients);

    cout << "\nClient Added Successfully.\n";
}

void DeleteClientFromSystem()
{
    system("cls");
    if (!CheckPermission(PermissionDeleteClient))
    {
        PrintDeniedMessage();
        return;
    }
    stClient Client;

    cout << "\t\t==============================================\n";
    cout << "\t\t            Delete Client Screen\n";
    cout << "\t\t==============================================\n\n";

    string AccountNumber = ReadData("Enter Account Number : ");

    while (!FindClientByAccountNumber(AccountNumber, Client))
    {
        cout << "\nAccount Number Not Found!\n";

        AccountNumber = ReadData("Enter Account Number : ");

        if (AccountNumber == "-1")
        {
            return;
        }
    }

    PrintClient(Client);

    char Answer;

    cout << "\nDo You Want Delete This Client (y/n) ? ";
    cin >> Answer;

    if (tolower(Answer) == 'y')
    {
        for (int i = 0; i < vClients.size(); i++)
        {
            if (vClients[i].AccountNumber == AccountNumber)
            {
                vClients.erase(vClients.begin() + i);

                SaveClientsDataToFile(vClients);

                cout << "\nClient Deleted Successfully.\n";

                return;
            }
        }
    }
}

void UpdateClientFromSystem()
{
    system("cls");
    if (!CheckPermission(PermissionUpdateClient))
    {
        PrintDeniedMessage();
        return;
    }
    stClient Client;

    cout << "\t\t==============================================\n";
    cout << "\t\t            Update Client Screen\n";
    cout << "\t\t==============================================\n\n";

    string AccountNumber = ReadData("Enter Account Number : ");

    while (!FindClientByAccountNumber(AccountNumber, Client))
    {
        cout << "\nAccount Number Not Found!\n";

        AccountNumber = ReadData("Enter Account Number : ");

        if (AccountNumber == "-1")
        {
            return;
        }
    }

    for (stClient& Client : vClients)
    {
        if (Client.AccountNumber == AccountNumber)
        {
            PrintClient(Client);

            char Answer;

            cout << "\nDo You Want Update This Client (y/n) ? ";
            cin >> Answer;

            if (tolower(Answer) == 'y')
            {
                ReadClientData(Client);

                SaveClientsDataToFile(vClients);

                cout << "\nClient Updated Successfully.\n";
            }

            return;
        }
    }
}

void Find()
{
    system("cls");
    if (!CheckPermission(PermissionFindClient))
    {
        PrintDeniedMessage();
        return;
    }
    stClient Client;

    cout << "\t\t==============================================\n";
    cout << "\t\t              Find Client Screen\n";
    cout << "\t\t==============================================\n\n";

    string AccountNumber = ReadData("Enter Account Number : ");

    if (FindClientByAccountNumber(AccountNumber, Client))
    {
        PrintClient(Client);
    }
    else
    {
        cout << "\nClient Not Found.\n";
    }
}

void DepositScreen()
{
    system("cls");

    stClient Client;

    cout << "-------------------------------------\n";
    cout << "\tDeposit Screen\n";
    cout << "-------------------------------------\n";

    string AccountNumber;

    cout << "Enter Account Number : ";
    cin >> AccountNumber;

    while (!FindClientByAccountNumber(AccountNumber, Client))
    {
        cout << "Account Number Not Found!\n";

        cout << "Enter Account Number : ";
        cin >> AccountNumber;
    }

    PrintClient(Client);

    double Amount;

    cout << "\nDeposit Amount : ";
    cin >> Amount;

    while (Amount <= 0)
    {
        cout << "Invalid Amount Enter Again : ";
        cin >> Amount;
    }

    for (stClient& Client : vClients)
    {
        if (Client.AccountNumber == AccountNumber)
        {
            Client.Balance += Amount;

            break;
        }
    }

    SaveClientsDataToFile(vClients);

    cout << "\nDeposit Successfully.\n";
}

void WithdrawScreen()
{
    system("cls");

    stClient Client;

    cout << "-------------------------------------\n";
    cout << "\tWithdraw Screen\n";
    cout << "-------------------------------------\n";

    string AccountNumber;

    cout << "Enter Account Number : ";
    cin >> AccountNumber;

    while (!FindClientByAccountNumber(AccountNumber, Client))
    {
        cout << "Account Number Not Found!\n";

        cout << "Enter Account Number : ";
        cin >> AccountNumber;
    }

    PrintClient(Client);

    double Amount;

    cout << "\nWithdraw Amount : ";
    cin >> Amount;

    while (Amount <= 0)
    {
        cout << "Invalid Amount Enter Again : ";
        cin >> Amount;
    }

    for (stClient& Client : vClients)
    {
        if (Client.AccountNumber == AccountNumber)
        {
            while (Amount > Client.Balance)
            {
                cout << "\nAmount Exceeds Balance.\n";

                cout << "Enter Another Amount : ";
                cin >> Amount;
            }

            Client.Balance -= Amount;

            break;
        }
    }

    SaveClientsDataToFile(vClients);

    cout << "\nWithdraw Successfully.\n";
}

void TotalBalanceScreen()
{
    system("cls");

    cout << "\t\t-------------------------------------\n";
    cout << "\t\t\tTotal Balance Screen\n";
    cout << "\t\t-------------------------------------\n";

    double TotalBalances = 0;

    cout << left
        << setw(20) << "Account Number"
        << setw(30) << "Client Name"
        << setw(20) << "Balance"
        << "\n";

    cout << string(70, '-') << "\n";

    for (const stClient& Client : vClients)
    {
        TotalBalances += Client.Balance;

        cout << left
            << setw(20) << Client.AccountNumber
            << setw(30) << Client.ClientName
            << setw(20) << fixed << setprecision(2) << Client.Balance
            << "\n";
    }

    cout << string(70, '-') << "\n";

    cout << "\nTotal Balances = " << TotalBalances << endl;
}




//Create Login Users 

stUser ConvertLineToStUser(const string& Line)
{
    stUser User;

    vector<string> Words = Split(Line, Separator);

    if (Words.size() >= 3)
    {
        User.UserName = Words[0];
        User.Password = Words[1];
        User.Permission = stoi(Words[2]);

    }

    return User;
}
string ConvertStUserToLine(const stUser& User)
{
    return User.UserName + Separator + User.Password + Separator
        + to_string(User.Permission);
}
void SaveUsersDataToFile(const vector<stUser>& vUsers)
{
    fstream MyFile(FileUserName, ios::out);

    if (MyFile.is_open())
    {
        for (const stUser& User : vUsers)
        {
            MyFile << ConvertStUserToLine(User) << endl;
        }

        MyFile.close();
    }
}
void PrintUser(const stUser& User)
{
    cout << "--------------------------------------------------\n";
    cout << "\n\t  User Info          \n";
    cout << "User Name         : " << User.UserName << endl;
    cout << "Password          : " << User.Password << endl;
    cout << "Permission        : " << fixed << setprecision(2) << User.Permission << endl;
    cout << "--------------------------------------------------\n";
}

vector<stUser> LoadDataUserFromFileUsers()
{
    vector<stUser> vUsers;

    fstream MyFile(FileUserName, ios::in);

    if (MyFile.is_open())
    {
        string Line;

        while (getline(MyFile, Line))
        {
            vUsers.push_back(ConvertLineToStUser(Line));
        }

        MyFile.close();
    }

    return vUsers;
}
bool FindUserByUserName(const string& UserName)
{
    for (const stUser& User : vUsers)
    {
        if (User.UserName == UserName )
        {
            return true;
        }
    }
    return false;
}
bool FindUserByUserNameAndPassword(const string& UserName, const string& Password)
{
    for (const stUser& User : vUsers)
    {
        if (User.UserName == UserName && User.Password == Password)
        {
            CurrentUser = User;
            return true;
        }
    }
    return false;
}
bool FindUserByUserNameAndPassword(const string& UserName, const string& Password,stUser& User)
{
    for (const stUser& User : vUsers)
    {
        if (User.UserName == UserName && User.Password == Password)
        {
            CurrentUser = User;
            return true;
        }
    }
    return false;
}
int ReadPermission()
{
    char AnswerAccess = 'n';
    int Permission = 0;
    cout << "\n Do You want to give full access (y,n) ? ";
    cin >> AnswerAccess;

    if (tolower(AnswerAccess) == 'y')
    {
       return -1;
    }
    
    cout << "\nDo You want to give access to : \n";
    cout << "\nShow Client List (y,n)?";
    cin >> AnswerAccess;
    if (tolower(AnswerAccess) == 'y')
    {
        Permission += enpermissionOfMainMenu::PermissionShowListClients;
    }
    cout << "\nAdd New Client  (y,n)?";
    cin >> AnswerAccess;
    if (tolower(AnswerAccess) == 'y')
    {
        Permission += enpermissionOfMainMenu::PermissionAddNewClient;
    }
    cout << "\nDelete Client (y,n)?";
    cin >> AnswerAccess;
    if (tolower(AnswerAccess) == 'y')
    {
        Permission += enpermissionOfMainMenu::PermissionDeleteClient;
    }
    cout << "\nUpdata Client (y,n)?";
    cin >> AnswerAccess;
    if (tolower(AnswerAccess) == 'y')
    {
        Permission += enpermissionOfMainMenu::PermissionUpdateClient;
    }
    cout << "\nFind Client  (y,n)?";
    cin >> AnswerAccess;
    if (tolower(AnswerAccess) == 'y')
    {
        Permission += enpermissionOfMainMenu::PermissionFindClient;
    }
    cout << "\nTransactions (y,n)?";
    cin >> AnswerAccess;
    if (tolower(AnswerAccess) == 'y')
    {
        Permission += enpermissionOfMainMenu::PermissionTransactions;
    }
    cout << "\nManage Users (y,n)?";
    cin >> AnswerAccess;
    if (tolower(AnswerAccess) == 'y')
    {
        Permission += enpermissionOfMainMenu::PermissionManageUser;
    }
    return Permission;
   
}


void ShowUsersListScreen()
{
    system("cls");

    cout << "\t\t==============================================\n";
    cout << "\t\t            Show Users Screen\n";
    cout << "\t\t==============================================\n";

    if (vUsers.empty())
    {
        cout << "\n\t\tNo Users Available.\n";
        return;
    }

    cout << "\n\t\t Users Count : " << vUsers.size() << "\n\n";

    cout << "\t\t"
        << setw(40) << left << "User Name"
        << setw(20) << left << "Password"
        << setw(15) << left << "Permission"

        << endl;

    cout << "\t\t---------------------------------------------------------------------------------------------\n";

    for (const stUser& User : vUsers)
    {
        cout << "\t\t"
            << setw(40) << left << User.UserName
            << setw(20) << left << User.Password
            << setw(15) << left << User.Permission
            << endl;
    }
}
void AddUser()
{
    char AnswerToContinue = 'y';
    while (AnswerToContinue == 'y')
    {
        system("cls");

        cout << "\t\t==============================================\n";
        cout << "\t\t              Add User Screen\n";
        cout << "\t\t==============================================\n\n";

        stUser User;

        User.UserName = ReadData("Enter User ? ");
        User.Password = ReadData("Enter Password ?");

        while (FindUserByUserName(User.UserName))
        {
            cout << "\nUser Name  Already Used!\n";

            User.UserName = ReadData("Enter User ? ");
            User.Password = ReadData("Enter Password ? ");
        }

        User.Permission = ReadPermission();
        vUsers.push_back(User);
        SaveUsersDataToFile(vUsers);






        cout << "\nUser Added Successfully.Do you want added new User (y,n)?\n";
        cin >> AnswerToContinue;

    }
}
void DeleteUsertFromSystem()
{
    system("cls");
    
    
    cout << "\t\t==============================================\n";
    cout << "\t\t            Delete User Screen\n";
    cout << "\t\t==============================================\n\n";

    string UserName = ReadData("Enter User Name: ");
    if (UserName == "admin")
    {
        cout << "You cannot delete admin.\n";
        return;
    }
    string Password = ReadData("Enter PassWord: ");
    while (!FindUserByUserNameAndPassword(UserName,Password))
    {
        cout << "\nNot Found UserName and Password !\n";

        UserName = ReadData("Enter User Name: ");
        Password = ReadData("Enter PassWord: ");

        if (Password == "-1")
        {
            return;
        }
    }

    

    char Answer;
    cout << "\nDo You Want Delete This User (y/n) ? ";
    cin >> Answer;

    if (tolower(Answer) == 'y')
    {
        for (int i = 0; i < vUsers.size(); i++)
        {
            if (vUsers[i].UserName == UserName && vUsers[i].Password == Password)
            {
                PrintUser(vUsers[i]);
                cout << "\nAre You Sure? (y,n) \n";
                cin >> Answer;
                if (tolower(Answer) == 'y')
                {
                    vUsers.erase(vUsers.begin() + i);

                    SaveUsersDataToFile(vUsers);

                    cout << "\nUser Deleted Successfully.\n";

                    return;
                }
            }
        }
    }
}
void UpdateUserFromScreen()
{
    system("cls");
    cout << "\t\t==============================================\n";
    cout << "\t\t            Update User Screen\n";
    cout << "\t\t==============================================\n\n";

    string UserName = ReadData("Enter User : ");
    string Password = ReadData("Enter Password : ");
    while (!FindUserByUserNameAndPassword(UserName,Password))
    {
        cout << "\nUser Not Found!\n";

        UserName = ReadData("Enter User : ");
        Password = ReadData("Enter Password : ");
        if (Password == "-1")
        {
            return;
        }
    }

    for (stUser& User : vUsers)
    {
        if (User.UserName == UserName && User.Password == Password)
        {
            PrintUser(User);

            char Answer;

            cout << "\nDo You Want Update This User (y/n) ? ";
            cin >> Answer;

            if (tolower(Answer) == 'y')
            {
               
               
                string NewUserName = ReadData("Enter New User Name : ");

                while (FindUserByUserName(NewUserName) && NewUserName != User.UserName)
                {
                    cout << "UserName already exists!\n";
                    NewUserName = ReadData("Enter Another User Name : ");
                }

                User.UserName = NewUserName;
                User.Password = ReadData("Enter New Password : \n");
                User.Permission = ReadPermission();

                SaveUsersDataToFile(vUsers);

                cout << "\nUSer Updated Successfully.\n";
            }

            return;
        }
    }
}
void    FindUserFromSecreen()
{
    system("cls");
    cout << "\t\t==============================================\n";
    cout << "\t\t            Find User Screen\n";
    cout << "\t\t==============================================\n\n";
    string UserName = ReadData("Enter UserName : \n");
    string Password = ReadData("Enter Password : \n");
    stUser User;
    if (FindUserByUserNameAndPassword(UserName, Password, User))
    {
        PrintUser(User);
    }
    else
    {
        cout << "\n\nUser Not Found \n\n";
    }
    
}

//Draw MainMenus

void DrawTransactionsScreen()
{
    system("cls");
    if (!CheckPermission(PermissionTransactions))
    {
        PrintDeniedMessage();
        return;
    }
    while (true)
    {


        cout << "\t\t==============================================\n";
        cout << "\t\t         Transactions Menu Screen\n";
        cout << "\t\t==============================================\n";

        cout << "\t\t[1] Deposit.\n";
        cout << "\t\t[2] Withdraw.\n";
        cout << "\t\t[3] Total Balance.\n";
        cout << "\t\t[4] Main Menu.\n";

        switch ((enTransactions)ReadChoiceNumber(1, 4))
        {
        case Deposit:
            DepositScreen();
            system("pause");
            break;

        case Withdraw:
            WithdrawScreen();
            system("pause");
            break;

        case TotalBalance:
            TotalBalanceScreen();
            system("pause");
            break;

        case MainMenuTransactions:
            return;
        }
    }
}

void DrawManageUsersScreen()
{
    system("cls");
    if (!CheckPermission(PermissionManageUser))
    {
        PrintDeniedMessage();
        return;
    }
    while (true)
    {
        system("cls");

        cout << "\t\t==============================================\n";
        cout << "\t\t         Manage Users Menu Screen\n";
        cout << "\t\t==============================================\n";

        cout << "\t\t[1] List Usera.\n";
        cout << "\t\t[2] Add New User.\n";
        cout << "\t\t[3] Delete User.\n";
        cout << "\t\t[4] Updata User.\n";
        cout << "\t\t[5] Find User.\n";
        cout << "\t\t[6] Main Menu.\n";

        switch ((enManageUser)ReadChoiceNumber(1, 6))
        {
        
        case ListUsers:
            ShowUsersListScreen();
            system("pause");
            break;
        
        case AddNewUser:
            AddUser();
            system("pause");
            break;
        
        
        case DeleteUser:
            DeleteUsertFromSystem();
            system("pause");
            break;

        case UpdataUser:
            UpdateUserFromScreen();
            system("pause");
            break;
        
        case FindUser:
            FindUserFromSecreen();
            system("pause > 0");
            break;
        
        
        case MainMenuManageUser:
            return;
        }
    }
}


void DrawMainScreen()
{
    while (true)
    {
        system("cls");

        cout << "\t\t==============================================\n";
        cout << "\t\t              Main Menu Screen\n";
        cout << "\t\t==============================================\n";

        cout << "\t\t[1] Show Clients List.\n";
        cout << "\t\t[2] Add New Client.\n";
        cout << "\t\t[3] Delete Client.\n";
        cout << "\t\t[4] Update Client Info.\n";
        cout << "\t\t[5] Find Client.\n";
        cout << "\t\t[6] Transactions.\n";
        cout << "\t\t[7] Manage User.\n";
        cout << "\t\t[8] Logout.\n";

        switch ((enChoices)ReadChoiceNumber(1, 8))
        {
        case ShowListClients:
            ShowClientsListScreen();
            system("pause");
            break;

        case AddNewClient:
            AddClient();
            system("pause");
            break;

        case DeleteClient:
            DeleteClientFromSystem();
            system("pause");
            break;

        case UpdateClient:
            UpdateClientFromSystem();
            system("pause");
            break;

        case FindClient:
            Find();
            system("pause");
            break;

        case Transactions:
            DrawTransactionsScreen();
            break;
        
        case ManageUser:
            DrawManageUsersScreen();
            break;
        case Exit:
            cout << "\nGood Bye.\n";
            return;
        }
    }
} 



void Login()
{
    while (true)
    {
        system("cls");

        cout << "\t\t==============================================\n";
        cout << "\t\t             Login Screen\n";
        cout << "\t\t==============================================\n";
        string UserName;
        string Password;
        
        cout << "Enter UserName : ";
        cin >> UserName;
        cout << "Enter Password : ";
        cin >> Password;

        while (!FindUserByUserNameAndPassword(UserName, Password))
        {
            cout << "Invaild UserName/Passwor .\n";
            cout << "Enter UserName : ";
            cin >> UserName;
            cout << "Enter Password : ";
            cin >> Password;
        }
        DrawMainScreen();

    }
    
}

int main()
{
    vClients = LoadClientsDataFromFile();
    vUsers = LoadDataUserFromFileUsers();
    Login();

    return 0;
}