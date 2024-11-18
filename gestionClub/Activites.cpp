// Activites.cpp : implementation file
//

#include "stdafx.h"
#include "gestionClub.h"
#include "Activites.h"
#include "afxdialogex.h"


// Activites dialog

IMPLEMENT_DYNAMIC(Activites, CDialogEx)

Activites::Activites(CWnd* pParent /*=NULL*/)
	: CDialogEx(Activites::IDD, pParent),db(CDaoDatabase()),recset(&db),villagesRecset(&db)
	, m_Designation(_T("")),m_VillageGratuite(FALSE)

{
	
	defaultQuery = L"SELECT No_Activite, Designation FROM Activites";
	defaultVillagesQuery = L"SELECT Proposer.No_Village as No_Village, Nom, Gratuite, No_Activite FROM Proposer INNER JOIN Village ON Village.No_Village = Proposer.No_Village";
	db.Open(L"base.mdb");
	recset.Open(AFX_DAO_USE_DEFAULT_TYPE, defaultQuery, NULL);
	
	Create(Activites::IDD, pParent);
	ShowWindow(SW_SHOW);
}

Activites::~Activites()
{
	if(villagesRecset.IsOpen()){
		recset.Close();
	}

	if(recset.IsOpen()){
		recset.Close();
	}

	if(db.IsOpen()){
		db.Close();
	}
}

void Activites::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ACTIVITES, m_ListActivites);
	DDX_Text(pDX, IDC_DESIGNATION_EDIT, m_Designation);
	DDV_MaxChars(pDX, m_Designation, 30);

	DDX_Control(pDX, IDC_LIST_VILLAGES, m_ListCurrentVillages);
	DDX_Control(pDX, IDC_COMBO_VILLAGES, m_VillagesCombo);
	DDX_Control(pDX, IDC_VILLAGE_GRATUITE, m_VillageCheckBox);
	DDX_Check(pDX, IDC_VILLAGE_GRATUITE, m_VillageGratuite);
	DDX_Control(pDX, IDC_DESIGNATION_EDIT, m_DesignationControl);
}


BEGIN_MESSAGE_MAP(Activites, CDialogEx)
	ON_BN_CLICKED(IDC_PREMIER, &Activites::OnBnClickedPremier)
	ON_BN_CLICKED(IDC_DERNIER, &Activites::OnBnClickedDernier)
	ON_BN_CLICKED(IDC_NEXT, &Activites::OnBnClickedNext)
	ON_BN_CLICKED(IDC_PREVIOUS, &Activites::OnBnClickedPrevious)
	ON_BN_CLICKED(IDC_TROUVER, &Activites::OnBnClickedTrouver)
	ON_BN_CLICKED(IDC_ACTUALISER, &Activites::OnBnClickedActualiser)
	ON_BN_CLICKED(IDC_SUPPRIMER, &Activites::OnBnClickedSupprimer)
	ON_BN_CLICKED(IDC_MODIFIER, &Activites::OnBnClickedModifier)
	ON_BN_CLICKED(IDC_AJOUTER, &Activites::OnBnClickedAjouter)

	ON_BN_CLICKED(IDC_PREMIER_VILLAGES, &Activites::OnBnClickedPremierVillages)
	ON_BN_CLICKED(IDC_PREVIOUS_VILLAGES, &Activites::OnBnClickedPreviousVillages)
	ON_BN_CLICKED(IDC_NEXT_VILLAGES, &Activites::OnBnClickedNextVillages)
	ON_BN_CLICKED(IDC_DERNIER_VILLAGES, &Activites::OnBnClickedDernierVillages)
	ON_BN_CLICKED(IDC_DISASSOCIER_VILLAGES, &Activites::OnBnClickedDisassocierVillages)
	ON_BN_CLICKED(IDC_AJOUTER_VILLAGES, &Activites::OnBnClickedAjouterVillages)
	ON_BN_CLICKED(IDC_MODIFIER_VILLAGES, &Activites::OnBnClickedModifierVillages)
END_MESSAGE_MAP()


// Activites message handlers

BOOL Activites::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_ListActivites.InsertColumn(0,L"No_Activité",LVCFMT_CENTER,120,-1);
	m_ListActivites.InsertColumn(1,L"Designation",LVCFMT_CENTER,289,-1);
	ListView_SetExtendedListViewStyle(m_ListActivites, LVS_EX_GRIDLINES);
	m_ListActivites.DeleteAllItems();
	m_ListActivites.SetBkColor(RGB(68, 220, 75));

	m_DesignationControl.SetCueBanner(L"Designation");

	//VILLAGES
	COleVariant var;
	m_ListCurrentVillages.InsertColumn(0,L"Village",LVCFMT_CENTER,135,-1);
	m_ListCurrentVillages.InsertColumn(1,L"Gratuite",LVCFMT_CENTER,80,-1);
	ListView_SetExtendedListViewStyle(m_ListCurrentVillages, LVS_EX_GRIDLINES);
	m_ListCurrentVillages.SetBkColor(RGB(58, 210, 255));
	m_ListCurrentVillages.DeleteAllItems();

	m_VillagesCombo.SetCueBanner(L"Villages");

	CDaoRecordset VillageSet(&db);
	VillageSet.Open(AFX_DAO_USE_DEFAULT_TYPE,L"select * from Village",NULL);
	var.ChangeType(VT_BSTR, NULL);
	CString Nom;

	int i = 0;
	while(!VillageSet.IsEOF()){
		VillageSet.GetFieldValue(L"Nom",var);
		Nom = (LPCWSTR)var.pbstrVal;
		m_VillagesCombo.InsertString(i,Nom);
		i++;
		VillageSet.MoveNext();
	}
	VillageSet.Close();


	RefreshList();

	return TRUE;
}

