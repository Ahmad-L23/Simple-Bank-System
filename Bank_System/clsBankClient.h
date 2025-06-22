#pragma once
#include <iostream>
#include <string>
#include "clsPerson.h"
#include "clsString.h"
#include <vector>
#include <fstream>

class clsBankClient : public clsPerson
{
private:
    enum enMode { EmptyMode = 0, UpdateMode = 1, AddNewMode = 2 };
	enMode _Mode;

	string _AccountNumber;
    string _PinCode;
    float _AccountBalance;
    bool _MarkedForDelete = false;


    static clsBankClient _ConvertLineToObject(string Line, string Separtor = "#//#")
    {
        vector<string> vClinet = clsString::Split(Line, Separtor);
        return clsBankClient(enMode::UpdateMode, vClinet[0], vClinet[1], vClinet[2], vClinet[3], vClinet[4], vClinet[5], stod(vClinet[6]));
    }

    static string _ConverClientObjectToLine(clsBankClient client, string Sperator = "#//#")
    {
        string stClientRecord = "";
        stClientRecord += client.FirstName + Sperator;
        stClientRecord += client.LastName + Sperator;
        stClientRecord += client.Email + Sperator;
        stClientRecord += client.Phone + Sperator;
        stClientRecord += client.AccountNumber() + Sperator;
        stClientRecord += client.PinCode + Sperator;
        stClientRecord += to_string(client.AccountBalance);

        return stClientRecord;
    }

    static clsBankClient _GetEmptyClientObject()
    {
        return clsBankClient(enMode::EmptyMode, "", "", "", "", "", "", 0);
    }

    static vector<clsBankClient> _LoadClientsDataFromFile()
    {
        vector<clsBankClient> _vClients;

        fstream MyFile("Clients.txt", ios::in);
        
        if (MyFile.is_open())
        {
            string Line = "";

            while (getline(MyFile, Line))
            {
                clsBankClient Client = _ConvertLineToObject(Line);
                _vClients.push_back(Client);
            }
            MyFile.close();
        }

        return _vClients;
    }


    void _SaveClientsDataToFile(vector<clsBankClient> Clients)
    {
        fstream MyFile("Clients.txt", ios::out);


        string DataLine = "";
        for (clsBankClient C : Clients)
        {
            if (C.MarkedForDeleted() == false)
            {
                //we only write records that are not marked for delete.  
                DataLine = _ConverClientObjectToLine(C);
                MyFile << DataLine << endl;

            }
        }
        MyFile.close();
    }

    void _Update()
    {
        vector<clsBankClient> _vClients = _LoadClientsDataFromFile();

        for (clsBankClient& Client : _vClients)
        {
            if (AccountNumber() == Client.AccountNumber())
            {
                Client = *this;
                break;
            }
        }
        _SaveClientsDataToFile(_vClients);

    }

    void _AddNew()
    {

        _AddDataLineToFile(_ConverClientObjectToLine(*this));
    }

    void _AddDataLineToFile(string  stDataLine)
    {
        fstream MyFile;
        MyFile.open("Clients.txt", ios::out | ios::app);

        if (MyFile.is_open())
        {

            MyFile << stDataLine << endl;

            MyFile.close();
        }

    }

public:
    clsBankClient(enMode Mode, string FirstName, string LastName,
        string Email, string Phone, string AccountNumber, string PinCode,
        float AccountBalance) :
        clsPerson(FirstName, LastName, Email, Phone)

    {
        _Mode = Mode;
        _AccountNumber = AccountNumber;
        _PinCode = PinCode;
        _AccountBalance = AccountBalance;

    }

	bool IsEmpty()
	{
		return (_Mode == enMode::EmptyMode);
	}

    bool MarkedForDeleted()
    {
        return _MarkedForDelete;
    }

    string AccountNumber()
    {
        return _AccountNumber;
    }

    void SetPinCode(string PinCode)
    {
        _PinCode = PinCode;
    }

    string GetPinCode()
    {
        return _PinCode;
    }
    __declspec(property(get = GetPinCode, put = SetPinCode)) string PinCode;

    void SetAccountBalance(float AccountBalance)
    {
        _AccountBalance = AccountBalance;
    }

