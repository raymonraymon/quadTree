//https://gamedevelopment.tutsplus.com/tutorials/quick-tip-use-quadtrees-to-detect-likely-collisions-in-2d-space--gamedev-374
//origin in java,modify it to c++11;
#include <iostream>
#include <vector>
using namespace std;
class Rectangle
{

private: float X;
		 float Y;
		 float width;
		 float height;
public:	 float getX() { return X;};
		 float getY() { return Y; };
		 float getWidth() { return width; };
		 float getHeight() { return height; };
public:	  Rectangle()
{
	X = 0.;
	Y = 0.;
	width = 0.;
	height = 0.;
}
public:	  Rectangle(float x,float y,float w,float h)
{
	X = x;
	Y = y;
	width = w;
	height = h;
}
};

class Quadtree
{
private: int MAX_OBJECTS = 10;
		 int MAX_LEVELS = 5;
		 
private:   int level;
		   vector<Rectangle> objects;
		   Rectangle bounds;
		   Quadtree* nodes[4];

public:	   Quadtree()
{
	level = 0;
	//vector<Rectangle> _objects;
	//_objects.reserve(10);
	//objects = _objects;
	bounds = Rectangle(0.,0.,0.,0.);
	//for (int i = 0; i < 4; i++) {
	//	nodes[i] = 0;
	//}
}
public:	   Quadtree(int pLevel, Rectangle pBounds)
{
	level = pLevel;
	//vector<Rectangle> _objects;
	//_objects.reserve(10);
	//objects = _objects;
	bounds = pBounds;
	//for (int i = 0; i < 4; i++) {
	//	nodes[i] = new Quadtree();
	//}
}

public:	void clear()
{
	objects.clear();

	for (int i = 0; i < 4; i++) {
		if (nodes[i] != 0) {
			nodes[i]->clear();
			nodes[i] = 0;
		}
	}
}
private:  void split() {
	int subWidth = (int)(bounds.getWidth() / 2);
	int subHeight = (int)(bounds.getHeight() / 2);
	int x = (int)bounds.getX();
	int y = (int)bounds.getY();

	nodes[0] = new Quadtree(level + 1, Rectangle(x + subWidth, y, subWidth, subHeight));
	nodes[1] = new Quadtree(level + 1, Rectangle(x, y, subWidth, subHeight));
	nodes[2] = new Quadtree(level + 1, Rectangle(x, y + subHeight, subWidth, subHeight));
	nodes[3] = new Quadtree(level + 1, Rectangle(x + subWidth, y + subHeight, subWidth, subHeight));
}

/*
 * Determine which node the object belongs to. -1 means
 * object cannot completely fit within a child node and is part
 * of the parent node
 */
private: int getIndex(Rectangle pRect) {
			  int index = -1;
			  double verticalMidpoint = bounds.getX() + (bounds.getWidth() / 2);
			  double horizontalMidpoint = bounds.getY() + (bounds.getHeight() / 2);

			  // Object can completely fit within the top quadrants
			  bool topQuadrant = (pRect.getY() < horizontalMidpoint && pRect.getY() + pRect.getHeight() < horizontalMidpoint);
			  // Object can completely fit within the bottom quadrants
			  bool bottomQuadrant = (pRect.getY() > horizontalMidpoint);

			  // Object can completely fit within the left quadrants
			  if (pRect.getX() < verticalMidpoint && pRect.getX() + pRect.getWidth() < verticalMidpoint) {
				  if (topQuadrant) {
					  index = 1;
				  }
				  else if (bottomQuadrant) {
					  index = 2;
				  }
			  }
			  // Object can completely fit within the right quadrants
			  else if (pRect.getX() > verticalMidpoint) {
				  if (topQuadrant) {
					  index = 0;
				  }
				  else if (bottomQuadrant) {
					  index = 3;
				  }
			  }
			  return index;
		  }
		 /*
 * Insert the object into the quadtree. If the node
 * exceeds the capacity, it will split and add all
 * objects to their corresponding nodes.
 */
public: void insert(Rectangle pRect) {
			 if (nodes[0] != 0) {
				 int index = getIndex(pRect);

				 if (index != -1) {
					 nodes[index]->insert(pRect);

					 return;
				 }
			 }

			 objects.push_back(pRect);

			 if (objects.size() > MAX_OBJECTS && level < MAX_LEVELS) {
				 if (nodes[0] == 0) {
					 split();
				 }

				 
				 int i = 0;
				 while (i< objects.size()) {
					 int index = getIndex(objects[i]);
					 
					 if (index != -1) {						 
						 nodes[index]->insert(objects[i]);								 
						 objects.erase(std::begin(objects) + i);
					 }
					 else {
						 i++;						 
					 }
				 }
			 }
		 }

/*
 * Return all objects that could collide with the given object
 */
public: vector<Rectangle> retrieve(vector<Rectangle>& returnObjects, Rectangle pRect) {
			int index = getIndex(pRect);
			if (index != -1 && nodes[0] != 0) {
				nodes[index]->retrieve(returnObjects, pRect);
			}
			for (size_t i=0;i<objects.size();i++)
			{
				returnObjects.push_back(objects[i]);
			}
			

			return returnObjects;
		}
};
int main()
{
	Rectangle bound = Rectangle(300., 300., 100., 100.);
	Quadtree quad = Quadtree(0, bound);
	quad.clear();
	vector<Rectangle> allObjects;
	for (int j = 10; j < 100; j = j + 20)
		for (int jj = 10; jj < 100; jj = jj + 20)
	{
			Rectangle b = Rectangle(j, jj, 2, 2);
			allObjects.push_back(b);
	}
	
	for (int i = 0; i < allObjects.size(); i++) {
		quad.insert(allObjects[i]);
	}
	vector<Rectangle> returnObjects;
	for (int i = 0; i < allObjects.size(); i++) {
		returnObjects.clear();
		quad.retrieve(returnObjects, allObjects[i]);

		for (int x = 0; x < returnObjects.size(); x++) {
			// Run collision detection algorithm between objects
		}
	}
	return 0;
}
