#include "MainForm.hpp"
#include <windows.system.threading.h>

using namespace System;
using namespace System::Windows::Forms;

void main() {

	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	ScannerProject::MainForm form;

	// Full screen
	form.FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
	form.WindowState = FormWindowState::Maximized;

	Application::Run(% form);
}

void ScannerProject::MainForm::MainForm_Load(System::Object^ sender, System::EventArgs^ e) {
	
	PictureBox^ backgroundImage = gcnew PictureBox();
	backgroundImage->Image = Image::FromFile("background.jpg");
	backgroundImage->SizeMode = PictureBoxSizeMode::Zoom;
	backgroundImage->Dock = DockStyle::Fill;
	this->Controls->Add(backgroundImage);

	CodeInputBox->Focus();

	LoadDatabase();

	// !Char::IsDigit
	CodeInputBox->KeyPress += gcnew KeyPressEventHandler(this, &MainForm::CodeInputBox_KeyPress);
	CodeInputBox->TextChanged += gcnew EventHandler(this, &MainForm::CodeInputBox_TextChanged);
	CodeInputBox->LostFocus += gcnew System::EventHandler(this, &MainForm::CodeInputBox_LostFocus);
	CodeInputBox->GotFocus += gcnew System::EventHandler(this, &MainForm::CodeInputBox_GotFocus);
	
	AutoReset->CheckedChanged += gcnew System::EventHandler(this, &MainForm::AutoReset_CheckedChanged);
	ProductsTable->AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode::Fill;
	ProductsTable->CellValueChanged += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &MainForm::ProductsTable_CellValueChanged);
	ProductsTable->CellEndEdit += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &MainForm::ProductsTable_CellEndEdit);
	ProductsTable->EditingControlShowing += gcnew DataGridViewEditingControlShowingEventHandler(this, &MainForm::ProductsTable_EditingControlShowing);
	
	CurrentProductBox->TextAlign = HorizontalAlignment::Center;
	
	WeightSumBox->TextAlign = HorizontalAlignment::Center;
}

void ScannerProject::MainForm::ProductsTable_EditingControlShowing(System::Object^ sender, System::Windows::Forms::DataGridViewEditingControlShowingEventArgs^ e) {
	if (ProductsTable->CurrentCell->ColumnIndex == 2)
	{
		TextBox^ textBox = dynamic_cast<TextBox^>(e->Control);
		if (textBox != nullptr)
		{
			textBox->KeyPress += gcnew KeyPressEventHandler(this, &MainForm::WeightTextBox_KeyPress);
		}
	}
}

void ScannerProject::MainForm::WeightTextBox_KeyPress(System::Object^ sender, KeyPressEventArgs^ e) {
	if (!Char::IsDigit(e->KeyChar) && e->KeyChar != ',' && e->KeyChar != 8) {
		e->Handled = true;
	}
}

void ScannerProject::MainForm::ButtonExit_Click(System::Object^ sender, System::EventArgs^ e) {
	Application::Exit();
}

void ScannerProject::MainForm::ButtonUndo_Click(System::Object^ sender, System::EventArgs^ e) {
	if (scannedCodesList->Count > 0) {

		scannedCodesList->RemoveAt(scannedCodesList->Count - 1);
		productsWeightList->RemoveAt(productsWeightList->Count - 1);

		RefreshDisplay();
		CodeInputBox->Focus();
	}
}

void ScannerProject::MainForm::ButtonReset_Click(System::Object^ sender, System::EventArgs^ e) {
	ResetForm();
}

void ScannerProject::MainForm::InputDelayTimer_Elapsed(System::Object^ sender, System::Timers::ElapsedEventArgs^ e) {
	if (InvokeRequired) {
		BeginInvoke(gcnew System::Action(this, &ScannerProject::MainForm::AddCodeToContainers));
	}
	else {
		AddCodeToContainers();
	}
}

void ScannerProject::MainForm::AutoResetTimer_Elapsed(System::Object^ sender, System::Timers::ElapsedEventArgs^ e) {
	if (InvokeRequired) {
		BeginInvoke(gcnew System::Timers::ElapsedEventHandler(this, &MainForm::AutoResetTimer_Elapsed), sender, e);
		return;
	}

	if (AutoReset->Checked) {
		ResetForm();
		autoResetTimer->Start();
	}
}

