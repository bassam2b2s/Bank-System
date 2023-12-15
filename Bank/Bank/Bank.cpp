#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;

struct stClient {

    string AccountNumber = "";
    string PINCode = "";
    string Name = "";
    string Phone = "";
    double AccountBalance;
    bool MarkForDelete = false;

};
struct stUser {

    string UserName = "";
    string Password = "";
    int Permissions = 0;
    bool MarkForDelete = false;

};

const string ClientsFile = "Clients.txt";
const string UsersFile = "Users.txt";

stUser CurrentUser;

enum enManageUsersMenuOptions { eListUsers = 1, eAddNewUser = 2, eDeleteUser = 3, eUpdateUserInfo = 4, eFindUser = 5, eMainMenu = 6 };
enum enTransactionsMenuOptions { eDeposit = 1, eWithdraw = 2, eShowTotalBalances = 3, eShowMainMenu = 4 };
enum enPermissions { ePAll = -1, ePShowClientList = 1, ePAddNewClient = 2, ePDeleteClient = 4, ePUpdateClientInfo = 8, ePFindClient = 16, ePTransactions = 32, ePManageUsers = 64, ePLogout = 0 };
enum enMainMenuOptions { eShowClientList = 1, eAddNewClient = 2, eDeleteClient = 3, eUpdateClient = 4, eFindClient = 5, eShowTransactionsMainMenu = 6, eManageUsers = 7, eLogout = 8 };

void ClearScreen();
void BackToMainMenu();
void ShowMainMenuScreen();
void ShowTransactionsMenuScreen();
void ShowManageUsersMenuScreen();
void Login();

vector <string> SplitString(string str, string Space) {


    vector <string> vWords = {};
    string Word = "";
    short Position = 0;

    while ((Position = str.find(Space)) != std::string::npos) {

        Word = str.substr(0, Position);

        if (Word != "") {

            vWords.push_back(Word);

        }

        str = str.erase(0, Position + Space.length());

    }

    if (str != "") {

        vWords.push_back(str);

    }

    return vWords;

}

stClient ConvertLineToClientRecord(string str, string Space) {

    stClient Client;
    vector <string> vWords = SplitString(str, Space);

    Client.AccountNumber = vWords.at(0);
    Client.PINCode = vWords.at(1);
    Client.Name = vWords.at(2);
    Client.Phone = vWords.at(3);
    Client.AccountBalance = stof(vWords.at(4));

    return Client;

}

stUser ConvertLineToUserRecord(string Line, string Space) {

    stUser User;
    vector <string> vUsersData = SplitString(Line, Space);

    User.UserName = vUsersData.at(0);
    User.Password = vUsersData.at(1);
    User.Permissions = stoi(vUsersData.at(2));

    return User;

}

string ConvertClientRecordToLine(stClient Client, string Space) {

    string Line = "";

    Line += Client.AccountNumber + Space;
    Line += Client.PINCode + Space;
    Line += Client.Name + Space;
    Line += Client.Phone + Space;
    Line += to_string(Client.AccountBalance);

    return Line;

}

string ConvertUserRecordToLine(stUser User, string Space) {

    string Line = "";

    Line += User.UserName + Space;
    Line += User.Password + Space;
    Line += to_string(User.Permissions);

    return Line;

}

vector <stClient> LoadClientsDataFromFile(string FileName) {

    fstream MyFile;

    MyFile.open(FileName, ios::in);

    vector <stClient> vClients = {};

    if (MyFile.is_open()) {

        string Line;
        stClient Client;

        while (getline(MyFile, Line))
        {

            Client = ConvertLineToClientRecord(Line, "//#");
            vClients.push_back(Client);

        }

        MyFile.close();

    }

    return vClients;

}

vector <stUser> LoadUsersDataFromFile(string FileName) {

    fstream MyFile;

    MyFile.open(FileName, ios::in);

    vector <stUser> vUsers = {};

    if (MyFile.is_open()) {

        string Line;
        stUser User;

        while (getline(MyFile, Line))
        {

            User = ConvertLineToUserRecord(Line, "//#");
            vUsers.push_back(User);

        }

        MyFile.close();

    }

    return vUsers;

}

