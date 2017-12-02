//for mingw
#ifndef _WIN32_IE
#define _WIN32_IE 0x0500
#endif

#include "const.h"
#include "utils.h"
#include "tabctl.h"
#include "tabsubmenu.h"

extern HWND hwnd_tab_ctl;
extern HINSTANCE hinst_module;
HMENU hmenu_tab_ctl_sub_menu;
static WNDPROC old_tab_ctl_proc = NULL;
static int last_row_count = 1;


static COLORREF color_table[10] =
{
	RGB(255,255,255),	//default
	RGB(204,182,238),	//c
	RGB(210,227,178),	//h
	RGB(219,236,249),	//txt
	RGB(249,179,180),	//js
	RGB(255,241,134),	//vb
};

//��������
static void SiTabCtl_AddCloseItem(void);

static LRESULT CALLBACK TabCtlSubClass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_LBUTTONDBLCLK:
		{
			SiTabCtl_OnLButtonDblClk();
		}
		break;
	case WM_RBUTTONDOWN:
	    {
	        SiTabCtrl_OnRButtonClk();
	    }
	    break;
    case WM_COMMAND:
        {
            // ȡ���һ��˵���ԴIDֵ, ���ж��û�ѡ�����ĸ��˵���
            switch(LOWORD(wParam))
            {
            case IDM_CLOSE_CUR_FILE:
                SiTabCtl_OnLButtonDblClk();
                break;
            case IDM_CLOSE_OTHER_FILE:
                SiTabSubMenu_CloseOtherFile();
                break;
            case IDM_CLOSE_ALL_FILE:
                SiTabSubMenu_CloseAllFile();
                break;
            case IDM_OPEN_CUR_DIR:
                SiTabSubMenu_OpenFileFolder();
                break;
            case IDM_OPEN_CUR_DIR_CMDLINE:
                SiTabSubMenu_OpenFileFolderCmdLine();
                break;
            case IDM_SET_ONLY_READ:
                SiTabSubMenu_SetFileAttributes();
                break;
            case IDM_CLR_ONLY_READ:
                SiTabSubMenu_ClearFileAttributes();
                break;
            case IDM_CPY_FILE_PATH_TO_BOARD:
                SiTabSubMenu_CpyFilePathToClipBoard();
                break;
            case IDM_CPY_FILE_NAME_TO_BOARD:
                SiTabSubMenu_CpyFileNameToClipBoard();
                break;
            case IDM_CPY_CUR_PATH_TO_BOARD:
                SiTabSubMenu_CpyCurPathToClipBoard();
                break;
            case IDM_SVN_UPDATE_CUR_FILE:
                SiTabSubMenu_SvnUpdateCurFile();
                break;
            case IDM_SVN_UPDATE_CUR_PATH:
                SiTabSubMenu_SvnUpdateCurPath();
                break;
            default:
                break;
            }
        }

    default:
        break;
	}

	LRESULT lr = CallWindowProc(old_tab_ctl_proc, hWnd, uMsg, wParam, lParam);
	return lr;
}

