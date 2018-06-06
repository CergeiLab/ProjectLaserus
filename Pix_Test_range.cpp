#include <iostream>
#include <ctime>
#include <iomanip>
using namespace std;

//Дописать функцию подсчета пикселей не от края, а от центра матрицы.
//если j<(b/2) то (b/2)-j || если j>(b/2) то j-(b/2)*****************
//Получем количество пикселей от центра до целевого пикселя.
//*******************************************************************

void find_point(int &a, int &b, int** &c)
{
	int x;
	int num = 0;
	cout << endl;
	for (int i = 0; i < a; i++)
	{
		for (int j = 0; j < b; j++)
		{
			if (c[i][j] == 255 || c[i][j] == 254) // искомое значение пикселя
			{
				if (j<(b / 2))
				{
					x = (b / 2) - j;
				}
				if (j>(b / 2))
				{
					x = j - (b / 2);
				}

				cout << "Точки: " << num << " i= " << i << " " << "j= " << j << " Значение пикселя: (" << c[i][j] << ")" << " X= " << x << endl;
				num++;
			}

		}
	}
}


int main()
{
	srand(time(NULL));
	setlocale(LC_ALL, "RUS");

	int value_a;
	int value_b;
	int ext = 1;
	while (ext)
	{
		cin >> value_a;
		cin >> value_b;

		int **arr = new int*[value_a];
		for (int count = 0; count < value_a; count++)
			arr[count] = new int[value_b];

		for (int i = 0; i < value_a; i++)
		{
			for (int j = 0; j < value_b; j++)
			{
				arr[i][j] = rand() % 256;
				if (arr[i][j] != 255 & arr[i][j] != 254)
				{
					arr[i][j] = 0;
				}
			}
		}


		for (int i = 0; i < value_a; i++)
		{
			for (int j = 0; j < value_b; j++)
			{
				cout << setw(4) << arr[i][j];
				if (j == (value_b - 1))
				{
					cout << endl;
				}

			}
		}
		find_point(value_a, value_b, arr);

		for (int count = 0; count < value_a; count++)
			delete[]arr[count];

		cout << "Продолжить (1) ДА " << "(0) Нет " << endl;
		cin >> ext;
	}


	//system("PAUSE");
	return 0;
}