#include "menu.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon("C:/Users/idris/Downloads/message.ico"));//add relative path for export

    Menu translator;
    translator.show();

    return app.exec();
}
