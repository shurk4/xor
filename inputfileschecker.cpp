#include "inputfileschecker.h"

InputFilesChecker::InputFilesChecker(const QString &_path, const QString &_mask, bool _deleteFiles, const uint _timePeriod, QObject *parent)
    : QObject{parent},
    path(_path),
    mask(_mask),
    timePeriod(_timePeriod),
    deleteFiles(_deleteFiles)
{}

InputFilesChecker::~InputFilesChecker()
{}

void InputFilesChecker::run()
{
    qDebug() << "InputFilesChecker::run";

    checkInputFiles();

    if(timePeriod > 0)
    {
        timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &InputFilesChecker::checkInputFiles);
        timer->start(timePeriod);
    }
    else emit finished();
}

void InputFilesChecker::stop()
{
    qDebug() << "InputFilesChecker::stop";
    if(timer != nullptr)
    {
        qDebug() << "InputFilesChecker::stop stop timer";
        timer->stop();
        timer->deleteLater();
    }
    emit finished();
}

void InputFilesChecker::checkInputFiles()
{
    qDebug() << "InputFilesChecker::checkInputFiles";

    QList<QString> newFilesList;

    if(deleteFiles)
    {
        newFilesList = QDir(path).entryList(QStringList(mask));
    }
    else
    {
        QList<QString> tempFilesList = QDir(path).entryList(QStringList(mask));
        if(filesList != tempFilesList)
        {
            for(const auto &i : tempFilesList)
            {
                QFileInfo currentFile(path + "/" + i);

                if(!filesList.contains(i))
                {
                    newFilesList.append(i);
                }
            }
        }
        filesList = tempFilesList;
    }
    if(newFilesList.isEmpty()) return;

    emit newFiles(newFilesList);
}
