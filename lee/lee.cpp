#include "stdafx.h"
#include <iostream> // подключаем заголовочный файл с классами, функциями и переменными для организации ввода-вывода в языке программирования C++.
#include <iomanip> // Подключаем манипуляторы при работе с потоковыми операциями.
#include <queue> // Подключаем заголовочный файл очереди.
#include <ctime> // Подключаем заголовочный файл времени.

using namespace std; // Указание пространства имён.

#pragma region Classes  

// Классы для реализации ввода + Наследование + Полиморфизм подтипов.
class Coordinates { // Базовый класс.
public:
	int X, Y, Z, size; // X,Y - координаты объекта в зависимости от класса, Z - фикс для вывода в консоли, size - размер карты.
	int const fault = 2; // Константа для устранения погрешности размера карты.
	int getCoordinates() {
		cout << "";
		cout << "";
		cin >> X;
		cout << "";
		cin >> Y;
		cout << "\n";
		return X, Y;
	}
};

class MapCoordinates : public Coordinates { // Дочерний класс для размеров карты.
public:
	int virtual getCoordinates() {
		cout << "Введите размер карты.\n";
		cout << "Size:= ";
		cin >> size;
		cout << "\n";
		size += fault; // Учёт погрешности в размерах карты(на стены).
		return size;
	}
};

class StartCoordinates : public Coordinates { // Дочерний класс для координатов точки старта.
public:
	int virtual getCoordinates() {
		cout << "Введите координаты точки старта.\n";
		cout << "X:= ";
		cin >> X;
		cout << "Y:= ";
		cin >> Y;
		cout << "\n";
		Z = X; // Замена X и Y местами для user-friendly вывода и ввода. X - теперь X, а Y - теперь Y, относительно монитора.
		X = Y;
		Y = Z;
		return X, Y;
	}
};

class TargetCoordinates : public Coordinates { // Дочерний класс для координатов точки конца.
public:
	int virtual getCoordinates() {
		cout << "Введите координаты точки конца.\n";
		cout << "X:= ";
		cin >> X;
		cout << "Y:= ";
		cin >> Y;
		Z = X; // Смотреть комментарий строки №48.
		X = Y;
		Y = Z;
		return X, Y;
	}
};

class BarrierCoordinates : public Coordinates { // Дочерний класс для препятствий.
public:
	int virtual getCoordinates() {
		cout << "\n";
		cout << "Введите координаты препятствия.\n";
		cout << "X:= ";
		cin >> X;
		cout << "Y:= ";
		cin >> Y;
		cout << "\n";
		Z = X; // Смотреть комментарий строки №48.
		X = Y;
		Y = Z;
		return X, Y;
	}
};

#pragma endregion

#pragma region Lee

void findWays(int size, int row, int col, char** maze, int** visited, int** ways, queue<int>& plan) { // Функция поиска. rol - строка, col - столбец.  

	if (!visited[row][col]) {
		// Проверка на возможность пройти.
		if (maze[row + 1] != "#" && (row + 1) >= 0 && !visited[row + 1][col] &&
			(maze[row + 1][col] == ' ' || maze[row + 1][col] == 'F')) {
			ways[row + 1][col] = ways[row][col] + 1;
			plan.push(row + 1);
			plan.push(col);
		}
		if (maze[row - 1] != "#" && (row - 1) >= 0 && !visited[row - 1][col] &&
			(maze[row - 1][col] == ' ' || maze[row - 1][col] == 'F')) {
			ways[row - 1][col] = ways[row][col] + 1;
			plan.push(row - 1);
			plan.push(col);
		}
		if (maze[col + 1] != "#" && (col + 1) >= 0 && !visited[row][col + 1] &&
			(maze[row][col + 1] == ' ' || maze[row][col + 1] == 'F')) {
			ways[row][col + 1] = ways[row][col] + 1;
			plan.push(row);
			plan.push(col + 1);
		}
		if (maze[col - 1] != "#" && (col - 1) >= 0 && !visited[row][col - 1] &&
			(maze[row][col - 1] == ' ' || maze[row][col - 1] == 'F')) {
			ways[row][col - 1] = ways[row][col] + 1;
			plan.push(row);
			plan.push(col - 1);
		}
		visited[row][col] = 1; // Отмечаем клетку в которой побывали.
	}
}

#pragma endregion

#pragma region Main

