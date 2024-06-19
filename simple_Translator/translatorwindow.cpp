
#include<QMainWindow>
#include "translatorwindow.h"



TranslatorWindow::TranslatorWindow(QWidget *parent)noexcept : QMainWindow(parent) {
    setWindowTitle("Translator");
    setMinimumSize(300, 200);
    //setupCentralWidget();
}


//English Translation Parts
void TranslatorWindow::setupCentralEnglishWidget() {
    centralWidget = new QWidget(this);
    mainLayout = new QVBoxLayout(centralWidget);
    centralWidget->setStyleSheet("background-color: #F0F0F0;");

    frenchLabel(mainLayout);
    addTextEdit(mainLayout);
    translateEnglish(mainLayout);
    addTranslationTextBrowser(mainLayout);

    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);//QT function

    readFileEngToFrench("C:/Users/idris/Downloads/en_fr2.txt", englishToFrench);
    //displayDictionary(englishToFrench);
}

//private slots
void TranslatorWindow::translateEnglishFrench(void){
    QString input =  translationTextEdit->toPlainText();
    if (!input.isEmpty()) {
        QString originalInputText = "Texte originale: " + input + "\n\n";
        QString correctedInput = correct(input);

        QString correctedInputText = "Texte corrigé: " + correctedInput+"\n\n";
        QString translation = translateSentence(correctedInput, englishToFrench);//traduction ici
        QString translationText = "Traduction: " + translation;

        translationTextBrowser->setText(originalInputText + correctedInputText + translationText);
    }else {
        return;
    }

}

void TranslatorWindow::translateEnglish(QVBoxLayout *layout) {
    translateButton = new QPushButton("Traduire", this);
    translateButton->setStyleSheet("background-color: #007BFF; color: #FFFFFF; border: none; border-radius: 20px; padding: 10px 20px; font-size: 18px;");
    layout->addWidget(translateButton);
    connect(translateButton, &QPushButton::clicked,this, &TranslatorWindow::translateEnglishFrench);
}

void TranslatorWindow::englishLabel(QVBoxLayout *layout) {
    QLabel *inputLabel = new QLabel("Enter a sentence in french", this);
    inputLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(inputLabel);
}

void TranslatorWindow::readFileEngToFrench(const QString& filename, QHash<QString, QString>& dictionary) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Unable to open file:" << filename;
        return;
    }
    QTextStream in(&file);
    QString line;

    static const QRegularExpression re("[.?!]");

    while (!(line = in.readLine()).isNull()) {

        QStringList phrases = line.split(re);

        if (phrases.size() < 2)
            continue;

        dictionary.insert(normalize(phrases[0].trimmed()), normalize(phrases[1].trimmed()));
    }
    file.close();
}



//French Translation parts

void TranslatorWindow::setupCentralFrenchWidget(){
    centralWidget = new QWidget(this);
    mainLayout = new QVBoxLayout(centralWidget);
    centralWidget->setStyleSheet("background-color: #F0F0F0;");


    englishLabel(mainLayout);
    addTextEdit(mainLayout);
    translateFrench(mainLayout);
    addTranslationTextBrowser(mainLayout);
    centralWidget->setLayout(mainLayout);

    setCentralWidget(centralWidget);//Qt function
    readFileFrenchToEng("C:/Users/idris/Downloads/en_fr2.txt", frenchToEnglish);

    //displayDictionary(frenchToEnglish);
    //readFile("C:/Users/idris/Documents/TranslatorQ/pg.txt", dictionary);
}

void TranslatorWindow::translateFrench(QVBoxLayout *layout){
    translateButton = new QPushButton("translate", this);
    translateButton ->setStyleSheet("background-color: #007BFF; color: #FFFFFF; border: none; border-radius: 20px; padding: 10px 20px; font-size: 18px;");
    layout ->addWidget(translateButton);
    connect(translateButton, &QPushButton::clicked,this, &TranslatorWindow::translateFrenchEnglish);
}

void TranslatorWindow::translateFrenchEnglish(void){
    QString input =  translationTextEdit->toPlainText();
    if (!input.isEmpty()) {
        QString originalInputText = "Original input: " + input + "\n\n";

        QString correctedInput = correct(input);
        QString correctedInputText = "Corrected input: " + correctedInput+"\n\n";

        QString translation = translateSentence(correctedInput, frenchToEnglish);//traduction ici

        QString translationText = "Translation: " + translation;

        translationTextBrowser->setText(originalInputText + correctedInputText + translationText);
    }else {
        return;
    }

}

void TranslatorWindow::frenchLabel(QVBoxLayout *layout) {
    QLabel *inputLabel = new QLabel("Entrez une phrase en anglais", this);
    inputLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(inputLabel);
}

void TranslatorWindow::readFileFrenchToEng(const QString& filename, QHash<QString, QString>& dictionary) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Unable to open file:" << filename;
        return;
    }
    QTextStream in(&file);
    QString line;

    static const QRegularExpression re("[.?!]");

    while (!(line = in.readLine()).isNull()) {

        QStringList phrases = line.split(re);


        if (phrases.size() < 2)
            continue;

        dictionary.insert(normalize(phrases[1].trimmed()), normalize(phrases[0].trimmed()));
    }
    file.close();
}

//Common parts + Widgets children

void TranslatorWindow::addTranslationTextBrowser(QVBoxLayout *layout) {
    translationTextBrowser = new QTextBrowser(this);
    translationTextBrowser->setStyleSheet("background-color: #FFFFFF; color: #333333; border: 2px solid #CCCCCC; border-radius: 10px; padding: 8px;");
    translationTextBrowser->setWordWrapMode(QTextOption::WordWrap);
    translationTextBrowser->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    layout->addWidget(translationTextBrowser);
}

