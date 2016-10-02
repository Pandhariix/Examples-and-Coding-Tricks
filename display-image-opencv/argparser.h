#ifndef _ARG_PARSER_H_
#define _ARG_PARSER_H_

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <string.h>

enum Traitement{VANILLA, GRAYSCALE, BINARIZE, NONE};

class ArgParser
{

public:
	ArgParser();
	void parse(std::string argument, Traitement &travail);

private:


};

#endif