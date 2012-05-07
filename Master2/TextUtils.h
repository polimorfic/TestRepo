#pragma once

int ReplaceSubString(CString& strSource, LPCTSTR lpFind, LPCTSTR lpReplace, BOOL bMatchCase);
int ReplaceSubStringConvertAW(CStringA& sSourceA, UINT nCodePage, CStringW& sSourceW, LPCTSTR lpFind, LPCTSTR lpReplace, BOOL bMatchCase);
int ReplaceStringArray(CStringArray& arrStrings, LPCTSTR lpFind, LPCTSTR lpReplace, BOOL bMatchCase);

CString FormatStringArray(const CStringArray& arrStrings, LPCTSTR lpSeps);
void SplitStringIntoArray(const CString& str, LPCTSTR lpSeps, CStringArray& arrStrings);

int FindSubString(LPCSTR lpSource, LPCSTR lpFind, BOOL bMatchCase);
int FindSubString(LPCWSTR lpSource, LPCWSTR lpFind, BOOL bMatchCase);
int RFindSubString(LPCSTR lpSource, LPCSTR lpFind, BOOL bMatchCase); // reverse
int RFindSubString(LPCWSTR lpSource, LPCWSTR lpFind, BOOL bMatchCase); // reverse

LPCSTR FindBoundedString(LPCSTR lpSource, LPCSTR lpLeftBound, LPCSTR lpRightBound, StringA& sValue);
