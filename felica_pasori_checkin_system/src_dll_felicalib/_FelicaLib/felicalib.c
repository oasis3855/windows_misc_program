/*
 felicalib - FeliCa access wrapper library

 Copyright (c) 2007, Takuya Murakami, All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are
 met:

 1. Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer. 

 2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution. 

 3. Neither the name of the project nor the names of its contributors
    may be used to endorse or promote products derived from this software
    without specific prior written permission. 

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/****
プリコンパイル済みヘッダを使わない設定とするため
プロジェクト － 設定 － このファイルを指定して － C++ － プリコンパイル済みヘッダ
で「使用しない」と設定する
****/

/**
  @file felicalib.c

  felicalib 本体
*/
#include "felicalib.h"
#include "felicaint.h"

/**
   @brief PaSoRi をオープンする
   @param[in] dummy ダミー (libpasori との互換性のため)
   @retval pasori ハンドル

   felica.dll をロード、初期化する
 */
pasori *pasori_open(char *dummy)
{
    pasori *p;

	if(!GetFelicaDllPath())
		return NULL;		// felica.dll が存在しない


    p = (pasori *)malloc(sizeof(pasori));
    
    /* open felica.dll */
//	p->hInstDLL = LoadLibrary(_T("H:\\Program Files\\Common Files\\Sony Shared\\FeliCaLibrary\\felica.dll"));
	p->hInstDLL = LoadLibrary(_T(GetFelicaDllPath()));
    if (p->hInstDLL == NULL) {
	free(p);
	return NULL;
    }
	
    /* resolve function pointers */
#define resolve_entry(f) p->f = (f ## _t)GetProcAddress(p->hInstDLL, #f)
    resolve_entry(initialize_library);
    resolve_entry(dispose_library);
    resolve_entry(open_reader_writer_auto);
    resolve_entry(close_reader_writer);
    resolve_entry(polling_and_get_card_information);
    resolve_entry(polling_and_request_system_code);
    resolve_entry(polling_and_search_service_code);
    resolve_entry(read_block_without_encryption);
    resolve_entry(write_block_without_encryption);

    if (!p->initialize_library()) {
	free(p);
	return NULL;
    }
    return p;
}

/**
   @brief PaSoRi ハンドルをクローズする
   @param[in] p pasoriハンドル

   felica.dll を解放する
*/
void pasori_close(pasori *p)
{
    p->dispose_library();
    FreeLibrary(p->hInstDLL);

	/**************
	p が開放されていないため、メモリーリークが起こっていた対策
	by INOUE.Hirokazu,  2007/11/30
	**************/

	free(p);
}

/**
   @brief PaSoRi を初期化する
   @param[in] p pasoriハンドル
   @return エラーコード
*/
int pasori_init(pasori *p)
{
    if (!p->open_reader_writer_auto()) {
	return -1;
    }
    return 0;
}

static felica *alloc_felica(pasori *p, uint16 systemcode)
{
    felica *f = (felica *)malloc(sizeof(felica));
    f->p = p;
    f->systemcode = H2NS(systemcode);

    return f;
}

/**
   @brief FeliCa をポーリングする
   @param[in] p pasoriハンドル
   @param[in] systemcode システムコード
   @param[in] RFU RFU (使用しない)
   @param[in] timeslot タイムスロット
   @return felicaハンドル
*/
felica *felica_polling(pasori *p, uint16 systemcode, uint8 RFU, uint8 timeslot)
{
    felica *f;
    POLLING polling;
    uint16 sc;
    uint8 number_of_cards = 0;
    CARD_INFO card_info;

    f = alloc_felica(p, systemcode);

    sc = H2NS(systemcode);
    polling.system_code = (uint8 *)&sc;
    polling.time_slot = timeslot;

    card_info.card_idm = f->IDm;
    card_info.card_pmm = f->PMm;

    if (!p->polling_and_get_card_information(&polling, &number_of_cards, &card_info) ||
	number_of_cards == 0) {
	free(f);
	return NULL;
    }

    return f;
}

