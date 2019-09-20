
#include <QApplication>

#include "data_is_beautiful.h"

int main(int argc, char *argv[]){
    QApplication app(argc, argv);
    
    DataIsBeautiful dib;
    
    dib.show();
    
    return app.exec();
}

