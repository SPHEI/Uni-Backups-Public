#include "Map.h"
#include "Map.cpp"
int main()
{
	//Testing basic functionality
	{
		std::cout << "inserting and getting test:\n";
		map<int, std::string> test;
		test.insert(2, "two");
		test.insert(3, "three");
		std::cout << test.get(2) << "\n";
		std::cout << test.get(3) << "\n";
		test[2] = "one";
		std::cout << test[2] << "\n";

		//outputs size and empty bool (0 if false, 1 if true)
		std::cout << "\nSize and empty function test:\n";
		std::cout << test.Size() << " ";
		std::cout << test.empty();

		//Outputs 1 if the map contains an object and 0 if it doesn't
		std::cout << "\nContains check:\n" << test.contains(2) << " " << test.contains(24);

		//Should output the minimum and maximum values
		std::cout << "\nMin Max check:\nMin: " << test.min() << "\nMax: " << test.max() << "\n";

		//This should make a copy of the tree
		std::cout << "\ncopy constructor test:\n\n";
		map<int, std::string> thiird(test);
		thiird.debugDraw(2);
		std::cout << "\noperator= copy assignment test:\n\n";
		map<int, std::string> secoond;
		secoond = test;
		secoond.debugDraw(2);

		//This should make a copy of the tree and then clear the original
		std::cout << "\nmove constructor test:\n\n";
		map<int, std::string> fourth(std::move(thiird));
		thiird.debugDraw(2);
		fourth.debugDraw(2);
		std::cout << "\noperator= move assignment test:\n\n";
		map<int, std::string> fifth;
		fifth = std::move(fourth);
		fourth.debugDraw(2);
		fifth.debugDraw(2);
	}
	//Testing whether the tree keeps its red-black properties when inserting new values
	{
		std::cout << "\nRed Black Structure Check:\n";
		map<int, float> redblacktest;
		redblacktest[26] = 1;
		redblacktest.debugDraw(4);
		redblacktest[46] = 1;
		redblacktest.debugDraw(4);
		redblacktest[90] = 1;
		redblacktest.debugDraw(4);
		redblacktest[9] = 1;
		redblacktest.debugDraw(4);
		redblacktest[52] = 1;
		redblacktest.debugDraw(4);
		redblacktest[21] = 1;
		redblacktest.debugDraw(4);
		redblacktest[15] = 1;
		redblacktest.debugDraw(4);
		redblacktest[17] = 1;
		redblacktest.debugDraw(4);
		redblacktest[20] = 1;
		redblacktest.debugDraw(4);
		redblacktest[60] = 1;
		redblacktest.debugDraw(4);

		//Writing and then reading a text file
		std::cout << "\nText File Save Check\n";
		std::cout << "\nText\n";
		std::ofstream file;
		file.open("redblacktest.txt");
		redblacktest.Write(file);
		file.close();
		std::cout << "FILE WRITTEN\n";

		std::ifstream rfile;
		rfile.open("redblacktest.txt");
		map<int, int> reader;
		reader.Read(rfile);
		std::cout << "FILE READ\n";
		reader.debugDraw(4);

		//Writing and then reading a binary file
		std::cout << "\nBinary\n";
		std::ofstream bfile;
		bfile.open("bint.txt", std::ios::binary);
		redblacktest.BinWrite(bfile);
		bfile.close();
		std::cout << "FILE WRITTEN\n";

		std::ifstream brfile;
		brfile.open("bint.txt", std::ios::binary);
		map<int, int> breader;
		breader.BinRead(brfile);
		std::cout << "FILE READ\n";
		breader.debugDraw(4);

		//Next should return the successor of the value
		//Previous should return the predecesor of the value
		std::cout << "\n Next And Previous Check\n";
		std::cout << "\n Nexts:\n";
		std::cout << redblacktest.next(9) << "\n";
		std::cout << redblacktest.next(15) << "\n";
		std::cout << redblacktest.next(17) << "\n";
		std::cout << redblacktest.next(20) << "\n";
		std::cout << redblacktest.next(21) << "\n";
		std::cout << redblacktest.next(26) << "\n";
		std::cout << redblacktest.next(46) << "\n";
		std::cout << redblacktest.next(52) << "\n";
		std::cout << redblacktest.next(60) << "\n";
		std::cout << redblacktest.next(90) << "\n";

		std::cout << "\n Previouses:\n";
		std::cout << redblacktest.previous(9) << "\n";
		std::cout << redblacktest.previous(15) << "\n";
		std::cout << redblacktest.previous(17) << "\n";
		std::cout << redblacktest.previous(20) << "\n";
		std::cout << redblacktest.previous(21) << "\n";
		std::cout << redblacktest.previous(26) << "\n";
		std::cout << redblacktest.previous(46) << "\n";
		std::cout << redblacktest.previous(52) << "\n";
		std::cout << redblacktest.previous(60) << "\n";
		std::cout << redblacktest.previous(90) << "\n";


		//Checking all 3 cases for erasing nodes of a tree (0, 1 and 2 children cases)
		std::cout << "\n ERASING CHECK:\n";
		redblacktest.erase(90);
		redblacktest.debugDraw(4);
		redblacktest.erase(17);
		redblacktest.debugDraw(4);
		redblacktest.erase(21);
		redblacktest.debugDraw(4);

		std::cout << "\n CLEARING CHECK:\n";
		redblacktest.clear();
		redblacktest.debugDraw(4);

	}

	//Swap should swap the values of the trees
	//Merge should add values from b to a and then clear b
	{
		std::cout << "\nSwap Check\n";
		map<int, int> a;
		map<int, int> b;
		a[1] = 4;
		a[2] = 6;
		a[3] = 9;
		b[4] = 1;
		b[5] = 6;
		b[6] = 215;
		std::cout << "A: \n";
		a.debugDraw(2);
		std::cout << "B: \n";
		b.debugDraw(2);
		a.swap(b);
		std::cout << "\n\n";
		std::cout << "A: \n";
		a.debugDraw(2);
		std::cout << "B: \n";
		b.debugDraw(2);

		std::cout << "\nMerge Check\n";

		a.merge(b);
		std::cout << "A: \n";
		a.debugDraw(4);
		std::cout << "B: \n";
		b.debugDraw(4);
	}
	//Loading 2000 items into the tree to see if it can handle it
	{
		std::cout << "\nStress Test:\n";
		map<std::string, float> stresser;
		std::ifstream str;
		str.open("stressTest.txt");
		for (int i = 0; i < 2000; i++)
		{
			std::string k;
			float v;
			str >> k;
			str >> v;
			v = v / 10;
			stresser.insert(k, v);
		}
		std::cout << "DONE LOADING " << stresser.Size() << " ITEMS\n";
		auto z = stresser.toDynArr();
		for (int i = 0; i < stresser.Size(); i++)
		{
			std::cout << (z + i)->first << " " << (z + i)->second << "\n";
		}
		std::cout << "\n";
		//Iterating without the dynArr
		std::string pos = stresser.min();
		while (pos != stresser.max())
		{
			std::cout << pos << " " << stresser[pos] << "\n";
			pos = stresser.next(pos);
		}
		std::cout << pos << " " << stresser[pos] << "\n";
	}
	return 0;
}