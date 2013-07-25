#include "miniide.h"

#include "Token.h"
#include "Lexer.h"
#include "Parser.h"

#include <QtWidgets/QMessageBox>
#include <QtWidgets/QFileDialog>
#include <QtCore/QTextStream>
#include <QtCore/QProcess>
#include <QtGui/QDesktopServices>

MiniIDE::MiniIDE(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    connect( ui.mOpenButton, SIGNAL( clicked() ), this, SLOT( OnOpenFile() ) );
    connect( ui.mSaveButton, SIGNAL( clicked() ), this, SLOT( OnSaveFile() ) );
    connect( ui.mScanButton, SIGNAL( clicked() ), this, SLOT( OnScan() ) );
    connect( ui.mCompileButton, SIGNAL( clicked() ), this, SLOT( OnCompile() ) );
    connect( ui.mCompileAndRunButton, SIGNAL( clicked() ), this, SLOT( OnCompileAndRun() ) );
}

MiniIDE::~MiniIDE()
{
   // empty
}

void MiniIDE::OnOpenFile()
{
   QString file_path = QFileDialog::getOpenFileName( this, "Open source code dialog", QApplication::applicationDirPath(), 
      "txt files (*.txt);;All files (*.*)" );

   if( !file_path.isEmpty() )
   {
      QFile file( file_path );
      if( !file.open( QIODevice::ReadOnly | QIODevice::Text ) )
      {
         QMessageBox::critical( this, "can't open source code file", file.errorString() );
         return;
      }

      QTextStream strm( &file );
      QString content = strm.readAll();

      file.close();

      ui.mSourceCode->setPlainText( content );
   }
}

void MiniIDE::OnSaveFile()
{
   QString file_path = QFileDialog::getSaveFileName( this, "Save source code dialog",
      QApplication::applicationDirPath() + "/source.txt", 
      "txt files (*.txt);;All files (*.*)" );

   if( !file_path.isEmpty() )
   {
      QFile file( file_path );
      if( !file.open( QIODevice::WriteOnly | QIODevice::Text ) )
      {
         QMessageBox::critical( this, "can't save source code file", file.errorString() );
         return;
      }

      QTextStream strm( &file );
      strm << ui.mSourceCode->toPlainText();
      
      file.close();
   }
}

void MiniIDE::OnScan()
{
   std::string input_flow = ui.mSourceCode->toPlainText().toStdString();
   Lexer my_lexer( input_flow );
   Token my_token;
   std::string token_string;

   ui.mConsoleOutput->clear();
   ui.mParseTree->clear();
   ui.mSymbolTable->clear();

   try
   {
      do
      {
         my_token = my_lexer.GetToken();
         token_string = my_token.ToString() + " ";
         ui.mConsoleOutput->appendPlainText( QString::fromStdString( token_string ) );
      }
      while( BLOCK != my_token.type && END != my_token.type );
   }
   catch( std::string const& block_string )
   {
      ui.mConsoleOutput->appendPlainText( QString::fromStdString( block_string ) );
   }
}

void MiniIDE::OnCompile()
{
   mIsCodeAccepted = false;
   std::string input_flow = ui.mSourceCode->toPlainText().toStdString();
   Parser my_parser;
   ui.mConsoleOutput->clear();
   ui.mParseTree->clear();
   ui.mSymbolTable->clear();

   try
   {
      ui.mConsoleOutput->setPlainText( QString::fromStdString( my_parser.Parse( input_flow ) ) );
      QFile asm_code_file( QApplication::applicationDirPath() + "/asmcode.asm" );
      if( !asm_code_file.open( QIODevice::WriteOnly | QIODevice::Text ) )
      {
         QMessageBox::critical( this, "Error", "Couldn't create asm code output file!" );
         return;
      }
      QTextStream strm( &asm_code_file );
      strm << QString::fromStdString( my_parser.AsmCode() );
      asm_code_file.close();
      ui.mConsoleOutput->appendPlainText( "ASM Code saved in \"AsmCode.asm\" file." );
      mIsCodeAccepted = true;
   }
   catch( std::string const& error_string )
   {
      ui.mConsoleOutput->appendPlainText( "!!! " + QString::fromStdString( error_string ) );
   }
   catch( std::exception const& ex )
   {
      QMessageBox::critical( this, "Error", ex.what() );
   }
   ui.mConsoleOutput->appendPlainText( "\n\nRead tokens:\n" + QString::fromStdString( my_parser.mTokens ) );
   ui.mParseTree->appendPlainText( QString::fromStdString( my_parser.mParsingTree ) );
   ui.mSymbolTable->appendPlainText( QString::fromStdString( my_parser.mVariables ) );
}

void MiniIDE::OnCompileAndRun()
{
   OnCompile();

   if( mIsCodeAccepted = true )
   {
      QProcess p;
      p.setWorkingDirectory( QApplication::applicationDirPath() );
      //QMessageBox::information( this, "info", QApplication::applicationDirPath() );
      //int ret_code = p.execute( "cmd.exe /k compile.bat" );
      QStringList arguments;
      arguments << "/k" << QApplication::applicationDirPath() + "/compile.bat";
      p.start( "C:\\Windows\System32\\cmd.exe", arguments );
      if( p.waitForStarted( 3000 ) )
      {
         QMessageBox::critical( this, "Error", "Couldn't start compile.bat\n\n" + p.errorString() );
      }
   }
}

#include "moc_miniide.cpp"