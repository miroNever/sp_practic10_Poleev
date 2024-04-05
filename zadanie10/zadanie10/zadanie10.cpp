#include <windows.h>
#include <conio.h>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

bool isPrime(int num) {
    if (num <= 1)
        return false;
    for (int i = 2; i * i <= num; ++i) {
        if (num % i == 0)
            return false;
    }
    return true;
}
DWORD WINAPI ThreadFunction1(LPVOID lpParam) {
    cout << "Поток создан." << endl;
    cout << "Простые числа до 50:" << endl;
    for (int i = 2; i <= 50; ++i) {
        if (isPrime(i)) {
            cout << i << " ";
        }
    }
    cout << endl;
    return 0;
}
// Функция для проверки, является ли число простым

DWORD WINAPI FindDifference(LPVOID lpParam) {
    setlocale(LC_ALL, "ru");
    int num1 = 1000;
    int num2 = 7;

    while (true) {
        int result = num1 - num2;
        if (result < 0) {
            cout << "Я проиграл?" << endl;
            return 1;
        }
        else {
            cout << num1 << " - " << num2 << " = " << result << endl;
            Sleep(50);
            // Уменьшаем num1 на num2 для следующей итерации
            num1 = result;
        }
    }
}

int main() {
    setlocale(LC_ALL, "ru");


    // Создаем структуру STARTUPINFO и PROCESS_INFORMATION
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    HANDLE hThread, hInheritProcess;
    DWORD IDThread;

    hThread = GetCurrentProcess();

    if (!DuplicateHandle(
        GetCurrentProcess(), // дескриптор текущего процесса
        hThread,            // исходный псевдодескриптор процесса
        GetCurrentProcess(), // дескриптор текущего процесса
        &hInheritProcess,    // новый дескриптор процесса
        0,                   // этот параметр игнорируется
        TRUE,                // новый дескриптор наследуемый
        DUPLICATE_SAME_ACCESS)) // доступ не изменяем
    {
        std::cerr << "Псевдодескриптор не был скопирован.\n";
        _cputs("Press any key to finish.\n");
        _getch();
        return GetLastError();
    }
    std::cout << "Создан наследуемый дубликат псевдодескриптора текущего процесса.\n";
    std::cout << "\nВведите любой знак для начала дочернего процесса: ";
    _getch();
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
    wchar_t lpszComLine[256];
    // Форматируем строку с командой для передачи в дочерний процесс
    wsprintf(lpszComLine, L"C:\\zadanie10\\x64\\Debug\\zadanie10dop.exe %d", (int)hInheritProcess);

    // Создаем новый процесс
    if (!CreateProcess(
        NULL,                     // Имя исполняемого файла (здесь не используется)
        lpszComLine,              // Путь к исполняемому файлу
        NULL,                     // Атрибуты безопасности процесса
        NULL,                     // Атрибуты безопасности потока
        TRUE,                     // Флаг наследования дескриптора
        CREATE_NEW_CONSOLE,       // Флаги создания процесса
        NULL,                     // Среда (используется текущая среда)
        NULL,                     // Текущий каталог (используется текущий каталог)
        &si,                      // Структура STARTUPINFO
        &pi                       // Структура PROCESS_INFORMATION
    )) {
        std::cerr << "Новый процесс не создан.\n";
        _cputs("Нажмите любую клавишу, чтобы завершить.\n");
        _getch();
        return GetLastError();
    }
    // Выводим PID и дескриптор дочернего процесса
    std::cout << "\n\nPID: " << pi.dwProcessId << std::endl;
    std::cout << "Дескриптор: " << pi.hProcess << std::endl;
    std::cout << "Псевдодескриптор текущего процесса: " << hThread << std::endl;


    // Ожидаем завершения дочернего процесса
    WaitForSingleObject(pi.hProcess, INFINITE);
    TerminateThread(pi.hThread, 0);
    TerminateProcess(pi.hProcess, 0);

    // Закрываем дескрипторы процесса и потока
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    // закрываем дескриптор наследуемого процесса
    CloseHandle(hInheritProcess);

    HANDLE hThread1 = NULL;
    HANDLE hThread2 = NULL;

    char input;
    while (true) {
        if (hThread1 != NULL) {
            WaitForSingleObject(hThread1, INFINITE);
        }
        if (hThread2 != NULL) {
            WaitForSingleObject(hThread2, INFINITE);
        }
        std::cout << "\nВыберите действие:\n";
        std::cout << "1. Создание 1 потока \n";
        std::cout << "2. Создание 2 потока\n";
        std::cout << "3. Завершить поток 1\n";
        std::cout << "9. Выход\n";
        std::cout << "Введи цифру: ";
        std::cin >> input;

        switch (input) {
        case '1':
            system("cls");
            if (hThread1 == NULL) {
                hThread1 = CreateThread(NULL, 0, ThreadFunction1, NULL, 0, NULL);
                std::cout << "ID потока 1: " << GetThreadId(hThread1) << std::endl;
                if (hThread1 == NULL) {
                    std::cerr << "Не удалось создать поток 1.\n";
                }
            }
            else {
                std::cerr << "Поток 1 уже создан.\n";
            }
            break;
        case '2':
            system("cls");
            if (hThread2 == NULL) {
                hThread2 = CreateThread(NULL, 0, FindDifference, NULL, 0, NULL);
                std::cout << "ID потока 2: " << GetThreadId(hThread2) << std::endl;
                Sleep(10000);
                TerminateThread(hThread2, 0);
                CloseHandle(hThread2);
                std::cout << "Поток 2 завершен.\n";
                if (hThread2 == NULL) {
                    std::cerr << "Не удалось создать поток 2.\n";
                }
            }
            else {
                std::cerr << "Поток 2 уже создан.\n";
            }
            break;
        case '3':
            system("cls");
            if (hThread1 != NULL) {
                TerminateThread(hThread1, 0);
                CloseHandle(hThread1);
                hThread1 = NULL;
                std::cout << "Поток 1 завершен.\n";
            }
            else {
                std::cerr << "Поток 1 не был создан.\n";
            }
            break;
        case '4':
            system("cls");
            if (hThread2 != NULL) {
                TerminateThread(hThread2, 0);
                CloseHandle(hThread2);
                hThread2 = NULL;
                std::cout << "Поток 2 завершен.\n";
            }
            else {
                std::cerr << "Поток 2 не был создан.\n";
            }
            break;
        case '9':
            system("cls");
            std::cout << "Выход из программы.\n";
            if (hThread1 != NULL) {
                TerminateThread(hThread1, 0);
                CloseHandle(hThread1);
            }
            return 0;
        default:
            std::cerr << "Неверный выбор. Попробуйте снова.\n";
            break;
        }
    }
    return 0;
}
