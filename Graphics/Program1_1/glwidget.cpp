#include "glwidget.h"
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <QTextStream>

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

using glm::vec3;
using glm::value_ptr;
using glm::ortho;

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent) { 
    tx = 0;
    ty = 0;
    tz = 0;

    sx = 1;
    sy = 1;
    sz = 1;

    rx = 0;
    ry = 0;
    rz = 0;

    camAngle = 0;
    Translate = glm::mat4(
            vec4(1,0,0,0),
            vec4(0,1,0,0),
            vec4(0,0,1,0),
            vec4(tx,ty,tz,1));

    Rotate1 = glm::mat4(
                vec4(1,0,0,0),
                vec4(0,(float)cos(rx),(float)sin(rx),0),
                vec4(0,(float)-sin(rx),(float)cos(rx), 0),
                vec4(0,0,0,1));
    Rotate2 = glm::mat4(
                vec4((float)cos(ry), 0,(float)-sin(ry),0),
                vec4(0,1,0,0),
                vec4((float)sin(ry),0,(float)cos(ry),0),
                vec4(0,0,0,1));
    Rotate3 = glm::mat4(
                vec4((float)cos(rz),(float)sin(rz),0,0),
                vec4((float)-sin(rz),(float)cos(rz), 0,0),
                vec4(0,0,1,0),
                vec4(0,0,0,1));

    Rotate = Rotate1* Rotate2 * Rotate3;
    Scale = glm::mat4(
                vec4(sx,0,0,0),
                vec4(0,sy,0,0),
                vec4(0,0,sz,0),
                vec4(0,0,0,1));
    Model = Translate * Rotate * Scale;
    View = glm::lookAt(
        glm::vec3(cos(camAngle),camY,sin(camAngle)),
        glm::vec3(0,0,0),
        glm::vec3(0,1,0)
    );
}

GLWidget::~GLWidget() {
}

void GLWidget::initializeGrid() {
    glGenVertexArrays(1, &gridVao);
    glBindVertexArray(gridVao);

    // Create a buffer on the GPU for position data
    GLuint positionBuffer;
    glGenBuffers(1, &positionBuffer);

    vec3 pts[84];
    for(int i = -10; i <= 10; i++) {

        pts[2*(i+10)] = vec3(i, -.5f, 10);
        pts[2*(i+10)+1] = vec3(i, -.5f, -10);

        pts[2*(i+10)+42] = vec3(10,-.5f, i);
        pts[2*(i+10)+43] = vec3(-10,-.5f, i);
    }

    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pts), pts, GL_STATIC_DRAW);

    // Load our vertex and fragment shaders into a program object
    // on the GPU
    GLuint program = loadShaders(":/grid_vert.glsl", ":/grid_frag.glsl");
    glUseProgram(program);
    gridProg = program;

    // Bind the attribute "position" (defined in our vertex shader)
    // to the currently bound buffer object, which contains our
    // position data for a single triangle. This information 
    // is stored in our vertex array object.
    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
    GLint positionIndex = glGetAttribLocation(program, "position");
    glEnableVertexAttribArray(positionIndex);
    glVertexAttribPointer(positionIndex, 3, GL_FLOAT, GL_FALSE, 0, 0);

    gridProjMatrixLoc = glGetUniformLocation(gridProg, "projection");
    viewGridPos = glGetUniformLocation(gridProg, "View");
    // Part 2 - Get any uniform variable locations that you'll need.
}

