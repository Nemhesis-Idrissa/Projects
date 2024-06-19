#ifndef TRANSLATORWINDOW_H
#define TRANSLATORWINDOW_H
#include <QApplication>
#include <QMainWindow>

#include <QSet>
#include <QFile>
#include <QLabel>
#include <QDebug>
#include <QTextEdit>
#include <QLineEdit>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTextBrowser>
#include <QPlainTextEdit>
#include <QRegularExpression>
#include <QStandardItemModel>


class WelcomeWindow;

class TranslatorWindow : public QMainWindow {
    Q_OBJECT

public:
    //constructor
    TranslatorWindow(QWidget *parent = nullptr)noexcept;

    TranslatorWindow(const TranslatorWindow&) = delete;
    TranslatorWindow(TranslatorWindow&&)      = delete;

    TranslatorWindow& operator=(const TranslatorWindow&) = delete;
    TranslatorWindow& operator=(TranslatorWindow&&)      = delete;

    //English parts
    void setupCentralEnglishWidget();
    void englishLabel       (QVBoxLayout *layout);
    void translateEnglish   (QVBoxLayout *layout);
    void readFileEngToFrench(const QString& filename, QHash<QString, QString>& dictionary);

    //french parts
    void setupCentralFrenchWidget();
    void frenchLabel        (QVBoxLayout *layout);
    void translateFrench    (QVBoxLayout *layout);
    void readFileFrenchToEng(const QString& filename, QHash<QString, QString>& dictionary);

    //common design
    void addTextEdit(QVBoxLayout *layout);
    void addVerticalSpacers(QVBoxLayout *layout);
    void addTranslationTextBrowser(QVBoxLayout *layout);

    void readFile(const QString& filename, QSet<QString>& dictionary);
    void displayDictionary(const QHash<QString, QString>& dictionary);

    //translator logic
    QString          normalize(const QString& word);
    QString          findError(const QString& word);
    QVector<QString> splitWords(const QString& text);
    QString          correct(const QString& userInput);
    QString          translateSentence(const QString& sentence);
    int              damerauLevenshteinDistance(const QString& s1, const QString& s2);
    QString          translateSentence(const QString& sentence, const QHash<QString, QString>& dictionary);


    QVBoxLayout* getTranslatorMainLayout(void){return mainLayout;};
    QWidget*     getTranslatorWidget(void){return centralWidget;}

public slots:

    void translateEnglishFrench(void);
    void translateFrenchEnglish(void);

private:
    QLineEdit      *inputLineEdit;
    QPushButton    *translateButton;
    QTextBrowser   *translationTextBrowser;
    QWidget        *centralWidget;
    QVBoxLayout    *mainLayout;
    QTextEdit* translationTextEdit;

    QSet<QString>           dictionary;
    QHash<QString, QString> frenchToEnglish{{"je", "I"}, {"suis","am"}, {"été","summer"},{"je suis parti à la mer", "I went to the beach"}};
    QHash<QString, QString> englishToFrench{{"hello"," bonjour"}, {"I","je"}, {"am", "suis"}, {"leaving","partir"}};
};

#endif // TRANSLATORWINDOW_H
