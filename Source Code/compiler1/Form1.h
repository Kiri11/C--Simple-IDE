#pragma once
#using <mscorlib.dll>
#include <msclr\marshal_cppstd.h>
#include <string>
#include "Parser.h"
#include "Lexer.h"
using namespace System;
using namespace System::Diagnostics;

namespace compiler1 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::IO;
	using namespace System::Runtime::InteropServices;
	using namespace msclr::interop;

	/// <summary>
	/// Summary for Form1
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if(components)
			{
				delete components;
			}
		}

	private: System::Windows::Forms::Button^  OpenButton;
	private: System::Windows::Forms::Button^  ScanButton;
	private: System::Windows::Forms::Button^  CompileAndRunButton;

	private: System::Windows::Forms::RichTextBox^  ConsoleTextBox;
	private: System::Windows::Forms::RichTextBox^  symbolTextbox;

	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Button^  SaveButton;
	private: System::Windows::Forms::TextBox^  SourceTextBox;
	private: System::Windows::Forms::Button^  CompileButton;


	private: System::Windows::Forms::RichTextBox^  parseTreeTextBox;
	private: System::Windows::Forms::Label^  label4;


	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;
		bool isCodeAccepted;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->OpenButton = (gcnew System::Windows::Forms::Button());
			this->ScanButton = (gcnew System::Windows::Forms::Button());
			this->CompileAndRunButton = (gcnew System::Windows::Forms::Button());
			this->ConsoleTextBox = (gcnew System::Windows::Forms::RichTextBox());
			this->symbolTextbox = (gcnew System::Windows::Forms::RichTextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->SaveButton = (gcnew System::Windows::Forms::Button());
			this->SourceTextBox = (gcnew System::Windows::Forms::TextBox());
			this->CompileButton = (gcnew System::Windows::Forms::Button());
			this->parseTreeTextBox = (gcnew System::Windows::Forms::RichTextBox());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// OpenButton
			// 
			this->OpenButton->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->OpenButton->Location = System::Drawing::Point(13, 416);
			this->OpenButton->Name = L"OpenButton";
			this->OpenButton->Size = System::Drawing::Size(75, 23);
			this->OpenButton->TabIndex = 1;
			this->OpenButton->Text = L"Open...";
			this->OpenButton->UseVisualStyleBackColor = true;
			this->OpenButton->Click += gcnew System::EventHandler(this, &Form1::OpenButton_Click);
			// 
			// ScanButton
			// 
			this->ScanButton->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->ScanButton->Location = System::Drawing::Point(621, 416);
			this->ScanButton->Name = L"ScanButton";
			this->ScanButton->Size = System::Drawing::Size(75, 23);
			this->ScanButton->TabIndex = 2;
			this->ScanButton->Text = L"Scan";
			this->ScanButton->UseVisualStyleBackColor = true;
			this->ScanButton->Click += gcnew System::EventHandler(this, &Form1::ScanButton_Click);
			// 
			// CompileAndRunButton
			// 
			this->CompileAndRunButton->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->CompileAndRunButton->Location = System::Drawing::Point(783, 416);
			this->CompileAndRunButton->Name = L"CompileAndRunButton";
			this->CompileAndRunButton->Size = System::Drawing::Size(102, 23);
			this->CompileAndRunButton->TabIndex = 3;
			this->CompileAndRunButton->Text = L"Compile and Run";
			this->CompileAndRunButton->UseVisualStyleBackColor = true;
			this->CompileAndRunButton->Click += gcnew System::EventHandler(this, &Form1::CompileAndRunButton_Click);
			// 
			// ConsoleTextBox
			// 
			this->ConsoleTextBox->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->ConsoleTextBox->Location = System::Drawing::Point(13, 305);
			this->ConsoleTextBox->Name = L"ConsoleTextBox";
			this->ConsoleTextBox->ReadOnly = true;
			this->ConsoleTextBox->Size = System::Drawing::Size(872, 105);
			this->ConsoleTextBox->TabIndex = 4;
			this->ConsoleTextBox->Text = L"";
			// 
			// symbolTextbox
			// 
			this->symbolTextbox->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->symbolTextbox->Location = System::Drawing::Point(712, 33);
			this->symbolTextbox->Name = L"symbolTextbox";
			this->symbolTextbox->ReadOnly = true;
			this->symbolTextbox->Size = System::Drawing::Size(173, 242);
			this->symbolTextbox->TabIndex = 5;
			this->symbolTextbox->Text = L"";
			// 
			// label1
			// 
			this->label1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(13, 286);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(80, 13);
			this->label1->TabIndex = 6;
			this->label1->Text = L"Console Output";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(13, 13);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(69, 13);
			this->label2->TabIndex = 7;
			this->label2->Text = L"Source Code";
			// 
			// label3
			// 
			this->label3->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(709, 13);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(71, 13);
			this->label3->TabIndex = 8;
			this->label3->Text = L"Symbol Table";
			// 
			// SaveButton
			// 
			this->SaveButton->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->SaveButton->Location = System::Drawing::Point(94, 416);
			this->SaveButton->Name = L"SaveButton";
			this->SaveButton->Size = System::Drawing::Size(75, 23);
			this->SaveButton->TabIndex = 9;
			this->SaveButton->Text = L"Save";
			this->SaveButton->UseVisualStyleBackColor = true;
			this->SaveButton->Click += gcnew System::EventHandler(this, &Form1::SaveButton_Click);
			// 
			// SourceTextBox
			// 
			this->SourceTextBox->AcceptsReturn = true;
			this->SourceTextBox->AcceptsTab = true;
			this->SourceTextBox->AllowDrop = true;
			this->SourceTextBox->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->SourceTextBox->Location = System::Drawing::Point(16, 33);
			this->SourceTextBox->Multiline = true;
			this->SourceTextBox->Name = L"SourceTextBox";
			this->SourceTextBox->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->SourceTextBox->Size = System::Drawing::Size(413, 242);
			this->SourceTextBox->TabIndex = 10;
			// 
			// CompileButton
			// 
			this->CompileButton->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->CompileButton->Location = System::Drawing::Point(702, 416);
			this->CompileButton->Name = L"CompileButton";
			this->CompileButton->Size = System::Drawing::Size(75, 23);
			this->CompileButton->TabIndex = 12;
			this->CompileButton->Text = L"Compile";
			this->CompileButton->UseVisualStyleBackColor = true;
			this->CompileButton->Click += gcnew System::EventHandler(this, &Form1::button1_Click);
			// 
			// parseTreeTextBox
			// 
			this->parseTreeTextBox->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->parseTreeTextBox->Location = System::Drawing::Point(435, 33);
			this->parseTreeTextBox->Name = L"parseTreeTextBox";
			this->parseTreeTextBox->ReadOnly = true;
			this->parseTreeTextBox->Size = System::Drawing::Size(271, 242);
			this->parseTreeTextBox->TabIndex = 13;
			this->parseTreeTextBox->Text = L"";
			// 
			// label4
			// 
			this->label4->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(432, 13);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(59, 13);
			this->label4->TabIndex = 14;
			this->label4->Text = L"Parse Tree";
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(897, 451);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->parseTreeTextBox);
			this->Controls->Add(this->CompileButton);
			this->Controls->Add(this->SourceTextBox);
			this->Controls->Add(this->SaveButton);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->symbolTextbox);
			this->Controls->Add(this->ConsoleTextBox);
			this->Controls->Add(this->CompileAndRunButton);
			this->Controls->Add(this->ScanButton);
			this->Controls->Add(this->OpenButton);
			this->MinimumSize = System::Drawing::Size(800, 300);
			this->Name = L"Form1";
			this->Text = L"C-- Simple IDE";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void richTextBox1_TextChanged(System::Object^  sender, System::EventArgs^  e) {
			 }
	private: System::Void OpenButton_Click(System::Object^  sender, System::EventArgs^  e) {

				 OpenFileDialog^ openFileDialog1 = gcnew OpenFileDialog;

				 // openFileDialog1->InitialDirectory = "c:\\";
				 openFileDialog1->Filter = "txt files (*.txt)|*.txt|All files (*.*)|*.*";
				 openFileDialog1->FilterIndex = 2;
				 openFileDialog1->RestoreDirectory = true;

				 if( openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK ){
					 if( (openFileDialog1->OpenFile()) != nullptr ){
						 auto MyReader = gcnew IO::StreamReader(openFileDialog1->FileName); 
						 SourceTextBox->Text= MyReader->ReadToEnd();
						 MyReader->Close();
					 }
				 }
			 }
