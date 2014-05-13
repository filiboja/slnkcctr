// Exit by pressing Escape

#include "stdafx.h"

// c
#include <ctime> // clock, CLOCKS_PER_SEC

// std::
#include <iostream> // cout, endl
#include <iomanip> // setiosflags, setprecision
#include <string> // string

// cv::
#include <opencv2/core/core.hpp> // Mat
#include <opencv2/highgui/highgui.hpp> // VideoCapture, namedWindow, imshow
#include <opencv2/imgproc/imgproc.hpp> // resize

// boost::
#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include "CapMode.h" // CapMode
#include "CaptureProperties.h" // CaptureProperties
#include "DetectorSlinky.h" // DetectorColor

static void onFps(int, void*);

static int fps;
static int delay;

enum {
	SUCCESS = 0,
	ERROR_SOURCE_OPEN = 1,
	ERROR_UNKNOWN_OPTION = 2,
};

int main(int argc, char *argv[]) {
	const char * const configFilename = "slnkcctr.ini";
	const std::string sourceWinname = "Source";

	po::options_description optionsBasic("Basic");
	optionsBasic.add_options()
		("help", "produce help message")
	;

	// Source options
	CapMode sourceCapMode = CAP_MODE_DEVICE;
	int sourceCapDevice = 0;
	std::string sourceCapFilename = "";
	int sourceFrameWidth = 640;
	int sourceFrameHeight = 480;
	double sourceFps = 15;
	bool sourceShow = true;

	po::options_description optionsSource("Source");
	optionsSource.add_options()
		("source.mode,m", po::value<CapMode>(&sourceCapMode)->default_value(sourceCapMode), "mode (d[evice] or f[ile])")
		("source.device,d", po::value<int>(&sourceCapDevice)->default_value(sourceCapDevice), "source capture device (only used in device mode)")
		("source.filename,f", po::value<std::string>(&sourceCapFilename), "source video filename (only used in file mode)")
		("source.width", po::value<int>(&sourceFrameWidth), "frame width (leave empty for original)")
		("source.height", po::value<int>(&sourceFrameHeight), "frame height (leave empty for original)")
		("source.fps", po::value<double>(&sourceFps), "frames per second")
		("source.show", po::value<bool>(&sourceShow)->default_value(true, "true"), "show source video stream")
	;

	// Detector options
	int detectorFrameWidth;
	int detectorFrameHeight;
	bool detectorShow = true;

	po::options_description optionsDetector("Detector");
	optionsSource.add_options()
		("detector.width", po::value<int>(&detectorFrameWidth), "frame width (leave empty for original)")
		("detector.height", po::value<int>(&detectorFrameHeight), "frame height (leave empty for original)")
		("detector.show", po::value<bool>(&detectorShow)->default_value(true, "true"), "show detector video stream")
	;

	po::options_description options("Options");
	options.add(optionsBasic).add(optionsSource).add(optionsDetector);

	po::variables_map vm;

	// Load options from command line
	try {
		po::store(po::parse_command_line(argc, argv, options), vm);
	} catch (po::unknown_option& e) {
		std::cerr << e.what() << std::endl;
		return ERROR_UNKNOWN_OPTION;
	}

	// Load options from configuration file
	try {
		po::basic_parsed_options<char> fileOptions = po::parse_config_file<char>(configFilename, optionsBasic);
		po::store(fileOptions, vm);
	} catch (po::reading_file& e) {
		std::cerr << e.what() << std::endl;
	}

	po::notify(vm);

	if (vm.count("help")) {
		std::cout << options << std::endl;
		return SUCCESS;
	}

	// Configuration (obsolete)
	const char * const WIN_DETECTOR = "Detector";
	const int KEY_ESC = 27;
	const int KEY_PAUSE = 32;
	const bool showControls = true;
	const std::string WIN_CONTROLS = "Playback controls";

	// Format `double` output
	std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(3);

	// Print instructions
	std::cout << "slnkcctr" << std::endl;
	std::cout << "Press Escape to exit." << std::endl;

	// Open source video stream
	cv::VideoCapture cap;
	std::cout << "Source capture mode: " << sourceCapMode << std::endl;
	switch (sourceCapMode) {
	case CAP_MODE_DEVICE:
		std::cout << "Source device: " << sourceCapDevice << std::endl;
		cap = cv::VideoCapture(sourceCapDevice);
		break;
	case CAP_MODE_FILE:
		std::cout << "Source filename: " << sourceCapFilename << std::endl;
		cap = cv::VideoCapture(sourceCapFilename);
		break;
	}
	if (!cap.isOpened()) {
		std::cerr << "Could not open source capture." << std::endl;
		return ERROR_SOURCE_OPEN;
	}

	// Set source frame dimensions
	if (vm.count("source.width")) {
		if (!cap.set(CV_CAP_PROP_FRAME_WIDTH, (double)sourceFrameWidth)) {
			std::cerr << "Failed to set source frame width." << std::endl;
		}
	}
	if (vm.count("source.height")) {
		if (!cap.set(CV_CAP_PROP_FRAME_HEIGHT, (double)sourceFrameHeight)) {
			std::cerr << "Failed to set source frame height." << std::endl;
		}
	}

	// Update source frame dimensions
	sourceFrameWidth = (int)cap.get(CV_CAP_PROP_FRAME_WIDTH);
	sourceFrameHeight = (int)cap.get(CV_CAP_PROP_FRAME_HEIGHT);

	// Set detector frame dimensions
	if (!vm.count("detector.width")) {
		detectorFrameWidth = sourceFrameWidth;
	}
	if (!vm.count("detector.height")) {
		detectorFrameHeight = sourceFrameHeight;
	}

	// Print capture info
	CaptureProperties capProps;
	std::cout << "Capture properties:" << std::endl;
	capProps.print(cap);
	
	// Open window
	if (sourceShow) {
		cv::namedWindow(sourceWinname, cv::WINDOW_NORMAL);
		cv::resizeWindow(sourceWinname, sourceFrameWidth, sourceFrameHeight);
	}

	// FPS
	if (!vm.count("source.fps")) {
		sourceFps = cap.get(CV_CAP_PROP_FPS);
		if (sourceFps <= 0.0) {
			std::cerr << "Failed to get FPS from capture." << std::endl;
			sourceFps = 1.0;
		}
	}
	assert(sourceFps > 0.0);

	/*onFps(0, NULL);
	if (showControls) {
		cv::namedWindow(WIN_CONTROLS, cv::WINDOW_AUTOSIZE);
		cv::createTrackbar("FPS", WIN_CONTROLS, &fps, 60, onFps);
	}*/
	delay = (int)(1000.0 / sourceFps);
	std::cout << "Desired FPS: " << fps << std::endl;
	std::cout << "Frame delay: " << delay << std::endl;
	std::cout << "Actual FPS: " << 1000.0 / delay << std::endl;

	cv::Size detectorFrameSize = cv::Size(detectorFrameWidth, detectorFrameHeight);
	DetectorSlinky detector(detectorFrameSize);

	// Main loop
	int key = 0;
	assert(key != KEY_ESC);
	bool pause = false;
	cv::Mat frameSource;
	cv::Mat frameDetector;
	clock_t clockBegin = clock();
	while (key != KEY_ESC) {
		if (key == KEY_PAUSE) {
			pause = !pause;
		}
		if (!pause) {
			cap >> frameSource;
			if (frameSource.empty()) {
				std::cout << "Empty source frame." << std::endl;
				break;
			}
			if (sourceShow) {
				cv::imshow(sourceWinname, frameSource);
			}
			cv::resize(frameSource, frameDetector, detectorFrameSize, 0.0, 0.0, cv::INTER_AREA);
		}
		FrameAnnotation annotation = detector.detect(frameDetector);
		//if (sourceShow) {
			//cv::imshow(sourceWinname, frameSource);
			//cv::Mat annotationImg = cv::Mat::zeros(frameDetector.size(), CV_8UC3);
			//annotation.draw(annotationImg);
			//cv::Mat frameAnnotated = frameSource + annotationImg;
			//cv::imshow(WIN_ORIGINAL, frameAnnotated);
		//}
		clock_t clockEnd = clock();
		assert(clockEnd >= clockBegin);
		clock_t clockDiff = clockEnd - clockBegin;
		int msecDiff = clockDiff * 1000 / CLOCKS_PER_SEC;
		int delayCur = delay - msecDiff;
		if (delayCur <= 0) {
			delayCur = 1;
		}
		clockBegin = clockEnd;
		assert(delayCur > 0); // Don't wait forever
		key = cv::waitKey(delayCur);
	}

	return SUCCESS;
}

static void onFps(int, void*)
{
	if (fps <= 0) {
		fps = 1;
	}
	delay = 1000 / fps; // in milliseconds
}
