#include <Windows.h>
#include <iostream>
#include <stdarg.h>
#include <strsafe.h>
#include <locale.h>

class CSystemLog
{
public:
	enum LOG_LEVEL
	{
		eLOG_LEVEL_DEBUG,
		eLOG_LEVEL_WARNING,
		eLOG_LEVEL_ERROR,
		eLOG_LEVEL_SYSTEM
	};

	void SetSystemLogDirectory(const WCHAR* directoryName)
	{
		StringCbCopy(mPathName, _countof(mPathName), directoryName);
	}

	void SetLogLevel(LOG_LEVEL level)
	{
		mLogLevel = level;
	}

	void SystemLog(const WCHAR *type, LOG_LEVEL level, const WCHAR* stringFormat, ...)
	{
		if (level < mLogLevel)
			return;

		SYSTEMTIME time;
		GetLocalTime(&time);

		WCHAR debugString[1000];
		WCHAR pathString[1000];
		va_list vl;
		
		va_start(vl, stringFormat);

		wvsprintf(debugString, stringFormat, vl);

		va_end(vl);

		CreateDirectory(mPathName, NULL);
		
		wsprintf(pathString, L"%s\\%s", mPathName, L"SYSLOG.txt");

		FILE* fPtr;

		_wfopen_s(&fPtr, (const wchar_t*)pathString, L"a");

		fwprintf(fPtr, L"[%s] [%d-%d-%d %d:%d:%d] %s\n", type, time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, debugString);

		fclose(fPtr);
	}

	static CSystemLog* GetInstance()
	{
		if (instance == nullptr)
		{
			instance = new CSystemLog;
		}
		return instance;
	}

private:
	CSystemLog()
		: mLogCounter(0)
	{
		ZeroMemory(mPathName, sizeof(mPathName));
	}
	static CSystemLog* instance;
	WCHAR mPathName[100];
	LOG_LEVEL mLogLevel;
	UINT64 mLogCounter;
};

CSystemLog* CSystemLog::instance;

int sum(int count, ...)
{
	va_list ap;
	va_start(ap, count);
	for (int i = 0; i < count; i++)
	{
		unsigned long long z = va_arg(ap, unsigned long long);
		printf("%d\n", z);
	}
	va_end(ap);

	return 1;
}

void MyFunc(const WCHAR* format, ...)
{
	WCHAR buf[1923];
	va_list vl;
	va_start(vl, format);

	wvsprintf(buf, format, vl);

	wprintf(L"%s", buf);

	va_end(vl);
}

int main()
{
	setlocale(LC_ALL, "");
	
	CSystemLog::GetInstance()->SetLogLevel(CSystemLog::LOG_LEVEL::eLOG_LEVEL_DEBUG);
	CSystemLog::GetInstance()->SetSystemLogDirectory(L"MyDirectory");

	CSystemLog::GetInstance()->SystemLog(L"BATTLE", CSystemLog::eLOG_LEVEL_DEBUG, L"테스3트입니다... %d", 2223);
	CSystemLog::GetInstance()->SystemLog(L"BATTLE3", CSystemLog::eLOG_LEVEL_DEBUG, L"테스3트입니다... %d", 2223);
	CSystemLog::GetInstance()->SystemLog(L"BAT3TLE", CSystemLog::eLOG_LEVEL_DEBUG, L"테스3트입니다... %d", 2223);
	CSystemLog::GetInstance()->SystemLog(L"BAT4LE", CSystemLog::eLOG_LEVEL_DEBUG, L"테스1트입니다... %d", 2223);
	CSystemLog::GetInstance()->SystemLog(L"BA42TTLE", CSystemLog::eLOG_LEVEL_DEBUG, L"테3스트입니다... %d", 2223);
	CSystemLog::GetInstance()->SystemLog(L"BA2TTLE", CSystemLog::eLOG_LEVEL_DEBUG, L"테스트입니다... %d", 2223);
	CSystemLog::GetInstance()->SystemLog(L"B1ATTLE", CSystemLog::eLOG_LEVEL_DEBUG, L"테스3트입니다... %d", 2223);

}
