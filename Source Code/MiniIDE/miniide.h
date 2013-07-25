#ifndef MINIIDE_H
#define MINIIDE_H

#include <QtWidgets/QMainWindow>
#include "GeneratedFiles/ui_miniide.h"

class MiniIDE : public QMainWindow
{
    Q_OBJECT

public:
    MiniIDE(QWidget *parent = 0);
    ~MiniIDE();

public slots:

   void OnOpenFile();
   void OnSaveFile();
   void OnScan();
   void OnCompile();
   void OnCompileAndRun();

private:
    bool mIsCodeAccepted;
    Ui::MiniIDEClass ui;
};

#endif // MINIIDE_H
