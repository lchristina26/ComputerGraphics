#include <QCoreApplication>

int main(int argc, char *argv[])
{
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    QCoreApplication a(argc, argv);

    return a.exec();
}
