/**
 * @file      PathUtils.cpp
 * @brief
 * @details
 * @author    Inan Wong
 * @contact   inanwong@gmail.com
 * @date      2014-09-07
 * @copyright Inan Wong Private License.
 */
#include "stdafx.h"
#include "PathUtils.h"
#include <memory>
#include <Shlwapi.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static BOOL sse2supported =
    ::IsProcessorFeaturePresent(PF_XMMI64_INSTRUCTIONS_AVAILABLE);


CString CPathUtils::GetFileFullName(const CString &csFilePath)
{
    // 文件路径，以'\'斜杠分隔的路径
    int nPos = csFilePath.ReverseFind('\\');

    // 获取文件全名，包括文件名和扩展名
    CString csFileFullName;
    csFileFullName = csFilePath.Right(csFilePath.GetLength() - nPos - 1);

    return csFileFullName;
}


CString CPathUtils::GetFileName(const CString &csFileFullName)
{
    int nPos = csFileFullName.ReverseFind('.');
    // 获取文件名
    CString csFileName = csFileFullName.Left(nPos);
    return csFileName;
}


CString CPathUtils::GetFileExtName(const CString &csFileFullName)
{
    int nPos = csFileFullName.ReverseFind('.');
    // 获取扩展名
    CString csFileExtName = csFileFullName.Right(csFileFullName.GetLength() - nPos - 1);
    return csFileExtName;
}


CString CPathUtils::GetAppDirectory(HMODULE hMod /* = NULL */)
{
    CString path;
    DWORD len = 0;
    DWORD bufferlen = MAX_PATH;     // MAX_PATH is not the limit here!
    path.GetBuffer(bufferlen);
    do
    {
        bufferlen += MAX_PATH;      // MAX_PATH is not the limit here!
        path.ReleaseBuffer(0);
        len = GetModuleFileName(hMod, path.GetBuffer(bufferlen + 1), bufferlen);
    }
    while(len == bufferlen);
    path.ReleaseBuffer();
    path = path.Left(path.ReverseFind('\\') + 1);
    return path;
}


CString CPathUtils::GetAppParentDirectory(HMODULE hMod /* = NULL */)
{
    CString path = GetAppDirectory(hMod);
    path = path.Left(path.ReverseFind('\\'));
    path = path.Left(path.ReverseFind('\\') + 1);
    return path;
}


CString CPathUtils::GetAppDataDirectory()
{
    PTSTR pszPath = NULL;
    if (SHGetKnownFolderPath(FOLDERID_RoamingAppData, KF_FLAG_CREATE, NULL,
                             &pszPath) != S_OK)
    {
        return CString();
    }

    CString path = pszPath;
    CoTaskMemFree(pszPath);

    path += _T("\\EverMind");
    if (!PathIsDirectory(path))
    {
        CreateDirectory(path, NULL);
    }

    return CString (path) + '\\';
}


CString CPathUtils::GetLocalAppDataDirectory()
{
    PTSTR pszPath = NULL;
    if (SHGetKnownFolderPath(FOLDERID_LocalAppData, KF_FLAG_CREATE, NULL,
                             &pszPath) != S_OK)
    {
        return CString();
    }

    CString path = pszPath;
    CoTaskMemFree(pszPath);

    path += _T("\\EverMind");
    if (!PathIsDirectory(path))
    {
        CreateDirectory(path, NULL);
    }

    return CString(path) + '\\';
}


