#include <windows.h>
#include <time.h>
#include <string>
#include <Commctrl.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HWND hWnd;	
HWND button;
HWND infoEdit;
HWND infoEdit2;
HWND infoEdit3;
HWND progressBar1;
HWND progressBar2;
HWND progressBar3;

void StartBubble();
void StartChoice();
void StartPut();
HANDLE thread_handle[6];
DWORD WINAPI ThreadFunction1(LPVOID);
DWORD WINAPI ThreadFunction2(LPVOID);
DWORD WINAPI ThreadFunction3(LPVOID);
DWORD WINAPI ThreadForBar1(LPVOID);
DWORD WINAPI ThreadForBar2(LPVOID);
DWORD WINAPI ThreadForBar3(LPVOID);

SYSTEMTIME forBubbleSt;
SYSTEMTIME forChoiceSt;
SYSTEMTIME forPutSt;
SYSTEMTIME forBubbleFn;
SYSTEMTIME forChoiceFn;
SYSTEMTIME forPutFn;

const int size = 46341;
long long maxIter=size*(size-1)/2;
int posForPut;
int posBar1;
int posBar2;
int posBar3;
long double iterBubble;
long double iterChoice;
long double iterPut;
long double cheinSumBuuble;
long double cheinSumChoice;
long double cheinSumPut;
int a[size];
int b[size];
int c[size];
char bubbleStr[20]="Пузырьком\0";
char choiceStr[20]="Выбором\0";
char putStr[20]="Вставкой\0";
int second;
char sec[10];
FILETIME timeSt, timeFn, timeKrnl, timeSome;
void GetTime(){
	return;
}

void Strt(){
	srand(time(0));
	for(int i=0; i<size; i++){
		a[i]=rand()%100;
		b[i]=a[i];
		c[i]=b[i];
	}
	return;
}
void StartBubble(){
	int tmp1;
	for(iterBubble=0; iterBubble<size-1; iterBubble++){
		for(int j=0; j<size-1; j++){
			if(a[j]>a[j+1]){
				tmp1=a[j];
				a[j]=a[j+1];
				a[j+1]=tmp1;
			}
		}
		cheinSumBuuble=iterBubble*(iterBubble-1)/2;
		//char buff[30];
		//ltoa(cheinSumBuuble, buff, 10);
		//MessageBox(hWnd,buff,"FEF",MB_OK);
		posBar1 = (int)(cheinSumBuuble/maxIter*14+1);
		//itoa(posBar1, buff, 10);
		//MessageBox(hWnd,buff,"FEF",MB_OK);
		//MessageBox(hWnd,"FEF","FEF",MB_OK);
		SendMessage(progressBar1, PBM_SETPOS,  posBar1, 0);
		//thread_handle[3]=CreateThread(NULL, 0, ThreadForBar1, 0, 0 ,0);
		//WaitForSingleObject(thread_handle[3], 100000);
		//WaitForMultipleObjects(1, thread_handle[3], TRUE, INFINITE);
	}
	return;
}

void StartChoice(){
	int tmp2;
	for(iterChoice = 0; iterChoice < size; iterChoice++) { 
        int pos = iterChoice; 
		tmp2 = b[(int)iterChoice];
        for(int j = iterChoice + 1; j < size; j++){
			if (b[j] < tmp2) {
               pos = j; 
               tmp2 = b[j]; 
           }
        }
		cheinSumChoice=iterChoice*(iterChoice-1)/2;
		posBar2 = (int)(cheinSumChoice/maxIter*14+1);
		SendMessage(progressBar2, PBM_SETPOS, (WPARAM) posBar2, 0);
		//thread_handle[4]=CreateThread(NULL, 0, ThreadForBar2, 0, 0 ,0);
        b[pos] = b[(int)iterChoice]; 
        b[(int)iterChoice] = tmp2;
		//WaitForSingleObject(thread_handle[4], 100000);
    }
	return;
}

void StartPut(){
	int tmp3;
	for(int iterPut=1;iterPut<size;iterPut++){
		for(int j=iterPut; j>0 && c[j-1]>c[iterPut];j--)
			int posForPut=j;
			//tmp3=c[j-1];
			//c[j-1]=c[j];
			//c[j]=tmp3;
		//thread_handle[5]=CreateThread(NULL, 0, ThreadForBar3, 0, 0 ,0);
		cheinSumPut=iterPut*(iterPut-1)/2;
		posBar3 = (int)(cheinSumPut/maxIter*14+1);
		SendMessage(progressBar3, PBM_SETPOS, (WPARAM) posBar3, 0);
		tmp3=c[iterPut];
		for(int t=iterPut; t>posForPut; t--)
			c[t]=c[t-1];
		c[posForPut]=tmp3;
		//WaitForSingleObject(thread_handle[5], 100000);
	}
	return;
}

