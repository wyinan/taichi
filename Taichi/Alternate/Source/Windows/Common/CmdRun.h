#pragma once
// The base class of a cross flatform waitable event
// NOTE: support Windows only currently
#include "stdafx.h"
#include <windows.h>

class CmdRun
{
public:
			
	static DWORD RunCommandProcessor(const CString pszCmd)
	{
		STARTUPINFO si = { sizeof(STARTUPINFO) };
		si.dwFlags = STARTF_USESHOWWINDOW;
		PROCESS_INFORMATION pi = { 0 };

		CString strCurDir;
		(void)GetModuleFileName(NULL, strCurDir.GetBuffer(MAX_PATH), MAX_PATH - 1);
		strCurDir.ReleaseBuffer();
		strCurDir = strCurDir.Left(strCurDir.ReverseFind(_T('\\')));

		CString strCmdLine;
		strCmdLine.Format(_T("%s\\%s"), strCurDir, pszCmd);
		BOOL bRet = CreateProcess(NULL, (LPTSTR)(LPCTSTR)strCmdLine,
			NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
		DWORD dwExitCode = 0;
		if (bRet)
		{
			(void)CloseHandle(pi.hThread);
			//(void)WaitForSingleObject(pi.hProcess, INFINITE);
			//(void)GetExitCodeProcess(pi.hProcess, &dwExitCode);
			(void)CloseHandle(pi.hProcess);
		}
		return 0 == dwExitCode;
	}


	static DWORD RunCommandProcessor(const CString& csCmd, CString& strOutPut)
	{
		// TODO: Add your control notification handler code here
		DWORD dwRet = ERROR_INVALID_FUNCTION;

#if 0
		CString strCurDir;
		(void)GetModuleFileName(NULL, strCurDir.GetBuffer(MAX_PATH), MAX_PATH - 1);
		strCurDir.ReleaseBuffer();
		strCurDir = strCurDir.Left(strCurDir.ReverseFind(_T('\\')));
#endif
		CString strCmdLine;
#if 0
		strCmdLine.Format(_T("%s\\%s"), strCurDir, csCmd);
#endif
		strCmdLine.Format(_T("%s"), csCmd);

		PROCESS_INFORMATION pi = { 0 };

		SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES) };
		sa.lpSecurityDescriptor = NULL;
		sa.bInheritHandle = TRUE;

		STARTUPINFO si = { sizeof(STARTUPINFO) };

		DWORD dwWaitRet = WAIT_FAILED;

		HANDLE hRead = NULL;
		HANDLE hWrite = NULL;
		BOOL bRet = CreatePipe(&hRead, &hWrite, &sa, 0);
		if (!bRet)
		{
			goto final;
		}

		GetStartupInfo(&si);
		si.hStdError = hWrite;
		si.hStdOutput = hWrite;
		si.wShowWindow = SW_HIDE;
		si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;

		bRet = CreateProcess(NULL, (LPTSTR)(LPCTSTR)strCmdLine, NULL, NULL, TRUE,
			NULL, NULL, NULL, &si, &pi);	// TRUE:继承父进程句柄
		if (!bRet)
		{
			goto final;
		}

		(void)CloseHandle(pi.hThread);
		dwWaitRet = WaitForSingleObject(pi.hProcess, INFINITE);
		(void)GetExitCodeProcess(pi.hProcess, &dwRet);
		(void)CloseHandle(pi.hProcess);

		// 在ReadFile之前要关掉，否则ReadFile一直等待写入
		(void)CloseHandle(hWrite);

		if (dwWaitRet == WAIT_FAILED)
		{
			goto final;
		}

		char buffer[4096] = { 0 };	// 没有考虑WCHAR
		DWORD bytesRead = 0;
		do
		{
			if (ReadFile(hRead, buffer, 4096, &bytesRead, NULL))
			{
				strOutPut += buffer;
			}
			else
			{
				break;
			}
		} while (TRUE);
		
		dwRet = ERROR_SUCCESS;
	final:

		(void)CloseHandle(hRead);
		return dwRet;
	}
	
};


