#ifndef FILEDATA_HPP
#define FILEDATA_HPP

#include <QString>

typedef enum { DELETE_FILE, COPY_FILE, IGNORE_FILE } FILE_PROCESS;

class FileData
{
  public:
    FileData(QString path, QString filename, qint64 size);

    QString path() const;
    QString filename() const;
    qint64 size() const;
    FILE_PROCESS process() const;

    void setProcess(FILE_PROCESS process);

  private:
    QString Path;
    QString Filename;
    qint64 Size;
    FILE_PROCESS Process;
};

#endif // FILEDATA_HPP
