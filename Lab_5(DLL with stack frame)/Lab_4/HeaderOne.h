#pragma once

#include <Windows.h>
#include <map>
#include <vector>
#include <string>
#include <iostream>

#define FIELD_SIZE 500

#define MAX_SIZE_MATRIX 10 //����������� �� ������� ������� � �������
#define MAX_SIZE_MATRIX_VALUE 1000 //����������� �� ������������ �������� � ���� ������� � �������

#define BTN_CALCULATE 8001 //������ ���������
#define BTN_RADIO_VECTOR 8002	//����� �������
#define BTN_RADIO_SCALAR 8003	// ����� �������
#define BTN_MULTIPLICATION 8004	//������������

// ��������� ����� ��������� ���� ��������
struct State {
	bool use;	// ��� ������������
	int rowCount, columnCount;	//����������� �������
	bool isVector;	//������������ �����?
	int sizeVector, valueScalar;	// ������ ������ / �������� �������
} stateInfo;

//�������� ������ ����
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