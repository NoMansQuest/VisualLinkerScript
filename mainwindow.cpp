#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qscrollbar.h"
#include "Components/QScintilla/ComponentHelpers.h"
#include "Components/QScintilla/src/Qsci/qscilexerlinkerscript.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);    
    this->scintilla = new QsciScintilla;
    Components::QScintilla::SetComponentStyles(*this->scintilla);    
    auto lexer = new QsciLexerLinkerScript;
    this->scintilla->setLexer((QsciLexer*)lexer);
    ui->splitter->addWidget(this->scintilla);    
}

MainWindow::~MainWindow()
{
    delete ui;
}
