#include <iostream>
#include <windows.h>
#include <conio.h>

using namespace std;

int main(int argc, char* argv[]) {

	setlocale(LC_ALL, "rus");
	//int n = 0;
	//while (n <= 5)
	//{
	//	int result = 1 << n; // Вычисляем 2 в степени n
	//	_cprintf("2^%d = %d\n", n, result);
	//	n++;
	//	Sleep(500);
	//}
	int value = atoi(argv[1]);
	int hundreds = value / 100;
	int tens = (value / 10) % 10;
	int units = value % 10;

	if (value % 2 != 0)
		std::cout << "Полученное значение: " << value << " (нечетное)" << std::endl;
	std::cout << "Полученное значение: " << value << " (четное)" << std::endl;
	// Выводим значения сотен, десятков и единиц
	cout << "Сотни: " << hundreds << endl;
	cout << "Десятки: " << tens << endl;
	cout << "Единицы: " << units << endl;


	HANDLE hThread;
	char c;

	// преобразуем символьное представление дескриптора в число
	hThread = (HANDLE)atoi(argv[1]);

	// ждем команды о завершении потока
	while (true) {
		std::cout << "\nВведите 't' для завершения процесса: ";
		c = _getch();
		if (c == 't') {
			std::cout << "t\n";
			break;
		}
	}

	TerminateThread(hThread, 0);
	CloseHandle(hThread);
	return 0;
}