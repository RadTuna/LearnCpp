
#include <Windows.h>
#include <stdio.h>

#define BUFSIZE 10

// 2개의 핸들 포인터를 선언함. (void*)
HANDLE hReadEvent;
HANDLE hWriteEvent;
int buf[BUFSIZE];

// Write 스레드 함수. // 보이드 포인터 매개변수는 사용하지 않음.
// 버퍼를 반복문 k값으로 채우는 기능을 수행함.
DWORD WINAPI WriteThread(LPVOID arg)
{
	DWORD retval;

	for (int k = 0; k <= 500; ++k)
	{
		// 신호가 Set될때 까지 대기함.
		// hReadEvent가 Auto이므로 해당 구문을 벗어나면 다시 Reset됨.
		retval = WaitForSingleObject(hReadEvent, INFINITE);
		if (retval != WAIT_OBJECT_0) // 신호가 Set되서 끝난것이 아닐경우 함수를 종료함.
		{
			break;
		}

		// k의 값으로 버퍼를 전부 채움.
		for (int i = 0; i < BUFSIZE; ++i)
		{
			buf[i] = k;
		}

		// hWriteEvent의 신호를 Set으로 설정함.
		SetEvent(hWriteEvent);
	}

	return 0;
}

DWORD WINAPI ReadThread(LPVOID arg)
{
	DWORD retval;

	for (int k = 0; k <= 500; ++k)
	{
		// 신호가 Set될때 까지 대기함.
		// hWriteEvent가 Auto이므로 해당 구문을 벗어나면 다시 Reset됨.
		retval = WaitForSingleObject(hWriteEvent, INFINITE);
		if (retval != WAIT_OBJECT_0) // 신호가 Set되서 끝난것이 아닐경우 함수를 종료함.
		{
			break;
		}

		// k의 값으로 버퍼를 전부 채움.
		for (int i = 0; i < BUFSIZE; ++i)
		{
			printf("%3d", buf[i]);
		}
		printf("\n");

		// hReadEvent의 신호를 Set으로 설정함.
		SetEvent(hReadEvent);
	}

	return 0;
}

// Main 함수.
int main(int argc, char* argv[])
{
	/*
	@ function : CreateEvent
	@ param lpEventAttributes : 자식 프로세스가 종료되고 반환된 핸들을 상속할 수 있는지 확인하는 보안변수
	@ param bManualReset : true이면, ResetEvent함수를 사용하여 신호의 Reset을 해줘야 함. false이면, 스레드의 대기 상태가 해제되는 순간 Reset으로 됨.
	@ param bInitialState : true이면, 이벤트의 신호 초기상태가 Set. false이면 Reset.
	@ param lpName : 이벤트 객체의 이름.
	@ return : 이벤트 객체의 핸들을 반환함. 함수가 실패하면 nullptr을 반환.
	*/
	// WriteEvent객체를 생성함.
	// 자동 - 신호 초기값은 Reset임.
	hWriteEvent = CreateEvent(nullptr, false, false, nullptr);
	if (hWriteEvent == nullptr) // 생성이 제대로 되지 않았으면 메인함수를 종료함.
	{
		return 1;
	}

	// ReadEvent객체를 생성함.
	// 자동 - 신호 초기값은 Set임.
	hReadEvent = CreateEvent(nullptr, false, true, nullptr);
	if (hReadEvent == nullptr) // 생성이 제대로 되지 않았으면 메인함수를 종료함.
	{
		return 1;
	}

	//스레드 핸들을 선언함.
	HANDLE hThread[3];

	/*
	@ function : CreateThread
	@ param SecurityAttributes : 자식 프로세스가 종료되고 반환된 핸들을 상속할 수 있는지 확인하는 보안변수
	@ param StackSize : 초기 스택크기를 결정하는 변수, Byte
	@ param StartFunction : 스레드에 의해 실행되는 함수의 함수포인터를 받는 변수
	@ param ThreadParameter : 스레드로 전달되는 하나의 변수
	@ param CreationFlags : 스레드가 생성되는 것을 제어하는 플래그
	@ param ThreadId : 스레드를 구분할 수 있는 식별자를 반환하는 변수 (Out)
	@ return : 함수가 정상적으로 동작했다면, 새롭게 생성한 스레드의 핸들을 반환함.
	*/
	// 스레드를 생성함. 스레드 함수는 Write 스레드함수를 바인딩.
	hThread[0] = CreateThread(nullptr, 0, WriteThread, nullptr, 0, nullptr);

	// 스레드를 생성함. 스레드 함수는 Read 스레드함수를 바인딩.
	hThread[1] = CreateThread(nullptr, 0, ReadThread, nullptr, 0, nullptr);

	// 스레드를 생성함. 스레드 함수는 Read 스레드함수를 바인딩.
	hThread[2] = CreateThread(nullptr, 0, ReadThread, nullptr, 0, nullptr);

	/*
	@ function : WaitForMultipleObjects
	@ param nCount : 핸들배열의 길이를 입력하는 변수. 해당 인자는 0이 될 수 없음.
	@ param lpHandles : 핸들배열을 입력하는 변수.
	@ param bWaitAll : true라면, 핸들배열의 모든 핸들의 신호가 Set될때 return, false라면, 핸들배열 중 1개의 핸들의 신호가 Set되면 return
	@ param dwMilliseconds : 신호가 Set되지 않더라도 해당 변수에 입력된 시간이 지나면 return
	@ return : 해당 함수가 리턴 된 이벤트를 나타냄 (WAIT_OBJECT_0, WAIT_ABANDONED_0 등)
	*/
	// 3개의 스레드가 종료될때까지 Main 스레드는 현재 구문에서 대기함.
	// 대기시간은 무한.
	WaitForMultipleObjects(3, hThread, true, INFINITE);

	// 2개의 이벤트 객체 핸들을 해제함.
	CloseHandle(hWriteEvent);
	CloseHandle(hReadEvent);
	return 0;
}