BOOL CPathUtils::MakeSureDirectoryPathExists(LPCTSTR path)
{
    const size_t len = wcslen(path);
    const size_t fullLen = len + 10;
    std::unique_ptr<TCHAR[]> buf(new TCHAR[fullLen]);
    std::unique_ptr<TCHAR[]> internalpathbuf(new TCHAR[fullLen]);
    TCHAR *pPath = internalpathbuf.get();
    SECURITY_ATTRIBUTES attribs;

    SecureZeroMemory(&attribs, sizeof(SECURITY_ATTRIBUTES));

    attribs.nLength = sizeof(SECURITY_ATTRIBUTES);
    attribs.bInheritHandle = FALSE;

    ConvertToBackslash(internalpathbuf.get(), path, fullLen);
    if (wcsncmp(internalpathbuf.get(), L"\\\\?\\", 4) == 0)
        pPath += 4;
    do
    {
        SecureZeroMemory(buf.get(), fullLen * sizeof(TCHAR));
        TCHAR *slashpos = wcschr(pPath, '\\');
        if (slashpos)
            wcsncpy_s(buf.get(), fullLen, internalpathbuf.get(), slashpos - internalpathbuf.get());
        else
            wcsncpy_s(buf.get(), fullLen, internalpathbuf.get(), fullLen);
        CreateDirectory(buf.get(), &attribs);
        pPath = wcschr(pPath, '\\');
    }
    while ((pPath++) && (wcschr(pPath, '\\')));

    const BOOL bRet = CreateDirectory(internalpathbuf.get(), &attribs);

    return bRet;
}


void CPathUtils::ConvertToBackslash(LPTSTR dest, LPCTSTR src, size_t len)
{
    wcscpy_s(dest, len, src);
    TCHAR *p = dest;
    for (; *p != '\0'; ++p)
    {
        if (*p == '/')
        {
            *p = '\\';
        }
    }

    return;
}


/**
* @brief 获取当前进程对应的可执行文件文件所在目录
* @param [out] a pointer to a buffer.
* @param [in] the size of the buffer, in TCHARs. eg. MAX_PATH.
* @return the results
*/
BOOL CPathUtils::GetModuleDirectory(LPTSTR lpDirectory, DWORD dwChars)
{
	if (lpDirectory == NULL)
	{
		return FALSE;
	}
	DWORD dwRet = GetModuleFileName(NULL, lpDirectory, dwChars);
	if (dwRet == 0 || dwRet == dwChars)
	{
		return FALSE;
	}
	(_tcsrchr(lpDirectory, _T('\\')))[0] = _T('\0');
	return TRUE;
}

/**
* @brief 获取当前进程的当前目录
* @param [out] a pointer to a buffer.
* @param [in] the size of the buffer, in TCHARs. eg. MAX_PATH.
* @return the results
*/
BOOL CPathUtils::GetCurrentDirectoryEx(LPTSTR lpDirectory, DWORD dwChars)
{
	if (lpDirectory == NULL)
	{
		return FALSE;
	}
	DWORD dwRet = ::GetCurrentDirectory(dwChars, lpDirectory);
	if (dwRet == 0 || dwRet >= dwChars)
	{
		return FALSE;
	}
	return TRUE;
}


CString CPathUtils::GetCurrentDirectory()
{
	CString csDir;
	DWORD dwRet = ::GetCurrentDirectory(MAX_PATH - 1, csDir.GetBuffer(MAX_PATH));
	csDir.ReleaseBuffer();
	if (dwRet == 0 || dwRet >= MAX_PATH - 1)
	{
		csDir.Empty();
	}
	csDir.Append(_T("\\"));
	return csDir;
}


/**
* @brief 将当前模块所在路径设为当前目录
* @return TRUE
*/
BOOL CPathUtils::FixedCurrentDirectory()
{
	TCHAR lpModuleDir[MAX_PATH] = { 0 };
	BOOL bRet = GetModuleDirectory(lpModuleDir, MAX_PATH);
	if (!bRet)
	{
		return FALSE;
	}

	TCHAR lpCurDir[MAX_PATH] = { 0 };
	bRet = GetCurrentDirectoryEx(lpCurDir, MAX_PATH);
	if (!bRet)
	{
		return FALSE;
	}

	if (0 == _tcsicmp(lpModuleDir, lpCurDir))
	{
		return TRUE;
	}

	return (0 == _tchdir(lpModuleDir));
}