    float GetAccountBalance()
    {
        return _AccountBalance;
    }
    __declspec(property(get = GetAccountBalance, put = SetAccountBalance)) float AccountBalance;

    void Print()
    {
        cout << "\nClinet Card   : ";
        cout << "\n-----------------------------";
        cout << "\nFirst Name   : " << FirstName;
        cout << "\nLast Name     : " << LastName;
        cout << "\nFull Name    : " << FullName();
        cout << "\nEmail         : " << Email;
        cout << "\nPhone         : " << Phone;
        cout << "\nAccount Number: " << _AccountNumber;
        cout << "\nPassword      : " << _PinCode;
        cout << "\nBalance       : " << _AccountBalance;
        cout << "\n-----------------------------\n";
    }


    static clsBankClient Find(string AccountNumber)
    {
        vector<clsBankClient>vClients;

        fstream MyFile;

        MyFile.open("Clients.txt", ios::in);

        if (MyFile.is_open())
        {
            string Line;
            while (getline(MyFile, Line))
            {
                clsBankClient Clinet = _ConvertLineToObject(Line);
                if (Clinet.AccountNumber() == AccountNumber)
                {
                    MyFile.close();
                    return Clinet;
                }
                vClients.push_back(Clinet);
            }
            MyFile.close();
        }
        return _GetEmptyClientObject();
    }

    static clsBankClient Find(string AccountNumber, string PinCode)
    {
        vector<clsBankClient>vClients;

        fstream MyFile;

        MyFile.open("Clients.txt", ios::in);

        if (MyFile.is_open())
        {
            string Line;
            while (getline(MyFile, Line))
            {
                clsBankClient Clinet = _ConvertLineToObject(Line);
                if (Clinet.AccountNumber() == AccountNumber && Clinet.GetPinCode() == PinCode)
                {
                    MyFile.close();
                    return Clinet;
                }
                vClients.push_back(Clinet);
            }
            MyFile.close();
        }
        return _GetEmptyClientObject();
    }

    static bool IsClientExist(string AccountNumber)
    {
        clsBankClient Client = Find(AccountNumber);
        return(!Client.IsEmpty());
    }


    bool Delete()
    {
        vector <clsBankClient> _vClients;
        _vClients = _LoadClientsDataFromFile();

        for (clsBankClient& C : _vClients)
        {
            if (C.AccountNumber() == _AccountNumber)
            {
                C._MarkedForDelete = true;
                break;
            }

        }

        _SaveClientsDataToFile(_vClients);

        *this = _GetEmptyClientObject();

        return true;

    }


    static clsBankClient GetAddNewClientObject(string AccountNumber)
    {
        return clsBankClient(enMode::AddNewMode, "", "", "", "", AccountNumber, "", 0);
    }

    enum enSaveResults{ svFaildEmptyObject = 0, svSucceeded = 1, svFaildAccountNumberExists = 2 };
    
    enSaveResults Save()
    {

        switch (_Mode)
        {
            case enMode::EmptyMode:
            {
                if (IsEmpty())
                {

                    return enSaveResults::svFaildEmptyObject;

                }

            }

            case enMode::UpdateMode:
            {


                _Update();

                return enSaveResults::svSucceeded;

                break;
            }

            case enMode::AddNewMode:
            {
                if (clsBankClient::IsClientExist(_AccountNumber))
                {
                    return enSaveResults::svFaildAccountNumberExists;
                }
                else
                {
                    _AddNew();

                    _Mode = enMode::UpdateMode;
                    return enSaveResults::svSucceeded;
                }

                break;
            }
        }

    }


    static vector <clsBankClient> GetClientsList()
    {
        return _LoadClientsDataFromFile();
    }

    static float GetTotalBalances()
    {
        vector <clsBankClient> vClients = clsBankClient::GetClientsList();

        double TotalBalances = 0;

        for (clsBankClient Client : vClients)
        {

            TotalBalances += Client.AccountBalance;
        }

        return TotalBalances;

    }


    void Deposit(double Amount)
    {
        _AccountBalance += Amount;
        Save();
    }

    bool Withdraw(double Amount)
    {
        if (Amount > _AccountBalance)
        {
            return false;
        }

        else
        {
            _AccountBalance -= Amount;
            Save();
        }
    }
};