//// ACTIVITES
void Activites::ListData(CString query)
{
  COleVariant var;           // start Variables
  var.ChangeType(VT_BSTR, NULL);
  CString Designation, No_Activite; // end variables
  m_ListActivites.DeleteAllItems(); // clear list
  if(query == L""){ query = defaultQuery; }
  query = query + L" Order by No_Activite";
  recset.Close();
  recset.Open(AFX_DAO_USE_DEFAULT_TYPE, query, NULL);
  
  int i = 0;
  while (!recset.IsEOF()) // do until eof
  {
    recset.GetFieldValue(L"No_Activite", var);
    No_Activite.Format(L"%d", (LPCWSTR)var.pbstrVal);
    m_ListActivites.InsertItem(i, No_Activite, 0);
    recset.GetFieldValue(L"Designation", var);
    Designation = (LPCWSTR)var.pbstrVal;
    m_ListActivites.SetItemText(i, 1, Designation);

    i++;
    recset.MoveNext();
  }
  RefreshVillages();
}

void Activites::RefreshList(){
	ListData(L"");
}

void Activites::OnBnClickedPremier()
{
	recset.MoveFirst();
	UpdateFields();
}


void Activites::OnBnClickedDernier()
{
	recset.MoveLast();
	UpdateFields();
}


void Activites::OnBnClickedNext()
{
	if(!recset.IsEOF()){
		try{
			recset.MoveNext();
			UpdateFields();
		}catch(...){
			OnBnClickedPrevious();
		}
	}else{
		OnBnClickedPremier();
	}
}


void Activites::OnBnClickedPrevious()
{
	if(!recset.IsBOF()){
		try{
			recset.MovePrev();
			UpdateFields();
		}catch(...){
			OnBnClickedNext();
		}
	}
}

void Activites::UpdateFields(){
	COleVariant var; // start Variables
	var.ChangeType(VT_BSTR, NULL);
	recset.GetFieldValue(L"Designation", var);
	m_Designation = (LPCWSTR)var.pbstrVal;
	UpdateData(false);
	RefreshVillages();
}



void Activites::OnBnClickedTrouver()
{
	// TODO: Add your control notification handler code here

	CString Designation;
	GetDlgItemText(IDC_DESIGNATION_EDIT, Designation);
	if(Designation != L""){
		ListData(defaultQuery +  L" WHERE Designation='" + Designation + L"'");
		OnBnClickedDernier();
	}
	
}


void Activites::OnBnClickedActualiser()
{
	// TODO: Add your control notification handler code here
	RefreshList();
}


void Activites::OnBnClickedSupprimer()
{
	// TODO: Add your control notification handler code here
	if(!recset.IsBOF() && !recset.IsEOF()){

		//////////
		COleVariant var;
		recset.GetFieldValue(L"No_Activite", var);
		CString No_Activite;
		No_Activite.Format(L"%d", (LPCWSTR)var.pbstrVal);
		db.Execute(L"DELETE FROM Proposer WHERE No_Activite = "+No_Activite);
		//////////

		recset.Delete();
		RefreshList();
		AfxMessageBox(L"supprimé avec succès !", MB_ICONINFORMATION | MB_OK);
	}
}


void Activites::OnBnClickedModifier()
{
	// TODO: Add your control notification handler code here
	if(!recset.IsBOF() && !recset.IsEOF()){
		GetDlgItemText(IDC_DESIGNATION_EDIT, m_Designation);
		recset.Edit();
		recset.SetFieldValue(L"Designation", (LPCWSTR)m_Designation);
		recset.Update();
		RefreshList();
	}
}

void Activites::OnBnClickedAjouter()
{
	// TODO: Add your control notification handler code here
	CString Designation;
	GetDlgItemText(IDC_DESIGNATION_EDIT, Designation);
	recset.AddNew();
	recset.SetFieldValue(L"Designation", (LPCWSTR)Designation);
	recset.Update();
	RefreshList();
	OnBnClickedDernier();
}


//// END ACTIVITES


//// VILLAGES

