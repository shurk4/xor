#ifndef INPUTFILESCHECKER_H
#define INPUTFILESCHECKER_H

#include <QObject>
#include <QThreadPool>
#include <QDir>
#include <QTimer>
#include <QMap>
#include <QDateTime>
#include <QDebug>

class InputFilesChecker : public QObject
{
    Q_OBJECT
public:
    explicit InputFilesChecker(const QString &_path, const QString &_mask, bool _deleteFiles, const uint _timePeriod = 0, QObject *parent = nullptr);
    ~InputFilesChecker();

private:
    QString path;
    QString mask;
    bool deleteFiles;
    uint timePeriod = 0;

    QTimer *timer = nullptr;
    bool isStopped = false;

    // Список файлов до изменения
    QList<QString> filesList;
    // Дата последнего изменения файла для режима с удалением входных файлов
    QDateTime lastFileChange;

signals:
    // Запускает таймер для режима определения новых входных фалов по таймеру
    void runTimer(int);
    // Сообщает о завершении работы экземпляра класса
    void finished();
    // Отправляет список новых фалов, если такие имеются
    void newFiles(QList<QString>);

public slots:
    // Запускает работу экземпляра класса и инициализирует таймер, если режим "По таймеру"
    void run();
    // Останавливает работу экземпляра класса
    void stop();
    // Проверяет директорию входных фалов на новые файлы
    void checkInputFiles();
};

#endif // INPUTFILESCHECKER_H
