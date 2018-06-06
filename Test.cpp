#include <iostream>
#include <Windows.h>
#include <conio.h>

using namespace std;

void Menu(bool& a, bool& b);

int main()
{

	setlocale(LC_ALL, "Rus");

	int inc1 = 0, 
		inc2 = 0;
	bool yes = true;
	bool Y = true;
	 
	while(Y)
	{
		while (yes)
		{
			if (inc1 == 10)
			{
				inc2 = inc1;
				yes = false;

			}
			cout << inc1 << " " << inc2 << endl;
			inc1++;
			Sleep(300);

		}
		inc1 = 0;
		cout << "2 цикл" << endl;
		
		Sleep(300);
		
		
		Menu(yes,Y);
	
	}

	system("PAUSE");
	return 0;
}

void Menu(bool &a, bool &b)
{

	switch (_getch())
	{
	case 'c':a = true;
		break;
	case 'q':b = false;
		break;
	}
}