vector <stClient> SaveClientsDataToFile(string FileName, vector <stClient> vClients) {

    fstream MyFile;

    MyFile.open(FileName, ios::out);

    string Line = "";

    if (MyFile.is_open()) {

        for (stClient& Client : vClients) {

            if (Client.MarkForDelete == false) {

                Line = ConvertClientRecordToLine(Client, "//#");
                MyFile << Line << endl;

            }

        }

        MyFile.close();

    }

    return vClients;

}

vector <stUser> SaveUsersDataToFile(string FileName, vector <stUser> vUsers) {

    fstream MyFile;

    MyFile.open(FileName, ios::out);

    string Line = "";

    if (MyFile.is_open()) {

        for (stUser& User : vUsers) {

            if (User.MarkForDelete == false) {

                Line = ConvertUserRecordToLine(User, "//#");
                MyFile << Line << endl;

            }

        }

        MyFile.close();

    }

    return vUsers;

}

void ShowAccessDeniedMessage() {

    ClearScreen();

    cout << "--------------------------------------\n";
    cout << "Access Denied,\nYou Dont Have Permission To Do This,\nPlease Contact Your Admin.";
    cout << "\n--------------------------------------\n";

    BackToMainMenu();

}

bool CheckPermissionAccess(enPermissions Permission) {

    if (CurrentUser.Permissions == enPermissions::ePAll)
    
        return true;

    if ((CurrentUser.Permissions & Permission) == Permission)

        return true;

    else

        return false;

}

void PrintClientData(stClient Client) {

    cout << "| " << left << setw(16) << Client.AccountNumber;
    cout << "| " << left << setw(10) << Client.PINCode;
    cout << "| " << left << setw(25) << Client.Name;
    cout << "| " << left << setw(12) << Client.Phone;
    cout << "| " << left << setw(12) << Client.AccountBalance;

}

void ShowAllClients() {

    if (!CheckPermissionAccess(enPermissions::ePShowClientList)) {

        ShowAccessDeniedMessage();
        return;

    }

    vector <stClient> vClients = LoadClientsDataFromFile(ClientsFile);

    cout << "\n\t\t\t Client list (" << vClients.size() << ") Client(s). \n\n";

    cout << "_________________________________________________________________________________________\n\n";
    cout << "| " << left << setw(16) << "Account Number";
    cout << "| " << left << setw(10) << "PIN Code";
    cout << "| " << left << setw(25) << "Client Name";
    cout << "| " << left << setw(12) << "Phone";
    cout << "| " << left << setw(12) << "Account Balance";
    cout << "\n_________________________________________________________________________________________\n\n";

    if (vClients.size() != 0) {

        for (stClient& Client : vClients) {

            PrintClientData(Client);
            cout << endl;

        }

    }

    else {

        cout << "No Clients Available In The System.\n";

    }

    cout << "_________________________________________________________________________________________\n";


}

void PrintUserData(stUser User) {

    cout << "| " << left << setw(16) << User.UserName;
    cout << "| " << left << setw(10) << User.Password;
    cout << "| " << left << setw(25) << User.Permissions;

}

void ShowAllUsers() {

    vector <stUser> vUsers = LoadUsersDataFromFile(UsersFile);

    cout << "\n\t\t\t User List (" << vUsers.size() << ") User(s). \n\n";

    cout << "_________________________________________________________________________________________\n\n";
    cout << "| " << left << setw(16) << "UserName";
    cout << "| " << left << setw(10) << "Password";
    cout << "| " << left << setw(25) << "Permissions";
    cout << "\n_________________________________________________________________________________________\n\n";

    if (vUsers.size() != 0) {

        for (stUser& User : vUsers) {

            PrintUserData(User);
            cout << endl;

        }

    }

    else {

        cout << "No Users Available In The System.\n";

    }

    cout << "_________________________________________________________________________________________\n";


}

bool ClientExistsByAccountNumber(string AccountNumber, string FileName) {

    vector <stClient> vClients = LoadClientsDataFromFile(ClientsFile);

    fstream MyFile;

    MyFile.open(FileName, ios::in);

    if (MyFile.is_open()) {

        for (stClient& Client : vClients) {

            if (Client.AccountNumber == AccountNumber) {

                MyFile.close();
                return true;

            }

        }

        MyFile.close();

    }

    return false;

}