void ScannerProject::MainForm::ResetForm() {
	CodeInputBox->Text = nullptr;
	CurrentProductBox->Text = nullptr;
	WeightSumBox->Text = nullptr;
	productsWeightList->Clear();
	scannedCodesList->Clear();
	RefreshDisplay();
	CodeInputBox->Focus();
}

void ScannerProject::MainForm::AutoReset_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {

	if (AutoReset->Checked) {
		autoResetTimer->Start();
	}
	else {
		autoResetTimer->Stop();
	}
}

void ScannerProject::MainForm::CodeInputBox_LostFocus(System::Object^ sender, System::EventArgs^ e) {
	if (!CodeInputBox->Focused) {
		CodeInputBox->BackColor = Color::Red;
	}
}

void ScannerProject::MainForm::CodeInputBox_GotFocus(System::Object^ sender, System::EventArgs^ e) {
	CodeInputBox->BackColor = SystemColors::Window;
}

float ScannerProject::MainForm::SumAllWeight() {
	float sum = 0.0;
	for each (float weight in productsWeightList) {
		sum += weight;
	}
	return sum;
} 

void ScannerProject::MainForm::ProductsTable_CellContentClick(System::Object^ sender, System::Windows::Forms::DataGridViewCellEventArgs^ e) {

}

void ScannerProject::MainForm::ProductsTable_CellEndEdit(System::Object^ sender, System::Windows::Forms::DataGridViewCellEventArgs^ e) {
	if (e->RowIndex >= 0 && e->ColumnIndex == 2) { 

		String^ newValue = ProductsTable->Rows[e->RowIndex]->Cells[e->ColumnIndex]->Value->ToString();
		float newWeight;

		if (float::TryParse(newValue, newWeight)) {
			if (e->RowIndex < productsWeightList->Count) {
			
				productsWeightList[e->RowIndex] = newWeight;
				RefreshDisplay();
			}
		}
	}
	CodeInputBox->Focus();
}

void ScannerProject::MainForm::ProductsTable_CellValueChanged(System::Object^ sender, System::Windows::Forms::DataGridViewCellEventArgs^ e) {
}

void ScannerProject::MainForm::UpdateProductsTable() {
	ProductsTable->Rows->Clear();
	ProductsTable->Columns->Clear();

	DataGridViewTextBoxColumn^ indexColumn = gcnew DataGridViewTextBoxColumn();
	indexColumn->AutoSizeMode = DataGridViewAutoSizeColumnMode::None;
	indexColumn->HeaderText = "";
	indexColumn->Width = 30;
	indexColumn->ReadOnly = true;
	ProductsTable->Columns->Add(indexColumn);

	DataGridViewTextBoxColumn^ codeColumn = gcnew DataGridViewTextBoxColumn();
	codeColumn->HeaderText = "Code";
	codeColumn->Width = 100;
	codeColumn->ReadOnly = true;
	ProductsTable->Columns->Add(codeColumn);

	DataGridViewTextBoxColumn^ weightColumn = gcnew DataGridViewTextBoxColumn();
	weightColumn->HeaderText = "Weight (kg)";
	weightColumn->Width = 100;
	weightColumn->ReadOnly = false;
	ProductsTable->Columns->Add(weightColumn);

	DataGridViewTextBoxColumn^ nameColumn = gcnew DataGridViewTextBoxColumn();
	nameColumn->HeaderText = "Product Name";
	nameColumn->Width = 200;
	nameColumn->ReadOnly = true;
	ProductsTable->Columns->Add(nameColumn);

	int currentIndex = 1;

	for (int i = 0; i < scannedCodesList->Count; ++i) {
		
		String^ scannedCode = scannedCodesList[i];
		String^ displayedWeight = FormatWeight(productsWeightList[i]);
		String^ recognizedProductName = RecognizeCode(scannedCode);

		ProductsTable->Rows->Add(currentIndex, scannedCode, displayedWeight, recognizedProductName);

		++currentIndex;
	}

	if (ProductsTable->RowCount > 0) {
		ProductsTable->FirstDisplayedScrollingRowIndex = ProductsTable->RowCount - 1;
	}
}

