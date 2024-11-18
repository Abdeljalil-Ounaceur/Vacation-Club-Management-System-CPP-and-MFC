// Langues.cpp : implementation file
//

#include "stdafx.h"
#include "gestionClub.h"
#include "Langues.h"
#include "afxdialogex.h"


// Langues dialog

IMPLEMENT_DYNAMIC(Langues, CDialogEx)

Langues::Langues(CWnd* pParent /*=NULL*/)
	: CDialogEx(Langues::IDD, pParent),db(CDaoDatabase()),recset(&db),villagesRecset(&db)
	, m_Designation(_T(""))
{
	
	defaultQuery = L"SELECT No_Langue, Designation FROM Langues";
	defaultVillagesQuery = L"SELECT Parler.No_Village as No_Village, Nom, No_Langue FROM Parler INNER JOIN Village ON Village.No_Village = Parler.No_Village";
	db.Open(L"base.mdb");
	recset.Open(AFX_DAO_USE_DEFAULT_TYPE, defaultQuery, NULL);
	
	Create(Langues::IDD, pParent);
	ShowWindow(SW_SHOW);
}

Langues::~Langues()
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

void Langues::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_LANGUES, m_ListLangues);
	DDX_Text(pDX, IDC_DESIGNATION_EDIT, m_Designation);
	DDV_MaxChars(pDX, m_Designation, 30);

	DDX_Control(pDX, IDC_LIST_VILLAGES, m_ListCurrentVillages);
	DDX_Control(pDX, IDC_COMBO_VILLAGES, m_VillagesCombo);
	DDX_Control(pDX, IDC_DESIGNATION_EDIT, m_DesignationControl);
}


BEGIN_MESSAGE_MAP(Langues, CDialogEx)
	ON_BN_CLICKED(IDC_PREMIER, &Langues::OnBnClickedPremier)
	ON_BN_CLICKED(IDC_DERNIER, &Langues::OnBnClickedDernier)
	ON_BN_CLICKED(IDC_NEXT, &Langues::OnBnClickedNext)
	ON_BN_CLICKED(IDC_PREVIOUS, &Langues::OnBnClickedPrevious)
	ON_BN_CLICKED(IDC_TROUVER, &Langues::OnBnClickedTrouver)
	ON_BN_CLICKED(IDC_ACTUALISER, &Langues::OnBnClickedActualiser)
	ON_BN_CLICKED(IDC_SUPPRIMER, &Langues::OnBnClickedSupprimer)
	ON_BN_CLICKED(IDC_MODIFIER, &Langues::OnBnClickedModifier)
	ON_BN_CLICKED(IDC_AJOUTER, &Langues::OnBnClickedAjouter)

	ON_BN_CLICKED(IDC_PREMIER_VILLAGES, &Langues::OnBnClickedPremierVillages)
	ON_BN_CLICKED(IDC_PREVIOUS_VILLAGES, &Langues::OnBnClickedPreviousVillages)
	ON_BN_CLICKED(IDC_NEXT_VILLAGES, &Langues::OnBnClickedNextVillages)
	ON_BN_CLICKED(IDC_DERNIER_VILLAGES, &Langues::OnBnClickedDernierVillages)
	ON_BN_CLICKED(IDC_DISASSOCIER_VILLAGES, &Langues::OnBnClickedDisassocierVillages)
	ON_BN_CLICKED(IDC_AJOUTER_VILLAGES, &Langues::OnBnClickedAjouterVillages)
	ON_BN_CLICKED(IDC_MODIFIER_VILLAGES, &Langues::OnBnClickedModifierVillages)
END_MESSAGE_MAP()


// Langues message handlers

BOOL Langues::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_ListLangues.InsertColumn(0,L"No_Langue",LVCFMT_CENTER,120,-1);
	m_ListLangues.InsertColumn(1,L"Designation",LVCFMT_CENTER,289,-1);
	ListView_SetExtendedListViewStyle(m_ListLangues, LVS_EX_GRIDLINES);
	m_ListLangues.DeleteAllItems();
	m_ListLangues.SetBkColor(RGB(255, 160, 8));

	m_DesignationControl.SetCueBanner(L"Designation");


	//VILLAGES
	COleVariant var;
	m_ListCurrentVillages.InsertColumn(0,L"Village",LVCFMT_CENTER,200,-1);
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

void Langues::ListData(LPCTSTR query)
{
  COleVariant var;           // start Variables
  var.ChangeType(VT_BSTR, NULL);
  CString Designation, No_Langue; // end variables
  m_ListLangues.DeleteAllItems(); // clear list
  if(query == L""){
	  query = defaultQuery;
  }
  recset.Close();
  recset.Open(AFX_DAO_USE_DEFAULT_TYPE, defaultQuery, NULL);
  
  int i = 0;
  while (!recset.IsEOF()) // do until eof
  {
    recset.GetFieldValue(L"No_Langue", var);
    No_Langue.Format(L"%d", (LPCWSTR)var.pbstrVal);
    m_ListLangues.InsertItem(i, No_Langue, 0);
    recset.GetFieldValue(L"Designation", var);
    Designation = (LPCWSTR)var.pbstrVal;
    m_ListLangues.SetItemText(i, 1, Designation);

    i++;
    recset.MoveNext();
  }
  RefreshVillages();
}

