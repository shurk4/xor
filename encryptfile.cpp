#include "encryptfile.h"

EncryptFile::EncryptFile(const QString &_fileName, const QString &_inPath, const QString &_outPath, const QString &_xorKey, bool _deleteFile, bool _useNumbering, QObject *parent)
    : QObject{parent},
    fileName(_fileName),
    inPath(_inPath),
    outPath(_outPath),
    deleteFile(_deleteFile),
    useNumbering(_useNumbering)
{
    xorKey = _xorKey.toUtf8();
    keySize = xorKey.size();
}

void EncryptFile::run()
{
    startActions();
}

void EncryptFile::startActions()
{
    emit encryptStarted(fileName);
    QFile file(inPath + "/" + fileName);
    QString baseFileName = QFileInfo(file).baseName();
    QString fileSuffix = QFileInfo(file).completeSuffix();
    fileSize = QFileInfo(file).size();
    progressStep = fileSize / 100;
    int readedDataSize = 0;
    int encryptProgress = 0;

    if(deleteFile)
    {
        file.rename(inPath + "/" + baseFileName + ".lock");
    }

    if(file.open(QIODevice::ReadOnly))
    {
        QString resultFileName = fileName;

        if(QFile::exists(outPath + "/" + fileName))
        {
            if(useNumbering)
            {
                int fileNum = getFileNumber(baseFileName);
                resultFileName = baseFileName + "_00" + QString::number(fileNum) + "." + fileSuffix;
            }
        }

        QFile resultFile(outPath + "/" + resultFileName);

        if(resultFile.open(QIODevice::WriteOnly))
        {
            while(!file.atEnd() && !isStopped)
            {
                QByteArray data = file.read(keySize);
                readedDataSize += data.size();

                resultFile.write(encryptData(data));

                if(readedDataSize >= progressStep)
                {
                    readedDataSize = 0;
                    encryptProgress++;
                    emit progress(encryptProgress);
                }
            }
            resultFile.close();
        }

        file.close();

        if(deleteFile)
        {
            file.remove();
        }
    }

    emit encryptComplited(fileName);
}

QByteArray EncryptFile::encryptData(QByteArray &_data)
{
    QByteArray result;
    for(int i = 0; i < _data.size(); i++)
    {
        result.push_back(_data[i] ^ xorKey[i % keySize]);
    }
    return result;
}

int EncryptFile::getFileNumber(const QString _fileName)
{
    QList<QString> tempFilesList = QDir(outPath).entryList(QStringList(_fileName + "*"));

    return tempFilesList.size();
}

void EncryptFile::stop()
{
    isStopped = true;
}