/**
   @brief 暗号化されていないブロックを読み込む
   @param[in] f felicaハンドル
   @param[in] servicecode サービスコード
   @param[in] mode モード(使用しない)
   @param[in] addr ブロック番号
   @param[out] data データ(16バイト)
   @return エラーコード

   サービスコード、ブロック番号を指定してブロックを読み込む。
   システムコードは felica_polling で指定したものが使用される。
*/
int felica_read_without_encryption02(felica *f, int servicecode, int mode, uint8 addr, uint8 *data)
{
    INSTR_READ_BLOCK irb;
    OUTSTR_READ_BLOCK orb;

    uint8 service_code_list[2];
    uint8 block_list[2];
    uint8 status_flag1, status_flag2;
    uint8 result_number_of_blocks = 0;

    service_code_list[0] = servicecode & 0xff;
    service_code_list[1] = servicecode >> 8;
    block_list[0] = 0x80;
    block_list[1] = addr;

    irb.card_idm = f->IDm;
    irb.number_of_services = 1;
    irb.service_code_list = service_code_list;
    irb.number_of_blocks = 1;
    irb.block_list = block_list;

    orb.status_flag_1 = &status_flag1;
    orb.status_flag_2 = &status_flag2;
    orb.result_number_of_blocks = &result_number_of_blocks;
    orb.block_data = data;

    if (!f->p->read_block_without_encryption(&irb, &orb)) {
	return -1;
    }
    if (status_flag1 != 0) {
	return -1;
    }
    return 0;
}

/*------------- ここからは libpasori 互換でない (独自) ------------*/

/**
   @brief 暗号化されていないブロックを書き込む
   @param[in] f felicaハンドル
   @param[in] servicecode サービスコード
   @param[in] mode モード(使用しない)
   @param[in] addr ブロック番号
   @param[out] data データ(16バイト)
   @return エラーコード

   サービスコード、ブロック番号を指定してブロックを読み込む。
   システムコードは felica_polling で指定したものが使用される。
   注意!!! 本関数は未テスト!!!!!
*/
int felica_write_without_encryption(felica *f, int servicecode, uint8 addr, uint8 *data)
{
    INSTR_WRITE_BLOCK irb;
    OUTSTR_WRITE_BLOCK orb;

    uint8 service_code_list[2];
    uint8 block_list[2];
    uint8 status_flag1, status_flag2;

    service_code_list[0] = servicecode & 0xff;
    service_code_list[1] = servicecode >> 8;
    block_list[0] = 0x80;
    block_list[1] = addr;

    irb.card_idm = f->IDm;
    irb.number_of_services = 1;
    irb.service_code_list = service_code_list;
    irb.number_of_blocks = 1;
    irb.block_list = block_list;
    irb.block_data = data;

    orb.status_flag_1 = &status_flag1;
    orb.status_flag_2 = &status_flag2;

    if (!f->p->write_block_without_encryption(&irb, &orb)) {
	return -1;
    }
    if (status_flag1 != 0) {
	return -1;
    }
    return 0;
}

/**
   @brief felica ハンドル解放
   @param[in] f felica ハンドル
*/
void felica_free(felica *f)
{
    free(f);
}

/**
   @brief IDm 取得
   @param[in] f felica ハンドル
   @param[out] buf IDm を格納するバッファ(8バイト)
*/
void felica_getidm(felica *f, uint8 *buf)
{
    memcpy(buf, f->IDm, 8);
}

/**
   @brief PMm 取得
   @param[in] f felica ハンドル
   @param[out] buf PMm を格納するバッファ(8バイト)
*/
void felica_getpmm(felica *f, uint8 *buf)
{
    memcpy(buf, f->PMm, 8);
}

/**
   @brief システムコードの列挙
   @param[in] p pasoriハンドル
   @return felicaハンドル

   結果は、felica構造体の num_system_code/system_code に格納される。
   なお、システムコードのエンディアンは逆に格納されているので注意すること。
*/
felica * felica_enum_systemcode(pasori *p)
{
    felica *f;
    POLLING polling;
    CARD_INFO card_info;
    INSTR_REQ_SYSTEM_CODE irs;
    OUTSTR_REQ_SYSTEM_CODE ors;

    f = alloc_felica(p, POLLING_ANY);

    polling.system_code = (uint8 *)&f->systemcode;
    polling.time_slot = 0;

    card_info.card_idm = f->IDm;
    card_info.card_pmm = f->PMm;

    irs.card_idm = f->IDm;
    
    ors.system_code_list = (uint8 *)f->system_code;

    if (!f->p->polling_and_request_system_code(&polling, &irs, &card_info, &ors)) {
	free(f);
	return NULL;
    }

    f->num_system_code = ors.number_of_system_codes;
    return f;
}

