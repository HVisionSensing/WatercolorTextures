#ifndef TEXT_IMAGE_H
#define TEXT_IMAGE_H

#include "staticimage.h"

class TextImage : public StaticImage
{
public:
	TextImage() {}

	void loadTextFile(std::string filename);

	void setText(std::string text);

protected:
	void initialize();

protected:
	std::vector<std::string> textLines;
};

#endif