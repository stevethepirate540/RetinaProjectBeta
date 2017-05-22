#pragma once
#include "stdafx.h"
#include <iostream>
//#include "VoronoiTest.h"
#include "Quadtree.h"
#include <string>
#include <Windows.h>

//NOTE: This quadtree is based on a quadtree implementation from https://codereview.stackexchange.com/questions/84374/quadtree-implementation with several modifications.

template<typename T>
Quadtree<T>::Quadtree() {
	nw = nullptr;
	ne = nullptr;
	sw = nullptr;
	se = nullptr;

	boundary = QuadRegion(Point(5.0,5.0), Point(5.0,5.0));
	position = "hello";
	objects = std::vector<Photoreceptor*>();
}

template<typename T>
Quadtree<T>::Quadtree(QuadRegion boundary)
{
	objects = std::vector<Photoreceptor*>();
	nw = nullptr;
	ne = nullptr;
	sw = nullptr;
	se = nullptr;
	position = "hi";
	this->boundary = boundary;
}

template<typename T>
Quadtree<T>::~Quadtree()
{
	if (nw != nullptr) {
		delete nw;
		delete sw;
		delete ne;
		delete se;
	}
}

template<typename T>
void Quadtree<T>::subdivide()
{
	Point qSize = Point(boundary.halfSize.x/2, boundary.halfSize.y/2);
	Point qCentre = Point(boundary.centre.x - qSize.x, boundary.centre.y - qSize.y);

	nw = new Quadtree(QuadRegion(qCentre, qSize));

	qCentre = Point(boundary.centre.x + qSize.x, boundary.centre.y - qSize.y);
	ne = new Quadtree(QuadRegion(qCentre, qSize));

	qCentre = Point(boundary.centre.x - qSize.x, boundary.centre.y + qSize.y);
	sw = new Quadtree(QuadRegion(qCentre, qSize));

	qCentre = Point(boundary.centre.x + qSize.x, boundary.centre.y + qSize.y);
	se = new Quadtree(QuadRegion(qCentre, qSize));
}
//int counter = 0;

template<typename T>
QuadRegion Quadtree<T>::getBoundary() {
	return boundary;
}

template<typename T>
bool Quadtree<T>::insert(Data<T> d)
{
	//std::cout << "putting stuff inside" << std::endl;
	if (!boundary.contains(d.pos))
	{
		//std::cout << "outside boundary" << std::endl;
		return false;
	}
	//std::cout << "inside boundary" << std::endl;
	if (objects.size() < CAPACITY)
	{
		//counter++;
		objects.push_back(d);
		return true;
	}

	if (nw == nullptr)
	{
		subdivide();
	} 

	if (nw->insert(d))
	{
		position = "nw";
		return true;
	}
	if (ne->insert(d))
	{
		position = "ne";
		return true;
	}
	if (sw->insert(d))
	{
		position = "sw";
		return true;
	}
	if (se->insert(d))
	{
		position = "se";
		return true;
	}

	return false;
}

template<typename T>
std::vector<Data<T>> Quadtree<T>::queryRange(QuadRegion range)
{
	std::vector<Data<T>> pInRange = std::vector<Data<T>>();

	if (!boundary.intersects(range))
	{
		return pInRange;
	}

	for (auto&& object : objects)
	{
		if (range.contains(object.pos))
		{
			pInRange.push_back(object);
		}
	}

	if (nw == nullptr)
	{
		return pInRange;
	}

	std::vector<Data<T>> temp = nw->queryRange(range);
	pInRange.insert(pInRange.end(), temp.begin(), temp.end());

	temp = ne->queryRange(range);
	pInRange.insert(pInRange.end(), temp.begin(), temp.end());

	temp = sw->queryRange(range);
	pInRange.insert(pInRange.end(), temp.begin(), temp.end());

	temp = se->queryRange(range);
	pInRange.insert(pInRange.end(), temp.begin(), temp.end());

	return pInRange;
}

