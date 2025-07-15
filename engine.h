#ifndef ENGINE_H
#define ENGINE_H

#include <QObject>

#include "inputfileschecker.h"
#include "encryptfile.h"

class Engine : public QObject
{
    Q_OBJECT
public:
    explicit Engine(QObject *parent = nullptr);

    void run();
    void stop();
    void setInFilesPath(const QString &_path);
    void setOutFilesPath(const QString &_path);
    void setFileMask(const QString &_mask);
    void setCheckPeriod(const int &_time);

    void setDeleteInFiles(const bool &_state);

    void setXorKey(const QString &_key);
    void setUseNumbering(const bool &_state);

    void setMaxEncryptThreads(const int &_threadsNum);

private:
    InputFilesChecker *fileChecker = nullptr;
    QThread *fileCheckerThread = nullptr;

    QString inPath = "";
    QString outPath = "";
    QString fileMask = "";
    int checkPeriod = 0;

    bool deleteInFiles = false;

    uint maxThreads = 1;
    uint threadsInWork = 0;
    QList<QString> filesToEncrypt;
    QString xorKey = "";
    bool useNumbering = false;

    void runFileChecker();
    void runEncryptFile();

signals:
    void signalStop();
    void signalEncryptStarted(QString);
    void signalEncryptProgress(int);
    void signalEncryptComplited(QString);

public slots:
    void fileCheckerThreadFinished();
    void newFiles(QList<QString> _filesList);
    void slotEncryptStarted(const QString _fileName);
    void slotEncryptProgress(int progress);
    void slotEncryptComplited(const QString _fileName);
};

#endif // ENGINE_H
