#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

struct bead
{
public:
	int ring;
	int pos;

	bead right()
	{
		bead tmp{ ring, pos };
		if (pos <= (ring * 2)) // Right side (and center)
		{
			tmp.pos += 1;
			tmp.ring += 1;
		}
		else if (pos <= (ring * 4)) // Down side
		{
			tmp.pos -= 1;
		}
		else if (pos >= (ring * 6)) // Up side
		{
			tmp.pos = (pos + 1) % (ring * 8);
		}
		else // Left side
		{
			tmp.pos -= 5;
			tmp.ring -= 1;
		}
		return tmp;
	}
	bead left()
	{
		bead tmp = { ring, pos };
		if (pos > (ring * 6) || (pos == 0 && ring != 0)) // Up side
		{
			tmp.pos = ((pos - 1) + (ring * 8)) % (ring * 8);
		}
		else if (pos > (ring * 4) || ring == 0) // Left side (and center)
		{
			tmp.pos += 5;
			tmp.ring += 1;
		}
		else if (pos >= (ring * 2)) // Down side
		{
			tmp.pos -= 1;
		}
		else // Right side
		{
			tmp.pos -= 1;
			tmp.ring -= 1;
		}
		return tmp;
	}
	bead up()
	{
		bead tmp = { ring, pos };
		if (pos >= (ring * 6) || pos == 0) // Up side (and center)
		{
			tmp.pos += 7;
			tmp.ring += 1;
		}
		else if (pos >= ring * 4) // Left side
			tmp.pos += 1;
		else if (pos <= ring * 2) // Right side
			tmp.pos -= 1;
		else // Down side
		{
			tmp.pos -= 3;
			tmp.ring -= 1;
		}
		return tmp;
	}
	bead down()
	{
		bead tmp{ ring, pos };
		if (pos < (ring * 2)) // Right side
			tmp.pos += 1;
		else if ((pos <= (ring * 4) && pos != 0) || ring == 0) // Down side (and center)
		{
			tmp.pos += 3;
			tmp.ring += 1;
		}
		else if (pos <= (ring * 6) && pos != 0) // Left side 
			tmp.pos -= 1;
		else // Up side
		{
			if (ring == 1)
				tmp.pos = 0;
			else
				tmp.pos = (pos - 7) % ((ring - 1) * 8);
			tmp.ring -= 1;
		}
		return tmp;
	}

	bead outer()
	{
		bead tmp{ 0, 0 };
		if (ring != 0)
		{
			tmp.ring = ring + 1;
			tmp.pos = pos + pos / ring + (pos % (2 * ring) < ring && pos % (2 * ring) > 0);
		}
		return tmp;
	}
	bead inner()
	{
		bead tmp{ 0, 0 };
		if (ring != 0)
		{
			tmp.ring = ring - 1;
			if (tmp.ring == 0)
				tmp.pos = 0;
			else
				tmp.pos = (pos - pos / ring - (pos % (2 * ring) < ring && pos % (2 * ring) > 0)) % tmp.ring;
		}
		return tmp;
	}

	bead next()
	{
		bead temp{ ring, pos };
		return ++temp;
	}
	bead previous()
	{
		bead temp{ ring, pos };
		return --temp;
	}

	bead& operator++()
	{
		pos = (pos + 1) % max(ring * 8, 1);
		return *this;
	}
	bead operator++(int)
	{
		bead temp = *this;
		++* this;
		return temp;
	}

	bead& operator--()
	{
		pos = ((pos - 1) + ring * 8) % max(ring * 8, 1);
		return *this;
	}
	bead operator--(int)
	{
		bead temp = *this;
		--* this;
		return temp;
	}
};


bead coordinateToBead(int x, int y, int totalRings)
{
	x -= totalRings - 1;
	y -= totalRings - 1;

	int ring = max(abs(x), abs(y));
	x += ring;
	y += ring;

	int pos;

	if (ring == 0)
		pos = 0;
	else if (x == ring * 2)
		pos = x * 2 - y;
	else if (x == 0)
		pos = (ring * 6 + y) % (ring * 8);
	else if (y == ring * 2)
		pos = x;
	else
		pos = ring * 6 - x;

	bead bead{ ring, pos };
	return bead;
}

