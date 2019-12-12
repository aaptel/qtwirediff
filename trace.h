// SPDX-License-Identifier: GPL-3.0
#ifndef TRACE_H
#define TRACE_H

#include <QVector>
#include <QCache>
#include <QtConcurrent>

class Trace
{
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
        Node* parent;
        QString name;
        QString val;
        QVector<Node*> children;

        bool isLeaf() const {return children.isEmpty();}
        uint cached_hash;
        uint hash();
        void dump(int n=0) const;
        int depth() const;
        bool operator==(const Node& rhs) const;
        static void releaseNodeHierarchy(Node *);
    };

    class ParseError : public QtConcurrent::Exception
    {
    public:
        void raise() const { throw *this; }
        ParseError *clone() const { return new ParseError(*this); }
    };

    explicit Trace() {};
    int loadTrace(const QString& fn, const QString& filter = "");
    Node* getPacket(int no);
    void dump();
    size_t getPacketCount() {return pkts_.size();}
    const Summary& getSummary(int n) { return pkts_[n];}

    QString getFilename() const { return fn_; }
    bool isLoaded() const { return loaded_; }
    QString getFilter() const { return filter_; }

private:
    // disable copy
    Trace(const Trace&);
    Trace& operator=(const Trace&);
    QByteArray* getPDML(int no);


    QString fn_;
    QString filter_;
    bool loaded_;
    QVector<Summary> pkts_;

    // cache owns the node root and its hierarchy and will free
    // the hierarchy when
    // - node goes out of cache (max cache size reached)
    // - Trace is destroyed
    //
    // the application should not hold more than 2 root references
    // (one for each trace) so cache max size just needs to be 2+
    struct Tree {
        Trace::Node *root;
        Tree(Trace::Node *p) : root(p) {}
        ~Tree() { Trace::Node::releaseNodeHierarchy(root); }
    private: // disable copy
        Tree(const Tree&);
        Tree& operator=(const Tree&);
    };
    QCache<int, Trace::Tree> cache_;

signals:

public slots:
};


#endif // TRACE_H