stClient ReadNewClient() {

    stClient Client;

    // cin >> ws to ignore all enters and white spaces in the buffer.

    cout << "Enter Account Number ? ";
    getline(cin >> ws, Client.AccountNumber);

    while (ClientExistsByAccountNumber(Client.AccountNumber, ClientsFile)) {

        cout << "Client With [" << Client.AccountNumber << "] is Already Exist, Enter Account Number ? ";
        getline(cin >> ws, Client.AccountNumber);

    }

    cout << "Enter PIN Code ? ";
    getline(cin, Client.PINCode);

    cout << "Enter Name ? ";
    getline(cin, Client.Name);

    cout << "Enter Phone ? ";
    getline(cin, Client.Phone);

    cout << "Enter Account Balance ? ";
    cin >> Client.AccountBalance;

    return Client;

}

void AddDataRecordToFile(string FileName, string Record) {

    fstream MyFile;

    MyFile.open(FileName, ios::out | ios::app);

    if (MyFile.is_open()) {

        MyFile << Record << endl;

        MyFile.close();

    }

}

void AddNewClient() {

    stClient Client;
    Client = ReadNewClient();

    AddDataRecordToFile(ClientsFile, ConvertClientRecordToLine(Client, "//#"));

}

void AddNewClients() {

    char Choice = ' ';

    do
    {
        cout << "\nAdding New Client : \n\n";

        AddNewClient();

        cout << "\nClient Added Successfully,  do you want to add more clients Y/N ? ";
        cin >> Choice;

    } while (Choice == 'Y' || Choice == 'y');

}

void ShowAddNewClientsScreen() {

    if (!CheckPermissionAccess(enPermissions::ePAddNewClient)) {

        ShowAccessDeniedMessage();
        return;

    }

    cout << "--------------------------------------\n";
    cout << "\tAdd New Client\n";
    cout << "--------------------------------------\n";

    AddNewClients();

}

bool UserExistsByUserName(string UserName, string FileName) {

    vector <stUser> vUsers = LoadUsersDataFromFile(UsersFile);

    fstream MyFile;

    MyFile.open(FileName, ios::in);

    if (MyFile.is_open()) {

        for (stUser& User : vUsers) {

            if (User.UserName == UserName) {

                MyFile.close();
                return true;

            }

        }

        MyFile.close();

    }

    return false;

}

int ReadPermissions() {

    string Choice = "";
    int Permissions = 0;

    cout << "\nDo You Want To Give Full Access ? y/n ? ";
    cin >> Choice;

    if (Choice == "y" || Choice == "Y")

        return -1;

    else

    {

        cout << "\nDo You Want To Give Access To : \n";

        cout << "\nShow Client List ? y/n ? ";
        cin >> Choice;

        if (Choice == "y" || Choice == "Y")
            Permissions += enPermissions::ePShowClientList;

        cout << "\nAdd New Client ? y/n ? ";
        cin >> Choice;

        if (Choice == "y" || Choice == "Y")
            Permissions += enPermissions::ePAddNewClient;

        cout << "\nDelete Client ? y/n ? ";
        cin >> Choice;

        if (Choice == "y" || Choice == "Y")
            Permissions += enPermissions::ePDeleteClient;

        cout << "\nUpdate Client Info ? y/n ? ";
        cin >> Choice;

        if (Choice == "y" || Choice == "Y")
            Permissions += enPermissions::ePUpdateClientInfo;

        cout << "\nFind Client ? y/n ? ";
        cin >> Choice;

        if (Choice == "y" || Choice == "Y")
            Permissions += enPermissions::ePFindClient;

        cout << "\nTransactions ? y/n ? ";
        cin >> Choice;

        if (Choice == "y" || Choice == "Y")
            Permissions += enPermissions::ePTransactions;

        cout << "\nManage Users ? y/n ? ";
        cin >> Choice;

        if (Choice == "y" || Choice == "Y")
            Permissions += enPermissions::ePManageUsers;

    }

    return Permissions;

}

stUser ReadNewUser() {

    stUser User;

    cout << "\nEnter Username ? ";
    getline(cin >> ws, User.UserName);

    while (UserExistsByUserName(User.UserName, UsersFile)) {

        cout << "User With [" << User.UserName << "] is Already Exist, Enter Username ? ";
        getline(cin >> ws, User.UserName);

    }

    cout << "\nEnter Password ? ";
    getline(cin, User.Password);

    User.Permissions = ReadPermissions();

    return User;

}

