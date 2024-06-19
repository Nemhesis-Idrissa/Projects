#ifndef MENU_H
#define MENU_H

#include <QApplication>

#include <QFont>
#include <QLabel>
#include <QWidget>
#include <QPixmap>
#include <QPainter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScopedPointer>
#include <QSharedPointer>

#include "translatorwindow.h"


class Menu : public QWidget {
    Q_OBJECT
public:
    Menu(QWidget *parent = nullptr)noexcept;

    Menu (const Menu&)  = delete;
    Menu (const Menu&&) = delete;

    Menu& operator=(const Menu&)  = delete;
    Menu& operator=(const Menu&&) = delete;


    void loadCentral(void);
    QLabel* label(const QString& text);
    QPushButton* pushButton(const QString& text);

protected:
    void paintEvent(QPaintEvent *event) override;
public slots:

    void onEnglishButtonClicked();
    void onFrenchButtonClicked();

private:

    QLabel      *m_translatorUltimateLabel;
    QWidget     *m_translationWidget;
    QPixmap      m_backgroundPixmap;
    QPushButton *m_englishButton;
    QPushButton *m_frenchButton;

    QScopedPointer<TranslatorWindow> m_ScopedPtrFrenchWidget;
    QScopedPointer<TranslatorWindow> m_ScopedPtrEnglishWidget;
};

#endif // MENU_H
