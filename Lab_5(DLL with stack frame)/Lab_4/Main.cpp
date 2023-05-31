#include "HeaderOne.h"

std::map<LPCWSTR, HWND> widgets;	//Ассоциативный массив(словарь) для хранения всех графических элементов на окне
std::vector<std::vector<HWND>> matrix, answer;	//Хранение ячеек матрицы и результата
std::vector<HWND> vec;	//Хранение ячеек вектора

//Иницилизация окна с расчетами
void initSubWindow(HWND hWnd) {
	CreateWindow(L"static", L"[ Matrix ]  -->  [ Answer ]", WS_VISIBLE | WS_CHILD, 50, 5, 170, 20, hWnd, 0, 0, 0);

	matrix.resize(stateInfo.rowCount);
	for (int i = 0; i < matrix.size(); ++i) {
		matrix[i].resize(stateInfo.columnCount);
		for (int j = 0; j < matrix[i].size(); ++j) {
			matrix[i][j] = CreateWindow(L"edit", L"0", WS_VISIBLE | WS_CHILD | ES_NUMBER | WS_BORDER, 5 + (j * 40), 30 + (i * 20), 40, 20, hWnd, 0, 0, 0);
		}
	}
	int posY = 40 + (matrix.size() * 20);

	if (stateInfo.isVector) {
		CreateWindow(L"static", L"[ Vector ]", WS_VISIBLE | WS_CHILD, 50, posY, 70, 20, hWnd, 0, 0, 0);
		vec.resize(stateInfo.sizeVector);
		for (int i = 0; i < vec.size(); ++i) {
			vec[i] = CreateWindow(L"edit", L"0", WS_VISIBLE | WS_CHILD | ES_NUMBER | WS_BORDER, 5 + (i * 40), posY + 20, 40, 20, hWnd, 0, 0, 0);
		}
		posY += 25;
	}
	else {
		char value[FIELD_SIZE];
		CreateWindowA("static", (std::string("[ Scalar = ") + std::to_string(stateInfo.valueScalar) + " ]").c_str(), WS_VISIBLE | WS_CHILD, 50, posY, 100, 20, hWnd, 0, 0, 0);
	}
	HWND buttonMul = CreateWindowA("button", "Multiplication", WS_VISIBLE | WS_CHILD, 10, posY + 25, 130, 20, hWnd, (HMENU)BTN_MULTIPLICATION, NULL, NULL);
	widgets.insert({ L"buttonMul", buttonMul });
}

//Иницилизация главного окна
void initMainWindow(HWND hWnd) {
	CreateWindow(L"static", L"[ Matrix info ]", WS_VISIBLE | WS_CHILD, 50, 5, 100, 20, hWnd, 0, 0, 0);

	CreateWindow(L"static", L"Input row count:", WS_VISIBLE | WS_CHILD, 10, 35, 130, 20, hWnd, 0, 0, 0);
	CreateWindow(L"static", L"Input column count:", WS_VISIBLE | WS_CHILD, 10, 65, 130, 20, hWnd, 0, 0, 0);

	editRowCount = CreateWindowA("edit", "", WS_VISIBLE | WS_CHILD | ES_NUMBER | WS_BORDER, 150, 35, 60, 20, hWnd, NULL, NULL, NULL);
	editColumnCount = CreateWindowA("edit", "", WS_VISIBLE | WS_CHILD | ES_NUMBER | WS_BORDER, 150, 65, 60, 20, hWnd, NULL, NULL, NULL);

	CreateWindow(L"static", L"[ Vector/Scalar info ]", WS_VISIBLE | WS_CHILD, 50, 95, 150, 20, hWnd, 0, 0, 0);

	radioVector = CreateWindowA("button", "Vector Size:", WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON, 10, 125, 100, 20, hWnd, (HMENU)BTN_RADIO_VECTOR, NULL, NULL);
	radioScalar = CreateWindowA("button", "Scalar:", WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON, 10, 155, 100, 20, hWnd, (HMENU)BTN_RADIO_SCALAR, NULL, NULL);
	editVectorSize = CreateWindowA("edit", "", WS_VISIBLE | WS_CHILD | ES_NUMBER | WS_BORDER, 120, 125, 80, 20, hWnd, NULL, NULL, NULL);
	editScalarValue = CreateWindowA("edit", "", WS_VISIBLE | WS_CHILD | ES_NUMBER | WS_BORDER, 120, 155, 80, 20, hWnd, NULL, NULL, NULL);

	buttonCalc = CreateWindowA("button", "Calculate", WS_VISIBLE | WS_CHILD, 10, 185, 100, 20, hWnd, (HMENU)BTN_CALCULATE, NULL, NULL);

	//Сохранение в ассоциативный массив созданые поля ввода  
	widgets.insert({ L"editRowCount", editRowCount });
	widgets.insert({ L"editColumnCount", editColumnCount });
	widgets.insert({ L"radioVector", radioVector });
	widgets.insert({ L"radioScalar", radioScalar });
	widgets.insert({ L"editVectorSize", editVectorSize });
	widgets.insert({ L"editScalarValue", editScalarValue });
	widgets.insert({ L"buttonCalc", buttonCalc });

	EnableWindow(editVectorSize, false);
	EnableWindow(editScalarValue, false);
}

