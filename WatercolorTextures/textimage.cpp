#include <fstream>
#include <streambuf>
#include "textimage.h"

void TextImage::loadTextFile(std::string filename)
{
	std::ifstream ifs(filename);
	textLines.clear();

	while (!ifs.eof()) {
		std::string line;
		std::getline(ifs, line);
		textLines.push_back(line);
	}

	initialize();
}

void TextImage::setText(std::string text)
{
	textLines.clear();
	textLines.push_back(text);
	initialize();
}

void TextImage::initialize()
{
	ASSERT(hasTarget())
	//*mask = opacity;
	*target = cv::Scalar(255,255,255);

	int i = 25;
	for (std::vector<std::string>::iterator it = textLines.begin(); it != textLines.end(); ++it, i += 20)
		cv::putText(*target, *it, cv::Point(15,i), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0,0,0));
}
