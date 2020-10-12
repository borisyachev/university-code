#include "Unit.h"

const double Goblin::goblinHP = 10;
const double Goblin::goblinSTR = 3;

const double Dragonkin::dragonkinHP = 30;
const double Dragonkin::dragonkinSTR = 8;

Unit::Unit(const char *_name, double _HP, double _STR, double _Intelligence)
{
	size_t len = strlen(_name);
	name = new char[len + 1];
	strcpy_s(name, len + 1, _name);


	HP = _HP;
	STR = _STR;
	Intelligence = _Intelligence;
}

Unit::~Unit()
{
	delete[] name;
}

Hero::Hero()
{
	name = new char[5];
	strcpy_s(name, 5, "Hero");
	HP = 100;
	currHP = 100;
	STR = 0;
	startingSTR = 0;
	Intelligence = 0;
	startingIntelligence = 0;
	Lvl = 0;
	monstrKilled = 0;
}


void Hero::attack(Unit* enemy)
{
}

double Hero::defend(const Unit & enemy)
{
	return 0.0;
}

void Hero::setName(const char * _name)
{
	size_t len = strlen(_name);
	name = new char[len + 1];
	strcpy_s(name, len + 1, _name);
}


void Hero::levelUp()
{
	HP = HP + HP / 10;
	currHP = HP;
	STR = STR + startingSTR / 3;
	Intelligence = Intelligence + startingIntelligence / 3;
	std::cout << "\n !! YOU JUST LEVELED UP !! \n";
}

bool Hero::isTimeToLevelUp() const
{	
	int check = 1;
	for (int i = 0; i < Lvl; ++i) {
		check *= 2;
	}
	return monstrKilled == check;
}


Monster::Monster()
{
	name = new char[8];
	strcpy_s(name, 8,"Monster");
}


Monster::~Monster()
{
}

void Monster::attack(Unit* enemy)
{
}

double Monster::defend(const Unit & enemy)
{
	return 0.0;
}

Warrior::Warrior() 
{
	name = new char[8];
	strcpy_s(name,8,"Warrior");
	STR = 13;
	Intelligence = 2;
	fury = 0;
	firstAttack = 1;
	furyBonus = 0;

	startingSTR = 13;
	startingIntelligence = 2;
	currHP = 100;
}

Warrior::~Warrior()
{

}

void Warrior::attack(Unit* enemy)
{
	double toDeal = STR + 3 / 10 * Intelligence;
	if (firstAttack) {
		int percentage = fury / 5;
		fury = 0;
		furyBonus = percentage / 100 * toDeal;
		firstAttack = false;
	}
	toDeal += furyBonus;
	double defAmount = enemy->defend(*this);
	toDeal -= defAmount;

	if (toDeal <= 0) {
		std::cout << "\n DMG BLOCKED ! \n";
		if (fury <= maxFury - 2)
			fury += 2;
		return;
	}
	else if (toDeal > enemy->getHP()) {
		++monstrKilled;
		std::cout << "\n ENEMY KILLED ! \n";
		enemy->setHP(0);
		firstAttack = true;
		furyBonus = 0;
		if (fury <= maxFury - 2)
			fury += 2;
		if (isTimeToLevelUp()) {
			levelUp();
			++Lvl;
		}
		return;
	}
	else {
		double hpLeft = enemy->getHP() - toDeal;
		std::cout << "\n" << toDeal << " DMG DEALED ! (monster left with " << hpLeft << " hp)\n";
		enemy->setHP(hpLeft);
		if (fury <= maxFury - 2)
			fury += 2;
	}
}

double Warrior::defend(const Unit & enemy)
{
	if (fury <= maxFury - 3) {
		fury += 3;
	}
	return STR/5;
}

