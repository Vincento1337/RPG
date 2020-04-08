#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

int getRandomNumber(int min, int max)
{
	static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);
	return static_cast<int>(rand() * fraction * (max - min + 1) + min);
}


class Creature
{
protected:
	std::string m_name;
	char m_symbol;
	int m_hp;
	int m_attack;
	int m_gold;

public:
	//constructor
	Creature(std::string name = "", char symbol = ' ', int hp = 0, int attack = 0, int gold = 0)
		: m_name{ name }, m_symbol{ symbol }, m_hp{ hp }, m_attack{ attack }, m_gold{ gold }
	{

	}

	//methods
	void reduceHealth(int hit) { m_hp -= hit; }

	bool isDead() { return m_hp <= 0; }

	void addGold(int gold) { m_gold += gold; }

	//getters
	std::string getName() { return m_name; }
	char getSymbol() { return m_symbol; }
	int getHealth() { return m_hp; }
	int getAttack() { return m_attack; }
	int getGold() { return m_gold; }
};

class Player : public Creature
{
private:
	int m_level = 1;

public:
	//constructor
	Player(std::string name = "")
		: Creature(name, '@', 10, 1, 0)
	{
	}

	//methods
	void levelUp()
	{
		++m_level;
		++m_attack;
	}

	void print()
	{
	}

	//getters
	int getLevel() { return m_level; }
	bool hasWon() { return m_level >= 20; }
};

class Monster : public Creature
{
public:
	enum Type
	{
		DRAGON,
		ORC,
		SLIME,
		MAX_TYPES,
	};

	struct MonsterData
	{
		const char* name;
		char symbol;
		int health;
		int damage;
		int gold;
	};

	//pre-declaration of monsterData array
	static MonsterData monsterData[];

	Monster(Type type)
		: Creature(monsterData[type].name, monsterData[type].symbol,
			monsterData[type].health, monsterData[type].damage, monsterData[type].gold)
	{

	}

	static Monster getRandomMonster()
	{
		int num = getRandomNumber(0, MAX_TYPES - 1);
		return Monster(static_cast<Type>(num));
	}
};

Monster::MonsterData Monster::monsterData[Monster::MAX_TYPES]
{
	{ "dragon", 'D', 20, 4, 100 },
	{ "orc", 'o', 4, 2, 25 },
	{ "slime", 's', 1, 1, 10 }
};



void attackMonster(Player& p, Monster& m)
{
	if (p.isDead())
		return;

	m.reduceHealth(p.getAttack());
	std::cout << "You hit monster for " << p.getAttack() << " health\n";

	if (m.isDead())
	{
		std::cout << "You killed monster\n";
		p.levelUp();
		std::cout << "You are now " << p.getLevel() << " level and have " << p.getAttack() << " attack\n";
		p.addGold(m.getGold());
		std::cout << "You got " << m.getGold() << " gold and now have " << p.getGold() << " gold\n";

	}
}

void attackPlayer(Player& p, Monster& m)
{
	p.reduceHealth(m.getAttack());
	std::cout << "Monster hit you for " << m.getAttack() << " helth\n";
}

void fightMonster(Player& p)
{
	Monster m = Monster::getRandomMonster();
	std::cout << "A " << m.getName() << " (" << m.getSymbol() << ") was created.\n";

	while (!p.isDead() && !m.isDead())
	{

		char choice;
		do
		{
			std::cout << "(R)un or (F)ight: ";
			std::cin >> choice;
		} while (choice != 'R' && choice != 'r' && choice != 'F' && choice != 'f');

		if (choice == 'F' || choice == 'f')
		{
			attackMonster(p, m);
			attackPlayer(p, m);
		}


		if (choice == 'R' || choice == 'r')
		{
			if (getRandomNumber(0, 1) == 0)
			{
				std::cout << "You failed to flee\n";
				attackPlayer(p, m);
				continue;
			}
			else
			{
				std::cout << "You fled successfully!\n";
				break;
			}
		}


	}
}

int main()
{
	//bad realisation of rand function in VS, when initial and further values are almost the same
	//so srand is used for setting up initial value and rand is used for the further value
	srand(static_cast<unsigned int>(time(0)));
	rand();

	//player creation
	std::cout << "Enter your name: ";
	std::string name;
	std::cin >> name;

	Player player(name);

	std::cout << "Welcome, " << player.getName() << std::endl;
	std::cout << "You have " << player.getHealth() << " health and are carrying " << player.getGold() << " gold" << std::endl;



	while (!player.isDead() && !player.hasWon())
	{
		fightMonster(player);
	}

	if (player.isDead())
	{
		std::cout << "You died at level " << player.getLevel() <<
			" and was carrying " << player.getGold() << " gold\n";
	}
	else
	{
		std::cout << "You won this game with " << player.getGold() << std::endl;
	}

	return 0;
}