void TranslatorWindow::addVerticalSpacers(QVBoxLayout *layout) {
    QSpacerItem *spacer = new QSpacerItem(1, 1, QSizePolicy::Minimum, QSizePolicy::Expanding);
    layout->addItem(spacer);

}

void TranslatorWindow::addTextEdit(QVBoxLayout *layout) {
    translationTextEdit = new QTextEdit(this);
    translationTextEdit->setStyleSheet("background-color: #FFFFFF; color: #333333; border: 2px solid #CCCCCC; border-radius: 10px; padding: 8px;");
    translationTextEdit->setWordWrapMode(QTextOption::WordWrap);

    // Set size policy to expanding horizontally and vertically
    translationTextEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    translationTextEdit->setFixedHeight(100);
    // Add the QTextEdit directly to the main layout
    layout->addWidget(translationTextEdit);
}


//Translation Logic

QString TranslatorWindow::translateSentence(const QString& sentence, const QHash<QString, QString>& dictionary) {
    QString normalizedSentence = normalize(sentence);

    // Check if the entire sentence exists in the dictionary
    auto it = dictionary.find(normalizedSentence);
    if (it != dictionary.end()) {
        return it.value(); // Return the translation if found
    }

    // If the entire sentence is not found, split the sentence into words and translate each word
    QTextStream stream(&normalizedSentence);
    QString word;
    QString translation;

    while (!stream.atEnd()) {
        stream >> word;
        QString normalizedWord = normalize(word);
        auto itWord = dictionary.find(normalizedWord);
        if (itWord != dictionary.end()) {
            translation += itWord.value() + " ";
        }else{
            translation += word + " ";
            QString("not found");
        }
    }
    return translation.trimmed();
}


//correction function
QString TranslatorWindow::correct(const QString& userInput) {

    QStringList words = userInput.split(" ", Qt::SkipEmptyParts);
    QString correctedSentence;

    for (const QString& word : words) {
        QString correctedWord = findError(word);

        if (!correctedWord.isEmpty()) {
            correctedSentence += correctedWord + " ";
        } else {
            correctedSentence += word + " ";
        }
    }
    return correctedSentence.trimmed();
}


QString TranslatorWindow::normalize(const QString& word) {
    QString result;
    for (QChar c : word) {
        if (c.isLetter() || c == ' ' || c == '\'' || c=='-') {
            result += c.toLower();
        }
    }
    while (!result.isEmpty() && !result.at(result.size() - 1).isLetter()) {
        result.chop(1);
    }
    return result;
}

void TranslatorWindow::readFile(const QString& filename, QSet<QString>& dictionary) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Unable to open file:" << filename;
        return;
    }
    QTextStream in(&file);
    QString line;

    static const QRegularExpression re("[.?!]");

    while (!(line = in.readLine()).isNull()) {
        QStringList phrases = line.split(re);
        for (const QString& phrase : phrases) {
            dictionary.insert(normalize(phrase.trimmed()));
        }
    }
    file.close();
}


QVector<QString> TranslatorWindow::splitWords(const QString& text) {
    QVector<QString> words;
    static QRegularExpression re("\\W+", QRegularExpression::UseUnicodePropertiesOption);
    QStringList wordList = text.split(re, Qt::SkipEmptyParts);
    for (const QString& word : wordList) {
        words.append(normalize(word));
    }
    return words;
}

int TranslatorWindow::damerauLevenshteinDistance(const QString& s1, const QString& s2) {
    const int len1 = s1.size();
    const int len2 = s2.size();
    QVector<QVector<int>> dp(len1 + 1, QVector<int>(len2 + 1, 0));
    for (int i = 0; i <= len1; ++i) {
        dp[i][0] = i;
    }
    for (int j = 0; j <= len2; ++j) {
        dp[0][j] = j;
    }
    for (int i = 1; i <= len1; ++i) {
        for (int j = 1; j <= len2; ++j) {
            int cost = (s1[i - 1] == s2[j - 1]) ? 0 : 1;
            dp[i][j] = std::min({
                dp[i - 1][j] + 1,          // Deletion
                dp[i][j - 1] + 1,          // Insertion
                dp[i - 1][j - 1] + cost   // Substitution
            });
            // Transposition
            if (i > 1 && j > 1 && s1[i - 1] == s2[j - 2] && s1[i - 2] == s2[j - 1]) {
                dp[i][j] = std::min(dp[i][j], dp[i - 2][j - 2] + cost);
            }
        }
    }
    return dp[len1][len2];
}

QString TranslatorWindow::findError(const QString& word) {
    QString closestMatch;
    double maxSimilarity = 0.0;

    double threshold = 0.5;
    // Compare input word with entries in the dictionary
    for (QSet<QString>::const_iterator it = dictionary.constBegin(); it != dictionary.constEnd(); ++it) {
        const QString& entry = *it;
        int distance = damerauLevenshteinDistance(word, entry);
        double similarity = 1.0 - (static_cast<double>(distance) / std::max(word.size(), entry.size()));
        if (similarity > maxSimilarity && similarity >= threshold) {
            maxSimilarity = similarity;
            closestMatch = entry;
        }
    }
    return closestMatch;
}

void TranslatorWindow::displayDictionary(const QHash<QString, QString>& dictionary) {
    QHash<QString, QString>::const_iterator i;
    for (i = dictionary.constBegin(); i != dictionary.constEnd(); ++i) {
        qDebug() <<i.key() << "\n" << i.value();
    }
}