void GLWidget::initializeCube() {
    // Create a new Vertex Array Object on the GPU which
    // saves the attribute layout of our vertices.
    glGenVertexArrays(1, &cubeVao);
    glBindVertexArray(cubeVao);

    // Create a buffer on the GPU for position data
    GLuint positionBuffer;
    glGenBuffers(1, &positionBuffer);

    GLuint colorBuffer;
    glGenBuffers(1, &colorBuffer);

    GLuint indexBuffer;
    glGenBuffers(1, &indexBuffer);

    vec3 pts[] = {
        // top
        vec3(1,1,1),    // 0
        vec3(1,1,-1),   // 1
        vec3(-1,1,-1),  // 2
        vec3(-1,1,1),   // 3

        // bottom
        vec3(1,-1,1),   // 4
        vec3(-1,-1,1),  // 5
        vec3(-1,-1,-1), // 6
        vec3(1,-1,-1),  // 7

        // front
        vec3(1,1,1),    // 8
        vec3(-1,1,1),   // 9
        vec3(-1,-1,1),  // 10
        vec3(1,-1,1),   // 11

        // back
        vec3(-1,-1,-1), // 12
        vec3(-1,1,-1),  // 13
        vec3(1,1,-1),   // 14
        vec3(1,-1,-1),  // 15

        // right
        vec3(1,-1,1),   // 16
        vec3(1,-1,-1),  // 17
        vec3(1,1,-1),   // 18
        vec3(1,1,1),     // 19

        // left
        vec3(-1,-1,1),  // 20
        vec3(-1,1,1),   // 21
        vec3(-1,1,-1),  // 22
        vec3(-1,-1,-1) // 23

    };

    for(int i = 0; i < 24; i++) {
        pts[i] *= .5;
    }

    vec3 colors[] = {
        // top
        vec3(0,1,0),    
        vec3(0,1,0),    
        vec3(0,1,0),    
        vec3(0,1,0),    

        // bottom
        vec3(0,.5f,0),  
        vec3(0,.5f,0),  
        vec3(0,.5f,0),  
        vec3(0,.5f,0),  

        // front
        vec3(0,0,1),    
        vec3(0,0,1),    
        vec3(0,0,1),    
        vec3(0,0,1),    

        // back
        vec3(0,0,.5f),  
        vec3(0,0,.5f),  
        vec3(0,0,.5f),  
        vec3(0,0,.5f),

        // right
        vec3(1,0,0),    
        vec3(1,0,0),    
        vec3(1,0,0),    
        vec3(1,0,0),    


        // left
        vec3(.5f,0,0),  
        vec3(.5f,0,0),  
        vec3(.5f,0,0),  
        vec3(.5f,0,0)  
    };

    GLuint restart = 0xFFFFFFFF;
    GLuint indices[] = {
        0,1,2,3, restart,
        4,5,6,7, restart,
        8,9,10,11, restart,
        12,13,14,15, restart,
        16,17,18,19, restart,
        20,21,22,23
    };

    // Upload the position data to the GPU, storing
    // it in the buffer we just allocated.
    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pts), pts, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Load our vertex and fragment shaders into a program object
    // on the GPU
    GLuint program = loadShaders(":/vert.glsl", ":/frag.glsl");
    glUseProgram(program);
    cubeProg = program;

    // Bind the attribute "position" (defined in our vertex shader)
    // to the currently bound buffer object, which contains our
    // position data for a single triangle. This information 
    // is stored in our vertex array object.
    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
    GLint positionIndex = glGetAttribLocation(cubeProg, "position");
    glEnableVertexAttribArray(positionIndex);
    glVertexAttribPointer(positionIndex, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    GLint colorIndex = glGetAttribLocation(cubeProg, "color");
    glEnableVertexAttribArray(colorIndex);
    glVertexAttribPointer(colorIndex, 3, GL_FLOAT, GL_FALSE, 0, 0);

    cubeProjMatrixLoc = glGetUniformLocation(cubeProg, "projection");
    viewCubePos = glGetUniformLocation(cubeProg, "View");
    modCubePos = glGetUniformLocation(cubeProg, "Model");
    // Part 2 & 3 - Get any uniform variable locations that you'll need.
}

void GLWidget::initializeGL() {
    initializeOpenGLFunctions();

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glPointSize(4.0f);

    glEnable(GL_DEPTH_TEST);
    GLuint restart = 0xFFFFFFFF;
    glPrimitiveRestartIndex(restart);
    glEnable(GL_PRIMITIVE_RESTART);

    initializeCube();
    initializeGrid();
}

void GLWidget::resizeGL(int w, int h) {
    glViewport(0,0,w,h);

    float aspect = (float)w/h;
    float d = (float)(1/tan(40.0f));
    float near = 0.1;
    float far = 10.0;
    float A = (float)((far+near)/(far-near));
    float B = (float)(2*far*near/(far-near));
    projection = mat4(
                vec4((float)d/aspect,0,0,0),
                vec4(0,-(float)(d/aspect),0,0),
                vec4(0,0,-A,-B),
                vec4(0,0,-1,1));
    //projection = glm::perspective(40.0f, aspect, near, far);

    glUseProgram(cubeProg);
    glUniformMatrix4fv(cubeProjMatrixLoc, 1, false, value_ptr(projection));
    glUniformMatrix4fv(viewCubePos, 1, false, glm::value_ptr(View));
    glUniformMatrix4fv(modCubePos, 1, false, glm::value_ptr(Model));

    glUseProgram(gridProg);
    glUniformMatrix4fv(gridProjMatrixLoc, 1, false, value_ptr(projection));
    glUniformMatrix4fv(viewGridPos, 1,false, glm::value_ptr(View));
}

void GLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderGrid();
    renderCube();
}

void GLWidget::renderCube() {
    glUseProgram(cubeProg);
    glBindVertexArray(cubeVao);
    glDrawElements(GL_TRIANGLE_FAN, 29, GL_UNSIGNED_INT, 0);
}

