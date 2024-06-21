#ifndef BOYERMOORESEARCH_H
#define BOYERMOORESEARCH_H

#include <QRegularExpression>
#include <QCoreApplication>
#include <QFutureWatcher>
#include <QSharedPointer>
#include <QElapsedTimer>
#include <QMutexLocker>
#include <QtConcurrent>
#include <QTextStream>
#include <QThreadPool>
#include <QFuture>
#include <QDebug>
#include <QFile>
#include <QMutex>


using QtPairString =  QPair<QString, QString>;

class BoyerMooreSearch {
public:
    BoyerMooreSearch()=default;
    BoyerMooreSearch(const QString& pattern = "") noexcept;

    BoyerMooreSearch(const BoyerMooreSearch&)= delete;
    BoyerMooreSearch& operator=(const BoyerMooreSearch& )=delete;

    BoyerMooreSearch (const BoyerMooreSearch&&)=delete;
    BoyerMooreSearch& operator=(BoyerMooreSearch&& )=delete;

    int  getLineNumber()const {return resultLineNumber;};
    QPair<QString, QString> findExpression(const QString& );
    QString normalizeText(const QString&);



private:
    QString      pattern;
    QVector<int> badCharacters;
    QVector<int> goodSuffixes;
    int          resultLineNumber{0};
    QMutex       resultMutex;


    void buildBadCharacters();
    void buildGoodSuffixes ();
    bool isPrefix          (const QString&, int);
    int  suffixLength      (const QString&, int);

    QPair<QString, QString> extractRow    (const QString&, int);
    QPair<QString, QString> searchInChunk (const QString&, int, QtPairString&);
    QPair<QString, QString> processChunk  (const QString&, int, QtPairString&);
};

#endif // BOYERMOORESEARCH_H
