#ifndef MAP_H 
#define MAP_H
#include <memory>
#include <iostream>
#include <fstream>
#include <iterator>
#include <sstream>

/* TODO:
-NO DUPLICATE KEYS - (DONE)

-RED-BLACK TREE STRUCTURE - (DONE)

swap function - (DONE)
merge function - (DONE)
erase function - (DONE)
clear function - (DONE)
next and previous - (DONE)
contains function - (DONE)
copy / move constructors - (DONE)
default constructor - (DONE)
assignment / move operators - (DONE)
adding container elements - (DONE)
searching for an element - (DONE)
sorting content - already sorted because of binary tree structure -> add MIN and MAX functions - (DONE)
serialization:
	-basic files - (DONE)
	-binary files - (DONE)
implementation of an iterator - no.
*/

//the node of a tree
template <class KEY, class VALUE>
class map_value
{
public:

	std::shared_ptr<map_value<KEY, VALUE>> left;
	std::shared_ptr<map_value<KEY, VALUE>> right;
	std::shared_ptr<map_value<KEY, VALUE>> parent;

	std::pair<KEY, VALUE> x;

	bool red;

	map_value()
	{
		left = nullptr;
		right = nullptr;
		red = false;
		parent = nullptr;
	}
	map_value(map_value& t)
	{
		left = t.left;
		right = t.right;
		red = t.red;
		parent = t.parent;
	}
	~map_value()
	{
		//memory leak checking
		//std::cout << "NODE: " << x.first << " " << x.second << " HAS BEEN ERASED\n";
	}
};

//for binary file saving
template <class KEY, class VALUE>
class map_value_save
{
public:

	KEY left;
	bool hasleft;
	KEY right;
	bool hasright;
	KEY parent;
	bool hasparent;

	std::pair<KEY, VALUE> x;

	bool red;

	map_value_save()
	{
	}
};

//holds the pointer to the top of the tree as well as the size and all map functionality
template <class KEY,class VALUE> 
class map
{
	//Data
	std::shared_ptr<map_value<KEY,VALUE>> top;	
	size_t size;
	
	//Different searching methods
	std::shared_ptr<map_value<KEY, VALUE>> findKey(KEY& key, std::shared_ptr<map_value<KEY, VALUE>> x);
	std::shared_ptr<map_value<KEY, VALUE>> getLeaf(KEY& key, std::shared_ptr<map_value<KEY, VALUE>> x);
	std::shared_ptr<map_value<KEY, VALUE>> getnext(std::shared_ptr<map_value<KEY, VALUE>> x);
	std::shared_ptr<map_value<KEY, VALUE>> getprevious(std::shared_ptr<map_value<KEY, VALUE>> x);

	//Red-Black tree properties
	void redblackcheck(std::shared_ptr<map_value<KEY, VALUE>> node);
	void leftrotate(std::shared_ptr<map_value<KEY, VALUE>> node);
	void rightrotate(std::shared_ptr<map_value<KEY, VALUE>> node);
	void recolor(std::shared_ptr<map_value<KEY, VALUE>> node);

	//Debug
	void debugDrawDeeper(std::shared_ptr<map_value<KEY, VALUE>> b, int wantheight, int currentheight);

	//Serializationa
	void WriteDeeper(std::ofstream& x, std::shared_ptr<map_value<KEY, VALUE>> node);
	void BinWriteDeeper(std::ofstream& x, std::shared_ptr<map_value<KEY, VALUE>> node);

	//Erasing
	std::shared_ptr<map_value<KEY, VALUE>> erasenode(std::shared_ptr<map_value<KEY, VALUE>> node);
public:
	map()
	{
		size = 0;
		top = nullptr;
	}
	map(map& x)
	{
		std::pair<KEY, VALUE>* l = x.toDynArr();
		for (int i = 0; i < x.Size(); i++)
		{
			insert((l + i)->first, (l + i)->second);
		}
		size = x.size;
	}
	map(map&& x) noexcept
	{
		std::pair<KEY, VALUE>* l = x.toDynArr();
		for (int i = 0; i < x.Size(); i++)
		{
			insert((l + i)->first, (l + i)->second);
		}
		size = x.size;
		x.clear();
	}
	~map()
	{
		clear();
		//memory leak checking
		//std::cout << "MAP HAS BEEN ERASED\n";
	}
	//Debug
	void debugDraw(int deepness);
	//Size
	bool empty();
	size_t Size();
	//Basic operations
	void clear();
	void insert(KEY x, VALUE y);
	void erase(KEY x);
	VALUE& get(KEY x);
	map<KEY, VALUE>& operator=(const map<KEY,VALUE>& other);
	map<KEY, VALUE>& operator=(const map<KEY, VALUE>&& other);
	VALUE& operator[](const KEY& k);
	bool contains(KEY x);
	//Tree navigation
	KEY min();
	KEY max();
	KEY next(KEY x);
	KEY previous(KEY x);
	void swap(map<KEY, VALUE>& x);
	void merge(map<KEY, VALUE>& x);
	std::shared_ptr<map_value<KEY, VALUE>> getTop();
	//Iterating

	//Read Only
	std::pair<KEY, VALUE>* toDynArr();

	//Serialization
	void Write(std::ofstream& x);
	void Read(std::ifstream& x);

	void BinWrite(std::ofstream& x);
	void BinRead(std::ifstream& x);
};

#endif