void Activites::RefreshVillages(){

	if(villagesRecset.IsOpen()){ villagesRecset.Close(); }

	CString query;
	int No_Activite = !recset.IsEOF() ? recset.GetFieldValue(L"No_Activite").intVal : 0;
    query.Format(_T("%s WHERE No_Activite = %d"), defaultVillagesQuery, No_Activite);
	villagesRecset.Open(AFX_DAO_USE_DEFAULT_TYPE, query , NULL);
	
	m_ListCurrentVillages.DeleteAllItems();
	COleVariant var;
	CString Nom, Gratuite;
	BOOL bGratuite;	
	int i = 0;
	while (!villagesRecset.IsEOF()) // do until eof
	{
		m_ListCurrentVillages.InsertItem(i, 0, 0);
		villagesRecset.GetFieldValue(L"Nom", var);
		Nom = (LPCWSTR)var.pbstrVal;
		m_ListCurrentVillages.SetItemText(i, 0, Nom);
		villagesRecset.GetFieldValue(L"Gratuite", var);
		bGratuite = var.boolVal;
		Gratuite = bGratuite? L"Oui":L"Non";
		m_ListCurrentVillages.SetItemText(i, 1, Gratuite);
		i++;
		villagesRecset.MoveNext();
	}
	m_VillageGratuite = FALSE;
	m_VillagesCombo.SetCurSel(CB_ERR);

}

void Activites::OnBnClickedPremierVillages()
{
	// TODO: Add your control notification handler code here
	villagesRecset.MoveFirst();
	villagesUpdateFields();
}


void Activites::OnBnClickedPreviousVillages()
{
	// TODO: Add your control notification handler code here
	if(!villagesRecset.IsBOF()){
		try{
			villagesRecset.MovePrev();
			villagesUpdateFields();
		}catch(...){
			OnBnClickedNextVillages();
		}
	}
}


void Activites::OnBnClickedNextVillages()
{
	// TODO: Add your control notification handler code here
	if(!villagesRecset.IsEOF()){
		try{
			villagesRecset.MoveNext();
			villagesUpdateFields();
		}catch(...){
			OnBnClickedPreviousVillages();
		}
	}else{
		OnBnClickedPremierVillages();
	}
}


void Activites::OnBnClickedDernierVillages()
{
	// TODO: Add your control notification handler code here
	villagesRecset.MoveLast();
	villagesUpdateFields();
}


void Activites::OnBnClickedDisassocierVillages()
{
	// TODO: Add your control notification handler code here
	if(!villagesRecset.IsBOF() && !villagesRecset.IsEOF()){
		villagesRecset.Delete();
		RefreshVillages();
		AfxMessageBox(L"acitivité disassociée avec succès !", MB_ICONINFORMATION | MB_OK);
	}
}


void Activites::OnBnClickedModifierVillages()
{
	// TODO: Add your control notification handler code here
	if(!villagesRecset.IsBOF() && !villagesRecset.IsEOF()){
		BOOL bGratuite = m_VillageCheckBox.GetCheck() == BST_CHECKED;
		COleVariant No_Activite = recset.GetFieldValue(L"No_Activite");
		villagesRecset.Edit();
		villagesRecset.SetFieldValue(L"No_Activite", No_Activite);
		villagesRecset.SetFieldValue(L"No_Village", GetComboVillageID());
		villagesRecset.SetFieldValue(L"Gratuite", COleVariant((LONG) bGratuite));
		villagesRecset.Update();
		RefreshVillages();
	}
}


void Activites::OnBnClickedAjouterVillages()
{
	//TODO: Add your control notification handler code here
	BOOL bGratuite = m_VillageCheckBox.GetCheck() == BST_CHECKED;
	COleVariant No_Activite = recset.GetFieldValue(L"No_Activite");
	villagesRecset.AddNew();
	villagesRecset.SetFieldValue(L"No_Activite", No_Activite);
	villagesRecset.SetFieldValue(L"No_Village", GetComboVillageID());
	villagesRecset.SetFieldValue(L"Gratuite", COleVariant((LONG) bGratuite));
	villagesRecset.Update();
	RefreshVillages();
	OnBnClickedDernierVillages();

}

void Activites::villagesUpdateFields(){
	COleVariant var; // start Variables
	var.ChangeType(VT_BSTR, NULL);
	villagesRecset.GetFieldValue(L"Nom", var);
	m_VillagesCombo.SelectString(0,(LPCWSTR)var.pbstrVal);
	villagesRecset.GetFieldValue(L"Gratuite", var);
	m_VillageGratuite = var.boolVal? TRUE: FALSE;
	UpdateData(false);
}

COleVariant Activites::GetComboVillageID(){
	CString Village;
	m_VillagesCombo.GetLBText(m_VillagesCombo.GetCurSel(),Village);
	CDaoRecordset VillageSet(&db);
	COleVariant No_Village((LONG)0);
	VillageSet.Open(AFX_DAO_USE_DEFAULT_TYPE, L"select * from Village where Nom = '"+Village+L"'", NULL);
	if (!VillageSet.IsEOF()) {
		VillageSet.MoveFirst();
		VillageSet.GetFieldValue(L"No_Village", No_Village);
	}
	VillageSet.Close();
	return No_Village;
}


//// END VILLAGES