template<typename T>
std::vector<Data<T>> Quadtree<T>::queryRange(QuadDifference range)
{
	std::vector<Data<T>> pInRange = std::vector<Data<T>>();

	if (!boundary.intersects(range))
	{
		return pInRange;
	}

	for (auto&& object : objects)
	{
		if (range.contains(object.pos))
		{
			pInRange.push_back(object);
		}
	}

	if (nw == nullptr)
	{
		return pInRange;
	}

	std::vector<Data<T>> temp = nw->queryRange(range);
	pInRange.insert(pInRange.end(), temp.begin(), temp.end());

	temp = ne->queryRange(range);
	pInRange.insert(pInRange.end(), temp.begin(), temp.end());

	temp = sw->queryRange(range);
	pInRange.insert(pInRange.end(), temp.begin(), temp.end());

	temp = se->queryRange(range);
	pInRange.insert(pInRange.end(), temp.begin(), temp.end());

	return pInRange;
}

template<typename T>
std::vector<T*> Quadtree<T>::queryRangeObjects(QuadRegion range) {
	std::vector<T*> pInRange = std::vector<T*>();

	if (!boundary.intersects(range))
	{
		return pInRange;
	}

	for (auto&& object : objects)
	{
		if (range.contains(object.pos))
		{
			pInRange.push_back(object.load);
		}
	}

	if (nw == nullptr)
	{
		return pInRange;
	}

	std::vector<T*> temp = nw->queryRangeObjects(range);
	pInRange.insert(pInRange.end(), temp.begin(), temp.end());

	temp = ne->queryRangeObjects(range);
	pInRange.insert(pInRange.end(), temp.begin(), temp.end());

	temp = sw->queryRangeObjects(range);
	pInRange.insert(pInRange.end(), temp.begin(), temp.end());

	temp = se->queryRangeObjects(range);
	pInRange.insert(pInRange.end(), temp.begin(), temp.end());

	return pInRange;
}

template<typename T>
std::vector<Data<T>> Quadtree<T>::queryRange(QuadDifference range) {
	std::vector<T*> pInRange = std::vector<T*>();

	if (!boundary.intersects(range))
	{
		return pInRange;
	}

	for (auto&& object : objects)
	{
		if (range.contains(object.pos))
		{
			pInRange.push_back(object.load);
		}
	}

	if (nw == nullptr)
	{
		return pInRange;
	}

	std::vector<T*> temp = nw->queryRangeObjects(range);
	pInRange.insert(pInRange.end(), temp.begin(), temp.end());

	temp = ne->queryRangeObjects(range);
	pInRange.insert(pInRange.end(), temp.begin(), temp.end());

	temp = sw->queryRangeObjects(range);
	pInRange.insert(pInRange.end(), temp.begin(), temp.end());

	temp = se->queryRangeObjects(range);
	pInRange.insert(pInRange.end(), temp.begin(), temp.end());

	return pInRange;
}

template<typename T>
void Quadtree<T>::getTree(std::string prechain, int level) {
	//std::cout << "in getTree()" << std::endl;
	if (!this) {
		//std::cout << "root is null";
		return;
	}
	
	for (auto i : this->objects)
		std::cout << i->getPoint().x << ' ' << i->getPoint().y << std::endl;
	//std::cout << (this->position).c_str() << "  ";

	std::cout << prechain << "Sublevel " << ++level << " NW Tree:\n";
	nw->getTree(prechain + "Sublevel " + std::to_string(level) + " NW Tree", level);
	std::cout << prechain << "Sublevel " << level << " NE Tree:\n";
	ne->getTree(prechain + "Sublevel " + std::to_string(level) + " NE Tree", level);
	std::cout << prechain << "Sublevel " << level << " SW Tree:\n";
	sw->getTree(prechain + "Sublevel " + std::to_string(level) + " SW Tree", level);
	std::cout << prechain << "Sublevel " << level << " SE Tree:\n";
	se->getTree(prechain + "Sublevel " + std::to_string(level) + " SE Tree", level);
}

template<typename T>
void Quadtree<T>::printTreeBoundaries(int level) {
	//std::cout << "in getTree()" << std::endl;
	if (!this) {
		//std::cout << "root is null";
		return;
	}

	std::cout << "Tree Boundary: Center: " + boundary.centre.toString() + "Half-Size: " + boundary.halfSize.toString() + "\n";
	if (nw == nullptr) {
		return;
	}
	int l = 0;
	std::cin >> l;
	std::cout << ++level << " NW ";
	nw->printTreeBoundaries(level); 
	std::cout << level << " NE ";
	ne->printTreeBoundaries(level);
	std::cout << level << " SW ";
	sw->printTreeBoundaries(level);
	std::cout << level << " SE ";
	se->printTreeBoundaries(level);
}