void AddNewUser() {

    stUser User;
    User = ReadNewUser();

    AddDataRecordToFile(UsersFile, ConvertUserRecordToLine(User, "//#"));

}

void AddNewUsers() {

    char Choice = ' ';

    do
    {
        cout << "\nAdding New User : \n\n";

        AddNewUser();

        cout << "\nUser Added Successfully,  do you want to add more users Y/N ? ";
        cin >> Choice;

    } while (Choice == 'Y' || Choice == 'y');

}

void ShowAddNewUsersScreen() {

    cout << "--------------------------------------\n";
    cout << "\tAdd New User\n";
    cout << "--------------------------------------\n";

    AddNewUsers();

}

string ReadClientAccountNumber() {

    string AccountNumber = "";

    cout << "Enter Account Number ? ";
    cin >> AccountNumber;

    return AccountNumber;

}

void PrintClientCard(stClient Client) {

    cout << "\nThe following are client detail : \n";

    cout << "--------------------------------------";
    cout << "\nAccount Number : " << Client.AccountNumber;
    cout << "\nPIN Code : " << Client.PINCode;
    cout << "\nName : " << Client.Name;
    cout << "\nPhone : " << Client.Phone;
    cout << "\nAccount Balance : " << Client.AccountBalance << "\n";
    cout << "--------------------------------------\n";


}

bool FindClientByAccountNumber(string AccountNumber, vector <stClient> vClients, stClient& FClient) {

    for (stClient& Client : vClients) {

        if (Client.AccountNumber == AccountNumber) {

            FClient = Client;
            return true;

        }
    }

    return false;

}

bool MarkClientByAccountNumberForDelete(string AccountNumber, vector <stClient>& vClients) {

    for (stClient& Client : vClients) {

        if (Client.AccountNumber == AccountNumber) {

            Client.MarkForDelete = true;
            return true;

        }

    }

    return false;

}

bool DeleteClientByAccountNumber(string AccountNumber, vector <stClient>& vClients) {

    stClient Client;
    char Choice = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client)) {

        cout << "\nDelete Client : \n\n";

        PrintClientCard(Client);

        cout << "\nDo you want to delete this client ? y/n ?\n";
        cin >> Choice;

        if (Choice == 'Y' || Choice == 'y') {

            MarkClientByAccountNumberForDelete(AccountNumber, vClients);
            SaveClientsDataToFile(ClientsFile, vClients);

            vClients = LoadClientsDataFromFile(ClientsFile);

            cout << "\nClient deleted successfuly.\n";
            return true;

        }

    }

    else {

        cout << "\nClient with account number " << AccountNumber << " is not found!\n";
        return false;

    }

}

void ShowDeleteClientScreen() {

    if (!CheckPermissionAccess(enPermissions::ePDeleteClient)) {

        ShowAccessDeniedMessage();
        return;

    }

    cout << "--------------------------------------\n";
    cout << "\tDelete Client\n";
    cout << "--------------------------------------\n";

    vector <stClient> vClients = LoadClientsDataFromFile(ClientsFile);
    string AccountNumber = ReadClientAccountNumber();
    DeleteClientByAccountNumber(AccountNumber, vClients);

}

string ReadUserUserName() {

    string UserName = "";

    cout << "\nEnter Username ? ";
    cin >> UserName;

    return UserName;

}

void PrintUserCard(stUser User) {

    cout << "\nThe following are user detail : \n";

    cout << "--------------------------------------";
    cout << "\nUserName : " << User.UserName;
    cout << "\nPassword : " << User.Password;
    cout << "\nPermissions : " << User.Permissions;
    cout << "\n--------------------------------------\n";

}

bool FindUserByUserName(string UserName, vector <stUser> vUsers, stUser& CurrentUser) {

    for (stUser& User : vUsers) {

        if (User.UserName == UserName) {

            CurrentUser = User;
            return true;

        }

    }

    return false;


}

bool MarkUserByUserNameForDelete(string UserName, vector <stUser>& vUsers) {

    for (stUser& User : vUsers) {

        if (User.UserName == UserName) {

            User.MarkForDelete = true;
            return true;

        }

    }

    return false;

}

