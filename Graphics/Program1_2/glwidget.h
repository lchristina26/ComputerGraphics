#ifndef __GLWIDGET__INCLUDE__
#define __GLWIDGET__INCLUDE__

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QMouseEvent>
#include <glm/glm.hpp>

// glm by default uses degrees, but that functionality
// is deprecated so GLM_FORCE_RADIANS turns off some 
// glm warnings
#define GLM_FORCE_RADIANS

using glm::vec2;
using glm::vec4;
using glm::mat4;
using glm::mat2;


class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core {
    Q_OBJECT

    public:
        GLWidget(QWidget *parent=0);
        ~GLWidget();
        mat2 w2nd(vec2 pt_w, float width, float height);

    protected:
        void initializeGL();
        void resizeGL(int w, int h);
        void paintGL();

        void mousePressEvent(QMouseEvent *event);
        void keyPressEvent(QKeyEvent *event);

    private:
        GLuint loadShaders();
        static const GLchar* readShader(const char* filename);

        GLuint positionBuffer;
        bool outline;
        GLenum drawMode;

        GLenum draw_dif[7] = {
            GL_POINTS,
            GL_LINES,
            GL_LINE_STRIP,
            GL_LINE_LOOP,
            GL_TRIANGLES,
            GL_TRIANGLE_STRIP,
            GL_TRIANGLE_FAN
        };

        int count;
        vec2 pts[1000];
        vec2 tmp[1000];
        mat4 ortho;
        int num_pts;
        GLint matPos;


};

#endif
