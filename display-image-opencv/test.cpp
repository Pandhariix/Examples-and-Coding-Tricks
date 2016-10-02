#include "argparser.h"
#include <opencv2/opencv.hpp>

int main( int argc, char** argv )
{
    ArgParser parser;
    Traitement travail;

    cv::Mat image;
    cv::Mat dispImage;

    if( argc != 3)
    {
     std::cout <<" Usage: display_image ImageToLoadAndDisplay, filter to apply" << std::endl;
     return -1;
    }

    image = cv::imread(argv[1], CV_LOAD_IMAGE_COLOR);   // Read the file

    if(! image.data )                              // Check for invalid input
    {
        std::cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }

    parser.parse(argv[2], travail);

    switch(travail)
    {
        case VANILLA:
            dispImage = image;
            break;

        case GRAYSCALE:
            cv::cvtColor(image, dispImage, CV_BGR2GRAY);
            break;

        case BINARIZE:
            cv::cvtColor(image, dispImage, CV_BGR2GRAY);
            cv::threshold(dispImage, dispImage, 50, 255, cv::THRESH_BINARY);
            break;

        case NONE:
            return 0;
            break;
    }

    cv::namedWindow( "Display window", cv::WINDOW_AUTOSIZE );// Create a window for display.
    cv::imshow( "Display window", dispImage );                   // Show our image inside it.

    cv::waitKey(0);                                          // Wait for a keystroke in the window
    return 0;
}