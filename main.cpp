//https://gamedevelopment.tutsplus.com/tutorials/quick-tip-use-quadtrees-to-detect-likely-collisions-in-2d-space--gamedev-374
//origin in java,modify it to c++11;
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include<algorithm>
using namespace std;
class Rectangle
{

private: float X;//leftDown Corner
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
	float subWidth = bounds.getWidth() / 2.;
	float subHeight = bounds.getHeight() / 2.;
	float x = bounds.getX();
	float y = bounds.getY();

	nodes[0] = new Quadtree(level + 1, Rectangle(x + subWidth, y + subHeight, subWidth, subHeight));
	nodes[1] = new Quadtree(level + 1, Rectangle(x, y + subHeight, subWidth, subHeight));
	nodes[2] = new Quadtree(level + 1, Rectangle(x, y, subWidth, subHeight));
	nodes[3] = new Quadtree(level + 1, Rectangle(x + subWidth , y, subWidth, subHeight));
}

/*
 * Determine which node the object belongs to. -1 means
 * object cannot completely fit within a child node and is part
 * of the parent node
 */
private: vector<int> getIndex(Rectangle pRect) {
			  vector<int> indexes;
				
			  double verticalMidpoint = bounds.getX() + (bounds.getWidth() / 2);
			  double horizontalMidpoint = bounds.getY() + (bounds.getHeight() / 2);

			  bool startIsNorth = pRect.getY() > horizontalMidpoint;
			  bool startIsWest = pRect.getX() < verticalMidpoint;
			  bool endIsEast = pRect.getX() + pRect.getWidth() > verticalMidpoint;
			  bool endIsSouth = pRect.getY() + pRect.getHeight() < horizontalMidpoint;

			  
			  if (startIsNorth) {
				  if (startIsWest && !endIsEast)
				  {
					  indexes.push_back(1);//top-left quad
				  }
				  else if (!startIsWest && endIsEast)
				  {
					  indexes.push_back(0);//top-right quad
				  }	
				  else
				  {
					  indexes.push_back(0);//top in the middle
					  indexes.push_back(1);
				  }
			  }

			  if (endIsSouth) {
				  if (startIsWest && !endIsEast)
				  {
					  indexes.push_back(2);//bottom-left quad
				  }
				  else if (!startIsWest && endIsEast)
				  {
					  indexes.push_back(3);//bottom-right quad
				  }
				  else
				  {
					  indexes.push_back(2);//bottom in the middle
					  indexes.push_back(3);
				  }
			  }
			  if (!startIsNorth && startIsWest && !endIsSouth)
			  {
				  indexes.push_back(1);//left in the middle
				  indexes.push_back(2);
			  }
			  if (!startIsWest && !startIsNorth && !endIsSouth)
			  {
				  indexes.push_back(0);//right in the middle
				  indexes.push_back(3);
			  }
			  return indexes;
		  }
/*
 * Insert the object into the quadtree. If the node
 * exceeds the capacity, it will split and add all
 * objects to their corresponding nodes.
 */
public: void insert(Rectangle pRect) {
			 if (nodes[0] != 0) {
				 vector<int> index = getIndex(pRect);
				 for (int i =0;i<index.size();i++)
				 {
					 if (index[i] != -1) {
						 nodes[index[i]]->insert(pRect);
						 return;
					 }
				 }
				 
			 }

			 objects.push_back(pRect);

			 if (objects.size() > MAX_OBJECTS && level < MAX_LEVELS) {
				 if (nodes[0] == 0) {
					 split();
				 }

				 int i = 0;
				 while (objects.size()>0)
				 {
					 
					 vector<int> index = getIndex(objects[i]);
					 for (int j = 0; j < index.size(); j++)
					 {
						 if (index[j] != -1) {
							 nodes[index[j]]->insert(objects[i]);
							 objects.erase(std::begin(objects) + i);
							 if (index.size() == 0 || objects.size() == 0)
							 {
								 break;
							 }
						 }						 
					 }
					 
				 }
			 }
		 }

/*
 * Return all objects that could collide with the given object
 */
public: vector<Rectangle> retrieve(vector<Rectangle>& returnObjects, Rectangle pRect) {
			vector<int> index = getIndex(pRect);
			for (int i =0;i<index.size();i++)
			{
				if (index[i] != -1 && nodes[0] != 0) {
					nodes[index[i]]->retrieve(returnObjects, pRect);
				}
				for (size_t i = 0; i < objects.size(); i++)
				{
					returnObjects.push_back(objects[i]);
				}
			}
						
			return returnObjects;
		}
};
int main()
{
	Rectangle bound = Rectangle(0., 0., 100., 100.);
	Quadtree quad = Quadtree(0, bound);
	quad.clear();
	vector<Rectangle> allObjects;

	srand((int)time(0));

	
	for (int j = 10; j < 100; j = j + 2) {
		for (int jj = 10; jj < 100; jj = jj + 2)
		{			
			int m = rand() % 100;
			int n = rand() % 100;
			Rectangle b = Rectangle(m, n, 2, 2);
			//Rectangle b = Rectangle(j, jj, 2, 2);
			allObjects.push_back(b);
		}
	}

	for (int i = 0; i < allObjects.size(); i++) {
		quad.insert(allObjects[i]);
	}
	vector<Rectangle> returnObjects;

	vector<Rectangle> testObjects;
	Rectangle b = Rectangle(12.5, 12.5, 1, 1);
	testObjects.push_back(b);

	for (int j = 10; j < 100; j = j + 20){
		for (int jj = 10; jj < 100; jj = jj + 20)
		{
			Rectangle b = Rectangle(j, jj, 2, 2);
			testObjects.push_back(b);
		}
	}

	for (int i = 0; i < testObjects.size(); i++) {
		returnObjects.clear();
		quad.retrieve(returnObjects, testObjects[i]);
		vector<float> Vecdis;
		for (int x = 0; x < returnObjects.size(); x++) {
			// Run collision detection algorithm between objects
			float dis = sqrt((returnObjects[x].getX() - testObjects[i].getX())*(returnObjects[x].getX() - testObjects[i].getX()) + (returnObjects[x].getY() - testObjects[i].getY())*(returnObjects[x].getY() - testObjects[i].getY()));
			Vecdis.push_back(dis);
		}
		sort(Vecdis.begin(), Vecdis.end());
		cout << "max(" << i << "):" <<Vecdis.back() << endl;
		cout << "min("<<i << "):" << Vecdis[0] << endl;
		cout << endl;
	}
	system("pause");
	return 0;
}
