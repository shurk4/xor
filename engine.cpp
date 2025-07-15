#include "engine.h"

Engine::Engine(QObject *parent)
    : QObject{parent}
{}

void Engine::run()
{
    qDebug() << "Engine::run";
    if(!QDir(inPath).exists() || !QDir(outPath).exists())
    {
        qDebug() << "Check the files pathes";
        return;
    }
    if(fileMask.isEmpty())
    {
        qDebug() << "The file mask is not specified";
        return;
    }
    if(xorKey.isEmpty())
    {
        qDebug() << "The enkrypt key is not specified";
        return;
    }

    runFileChecker();
}

void Engine::stop()
{
    qDebug() << "Engine::stop";
    filesToEncrypt.clear();
    emit signalStop();
}

void Engine::setInFilesPath(const QString &_path)
{
    qDebug() << "Engine::setInFilesPath";
    inPath = _path;
}

void Engine::setOutFilesPath(const QString &_path)
{
    qDebug() << "Engine::setOutFilesPath";
    outPath = _path;
}

void Engine::setFileMask(const QString &_mask)
{
    qDebug() << "Engine::setFileMask";
    fileMask = _mask;
}

void Engine::setCheckPeriod(const int &_time)
{
    qDebug() << "Engine::setCheckPeriod: " << _time;
    checkPeriod = _time;
}

void Engine::setDeleteInFiles(const bool &_state)
{
    qDebug() << "Engine::setDeleteInFiles";
    deleteInFiles = _state;
}

void Engine::setXorKey(const QString &_key)
{
    qDebug() << "Engine::setXorKey";
    xorKey = _key;
}

void Engine::setUseNumbering(const bool &_state)
{
    qDebug() << "Engine::setUseNumbering";
    useNumbering = _state;
}

void Engine::setMaxEncryptThreads(const int &_threadsNum)
{
    qDebug() << "Engine::setMaxEncryptThreads";
    maxThreads = _threadsNum;
}

void Engine::runFileChecker()
{
    qDebug() << "Engine::runFileChecker";
    fileCheckerThread = new QThread(this);

    fileChecker = new InputFilesChecker(inPath, fileMask, deleteInFiles, checkPeriod);
    fileChecker->moveToThread(fileCheckerThread);

    connect(fileCheckerThread, &QThread::started, fileChecker, &InputFilesChecker::run);
    connect(fileChecker, &InputFilesChecker::finished, fileCheckerThread, &QThread::quit, Qt::DirectConnection);
    connect(fileCheckerThread, &QThread::finished, this, &Engine::fileCheckerThreadFinished);
    connect(this, &Engine::signalStop, fileChecker, &InputFilesChecker::stop);
    connect(fileChecker, &InputFilesChecker::newFiles, this, &Engine::newFiles);

    fileCheckerThread->start();
}

void Engine::runEncryptFile()
{
    qDebug() << "Engine::runEncryptFile";
    if(threadsInWork < maxThreads && !filesToEncrypt.isEmpty())
    {
        EncryptFile *encryptFile = new EncryptFile(filesToEncrypt[0], inPath, outPath, xorKey, deleteInFiles, useNumbering);
        connect(encryptFile, &EncryptFile::encryptStarted, this, &Engine::slotEncryptStarted);
        connect(encryptFile, &EncryptFile::encryptComplited, this, &Engine::slotEncryptComplited);
        connect(encryptFile, &EncryptFile::progress, this, &Engine::slotEncryptProgress);
        connect(this, &Engine::signalStop, encryptFile, &EncryptFile::stop);
        QThreadPool::globalInstance()->start(encryptFile);

        threadsInWork++;
    }
}

void Engine::fileCheckerThreadFinished()
{
    qDebug() << "Engine::fileCheckerThreadFinished";

    if(fileCheckerThread != nullptr)
    {
        fileCheckerThread->wait();
        delete fileCheckerThread;
        fileCheckerThread = nullptr;
    }

    if(fileChecker != nullptr)
    {
        delete fileChecker;
        fileChecker = nullptr;
    }
}

void Engine::newFiles(QList<QString> _filesList)
{
    filesToEncrypt.append(_filesList);
    qDebug() << "Engine::newFiles filesToEncrypt: " << filesToEncrypt;

    runEncryptFile();
}

void Engine::slotEncryptStarted(const QString _fileName)
{
    emit signalEncryptStarted(_fileName);
}

void Engine::slotEncryptProgress(int progress)
{
    emit signalEncryptProgress(progress);
}

void Engine::slotEncryptComplited(const QString _fileName)
{
    qDebug() << "Engine::slotEncryptComplited";
    filesToEncrypt.removeOne(_fileName);
    threadsInWork--;
    emit signalEncryptComplited(_fileName);
    runEncryptFile();
}
