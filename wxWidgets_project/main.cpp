#include <wx/wxprec.h>
#include <wx/colordlg.h>
#include <wx/clipbrd.h>

class MyApp : public wxApp
{
public:
	virtual bool OnInit();
};

class MyFrame : public wxFrame
{
public:
	MyFrame();
private:
	wxButton* WxButton2;
	wxButton* WxButton1;
	wxBoxSizer* WxBoxSizer2;
	wxPanel* WxPanel1;
	wxBoxSizer* WxBoxSizer1;
	wxBitmap MyBitmap;
	wxImage  MyImage;
	wxCheckBox* WxCheckBox1;
	wxScrollBar* WxScrollBar1;
	wxTextCtrl* WxEdit1;
	wxString WxStaticText1 = "tekst";
	wxGauge* gauge;
	wxChoice* WxChoice1;
	wxColor starcolor = *wxBLACK;
	wxBitmap banana;

	void WxButton1_Click(wxCommandEvent& e);
	void WxButton2_Click(wxCommandEvent& e);
	void WxCheckBox1_Click(wxCommandEvent& e);
	void WxScrollBar1_Scroll(wxScrollEvent& e);
	void WxEdit1_Updated(wxCommandEvent& e);
	void Form_Paint(wxPaintEvent& e);
	void Choice(wxCommandEvent& e);

	int scrollPos = 0;
	int choice = 0;
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
	SetProcessDPIAware();
	MyFrame* frame = new MyFrame();
	frame->Show(true);
	return true;
}

MyFrame::MyFrame() : wxFrame(NULL, wxID_ANY, "lab5")
{
	SetTitle(_("lab5"));
	SetIcon(wxNullIcon);
	Center();

	WxBoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
	this->SetSizer(WxBoxSizer1);
	this->SetAutoLayout(true);

	WxPanel1 = new wxPanel(this, wxID_ANY, wxPoint(5, 39), wxSize(400, 400));
	WxBoxSizer1->Add(WxPanel1, 1, /*wxALIGN_CENTER |*/ wxEXPAND | wxALL, 5);

	WxBoxSizer2 = new wxBoxSizer(wxVERTICAL);
	WxBoxSizer1->Add(WxBoxSizer2, 0, wxALIGN_CENTER | wxALL, 5);

	//WxStaticText1 = new wxStaticText(this, wxID_ANY, _("tekst"), wxPoint(100, 300), wxDefaultSize, 0, _("WxStaticText1"));


	WxButton1 = new wxButton(this, wxID_ANY, _("zapis do pliku"), wxPoint(5, 5), wxSize(86, 29), 0, wxDefaultValidator, _("WxButton1"));
	WxBoxSizer2->Add(WxButton1, 0, wxALIGN_CENTER | wxALL, 5);

	WxCheckBox1 = new wxCheckBox(this, 1003, _("banan"), wxPoint(23, 65), wxSize(105, 19), 0, wxDefaultValidator, _("WxCheckBox1"));
	WxBoxSizer2->Add(WxCheckBox1, 0, wxALIGN_CENTER | wxALL, 5);

	WxScrollBar1 = new wxScrollBar(this, 1004, wxPoint(21, 106), wxSize(200, 18), wxSB_HORIZONTAL, wxDefaultValidator, _("WxScrollBar1"));
	WxScrollBar1->SetScrollbar(0, 0, 100, 10);
	WxBoxSizer2->Add(WxScrollBar1, 0, wxALIGN_CENTER | wxALL, 5);
	WxScrollBar1->Enable(false);

	gauge = new wxGauge(this, wxID_ANY, 100, wxDefaultPosition, wxDefaultSize, wxGA_HORIZONTAL);
	WxBoxSizer2->Add(gauge, 0, wxALIGN_CENTER | wxALL, 5);


	WxButton2 = new wxButton(this, wxID_ANY, _("kolor gwiazdki"), wxPoint(5, 44), wxSize(86, 29), 0, wxDefaultValidator, _("WxButton2"));
	WxBoxSizer2->Add(WxButton2, 0, wxALIGN_CENTER | wxALL, 5);


	WxEdit1 = new wxTextCtrl(this, 1002, _("tekst"), wxPoint(182, 21), wxSize(131, 20), 0, wxDefaultValidator, _("WxEdit1"));
	WxBoxSizer2->Add(WxEdit1, 0, wxALIGN_CENTER | wxALL, 5);

	wxArrayString choiceArray;
	WxChoice1 = new wxChoice(this, wxID_ANY, wxPoint(447, 21), wxSize(200, 32), choiceArray, 0, wxDefaultValidator, _("gwiazdka"));

	WxChoice1->Append("gwiazdka");
	WxChoice1->Append("ksiezyc");
	WxChoice1->Append("slonce");

	WxBoxSizer2->Add(WxChoice1, 0, wxALIGN_CENTER | wxALL, 5);
	WxChoice1->SetSelection(0);
	WxChoice1->Bind(wxEVT_CHOICE, &MyFrame::Choice, this);


	Layout();
	GetSizer()->SetSizeHints(this);
	Center();

	
	WxButton1->Bind(wxEVT_BUTTON, &MyFrame::WxButton1_Click, this);
	WxButton2->Bind(wxEVT_BUTTON, &MyFrame::WxButton2_Click, this);
	Bind(wxEVT_CHECKBOX, &MyFrame::WxCheckBox1_Click, this, 1003);
	Bind(wxEVT_SCROLL_THUMBTRACK, &MyFrame::WxScrollBar1_Scroll, this, 1004);
	Bind(wxEVT_TEXT, &MyFrame::WxEdit1_Updated, this, 1002);
	
	WxPanel1->Bind(wxEVT_PAINT, &MyFrame::Form_Paint, this);
	MyImage.AddHandler(new wxJPEGHandler);
	MyImage.AddHandler(new wxPNGHandler);
}

