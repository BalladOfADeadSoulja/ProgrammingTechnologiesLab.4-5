#pragma once

#include <Windows.h>
#include <map>
#include <vector>
#include <string>
#include <iostream>

#define FIELD_SIZE 500

#define MAX_SIZE_MATRIX 10 //Ограничение на размеры матрицы и вектора
#define MAX_SIZE_MATRIX_VALUE 1000 //Ограничение на максимальное значение в поле матрицы и вектора

#define BTN_CALCULATE 8001 //Кнопка расчитать
#define BTN_RADIO_VECTOR 8002	//Выбор вектора
#define BTN_RADIO_SCALAR 8003	// Выбор скаляра
#define BTN_MULTIPLICATION 8004	//Перемножение

// Состояние перед созданием окна расчетов
struct State {
	bool use;	// Уже используется
	int rowCount, columnCount;	//Размерности матрицы
	bool isVector;	//Используется ветор?
	int sizeVector, valueScalar;	// Размер векора / значение скаляра
} stateInfo;

//Создание класса окна
WNDCLASSEX createWindowsClass(HINSTANCE instance, LPCWSTR className, WNDPROC wndCallback) {
	WNDCLASSEX wcx = { 0 };
	wcx.cbSize = sizeof(WNDCLASSEX);
	wcx.hInstance = instance;
	wcx.lpszClassName = className;
	wcx.lpfnWndProc = wndCallback;
	wcx.style = CS_HREDRAW | CS_VREDRAW;
	return wcx;
}

HWND editRowCount;
HWND editColumnCount;
HWND radioVector;
HWND radioScalar;
HWND editVectorSize;
HWND editScalarValue;
HWND buttonCalc;

WNDCLASSEX classMainWindow;
WNDCLASSEX classSubWindow;