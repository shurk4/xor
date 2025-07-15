#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qRegisterMetaType<QList<QString>>("QList<QString>");

    setResultFileActions();
    setProgrammModes();
    setDefaultUIState();

    startEngine();
}

MainWindow::~MainWindow()
{
    engine.stop();
    delete ui;
}

void MainWindow::setResultFileActions()
{
    ui->comboBoxResultFileAct->addItem("Заменить");
    ui->comboBoxResultFileAct->addItem("Нумеровать");
}

void MainWindow::setProgrammModes()
{
    ui->comboBoxMode->blockSignals(true);
    ui->comboBoxMode->addItem("Разовый запуск");
    ui->comboBoxMode->addItem("По таймеру");
    ui->comboBoxMode->blockSignals(false);
    ui->lineEditTimer->setInputMask("99999");
    ui->widgetTimer->hide();
}

void MainWindow::setDefaultUIState()
{
    this->setWindowTitle("xor");
    ui->lineEditTimer->setAlignment(Qt::AlignHCenter);
    ui->progressBar->setTextVisible(true);
    ui->progressBar->hide();
    ui->progressBar->setAlignment(Qt::AlignCenter);
    ui->labelComplitedFiles->setAlignment(Qt::AlignHCenter);
}

void MainWindow::startEngine()
{
    connect(&engine, &Engine::signalEncryptStarted, this, &MainWindow::encryptStarted);
    connect(&engine, &Engine::signalEncryptProgress, this, &MainWindow::encryptProgress);
    connect(&engine, &Engine::signalEncryptComplited, this, &MainWindow::encryptComplited);
}

void MainWindow::encryptStarted(const QString _fileName)
{
    ui->progressBar->setFormat(_fileName + " %p%");
    ui->progressBar->show();
}

void MainWindow::encryptProgress(int progress)
{
    ui->progressBar->setValue(progress);
}

void MainWindow::encryptComplited(const QString _fileName)
{
    ui->listWidgetFiles->addItem(_fileName);
    ui->progressBar->setValue(100);
    // ui->progressBar->hide();
}

void MainWindow::on_comboBoxMode_currentIndexChanged(int index)
{
    if(index == 0)
    {
        ui->widgetTimer->hide();
        engine.setCheckPeriod(0);
    }
    else
    {
        ui->widgetTimer->show();
        engine.setCheckPeriod(ui->lineEditTimer->text().toInt());
    }
}

void MainWindow::on_pushButtonInPath_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this, "Выбрать путь к входным файлам");
    if(!path.isEmpty())
    {
        ui->labelInPath->setText(path);
        engine.setInFilesPath(path);
    }
}

void MainWindow::on_pushButtoOutPath_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this, "Выбрать путь сохранения результирующих файлов");
    if(!path.isEmpty())
    {
        ui->labelOutPath->setText(path);
        engine.setOutFilesPath(path);
    }
}

void MainWindow::on_pushButtonStart_clicked()
{
    ui->listWidgetFiles->blockSignals(true);
    ui->listWidgetFiles->clear();
    ui->listWidgetFiles->blockSignals(false);
    engine.run();
}

void MainWindow::on_pushButtonStop_clicked()
{
    engine.stop();
}

void MainWindow::on_lineEditInFileMask_editingFinished()
{
    engine.setFileMask(ui->lineEditInFileMask->text());
}

void MainWindow::on_comboBoxResultFileAct_currentIndexChanged(int index)
{
    engine.setUseNumbering((bool)index);
}

void MainWindow::on_checkBoxDeleteInFiles_stateChanged(int arg1)
{
    engine.setDeleteInFiles((bool)arg1);
}

void MainWindow::on_lineEditTimer_textEdited(const QString &arg1)
{
    engine.setCheckPeriod(ui->lineEditTimer->text().toInt());
}

void MainWindow::on_lineEditXORKey_editingFinished()
{
    engine.setXorKey(ui->lineEditXORKey->text());
}

