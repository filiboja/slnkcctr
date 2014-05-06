// Displays video stream from first camera
// Exit by pressing Escape

#include "stdafx.h"

// std::
#include <iostream> // cout, endl
#include <iomanip> // setiosflags, setprecision
#include <set> // set
#include <utility> // pair

// cv::
#include <opencv2/core/core.hpp> // Mat
#include <opencv2/highgui/highgui.hpp> // VideoCapture

static void initProperties();
static void printProperties(cv::VideoCapture& cap);

int main(int argc, char *argv[]) {
	// Configuration
	const int delay = 10;
	const char* WIN_MAIN = "slnkcctr";
	const char KEY_ESC = (char)27;
	double frameWidth = 640.0;
	double frameHeight = 480.0;

	initProperties();

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

// http://en.cppreference.com/w/cpp/language/enum
typedef int PropertyBase;

// <index, description>
typedef std::pair<const PropertyBase, const char * const> Property;
typedef std::set<Property> Properties;

static Properties properties;

static void initProperties() {
	properties.insert(Property(CV_CAP_PROP_FRAME_WIDTH, "Frame width"));
	properties.insert(Property(CV_CAP_PROP_FRAME_HEIGHT, "Frame height"));
	properties.insert(Property(CV_CAP_PROP_FPS, "FPS"));
	properties.insert(Property(CV_CAP_PROP_MODE, "Mode"));
	properties.insert(Property(CV_CAP_PROP_FORMAT, "Format"));
}

// Prints some properties of `cap` into `std::cout`
// http://docs.opencv.org/modules/highgui/doc/reading_and_writing_images_and_video.html#videocapture-get
static void printProperties(cv::VideoCapture& cap) {
	for (Properties::iterator it=properties.begin(); it!=properties.end(); ++it) {
		const PropertyBase& index = it->first;
		const char * const& description = it->second;
		std::cout << description << ": " << cap.get(index) << std::endl;
	}
}
