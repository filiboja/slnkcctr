// Exit by pressing Escape

#include "stdafx.h"

// std::
#include <iostream> // cout, endl
#include <iomanip> // setiosflags, setprecision
#include <string> // string

// cv::
#include <opencv2/core/core.hpp> // Mat
#include <opencv2/highgui/highgui.hpp> // VideoCapture, namedWindow, imshow

#include "CaptureProperties.hpp" // CaptureProperties
#include "DetectorColor.hpp" // DetectorColor

int main(int argc, char *argv[]) {
	// Configuration
	const char * const WIN_ORIGINAL = "slnkcctr";
	const char * const WIN_DETECTOR = "Detector";
	const int KEY_ESC = 27;
	const int KEY_PAUSE = 32;
	double frameWidth = 640.0;
	double frameHeight = 480.0;
	enum CapMode { CAP_MODE_DEVICE, CAP_MODE_FILE };
	CapMode capMode = CAP_MODE_FILE;
	const int capDevice = 0;
	const std::string capFilename = "C:\\Users\\TTT\\Videos\\Logitech Webcam\\Slinky night.wmv";
	const bool output = false;
	const std::string outputFilename = "out.avi";
	const int defaultFps = 15; // in case FPS is not provided by `cap`
	const bool showOriginal = true;

	// Format `double` output
	std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(3);

	// Print instructions
	std::cout << "slnkcctr" << std::endl;
	std::cout << "Press Escape to exit." << std::endl;

	// Open cap stream
	cv::VideoCapture cap;
	switch (capMode) {
	case CAP_MODE_DEVICE:
		std::cout << "Capture mode: device" << std::endl;
		std::cout << "Capture device: " << capDevice << std::endl;
		cap = cv::VideoCapture(capDevice);
		break;
	case CAP_MODE_FILE:
		std::cout << "Capture mode: file" << std::endl;
		std::cout << "Capture filename: " << capFilename << std::endl;
		cap = cv::VideoCapture(capFilename);
		break;
	}
	if (!cap.isOpened()) {
		std::cout << "Could not open capture." << std::endl;
		std::cout << "Exiting with code 1." << std::endl;
		return 1;
	}

	// Set
	std::cout << "Set frame width: " << cap.set(CV_CAP_PROP_FRAME_WIDTH, frameWidth) << std::endl;
	std::cout << "Set frame height: " << cap.set(CV_CAP_PROP_FRAME_HEIGHT, frameHeight) << std::endl;

	// Print capture info
	CaptureProperties capProps;
	std::cout << "Capture properties:" << std::endl;
	capProps.print(cap);
	
	// Open window
	if (showOriginal) {
		cv::namedWindow(WIN_ORIGINAL, CV_WINDOW_AUTOSIZE);
	}

	int fps = (int)cap.get(CV_CAP_PROP_FPS);
	if (fps <= 0) {
		fps = defaultFps;
	}
	int delay = 1000 / fps;
	std::cout << "Desired FPS: " << fps << std::endl;
	std::cout << "Frame delay: " << delay << std::endl;
	std::cout << "Actual FPS: " << 1000.0 / delay << std::endl;

	// Open output video
	cv::Size size((int) cap.get(CV_CAP_PROP_FRAME_WIDTH), (int) cap.get(CV_CAP_PROP_FRAME_HEIGHT));
	cv::VideoWriter outputVideo;
	if (output) {
		outputVideo.open(outputFilename, -1, fps, size, true);
		if (!outputVideo.isOpened()) {
			std::cout << "Could not open output video stream." << std::endl;
		}
	}

	DetectorColor detector(WIN_DETECTOR);

	// Main loop
	int key = 0;
	assert(key != KEY_ESC);
	bool pause = false;
	cv::Mat frame;
	while (key != KEY_ESC) {
		if (key == KEY_PAUSE) {
			pause = !pause;
		}
		if (!pause) {
			cap >> frame;
			if (frame.empty()) {
				std::cout << "Empty frame." << std::endl;
				break;
			}
		}
		FrameAnnotation annotation = detector.detect(frame);
		if (showOriginal) {
			cv::Mat annotationImg = cv::Mat::zeros(frame.size(), CV_8UC3);
			annotation.draw(annotationImg);
			cv::Mat frameAnnotated = frame + annotationImg;
			cv::imshow(WIN_ORIGINAL, frameAnnotated);
		}
		if (output && outputVideo.isOpened()) {
			outputVideo << frame;
		}
		key = cv::waitKey(delay);
	}

	return 0;
}
