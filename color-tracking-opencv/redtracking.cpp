#include <iostream>
#include <string>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

enum EnableTracking{OBJECT_DETECTED,OBJECT_NOT_DETECTED};


bool compare_rect(const Rect &a, const Rect &b)
{
    return a.area() < b.area();
}

void defineTarget(std::vector<cv::Rect> potentialTargets, std::vector<cv::Rect> &target, int nbTargets)
{

    std::sort(potentialTargets.begin(), potentialTargets.end(), compare_rect);

    if(nbTargets >= potentialTargets.size())
    {
        nbTargets = potentialTargets.size()-1;
    }

    target.clear();

    for(std::vector<cv::Rect>::iterator it = potentialTargets.end()-nbTargets; it != potentialTargets.end(); it++)
    {
        target.push_back(*it);
    }
}

void defineCenter(std::vector<cv::Rect> target, std::vector<cv::Point> &centers)
{
    cv::Point currentPoint;
    centers.clear();

    for(std::vector<cv::Rect>::iterator it = target.begin(); it != target.end(); it++)
    {
        currentPoint.x = (*it).x + (*it).width/2;
        currentPoint.y = (*it).y + (*it).height/2;

        centers.push_back(currentPoint);
    }
}



int main( int argc, char** argv )
{

    VideoCapture cap(0); //capture the video from web cam

    if ( !cap.isOpened() )  // if not success, exit program
    {
        cout << "Cannot open the web cam" << endl;
        return -1;
    }

    namedWindow("Autopilote Target Setter", CV_WINDOW_AUTOSIZE); //create a window called "Control"

    EnableTracking trackingStatus = OBJECT_NOT_DETECTED;
    std::vector<cv::Vec4i> hierarchy;
    std::vector<std::vector<cv::Point> > contours;
    std::vector<std::vector<cv::Point> > contours_poly;
    std::vector<cv::Rect> targetZone;
    std::vector<cv::Rect> target;
    std::vector<cv::Point> centers;

    int iLowH = 0;
    int iHighH = 179;

    int iLowS = 0;
    int iHighS = 255;

    int iLowV = 0;
    int iHighV = 255;

    int tracking = 0;
    int nbTargets = 1;

    //Create trackbars in "Autopilote Target Setter" window
    cvCreateTrackbar("LowH", "Autopilote Target Setter", &iLowH, 179); //Hue (0 - 179)
    cvCreateTrackbar("HighH", "Autopilote Target Setter", &iHighH, 179);

    cvCreateTrackbar("LowS", "Autopilote Target Setter", &iLowS, 255); //Saturation (0 - 255)
    cvCreateTrackbar("HighS", "Autopilote Target Setter", &iHighS, 255);

    cvCreateTrackbar("LowV", "Autopilote Target Setter", &iLowV, 255); //Value (0 - 255)
    cvCreateTrackbar("HighV", "Autopilote Target Setter", &iHighV, 255);

    cvCreateTrackbar("nbTrackedOjects", "Autopilote Target Setter", &nbTargets, 50); //tracking (0 - 1)
    cvCreateTrackbar("Tracking", "Autopilote Target Setter", &tracking, 1); //tracking (0 - 1)

    while (true)
    {
        Mat imgOriginal;

        bool bSuccess = cap.read(imgOriginal); // read a new frame from video

        if (!bSuccess) //if not success, break loop
        {
            cout << "Cannot read a frame from video stream" << endl;
            break;
        }

        Mat imgHSV;

        cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV

        Mat imgThresholded;

        inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image

        //morphological opening (remove small objects from the foreground)
        erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

        //morphological closing (fill small holes in the foreground)
        dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

        if(tracking)
        {
            trackingStatus = OBJECT_DETECTED;
        }
        else
        {
            trackingStatus = OBJECT_NOT_DETECTED;
        }

        if(trackingStatus == OBJECT_DETECTED)
        {
            dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
            cv::findContours(imgThresholded, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE, cv::Point(0,0));

            contours_poly.clear();
            contours_poly.resize(contours.size());

            targetZone.clear();
            targetZone.resize(contours.size());

            for(size_t j=0;j<contours.size();j++)
            {
                cv::approxPolyDP(cv::Mat(contours[j]), contours_poly[j], 3, true);
                targetZone[j] = cv::boundingRect(cv::Mat(contours_poly[j]));
            }

            defineTarget(targetZone, target, nbTargets);
            defineCenter(target, centers);

            if(contours.size() != 0)
            {
                for(size_t i=0; i<target.size(); i++)
                {
                    //cv::rectangle(imgOriginal, target[i], cv::Scalar( 0, 0, 255), 2, 8, 0 );
                    cv::circle(imgOriginal, centers[i], 1, cv::Scalar(0,255,0), 7, 24);
                    cv::putText(imgOriginal, "X: "+std::to_string(centers[i].x)+" Y: "+std::to_string(centers[i].y), centers[i], cv::FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0), 2.0);
                }
            }
        }

        imshow("Thresholded Image", imgThresholded);
        imshow("Original", imgOriginal); //show the original image

        if (waitKey(30) == 27)
        {
            cout << "esc key is pressed by user" << endl;
            break;
        }
    }

    return 0;

}
