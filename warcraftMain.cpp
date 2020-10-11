#include "Unit.h"
#include <ctime>
#include <cstdlib>
#include <iomanip>


void saveInFile(const Hero* toSave) {
	toSave->saveInFile();
}

void readFromFile(Hero*& toFill) {
	
	std::ifstream readFile("Heroes.dat", std::ios::binary);
	if (!readFile) {
		std::cerr << "There are no saved champions.\n";
		return;
	}
	char buffer[128];
	int cnt = 1;
	while (!readFile.eof()) {
		size_t nameLen;
		readFile.read((char*)&nameLen, sizeof(size_t));
		if (readFile.eof()) {
			break;
		}
		char* name = new char[nameLen];
		readFile.read(name, nameLen);

		int Lvl;
		double currHP;
		readFile.read((char*)&Lvl, sizeof(int));
		readFile.read((char*)&currHP, sizeof(double));
		std::cout << cnt << ") Hero: " << name << " | Level: " << Lvl << " | Current HP: " << currHP << '\n';
		
		readFile.getline(buffer, 128, '|');
		++cnt;
	}
	--cnt;
	
	std::cout << "\nWhich champion do you want to load ?\n";
	std::cout << "Your wish(1-" << cnt << "): ";
	int choice;
	std::cin >> choice;
	while (!std::cin.good() || (choice < 1 || choice > cnt)) {
		std::cin.clear();
		std::cin.ignore();
		std::cout << "Enter a number between 1 and " << cnt << ": ";
		std::cin >> choice;
	}
	
	readFile.close();

	std::ifstream readFile2("Heroes.dat", std::ios::binary);
	if (!readFile2) {
		std::cerr << "Couldn't open \"Heroes.dat\"\n";
		return;
	}

	
	for (int i = 0; i < choice - 1; ++i) {
		readFile2.getline(buffer, 128, '|');
	}

	size_t nameLen = 0;
	readFile2.read((char*)&nameLen, sizeof(size_t));
	char* name = new char[nameLen];
	readFile2.read(name, nameLen);

	if (!strcmp(name, "Warrior")) {
		Warrior* newWarr = new Warrior;
		newWarr->setName("Warrior");
		readFile2.read((char*)&newWarr->Lvl, sizeof(int));
		readFile2.read((char*)&newWarr->currHP, sizeof(double));
		readFile2.read((char*)&newWarr->HP, sizeof(double));
		readFile2.read((char*)&newWarr->STR, sizeof(double));
		readFile2.read((char*)&newWarr->Intelligence, sizeof(double));
		readFile2.read((char*)&newWarr->startingSTR, sizeof(double));
		readFile2.read((char*)&newWarr->startingIntelligence, sizeof(double));
		readFile2.read((char*)&newWarr->monstrKilled, sizeof(int));
		readFile2.read((char*)&newWarr->fury, sizeof(short));

		toFill = newWarr;
	}
	else if (!strcmp(name, "Mage")) {

		Mage* newMage = new Mage;
		newMage->setName("Mage");
		readFile2.read((char*)&newMage->Lvl, sizeof(int));
		readFile2.read((char*)&newMage->currHP, sizeof(double));
		readFile2.read((char*)&newMage->HP, sizeof(double));
		readFile2.read((char*)&newMage->STR, sizeof(double));
		readFile2.read((char*)&newMage->Intelligence, sizeof(double));
		readFile2.read((char*)&newMage->startingSTR, sizeof(double));
		readFile2.read((char*)&newMage->startingIntelligence, sizeof(double));
		readFile2.read((char*)&newMage->monstrKilled, sizeof(int));

		toFill = newMage;
	}
	else {

		Paladin* newPaladin = new Paladin;
		newPaladin->setName("Paladin");
		readFile2.read((char*)&newPaladin->Lvl, sizeof(int));
		readFile2.read((char*)&newPaladin->currHP, sizeof(double));
		readFile2.read((char*)&newPaladin->HP, sizeof(double));
		readFile2.read((char*)&newPaladin->STR, sizeof(double));
		readFile2.read((char*)&newPaladin->Intelligence, sizeof(double));
		readFile2.read((char*)&newPaladin->startingSTR, sizeof(double));
		readFile2.read((char*)&newPaladin->startingIntelligence, sizeof(double));
		readFile2.read((char*)&newPaladin->monstrKilled, sizeof(int));
		
		toFill = newPaladin;
	}
	readFile2.close();
	std::cin.ignore();
}