//Получить значение из поля ввода по ключу
bool getFieldValue(LPCWSTR key, int& value, int limit)
{
	char buffer[FIELD_SIZE];
	GetWindowTextA(widgets[key], buffer, FIELD_SIZE);
	if (!strlen(buffer))
		return false;
	value = atoi(buffer);
	return value > 0 && value <= limit;
}

//Получить значение из поля ввода (перегружена)
bool getFieldValue(HWND wnd, int& val, int limit)
{
	char buffer[FIELD_SIZE];
	GetWindowTextA(wnd, buffer, FIELD_SIZE);
	if (!strlen(buffer))
		return false;
	val = atoi(buffer);
	return val >= 0 && val <= limit;
}

//Функция создания окна рачетов (+ проверки на допустимость полей)
bool createSubWindow(HWND hWnd) {

	if (stateInfo.use) {
		MessageBox(NULL, L"Calculate window already created", L"Error", NULL);
		return false;
	}
	if (!getFieldValue(L"editRowCount", stateInfo.rowCount, MAX_SIZE_MATRIX)) {
		MessageBox(NULL, L"Invalid row count", L"Error", NULL);
		return false;
	}
	if (!getFieldValue(L"editColumnCount", stateInfo.columnCount, MAX_SIZE_MATRIX)) {
		MessageBox(NULL, L"Invalid column count", L"Error", NULL);
		return false;
	}

	stateInfo.isVector = IsDlgButtonChecked(hWnd, BTN_RADIO_VECTOR); //Получение информации о выбранном (векторе/скаляре)
	if (stateInfo.isVector) {
		if (!getFieldValue(L"editVectorSize", stateInfo.sizeVector, MAX_SIZE_MATRIX)) {
			MessageBox(NULL, L"Invalid vector size", L"Error", NULL);
			return false;
		}
		if (stateInfo.sizeVector != stateInfo.columnCount) {
			MessageBox(NULL, L"Column count matrix != Vector size (Impossible multiplication)", L"Error", NULL);
			return false;
		}
	}
	else {
		if (!getFieldValue(L"editScalarValue", stateInfo.valueScalar, MAX_SIZE_MATRIX_VALUE)) {
			MessageBox(NULL, L"Invalid scalar value", L"Error", NULL);
			return false;
		}
	}
	return stateInfo.use = true;
}