bool DeleteUserByUserName(string UserName, vector <stUser>& vUsers) {

    stUser User;
    char Choice = 'n';

    if (FindUserByUserName(UserName, vUsers, User)) {

        if (User.UserName == "Admin") {

            cout << "\nYou Cannot Delete This User.\n";
            return false;

        }

        cout << "\nDelete User : \n\n";

        PrintUserCard(User);

        cout << "\nDo you want to delete this user ? y/n ?\n";
        cin >> Choice;

        if (Choice == 'Y' || Choice == 'y') {

            MarkUserByUserNameForDelete(UserName, vUsers);
            SaveUsersDataToFile(UsersFile, vUsers);

            vUsers = LoadUsersDataFromFile(UsersFile);

            cout << "\nClient deleted successfuly.\n";
            return true;

        }

    }

    else {

        cout << "\nClient with username " << UserName << " is not found!\n";
        return false;

    }

}

void ShowDeleteUserScreen() {

    cout << "--------------------------------------\n";
    cout << "\tDelete User\n";
    cout << "--------------------------------------\n";

    vector <stUser> vUsers = LoadUsersDataFromFile(UsersFile);
    string UserName = ReadUserUserName();
    DeleteUserByUserName(UserName, vUsers);

}

stUser ChangeUserRecord(string UserName) {

    stUser User;

    User.UserName = UserName;

    cout << "Enter Password ? ";
    cin >> User.Password;

    User.Permissions = ReadPermissions();

    return User;

}

bool UpdateUserByUserName(string UserName, vector <stUser>& vUsers) {

    stUser User;
    char Choice = 'n';

    if (FindUserByUserName(UserName, vUsers, User)) {

        PrintUserCard(User);

        cout << "\nDo you want to update this user ? y/n ? ";
        cin >> Choice;

        if (Choice == 'Y' || Choice == 'y') {

            for (stUser& User : vUsers) {

                if (User.UserName == UserName) {

                    User = ChangeUserRecord(UserName);
                    break;

                }

            }

            SaveUsersDataToFile(UsersFile, vUsers);

            cout << "\nUser Updated successfuly.\n";
            return true;

        }

    }

    else {

        cout << "\nUser with username " << UserName << " is not found!\n";
        return false;

    }

}

void ShowUpdateUserScreen() {

    cout << "--------------------------------------\n";
    cout << "\tUpdate User Information\n";
    cout << "--------------------------------------\n";

    vector <stUser> vUsers = LoadUsersDataFromFile(UsersFile);
    string UserName = ReadUserUserName();
    UpdateUserByUserName(UserName, vUsers);

}

stClient ChangeClientRecord(string AccountNumber) {

    stClient Client;

    Client.AccountNumber = AccountNumber;

    cout << "Enter PIN Code ? ";
    getline(cin >> ws, Client.PINCode);

    cout << "Enter Name ? ";
    getline(cin, Client.Name);

    cout << "Enter Phone ? ";
    getline(cin, Client.Phone);

    cout << "Enter Account Balance ? ";
    cin >> Client.AccountBalance;

    return Client;

}

bool UpdateClientByAccountNumber(string AccountNumber, vector <stClient>& vClients) {

    stClient Client;
    char Choice = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client)) {

        PrintClientCard(Client);

        cout << "\nDo you want to update this client ? y/n ?\n";
        cin >> Choice;

        if (Choice == 'Y' || Choice == 'y') {

            for (stClient& Client : vClients) {

                if (Client.AccountNumber == AccountNumber) {

                    Client = ChangeClientRecord(AccountNumber);
                    break;

                }

            }

            SaveClientsDataToFile(ClientsFile, vClients);

            cout << "\nClient Updated successfuly.\n";
            return true;

        }

    }

    else {

        cout << "\nClient with account number " << AccountNumber << " is not found!\n";
        return false;

    }

}

void ShowUpdateClientScreen() {

    if (!CheckPermissionAccess(enPermissions::ePUpdateClientInfo)) {

        ShowAccessDeniedMessage();
        return;

    }

    cout << "--------------------------------------\n";
    cout << "\tUpdate Client Information\n";
    cout << "--------------------------------------\n";

    vector <stClient> vClients = LoadClientsDataFromFile(ClientsFile);
    string AccountNumber = ReadClientAccountNumber();
    UpdateClientByAccountNumber(AccountNumber, vClients);


}