int main()
{

	fstream fio("program.o", fstream::in);
	if (!fio.is_open())
	{
		cout << "Error! File is missing!";
		return 0;
	}
	
	string line;
	getline(fio, line);
	int ringsSize = line.length() / 2 + 1;
	fio.seekg(0);
	
	vector<vector<char>> rings(ringsSize);

	rings[0] = vector<char>(1);
	for (int i = 1; i < ringsSize; i++)
		rings[i] = vector<char>(8 * i, 1);

	for (int i = 0; i < (2 * ringsSize - 1); i++)
	{
		string line;
		getline(fio, line);
		for (int j = 0; j < (2 * ringsSize - 1); j++)
		{
			char c = line[j];
			bead bead = coordinateToBead(i, j, ringsSize);
			// cout << bead.ring << " " << bead.pos << " " << c << endl;
			rings[bead.ring][bead.pos] = c;
		}
	}

	/*for (int i = 0; i < ringsSize; i++)
	{
		cout << "Ring " << i << ": ";
		for (auto bead : rings[i])
		{
			cout << bead << ' ';
		}
		cout << endl;
	}*/
	bead pointer{ 0, 0 };

	bool direction = true;
	int alter = 0;
	
	while (true)
	{
		/*cout << "Current bead: " << pointer.ring << " " << pointer.pos << endl;
		cout << "Alter: " << alter << " Direction: " << direction << endl;
		cout << "Current command: " << rings[pointer.ring][pointer.pos] << endl;
		cout << endl;*/

		switch (rings[pointer.ring][pointer.pos])
		{
		// Pointer control
		case '>':
			if (!alter)
			{
				if (pointer.right().ring == pointer.ring)
					direction = true;
				pointer = pointer.right();
			} else
			{
				if (rings[pointer.left().ring][pointer.left().pos] != 0)
				{
					if (pointer.right().ring == pointer.ring)
						direction = true;
					pointer = pointer.right();
				} else
				{
					if (direction)
						++pointer;
					else
						--pointer;
				}
			}
			break;
		case '<':
			if (!alter)
			{
				if (pointer.left().ring == pointer.ring)
					direction = false;
				pointer = pointer.left();
			}
			else
			{
				if (rings[pointer.left().ring][pointer.left().pos] != 0)
				{
					if (pointer.left().ring == pointer.ring)
						direction = false;
					pointer = pointer.left();
				}
				else
				{
					if (direction)
						++pointer;
					else
						--pointer;
				}
			}
			break;
		case '^':
			if (!alter)
			{
				if (pointer.up().ring == pointer.ring)
					direction = true;
				pointer = pointer.up();
			}
			else
			{
				if (rings[pointer.up().ring][pointer.up().pos] != 0)
				{
					if (pointer.up().ring == pointer.ring)
						direction = true;
					pointer = pointer.up();
				}
				else
				{
					if (direction)
						++pointer;
					else
						--pointer;
				}
			}
			break;
		case 'v':
			if (!alter)
			{
				if (pointer.down().ring == pointer.ring)
					direction = false;
				pointer = pointer.down();
			}
			else
			{
				if (rings[pointer.down().ring][pointer.down().pos] != 0)
				{
					if (pointer.down().ring == pointer.ring)
						direction = false;
					pointer = pointer.down();
				}
				else
				{
					if (direction)
						++pointer;
					else
						--pointer;
				}
			}
			break;
		// IO
		case 'r':
			if (alter)
			{
				int value;
				cin >> value;
				rings[pointer.inner().ring][pointer.inner().pos] = value;
			} else
			{
				char value;
				cin >> value;
				rings[pointer.inner().ring][pointer.inner().pos] = value;
			}
			if (direction)
				++pointer;
			else
				--pointer;
			break;
		case 'R':
			if (alter)
			{
				int value;
				cin >> value;
				rings[pointer.outer().ring][pointer.outer().pos] = value;
			}
			else
			{
				char value;
				cin >> value;
				rings[pointer.outer().ring][pointer.outer().pos] = value;
			}
			if (direction)
				++pointer;
			else
				--pointer;
			break;
		case 'w':
			if (!alter)
				cout << rings[pointer.inner().ring][pointer.inner().pos];
			else
				cout << (int)rings[pointer.inner().ring][pointer.inner().pos];
			if (direction)
				++pointer;
			else
				--pointer;
			break;
		case 'W':
			if (!alter)
				cout << rings[pointer.outer().ring][pointer.outer().pos];
			else
				cout << (int)rings[pointer.outer().ring][pointer.outer().pos];
			if (direction)
				++pointer;
			else
				--pointer;
			break;
		// Arithmetic operators
		case 'a':
			if (!alter)
				rings[pointer.inner().ring][pointer.inner().pos] += rings[pointer.outer().ring][pointer.outer().pos];
			else
				++rings[pointer.inner().ring][pointer.inner().pos];
			if (direction)
				++pointer;
			else
				--pointer;
			break;
		case 'A':
			if (!alter)
				rings[pointer.outer().ring][pointer.outer().pos] += rings[pointer.inner().ring][pointer.inner().pos];
			else
				++rings[pointer.outer().ring][pointer.outer().pos];
			if (direction)
				++pointer;
			else
				--pointer;
			break;
		case 's':
			if (!alter)
				rings[pointer.inner().ring][pointer.inner().pos] -= rings[pointer.outer().ring][pointer.outer().pos];
			else
				--rings[pointer.inner().ring][pointer.inner().pos];
			if (direction)
				++pointer;
			else
				--pointer;
			break;
		case 'S':
			if (!alter)
				rings[pointer.outer().ring][pointer.outer().pos] -= rings[pointer.inner().ring][pointer.inner().pos];
			else
				--rings[pointer.outer().ring][pointer.outer().pos];
			if (direction)
				++pointer;
			else
				--pointer;
			break;
		case 'm':
			if (!alter)
				rings[pointer.inner().ring][pointer.inner().pos] *= rings[pointer.outer().ring][pointer.outer().pos];
			else
				rings[pointer.inner().ring][pointer.inner().pos] = pow(rings[pointer.inner().ring][pointer.inner().pos], rings[pointer.outer().ring][pointer.outer().pos]);
			if (direction)
				++pointer;
			else
				--pointer;
			break;
		case 'M':
			if (!alter)
				rings[pointer.outer().ring][pointer.outer().pos] *= rings[pointer.inner().ring][pointer.inner().pos];
			else
				rings[pointer.outer().ring][pointer.outer().pos] = pow(rings[pointer.outer().ring][pointer.outer().pos], rings[pointer.inner().ring][pointer.inner().pos]);
			if (direction)
				++pointer;
			else
				--pointer;
			break;
		case 'p':
			rings[pointer.inner().ring][pointer.inner().pos] = rings[pointer.outer().ring][pointer.outer().pos];
			if (direction)
				++pointer;
			else
				--pointer;
			break;
		case 'P':
			rings[pointer.outer().ring][pointer.outer().pos] = rings[pointer.inner().ring][pointer.inner().pos];
			if (direction)
				++pointer;
			else
				--pointer;
			break;
		// Altered behaviour
		case '#':
			alter = 2;
			if (direction)
				++pointer;
			else
				--pointer;
			break;
		// Other
		case 'q':
			return 0;
		case '-':
		case '\\':
		case '/':
		case '|':
			if (direction)
				++pointer;
			else
				--pointer;
			break;
		default:
			cout << "Unrecognized symbol at ring " << pointer.ring << " pos " << pointer.pos << ". Program will terminate.";
			return 0;
		}
		if (alter)
			alter--;
	}
}
