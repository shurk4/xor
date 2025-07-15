#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QStandardPaths>

#include "inputfileschecker.h"
#include "encryptfile.h"

#include "engine.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_comboBoxMode_currentIndexChanged(int index);

    void on_pushButtonInPath_clicked();

    void on_pushButtoOutPath_clicked();

    void on_pushButtonStart_clicked();

    void on_pushButtonStop_clicked();

    void on_lineEditInFileMask_editingFinished();

    void on_comboBoxResultFileAct_currentIndexChanged(int index);

    void on_checkBoxDeleteInFiles_stateChanged(int arg1);

    void on_lineEditTimer_textEdited(const QString &arg1);

    void on_lineEditXORKey_editingFinished();

private:
    Ui::MainWindow *ui;

    void setResultFileActions();
    void setProgrammModes();
    void setDefaultUIState();
    void startEngine();

    Engine engine;

public slots:
    void encryptStarted(const QString _fileName);
    void encryptProgress(int progress);
    void encryptComplited(const QString _fileName);
};
#endif // MAINWINDOW_H
