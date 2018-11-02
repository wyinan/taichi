/**
 * @file      PathUtils.h
 * @brief
 * @details
 * @author    Inan Wong
 * @contact   inanwong@gmail.com
 * @date      2014-09-07
 * @copyright Inan Wong Private License.
 */
#pragma once
#include <afx.h>

/**
 * @class CPathUtils
 * @brief
 */
class CPathUtils
{
public:

    static CString GetFileFullName(const CString &csFilePath);


    static CString GetFileName(const CString &csFileFullName);


    static CString GetFileExtName(const CString &csFileFullName);


    /**
    * Returns the path to the installation folder, in our case the EverMind/Application folder.
    * \remark the path returned has a trailing backslash
    */
    static CString GetAppDirectory(HMODULE hMod = NULL);


    /**
     * Returns the path to the installation parent folder, in our case the EverMind folder.
     * \remark the path returned has a trailing backslash
     */
    static CString GetAppParentDirectory(HMODULE hMod = NULL);


    /**
     * Returns the path to the application data folder, in our case the %APPDATA%EverMind folder.
     * \remark the path returned has a trailing backslash
     */
    static CString GetAppDataDirectory();


    static CString GetLocalAppDataDirectory();


    static BOOL MakeSureDirectoryPathExists(LPCTSTR path);


    static void ConvertToBackslash(LPTSTR dest, LPCTSTR src, size_t len);


	/**
	* @brief 获取当前进程对应的可执行文件文件所在目录
	* @param [out] a pointer to a buffer.
	* @param [in] the size of the buffer, in TCHARs. eg. MAX_PATH.
	* @return the results
	*/
	static BOOL GetModuleDirectory(LPTSTR lpDirectory, DWORD dwChars);


	/**
	* @brief 获取当前进程的当前目录
	* @param [out] a pointer to a buffer.
	* @param [in] the size of the buffer, in TCHARs. eg. MAX_PATH.
	* @return the results
	*/
	static BOOL GetCurrentDirectoryEx(LPTSTR lpDirectory, DWORD dwChars);


	static CString GetCurrentDirectory();


	/**
	* @brief 将当前模块所在路径设为当前目录
	* @return TRUE
	*/
	static BOOL FixedCurrentDirectory();
	
};