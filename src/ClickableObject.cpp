/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	ClickableObject.cpp
 * \brief Implementation of the ClickableObject, that can receive mouse input.
 */
#include "ClickableObject.h"

std::vector<ClickableObject*> ClickableObject::objectList;

ClickableObject::ClickableObject(void)
{
  objectList.push_back(this);
}

void ClickableObject::clickAll()
{
  for (unsigned int i = 0; i < objectList.size(); i++)
  {
    objectList.at(i)->mouseClick();
  }
}

void ClickableObject::drawAll()
{
  for (unsigned int i = 0; i < objectList.size(); i++)
  {
    objectList.at(i)->draw();
  }
}