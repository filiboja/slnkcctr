// Displays video stream from first camera
// Exit by pressing Escape

#include "stdafx.h"

#include <iostream> // cout
#include <iomanip> // setiosflags, setprecision

#include <opencv2/core/core.hpp> // Mat
#include <opencv2/highgui/highgui.hpp> // VideoCapture

int main(int argc, char *argv[]) {
	// Configuration
	const int delay = 10;
	const char* WIN_MAIN = "slnkcctr";
	const char KEY_ESC = (char)27;

	// Format `double` output
	std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(3);

	// Print instructions
	std::cout << "Exit by pressing Escape." << std::endl;

	// Open video stream
	cv::VideoCapture video(0);
	if (!video.isOpened()) {
		std::cout << "Could not open video stream." << std::endl;
		return 1;
	}
	
	// Open window
	cv::namedWindow(WIN_MAIN, CV_WINDOW_AUTOSIZE);

	char c = 0;
	// Main loop
	while (c != KEY_ESC) {
		cv::Mat frame;
		video >> frame;
		if (frame.empty()) {
			std::cout << "Empty frame." << std::endl;
		}
		cv::imshow(WIN_MAIN, frame);
		c = (char)cvWaitKey(delay);
	}

	return 0;
}
