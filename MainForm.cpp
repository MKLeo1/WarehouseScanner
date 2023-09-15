#include "MainForm.hpp"
#include <windows.system.threading.h>

void main() {

	System::Windows::Forms::Application::EnableVisualStyles();
	System::Windows::Forms::Application::SetCompatibleTextRenderingDefault(false);
	ScannerProject::MainForm form;

	// Full screen
	form.FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
	form.WindowState = System::Windows::Forms::FormWindowState::Maximized;

	System::Windows::Forms::Application::Run(% form);
}

void ScannerProject::MainForm::MainForm_Load(System::Object^ sender, System::EventArgs^ e) {
	
	System::Windows::Forms::PictureBox^ backgroundImage = gcnew System::Windows::Forms::PictureBox();
	backgroundImage->Image = System::Drawing::Image::FromFile("background.jpg");
	backgroundImage->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
	backgroundImage->Dock = System::Windows::Forms::DockStyle::Fill;
	this->Controls->Add(backgroundImage);

	CodeInputBox->Focus();

	LoadDatabase();

	// !Char::IsDigit
	CodeInputBox->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MainForm::CodeInputBox_KeyPress);
	CodeInputBox->TextChanged += gcnew System::EventHandler(this, &MainForm::CodeInputBox_TextChanged);
	CodeInputBox->LostFocus += gcnew System::EventHandler(this, &MainForm::CodeInputBox_LostFocus);
	CodeInputBox->GotFocus += gcnew System::EventHandler(this, &MainForm::CodeInputBox_GotFocus);
	
	AutoReset->CheckedChanged += gcnew System::EventHandler(this, &MainForm::AutoReset_CheckedChanged);
	ProductsTable->AutoSizeColumnsMode = System::Windows::Forms::DataGridViewAutoSizeColumnsMode::Fill;
	ProductsTable->CellValueChanged += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &MainForm::ProductsTable_CellValueChanged);
	ProductsTable->CellEndEdit += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &MainForm::ProductsTable_CellEndEdit);
	ProductsTable->EditingControlShowing += gcnew System::Windows::Forms::DataGridViewEditingControlShowingEventHandler(this, &MainForm::ProductsTable_EditingControlShowing);
	
	CurrentProductBox->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
	
	WeightSumBox->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
}

void ScannerProject::MainForm::ProductsTable_EditingControlShowing(System::Object^ sender, System::Windows::Forms::DataGridViewEditingControlShowingEventArgs^ e) {
	if (ProductsTable->CurrentCell->ColumnIndex == 2)
	{
		System::Windows::Forms::TextBox^ textBox = dynamic_cast<System::Windows::Forms::TextBox^>(e->Control);
		if (textBox != nullptr)
		{
			textBox->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MainForm::WeightTextBox_KeyPress);
		}
	}
}

void ScannerProject::MainForm::WeightTextBox_KeyPress(System::Object^ sender, System::Windows::Forms::KeyPressEventArgs^ e) {
	if (!System::Char::IsDigit(e->KeyChar) && e->KeyChar != ',' && e->KeyChar != 8) {
		e->Handled = true;
	}
}

void ScannerProject::MainForm::ButtonExit_Click(System::Object^ sender, System::EventArgs^ e) {
	System::Windows::Forms::Application::Exit();
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
		CodeInputBox->BackColor = System::Drawing::Color::Red;
	}
}