void SiTabCtl_Create(HWND parent)
{
    hmenu_tab_ctl_sub_menu = LoadMenu(hinst_module, IDM_TAB_CTL_SUB_MENU);
    if (hmenu_tab_ctl_sub_menu == NULL)
    {
        MessageBox(hwnd_tab_ctl, "���ر�ǩ�Ӳ˵�ʧ��!", "ERROR", MB_OK);
    }

    /*
	hwnd_tab_ctl = CreateWindowEx(0, WC_TABCONTROL, "",
								WS_CHILD|WS_VISIBLE|TCS_SINGLELINE|TCS_RAGGEDRIGHT|TCS_SCROLLOPPOSITE|TCS_FOCUSNEVER|TCS_OWNERDRAWFIXED|WS_CLIPSIBLINGS,
								0,0,0,SI_TAB_HEIGHT,parent,hmenu_tab_ctl_sub_menu,hinst_module,NULL);
    */
    /*
    hwnd_tab_ctl = CreateWindowEx(0, WC_TABCONTROL, "",
								WS_CHILD|WS_VISIBLE|TCS_BUTTONS|TCS_FLATBUTTONS|TCS_BOTTOM|TCS_SINGLELINE|TCS_RAGGEDRIGHT|TCS_SCROLLOPPOSITE|TCS_FOCUSNEVER|TCS_OWNERDRAWFIXED|WS_CLIPSIBLINGS,
								0,0,0,SI_TAB_HEIGHT,parent,hmenu_tab_ctl_sub_menu,hinst_module,NULL);

    */

    hwnd_tab_ctl = CreateWindowEx(0, WC_TABCONTROL, "",
								WS_CHILD|WS_VISIBLE|TCS_SINGLELINE|TCS_FOCUSNEVER|TCS_FORCEICONLEFT|TCS_OWNERDRAWFIXED|WS_CLIPSIBLINGS,
								0,0,0,SI_TAB_HEIGHT,parent,hmenu_tab_ctl_sub_menu,hinst_module,NULL);

	HFONT font = CreateFont(16,0,0,0,FW_NORMAL,FALSE,FALSE,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH | FF_SWISS,"Courier New");
	SendMessage(hwnd_tab_ctl,WM_SETFONT,(WPARAM)font,MAKELPARAM(1,0));

	SiTabCtl_AddCloseItem();
	last_row_count = 1;
	old_tab_ctl_proc = (WNDPROC)GetWindowLong(hwnd_tab_ctl, GWL_WNDPROC);
	if(old_tab_ctl_proc == NULL)
	{
		OutputDebugString("old_tab_ctl_proc null!");
	}
	SetWindowLong(hwnd_tab_ctl, GWL_WNDPROC, (DWORD)TabCtlSubClass);

    //Init
	EnableMenuItem(hmenu_tab_ctl_sub_menu, IDM_SET_ONLY_READ, MF_ENABLED);
    EnableMenuItem(hmenu_tab_ctl_sub_menu, IDM_CLR_ONLY_READ, MF_GRAYED);
}

//��ȡҳ����
int SiTabCtl_GetItemCount(void)
{
	//��"ȫ���ر�"��ťҲ������
	return TabCtrl_GetItemCount(hwnd_tab_ctl);
}

//��ȡ����
int SiTabCtl_GetRowCount(void)
{
	int row = TabCtrl_GetRowCount(hwnd_tab_ctl);
	if(row == 0)
		row = 1;
	return row;
}

//��ȡ�߶�
int SiTabCtl_GetHeight(void)
{
	return (SiTabCtl_GetRowCount()*SI_TAB_HEIGHT);
}

//��ӹر�Item
static void SiTabCtl_AddCloseItem(void)
{
	RECT rect;
	TCITEM tci;
	memset(&tci,0,sizeof(TCITEM));
	tci.mask = TCIF_TEXT;
	tci.pszText = "ȫ���ر�";
	TabCtrl_InsertItem(hwnd_tab_ctl,SI_TAB_CLOSE_IDX,&tci);
	//���ù̶��߶�
	SendMessage(hwnd_tab_ctl,TCM_GETITEMRECT,SI_TAB_CLOSE_IDX,(LPARAM)&rect);
	SendMessage(hwnd_tab_ctl,TCM_SETITEMSIZE,SI_TAB_CLOSE_IDX,MAKELPARAM(rect.right-rect.left+1,SI_TAB_HEIGHT));
}

//���Item
void SiTabCtl_AddItem(char* title,HWND hwnd)
{
	//����
	int idx = SiTabCtl_GetItemCount();
	RECT rect;
	TCITEM tci;
	memset(&tci,0,sizeof(TCITEM));
	tci.mask = TCIF_TEXT | TCIF_PARAM;
	tci.pszText = (LPSTR)title;
	tci.lParam = (LPARAM)hwnd;
	TabCtrl_InsertItem(hwnd_tab_ctl,idx,&tci);
	//���ù̶��߶�
	SendMessage(hwnd_tab_ctl,TCM_GETITEMRECT,idx,(LPARAM)&rect);
	SendMessage(hwnd_tab_ctl,TCM_SETITEMSIZE,idx,MAKELPARAM(rect.right-rect.left+1,SI_TAB_HEIGHT));
}

