#include "pch.h"
#include "Console.h"

#include <iostream>
#include <string>

using namespace std;

void InitConsoleWindow()
{
	AllocConsole();
	//HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE); 
	//int hCrt = _open_osfhandle((long)handle,_O_TEXT); 
	//FILE * hf = _fdopen( hCrt, "w" ); 
	//*stdout = *hf; 

	FILE* stream; freopen_s(&stream, "CONOUT$", "w", stdout);
}

// �ڵ���FreeConsoleWindow()���رտ���̨֮ǰ����Ҫ���ͷ���ص�handle��
// �ο���https://social.msdn.microsoft.com/Forums/en-US/c8b63202-257d-4339-935f-b47c8042834e/windows-api-freeconsole-not-closing-a-console-before-an-hwnd-is-made
void CloseConsoleWindow() {
	fclose(stdout);
	FreeConsole();
}

//	==================================================
//	�����߳�ʹ�÷�ʽ�Ƿ���ȷ��ֱ����AfxBeginThread���������߳�
void ThreadTestFunc(int I) {

	InitConsoleWindow();	//	��ʼ��Console

	int sum = 0;
	for (int i = 0; i < 10; ++i) {
		sum += i;
		/*TRACE("==> sum(%2d) = %10d\n",i,sum);*/
		printf("==> sum(%2d) = %10d\n", i, sum);

		Sleep(100);
	}

	system("pause");
	//FreeConsole();
	CloseConsoleWindow();
}


void TestCoutInConsole() {
	InitConsoleWindow();

	string str;
	str = "hello world!";
	cout << str << endl;

	system("pause");
	CloseConsoleWindow();
}