void Warrior::saveInFile() const
{
	std::ofstream appFile("Heroes.dat", std::ios::app);
	if (!appFile) {
		std::cerr << "Couldn't open file \"Heroes.dat\"\n";
		return;
	}
	
	size_t nameLen = strlen(name)+1;
	appFile.write((const char*)&nameLen, sizeof(size_t));
	appFile.write(name, nameLen);
	appFile.write((const char*)&Lvl, sizeof(int));
	appFile.write((const char*)&currHP, sizeof(double));

	appFile.write((const char*)&HP, sizeof(double));
	appFile.write((const char*)&STR, sizeof(double));
	appFile.write((const char*)&Intelligence, sizeof(double));
	
	appFile.write((const char*)&startingSTR, sizeof(double));
	appFile.write((const char*)&startingIntelligence, sizeof(double));
	appFile.write((const char*)&monstrKilled, sizeof(int));
	
	appFile.write((const char*)&fury, sizeof(short));
	
	appFile.write("|", 1);
	appFile.close();
}



Mage::Mage()
{
	name = new char[5];
	strcpy_s(name, 5, "Mage");
	STR = 4;
	Intelligence = 11;
	mana = 100;
}

Mage::~Mage()
{
}

void Mage::attack(Unit* enemy)
{
	double toDeal = Intelligence + mana / 100 * 3;
	if(mana>=10)
		mana -= 10;
	double defAmount = enemy->defend(*this);
	toDeal -= defAmount;

	if (toDeal <= 0) {
		std::cout << "\n DMG BLOCKED \n";
		return;
	}
	else if (toDeal >=enemy->getHP()) {
		enemy->setHP(0);
		std::cout << "\n MONSTER KILLED ! \n";
		++monstrKilled;
		if (isTimeToLevelUp()) {
			levelUp();
			++Lvl;
		}
		mana = maxMana;
		return;
	}
	else {
		double hpLeft = enemy->getHP() - toDeal;
		std::cout << "\n" << toDeal << " DMG DEALED ! (monster left with " << hpLeft << " hp)\n";
		enemy->setHP(hpLeft);
	}
}

double Mage::defend(const Unit & enemy)
{
	return Intelligence/8;
}

void Mage::saveInFile() const
{
	std::ofstream appFile("Heroes.dat", std::ios::app);
	if (!appFile) {
		std::cerr << "Couldn't open file \"Heroes.dat\"\n";
		return;
	}

	size_t nameLen = strlen(name) + 1;
	appFile.write((const char*)&nameLen, sizeof(size_t));
	appFile.write(name, nameLen);
	appFile.write((const char*)&Lvl, sizeof(int));
	appFile.write((const char*)&currHP, sizeof(double));

	appFile.write((const char*)&HP, sizeof(double));
	appFile.write((const char*)&STR, sizeof(double));
	appFile.write((const char*)&Intelligence, sizeof(double));

	appFile.write((const char*)&startingSTR, sizeof(double));
	appFile.write((const char*)&startingIntelligence, sizeof(double));
	appFile.write((const char*)&monstrKilled, sizeof(int));


	appFile.write("|", 1);
	appFile.close();
}


Paladin::Paladin()
{
	name = new char[8];
	strcpy_s(name, 8, "Paladin");
	STR = 9;
	Intelligence = 6;
	hitCnt = 0;
}

Paladin::~Paladin()
{
}

void Paladin::attack(Unit* enemy)
{
	double toDeal = STR/2 + Intelligence/2;
	bool empowered = false;
	if (hitCnt == 3) {
		toDeal = toDeal + 1 / 2 * toDeal;
		hitCnt = 1;
		empowered = true;
	}
	double defAmount = enemy->defend(*this);
	toDeal -= defAmount;

	if (toDeal <= 0) {
		std::cout << "\n DMG BLOCKED \n";
		return;
	}
	else if (toDeal >= enemy->getHP()) {
		enemy->setHP(0);
		std::cout << "\n MONSTER KILLED ! \n";
		++monstrKilled;
		if (isTimeToLevelUp()) {
			levelUp();
			++Lvl;
		}
		hitCnt = 1;
		return;
	}
	else {
		double hpLeft = enemy->getHP() - toDeal;
		std::cout << "\n";
		if (empowered) {
			std::cout << "CRIT ! ";
		}
		std::cout << toDeal << " DMG DEALED ! (monster left with " << hpLeft << " hp)\n";
		enemy->setHP(hpLeft);
	}
	if (!empowered)
		++hitCnt;
}

double Paladin::defend(const Unit & enemy)
{
	return STR/12 + Intelligence/12;
}