int main()
{
	setlocale(LC_ALL, "Russian"); // Русификатор.
beginning: // Для повторной работы программы.
	char question; // Для хранения +/- в вопросе.
	queue <int> plan; // Объявляем пустую очередь типа int.
	MapCoordinates map; // Объявляем объект map, класса MapCoordinates.
	map.getCoordinates(); // Вызов функции класса, получаем размер карты.

	StartCoordinates start; // Объявляем объект start, класса StartCoordinates.
	start.getCoordinates(); // Вызов функции класса, получаем координаты начала.

	TargetCoordinates target; // Объявляем объект target, класса TargetCoordinates.
	target.getCoordinates(); // Вызов функции класса, получаем координаты конца.

	char** maze = new char *[map.size];
	int** visited = new int *[map.size];
	int** ways = new int *[map.size];

	for (int i = 0; i < map.size; i++) {
		maze[i] = new char[map.size];   // Массив для хранения лабиринта.
		visited[i] = new int[map.size]; // Массив для хранения информации о посещении клеток.
		ways[i] = new int[map.size];  // Массив для хранения найденных путей.

		for (int j = 0; j < map.size; j++) {
			visited[i][j] = 0;
			ways[i][j] = -1;
			maze[i][j] = ' ';
			// Поиск начала пути.
			if (i == 0 || i == map.size - 1 || j == 0 || j == map.size - 1)
				maze[i][j] = '#';
			if (i == start.X && j == start.Y) {
				maze[i][j] = 'S';
				ways[i][j] = 1;
				plan.push(i);
				plan.push(j);
			}
			else if (i == target.X && j == target.Y) {
				maze[i][j] = 'F';
			}
		}
	}

	BarrierCoordinates barrier; // Объявляем объект target, класса TargetCoordinates.
createBarrier: // Ещё 1 препятствиие.
	barrier.getCoordinates(); // Вызов функции класса, получаем координаты препятствия.

	maze[barrier.X][barrier.Y] = '#';
	cout << "Ещё препятствие?(+/-)";
	cin >> question;
	if (question == '+')
		goto createBarrier; // Строка №171.
	system("cls"); //Очистка экрана по окончанию ввода параметров.

	clock_t time; // Тип данных способный представлять временные тики. 
	time = clock();

	while (!plan.empty()) {
		start.X = plan.front();
		plan.pop();
		start.Y = plan.front();
		plan.pop();
		findWays(map.size, start.X, start.Y, maze, visited, ways, plan); // Продолжаем поиск пути.
	}
	time = clock() - time;
	printf("Нужная точка достигнута, за: %f\n", (double)time / CLOCKS_PER_SEC);
	if (!visited[target.X][target.Y]) {
		cout << "Маршрут не существует!" << "\n";
	}
	else {
		cout << "Маршрут найден!" << "\n" << "\n";
		start.X = target.X;
		start.Y = target.Y;
		maze[start.X][start.Y] = '+';
		while (ways[start.X][start.Y] != 2) { // Восстановление пути.
			if ((start.X - 1) >= 0 && (start.X - 1) < map.size && (ways[start.X - 1][start.Y] == ways[start.X][start.Y] - 1)) {
				start.X = start.X - 1;
				maze[start.X][start.Y] = '.';
			}
			else if ((start.X + 1) >= 0 && (start.X + 1) < map.size && (ways[start.X + 1][start.Y] == ways[start.X][start.Y] - 1)) {
				start.X = start.X + 1;
				maze[start.X][start.Y] = '.';
			}
			else if ((start.Y - 1) >= 0 && (start.Y - 1) < map.size && (ways[start.X][start.Y - 1] == ways[start.X][start.Y] - 1)) {
				start.Y = start.Y - 1;
				maze[start.X][start.Y] = '.';
			}
			else if ((start.Y + 1) >= 0 && (start.Y + 1) < map.size && (ways[start.X][start.Y + 1] == ways[start.X][start.Y] - 1)) {
				start.Y = start.Y + 1;
				maze[start.X][start.Y] = '.';
			}
		}
		for (int i = 0; i < map.size; i++) {
			for (int j = 0; j < map.size; j++) {
				if (i == target.X && j == target.Y) {
					maze[i][j] = 'F';
				}
				cout << maze[i][j];
			}
			cout << "\n";
		}
	}
	cout << "\n" <<"Хотите ввести другие значения?(+/-)";
	cin >> question;
	if (question == '+') { // Повторное выполнение программы при question == "+".
		system("cls"); //Очистка экрана.
		goto beginning; // Строка №130.
	}
	return 0;
}
#pragma endregion