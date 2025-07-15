#ifndef ENCRYPTFILE_H
#define ENCRYPTFILE_H

#include <QObject>
#include <QThreadPool>
#include <QDir>
#include <QFile>
#include <QFileInfo>

class EncryptFile : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit EncryptFile(const QString &_fileName, const QString &_inPath, const QString &_outPath, const QString &_xorKey, bool _deleteFile, bool _useNumbering, QObject *parent = nullptr);
    void run() override;

private:
    QString fileName;
    QString inPath;
    QString outPath;
    QByteArray xorKey;
    int keySize;
    bool deleteFile = false;
    bool useNumbering = false;

    // Работа с файлами
    void startActions();
    int dataSize = 256; // Размер считываемых данных
    QByteArray encryptData(QByteArray &_data); // Шифрование данных


    int fileSize;
    int progressStep;
    int getFileNumber(const QString _fileName);
    bool isStopped = false;

signals:
    void encryptComplited(QString);
    void encryptStarted(QString);
    void progress(int);

public slots:
    void stop();
};

#endif // ENCRYPTFILE_H
