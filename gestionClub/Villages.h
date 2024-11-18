#pragma once
#include "afxcmn.h"
#include <afxdao.h>
#include "afxwin.h"


// Villages dialog

class Villages : public CDialogEx
{
	DECLARE_DYNAMIC(Villages)

public:
	Villages(CWnd* pParent = NULL);   // standard constructor
	virtual ~Villages();

// Dialog Data
	enum { IDD = IDD_VILLAGES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

	// Generated message map functions
	


public:
	CDaoDatabase db;
	CDaoRecordset recset;
	

	CString defaultQuery;
	CListCtrl m_ListVillages;
	CString m_CodeVillageEdit;
	CString m_NomVillageEdit;
	CComboBox m_PaysCombo;

	//VILLAGES
	COleVariant GetComboPaysID();
	void RefreshList();
	void UpdateFields();
	void ListData(CString query);

	afx_msg void OnBnClickedPremier();
	afx_msg void OnBnClickedDernier();
	afx_msg void OnBnClickedNext();
	afx_msg void OnBnClickedPrevious();
	afx_msg void OnBnClickedTrouver();
	afx_msg void OnBnClickedActualiser();
	afx_msg void OnBnClickedSupprimer();
	afx_msg void OnBnClickedModifier();
	afx_msg void OnBnClickedAjouter();
	

	//ACTIVITES
	CString defaultActivitesQuery;
	CDaoRecordset activitesRecset;
	CListCtrl m_ListCurrentActivites;
	CComboBox m_ActivitesCombo;
	CButton m_ActiviteCheckBox;
	BOOL m_ActiviteGratuite;

	COleVariant GetComboActiviteID();
	void RefreshActivites();
	void activitesUpdateFields();

	afx_msg void OnBnClickedPremierActivites();
	afx_msg void OnBnClickedPreviousActivites();
	afx_msg void OnBnClickedNextActivites();
	afx_msg void OnBnClickedDernierActivites();
	afx_msg void OnBnClickedDisassocierActivites();
	afx_msg void OnBnClickedAjouterActivites();
	afx_msg void OnBnClickedModifierActivites();


	//LANGUES
	CString defaultLanguesQuery;
	CDaoRecordset languesRecset;
	CListCtrl m_ListCurrentLangues;
	CComboBox m_LanguesCombo;

	COleVariant GetComboLangueID();
	void RefreshLangues();
	void languesUpdateFields();

	afx_msg void OnBnClickedPremierLangues();
	afx_msg void OnBnClickedPreviousLangues();
	afx_msg void OnBnClickedNextLangues();
	afx_msg void OnBnClickedDernierLangues();
	afx_msg void OnBnClickedDisassocierLangues();
	afx_msg void OnBnClickedAjouterLangues();
	CEdit m_CodeVillageControl;
	CEdit m_NomVillageControl;
};
