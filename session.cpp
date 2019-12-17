#include "session.h"
#include <QStandardPaths>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QDebug>

SessionList::SessionList(const QString& fnarg)
{
    needSave_ = false;
    QDir dir;

    if (fnarg == "") {
        dir = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
        fn_ = dir.filePath("sessions");
    } else {
        fn_ = fnarg;
        dir = QDir(fnarg).dirName();
    }

    QFile f(fn_);

    if (!dir.mkpath(dir.absolutePath())) {
        qDebug() << "cannot create dir" << dir.absolutePath();
        return;
    }
    if (!f.exists()) {
        qDebug() << "no session file" << fn_;
        return;
    }

    if (!f.open(QIODevice::ReadOnly)) {
        qDebug() << "open session failed";
        return;
    }

    QTextStream in(&f);
    while (!in.atEnd()) {
          QString line = in.readLine();
          auto cols = line.split("\t");
          if (cols.length() < 5) {
              qDebug() << "bad session line <"<<line<<">";
              continue;
          }
          ses_.append(Session({cols[0], cols[1], cols[2], cols[3], cols[4]}));
    }
    f.close();
}

bool SessionList::save()
{
    QFile f(fn_);
    if (!f.open(QIODevice::WriteOnly|QIODevice::Truncate)) {
        qDebug() << "failed to open w+" << fn_;
        return false;
    }
    QTextStream out(&f);
    for (int i = 0; i < qMin(ses_.size(), 5); i++) {
        auto &s = ses_[i];
        out << s.fileA << "\t"
            << s.filterA << "\t"
            << s.fileB << "\t"
            << s.filterB << "\t"
            << s.diffFilter << "\n";
    }
    f.close();
    return true;
}

void SessionList::add(const QString &fna, const QString &fa, const QString &fnb, const QString &fb, const QString &df)
{
    needSave_ = true;
    for (auto &s : ses_) {
        if (s.fileA == fna && s.fileB == fnb) {
            s.filterA = fa;
            s.filterB = fb;
            s.diffFilter = df;
            return;
        }
    }
    ses_.append(Session({fna, fa, fnb, fb, df}));
}