private: System::Void SaveButton_Click(System::Object^  sender, System::EventArgs^  e) {
			 SaveFileDialog^ saveFileDialog1 = gcnew SaveFileDialog;
			 saveFileDialog1->Filter = "txt files (*.txt)|*.txt|All files (*.*)|*.*";
			 saveFileDialog1->FileName = "source.txt";
			 if(saveFileDialog1->ShowDialog() == Windows::Forms::DialogResult::OK) {
				 try{
					 // Создание экземпляра StreamWriter для записи в файл:
					 auto MyWriter = gcnew IO::StreamWriter(saveFileDialog1->FileName, false, System::Text::Encoding::GetEncoding(CP_UTF8));
					 MyWriter->Write(SourceTextBox->Text);
					 MyWriter->Close(); 
					 SourceTextBox->Modified = false;
				 }
				 catch (Exception^ Ситуация){
					 MessageBox::Show(Ситуация->Message, "Error", MessageBoxButtons::OK, MessageBoxIcon::Exclamation);
				 }

			 }
		 }

private: System::Void ScanButton_Click(System::Object^  sender, System::EventArgs^  e) {
			std::string input_flow = marshal_as<std::string>(SourceTextBox->Text); // get string from input box
			Lexer myLexer (input_flow); // input flow
			Token myToken;
			std::string token_string = ""; // initialize as an empty string
			ConsoleTextBox->Text = ""; // emptying the output window
			parseTreeTextBox->Text = "";
			symbolTextbox->Text = "";
			try{
				do{
					myToken = myLexer.GetToken();
					token_string = myToken.ToString() + " ";
					ConsoleTextBox->Text += marshal_as<String^>(token_string);
				}while(myToken.type != BLOCK && myToken.type != END); // get tokens until get BLOCK or END token
			} catch (std::string block_string){
				ConsoleTextBox->Text += marshal_as<String^>(block_string);
			}
			
		 }
