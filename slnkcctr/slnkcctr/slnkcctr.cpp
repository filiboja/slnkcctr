// Displays video stream from first camera
// Exit by pressing Escape

#include "stdafx.h"

#include <iostream> // cout, endl
#include <iomanip> // setiosflags, setprecision

#include <opencv2/core/core.hpp> // Mat
#include <opencv2/highgui/highgui.hpp> // VideoCapture

static void printProperties(cv::VideoCapture& cap);

int main(int argc, char *argv[]) {
	// Configuration
	const int delay = 10;
	const char* WIN_MAIN = "slnkcctr";
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

	// Print camera info
	printProperties(video);

	// Set
	std::cout << "Set frame width: " << video.set(CV_CAP_PROP_FRAME_WIDTH, frameWidth) << std::endl;
	std::cout << "Set frame height: " << video.set(CV_CAP_PROP_FRAME_HEIGHT, frameHeight) << std::endl;

	// Print camera info
	printProperties(video);
	
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

// Prints some properties of `cap` into `std::cout`
// http://docs.opencv.org/modules/highgui/doc/reading_and_writing_images_and_video.html#videocapture-get
static void printProperties(cv::VideoCapture& cap) {
	std::cout << "Frame width: " << cap.get(CV_CAP_PROP_FRAME_WIDTH) << std::endl;
	std::cout << "Frame height: " << cap.get(CV_CAP_PROP_FRAME_HEIGHT) << std::endl;
	std::cout << "FPS: " << cap.get(CV_CAP_PROP_FPS) << std::endl;
	std::cout << "Mode: " << cap.get(CV_CAP_PROP_MODE) << std::endl;
	std::cout << "Format: " << cap.get(CV_CAP_PROP_FORMAT) << std::endl;
}