DWORD WINAPI ThreadFunction1(LPVOID){
	GetSystemTime(&forBubbleSt);
	StartBubble();
	GetSystemTime(&forBubbleFn);
	SendMessage(hWnd, WM_COMMAND, MAKEWPARAM(0, 201), 0);
	return 1;
}
DWORD WINAPI ThreadFunction2(LPVOID){
	GetSystemTime(&forChoiceSt);
	StartChoice();
	GetSystemTime(&forChoiceFn);
	SendMessage(hWnd, WM_COMMAND, MAKEWPARAM(0, 202), 0);
	return 1;
}
DWORD WINAPI ThreadFunction3(LPVOID){
	GetSystemTime(&forPutSt);
	StartPut();
	GetSystemTime(&forPutFn);
	SendMessage(hWnd, WM_COMMAND, MAKEWPARAM(0, 203), 0);
	return 1;
}
DWORD WINAPI ThreadForBar1(LPVOID){
	posBar1 = (int)(iterBubble/maxIter*14);
	SendMessage(progressBar1, PBM_SETPOS, (WPARAM) posBar1, 0);
	return 1;
}
DWORD WINAPI ThreadForBar2(LPVOID){
	posBar2 = (int)(iterChoice/maxIter*14);
	SendMessage(progressBar2, PBM_SETPOS, (WPARAM) posBar2, 0);
	return 1;
}
DWORD WINAPI ThreadForBar3(LPVOID){
	posBar3 = (int)(iterPut/maxIter*14);
	SendMessage(progressBar3, PBM_SETPOS, (WPARAM) posBar3, 0);
	return 1;
}


char WinName[] = "MainFrame";
int height=600,							//высота окна
	width=600;							//ширина окна
int count=0;							//счетчик изменений окна

	

int WINAPI WinMain(HINSTANCE This,			// Дескриптор текущего приложения
	HINSTANCE Prev,							// В современных системах всегда 0
	LPTSTR cmd,								// Командная строка
	int mode)								// Режим отображения окна
{								// Дескриптор главного окна программы
	MSG msg;									// Структура для хранения сообщения
	WNDCLASS wc;								// Класс окна

													// Определение класса окна	
	wc.hInstance = This;		
	wc.lpszClassName = WinName;						// Имя класса окна
	wc.lpfnWndProc = WndProc;						// Функция окна
	wc.style = CS_HREDRAW | CS_VREDRAW;				// Стиль окна
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);		// Стандартная иконка
	wc.hCursor = LoadCursor(NULL,IDC_ARROW);		// Стандартный курсор
	wc.lpszMenuName = NULL;							// Нет меню
	wc.cbClsExtra = 0;								// Нет дополнительных данных класса
	wc.cbWndExtra = 0;								// Нет дополнительных данных окна


	//============ Заполнение окна цветом===============
	wc.hbrBackground = (HBRUSH)(COLOR_3DHIGHLIGHT);

	if(!RegisterClass(&wc)) return 0;		// Регистрация класса окна 


											//Создание окна


	hWnd = CreateWindow(WinName,			// Имя класса окна
	"lab3",									// Заголовок окна
	WS_OVERLAPPEDWINDOW,					// Стиль окна
	10,										// x
	10,										// y	Размеры окна
	width,									// Width
	height,									// Height
	HWND_DESKTOP,							// Дескриптор родительского окна
	NULL,									// Нет меню
	This,									// Дескриптор приложения
	NULL);									// Дополнительной информации нет
		


	ShowWindow(hWnd, mode);					//Показать окно
	
	//============ Цикл обработки сообщений============



	while(GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);// Функция трансляции кодов нажатой клавиши
		DispatchMessage(&msg); // Посылает сообщение функции WndProc()
	}
	return 0;
}

// Оконная функция вызывается операционной системой
// и получает сообщения из очереди для данного приложения

LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
				 WPARAM wParam, LPARAM lParam)
{	
	
	// Обработчик сообщений
	switch(message)
	{
	case WM_DESTROY : PostQuitMessage(0); 
		break; // Завершение программы
		// Обработка сообщения по умолчанию

	case WM_CREATE:
	{		
		button = CreateWindow ("button", "Start", WS_CHILD | WS_VISIBLE, 20, 450, 550, 50, hWnd, (HMENU)100, 0, 0);
		infoEdit = CreateWindow ("static", "", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT , 100, 100, 100, 100, hWnd, (HMENU)101, 0, 0);
		infoEdit2 = CreateWindow ("static", "", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT , 400, 100, 100, 100, hWnd, (HMENU)102, 0, 0);
		infoEdit3 = CreateWindow ("static", "", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT , 250, 250, 100, 100, hWnd, (HMENU)103, 0, 0);
		SetWindowText(infoEdit, bubbleStr);
		SetWindowText(infoEdit2, choiceStr);
		SetWindowText(infoEdit3, putStr);
		progressBar1 = CreateWindowEx(0, PROGRESS_CLASS, (LPTSTR) NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 20, 30, 250, 30, hWnd, (HMENU) 104, NULL, NULL);
		progressBar2 = CreateWindowEx(0, PROGRESS_CLASS, (LPTSTR) NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 320, 30, 250, 30, hWnd, (HMENU) 105, NULL, NULL);
		progressBar3 = CreateWindowEx(0, PROGRESS_CLASS, (LPTSTR) NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 175, 380, 250, 30, hWnd, (HMENU) 106, NULL, NULL);
		SendMessage(progressBar1, PBM_SETRANGE, 0, MAKELPARAM(0, 14));
		SendMessage(progressBar2, PBM_SETRANGE, 0, MAKELPARAM(0, 14));
		SendMessage(progressBar3, PBM_SETRANGE, 0, MAKELPARAM(0, 14));
		SendMessage(progressBar1, PBM_SETSTEP, (WPARAM) 1, 0);
		SendMessage(progressBar2, PBM_SETSTEP, (WPARAM) 1, 0);
		SendMessage(progressBar3, PBM_SETSTEP, (WPARAM) 1, 0);
		break;
	}

	case WM_LBUTTONDOWN:
	{
		
		break;
	}

	case WM_RBUTTONDOWN:
	{
		
		break;
	}


	case WM_MOUSEMOVE:
	{
			
		break;
	}

	case WM_COMMAND:
	{	
		switch(HIWORD(wParam)){
			case BN_CLICKED:{
				if(LOWORD(wParam)==100){
					Strt();
					strcpy(bubbleStr, "Пузырьком\0");
					strcpy(choiceStr, "Выбором\0");
					strcpy(putStr, "Вставкой\0");
					SetWindowText(infoEdit, bubbleStr);
					SetWindowText(infoEdit2, choiceStr);
					SetWindowText(infoEdit3, putStr);
					thread_handle[0]=CreateThread(NULL, 0, ThreadFunction1, 0, 0 ,0);
					thread_handle[1]=CreateThread(NULL, 0, ThreadFunction2, 0, 0 ,0);
					thread_handle[2]=CreateThread(NULL, 0, ThreadFunction3, 0, 0 ,0);
					//WaitForMultipleObjects(3, thread_handle, TRUE, INFINITE);
				}
				break;	
			}
			case 201:{
				//SendMessage(hWnd, WM_COMMAND, MAKEWPARAM(0, 2001), 0);
				second=forBubbleFn.wSecond-forBubbleSt.wSecond;
				if(second<0) second+=60;
				itoa(second, sec, 10);
				strcat(bubbleStr, " - ");
				strcat(bubbleStr, sec);
				strcat(bubbleStr, " sec\0");
				SetWindowText(infoEdit, bubbleStr);
				break;	
			}
			case 202:{
				//SendMessage(hWnd, WM_COMMAND, MAKEWPARAM(0, 2002), 0);
				second=forChoiceFn.wSecond-forChoiceSt.wSecond;
				if(second<0) second+=60;
				itoa(second, sec, 10);
				strcat(choiceStr, " - ");
				strcat(choiceStr, sec);
				strcat(choiceStr, " sec\0");
				SetWindowText(infoEdit2, choiceStr);
				break;		
			}
			case 203:{
				//SendMessage(hWnd, WM_COMMAND, MAKEWPARAM(0, 2003), 0);
				second=forPutFn.wSecond-forPutSt.wSecond;
				if(second<0) second+=60;
				itoa(second, sec, 10);
				strcat(putStr, " - ");
				strcat(putStr, sec);
				strcat(putStr, " sec\0");
				SetWindowText(infoEdit3, putStr);
				break;	
			}
			case 2001:{
				//GetThreadTimes(thread_handle[0],&timeSt,&timeFn,&timeKrnl,&timeSome);
				//second=(timeFn.dwLowDateTime-timeSt.dwLowDateTime)/10000000;
				//sec1=std::to_string(second);
				second=forBubbleFn.wSecond-forBubbleSt.wSecond;
				if(second<0) second+=60;
				itoa(second, sec, 10);
				strcat(bubbleStr, " - ");
				strcat(bubbleStr, sec);
				strcat(bubbleStr, " sec\0");
				SetWindowText(infoEdit, bubbleStr);
				break;	
			}
			case 2002:{
				//GetThreadTimes(thread_handle[1],&timeSt,&timeFn,&timeKrnl,&timeSome);
				//second=(timeFn.dwLowDateTime-timeSt.dwLowDateTime)/10000000;
				//sec1=std::to_string(second);
				second=forChoiceFn.wSecond-forChoiceSt.wSecond;
				if(second<0) second+=60;
				itoa(second, sec, 10);
				strcat(choiceStr, " - ");
				strcat(choiceStr, sec);
				strcat(choiceStr, " sec\0");
				SetWindowText(infoEdit2, choiceStr);
				break;	
			}		
			case 2003:{
				//GetThreadTimes(thread_handle[2],&timeSt,&timeFn,&timeKrnl,&timeSome);
				//second=(timeFn.dwLowDateTime-timeSt.dwLowDateTime)/10000000;
				//sec1=std::to_string(second);
				second=forPutFn.wSecond-forPutSt.wSecond;
				if(second<0) second+=60;
				itoa(second, sec, 10);
				strcat(putStr, " - ");
				strcat(putStr, sec);
				strcat(putStr, " sec\0");
				SetWindowText(infoEdit3, putStr);
				break;	
			}
		}
		break;			
	}
	}

	return DefWindowProc(hWnd, message, wParam, lParam);

}