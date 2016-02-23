#include "glwidget.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <QTextStream>
 #include <QLabel>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <FreeImage.h>
#include <stdlib.h>

using namespace std;
void load_image(const char *fname);

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent), outline(false), drawMode(0) {
    num_pts = 0;
    drawMode = draw_dif[0];
    count = 1;
}

GLWidget::~GLWidget() {
}

void GLWidget::keyPressEvent(QKeyEvent *event) {
    switch(event->key()) {
        case Qt::Key_C:
            cout << "Cleared all the points. Make sure to " 
                    "update this once you modify how the points "
                    "are stored." << endl;
            num_pts = 0;
            glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
            glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_DYNAMIC_DRAW);
            break;
        case Qt::Key_W:
            outline = !outline;
            if(outline) {
                cout << "Displaying outlines." << endl;
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            } else {
                cout << "Displaying filled polygons." << endl;
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
            break;
        case Qt::Key_Space:
            cout << "Changed draw mode" << endl;
            drawMode = draw_dif[count];
            count++;
            if (count >= 7)
                count = 0;



            break;
    }
    update();
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
    if(num_pts < 1000) {
        // devicePixelRatio will be 1 on standard displays.
        // On retina display devices (mac book pro retina, tablets, phones, etc)
        // devicePixelRatio will be greater than 1.
        // The coordinates of the event are in points (like font points,
        // not vertex points) rather than pixels.
        // There are devicePixelRatio pixels in a single point, so here
        // we are converting to pixels.
        qreal pixelRatio = this->devicePixelRatio();
        pts[num_pts].x = event->x()*pixelRatio;
        pts[num_pts].y = event->y()*pixelRatio;

        //pts[num_pts].x = w2nd(pts[num_pts], 640, 480).x;
        //pts[num_pts].x = w2nd(pts[num_pts], 640, 480).y;


        //pts[num_pts] = (tmp[num_pts].x * ortho[0][0]) + (tmp[num_pts].y * ortho[0][1]);
        //pts[num_pts].y = (tmp[num_pts].x * ortho[1][0]) + (tmp[num_pts].y * ortho[1][1]);
        //pts[num_pts] = w2nd(pts[num_pts], 640, 480) * pts[num_pts];

        cout << "Added point (" << pts[num_pts].x << ", " << pts[num_pts].y << ") " << endl;

        cout << "Make sure your orthographic projection matrix "
                "is set up so you can see the points." << endl;

        num_pts++;

        glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(pts), pts, GL_DYNAMIC_DRAW);
        update();
    } else {
        cout << "Th[2][2][2][2]ree points is the max. "
                "You must change this to make "
                "it so any number of points can be created." << endl;
    }
}

void GLWidget::initializeGL() {
    initializeOpenGLFunctions();

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glPointSize(4.0f);

    // Create a new Vertex Array Object on the GPU which
    // saves the attribute layout of our vertices.
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Create a buffer on the GPU for position data
    glGenBuffers(1, &positionBuffer);

    // Upload the position data to the GPU, storing
    // it in the buffer we just allocated.
    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
    glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_DYNAMIC_DRAW);

    load_image(":/test3.png");

    // Load our vertex and fragment shaders into a program object
    // on the GPU
    GLuint program = loadShaders();

    //GLfloat matVal;
    //
    // Bind the attribute "position" (defined in our vertex shader)
    // to the currently bound buffer object, which contains our
    // position data for a single triangle. This information 
    // is stored in our vertex array object.
    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
    GLint positionIndex = glGetAttribLocation(program, "position");
    glEnableVertexAttribArray(positionIndex);
    glVertexAttribPointer(positionIndex, 2, GL_FLOAT, GL_FALSE, 0, 0);
    matPos = glGetUniformLocation(program, "ortho");

}

void GLWidget::resizeGL(int w, int h) {

    glViewport(0,0,w,h);
    float width = w;
    float height = h;

    ortho = mat4((vec4(2.0/width, 0,0,0)),
                (vec4(0,-2.0/height, 0,0)),
                (vec4(0,0,-1.0,0)),
                (vec4(-1,1,1,1)));

             //w2nd(pts[num_pts], 640, 480);
            /* mat2(2/w, 1/(pts[num_pts].x),
                 -1/(pts[num_pts].y), -2/h); */

    glUniformMatrix4fv(matPos, 1, GL_FALSE, glm::value_ptr(ortho));
}

void GLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT);

    // draw primitives based on the current draw mode
    glDrawArrays(drawMode, 0, num_pts);
    
    // draw points so we can always see them
    // glPointSize adjusts the size of point
    // primitives
    glDrawArrays(GL_POINT, 0, num_pts);
}