private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
			isCodeAccepted = false;
			std::string input_flow = marshal_as<std::string>(SourceTextBox->Text); // get string from input box
			Parser myParser;
			ConsoleTextBox->Text = ""; // emptying the output window
			try{
				ConsoleTextBox->Text = marshal_as<String^>(myParser.Parse(input_flow));
				// Создание экземпляра StreamWriter для записи в файл:
				auto MyWriter = gcnew IO::StreamWriter("asmCode.asm", false, System::Text::Encoding::GetEncoding(ANSI_CHARSET));
				MyWriter->Write(marshal_as<String^>(myParser.AsmCode()));
				MyWriter->Close(); 
				ConsoleTextBox->Text += "ASM Code saved in \"AsmCode.asm\" file.";
				isCodeAccepted = true;
			} catch (std::string error_string){
				ConsoleTextBox->Text += "!!! ";
				ConsoleTextBox->Text += marshal_as<String^>(error_string);
			} catch (Exception^ ex){
				MessageBox::Show(ex->Message, "Error", MessageBoxButtons::OK, MessageBoxIcon::Exclamation);
			}
			ConsoleTextBox->Text += "\n\nRead tokens:\n" + marshal_as<String^>(myParser.mTokens);
			parseTreeTextBox->Text = marshal_as<String^>(myParser.mParsingTree);
			symbolTextbox->Text = marshal_as<String^>(myParser.mVariables);
		 }
private: System::Void CompileAndRunButton_Click(System::Object^  sender, System::EventArgs^  e) {
			button1_Click(sender, e);
			if(isCodeAccepted == true) {
				try{
					Process ^p;
					ProcessStartInfo ^pInfo;
					pInfo = gcnew ProcessStartInfo();
					pInfo->Verb = "open";
					pInfo->FileName = "compile.bat";
					pInfo->UseShellExecute = true;
					p = Process::Start(pInfo);
				}
				catch (Exception^ Ситуация){
						 MessageBox::Show(Ситуация->Message, "Error", MessageBoxButtons::OK, MessageBoxIcon::Exclamation);
				}
			}
		 }
};
}

