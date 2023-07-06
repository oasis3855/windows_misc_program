#include "StdAfx.h"
#include "CookieCtrl.h"
#include "GlobalFunc.h"
#include <io.h>			// findfirst

// ************************************************************
// 自動削除 メインルーチン
// ************************************************************
BOOL DeleteCookie(void)
{
	char str_path[MAX_PATH];
	char str_m_ext[MAX_PATH];
	char str_m_exclude[MAX_PATH];
	int int_days;

	char str_srch_path[MAX_PATH*2];
	char str_full_path[MAX_PATH*2];

	char *str_ext;

	struct _finddata_t finddata;
	long hFind;

	// 各種設定値のセット
	strcpy(str_path, ::gstr_folder);
	strcpy(str_m_ext, ::gstr_ext);
	if(::gint_sw_exclude)
		strcpy(str_m_exclude, ::gstr_exclude);
	else
		strcpy(str_m_exclude, "");
	if(::gint_sw_days)
		int_days = ::gint_days;
	else
		int_days = -1;		// 負の数で対象外とする設定


	// パス名または拡張子が指定されていないとき
	if((strlen(str_path) == 0) || (strlen(str_m_ext) == 0)) return FALSE;

	// パスの最後は \ 記号
	if(str_path[strlen(str_path)-1] != '\\') strcat(str_path, "\\");


	// ************************************************************
	// 拡張子（複数指定）の切り分け   （例： *.txt;*.url）
	// ************************************************************
	str_ext = strtok(str_m_ext, ";");
	while(str_ext != NULL)
	{
		// 検索パスの完成
		sprintf(str_srch_path, "%s%s", str_path, str_ext);

	    if( (hFind = _findfirst((LPCSTR)str_srch_path, &finddata )) == -1L )
		{	// 1つ目のファイルが見つからない
			str_ext = strtok(NULL, ";");	// 次の拡張子
			continue;	
		}

		// ************************************************************
		// ワイルドカード指定によるファイルの検索
		// ************************************************************
		if(!(finddata.attrib & _A_SUBDIR))
		{
			sprintf(str_full_path, "%s%s", str_path, finddata.name);
			if(ChkExcludeFile(finddata.name, str_m_exclude, finddata.time_create, int_days))
				::unlink((LPCSTR)str_full_path);	// 削除
		}

		while(_findnext( hFind, &finddata ) == 0)
		{
			if(!(finddata.attrib & _A_SUBDIR))
			{
				sprintf(str_full_path, "%s%s", str_path, finddata.name);
				if(ChkExcludeFile(finddata.name, str_m_exclude, finddata.time_create, int_days))
					::unlink((LPCSTR)str_full_path);	// 削除
			}
		}
		str_ext = strtok(NULL, ";");	// 次の拡張子
	}
	return TRUE;
}

// ************************************************************
// 与えられたファイルが消去すべき対象かどうか判断する
//
// str_fname : 対象ファイル
// str_m_exclude : 対象外の文字列（セミコロンで複数指定可）
// time_create : 対象ファイルの作成日 (CTime)
// days : この日付以上立ったファイルは消去対象とする（負の数で判定外）
// 消去して OK の場合 TRUE
// ************************************************************
BOOL ChkExcludeFile(char *str_fname, char *str_m_exclude, time_t time_create, int days)
{
	char str_tmp[MAX_PATH];
	char *str_exclude;
	CString str_tmp_find;
	time_t time_now;

	_tzset();
	time(&time_now);

	// 日付指定があるとき
	if(days >= 0)
	{
		// 日付が指定より経過している場合 TRUE (一致文字列が何であろうとも)
		if(time_now > time_create + (long)(days*24*60*60)) return TRUE;
		// 一致文字列が指定されていない場合 期限がくるまで FALSE
		if(strlen(str_m_exclude) == 0) return FALSE;
		// アスタリスクは全ての文字列に一致と解釈し、期限がくるまで FALSE
		if(strcmp(str_m_exclude, "*") == 0) return FALSE;
	}
	else
	{	// 日付指定が無いとき
		// 一致文字列が指定されていない場合 直ちに消去 TRUE
		if(strlen(str_m_exclude) == 0) return TRUE;
	}

	// 文字列が一致するかどうか調べる
	strcpy(str_tmp, str_m_exclude);
	str_exclude = strtok(str_tmp, ";");
	while(str_exclude != NULL)
	{
		str_tmp_find = str_fname;
		// 一致した文字列が無い場合 -1 が返る Find 関数を使う
		if(str_tmp_find.Find((LPTSTR)str_exclude) != -1) return FALSE;	// 一致した
		str_exclude = strtok(NULL, ";");
	}

	// ケース１：日付指定があり、期限が到達していない場合で、文字列指定があり一致しない場合
	// ケース２：日付指定が無く、文字列指定があり一致しない場合
	//   は消去する TRUE
	return TRUE;
}

