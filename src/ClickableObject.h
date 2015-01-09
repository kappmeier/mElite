/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	ClickableObject.h
 * \brief Declaration of the ClickableObject class, that is a graphcis object that can receive mouse input
 */
#pragma once
#include <vector>

//! An abstract class for objects that can receive mouse input. The mouse handling and drawing must be implemented.
class ClickableObject abstract
{
public:
	ClickableObject( void );
	virtual void mouseClick() = 0;
	virtual bool isMouseOverObject() = 0;
	virtual void draw() = 0;
	static void clickAll();
	static void drawAll();
private:
	static std::vector<ClickableObject*> objectList;
};