void chooseChamp(Hero*& hero) {

	std::cout << "Choose your champion:\n";
	std::cout << "Type W for Warrior | M for Mage | P for Paladin: ";
	char buffer[10];
	std::cin.ignore();
	std::cin.getline(buffer,10);
	while(strcmp(buffer,"M") && strcmp(buffer,"W") && strcmp(buffer,"P")) {
		std::cout << "Choose from M, W and P: ";
		std::cin.getline(buffer, 10);
	}
	char c = *buffer;
	switch (c) {
	case 'W': {
		std::cout << "You just chose to be a Warrior !\n";
		hero = new Warrior; break;
	}
	case 'M':
	{
		std::cout << "You just chose to be a Mage !\n";
		hero = new Mage; break;
	}
	case 'P':
	{
		std::cout << "You just chose to be a Paladin !\n";
		hero = new Paladin; break;
	}
	}
	std::cout << '\n';
}

void Menu() {
	std::cout << "- To check the map and your hero information, type INFO.\n";
	std::cout << "- To choose a zone to attack, type ATTACK.\n";
	std::cout << "- To quit, type EXIT.\n";
}

bool mapEmpty(Unit*** map, int size) {
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			if (map[i][j]) {
				return false;
			}
		}
	}
	return true;
}
void createMap(Unit***& map, int size) {

	map = new Unit**[size];
	for (int i = 0; i < size; ++i) {
		map[i] = new Unit*[size];
		for (int j = 0; j < size; ++j) {
			map[i][j] = nullptr;
		}
	}
}

void fillMap(Unit***& map, int size) {

	srand(time(NULL));
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			int monster = rand() % 4;
			switch (monster) {
			case 0:break;       
			case 1:map[i][j] = new Goblin; break;   
			case 2:map[i][j] = new Dragonkin; break;   
			case 3:map[i][j] = new DeathKnight; break;  
			}
		}
	}
}

void deleteMap(Unit*** map, int size) {
	for (int i = 0; i < size; ++i) {
		for(int j=0;j<size;++j)
			delete map[i][j];
	}
	for (int i = 0; i < size; ++i) {
		delete map[i];
	}
	delete map;
}

