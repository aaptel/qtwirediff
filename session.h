#ifndef SESSION_H
#define SESSION_H

#include <QVector>

struct Session
{
    QString fileA, filterA;
    QString fileB, filterB;
    QString diffFilter;
};


class SessionList
{
    QString fn_;
    bool needSave_;
public:
    QVector<Session> ses_;
    SessionList(const QString& fn = "");
    void add(const QString &fna, const QString &fa, const QString &fnb, const QString &fb, const QString &df);
    bool save();

};


#endif // SESSION_H
