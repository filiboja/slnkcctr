// Displays video stream from first camera
// Exit by pressing Escape

#include "stdafx.h"

// std::
#include <iostream> // cout, endl
#include <iomanip> // setiosflags, setprecision

// cv::
#include <opencv2/core/core.hpp> // Mat
#include <opencv2/highgui/highgui.hpp> // VideoCapture, namedWindow, imshow

#include "CaptureProperties.hpp" // CaptureProperties
#include "DetectorColor.hpp" // DetectorColor

int main(int argc, char *argv[]) {
	// Configuration
	const int delay = 10;
	const char * const WIN_MAIN = "slnkcctr";
	const char KEY_ESC = (char)27;
	double frameWidth = 640.0;
	double frameHeight = 480.0;

	// Format `double` output
	std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(3);

	// Print instructions
	std::cout << "Press Escape to exit." << std::endl;

	// Open video stream
	cv::VideoCapture video(0);
	if (!video.isOpened()) {
		std::cout << "Could not open video stream." << std::endl;
		return 1;
	}

	CaptureProperties capProps;

	// Set
	std::cout << "Set frame width: " << video.set(CV_CAP_PROP_FRAME_WIDTH, frameWidth) << std::endl;
	std::cout << "Set frame height: " << video.set(CV_CAP_PROP_FRAME_HEIGHT, frameHeight) << std::endl;

	// Print camera info
	capProps.print(video);
	
	// Open window
	cv::namedWindow(WIN_MAIN, CV_WINDOW_AUTOSIZE);

	DetectorColor detector;

	char c = 0;
	// Main loop
	while (c != KEY_ESC) {
		cv::Mat frame;
		video >> frame;
		if (frame.empty()) {
			std::cout << "Empty frame." << std::endl;
		}
		cv::imshow(WIN_MAIN, frame);
		FrameAnnotation annotation = detector.detect(frame);
		std::cout << annotation << std::endl;
		c = (char)cvWaitKey(delay);
	}

	return 0;
}
