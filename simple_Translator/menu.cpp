#include "menu.h"


Menu::Menu(QWidget *parent)noexcept : QWidget(parent){
    setWindowTitle("Translator");
    setFixedSize(650, 400);

    m_backgroundPixmap.load("C:/Users/idris/Documents/TranslatorU/Wallpaper.jpeg");//add relative path for export

    loadCentral();
}

void Menu::loadCentral(void){

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QHBoxLayout *contentLayout = new QHBoxLayout();


    m_translationWidget = new QWidget(this);
    m_translationWidget->setFixedSize(200, 200);
    QVBoxLayout *translationLayout = new QVBoxLayout(m_translationWidget);


    m_englishButton =  pushButton("Anglais");
    translationLayout->addWidget(m_englishButton);


    m_frenchButton =  pushButton("French");
    translationLayout->addWidget(m_frenchButton);

    m_translationWidget->setLayout(translationLayout);
    contentLayout->addWidget(m_translationWidget);


    m_translatorUltimateLabel = label("TRANSLATOR\nUltimate");
    contentLayout->addWidget(m_translatorUltimateLabel);

    mainLayout->addLayout(contentLayout);
    setLayout(mainLayout);

    connect(m_englishButton, &QPushButton::clicked, this, &Menu::onEnglishButtonClicked);
    connect(m_frenchButton, &QPushButton::clicked, this, &Menu::onFrenchButtonClicked);
}

QLabel* Menu::label(const QString& text){
    QLabel* label = new QLabel(text, this);
    label->setStyleSheet("color: white; font-size: 50px;");
    label->setAlignment(Qt::AlignCenter);

    return label;
}

void Menu::paintEvent(QPaintEvent *event){
    QWidget::paintEvent(event);
    QPainter painter(this);
    painter.drawPixmap(rect(), m_backgroundPixmap);
}

QPushButton* Menu::pushButton(const QString& text) {
    QPushButton* button= new QPushButton(text, this);
    button->setCursor(Qt::PointingHandCursor); // Make the button appear clickable
    button->setStyleSheet(
        "QPushButton {"
        "   border-radius: 10px;"
        "   color: white;"
        "   background-color: grey;"
        "}"
        "QPushButton:hover {"
        "   background-color: #FFCCCC;"
        "}"
        );
    button->setFixedSize(100, 50);

    return button;
}

//Slots
void Menu::onEnglishButtonClicked() {

    m_ScopedPtrEnglishWidget.reset(new TranslatorWindow(nullptr));

    m_ScopedPtrEnglishWidget->setupCentralEnglishWidget();
    m_ScopedPtrEnglishWidget->show();

}

void Menu::onFrenchButtonClicked(){

    m_ScopedPtrFrenchWidget.reset(new TranslatorWindow(nullptr));
    // Setup the central widget
    m_ScopedPtrFrenchWidget->setupCentralFrenchWidget();
    m_ScopedPtrFrenchWidget->show();
}

