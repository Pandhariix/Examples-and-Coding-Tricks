#include "argparser.h"

ArgParser::ArgParser(){}

void ArgParser::parse(std::string argument, Traitement &travail)
{
	if(argument == "vanilla")
	{
		travail = VANILLA;
	}
	else if(argument == "grayscale")
	{
		travail = GRAYSCALE;
	}
	else if(argument == "binarize")
	{
		travail = BINARIZE;
	}
	else
	{
		travail = NONE;
		std::cout<<"options : vanilla, grayscale, binarize"<<std::endl;
	}
}