//����item
int SiTabCtl_FindItem(HWND hwnd)
{
	int rtv = -1;
	int i;
	int count = SiTabCtl_GetItemCount();
	TCITEM tci;
	for(i=0;i<count;i++)
	{
		memset(&tci,0,sizeof(TCITEM));
		tci.mask = TCIF_TEXT | TCIF_PARAM;
		TabCtrl_GetItem(hwnd_tab_ctl,i,&tci);
		if((HWND)tci.lParam == hwnd)
		{
			rtv = i;
			break;
		}
	}

	return rtv;
}

//ɾ��Item
void SiTabCtl_DelItem(HWND hwnd)
{
	int idx = SiTabCtl_FindItem(hwnd);
	if(idx == -1)
		return;
	TabCtrl_DeleteItem(hwnd_tab_ctl,idx);
}

//����Item����
void SiTabCtl_SetItemText(HWND hwnd,char* text)
{
	int idx = SiTabCtl_FindItem(hwnd);
	if(idx == -1)
		return;
	TCITEM tci;
	memset(&tci,0,sizeof(TCITEM));
	tci.mask = TCIF_TEXT | TCIF_PARAM;
	TabCtrl_GetItem(hwnd_tab_ctl,idx,&tci);

	tci.pszText = (LPSTR)text;
	TabCtrl_SetItem(hwnd_tab_ctl,idx,&tci);
}

//ѡ��Item
void SiTabCtl_SetCurItem(HWND hwnd)
{
	int idx = SiTabCtl_FindItem(hwnd);
	if(idx == -1)
		return;
	TabCtrl_SetCurSel(hwnd_tab_ctl,idx);
}

//��ȡ��ǰItem
int SiTabCtl_GetCurItem(void)
{
	return TabCtrl_GetCurSel(hwnd_tab_ctl);
}

//�ж��Ƿ������ı�
BOOL SiTabCtl_IsRowChanged(void)
{
	int rtv = FALSE;
	int row = SiTabCtl_GetRowCount();
	if(last_row_count != row)
	{
		rtv = TRUE;
		last_row_count = row;
	}

	return rtv;
}

//λ�øı�
void SiTabCtl_OnPosChanging(WINDOWPOS* pos)
{
	if(!(pos->flags & SWP_NOSIZE))
	{
		int row;
		SetWindowPos(hwnd_tab_ctl,NULL,pos->x,pos->y,pos->cx,SI_TAB_HEIGHT,pos->flags);
		row = SiTabCtl_GetRowCount();
		SetWindowPos(hwnd_tab_ctl,NULL,0,0,pos->cx,SI_TAB_HEIGHT*row,SWP_NOMOVE);
	}
}

//��ǩѡ�иı�
void SiTabCtl_OnSelChange(void)
{
	HWND hwnd;
	int idx = SiTabCtl_GetCurItem();
	TCITEM tci;
	memset(&tci,0,sizeof(TCITEM));
	tci.mask = TCIF_TEXT | TCIF_PARAM;
	TabCtrl_GetItem(hwnd_tab_ctl,idx,&tci);
	hwnd = (HWND)tci.lParam;
	SendMessage(GetParent(hwnd),WM_MDIACTIVATE,(WPARAM)hwnd,0);
}

//˫��ѡ�, �رյ�ǰ�ļ�
void SiTabCtl_OnLButtonDblClk(void)
{
	HWND hwnd;
	int idx = SiTabCtl_GetCurItem();
	if(idx == -1)
		return;

	TCITEM tci;
	memset(&tci,0,sizeof(TCITEM));
	tci.mask = TCIF_TEXT | TCIF_PARAM;
	TabCtrl_GetItem(hwnd_tab_ctl,idx,&tci);
	hwnd = (HWND)tci.lParam;

	SendMessage(hwnd, WM_SYSCOMMAND, SC_CLOSE, 0);
}