void ShowFindUserScreen() {

    cout << "--------------------------------------\n";
    cout << "\tFind User\n";
    cout << "--------------------------------------\n";

    vector <stUser> vUsers = LoadUsersDataFromFile(UsersFile);
    string UserName = ReadUserUserName();
    stUser User;

    if (FindUserByUserName(UserName, vUsers, User))

        PrintUserCard(User);

    else

        cout << "\nUser with username " << UserName << " is not found!\n";

}

void ShowFindClientScreen() {

    if (!CheckPermissionAccess(enPermissions::ePFindClient)) {

        ShowAccessDeniedMessage();
        return;

    }

    cout << "--------------------------------------\n";
    cout << "\tFind Client\n";
    cout << "--------------------------------------\n";

    vector <stClient> vClients = LoadClientsDataFromFile(ClientsFile);
    string AccountNumber = ReadClientAccountNumber();
    stClient Client;

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))

        PrintClientCard(Client);

    else

        cout << "\nClient with account number " << AccountNumber << " is not found!\n";

}

double ReadAmount() {

    float DepositAmount = 0;

    cin >> DepositAmount;

    return DepositAmount;

}

bool DepositToClientByAccountNumber(string AccountNumber, double DepositAmount, vector <stClient>& vClients) {

    char Choice = 'n';

    cout << "\nDo you want to preform this transaction ? y/n ?\n";
    cin >> Choice;

    if (Choice == 'Y' || Choice == 'y') {

        for (stClient& Client : vClients) {

            if (Client.AccountNumber == AccountNumber) {

                Client.AccountBalance += DepositAmount;
                break;

            }

        }

        SaveClientsDataToFile(ClientsFile, vClients);

        cout << "\nThe transaction is done successfuly.\n";
        return true;

    }

    return false;

}

void ShowDespositScreen() {

    cout << "--------------------------------------\n";
    cout << "\tDeposit Screen\n";
    cout << "--------------------------------------\n";

    vector <stClient> vClients = LoadClientsDataFromFile(ClientsFile);
    string AccountNumber = ReadClientAccountNumber();
    stClient Client;

    while (!FindClientByAccountNumber(AccountNumber, vClients, Client)) {

        cout << "Client with account number " << AccountNumber << " is not exists!\n";
        AccountNumber = ReadClientAccountNumber();

    }

    PrintClientCard(Client);

    double DepositAmount = 0;

    cout << "\nPlease Enter Deposit Amount : ";
    DepositAmount = ReadAmount();

    DepositToClientByAccountNumber(AccountNumber, DepositAmount, vClients);

}

void ShowWithdrawScreen() {

    cout << "--------------------------------------\n";
    cout << "\tWithdraw Screen\n";
    cout << "--------------------------------------\n";

    vector <stClient> vClients = LoadClientsDataFromFile(ClientsFile);
    string AccountNumber = ReadClientAccountNumber();
    stClient Client;

    while (!FindClientByAccountNumber(AccountNumber, vClients, Client)) {

        cout << "Client with account number " << AccountNumber << " is not exists!\n";
        AccountNumber = ReadClientAccountNumber();

    }

    PrintClientCard(Client);

    cout << "\nPlease Enter Withdraw Amount : ";
    double WithdrawAmount = ReadAmount();

    while (WithdrawAmount > Client.AccountBalance)
    {
        cout << "\nClient with account number " << AccountNumber << " dose not have enough money!, you can only withdraw up to : " << Client.AccountBalance << "\n";
        cout << "\nPlease Enter Withdraw Amount : ";
        WithdrawAmount = ReadAmount();

    }

    DepositToClientByAccountNumber(AccountNumber, WithdrawAmount * -1, vClients);

}

double TotalBalancesForAllClients(vector <stClient> vClients) {

    double TotalBalances = 0;

    for (stClient& Client : vClients) {

        TotalBalances += Client.AccountBalance;

    }

    return TotalBalances;

}

