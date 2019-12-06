#ifndef TRACE_H
#define TRACE_H

#include <QObject>
#include <QVector>
#include <QCache>
#include <QtConcurrent>

class Trace : public QObject
{
    Q_OBJECT
public:
    struct Summary {
        int no;
        double time;
        QString src;
        QString dst;
        QString proto;
        QString info;
    };

    struct Node {
        QString name;
        QString val;
        QVector<Node*> children;

        void dump(int n=0);
    };

    class ParseError : public QtConcurrent::Exception
    {
    public:
        void raise() const { throw *this; }
        ParseError *clone() const { return new ParseError(*this); }
    };

    explicit Trace(QObject *parent = nullptr);
    int loadTrace(const QString& fn);
    Node* getPacket(int no);
    void dump();

private:
    QString fn_;
    bool loaded_;    
    QVector<Summary> pkts_;
    QCache<int, QByteArray> cache_;

    QByteArray* getPDML(int no);

signals:

public slots:
};


#endif // TRACE_H