void ScannerProject::MainForm::CodeInputBox_GotFocus(System::Object^ sender, System::EventArgs^ e) {
	CodeInputBox->BackColor = System::Drawing::SystemColors::Window;
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

		System::String^ newValue = ProductsTable->Rows[e->RowIndex]->Cells[e->ColumnIndex]->Value->ToString();
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

	System::Windows::Forms::DataGridViewTextBoxColumn^ indexColumn = gcnew System::Windows::Forms::DataGridViewTextBoxColumn();
	indexColumn->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::None;
	indexColumn->HeaderText = "";
	indexColumn->Width = 30;
	indexColumn->ReadOnly = true;
	ProductsTable->Columns->Add(indexColumn);

	System::Windows::Forms::DataGridViewTextBoxColumn^ codeColumn = gcnew System::Windows::Forms::DataGridViewTextBoxColumn();
	codeColumn->HeaderText = "Code";
	codeColumn->Width = 100;
	codeColumn->ReadOnly = true;
	ProductsTable->Columns->Add(codeColumn);

	System::Windows::Forms::DataGridViewTextBoxColumn^ weightColumn = gcnew System::Windows::Forms::DataGridViewTextBoxColumn();
	weightColumn->HeaderText = "Weight (kg)";
	weightColumn->Width = 100;
	weightColumn->ReadOnly = false;
	ProductsTable->Columns->Add(weightColumn);

	System::Windows::Forms::DataGridViewTextBoxColumn^ nameColumn = gcnew System::Windows::Forms::DataGridViewTextBoxColumn();
	nameColumn->HeaderText = "Product Name";
	nameColumn->Width = 200;
	nameColumn->ReadOnly = true;
	ProductsTable->Columns->Add(nameColumn);

	int currentIndex = 1;

	for (int i = 0; i < scannedCodesList->Count; ++i) {
		
		System::String^ scannedCode = scannedCodesList[i];
		System::String^ displayedWeight = FormatWeight(productsWeightList[i]);
		System::String^ recognizedProductName = RecognizeCode(scannedCode);

		ProductsTable->Rows->Add(currentIndex, scannedCode, displayedWeight, recognizedProductName);

		++currentIndex;
	}

	if (ProductsTable->RowCount > 0) {
		ProductsTable->FirstDisplayedScrollingRowIndex = ProductsTable->RowCount - 1;
	}
}

System::String^ ScannerProject::MainForm::FormatWeight(float weight) {
	System::String^ formattedWeight;
	if (weight == (int)weight) {
		formattedWeight = System::String::Format("{0:0.00}", weight);
	}
	else {
		formattedWeight = System::String::Format("{0:0.###}", weight);
	}
	return formattedWeight;
}

void ScannerProject::MainForm::SetFocusToCodeInputBox() {
	CodeInputBox->Focus();
}

void ScannerProject::MainForm::RefreshDisplay() {
	float totalWeight = SumAllWeight();
	System::String^ formattedWeight = FormatWeight(totalWeight);
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
	System::String^ scannedCode = CodeInputBox->Text;

	System::String^ recognizedProduct = RecognizeCode(scannedCode);
	if (!System::String::IsNullOrEmpty(recognizedProduct)) {
		CurrentProductBox->Clear();
		CurrentProductBox->Text = recognizedProduct;

		float extractedWeight = ExtractWeight(scannedCode);

		scannedCodesList->Add(scannedCode);
		productsWeightList->Add(extractedWeight);

		RefreshDisplay();

		CodeInputBox->Clear();
	}
	else {
		if (!System::String::IsNullOrEmpty(scannedCode)) {
			CurrentProductBox->Text = "Unrecognized product";
		}
		CodeInputBox->Clear();
	}
}

void ScannerProject::MainForm::CodeInputBox_KeyPress(System::Object^ sender, System::Windows::Forms::KeyPressEventArgs^ e) {
	if (!System::Char::IsDigit(e->KeyChar)) {
		e->Handled = true;
	}
}

float ScannerProject::MainForm::ExtractWeight(System::String^ scannedCode) {
	int codeLength = scannedCode->Length;
	System::String^ lastFourDigits = scannedCode->Substring(codeLength - 5, 4);

	if (codeLength == 13) {
		return float::Parse(lastFourDigits) / 1000.0f;
	}
	if (codeLength == 20) {
		return float::Parse(lastFourDigits) / 100.0f;
	}
	else return 0.000f;
}

System::String^ ScannerProject::MainForm::RecognizeCode(System::String^ scannedCode) {

	System::String^ recognizedProduct = "";
	System::String^ subScannedCode = "";

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
			System::Windows::Forms::MessageBox::Show("Database not found", "Error", System::Windows::Forms::MessageBoxButtons::OK, System::Windows::Forms::MessageBoxIcon::Error);
			return;
		}

		while (!inputFile->EndOfStream) {
			System::String^ line = inputFile->ReadLine();
			array<System::String^>^ parts = line->Split('|');

			if (parts->Length == 2) {
				System::String^ databaseCode = parts[0];
				System::String^ productName = parts[1];
				databaseDictionary->Add(databaseCode, productName);
			}
		}
		inputFile->Close();
	}
	catch (System::Exception^ e) {
		System::Windows::Forms::MessageBox::Show("Database error: " + e->Message, "Błąd", System::Windows::Forms::MessageBoxButtons::OK, System::Windows::Forms::MessageBoxIcon::Error);
	}
}