void Langues::RefreshList(){
	ListData(L"");
}

void Langues::OnBnClickedPremier()
{
	recset.MoveFirst();
	UpdateFields();
}


void Langues::OnBnClickedDernier()
{
	recset.MoveLast();
	UpdateFields();
}


void Langues::OnBnClickedNext()
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


void Langues::OnBnClickedPrevious()
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

void Langues::UpdateFields(){
	COleVariant var; // start Variables
	var.ChangeType(VT_BSTR, NULL);
	recset.GetFieldValue(L"Designation", var);
	m_Designation = (LPCWSTR)var.pbstrVal;
	UpdateData(false);
	RefreshVillages();
}



void Langues::OnBnClickedTrouver()
{
	// TODO: Add your control notification handler code here

	CString Designation;
	GetDlgItemText(IDC_DESIGNATION_EDIT, Designation);
	if(Designation != L""){
		ListData(defaultQuery +  L" WHERE Designation='" + Designation + L"'");
		OnBnClickedDernier();
	}
	
}


void Langues::OnBnClickedActualiser()
{
	// TODO: Add your control notification handler code here
	RefreshList();
}


void Langues::OnBnClickedSupprimer()
{
	// TODO: Add your control notification handler code here
	if(!recset.IsBOF() && !recset.IsEOF()){

		//////////
		COleVariant var;
		recset.GetFieldValue(L"No_Langue", var);
		CString No_Langue;
		No_Langue.Format(L"%d", (LPCWSTR)var.pbstrVal);
		db.Execute(L"DELETE FROM Parler WHERE No_Langue = "+No_Langue);
		//////////

		recset.Delete();
		RefreshList();
		AfxMessageBox(L"supprimé avec succès !", MB_ICONINFORMATION | MB_OK);
	}
}


void Langues::OnBnClickedModifier()
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

void Langues::OnBnClickedAjouter()
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


//// LANGUES

//// VILLAGES

void Langues::RefreshVillages(){

	if(villagesRecset.IsOpen()){ villagesRecset.Close(); }

	CString query;
	int No_Langue = !recset.IsEOF() ? recset.GetFieldValue(L"No_Langue").intVal : 0;
    query.Format(_T("%s WHERE No_Langue = %d Order by Parler.No_Village"), defaultVillagesQuery, No_Langue);
	villagesRecset.Open(AFX_DAO_USE_DEFAULT_TYPE, query , NULL);
	
	m_ListCurrentVillages.DeleteAllItems();
	COleVariant var;
	CString Nom;
	int i = 0;
	while (!villagesRecset.IsEOF()) // do until eof
	{
		m_ListCurrentVillages.InsertItem(i, 0, 0);
		villagesRecset.GetFieldValue(L"Nom", var);
		Nom = (LPCWSTR)var.pbstrVal;
		m_ListCurrentVillages.SetItemText(i, 0, Nom);
		i++;
		villagesRecset.MoveNext();
	}
	m_VillagesCombo.SetCurSel(CB_ERR);

}

void Langues::OnBnClickedPremierVillages()
{
	// TODO: Add your control notification handler code here
	villagesRecset.MoveFirst();
	villagesUpdateFields();
}


void Langues::OnBnClickedPreviousVillages()
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


void Langues::OnBnClickedNextVillages()
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


void Langues::OnBnClickedDernierVillages()
{
	// TODO: Add your control notification handler code here
	villagesRecset.MoveLast();
	villagesUpdateFields();
}


void Langues::OnBnClickedDisassocierVillages()
{
	// TODO: Add your control notification handler code here
	if(!villagesRecset.IsBOF() && !villagesRecset.IsEOF()){
		villagesRecset.Delete();
		RefreshVillages();
		AfxMessageBox(L"acitivité disassociée avec succès !", MB_ICONINFORMATION | MB_OK);
	}
}


void Langues::OnBnClickedModifierVillages()
{
	// TODO: Add your control notification handler code here
	if(!villagesRecset.IsBOF() && !villagesRecset.IsEOF()){
		COleVariant No_Langue = recset.GetFieldValue(L"No_Langue");
		villagesRecset.Edit();
		villagesRecset.SetFieldValue(L"No_Langue", No_Langue);
		villagesRecset.SetFieldValue(L"No_Village", GetComboVillageID());
		villagesRecset.Update();
		RefreshVillages();
	}
}


void Langues::OnBnClickedAjouterVillages()
{
	//TODO: Add your control notification handler code here
	COleVariant No_Langue = recset.GetFieldValue(L"No_Langue");
	villagesRecset.AddNew();
	villagesRecset.SetFieldValue(L"No_Langue", No_Langue);
	villagesRecset.SetFieldValue(L"No_Village", GetComboVillageID());
	villagesRecset.Update();
	RefreshVillages();
	OnBnClickedDernierVillages();

}

void Langues::villagesUpdateFields(){
	COleVariant var; // start Variables
	var.ChangeType(VT_BSTR, NULL);
	villagesRecset.GetFieldValue(L"Nom", var);
	m_VillagesCombo.SelectString(0,(LPCWSTR)var.pbstrVal);
	UpdateData(false);
}

COleVariant Langues::GetComboVillageID(){
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