void GLWidget::renderGrid() {
    glUseProgram(gridProg);
    glBindVertexArray(gridVao);
    glDrawArrays(GL_LINES, 0, 84);
}

GLuint GLWidget::loadShaders(const char* vertf, const char* fragf) {
    GLuint program = glCreateProgram();

    // read vertex shader from Qt resource file
    QFile vertFile(vertf);
    vertFile.open(QFile::ReadOnly | QFile::Text);
    QString vertString;
    QTextStream vertStream(&vertFile);
    vertString.append(vertStream.readAll());
    std::string vertSTLString = vertString.toStdString();

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
    QFile fragFile(fragf);
    fragFile.open(QFile::ReadOnly | QFile::Text);
    QString fragString;
    QTextStream fragStream(&fragFile);
    fragString.append(fragStream.readAll());
    std::string fragSTLString = fragString.toStdString();

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

    return program;
}

void GLWidget::translateX(int x) {
    tx = (float)x/10;
    std::cout << "Translate X " << tx << std::endl;
    updateModelMatrix();
}

void GLWidget::translateY(int y) {
    ty = (float)y/10;
    std::cout << "Translate Y " << ty << std::endl;
    updateModelMatrix();
}

void GLWidget::translateZ(int z) {
    tz = (float)z/10;
    std::cout << "Translate Z " << tz << std::endl;
    updateModelMatrix();
}

void GLWidget::rotateX(int x) {
    std::cout << "Rotate X " << x << std::endl;
    rx = (float)x*M_PI/180;
    updateModelMatrix();
}

void GLWidget::rotateY(int y) {
    std::cout << "Rotate Y " << y << std::endl;
    ry = (float)y*M_PI/180;
    updateModelMatrix();
}

void GLWidget::rotateZ(int z) {
    std::cout << "Rotate Z " << z << std::endl;
    rz = (float)z*M_PI/180;
    updateModelMatrix();
}

void GLWidget::scaleX(int x) {
    sx = (float)x/10;
    std::cout << "Scale X " << sx << std::endl;
    updateModelMatrix();
}

void GLWidget::scaleY(int y) {
    sy = (float)y/10;
    std::cout << "Scale Y " << sy << std::endl;
    updateModelMatrix();
}

void GLWidget::scaleZ(int z) {
    sz = (float)z/10;
    std::cout << "Scale Z " << sz << std::endl;
    updateModelMatrix();
}

void GLWidget::cameraPosition(int angle) {
    camAngle = (float)(angle*M_PI/180);
    std::cout << "Camera angle " << angle << std::endl;
    updateViewMatrix();
}

void GLWidget::cameraY(int y) {
    camY = (float)y/10;
    std::cout << "Camera Y " << y << std::endl;
    updateViewMatrix();
}

void GLWidget::updateModelMatrix() {
    Translate = glm::mat4(
            vec4(1,0,0,0),
            vec4(0,1,0,0),
            vec4(0,0,1,0),
            vec4(tx,ty,tz,1));

    Rotate1 = glm::mat4(
                vec4(1,0,0,0),
                vec4(0,(float)cos(rx),(float)sin(rx),0),
                vec4(0,(float)-sin(rx),(float)cos(rx), 0),
                vec4(0,0,0,1));
    Rotate2 = glm::mat4(
                vec4((float)cos(ry), 0,(float)-sin(ry),0),
                vec4(0,1,0,0),
                vec4((float)sin(ry),0,(float)cos(ry),0),
                vec4(0,0,0,1));
    Rotate3 = glm::mat4(
                vec4((float)cos(rz),(float)sin(rz),0,0),
                vec4((float)-sin(rz),(float)cos(rz), 0,0),
                vec4(0,0,1,0),
                vec4(0,0,0,1));

    Rotate = Rotate1* Rotate2 * Rotate3;
    Scale = glm::mat4(
                vec4(sx,0,0,0),
                vec4(0,sy,0,0),
                vec4(0,0,sz,0),
                vec4(0,0,0,1));
    Model = Translate * Rotate * Scale;
    // Part 3 - Construct a combined model matrix from the translation,
    // rotation and scale values and upload it as a uniform variable to
    // the cube program (don't use it to change the grid). Update your
    // vertex shader accordingly.
}

void GLWidget::updateViewMatrix() {

    View = glm::lookAt(
        glm::vec3(cos(camAngle),camY,sin(camAngle)),
        glm::vec3(0,0,0),
        glm::vec3(0,1,0)
    );

    /*View = glm::inverse(mat4(
                            vec4()));*/
    //printf("%f\n", View[1][2]);
    // Part 2 - Construct a view matrix and upload as a uniform variable
    // to the cube and grid programs. Update your vertex shader accordingly.
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