void printMap(Unit***map, int size,const Hero*champ) {

	std::cout << std::setw(40) << std::setfill(' ');
	std::cout << "MAP INFO:\n\n";
	std::cout << std::setw(21) << std::setfill(' ');
	for (int i = 0; i < size; ++i) {
		std::cout << "  " <<  i << "  ";
	}
	std::cout << '\n';
	std::cout << std::setw(20) << std::setfill(' ');
	for (int i = 0; i < size; ++i) {
		std::cout << std::setw(20) << std::setfill(' ');
		std::cout << " ";
		for (int j = 0; j < size; ++j) {
			std::cout << "-----";
		}
		std::cout << '\n';
		std::cout << std::setw(19) << std::setfill(' ');
		std::cout << i << ":|";
		for (int j = 0; j < size; ++j) {
			if(map[i][j] == nullptr){
				std::cout << " 0  ";
			}
			else if (!strcmp(map[i][j]->getName(), "Goblin")) {
				std::cout << " G  ";
			}
			else if (!strcmp(map[i][j]->getName(), "Dragonkin")) {
				std::cout << " Dr ";
			}
			else if (!strcmp(map[i][j]->getName(), "DeathKnight")) {
				std::cout << " DK ";
			}
			std::cout << '|';
		}
		std::cout << '\n';
	}
	std::cout << std::setw(20) << std::setfill(' ');
	std::cout << " ";
	for (int j = 0; j < size; ++j) {
		std::cout << "-----";
	}
	std::cout << "\n\n";
	std::cout << "G - Goblin | ";
	std::cout << "Dr - Dragonkin | ";
	std::cout << "DK - DeadKnight | ";
	std::cout << "0 - Empty\n\n";
	champInfo(champ);
}
int main() {


	int mapSize = 3;
	const int maxMapSize = 10;
	Unit*** map = nullptr;
	createMap(map, mapSize);
	fillMap(map, mapSize);
	
	
	std::cout << std::setw(55) << std::setfill(' ');
	std::cout << "WELCOME TO WARCRAFT BETA VERSION !\n\n";
	Hero* player = nullptr;

	std::cout << "Start a new champion(type 1) or load from file(type 2): ";
	int n;
	std::cin >> n;
	while (!std::cin.good() || n < 1 || n > 2) {
		std::cin.clear();
		std::cin.ignore();
		std::cout << "Enter 1 or 2: ";
		std::cin >> n;
	}
	std::cout << "\n\n";

	if (n == 1)
		chooseChamp(player);
	else
		readFromFile(player);

	std::cout << "\n\n";
	if (!player) {
		chooseChamp(player);
	}

	std::cout << "\n\n";
	char command[25];
	do {
		Menu();
		std::cout << "\nEnter command:";
		std::cin.getline(command, 25);
	

		if (!strcmp(command, "INFO")) {
			printMap(map, mapSize, player);
		}
		else if (!strcmp(command, "ATTACK")) {

			int x;
			std::cout << "\n\nVertical field: ";
			std::cin >> x;
			while (!std::cin.good() || x<0 || x>=mapSize) {
				std::cin.clear();
				std::cin.ignore();
				std::cout << "Enter number between 0 and " << mapSize - 1 << ": ";
				std::cin >> x;
			}
			int y;
			std::cout << "Horizontal field: ";
			std::cin >> y;
			while (!std::cin.good() || y<0 || y>=mapSize) {
				std::cin.clear();
				std::cin.ignore();
				std::cout << "Enter number between 0 and " << mapSize - 1 << ": ";
				std::cin >> y;
			}
			while(!map[x][y]) {
				std::cout << "\nThe field you want to attack is empty !\n\n";
				std::cout << "Please enter new coordinates !\n";
				std::cout << "Vertical field: ";
				std::cin >> x;
				while (!std::cin.good() || x<0 || x>=mapSize) {
					std::cin.clear();
					std::cin.ignore();
					std::cout << "Enter number between 0 and " << mapSize - 1 << ": ";
					std::cin >> x;
				}
				std::cout << "Horizontal field: ";
				std::cin >> y;
				while (!std::cin.good() || y<0 || y>=mapSize) {
					std::cin.clear();
					std::cin.ignore();
					std::cout << "Enter number between 0 and " << mapSize - 1 << ": ";
					std::cin >> y;
				}
			}
			while (!player->isDead() && !map[x][y]->isDead()) {
				player->attack(map[x][y]);
				if (map[x][y]->isDead()) {
					delete map[x][y];
					map[x][y] = nullptr;
					break;
				}
				map[x][y]->attack(player);
			}
			if (mapEmpty(map, mapSize)) {
				deleteMap(map, mapSize);
				if (mapSize < maxMapSize) {
					++mapSize;
				}
				createMap(map, mapSize);
				fillMap(map, mapSize);
				std::cout << "\n You killed all monsters on this map, so we proceed to a new area !\n";
			}
			std::cin.ignore();
		}
		else if (!strcmp(command, "EXIT")) {
			std::cout << "Do you want to save your progress before leaving ? (type: yes/no)\n";
			std::cout << "Your wish:";
			std::cin.getline(command, 25);
			while(strcmp(command, "yes") && strcmp(command,"no")) {
				std::cout << "Type yes or no: ";
				std::cin.getline(command, 25);
			}
			if (!strcmp(command, "yes")) {
				saveInFile(player);
				break;
			}
			else {
				break;
			}
		}
		std::cout << "\n\n";
		if (player->isDead()) {
			std::cout << "Type L if you want to load a Hero or something else to quit.\n";
			char load;
			std::cin >> load;
			if (load == 'L') {
				delete player;
				player = nullptr;
				readFromFile(player);
				if (!player) {
					std::cout << "Type something to exit.\n";
					char a;
					std::cin >> a;
					break;
				}
			}
			else
				break;
		}
	} while (strcmp(command, "EXIT"));

	delete player;
	deleteMap(map, mapSize);

	return 0;
}