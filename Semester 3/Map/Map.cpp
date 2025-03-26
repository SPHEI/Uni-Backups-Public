#include "Map.h"

template <class KEY, class VALUE>
std::shared_ptr<map_value<KEY, VALUE>> map<KEY, VALUE>::findKey(KEY& key, std::shared_ptr<map_value<KEY, VALUE>> x)
{
	if (!x)
	{
		return nullptr;
	}
	if (key == x->x.first)
	{
		return x;
	}
	std::shared_ptr<map_value<KEY, VALUE>> result;
	if (key < x->x.first)
	{
		result = findKey(key, x->left);
		if (result)
		{
			return result;
		}
	}
	else
	{
		result = findKey(key, x->right);
		if (result)
		{
			return result;
		}
	}
	return nullptr;
}
template <class KEY, class VALUE>
std::shared_ptr<map_value<KEY, VALUE>> map<KEY, VALUE>::getLeaf(KEY& key, std::shared_ptr<map_value<KEY, VALUE>> x)
{
	if (key < x->x.first)
	{
		if (!x->left)
		{
			return x;
		}
		return getLeaf(key, x->left);
	}
	else
	{
		if (!x->right)
		{
			return x;
		}
		return getLeaf(key, x->right);
	}
}
template <class KEY, class VALUE>
std::shared_ptr<map_value<KEY, VALUE>> map<KEY, VALUE>::getnext(std::shared_ptr<map_value<KEY, VALUE>> x)
{
	if (!x)
	{
		return nullptr;
	}
	std::shared_ptr<map_value<KEY, VALUE>> start;
	start = x;
	while (true)
	{
		if (start->right && start == x)
		{
			x = x->right;
			while (true)
			{
				if (!x->left)
				{
					return x;
				}
				else
				{
					x = x->left;
				}
			}
		}
		else if (start->parent)
		{
			if (start->parent->left)
			{
				if (start->parent->left == start)
				{
					return start->parent;
				}
			}
			start = start->parent;
		}
		else
		{
			return x;
		}
	}

}
template <class KEY, class VALUE>
std::shared_ptr<map_value<KEY, VALUE>> map<KEY, VALUE>::getprevious(std::shared_ptr<map_value<KEY, VALUE>> x)
{
	if (!x)
	{
		return nullptr;
	}
	std::shared_ptr<map_value<KEY, VALUE>> start;
	start = x;
	while (true)
	{
		if (start->left && start == x)
		{
			x = x->left;
			while (true)
			{
				if (!x->right)
				{
					return x;
				}
				else
				{
					x = x->right;
				}
			}
		}
		else if (start->parent)
		{
			if (start->parent->right)
			{
				if (start->parent->right == start)
				{
					return start->parent;
				}
			}
			start = start->parent;
		}
		else
		{
			return x;
		}
	}

}
template <class KEY, class VALUE>
void map<KEY,VALUE>::debugDraw(int deepness)
{
	for (int i = 0; i < deepness; i++)
	{
		for (int j = 0; j < deepness - i; j++)
		{
			std::cout << "\t";
		}
		debugDrawDeeper(top, i, 0);
		std::cout << "\n";
	}
}
template <class KEY, class VALUE>
void  map<KEY, VALUE>::debugDrawDeeper(std::shared_ptr<map_value<KEY, VALUE>> b, int wantheight, int currentheight)
{
	if (!b)
	{
		std::cout << "-\t";
		return;
	}
	if (wantheight == currentheight)
	{
		std::cout << b->x.first << "(" << b->red << ")\t";
		return;
	}
	else
	{
		debugDrawDeeper(b->left, wantheight, currentheight + 1);
		debugDrawDeeper(b->right, wantheight, currentheight + 1);
	}
}
template <class KEY, class VALUE>
bool map<KEY, VALUE>::empty()
{
	if (size == 0)
	{
		return true;
	}
	return false;
}
template <class KEY, class VALUE>
size_t map<KEY, VALUE>::Size()
{
	return size;
}
template <class KEY, class VALUE>
void map<KEY, VALUE>::clear()
{
	do
	{
		erasenode(top);
	} while (size > 0);
	top = nullptr;
}
template <class KEY, class VALUE>
void map<KEY, VALUE>::insert(KEY x, VALUE y)
{
	if (size == 0)
	{
		top.reset(new map_value<KEY, VALUE>);
		top->x = std::make_pair(x, y);
		top->red = false;
	}
	else
	{
		std::shared_ptr<map_value<KEY, VALUE>> temp;
		if (findKey(x, top))
		{
			temp = findKey(x, top);
			temp->x = std::make_pair(x, y);
		}
		else
		{
			temp = getLeaf(x, top);
			if (x < temp->x.first)
			{
				temp->left.reset(new map_value<KEY, VALUE>);
				temp->left->x = std::make_pair(x, y);
				temp->left->parent = temp;
				temp->left->red = true;
				redblackcheck(temp->left);
			}
			else
			{
				temp->right.reset(new map_value<KEY, VALUE>);
				temp->right->x = std::make_pair(x, y);
				temp->right->parent = temp;
				temp->right->red = true;
				redblackcheck(temp->right);
			}
		}
	}
	size++;
}
template <class KEY, class VALUE>
void map<KEY, VALUE>::erase(KEY x)
{
	std::shared_ptr<map_value<KEY, VALUE>> a = findKey(x, top);
	std::shared_ptr<map_value<KEY, VALUE>> b;
	if (a)
	{
		b = erasenode(a);
	}
	if (b)
	{
		redblackcheck(b);
		return;
	}
	b = nullptr;
	return;
}
template <class KEY, class VALUE>
VALUE& map<KEY, VALUE>::get(KEY x)
{
	std::shared_ptr<map_value<KEY, VALUE>> z = findKey(x, top);
	if (!z)
	{
		insert(x,*(new VALUE));
	}
	z = findKey(x, top);
	return z->x.second;
}
template <class KEY, class VALUE>
map<KEY, VALUE>& map<KEY, VALUE>::operator=(const map<KEY, VALUE>& other)
{
	if (size > 0)
	{
		clear();
	}
	size = other.size;
	size_t inserted = 0;
	std::shared_ptr < map_value<KEY, VALUE>> node = other.top;
	//0 = came from above
	//1 = came from left
	//2 = came from right
	int wherewent = 0;

	top.reset(new map_value<KEY, VALUE>);
	std::shared_ptr < map_value<KEY, VALUE>> current = top;
	current->parent = nullptr;
	while (inserted < size)
	{
		switch (wherewent)
		{
		case 0:
			current->x = node->x;
			current->red = node->red;
			inserted++;
			if (node->left)
			{
				current->left.reset(new map_value<KEY, VALUE>);
				current->left->parent = current;
				current = current->left;
				node = node->left;
				wherewent = 0;
				break;
			}
			else if(node->right)
			{
				current->right.reset(new map_value<KEY, VALUE>);
				current->right->parent = current;
				current = current->right;
				node = node->right;
				wherewent = 0;
				break;
			}
			else
			{
				if (current->parent->left == current)
				{
					wherewent = 1;
					current = current->parent;
					node = node->parent;
					break;
				}
				else
				{
					wherewent = 2;
					current = current->parent;
					node = node->parent;
					break;
				}
			}
			break;
		case 1:
			if (node->right)
			{
				current->right.reset(new map_value<KEY, VALUE>);
				current->right->parent = current;
				current = current->right;
				node = node->right;
				wherewent = 0;
				break;
			}
			else
			{
				if (current->parent->left == current)
				{
					wherewent = 1;
					current = current->parent;
					node = node->parent;
					break;
				}
				else
				{
					wherewent = 2;
					current = current->parent;
					node = node->parent;
					break;
				}
			}
			break;
		case 2:
			if (current->parent->left == current)
			{
				wherewent = 1;
				current = current->parent;
				node = node->parent;
				break;
			}
			else
			{
				wherewent = 2;
				current = current->parent;
				node = node->parent;
				break;
			}
			break;
		}
	}
	return *this;
}
template <class KEY, class VALUE>
map<KEY, VALUE>& map<KEY, VALUE>::operator=(const map<KEY, VALUE>&& other) 
{
	if (size > 0)
	{
		clear();
	}
	size = other.size;
	size_t inserted = 0;
	std::shared_ptr < map_value<KEY, VALUE>> node = other.top;
	//0 = came from above
	//1 = came from left
	//2 = came from right
	int wherewent = 0;

	top.reset(new map_value<KEY, VALUE>);
	std::shared_ptr < map_value<KEY, VALUE>> current = top;
	current->parent = nullptr;
	while (inserted < size)
	{
		switch (wherewent)
		{
		case 0:
			current->x = node->x;
			current->red = node->red;
			inserted++;
			if (node->left)
			{
				current->left.reset(new map_value<KEY, VALUE>);
				current->left->parent = current;
				current = current->left;
				node = node->left;
				wherewent = 0;
				break;
			}
			else if (node->right)
			{
				current->right.reset(new map_value<KEY, VALUE>);
				current->right->parent = current;
				current = current->right;
				node = node->right;
				wherewent = 0;
				break;
			}
			else
			{
				if (current->parent->left == current)
				{
					wherewent = 1;
					current = current->parent;
					node = node->parent;
					break;
				}
				else
				{
					wherewent = 2;
					current = current->parent;
					node = node->parent;
					break;
				}
			}
			break;
		case 1:
			if (node->right)
			{
				current->right.reset(new map_value<KEY, VALUE>);
				current->right->parent = current;
				current = current->right;
				node = node->right;
				wherewent = 0;
				break;
			}
			else
			{
				if (current->parent->left == current)
				{
					wherewent = 1;
					current = current->parent;
					node = node->parent;
					break;
				}
				else
				{
					wherewent = 2;
					current = current->parent;
					node = node->parent;
					break;
				}
			}
			break;
		case 2:
			if (current->parent->left == current)
			{
				wherewent = 1;
				current = current->parent;
				node = node->parent;
				break;
			}
			else
			{
				wherewent = 2;
				current = current->parent;
				node = node->parent;
				break;
			}
			break;
		}
	}
	other.~map();
	return *this;
}
template <class KEY, class VALUE>
VALUE& map<KEY, VALUE>::operator[](const KEY& k)
{
	KEY x = k;
	std::shared_ptr<map_value<KEY, VALUE>> z = findKey(x, top);
	if (!z)
	{
		insert(x,*(new VALUE));
	}
	z = findKey(x, top);
	return z->x.second;
}
template <class KEY, class VALUE>
bool map<KEY, VALUE>::contains(KEY x)
{
	if (findKey(x, top))
	{
		return true;
	}
	return false;
}
template <class KEY, class VALUE>
KEY map<KEY, VALUE>::min()
{
	std::shared_ptr<map_value<KEY, VALUE>> x = top;

	while (true)
	{
		if (!x->left)
		{
			return x->x.first;
		}
		else
		{
			x = x->left;
		}
	}
}
template <class KEY, class VALUE>
KEY map<KEY, VALUE>::max()
{
	std::shared_ptr<map_value<KEY, VALUE>> x = top;

	while (true)
	{
		if (!x->right)
		{
			return x->x.first;
		}
		else
		{
			x = x->right;
		}
	}
}
template <class KEY, class VALUE>
void map<KEY, VALUE>::redblackcheck(std::shared_ptr<map_value<KEY, VALUE>> node)
{
	std::shared_ptr<map_value<KEY, VALUE>> parent = node->parent;
	std::shared_ptr<map_value<KEY, VALUE>> grandpa = parent->parent;
	if (!parent || !grandpa)
	{
		return;
	}
	std::shared_ptr<map_value<KEY, VALUE>> uncle;
	bool uncleright = false;
	if (parent == grandpa->left)
	{
		uncleright = true;
		uncle = grandpa->right;
	}
	else
	{
		uncle = grandpa->left;
	}

	if (uncle)
	{
		if (node->red && parent->red && uncle->red)
		{
			//case 1 or 4
			recolor(node);
			if (grandpa->parent && grandpa->red)
			{
				if (grandpa->parent->red)
				{
					redblackcheck(grandpa);
				}
			}
			return;
		}
		if (uncleright)
		{
			if (node == parent->right && node->red && parent->red && !uncle->red)
			{
				//case 2 with uncle
				leftrotate(node);
				rightrotate(node);
				recolor(node->left);
				return;
			}
			if (node == parent->left && node->red && parent->red && !uncle->red)
			{
				//case 3
				rightrotate(parent);
				recolor(node);
				return;
			}
		}
		else
		{
			if (node == parent->left && node->red && parent->red && !uncle->red)
			{
				//case 5
				rightrotate(node);
				leftrotate(node);
				recolor(node->right);
				return;
			}
			if (node == parent->right && node->red && parent->red && !uncle->red)
			{
				//case 6
				leftrotate(parent);
				recolor(node);
				return;
			}
		}
	}
	else
	{
		//no uncle edition
		if (uncleright)
		{
			if (node == parent->right && node->red && parent->red)
			{
				//case 2
				leftrotate(node);
				rightrotate(node);
				recolor(node->left);
				return;
			}
			if (node == parent->left && node->red && parent->red)
			{
				//case 3
				rightrotate(parent);
				recolor(node);
				return;
			}
		}
		else
		{
			if (node == parent->left && node->red && parent->red)
			{
				//case 5
				rightrotate(node);
				leftrotate(node);
				recolor(node->right);
				return;
			}
			if (node == parent->right && node->red && parent->red)
			{
				//case 6
				leftrotate(parent);
				recolor(node);
				return;
			}
		}
	}
	return;
}
template <class KEY, class VALUE>
void map<KEY, VALUE>::leftrotate(std::shared_ptr<map_value<KEY, VALUE>> node)
{
	std::shared_ptr<map_value<KEY, VALUE>> temp = node->left;
	std::shared_ptr<map_value<KEY, VALUE>> parent = node->parent;
	if (!parent)
	{
		return;
	}
	node->parent = parent->parent;
	node->left = parent;
	parent->parent = node;
	parent->right = temp;
	if (temp)
	{
		temp->parent = parent;
	}
	if (!node->parent)
	{
		top = node;
	}
	else
	{
		if (node->parent->left == parent)
		{
			node->parent->left = node;
		}
		else
		{
			node->parent->right = node;
		}
	}
	return;
}
template <class KEY, class VALUE>
void map<KEY, VALUE>::rightrotate(std::shared_ptr<map_value<KEY, VALUE>> node)
{
	std::shared_ptr<map_value<KEY, VALUE>> temp = node->right;
	std::shared_ptr<map_value<KEY, VALUE>> parent = node->parent;
	if (!parent)
	{
		return;
	}

	node->parent = parent->parent;
	node->right = parent;
	parent->parent = node;
	parent->left = temp;
	if (temp)
	{
		temp->parent = parent;
	}

	if (!node->parent)
	{
		top = node;
	}
	else
	{
		if (node->parent->left == parent)
		{
			node->parent->left = node;
		}
		else
		{
			node->parent->right = node;
		}
	}
	return;
}
template <class KEY, class VALUE>
void map<KEY, VALUE>::recolor(std::shared_ptr<map_value<KEY, VALUE>> node)
{
	std::shared_ptr<map_value<KEY, VALUE>> parent = node->parent;
	std::shared_ptr<map_value<KEY, VALUE>> grandpa = parent->parent;
	std::shared_ptr<map_value<KEY, VALUE>> uncle;
	std::shared_ptr<map_value<KEY, VALUE>> sibling;
	if (parent)
	{
		if (node == parent->left)
		{
			sibling = parent->right;
		}
		else
		{
			sibling = parent->left;
		}
	}
	if (grandpa)
	{
		if (parent == grandpa->left)
		{
			uncle = grandpa->right;
		}
		else
		{
			uncle = grandpa->left;
		}
	}
	if (sibling)
	{
		sibling->red = true;
	}
	if (parent)
	{
		parent->red = false;
	}
	if (uncle)
	{
		uncle->red = false;
	}
	if (grandpa)
	{
		if (grandpa == top)
		{
			grandpa->red = false;
		}
		else
		{
			grandpa->red = true;
		}
	}
	return;
}
template <class KEY, class VALUE>
std::shared_ptr<map_value<KEY, VALUE>> map<KEY, VALUE>::erasenode(std::shared_ptr<map_value<KEY, VALUE>> node)
{
	if (!node)
	{
		return nullptr;
	}
	bool hasleft = false;
	bool hasright = false;
	int children = 0;
	std::shared_ptr<map_value<KEY, VALUE>> result;
	if (node->left)
	{
		hasleft = true;
		children++;
	}
	if (node->right)
	{
		hasright = true;
		children++;
	}
	switch (children)
	{
	case 0:
		if (!node->parent)
		{
			top = nullptr;
			result = nullptr;
			break;
		}
		if (node->parent->left)
		{
			if (node->parent->left == node)
			{
				node->parent->left = nullptr;
			}
			else if (node->parent->right)
			{
				node->parent->right = nullptr;
			}
		}
		result = nullptr;
		break;
	case 1:
		if (hasleft)
		{
			node->left->parent = node->parent;
			if (node->parent)
			{
				if (node->parent->left)
				{
					if (node->parent->left == node)
					{
						node->parent->left = node->left;
						result = node->left;
					}
					else if (node->parent->right)
					{
						node->parent->right = node->left;
						result = node->left;
					}
				}
			}
			else
			{
				top = node->left;
				result = nullptr;
			}
		}
		if (hasright)
		{
			if (node->parent)
			{
				node->right->parent = node->parent;
				if (node->parent->left)
				{
					if (node->parent->left == node)
					{
						node->parent->left = node->right;
						result = node->right;
					}
					else if (node->parent->right)
					{
						node->parent->right = node->right;
						result = node->right;
					}
				}
			}
			else
			{
				top = node->right;
				result = nullptr;
			}
		}
		break;
	case 2:
		std::shared_ptr<map_value<KEY, VALUE>> nxt = getnext(node);
		if (!nxt)
		{
			break;
		}
		if (nxt->right)
		{
			nxt->right->parent = nxt->parent;
			if (nxt->parent->left == nxt)
			{
				nxt->parent->left = nxt->right;
			}
			else
			{
				nxt->parent->right = nxt->right;
			}
		}
		else
		{
			if (nxt->parent->left == nxt)
			{
				nxt->parent->left = nullptr;
			}
			else
			{
				nxt->parent->right = nullptr;
			}

		}

		if (node->right == nxt)
		{
			nxt->right = nullptr;
		}
		else
		{
			nxt->right = node->right;
		}
		nxt->left = node->left;
		nxt->parent = node->parent;
		if (nxt->right)
		{
			nxt->right->parent = nxt;
		}
		if (node->parent)
		{
			if (node->parent->left == node)
			{
				node->parent->left = nxt;
			}
			else
			{
				node->parent->right = nxt;
			}

		}
		else
		{
			top = nxt;
		}
		break;

	}
	node->left = nullptr;
	node->right = nullptr;
	node->parent = nullptr;
	size--;
	return result;
}
template <class KEY, class VALUE>
KEY map<KEY, VALUE>::next(KEY x)
{
	if (x == max())
	{
		return x;
	}
	std::shared_ptr<map_value<KEY, VALUE>> temp = findKey(x,top);
	if (!temp)
	{
		return x;
	}
	return (getnext(temp)->x.first);
}
template <class KEY, class VALUE>
KEY map<KEY, VALUE>::previous(KEY x)
{
	if (x == min())
	{
		return x;
	}
	std::shared_ptr<map_value<KEY, VALUE>> temp = findKey(x, top);
	if (!temp)
	{
		return x;
	}
	return (getprevious(temp)->x.first);
}
template <class KEY, class VALUE>
void map<KEY, VALUE>::swap(map<KEY, VALUE>& x)
{
	size_t temp = x.size;
	std::shared_ptr<map_value<KEY, VALUE>> temptop = x.top;
	x.top = top;
	x.size = size;
	size = temp;
	top = temptop;
}
template <class KEY, class VALUE>
void map<KEY, VALUE>::merge(map<KEY, VALUE>& x)
{
	std::pair<KEY, VALUE>* l = x.toDynArr();
	for (int i = 0; i < x.Size(); i++)
	{
		if (!findKey((l + i)->first, top))
		{
			insert((l + i)->first, (l + i)->second);
		}
	}
	x.clear();
}
template <class KEY, class VALUE>
std::shared_ptr<map_value<KEY, VALUE>> map<KEY, VALUE>::getTop()
{
	return top;
}
template <class KEY, class VALUE>
std::pair<KEY, VALUE>* map<KEY, VALUE>::toDynArr()
{
	if (size == 0)
	{
		return nullptr;
	}
	std::pair<KEY, VALUE>* start;
	start = new std::pair<KEY, VALUE>[size];
	KEY m = min();
	std::shared_ptr<map_value<KEY, VALUE>> current = findKey(m, top);
	for (int i = 0; i < size; i++)
	{
		*(start + i) = current->x;
		current = getnext(current);
	}
	return start;
}
template <class KEY, class VALUE>
void map<KEY, VALUE>::Write(std::ofstream& x)
{
	x << size << "\n";
	WriteDeeper(x, top);
}
template <class KEY, class VALUE>
void map<KEY, VALUE>::WriteDeeper(std::ofstream& x, std::shared_ptr<map_value<KEY, VALUE>> node)
{
	x << node->x.first << " " << node->x.second << " " << node->red << " ";
	if (node->left)
	{
		x << "L: " << node->left->x.first << " ";
	}
	if (node->right)
	{
		x << "R: " << node->right->x.first << " ";
	}
	if (node->parent)
	{
		x << "P: " << node->parent->x.first << " ";
	}
	x << "\n";
	if (node->left)
	{
		WriteDeeper(x, node->left);
	}
	if (node->right)
	{
		WriteDeeper(x, node->right);
	}
}
template <class KEY, class VALUE>
void map<KEY, VALUE>::Read(std::ifstream& f)
{
	f >> size;
	std::string o;
	std::getline(f, o);
	while (std::getline(f,o))
	{
		std::stringstream x(o);
		KEY k;
		x >> k;
		VALUE v;
		x >> v;
		bool r;
		x >> r;
		std::shared_ptr<map_value<KEY, VALUE>> node;

		if (top && findKey(k,top))
		{
			node = findKey(k,top);
		}
		else
		{
			node.reset(new map_value<KEY, VALUE>);
		}
		node->x = std::make_pair(k, v);
		node->red = r;

		std::string tester;
		x >> tester;
		if (tester == "L:")
		{
			node->left.reset(new map_value<KEY, VALUE>);
			KEY l;
			x >> l;
			node->left->x.first = l;
			x >> tester;
		}
		if (tester == "R:")
		{
			node->right.reset(new map_value<KEY, VALUE>);
			KEY r;
			x >> r;
			node->right->x.first = r;
			x >> tester;
		}
		if (tester == "P:")
		{
			KEY p;
			x >> p;
			node->parent = findKey(p,top);
		}
		else
		{
			top = node;
		}
	}
}
template <class KEY, class VALUE>
void map<KEY, VALUE>::BinWrite(std::ofstream& x)
{
	x.write(reinterpret_cast<char*>(&size), sizeof(size_t));
	BinWriteDeeper(x, top);
}
template <class KEY, class VALUE>
void map<KEY, VALUE>::BinWriteDeeper(std::ofstream& x, std::shared_ptr<map_value<KEY, VALUE>> node)
{
	map_value_save<KEY, VALUE> l;
	l.x = node->x;
	l.red = node->red;
	if (node->left)
	{
		l.hasleft = true;
		l.left = node->left->x.first;
	}
	else
	{
		l.hasleft = false;
	}

	if (node->right)
	{
		l.hasright = true;
		l.right = node->right->x.first;
	}
	else
	{
		l.hasright = false;
	}

	if (node->parent)
	{
		l.hasparent = true;
		l.parent = node->parent->x.first;
	}
	else
	{
		l.hasparent = false;
	}
	x.write(reinterpret_cast<char*>(&l), sizeof(map_value_save<KEY,VALUE>));
	if (node->left)
	{
		BinWriteDeeper(x, node->left);
	}
	if (node->right)
	{
		BinWriteDeeper(x, node->right);
	}
}
template <class KEY, class VALUE>
void map<KEY, VALUE>::BinRead(std::ifstream& f)
{
	f.read(reinterpret_cast<char*>(&size), sizeof(size_t));

	for(int i = 0; i < size; i++)
	{
		map_value_save<KEY, VALUE> x;
		f.read(reinterpret_cast<char*>(&x), sizeof(map_value_save<KEY, VALUE>));

		std::shared_ptr<map_value<KEY, VALUE>> node;

		if (top && findKey(x.x.first, top))
		{
			node = findKey(x.x.first, top);
		}
		else
		{
			node.reset(new map_value<KEY, VALUE>);
		}
		node->x = x.x;
		node->red = x.red;

		if (x.hasleft)
		{
			node->left.reset(new map_value<KEY, VALUE>);
			node->left->x.first = x.left;
		}
		if (x.hasright)
		{
			node->right.reset(new map_value<KEY, VALUE>);
			node->right->x.first = x.right;
		}
		if (x.hasparent)
		{
			node->parent = findKey(x.parent, top);
		}
		else
		{
			top = node;
		}
	}
}