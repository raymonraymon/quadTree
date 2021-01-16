//https://gamedevelopment.tutsplus.com/tutorials/quick-tip-use-quadtrees-to-detect-likely-collisions-in-2d-space--gamedev-374
//origin in java,modify it to c++11;
#include <iostream>

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
		   List objects;
		   Rectangle bounds;
		   Quadtree[] nodes;
public:	   Quadtree(int pLevel, Rectangle pBounds)
{
	level = pLevel;
	objects = new std:vector<Rectangle>;
	bounds = pBounds;
	nodes = new Quadtree[4];
}

public:	void clear()
{
	objects.clear();

	for (int i = 0; i < nodes.length; i++) {
		if (nodes[i] != null) {
			nodes[i].clear();
			nodes[i] = null;
		}
	}
}
private:  void split() {
	int subWidth = (int)(bounds.getWidth() / 2);
	int subHeight = (int)(bounds.getHeight() / 2);
	int x = (int)bounds.getX();
	int y = (int)bounds.getY();

	nodes[0] = new Quadtree(level + 1, new Rectangle(x + subWidth, y, subWidth, subHeight));
	nodes[1] = new Quadtree(level + 1, new Rectangle(x, y, subWidth, subHeight));
	nodes[2] = new Quadtree(level + 1, new Rectangle(x, y + subHeight, subWidth, subHeight));
	nodes[3] = new Quadtree(level + 1, new Rectangle(x + subWidth, y + subHeight, subWidth, subHeight));
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
			 if (nodes[0] != null) {
				 int index = getIndex(pRect);

				 if (index != -1) {
					 nodes[index].insert(pRect);

					 return;
				 }
			 }

			 objects.add(pRect);

			 if (objects.size() > MAX_OBJECTS && level < MAX_LEVELS) {
				 if (nodes[0] == null) {
					 split();
				 }

				 int i = 0;
				 while (i < objects.size()) {
					 int index = getIndex(objects.get(i));
					 if (index != -1) {
						 nodes[index].insert(objects.remove(i));
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
		public List retrieve(List returnObjects, Rectangle pRect) {
			int index = getIndex(pRect);
			if (index != -1 && nodes[0] != null) {
				nodes[index].retrieve(returnObjects, pRect);
			}

			returnObjects.addAll(objects);

			return returnObjects;
		}
};
void main()
{
	Quadtree quad = new Quadtree(0, new Rectangle(0, 0, 600, 600));
	quad.clear();
	for (int i = 0; i < allObjects.size(); i++) {
		quad.insert(allObjects.get(i));
	}
	List returnObjects = new ArrayList();
	for (int i = 0; i < allObjects.size(); i++) {
		returnObjects.clear();
		quad.retrieve(returnObjects, objects.get(i));

		for (int x = 0; x < returnObjects.size(); x++) {
			// Run collision detection algorithm between objects
		}
	}
	return;
}