String^ ScannerProject::MainForm::FormatWeight(float weight) {
	String^ formattedWeight;
	if (weight == (int)weight) {
		formattedWeight = String::Format("{0:0.00}", weight);
	}
	else {
		formattedWeight = String::Format("{0:0.###}", weight);
	}
	return formattedWeight;
}

void ScannerProject::MainForm::SetFocusToCodeInputBox() {
	CodeInputBox->Focus();
}

void ScannerProject::MainForm::RefreshDisplay() {
	float totalWeight = SumAllWeight();
	String^ formattedWeight = FormatWeight(totalWeight);
	WeightSumBox->Text = formattedWeight + " kg";
	WeightSumBox->Refresh();
	UpdateProductsTable();
}

void ScannerProject::MainForm::WeightSumBox_TextChanged(System::Object^ sender, System::EventArgs^ e) {

}

void ScannerProject::MainForm::CodeInputBox_TextChanged(System::Object^ sender, System::EventArgs^ e) {
	inputDelayTimer->Stop();
	inputDelayTimer->Start();
}

void ScannerProject::MainForm::AddCodeToContainers() {
	String^ scannedCode = CodeInputBox->Text;

	String^ recognizedProduct = RecognizeCode(scannedCode);
	if (!String::IsNullOrEmpty(recognizedProduct)) {
		CurrentProductBox->Clear();
		CurrentProductBox->Text = recognizedProduct;

		float extractedWeight = ExtractWeight(scannedCode);

		scannedCodesList->Add(scannedCode);
		productsWeightList->Add(extractedWeight);

		RefreshDisplay();

		CodeInputBox->Clear();
	}
	else {
		if (!String::IsNullOrEmpty(scannedCode)) {
			CurrentProductBox->Text = "Unrecognized product";
		}
		CodeInputBox->Clear();
	}
}

void ScannerProject::MainForm::CodeInputBox_KeyPress(System::Object^ sender, System::Windows::Forms::KeyPressEventArgs^ e) {
	if (!Char::IsDigit(e->KeyChar)) {
		e->Handled = true;
	}
}

float ScannerProject::MainForm::ExtractWeight(String^ scannedCode) {
	int codeLength = scannedCode->Length;
	String^ lastFourDigits = scannedCode->Substring(codeLength - 5, 4);

	if (codeLength == 13) {
		return float::Parse(lastFourDigits) / 1000.0f;
	}
	if (codeLength == 20) {
		return float::Parse(lastFourDigits) / 100.0f;
	}
	else return 0.000f;
}

String^ ScannerProject::MainForm::RecognizeCode(String^ scannedCode) {

	String^ recognizedProduct = "";
	String^ subScannedCode = "";

	if (scannedCode->Length == 13) {
		subScannedCode = scannedCode->Substring(0, 7);
	}
	if (scannedCode->Length == 20) {
		subScannedCode = scannedCode->Substring(0, 14);
	}
	if (databaseDictionary->ContainsKey(subScannedCode)) {
		recognizedProduct = databaseDictionary[subScannedCode];
	}
	return recognizedProduct;
}

void ScannerProject::MainForm::LoadDatabase() {
	try {
		System::IO::StreamReader^ inputFile = gcnew System::IO::StreamReader("Insert/here/path/to/database");
		if (!inputFile) {
			MessageBox::Show("Database not found", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
			return;
		}

		while (!inputFile->EndOfStream) {
			String^ line = inputFile->ReadLine();
			array<String^>^ parts = line->Split('|');

			if (parts->Length == 2) {
				String^ databaseCode = parts[0];
				String^ productName = parts[1];
				databaseDictionary->Add(databaseCode, productName);
			}
		}
		inputFile->Close();
	}
	catch (Exception^ e) {
		MessageBox::Show("Database error: " + e->Message, "Błąd", MessageBoxButtons::OK, MessageBoxIcon::Error);
	}
}