//����ѡ�
void SiTabCtl_OnLButtonClk(void)
{
	int rtv;
	int idx = SiTabCtl_GetCurItem();
	int count = SiTabCtl_GetItemCount();
	if(idx != 0)
		return;//������Ǳ�ǩ"ȫ���ر�", ֱ�ӷ��ؼ��ɡ�
	if(count == 0)
		return;

	rtv = MessageBox(hwnd_tab_ctl,"�Ƿ�ر����б�ǩ?","sitab plugin by Red_angelX",MB_OKCANCEL|MB_ICONQUESTION|MB_DEFBUTTON2);
	if(rtv == IDCANCEL)
		return;

	while(count > 0)
	{
		HWND hwnd;
		TCITEM tci;
		memset(&tci, 0, sizeof(TCITEM));
		tci.mask = TCIF_TEXT | TCIF_PARAM;
		TabCtrl_GetItem(hwnd_tab_ctl, count-1, &tci);
		hwnd = (HWND)tci.lParam;

		SendMessage(hwnd,WM_SYSCOMMAND,SC_CLOSE,0);
		count--;
	}
}

//�һ�ѡ�
void SiTabCtrl_OnRButtonClk(void)
{
    //MessageBox(hwnd_tab_ctl, "�һ���ǩ", "��Ӧ", MB_OK);
    POINT tPos;
    int idx = SiTabCtl_GetCurItem();//��������ֵ������Ӧ�ı�ǩֵ������Ӧ������
    int count = SiTabCtl_GetItemCount();
	if(idx == count)
		return;
	if(count == 0)
	    return;

	GetCursorPos(&tPos);//�Ե�ǰ�����Ϊ��׼
    HMENU hpop = GetSubMenu(hmenu_tab_ctl_sub_menu, 0);
    TrackPopupMenu(hpop, TPM_LEFTALIGN, tPos.x, tPos.y, NULL, hwnd_tab_ctl, NULL);
}

//�ػ�ѡ�
void SiTabCtl_OnDrawItem(DRAWITEMSTRUCT* item)
{
	HBRUSH hBrush;
	TCITEM tci;
	char text[SI_BUF_SIZE];
	int type;

	memset(&tci,0,sizeof(TCITEM));
	tci.mask = TCIF_TEXT | TCIF_STATE;
	tci.pszText = (LPSTR)text;
	tci.cchTextMax = SI_BUF_SIZE;
	TabCtrl_GetItem(hwnd_tab_ctl,item->itemID,&tci);

	type = GetColorIndex(text);
	//������ɫΪhdc�����ھ��Σ�����ɫ��ʵ��ˢ
	hBrush = CreateSolidBrush(color_table[type]);
	if(SiTabCtl_GetCurItem() == item->itemID)
	{
		hBrush = CreateSolidBrush(RGB(255,0,0));
	}
    //SetTextColor(lpDrawItem->hDC, RGB(0, 0, 255));
    //FrameRect(item->hDC,&item->rcItem,hBrush);
    FillRect(item->hDC, &item->rcItem, hBrush);
    DeleteObject(hBrush);
    SetBkMode(item->hDC, TRANSPARENT);
    DrawText(item->hDC, text, strlen(text), &item->rcItem, DT_CENTER|DT_LEFT|DT_VCENTER|DT_SINGLELINE);
}

HWND SiTabCtl_GetItemWnd(int idx)
{
	HWND hwnd;
	TCITEM tci;
	memset(&tci, 0, sizeof(TCITEM));
	tci.mask = TCIF_TEXT | TCIF_PARAM;
	TabCtrl_GetItem(hwnd_tab_ctl, idx, &tci);
	hwnd = (HWND)tci.lParam;
	return hwnd;
}