/**
   @brief サービス/エリアコードの列挙
   @param[in] p pasoriハンドル
   @param[in] systemcode システムコード
   @return felicaハンドル

   結果は、felica構造体の num_area_code/area_code/end_service_code および
   num_service_code/service_code に格納される。
*/
felica * felica_enum_service(pasori *p, uint16 systemcode)
{
    felica *f;

    POLLING polling;
    CARD_INFO card_info;
    INSTR_SEARCH_SERVICE iss;
    OUTSTR_SEARCH_SERVICE oss;

    f = alloc_felica(p, systemcode);

    polling.system_code = (uint8 *)&f->systemcode;
    polling.time_slot = 0;

    card_info.card_idm = f->IDm;
    card_info.card_pmm = f->PMm;

    iss.buffer_size_of_area_codes = MAX_AREA_CODE;
    iss.buffer_size_of_service_codes = MAX_SERVICE_CODE;
    iss.offset_of_area_service_index = 0;

    oss.num_area_codes = 10;
    oss.num_service_codes = 10;
    oss.service_code_list     = (uint8 *)f->service_code;
    oss.area_code_list        = (uint8 *)f->area_code;
    oss.end_service_code_list = (uint8 *)f->end_service_code;
    
    if (!f->p->polling_and_search_service_code(&polling, &iss, &card_info, &oss)) {
	free(f);
	return NULL;
    }

    f->num_area_code = oss.num_area_codes;
    f->num_service_code = oss.num_service_codes;

    return f;
}


/***************************************************
追加 by INOUE. Hirokazu 2007/11/30
***************************************************/

#include <stdio.h>
#include <sys/stat.h>

/**************
felica.dllの位置をレジストリから得る

felica.dllが存在する場合は、DLLへのフルパス名文字列へのポインタを返す
felica.dllが存在しない場合は、NULLを返す
***************/
#define STR_REGKEY_FELICA			"Software\\Sony Corporation\\FeliCa Reader Writer Control Library"
#define STR_REGKEY_FELICA_PLUGINS	"plugins_home_directory"

char *GetFelicaDllPath(void)
{
	HKEY hCU;
	DWORD lpType;
	ULONG ulSize = MAX_PATH;
	static char sTemp[MAX_PATH];
	char *sRootPtr;
	struct stat statInfo;

	strcpy(sTemp, "");			// パスの初期化（パスを得るモードのみ）

	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, STR_REGKEY_FELICA, 0, KEY_QUERY_VALUE, &hCU) != ERROR_SUCCESS)
		return NULL;	// キーが存在しない時

	RegQueryValueEx( hCU, STR_REGKEY_FELICA_PLUGINS, NULL, &lpType, (unsigned char *)&sTemp, &ulSize);
	RegCloseKey(hCU);

	// パスの plugins という部分を削除する
	sRootPtr = strrchr(sTemp, '\\');
	if(sRootPtr == NULL)
		return NULL;	// '\' 文字が存在しないことはありえない
	strcpy(sRootPtr, "\\felica.dll");	// felica.dll のフルパス名完成

	// ファイルの存在をチェック
	if(stat(sTemp, &statInfo) != 0)
		return NULL;

	//felica.dllのフルパス名を返す
	return sTemp;

}

/**************
サービスの状態（読書、保護等）の文字列を作成
 （felicalib の FelicaDump.c の主要プログラム構造をコピーした）

引数
uint16 s : サービスコード

戻り値
サービス状態文字列へのポインタ
***************/

char *printserviceinfo(uint16 s)
{
	char *ident;
	static char sTemp[128];

	switch ((s >> 1) & 0xf) {
	case 0: ident = "Area Code"; break;
	case 4: ident = "Random Access R/W"; break; 
	case 5: ident = "Random Access Read only"; break; 
	case 6: ident = "Cyclic Access R/W"; break; 
	case 7: ident = "Cyclic Access Read only"; break; 
	case 8: ident = "Purse (Direct)"; break;
	case 9: ident = "Purse (Cashback/decrement)"; break;
	case 10: ident = "Purse (Decrement)"; break;
	case 11: ident = "Purse (Read only)"; break;
	default: ident = "INVALID or UNKOWN"; break;
	}

	sprintf(sTemp, "# Serivce code = %04X : %s", s, ident);
	if ((s & 0x1) == 0)
	{
		strcat(sTemp, " (Protected)");
    }

	strcat(sTemp, "\r\n");

	return sTemp;
}


/**************
16進数を文字列に変換する
IDm PMm をテキスト表示するときに使用
 （felicalib の FelicaDump.c の主要プログラム構造をコピーした）

引数
uint8 *addr : 変換元のバイナリデータ
int n : 変換元のバイト数
char *sBuf : 変換した文字列を格納する文字列へのポインタ（必要な桁数は確保されていること）
***************/
#include <stdio.h>
void HexDumpToStr(uint8 *addr, char *sBuf, int n)
{
	int i;
	char sTemp[4];

	strcpy(sBuf, "");	/* 文字列の初期化 */

	for (i = 0; i < n; i++) {
		// DLL化に伴い、IDの各数値の間にスペースが入らないように変更した   2008/10/11
		// sprintf(sTemp, "%02X", addr[i]);
		sprintf(sTemp, "%02X", addr[i]);
		strcat(sBuf, sTemp);
    }

	return;
}
