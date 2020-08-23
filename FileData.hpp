#ifndef FILEDATA_HPP
#define FILEDATA_HPP

#include <QString>

typedef enum { DELETE_FILE, COPY_FILE, IGNORE_FILE } FILE_PROCESS;
typedef enum { TYPE_DIRECTORY, TYPE_FILE } FILE_TYPE;
#define NO_SIZE 0

class FileData
{
  public:
    FileData(QString filename, qint64 size, FILE_PROCESS process, FILE_TYPE type);

    QString filename() const;
    qint64 size() const;
    FILE_PROCESS process() const;
    FILE_TYPE type() const;
    void setProcess(FILE_PROCESS process);

  private:
    QString Filename;
    qint64 Size;
    FILE_PROCESS Process;
    FILE_TYPE Type;
};

#endif // FILEDATA_HPP