void ShowTotalBalances() {

    vector <stClient> vClients = LoadClientsDataFromFile(ClientsFile);

    cout << "\n\t\t\t Balances list (" << vClients.size() << ") Client(s). \n\n";

    cout << "_________________________________________________________________________________________\n\n";
    cout << "| " << left << setw(16) << "Account Number";
    cout << "| " << left << setw(10) << "PIN Code";
    cout << "| " << left << setw(25) << "Client Name";
    cout << "| " << left << setw(12) << "Phone";
    cout << "| " << left << setw(12) << "Account Balance";
    cout << "\n_________________________________________________________________________________________\n\n";

    if (vClients.size() != 0) {

        for (stClient& Client : vClients) {

            PrintClientData(Client);
            cout << endl;

        }

    }

    else {

        cout << "No Clients Available In The System.\n";

    }

    cout << "_________________________________________________________________________________________\n";

    cout << "\n\t\t\t Total Balances = " << TotalBalancesForAllClients(vClients) << "\n\n";

}

void ShowTotalBalancesScreen() {

    ShowTotalBalances();

}

void BackToTransactionMenu() {

    cout << "\nPress Any Key To Go Back To Main Menu...";
    system("pause>0");
    ShowTransactionsMenuScreen();

}

enTransactionsMenuOptions ReadTransactionsMenuOption(short From, short To) {

    short Option = 0;

    do {

        cout << "\nChoose What Do You Want To Do [" << From << " To " << To << "] : ";
        cin >> Option;

    } while (Option < From || Option > To);

    return (enTransactionsMenuOptions)Option;

}

void PreformTransactionsMenuOption(enTransactionsMenuOptions Option) {

    switch (Option)
    {

    case eDeposit:

        ClearScreen();
        ShowDespositScreen();
        BackToTransactionMenu();
        break;

    case eWithdraw:

        ClearScreen();
        ShowWithdrawScreen();
        BackToTransactionMenu();
        break;

    case eShowTotalBalances:

        ClearScreen();
        ShowTotalBalancesScreen();
        BackToTransactionMenu();
        break;

    case eShowMainMenu:

        ClearScreen();
        ShowMainMenuScreen();
        break;

    }

}

void ShowTransactionsMenuScreen() {

    if (!CheckPermissionAccess(enPermissions::ePTransactions)) {

        ShowAccessDeniedMessage();
        return;
    }

    ClearScreen();

    cout << "==================================================================\n";
    cout << "\t\t\tTransactions Menu Screen\n";
    cout << "==================================================================\n";

    cout << "\t[1] Deposit.\n";
    cout << "\t[2] Withdraw.\n";
    cout << "\t[3] Total Balances.\n";
    cout << "\t[4] Main Menu.\n";

    cout << "==================================================================\n";

    PreformTransactionsMenuOption(ReadTransactionsMenuOption(1, 4));

}

void BackToManageUsersMenu() {

    cout << "\nPress Any Key To Go Back To Manage Users Menu...";
    system("pause>0");
    ShowManageUsersMenuScreen();

}

enManageUsersMenuOptions ReadManageUsersMenuOption(short From, short To) {

    short Option = 0;

    do {

        cout << "\nChoose What Do You Want To Do [" << From << " To " << To << "] : ";
        cin >> Option;

    } while (Option < From || Option > To);

    return (enManageUsersMenuOptions)Option;

}

void PreformManageUsersMenuOption(enManageUsersMenuOptions Option) {

    switch (Option)
    {
    case eListUsers:

        ClearScreen();
        ShowAllUsers();
        BackToManageUsersMenu();

        break;

    case eAddNewUser:

        ClearScreen();
        ShowAddNewUsersScreen();
        BackToManageUsersMenu();

        break;

    case eDeleteUser:

        ClearScreen();
        ShowDeleteUserScreen();
        BackToManageUsersMenu();

        break;

    case eUpdateUserInfo:

        ClearScreen();
        ShowUpdateUserScreen();
        BackToManageUsersMenu();

        break;

    case eFindUser:

        ClearScreen();
        ShowFindUserScreen();
        BackToManageUsersMenu();

        break;

    case eMainMenu:

        ClearScreen();
        ShowMainMenuScreen();
        break;

    }

}