void MyFrame::WxButton2_Click(wxCommandEvent& e)
{
	wxColourData data;
	wxColour color;

	data.SetColour(color);

	wxColourDialog dialog(this, &data);


	if (dialog.ShowModal() == wxID_OK) {

		wxColourData retData = dialog.GetColourData();
		color = retData.GetColour();
		starcolor = color;

	}
	Refresh();
}

void MyFrame::WxButton1_Click(wxCommandEvent& e)
{
	int w, h;
	WxPanel1->GetClientSize(&w, &h);

	wxBitmap MyBitmap(w, h, 24);

	wxMemoryDC MyBitmapDC;
	MyBitmapDC.SelectObject(MyBitmap);

	wxClientDC MyDC(WxPanel1);
	MyBitmapDC.Blit(0, 0, w, h, &MyDC, 0, 0);

	if (wxTheClipboard->Open()) {
		wxTheClipboard->SetData(new wxBitmapDataObject(MyBitmap));
		wxTheClipboard->Close();
	}

	wxString defaultDir = wxGetCwd();
	wxString defaultFile = "malpka.png";
	wxString wildcard = "PNG files (*.png)|*.png";

	wxFileDialog saveFileDialog(this, _("Save file"), defaultDir, defaultFile, wildcard, wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	if (saveFileDialog.ShowModal() == wxID_CANCEL) {
		return;
	}

	wxString filePath = saveFileDialog.GetPath();
	MyBitmap.SaveFile(filePath, wxBITMAP_TYPE_PNG);
}

void MyFrame::WxCheckBox1_Click(wxCommandEvent& e)
{
	if (WxCheckBox1->IsChecked()) {
		WxScrollBar1->Enable(true);
		MyImage.LoadFile("banan.png", wxBITMAP_TYPE_PNG);
		wxImage TempImg(MyImage);
		TempImg.Rescale(120, 80);
		MyImage.Paste(TempImg, MyImage.GetWidth(), 0);
		banana = wxBitmap(MyImage);
		if (banana.Ok()) {
			Refresh();
		}
	}
	else {
		WxScrollBar1->Enable(false);
		banana = wxNullBitmap;
		Refresh();
	}
}

void MyFrame::WxScrollBar1_Scroll(wxScrollEvent& e)
{
	scrollPos = WxScrollBar1->GetThumbPosition();
	gauge->SetValue(scrollPos);
	Refresh();
}

void MyFrame::WxEdit1_Updated(wxCommandEvent& e)
{
	if (WxEdit1) WxStaticText1 = WxEdit1->GetValue();
	Refresh();
}

void MyFrame::Form_Paint(wxPaintEvent& event)
{
	wxPaintDC dc(WxPanel1);

	dc.SetBackground(wxBrush(wxColour(255, 255, 255), wxBRUSHSTYLE_SOLID));
	dc.Clear();
	dc.DrawLine(200, 100, 200, 200);
	dc.DrawLine(200, 100, 250, 150);
	dc.DrawLine(200, 100, 120, 150 - scrollPos);
	dc.DrawLine(200, 200, 250, 275);
	dc.DrawLine(200, 200, 150, 275);
	dc.DrawCircle(200, 50, 50);
	dc.DrawCircle(180, 30, 10);
	dc.DrawCircle(220, 30, 10);

	
	if (banana.Ok()) {
		dc.DrawBitmap(banana, 100, 130 - scrollPos);
		dc.DrawEllipticArc(185, 60, 30, 30, 180, 0);
	}
	else {
		dc.DrawEllipticArc(185, 60, 30, 30, 0, 180);
	}

	dc.DrawText(WxStaticText1, 80, 250);
	wxFont font(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_BOLD);
	dc.SetFont(font);
	dc.DrawRotatedText(WxStaticText1, 300, 250, 90);


	dc.SetBrush(*wxTRANSPARENT_BRUSH);
	dc.SetPen(starcolor);

	
	if (choice == 0) {

		// Define the vertices of the star
		wxPoint starVertices[10];
		int centerX = 75;
		int centerY = 75;
		int radiusOuter = 50;
		int radiusInner = 20;

		// Calculate the outer vertices of the star
		for (int i = 0; i < 5; ++i) {
			double angleOuter = i * 4 * M_PI / 5;
			starVertices[2 * i] = wxPoint(centerX + radiusOuter * cos(angleOuter), centerY + radiusOuter * sin(angleOuter));
		}

		for (int i = 0; i < 5; ++i) {
			double angleInner = (i * 4 + 2) * M_PI / 5;
			starVertices[2 * i + 1] = wxPoint(centerX + radiusInner * cos(angleInner), centerY + radiusInner * sin(angleInner));
		}
		dc.DrawPolygon(10, starVertices, wxWINDING_RULE);

	}
	else if (choice == 1) {
		dc.SetPen(*wxBLACK);
		wxPoint p1(60, 60);
		wxPoint p2(110, 110);
		wxPoint k1(80, 80);
		wxPoint k2(100, 60);
		dc.DrawArc(p1, p2, k1);
		dc.DrawArc(p1, p2, k2);
	}
	else {
		dc.SetPen(*wxBLACK);
		dc.SetBrush(*wxYELLOW_BRUSH);
		dc.DrawCircle(75, 75, 40);
	}
}

void MyFrame::Choice(wxCommandEvent& event) {
	choice = WxChoice1->GetSelection();

	Refresh();
}