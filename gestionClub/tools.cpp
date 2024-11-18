#include "stdafx.h"
#include "tools.h"

CFont* m_Font(){
    static CFont * p_Font = nullptr;
    if(!p_Font){
        p_Font = new CFont;
        p_Font->CreateFont(15, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
    }
    return p_Font;
}