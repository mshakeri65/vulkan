#include "vulkanwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    VulkanWindow app;
    app.show();

    return a.exec();
}
