#include"boyermooresearch.h"

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    QElapsedTimer time;
    time.start();

    QString searchString = "For example, to search for PCT international applications filed by applicants from France and entering the national phase in Australia, the search expression below can be used:";

    BoyerMooreSearch search(searchString);
    QString filePath = "C:/Users/idris/Documents/en-fr.csv";//file path

    QPair<QString, QString> result = search.findExpression(filePath);

    qDebug() <<"Line where the result is found: " <<search.getLineNumber()<< "\nSearch result:" << result<<"\n";
    qDebug()<<"English part: "<<result.first;
    qDebug()<<"French part: " << result.second<<"\n";


    qint64 elapsed = time.elapsed();
    double seconde = static_cast<double>(elapsed)/1000;
    qDebug()<<"Elapsed time in seconde: "<<seconde<<"s";
    return app.exec();
}
