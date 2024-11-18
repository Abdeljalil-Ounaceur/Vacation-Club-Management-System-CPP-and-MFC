#pragma once
#include "afxcmn.h"
#include <afxdao.h>
#include "afxwin.h"


// Pays dialog

class Pays : public CDialogEx
{
	DECLARE_DYNAMIC(Pays)

public:
	Pays(CWnd* pParent = NULL);   // standard constructor
	virtual ~Pays();

// Dialog Data
	enum { IDD = IDD_PAYS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

	// Generated message map functions
	


public:
	CListCtrl m_ListPays;
	CString defaultQuery;
	void start();
	void UpdateFields();
	void ListData(CString query);
	void RefreshList();
	COleVariant GetComboContinentID();


	CDaoDatabase db;
	CDaoRecordset recset;
	

	CString m_CodePaysEdit;
	CString m_NomPaysEdit;
	CComboBox m_ContinentCombo;

	afx_msg void OnBnClickedPremier();
	afx_msg void OnBnClickedDernier();
	afx_msg void OnBnClickedNext();
	afx_msg void OnBnClickedPrevious();
	afx_msg void OnBnClickedTrouver();
	afx_msg void OnBnClickedActualiser();
	afx_msg void OnBnClickedSupprimer();
	afx_msg void OnBnClickedModifier();

	
	afx_msg void OnBnClickedAjouter();
	CEdit m_CodePaysControl;
	CEdit m_NomPaysControl;
};
