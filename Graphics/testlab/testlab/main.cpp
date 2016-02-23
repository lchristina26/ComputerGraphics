#include <QImage>
#include <QColor>
#include <QString>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <QCoreApplication>
#include <QTextStream>
#include <QFile>
#include <QDebug>

using namespace std;

typedef struct {
    float x, y;
} Vertex;

bool _compare_min_x(Vertex const &p1, Vertex const &p2) { return p1.x < p2.x; }
bool _compare_min_y(Vertex const &p1, Vertex const &p2) { return p1.y < p2.y; }

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    float min_x, min_y, max_x, max_y;
    QString ptstr;
    QStringList points, xy_split;
    QVector<Vertex> pt_arr(3);
    int i = 0, count = 0;
    QTextStream streamOut(stdout);
    QTextStream streamIn(stdin);

    streamOut << "Enter 3 space separated points (enter a point as x,y:r,g,b): " << endl;
    ptstr = streamIn.readLine();
    points = ptstr.split(" ");

    streamOut << "You entered: \n";
    for (i = 0; i < points.size(); i++) {
        streamOut << points[i] << endl;
        xy_split = points[i].split(",");
        //xy_split has to be made into two diff floats...
        if (count % 2 == 0) {
            //pt_arr[i].x = xy_split[i].toFloat();
            count++;
        }
        else {
            //pt_arr[i].y = xy_split[i].toFloat();
            count++;
        }
    }


    //QImage image(640, 480, QImage::Format_RGB32);

    /*
       Part 1:
       Calculate the bounding box of the 3 provided points and loop
       over each pixel in that box and set it to white using:
     */

    if(pt_arr.size()>1) {
        min_x = (*std::min_element(pt_arr.begin(), pt_arr.end(), &_compare_min_x)).x;
        min_y = (*std::min_element(pt_arr.begin(), pt_arr.end(), &_compare_min_y)).y;

        max_x = (*std::max_element(pt_arr.begin(), pt_arr.end(), &_compare_min_x)).x;
        max_y = (*std::max_element(pt_arr.begin(), pt_arr.end(), &_compare_min_y)).y;
    }
    streamOut << min_x << " " << min_y << " " << max_x << " " << max_y;
    //image.setPixel(50,50, qRgb(255,255,255));
    /*
       Part 2:
       Modify your loop from part 1. Using barycentric coordinates,
       determine if the pixel lies within the triangle defined by
       the 3 provided points. If it is color it white, otherwise
       move on to the next pixel.

       For more on barycentric coordinates:
http://en.wikipedia.org/wiki/Barycentric_coordinate_system

Part 3:
For each pixel in the triangle, calculate the color based on
the calculated barycentric coordinates and the 3 provided
colors. Your colors should have been entered as floating point
numbers from 0 to 1. Since we have a 32 bit image, the red,
green and blue components range from 0 to 255. Be sure to make
the conversion.
     */

    /*if(image.save("triangle.jpg",0,100)) {
        cout << "Output triangle.jpg" << endl;
    } else {
        cout << "Unable to save triangle.jpg" << endl;
    }*/

    return a.exec();
}