//Перемножение и вывод результата
bool multiplication(HWND hWnd) {

	HMODULE hDll = LoadLibrary(L"DynamicDLL.dll");
	if (hDll == NULL)
	{
		// библиотека не найдена
		MessageBox(NULL, L"Библиотека DynamicDLL не найдена", L"Ошибка", MB_OK | MB_ICONERROR);
		return 1;
	}

	// вызов функции из библиотеки
	typedef int (*MyFunction)(int);
	typedef int (*mull) (int, int);
	mull Multiply = (mull)GetProcAddress(hDll, "Mull");
	if (Multiply == NULL)
	{
		// функция не найдена
		MessageBox(NULL, L"Функция Multiply не найдена в библиотеке DynamicDLL", L"Ошибка", MB_OK | MB_ICONERROR);
		return 1;
	}

	std::vector<std::vector<int>> mtx(matrix.size(), std::vector<int>(matrix[0].size()));
	for (int i = 0; i < matrix.size(); ++i) {
		for (int j = 0; j < matrix[i].size(); ++j) {
			if (!getFieldValue(matrix[i][j], mtx[i][j], MAX_SIZE_MATRIX_VALUE)) {
				MessageBoxA(NULL, (std::string("Invalid value : Matrix[") + std::to_string(i + 1) + "][" + std::to_string(j + 1) + "]").c_str(), "Error", NULL);
				return false;
			}
		}
	}
	for (int i = 0; i < answer.size(); ++i) {
		for (int j = 0; j < answer[i].size(); ++j) {
			DestroyWindow(answer[i][j]);
		}
	}

	if (stateInfo.isVector) { //Если используется вектор
		std::vector<int> v(stateInfo.sizeVector);
		for (int i = 0; i < v.size(); ++i) {
			if (!getFieldValue(vec[i], v[i], MAX_SIZE_MATRIX_VALUE)) {
				MessageBoxA(NULL, (std::string("Invalid value : Vector[") + std::to_string(i + 1) + "]").c_str(), "Error", NULL);
				return false;
			}
		}
		for (int i = 0; i < mtx.size(); ++i) {
			int res = 0;
			for (int j = 0; j < v.size(); ++j) {
				res += Multiply(mtx[i][j], v[j]);
				//res += Sum(mtx[i][j], v[j]);
			}
			answer.resize(1);
			answer[0].push_back(CreateWindowA("edit", std::to_string(res).c_str(),
				WS_VISIBLE | WS_CHILD | ES_NUMBER | WS_BORDER,
				25 + (mtx[0].size() * 40), 30 + (i * 20), 40, 20, hWnd, 0, 0, 0));
			//FreeLibrary(hDll);
		}
		//FreeLibrary(hDll);
	}
	else {	// Если используется скаляр
		answer.resize(mtx.size());
		for (int i = 0; i < mtx.size(); ++i) {
			answer[i].resize(mtx[i].size());
			//int res = 0;
			for (int j = 0; j < mtx[i].size(); ++j) {
				//int skal = stateInfo.valueScalar;
				//res = Sum2(mtx[i][j], skal);
				answer[i][j] = CreateWindowA("edit", std::to_string(mtx[i][j] * stateInfo.valueScalar).c_str(), /*Sum(mtx[i][j], stateInfo.valueScalar*/
					WS_VISIBLE | WS_CHILD | ES_NUMBER | WS_BORDER,
					25 + (mtx.size() * 40) + (j * 40), 30 + (i * 20), 40, 20, hWnd, 0, 0, 0);
			}
		}
	}

	return true;
}

//Функция обратного вызова для окна расчетов
LRESULT CALLBACK callbackSubWindow(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg)
	{
	case WM_CREATE:
		initSubWindow(hWnd);
		break;
	case WM_DESTROY:
		stateInfo.use = false;
		break;
	case WM_COMMAND:
		if (wParam == BTN_MULTIPLICATION) {
			multiplication(hWnd); //Перемножить
		}
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}

//Функция обратного вызова для главного расчетов
LRESULT CALLBACK callbackMainWindow(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg)
	{
	case WM_CREATE:
		initMainWindow(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_COMMAND:
		if (wParam == BTN_CALCULATE) {
			if (createSubWindow(hWnd)) { //Проверка перед созданием окна расчетов
				CreateWindow(L"classSubWindow", L"Calculate",
					WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT,
					1200, 350, 0, 0, 0, 0);
			}
		}

		if (LOWORD(wParam) == BTN_RADIO_VECTOR) { // Выбор вектора
			EnableWindow(widgets[L"editVectorSize"], true);
			EnableWindow(widgets[L"editScalarValue"], false);
		}
		if (LOWORD(wParam) == BTN_RADIO_SCALAR) {	//Выбор скаляра
			EnableWindow(widgets[L"editVectorSize"], false);
			EnableWindow(widgets[L"editScalarValue"], true);
		}
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}

//Точка входа
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR szCmdLine, int nShowCmd) {

	classMainWindow = createWindowsClass(hInst, L"classMainWindow", callbackMainWindow);
	classSubWindow = createWindowsClass(hInst, L"classSubWindow", callbackSubWindow);

	if (!RegisterClassEx(&classMainWindow)) {
		MessageBox(NULL, L"Failed RegisterClassEx() for classMainWindow", L"Error", NULL);
		return 1;
	}

	if (!RegisterClassEx(&classSubWindow)) {
		MessageBox(NULL, L"Failed RegisterClassEx() for classSubWindow", L"Error", NULL);
		return 1;
	}

	CreateWindow(L"classMainWindow", L"Application",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT,
		250, 300, 0, 0, 0, 0);

	MSG msg = { 0 };
	while (GetMessage(&msg, 0, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}