void ShowManageUsersMenuScreen() {

    if (!CheckPermissionAccess(enPermissions::ePManageUsers)) {

        ShowAccessDeniedMessage();
        return;

    }

    ClearScreen();

    cout << "==================================================================\n";
    cout << "\t\t\tManage Users Menu Screen\n";
    cout << "==================================================================\n";

    cout << "\t[1] List Users.\n";
    cout << "\t[2] Add New User.\n";
    cout << "\t[3] Delete User.\n";
    cout << "\t[4] Update User Info.\n";
    cout << "\t[5] Find User.\n";
    cout << "\t[6] Main Menu.\n";


    cout << "==================================================================\n";

    PreformManageUsersMenuOption(ReadManageUsersMenuOption(1, 6));

}

void BackToMainMenu() {

    cout << "\nPress Any Key To Go Back To Main Menu...";
    system("pause>0");
    ShowMainMenuScreen();

}

enMainMenuOptions ReadMainMenuOption(short From, short To) {

    short Option = 0;

    do {

        cout << "\nChoose What Do You Want To Do [" << From << " To " << To << "] : ";
        cin >> Option;

    } while (Option < From || Option > To);

    return (enMainMenuOptions)Option;

}

void PreformMainMenuOption(enMainMenuOptions Option) {

    switch (Option)
    {

    case eShowClientList:

        ClearScreen();
        ShowAllClients();
        BackToMainMenu();
        break;

    case eAddNewClient:

        ClearScreen();
        ShowAddNewClientsScreen();
        BackToMainMenu();
        break;

    case eDeleteClient:

        ClearScreen();
        ShowDeleteClientScreen();
        BackToMainMenu();
        break;

    case eUpdateClient:

        ClearScreen();
        ShowUpdateClientScreen();
        BackToMainMenu();
        break;

    case eFindClient:

        ClearScreen();
        ShowFindClientScreen();
        BackToMainMenu();
        break;

    case eShowTransactionsMainMenu:

        ClearScreen();
        ShowTransactionsMenuScreen();
        break;

    case eManageUsers:

        ClearScreen();
        ShowManageUsersMenuScreen();
        break;

    case eLogout:

        ClearScreen();
        Login();
        break;

    }

}

void ClearScreen() {

    system("cls");

}

void ShowMainMenuScreen() {

    system("color 0F");
    ClearScreen();

    cout << "==================================================================\n";
    cout << "\t\t\tMain Menu Screen\n";
    cout << "==================================================================\n";

    cout << "\t[1] Show Client List.\n";
    cout << "\t[2] Add New Client.\n";
    cout << "\t[3] Delete Client.\n";
    cout << "\t[4] Update Client Information.\n";
    cout << "\t[5] Find Client.\n";
    cout << "\t[6] Transactions.\n";
    cout << "\t[7] Manage Users.\n";
    cout << "\t[8] Logout.\n";


    cout << "==================================================================\n";

    PreformMainMenuOption(ReadMainMenuOption(1, 8));

}

bool FindUserByUserNameAndPassword(string UserName, string Password, stUser& CurrentUser) {

    vector <stUser> vUsers = LoadUsersDataFromFile(UsersFile);

    for (stUser& User : vUsers) {

        if (User.UserName == UserName && User.Password == Password) {

            CurrentUser = User;
            return true;

        }

    }

    return false;

}

void SetScreenColor(bool Answer) {

    if (!Answer) {
        system("color 2F");
    }
    else {
        system("color 4F");
        cout << "\a";
    }

}

void ShowLoginScreen() {

    cout << "--------------------------------------\n";
    cout << "\t Login Screen\n";
    cout << "--------------------------------------\n";

}

bool LoadUserInfo(string UserName, string Password) {

    if (FindUserByUserNameAndPassword(UserName, Password, CurrentUser))
       
        return true;

    else

        return false;

}

void Login() {

    bool LoginFaild = false;

    string UserName = "", Password = "";
    

    do {

        ClearScreen();
        ShowLoginScreen();

        if (LoginFaild) {
           
            SetScreenColor(LoginFaild);
            cout << "Invalid Username/Password!\n";

        }

        cout << "\nEnter Username ? ";
        cin >> UserName;

        cout << "\nEnter Password ? ";
        cin >> Password;

        LoginFaild = !(LoadUserInfo(UserName, Password));


    } while (LoginFaild);
    
    SetScreenColor(LoginFaild);
    ShowMainMenuScreen();

}

int main()
{

    Login();

    return 0;
}

