#pragma once

#include <iostream>
#include "clsScreen.h"
#include "clsUser.h"
#include <iomanip>
#include "clsMainScreen.h"
#include "Global.h"

class clsLoginScreen :protected clsScreen
{

private:

    static  void _Login()
    {
        bool LoginFaild = false;
        string Username, Password;
        do
        {

            if (LoginFaild)
            {
                cout << "\nInvlaid Username/Password!\n\n";
                cout << "you have " << Trial << " to login.\n\n";
            }
            if (Trial == 0)
            {
                cout << "You are Locked after 3 faild trials" << endl;
                return;
            }

            cout << "Enter Username? ";
            cin >> Username;

            cout << "Enter Password? ";
            cin >> Password;

            CurrentUser = clsUser::Find(Username, Password);

            LoginFaild = CurrentUser.IsEmpty();
            Trial--;

        } while (LoginFaild);

        CurrentUser.RegisterLogin();
        clsMainScreen::ShowMainMenue();

    }

public:


    static void ShowLoginScreen()
    {
        system("cls");
        _DrawScreenHeader("\t  Login Screen");
        _Login();

    }

};

