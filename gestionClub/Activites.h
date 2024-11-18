#pragma once
#include "afxcmn.h"
#include <afxdao.h>
#include "afxwin.h"


// Activites dialog

class Activites : public CDialogEx
{
	DECLARE_DYNAMIC(Activites)

public:
	Activites(CWnd* pParent = NULL);   // standard constructor
	virtual ~Activites();

// Dialog Data
	enum { IDD = IDD_ACTIVITES_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

	// Generated message map functions
	


public:
	CListCtrl m_ListActivites;
	CString defaultQuery;
	void start();
	void UpdateFields();
	void ListData(CString query);
	void RefreshList();


	CDaoDatabase db;
	CDaoRecordset recset;
	

	CString m_Designation;

	afx_msg void OnBnClickedPremier();
	afx_msg void OnBnClickedDernier();
	afx_msg void OnBnClickedNext();
	afx_msg void OnBnClickedPrevious();
	afx_msg void OnBnClickedTrouver();
	afx_msg void OnBnClickedActualiser();
	afx_msg void OnBnClickedSupprimer();
	afx_msg void OnBnClickedModifier();
	afx_msg void OnBnClickedAjouter();

	//VILLAGES
	CString defaultVillagesQuery;
	CDaoRecordset villagesRecset;
	CListCtrl m_ListCurrentVillages;
	CComboBox m_VillagesCombo;
	CButton m_VillageCheckBox;
	BOOL m_VillageGratuite;

	COleVariant GetComboVillageID();
	void RefreshVillages();
	void villagesUpdateFields();

	afx_msg void OnBnClickedPremierVillages();
	afx_msg void OnBnClickedPreviousVillages();
	afx_msg void OnBnClickedNextVillages();
	afx_msg void OnBnClickedDernierVillages();
	afx_msg void OnBnClickedDisassocierVillages();
	afx_msg void OnBnClickedAjouterVillages();
	afx_msg void OnBnClickedModifierVillages();
	CEdit m_DesignationControl;
};
