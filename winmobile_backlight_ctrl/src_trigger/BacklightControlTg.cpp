// BacklightControlTg.cpp : Defines the entry point for the application.
//

// ***********************
// BacklightControl for CE  (Windows CE のバックライト制御）
// 
// Copyright (C) 2008 INOUE. Hirokazu
//
// 
// http://www.opensource.jp/gpl/gpl.ja.html
// このプログラムはフリーソフトウェアです。あなたはこれを、フリーソフトウェ
// ア財団によって発行された GNU 一般公衆利用許諾契約書(バージョン2か、希
// 望によってはそれ以降のバージョンのうちどれか)の定める条件の下で再頒布
// または改変することができます。
// 
// このプログラムは有用であることを願って頒布されますが、*全くの無保証* 
// です。商業可能性の保証や特定の目的への適合性は、言外に示されたものも含
// め全く存在しません。詳しくはGNU 一般公衆利用許諾契約書をご覧ください。
// 
// あなたはこのプログラムと共に、GNU 一般公衆利用許諾契約書の複製物を一部
// 受け取ったはずです。もし受け取っていなければ、フリーソフトウェア財団ま
// で請求してください(宛先は the Free Software Foundation, Inc., 59
// Temple Place, Suite 330, Boston, MA 02111-1307 USA)。
//

#include "stdafx.h"
#include "BacklightControlTg.h"

#include <pm.h>


int WINAPI WinMain(	HINSTANCE hInstance,
					HINSTANCE hPrevInstance,
					LPTSTR    lpCmdLine,
					int       nCmdShow)
{
	CEDEVICE_POWER_STATE devState;

	if(::GetDevicePower(_T("BKL1:"), POWER_NAME, &devState) != ERROR_SUCCESS)
	{
		// バックライトの状態の読み出しに失敗
		return 0;
	}

	if(devState == D0)
	{	// バックライトがONのとき

		// バックライトを OFFにする
		::SetDevicePower(_T("BKL1:"), POWER_NAME, D4);	// OFF

	}
	else
	{	// バックライトがOFFのとき （ON以外のときすべて）

		// バックライトを ONにする

		::SetDevicePower(_T("BKL1:"), POWER_NAME, D4);	// OFF (一旦OFFにしないと反応しない場合あり)
		::Sleep(200);
		::SetDevicePower(_T("BKL1:"), POWER_NAME, PwrDeviceUnspecified);	// 自動制御モード

	}

	return 0;
}

