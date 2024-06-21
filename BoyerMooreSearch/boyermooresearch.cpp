#include "boyermooresearch.h"

BoyerMooreSearch::BoyerMooreSearch(const QString& pattern )noexcept: pattern{pattern} {
    buildBadCharacters();
    buildGoodSuffixes();
}

void BoyerMooreSearch::buildBadCharacters() {
    int m = pattern.size();
    badCharacters.resize(256, m); // Initialize all to pattern size
    for (int j = 0; j < m - 1; ++j){
        badCharacters[static_cast<unsigned char>(pattern[j].toLatin1())] = m - 1 - j;
    }
}

void BoyerMooreSearch::buildGoodSuffixes(){
    int m = pattern.size();
    goodSuffixes.resize(m, m);
    int j = 0;
    for (int i = m - 1; i >= 0; --i){
        if (isPrefix(pattern, i + 1)){ j = m - 1 - i;}
        goodSuffixes[i] = j;
    }
    for (int i = 0; i < m - 1; ++i){
        int len = suffixLength(pattern, i);
        goodSuffixes[m - 1 - len] = m - 1 - i + len;
    }
}

bool BoyerMooreSearch::isPrefix(const QString& pattern, int p) {
    int m = pattern.size();
    for (int i = p, j = 0; i < m; ++i, ++j){
        if (pattern[i] != pattern[j]){ return false;}
    }
    return true;
}

int BoyerMooreSearch::suffixLength(const QString& pattern, int p) {
    int len = 0;
    int m = pattern.size();
    for (int i = p, j = m - 1; i >= 0 && pattern[i] == pattern[j]; --i, --j){++len;}
    return len;
}

QString BoyerMooreSearch::normalizeText(const QString& text) {
    // Updated regular expression to include characters to remove
    static const QRegularExpression punctuationRegex("[,:.!?\"\\\\]");
    QString normalized = text;
    normalized.remove(punctuationRegex);

    return normalized;
}


QPair<QString, QString> BoyerMooreSearch::extractRow(const QString& chunk, int matchIndex) {
    QString row;
    int lineStart = chunk.lastIndexOf('\n', matchIndex - 1) + 1; // Find the start of the line
    int lineEnd   = chunk.indexOf('\n', matchIndex); // Find the end of the line

    if (lineEnd == -1) {
        lineEnd = chunk.size(); // If end of line not found, use end of chunk
    }

    // Extract the entire line
    row = chunk.mid(lineStart, lineEnd - lineStart);

    // Find the delimiter that separates English and French parts
    static const QRegularExpression delimiterRegex("[:.!?]");
    QRegularExpressionMatch match = delimiterRegex.match(row);
    if (!match.hasMatch()) {
        return { "", "" }; // Return empty pair if delimiter not found
    }

    // Determine the boundary of the English part and French part
    int     splitIndex = match.capturedEnd(); // Index of the delimiter
    QString firstPart  = row.left(splitIndex).trimmed();
    QString secondPart = row.mid (splitIndex).trimmed();

    return QPair<QString, QString>(firstPart, secondPart);
}

QPair<QString, QString> BoyerMooreSearch::searchInChunk(const QString& chunk, int chunkStartLine, QtPairString& sharedResult) {
    int n = chunk.size();
    int m = pattern.size();
    int i = 0;

    while (i <= n -m) {
        int j = m - 1;
        while (j >= 0 && pattern[j] == chunk[i + j]){ --j;}

        if (j < 0) {

            // Match found, extract entire row
            QPair<QString, QString> matchedRow = extractRow(chunk, i);
            {
                QMutexLocker locker(&resultMutex);
                if (sharedResult.first.isEmpty() && sharedResult.second.isEmpty()) {
                    resultLineNumber = chunkStartLine + i + 1;
                    sharedResult = matchedRow;
                }
            }
        } else {
            // Move i based on max of bad character and good suffix rules
            int badCharSkip = badCharacters[static_cast<unsigned char>(chunk[i + j].toLatin1())];
            int goodSuffixSkip = goodSuffixes[j];
            i += std::max(badCharSkip, goodSuffixSkip);
        }

        {
            QMutexLocker locker(&resultMutex);
            if (!sharedResult.first.isEmpty() || !sharedResult.second.isEmpty()){
                break; // Stop if result is already found
            }
        }
    }

    return QPair<QString, QString>();
}

QPair<QString, QString> BoyerMooreSearch::processChunk(const QString& chunk, int chunkStartLine, QtPairString& sharedResult) {
    QStringList lines = chunk.split('\n');
    for (int i = 0; i < lines.size(); ++i) {
        searchInChunk(lines[i], chunkStartLine + i, sharedResult);
        {
            QMutexLocker locker(&resultMutex);
            if (!sharedResult.first.isEmpty() || !sharedResult.second.isEmpty())
                break;
        }
    }
    return sharedResult;
}

QPair<QString, QString> BoyerMooreSearch::findExpression(const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Error opening the file:" << file.errorString();
        return QPair<QString, QString>();
    }

    QTextStream in(&file);
    int chunkSize = 20000; // Adjust chunk size based on testing and performance requirements
    QString chunk;
    int chunkStartLine = 0;

    // result variable
    QPair<QString, QString> sharedResult;


    QThreadPool::globalInstance()->setMaxThreadCount(2);
    QList<QFuture<QPair<QString, QString>>> futures;

    // Process chunks of lines from the file
    while (!in.atEnd()) {
        chunk.clear();
        // Read chunkSize lines into 'chunk'
        for (int i = 0; i < chunkSize && !in.atEnd(); ++i) {
            chunk += in.readLine() + '\n';
        }

        // Use QtConcurrent::run to process each chunk in a separate thread
        QFuture<QPair<QString, QString>> future = QtConcurrent::run(&BoyerMooreSearch::processChunk, this, chunk, chunkStartLine, std::ref(sharedResult));
        futures.append(future);

        chunkStartLine += chunkSize;

        {
            QMutexLocker locker(&resultMutex);
            if (!sharedResult.first.isEmpty() || !sharedResult.second.isEmpty())
                break; // Stop if result is already found
        }
    }

    // Retrieve results from futures
    for (auto& future : futures) {
        future.waitForFinished();
    }

    sharedResult.first  = normalizeText(sharedResult.first);
    sharedResult.second = normalizeText(sharedResult.second);

    file.close();

    return sharedResult;
}
