#ifndef UNIT_H
#define UNIT_H

#include <iostream>
#include <fstream>


class Unit {

protected:
	char* name;
	double HP;
	double STR;
	double Intelligence;
	
public:

	Unit(const char* ="Unit",double=0,double=0,double=0);
	Unit(const Unit&other) = delete;
	Unit& operator=(const Unit&other) = delete;
	virtual ~Unit();

	virtual void attack(Unit* enemy) = 0;
	virtual double defend(const Unit& attacking) = 0;
	const char* getName() const { return name; }
	double getHP() const { return HP; }
	double getSTR() const { return STR; }
	double getIntelligence() const { return Intelligence; }
	void setHP(double _HP) { HP = _HP; }
	virtual bool isDead() const { return HP == 0; }
	virtual void setCurrHP(int newHP) = 0;
	virtual double getCurrHP() const = 0;

};


class Hero : public Unit {

protected:
	double startingSTR;
	double startingIntelligence;
	double currHP;
	int Lvl;
	int monstrKilled;


	void levelUp();
	bool isTimeToLevelUp() const;

public:
	Hero();
	Hero(const Hero&other) = delete;
	Hero& operator=(const Hero&other) = delete;
	virtual ~Hero() = default;

	virtual void attack(Unit* enemy);
	virtual double defend(const Unit& enemy);
	void setName(const char* _name);
	int getLvl() const { return Lvl; }
	virtual bool isDead() const { return currHP == 0; }
	virtual void setCurrHP(double newHP) { currHP = newHP; }
	virtual double getCurrHP() const { return currHP; }

	virtual void saveInFile() const = 0;

	friend void readFromFile(Hero*& toFill);
};


class Monster : public Unit {


public:
	Monster();
	Monster(const Monster&other) = delete;
	Monster& operator=(const Monster&other) = delete;
	virtual ~Monster();

	virtual	void attack(Unit* enemy);
	virtual double defend(const Unit& enemy);
	virtual double getCurrHP() const { return HP; }
	virtual void setCurrHP(int newHP) = 0;
};


class Warrior :public Hero {

private:
	const short maxFury = 100;
	short fury;
	bool firstAttack;
	double furyBonus;

public:
	Warrior();
	Warrior(const Warrior&other) = delete;
	Warrior& operator=(const Warrior&other) = delete;
	virtual ~Warrior();

	virtual	void attack(Unit* enemy);
	virtual double defend(const Unit& enemy);
	virtual void setCurrHP(int newHP) { currHP = newHP; }
	virtual double getCurrHP() const { return currHP; }
	virtual void saveInFile() const;

	friend void readFromFile(Hero*& toFill);

};

class Mage : public Hero {

private:
	const short maxMana = 100;
	short mana;

public:

	Mage();
	Mage(const Mage&other) = delete;
	Mage& operator=(const Mage&other) = delete;
	virtual ~Mage();
	
	virtual	void attack(Unit* enemy);
	virtual double defend(const Unit& enemy);
	virtual void setCurrHP(int newHP) { currHP = newHP; }
	virtual double getCurrHP() const { return currHP; }
	virtual void saveInFile() const; 
	
	friend void readFromFile(Hero*& toFill);

};

class Paladin : public Hero {

private:
	short hitCnt;

public:
	Paladin();
	Paladin(const Paladin& other) = delete;
	Paladin& operator=(const Paladin& other) = delete;
	virtual ~Paladin();

	virtual	void attack(Unit* enemy);
	virtual double defend(const Unit& enemy); 
	virtual void setCurrHP(int newHP) { currHP = newHP; }
	virtual double getCurrHP() const { return currHP; }
	virtual void saveInFile() const; 

	friend void readFromFile(Hero*& toFill);
};


class Goblin : virtual public Monster {

public:
	const static double goblinHP;
	const static double goblinSTR;

public:
	Goblin();
	Goblin(const Goblin&other) = delete;
	Goblin& operator=(const Goblin&other) = delete;
	virtual ~Goblin();

	virtual	void attack(Unit* enemy);
	virtual double defend(const Unit& enemy);
	virtual void setCurrHP(int newHP) { HP = newHP; }
	virtual double getCurrHP() const { return HP; }
};

class Dragonkin : virtual public Monster {

public:
	static const double dragonkinHP;
	static const double dragonkinSTR;

private:
	short defCnt;

public:
	Dragonkin();
	Dragonkin(const Dragonkin& other) = delete;
	Dragonkin& operator=(const Dragonkin& other) = delete;
	virtual ~Dragonkin();

	virtual	void attack(Unit* enemy);
	virtual double defend(const Unit& enemy);
	virtual void setCurrHP(int newHP) { HP = newHP; }
	virtual double getCurrHP() const { return HP; }
};

class DeathKnight : public Goblin, public Dragonkin {

private:
	bool swapAttack;

public:
	DeathKnight();
	DeathKnight(const DeathKnight&other) = delete;
	DeathKnight& operator=(const DeathKnight&other) = delete;
	virtual ~DeathKnight();

	virtual	void attack(Unit* enemy);
	virtual double defend(const Unit& enemy);
	virtual void setCurrHP(int newHP) { HP = newHP; }
	virtual double getCurrHP() const { return HP; }
};

void champInfo(const Hero* champ);

#endif