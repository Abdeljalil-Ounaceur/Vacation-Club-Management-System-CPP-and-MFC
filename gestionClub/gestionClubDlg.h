
// gestionClubDlg.h : header file
//

#pragma once
#include "Villages.h"
#include "Activites.h"
#include "Pays.h"
#include "Langues.h"

// CgestionClubDlg dialog
class CgestionClubDlg : public CDialogEx
{
// Construction
public:
	CgestionClubDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_GESTIONCLUB_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	CWnd* currentChild;
	CRect* rc;
	CWnd* cadre;
public:
	afx_msg void OnBnClickedBtnVillages();
	afx_msg void OnBnClickedBtnActivites();
	void ShowChild(CWnd* child);
	afx_msg void OnBnClickedBtnPays();
	afx_msg void OnBnClickedBtnLangues();
	afx_msg void OnFichierCreerBd();
	afx_msg void OnBnClickedCadre();
	afx_msg void OnBnClickedBtnPays2();
	afx_msg void OnAideApropos();
	afx_msg void OnBnClickedBtnCreebd();
	afx_msg void OnBnClickedBtnApropos();
	afx_msg void OnFichierQuitter();
	afx_msg void OnPagesVillages();
	afx_msg void OnPagesActivites();
	afx_msg void OnPagesLangues();
	afx_msg void OnPagesPays();
	afx_msg void OnBnClickedBtnQuitter();
};
