#pragma once
#ifndef _FONT_METRICS_H
#define _FONT_METRICS_H

#include "FontMetrics.h"

#include <fstream>

#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>

#include <sstream>

using namespace rapidxml;


FontMetrics::FontMetrics(std::string &xmlPath)
{
	loadFromXML(xmlPath);
}

bool FontMetrics::loadFromXML(std::string &xmlPath)
{
	xml_document<> doc;
	
	std::ifstream file(xmlPath);
	
	std::stringstream buffer;
	buffer << file.rdbuf();

	file.close();

	std::string content(buffer.str());

	try
	{
		doc.parse<parse_full>(&content[0]);
	}
	catch(parse_error error)
	{
		//Make an array of characters to hold everything up to where the error occurred.
		//Use some clever pointer arithmetic. :P
		unsigned int size = error.where<char>() - &content[0];
		char *test = new char[size];

		memcpy(test, &content[0], size);

		unsigned int lineCount = 1;

		for(unsigned int i = 0; i < size; i++)
			if(test[i] == '\r')
				lineCount++;

		delete [] test;

		//Now we have the line the error occurred on!
		std::cerr << error.what() << ": on line " << lineCount;

		return false;
	}


	xml_node<> *metricsnode;

	metricsnode = doc.first_node("fontMetrics");

	//int entry = 0;

	if(!metricsnode)
		return false;

	xml_node<> *node = metricsnode->first_node();

	while(node != NULL)
	{
		xml_node<> *x, *y, *width, *height;

		xml_attribute<> *key, *path;

		key = node->first_attribute("key");
		path = node->first_attribute("path");

		x = node->first_node("x");
		y = node->first_node("y");
		width = node->first_node("width");
		height = node->first_node("height");

		//size_t key = atoi(name->value());

		node = node->next_sibling();

		if(!(key && x && y && width && height))
		{
			std::cerr << "ERROR!\n";
			continue;
		}

		GlyphMetrics &g = mCharacterMetrics[atoi(key->value()) % MAX_ALPHABET];

		g.mPos.setX(atof(x->value()));
		g.mPos.setY(atof(y->value()));

		g.mSize.setX(atof(width->value()));
		g.mSize.setY(atof(height->value()));

		//entry++;
	}

	return true;
}

#endif