// Copied from LoadShaders.cpp in the the oglpg-8th-edition.zip
// file provided by the OpenGL Programming Guide, 8th edition.
const GLchar* GLWidget::readShader(const char* filename) {
#ifdef WIN32
        FILE* infile;
        fopen_s( &infile, filename, "rb" );
#else
    FILE* infile = fopen( filename, "rb" );
#endif // WIN32

    if ( !infile ) {
#ifdef _DEBUG
        std::cerr << "Unable to open file '" << filename << "'" << std::endl;
#endif /* DEBUG */
        return NULL;
    }

    fseek( infile, 0, SEEK_END );
    int len = ftell( infile );
    fseek( infile, 0, SEEK_SET );

    GLchar* source = new GLchar[len+1];

    fread( source, 1, len, infile );
    fclose( infile );

    source[len] = 0;

    return const_cast<const GLchar*>(source);
}

GLuint GLWidget::loadShaders() {
    GLuint program = glCreateProgram();

    // read vertex shader from Qt resource file
    QFile vertFile(":/vert.glsl");
    vertFile.open(QFile::ReadOnly | QFile::Text);
    QString vertString;
    QTextStream vertStream(&vertFile);
    vertString.append(vertStream.readAll());
    string vertSTLString = vertString.toStdString();

    cout << "Vertex Shader:" << endl;
    cout << vertSTLString << endl;

    const GLchar* vertSource = vertSTLString.c_str();

    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertShader, 1, &vertSource, NULL);
    glCompileShader(vertShader);
    {
        GLint compiled;
        glGetShaderiv( vertShader, GL_COMPILE_STATUS, &compiled );
        if ( !compiled ) {
            GLsizei len;
            glGetShaderiv( vertShader, GL_INFO_LOG_LENGTH, &len );

            GLchar* log = new GLchar[len+1];
            glGetShaderInfoLog( vertShader, len, &len, log );
            std::cout << "Shader compilation failed: " << log << std::endl;
            delete [] log;
        }
    }
    glAttachShader(program, vertShader);

    // read fragment shader from Qt resource file
    QFile fragFile(":/frag.glsl");
    fragFile.open(QFile::ReadOnly | QFile::Text);
    QString fragString;
    QTextStream fragStream(&fragFile);
    fragString.append(fragStream.readAll());
    string fragSTLString = fragString.toStdString();

    cout << "Fragment Shader:" << endl;
    cout << fragSTLString << endl;

    const GLchar* fragSource = fragSTLString.c_str();

    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &fragSource, NULL);
    glCompileShader(fragShader);
    {
        GLint compiled;
        glGetShaderiv( fragShader, GL_COMPILE_STATUS, &compiled );
        if ( !compiled ) {
            GLsizei len;
            glGetShaderiv( fragShader, GL_INFO_LOG_LENGTH, &len );

            GLchar* log = new GLchar[len+1];
            glGetShaderInfoLog( fragShader, len, &len, log );
            std::cerr << "Shader compilation failed: " << log << std::endl;
            delete [] log;
        }
    }
    glAttachShader(program, fragShader);

    glLinkProgram(program);
    glUseProgram(program);

    return program;
}

void load_image(const char *fname) {
    // active only for static linking
    #ifdef FREEIMAGE_LIB
    FreeImage_Initialise();
    #endif
    FIBITMAP *bitmap;
    // Get the format of the image file
    FREE_IMAGE_FORMAT fif =FreeImage_GetFileType(fname, 0);
    // If the format can't be determined, try to guess the format from the file name
    if(fif == FIF_UNKNOWN) {
        fif = FreeImage_GetFIFFromFilename(fname);
    }
    // Load the data in bitmap if possible
    if(fif != FIF_UNKNOWN && FreeImage_FIFSupportsReading(fif)) {
        bitmap = FreeImage_Load(fif, fname);
    }
    else {
        bitmap = NULL;
    }
    // PROCESS IMAGE if bitmap was successfully initialized
    if(bitmap) {
        unsigned int w = FreeImage_GetWidth(bitmap);
        unsigned int h = FreeImage_GetHeight(bitmap);
        unsigned pixel_size = FreeImage_GetBPP(bitmap);
        // Get a pointer to the pixel data
        BYTE *data = (BYTE*)FreeImage_GetBits(bitmap);
        // Process only RGB and RGBA images
        if(pixel_size == 24) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_BGR, GL_UNSIGNED_BYTE, (GLvoid*)data);
        }
        else if (pixel_size == 32) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_BGRA, GL_UNSIGNED_BYTE, (GLvoid*)data);
        }
        else {
            std::cerr << "pixel size = " << pixel_size << " don't know how to process this case. I'm out!" << std::endl;
            exit(-1);
        }
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else {
        std::cerr << "Unable to load the image file " << fname << " I'm out!" << std::endl;
        exit(-1);
    }
    // Clean bitmap;
    FreeImage_Unload(bitmap);
    // active only for static linking
    #ifdef FREEIMAGE_LIB
        FreeImage_DeInitialise();
    #endif
}
/*mat2 GLWidget::w2nd(vec2 pt_w, float w, float h) {

    ortho = mat2(2/w, 1/(pt_w.x),
                 -1/(pt_w.y), -2/h);
    return ortho;
}*/
