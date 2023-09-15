#pragma once

namespace ScannerProject {

	public ref class MainForm : public System::Windows::Forms::Form
	{
	public:
		MainForm(void)
		{
			InitializeComponent();
			scannedCodesList = gcnew System::Collections::Generic::List<System::String^>();
			productsWeightList = gcnew System::Collections::Generic::List<float>();
			databaseDictionary = gcnew System::Collections::Generic::Dictionary<System::String^, System::String^>();

			inputDelayTimer = gcnew System::Timers::Timer();
			inputDelayTimer->Interval = 500;
			inputDelayTimer->Elapsed += gcnew System::Timers::ElapsedEventHandler(this, &MainForm::InputDelayTimer_Elapsed);
			
			autoResetTimer = gcnew System::Timers::Timer();
			autoResetTimer->Interval = 100000;
			autoResetTimer->AutoReset = false;
			autoResetTimer->Elapsed += gcnew System::Timers::ElapsedEventHandler(this, &MainForm::AutoResetTimer_Elapsed);
		}
	protected:

		~MainForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private:

		System::Collections::Generic::List<System::String^>^ scannedCodesList;
		System::Collections::Generic::List<float>^ productsWeightList;
		System::Collections::Generic::Dictionary<System::String^, System::String^>^ databaseDictionary;

	System::Void MainForm_Load(System::Object^, System::EventArgs^);
	System::Void ButtonExit_Click(System::Object^, System::EventArgs^);
	System::Void ButtonUndo_Click(System::Object^, System::EventArgs^);
	System::Void ButtonReset_Click(System::Object^, System::EventArgs^);
	System::Void WeightSumBox_TextChanged(System::Object^, System::EventArgs^);
	System::Void CodeInputBox_TextChanged(System::Object^, System::EventArgs^);
	System::Void CodeInputBox_KeyPress(System::Object^, System::Windows::Forms::KeyPressEventArgs^);
	System::Void ProductsTable_CellContentClick(System::Object^, System::Windows::Forms::DataGridViewCellEventArgs^);
	System::Void ProductsTable_CellEndEdit(System::Object^, System::Windows::Forms::DataGridViewCellEventArgs^);
	System::Void ProductsTable_CellValueChanged(System::Object^, System::Windows::Forms::DataGridViewCellEventArgs^);
	System::Void ProductsTable_EditingControlShowing(System::Object^, System::Windows::Forms::DataGridViewEditingControlShowingEventArgs^);
	System::Void WeightTextBox_KeyPress(System::Object^, System::Windows::Forms::KeyPressEventArgs^);
	
	System::Void MainForm::InputDelayTimer_Elapsed(System::Object^, System::Timers::ElapsedEventArgs^);
	System::Void MainForm::AutoResetTimer_Elapsed(System::Object^, System::Timers::ElapsedEventArgs^);
	System::Void MainForm::AutoReset_CheckedChanged(System::Object^, System::EventArgs^);
	System::Void MainForm::CodeInputBox_LostFocus(System::Object^ sender, System::EventArgs^ e);
	System::Void MainForm::CodeInputBox_GotFocus(System::Object^ sender, System::EventArgs^ e);

	System::ComponentModel::Container ^components;

	float SumAllWeight();
	float ExtractWeight(System::String^);
	void AddCodeToContainers();
	void ResetForm();
	void RefreshDisplay();
	void UpdateProductsTable();
	void LoadDatabase();
	void SetFocusToCodeInputBox();
	System::String^ RecognizeCode(System::String^);
	System::String^ FormatWeight(float);

	System::Timers::Timer^ autoResetTimer;
	System::Timers::Timer^ inputDelayTimer;

	System::Windows::Forms::DataGridView^ ProductsTable;
	System::Windows::Forms::Button^ ButtonReset;
	System::Windows::Forms::Button^ ButtonExit;
    System::Windows::Forms::Button^ ButtonUndo;
	System::Windows::Forms::CheckBox^ AutoReset;
	System::Windows::Forms::TextBox^ CurrentProductBox;	
	System::Windows::Forms::TextBox^ WeightSumBox;
	System::Windows::Forms::TextBox^ CodeInputBox;

#pragma region Windows Form Designer generated code

		void InitializeComponent(void)
		{
			System::Windows::Forms::DataGridViewCellStyle^ dataGridViewCellStyle2 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
			this->CurrentProductBox = (gcnew System::Windows::Forms::TextBox());
			this->ProductsTable = (gcnew System::Windows::Forms::DataGridView());
			this->ButtonReset = (gcnew System::Windows::Forms::Button());
			this->ButtonExit = (gcnew System::Windows::Forms::Button());
			this->ButtonUndo = (gcnew System::Windows::Forms::Button());
			this->AutoReset = (gcnew System::Windows::Forms::CheckBox());
			this->WeightSumBox = (gcnew System::Windows::Forms::TextBox());
			this->CodeInputBox = (gcnew System::Windows::Forms::TextBox());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->ProductsTable))->BeginInit();
			this->SuspendLayout();
			// 
			// CurrentProductBox
			// 
			this->CurrentProductBox->Font = (gcnew System::Drawing::Font(L"Microsoft YaHei", 21.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(238)));
			this->CurrentProductBox->Location = System::Drawing::Point(24, 12);
			this->CurrentProductBox->Multiline = true;
			this->CurrentProductBox->Name = L"CurrentProductBox";
			this->CurrentProductBox->Size = System::Drawing::Size(1365, 77);
			this->CurrentProductBox->TabIndex = 0;
			// 
			// ProductsTable
			// 
			this->ProductsTable->BackgroundColor = System::Drawing::SystemColors::Window;
			dataGridViewCellStyle2->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleLeft;
			dataGridViewCellStyle2->BackColor = System::Drawing::SystemColors::Control;
			dataGridViewCellStyle2->Font = (gcnew System::Drawing::Font(L"Microsoft YaHei", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(238)));
			dataGridViewCellStyle2->ForeColor = System::Drawing::SystemColors::WindowText;
			dataGridViewCellStyle2->SelectionBackColor = System::Drawing::SystemColors::Highlight;
			dataGridViewCellStyle2->SelectionForeColor = System::Drawing::SystemColors::HighlightText;
			dataGridViewCellStyle2->WrapMode = System::Windows::Forms::DataGridViewTriState::True;
			this->ProductsTable->ColumnHeadersDefaultCellStyle = dataGridViewCellStyle2;
			this->ProductsTable->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->ProductsTable->Location = System::Drawing::Point(387, 133);
			this->ProductsTable->Name = L"ProductsTable";
			this->ProductsTable->RowHeadersWidth = 62;
			this->ProductsTable->Size = System::Drawing::Size(642, 633);
			this->ProductsTable->TabIndex = 1;
			this->ProductsTable->CellContentClick += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &MainForm::ProductsTable_CellContentClick);
			this->ProductsTable->CellValueChanged += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &MainForm::ProductsTable_CellValueChanged);
			// 
			// ButtonReset
			// 
			this->ButtonReset->Font = (gcnew System::Drawing::Font(L"Microsoft YaHei", 20.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(238)));
			this->ButtonReset->Location = System::Drawing::Point(1089, 358);
			this->ButtonReset->Name = L"ButtonReset";
			this->ButtonReset->Size = System::Drawing::Size(300, 151);
			this->ButtonReset->TabIndex = 2;
			this->ButtonReset->Text = L"Reset";
			this->ButtonReset->UseVisualStyleBackColor = true;
			this->ButtonReset->Click += gcnew System::EventHandler(this, &MainForm::ButtonReset_Click);
			// 
			// ButtonExit
			// 
			this->ButtonExit->Font = (gcnew System::Drawing::Font(L"Microsoft YaHei", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(238)));
			this->ButtonExit->Location = System::Drawing::Point(1089, 637);
			this->ButtonExit->Name = L"ButtonExit";
			this->ButtonExit->Size = System::Drawing::Size(300, 129);
			this->ButtonExit->TabIndex = 3;
			this->ButtonExit->Text = L"Wy³¹cz aplikacje";
			this->ButtonExit->UseVisualStyleBackColor = true;
			this->ButtonExit->Click += gcnew System::EventHandler(this, &MainForm::ButtonExit_Click);
			// 
			// ButtonUndo
			// 
			this->ButtonUndo->Font = (gcnew System::Drawing::Font(L"Microsoft YaHei", 20.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(238)));
			this->ButtonUndo->Location = System::Drawing::Point(1089, 133);
			this->ButtonUndo->Name = L"ButtonUndo";
			this->ButtonUndo->Size = System::Drawing::Size(300, 151);
			this->ButtonUndo->TabIndex = 4;
			this->ButtonUndo->Text = L"Cofnij";
			this->ButtonUndo->UseVisualStyleBackColor = true;
			this->ButtonUndo->Click += gcnew System::EventHandler(this, &MainForm::ButtonUndo_Click);
			// 
			// AutoReset
			// 
			this->AutoReset->AutoSize = true;
			this->AutoReset->BackColor = System::Drawing::Color::LightGray;
			this->AutoReset->Font = (gcnew System::Drawing::Font(L"Microsoft YaHei", 20.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(238)));
			this->AutoReset->Location = System::Drawing::Point(1089, 553);
			this->AutoReset->Name = L"AutoReset";
			this->AutoReset->Size = System::Drawing::Size(184, 40);
			this->AutoReset->TabIndex = 5;
			this->AutoReset->Text = L"Auto Reset";
			this->AutoReset->UseVisualStyleBackColor = false;
			this->AutoReset->CheckedChanged += gcnew System::EventHandler(this, &MainForm::AutoReset_CheckedChanged);
			// 
			// WeightSumBox
			// 
			this->WeightSumBox->Font = (gcnew System::Drawing::Font(L"Microsoft YaHei", 24, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(238)));
			this->WeightSumBox->Location = System::Drawing::Point(24, 637);
			this->WeightSumBox->Multiline = true;
			this->WeightSumBox->Name = L"WeightSumBox";
			this->WeightSumBox->Size = System::Drawing::Size(300, 129);
			this->WeightSumBox->TabIndex = 6;
			this->WeightSumBox->TextChanged += gcnew System::EventHandler(this, &MainForm::WeightSumBox_TextChanged);
			// 
			// CodeInputBox
			// 
			this->CodeInputBox->Location = System::Drawing::Point(387, 95);
			this->CodeInputBox->Multiline = true;
			this->CodeInputBox->Name = L"CodeInputBox";
			this->CodeInputBox->Size = System::Drawing::Size(642, 32);
			this->CodeInputBox->TabIndex = 7;
			this->CodeInputBox->TextChanged += gcnew System::EventHandler(this, &MainForm::CodeInputBox_TextChanged);
			this->CodeInputBox->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MainForm::CodeInputBox_KeyPress);
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::Black;
			this->ClientSize = System::Drawing::Size(1415, 778);
			this->Controls->Add(this->CodeInputBox);
			this->Controls->Add(this->WeightSumBox);
			this->Controls->Add(this->AutoReset);
			this->Controls->Add(this->ButtonUndo);
			this->Controls->Add(this->ButtonExit);
			this->Controls->Add(this->ButtonReset);
			this->Controls->Add(this->ProductsTable);
			this->Controls->Add(this->CurrentProductBox);
			this->Name = L"MainForm";
			this->Text = L"Scanner";
			this->Load += gcnew System::EventHandler(this, &MainForm::MainForm_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->ProductsTable))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
};
}
