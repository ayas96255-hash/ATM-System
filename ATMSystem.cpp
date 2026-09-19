#include <iostream>
#include<string>
#include<vector>
#include<cctype>
#include<iomanip>
#include<algorithm>
#include<fstream>
using namespace std;
void Login();
void ATMMainManueScreen();
void QuickWithdraw();
const string ClientsFileName = "Clients.txt";
struct sClient
{
	string AccountNumber;
	string PinCode;
	string Name;
	string Phone;
	float AccountBalance;
	bool MarkForDelete = false;

};
sClient CurrentClient;
vector<string> SplitString(string S1, string Delim)
{
	string sWord;
	short pos = 0;
	int count = 0;
	vector<string>vString;
	while ((pos = S1.find(Delim)) != std::string::npos)
	{
		sWord = S1.substr(0, pos);
		if (sWord != "")
		{
			vString.push_back(sWord);
			count++;
		}
		S1.erase(0, pos + Delim.length());
	}
	if (S1 != Delim)
	{
		vString.push_back(S1);
		count++;
	}
	//cout << "Tokens: " << count << endl;
	return vString;
}
string TrimLeft(string S1)
{
	for (int i = 0;i < S1.length();i++)
	{
		if (S1[i] != ' ')
		{
			return S1.substr(i, S1.length() - i);
		}
	}
	return "";
}
string TrimRight(string S1)
{
	for (int i = S1.length() - 1;i >= 0;i--)
	{
		if (S1[i] != ' ')
		{
			return 	S1.substr(0, i + 1);

		}
	}
	return "";
}
string Trim(string S1)
{
	return TrimLeft(TrimRight(S1));
}
string ConvertRecordToLine(sClient Client, string Separator = "#//#")
{
	string stClientRecord = "";
	stClientRecord += Client.AccountNumber + Separator;
	stClientRecord += Client.PinCode + Separator;
	stClientRecord += Client.Name + Separator;
	stClientRecord += Client.Phone + Separator;
	stClientRecord += to_string(Client.AccountBalance);
	return stClientRecord;
}
sClient ConvertLineToRecord(string S1, string Separator = "#//#")
{
	sClient Client;
	vector<string> vClient = SplitString(S1, Separator);
	if (vClient.size() >= 5)
	{
		Client.AccountNumber = vClient[0];
		Client.PinCode = vClient[1];
		Client.Name = vClient[2];
		Client.Phone = vClient[3];
		Client.AccountBalance = stod(vClient[4]);
	}
	return Client;
}
bool ClientExistsByAccountNumber(string AccountNumber, string FileName)
{
	vector<sClient>vClient;
	fstream client;
	client.open(ClientsFileName, ios::in);
	if (client.is_open())
	{
		string Line;
		sClient Client;
		while (getline(client, Line))
		{
			Client = ConvertLineToRecord(Line);
			if (Client.AccountNumber == AccountNumber)
			{
				client.close();
				return true;
			}
			client.close();
		}
	}
	return false;
}
sClient ReadeNewClient()
{
	sClient Client;
	cout << "\nPlease Enter Client Data:\n";
	cout << "Enter Account Number? ";
	getline(cin >> ws, Client.AccountNumber);
	while (ClientExistsByAccountNumber(Client.AccountNumber, ClientsFileName))
	{
		cout << "\nClient with [" << Client.AccountNumber << "] already exists,Enter another Account Number: ";
		getline(cin >> ws, Client.AccountNumber);
	}
	cout << "Enter PinCode? ";
	getline(cin, Client.PinCode);
	cout << "Enter Name? ";
	getline(cin, Client.Name);
	cout << "Enter Phone? ";
	getline(cin, Client.Phone);
	cout << "Enter Account Balance? ";
	cin >> Client.AccountBalance;

	return Client;
}
void PrintClient(sClient Client)
{
	cout << "\n-----------------------------------------------------";
	cout << "\nThe following is the extracted client record:\n";
	cout << "Account Number : " << Client.AccountNumber << endl;
	cout << "PinCode        : " << Client.PinCode << endl;
	cout << "Name           : " << Client.Name << endl;
	cout << "Phone          : " << Client.Phone << endl;
	cout << "Account Balance: " << Client.AccountBalance << endl;
	cout << "-----------------------------------------------------\n";
}
vector<sClient> LoadClientsDataFromFile(string FileName)
{
	sClient Client;
	vector<sClient>vClient;
	fstream client;
	client.open(ClientsFileName, ios::in);
	string Line;
	if (client.is_open())
	{
		while (getline(client, Line))
		{
			if (Line == " ")
				continue;
			Client = ConvertLineToRecord(Line);
			vClient.push_back(Client);
		}
		client.close();
	}
	return vClient;
}
bool FindClient(string AccountNumber,string PinCode, vector<sClient>& vClient, sClient& Client)
{
	//vClient = LoadClientsDataFromFile(ClientsFileName);
	for (sClient C : vClient)
	{

		if (Trim(C.AccountNumber) == AccountNumber && Trim(C.PinCode)==PinCode)
		{
			Client = C;
			return true;
		}
	}
	return false;

}
vector<sClient>SaveClientsDataToFile(string FileName, vector<sClient>& vClient)
{

	fstream client;
	string Line;

	client.open(ClientsFileName, ios::out);
	if (client.is_open())
	{
		for (sClient& C : vClient)
		{
			if (C.MarkForDelete == false)
			{
				C.AccountNumber = Trim(C.AccountNumber);
				C.Name = Trim(C.Name);
				Line = ConvertRecordToLine(C);
				client << Line << endl;
			}
		}
		client.close();
	}
	return vClient;
}
string ReadAccountNumber()
{
	string AccountNumber;
	cout << "\nEnter AccountNNumber: ";
	cin >> AccountNumber;
	return AccountNumber;
	
}
string ReadPinCode()
{
	string PinCode;
	cout << "Enter PinCode: ";
	cin >>PinCode;
	return PinCode;
}
int ReadChoose()
{
	int Choose;
	cout << "\nChoose what do you want to do ? [1 to 5 ]: ";
	cin >> Choose;
	return Choose;
}
enum enATMMainManue{ eQuikWithdraw=1,eNormalWithdraw=2,eDeposit=3,eCkeckBalance=4,eLogout=5};
void CheckBalance(sClient & Client )
{
	cout << "Your Balance is  : "<<Client.AccountBalance;

}
void ShowBalance()
{
	system("cls");
	cout << "\n===================================================================\n";
	cout << "\t\t" << "Check Balance Screen";
	cout << "\n===================================================================\n";
	CheckBalance(CurrentClient);
}
void GoBackToMainMenu()
{
	cout << "\nPress any key to go back to main menu...";
	system("pause>nul");
	ATMMainManueScreen();
}
enum enWithDraw{ e20=1,e50=2,e100=3,e200=4,e400=5,e600=6,e800=7,e1000=8,eExit=9};
int ReadWithdraw()
{
	int Choose;
	cout << "\nChoose what to Withdraw from [1] to [9] ? ";
	cin >> Choose;
	return Choose;
}
short GetQuickWithDrawAmount(short QuickWithDrawOption)
{
	switch (QuickWithDrawOption)
	{
	case 1:
		return 20;
		
	case2:
		return 50;
	case 3:
		return 100;
	case 4:
		return 200;
	case 5:
		return 400;
	case 6:
		return 600;
	case 7:
		return 800;
	case 8:
		return 1000;
	default :
		ATMMainManueScreen();
		return 0;
	}
}
void PreWithDraw(short Option)
{
	if (Option == 9)
		return;
	short Amount = GetQuickWithDrawAmount(Option);
	if (Amount>CurrentClient.AccountBalance)
	{
		cout << "\nThe amount exceeds your balance, make another choice.\n ";
		system("pause");
		QuickWithdraw();
		return;
	}
	char Sure;
	cout << "\n Are you sure you want perfrom this transaction ? y/n ? ";
	cin >> Sure;
	if (tolower(Sure) =='y')
	{
		
		CurrentClient.AccountBalance= CurrentClient.AccountBalance-Amount;
		vector<sClient>vClient = LoadClientsDataFromFile(ClientsFileName);
		for (sClient & C:vClient)
		{
			if (C.AccountNumber == CurrentClient.AccountNumber)
			{
				C.AccountBalance = CurrentClient.AccountBalance;
				break;
			}
			
		}
		SaveClientsDataToFile(ClientsFileName, vClient);
		cout << "Done Successfully.Now balance is " << CurrentClient.AccountBalance;
	}
	

}
void PreNormalWithDraw()
{
	cout << "\n===================================================================\n";
	cout << "\t\t" << "Normal Withdraw Screen";
	cout << "\n===================================================================\n";
	int Amount;
	do
	{
		cout << "\nEnter an amount multiple of 5s ? ";
		cin >> Amount;
		if (Amount > CurrentClient.AccountBalance)
		{
			cout << "\nThe amount exceeds your balance, make another choice.\n ";
			system("pause");
			system("cls");
			PreNormalWithDraw();

			return;
		}
	} while (Amount%5!=0);
	char Sure;
	cout << "\n Are you sure you want perfrom this transaction ? y/n ? ";
	cin >> Sure;
	if (tolower(Sure) == 'y')
	{

		CurrentClient.AccountBalance = CurrentClient.AccountBalance - Amount;
		vector<sClient>vClient = LoadClientsDataFromFile(ClientsFileName);
		for (sClient& C : vClient)
		{
			if (C.AccountNumber == CurrentClient.AccountNumber)
			{
				C.AccountBalance = CurrentClient.AccountBalance;
				break;
			}

		}
		SaveClientsDataToFile(ClientsFileName, vClient);
		cout << "Done Successfully.Now balance is " << CurrentClient.AccountBalance;
	}

}
void Deposit()
{
	cout << "\n===================================================================\n";
	cout << "\t\t" << "Deposit Screen";
	cout << "\n===================================================================\n";
	int Amount;
	do
	{
		cout << "\nEnter a positive Deposite amount ? ";
		cin >> Amount;		
	} while (Amount < 0);
	char Sure;
	cout << "\n Are you sure you want perfrom this transaction ? y/n ? ";
	cin >> Sure;
	if (tolower(Sure) == 'y')
	{

		CurrentClient.AccountBalance = CurrentClient.AccountBalance + Amount;
		vector<sClient>vClient = LoadClientsDataFromFile(ClientsFileName);
		for (sClient& C : vClient)
		{
			if (C.AccountNumber == CurrentClient.AccountNumber)
			{
				C.AccountBalance = CurrentClient.AccountBalance;
				break;
			}

		}
		SaveClientsDataToFile(ClientsFileName, vClient);
		cout << "Done Successfully.Now balance is " << CurrentClient.AccountBalance;
	}

}
void QuickWithdraw()
{
	cout << "\n===================================================================\n";
	cout << "\t\t" << "Quick Withdraw";
	cout << "\n===================================================================\n";
	cout << "\t " << "[1] 20 " << "\t" << "[2] 50 \n";
	cout << "\t " << "[3] 100 " << "\t" << "[4] 200 \n";
	cout << "\t " << "[5] 400 " << "\t" << "[6] 600 \n";
	cout << "\t " << "[7] 800 " << "\t" << "[8] 1000 \n";
	cout << "\t " << "[9] Exit ";
	cout << "\n===================================================================\n";
	cout << "\nYour balance is: " << CurrentClient.AccountBalance << endl;
	PreWithDraw(ReadWithdraw());
}
void PreFromATMOptions(enATMMainManue ATMOptions)
{
	switch (ATMOptions)
	{
	case enATMMainManue::eQuikWithdraw:
		system("cls");
		QuickWithdraw();
		GoBackToMainMenu();
		break;
	case enATMMainManue::eNormalWithdraw:
		system("cls");
		PreNormalWithDraw();
		GoBackToMainMenu();
	    break;
	case enATMMainManue::eDeposit:
		system("cls");
		Deposit();
		GoBackToMainMenu();
	    break;
	case enATMMainManue::eCkeckBalance:
		system("cls");
		ShowBalance();
		GoBackToMainMenu();
	    break;
	case enATMMainManue::eLogout:
		system("cls");
		Login();
		GoBackToMainMenu();
	    break;
	
	}
}
void ATMMainManueScreen()
{
	system("cls");
	cout << "\n===================================================================\n";
	cout << "\t\t" << "ATM Main Manue Screen";
	cout << "\n===================================================================\n";
	cout << " \t" << "[1] Quick Withdraw.\n ";
	cout << " \t" << "[2] Normal Withdraw.\n ";
	cout << " \t" << "[3] Deposit.\n ";
	cout << " \t" << "[4] Check Balance.\n ";
	cout << " \t" << "[5] Logout.\n ";
	cout << "\n===================================================================\n";
	PreFromATMOptions(enATMMainManue(ReadChoose()));
}
bool CheckClient()
{
	sClient Client;
	vector<sClient>vClient=LoadClientsDataFromFile(ClientsFileName);
	string AccountNumber = ReadAccountNumber();
	string PinCode = ReadPinCode();
	if (FindClient(AccountNumber,PinCode , vClient, CurrentClient))
	{
		ATMMainManueScreen();
		return true;
	}
	else
	{
		return false;
	}

}
void Login()
{
	bool LoginFailed = false;
	cout << "\n===================================================================\n";
	cout << "\t\t" << "Login Screen";
	cout << "\n===================================================================\n";
	do
	{
		if (LoginFailed)
		{
			cout << "\nInvalid AccountNumber/PinCode..";
			/*system("cls");*/
		}
		LoginFailed = !CheckClient();
	} while (LoginFailed);
}
int main()
{
	Login();
}