void Paladin::saveInFile() const
{
	std::ofstream appFile("Heroes.dat", std::ios::app);
	if (!appFile) {
		std::cerr << "Couldn't open file \"Heroes.dat\"\n";
		return;
	}

	size_t nameLen = strlen(name) + 1;
	appFile.write((const char*)&nameLen, sizeof(size_t));
	appFile.write(name, nameLen);
	appFile.write((const char*)&Lvl, sizeof(int));
	appFile.write((const char*)&currHP, sizeof(double));

	appFile.write((const char*)&HP, sizeof(double));
	appFile.write((const char*)&STR, sizeof(double));
	appFile.write((const char*)&Intelligence, sizeof(double));

	appFile.write((const char*)&startingSTR, sizeof(double));
	appFile.write((const char*)&startingIntelligence, sizeof(double));
	appFile.write((const char*)&monstrKilled, sizeof(int));


	appFile.write("|", 1);
	appFile.close();
}


Goblin::Goblin()
{
	name = new char[7];
	strcpy_s(name, 7, "Goblin");
	HP = 10;
	STR = 3;
}


Goblin::~Goblin()
{
}

void Goblin::attack(Unit* enemy)
{
	double toDeal = STR + enemy->getIntelligence() / 10;
	double defAmount = enemy->defend(*this);
	toDeal -= defAmount;

	if (toDeal <= 0) {
		std::cout << "\n YOU SUCCESSFULLY BLOCKED AN ATTACK !\n";
		return;
	}
	else if (toDeal >= enemy->getCurrHP()) {
		std::cout << "\n YOU JUST GOT KILLED - GAME OVER !\n";
		enemy->setCurrHP(0);
		return;
	}
	else {
		double hpLeft = enemy->getCurrHP() - toDeal;
		std::cout << "\n" << toDeal << " DMG TAKEN ! (" << hpLeft << " hp remaining)\n";
		enemy->setCurrHP(hpLeft);
	}
}

double Goblin::defend(const Unit & enemy)
{
	return STR/2;
}

Dragonkin::Dragonkin()
{
	name = new char[10];
	strcpy_s(name, 10, "Dragonkin");
	HP = 30;
	STR = 8;
	defCnt = 1;
}


Dragonkin::~Dragonkin()
{
}

void Dragonkin::attack(Unit* enemy)
{
	double toDeal = STR;
	double defAmount = enemy->defend(*this);
	toDeal -= defAmount;

	if (toDeal <= 0) {
		std::cout << "\n YOU SUCCESSFULLY BLOCKED AN ATTACK !\n";
		return;
	}
	else if (toDeal >= enemy->getCurrHP()) {
		std::cout << "\n YOU JUST GOT KILLED - GAME OVER !\n";
		enemy->setCurrHP(0);
		return;
	}
	else {
		double hpLeft = enemy->getCurrHP() - toDeal;
		std::cout << "\n" << toDeal << " DMG TAKEN ! (" << hpLeft << " hp remaining)\n";
		enemy->setCurrHP(hpLeft);
	}
}

double Dragonkin::defend(const Unit & enemy)
{
	if (defCnt == 3) {
		defCnt = 1;
		return 100;
	}
	return defCnt++;
}

DeathKnight::DeathKnight() 
{
	name = new char[12];
	strcpy_s(name, 12, "DeathKnight");
	STR = goblinSTR + dragonkinSTR;
	HP = goblinHP + dragonkinHP;
}


DeathKnight::~DeathKnight()
{
}

void DeathKnight::attack(Unit* enemy)
{
	if (swapAttack) {
		Goblin::attack(enemy);
		swapAttack = !swapAttack;
		return;
	}
	Dragonkin::attack(enemy);
	swapAttack = !swapAttack;
}

double DeathKnight::defend(const Unit & enemy)
{
	return Dragonkin::defend(enemy);
}

void champInfo(const Hero* champ)
{
	std::cout << "Hero: " << champ->getName() << " | Level: " << champ->getLvl() << '\n';
	std::cout << "maxHP: " << champ->getHP() << " | currHP: " << champ->getCurrHP() << " | STR: " << champ->getSTR() << " | Intelligence: " << champ->getIntelligence() << '\n';
}
