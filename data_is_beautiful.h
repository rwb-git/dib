
#include <QWidget>

class DataIsBeautiful : public QWidget{
    Q_OBJECT

public:
    DataIsBeautiful(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
};


