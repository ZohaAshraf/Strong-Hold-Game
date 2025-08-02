#include "Stronghold.h"
#include<iomanip>
#include <algorithm>
#include<cstring>
// Military class implementation
Military::Military() : soldiers(0), archers(0), cavalry(0), siegeUnits(0) {}

void Military::addSoldiers(int count) { soldiers += count; }
void Military::addArchers(int count) { archers += count; }
void Military::addCavalry(int count) { cavalry += count; }
void Military::addSiegeUnits(int count) { siegeUnits += count; }

int Military::getSoldiers() const { return soldiers; }
int Military::getArchers() const { return archers; }
int Military::getCavalry() const { return cavalry; }
int Military::getSiegeUnits() const { return siegeUnits; }

int Military::calculateAttackPower() const {
	return soldiers * 10 + archers * 15 + cavalry * 20 + siegeUnits * 25;
}

int Military::calculateDefensePower() const {
	return soldiers * 12 + archers * 10 + cavalry * 15 + siegeUnits * 20;
}

void Military::trainUnits(ResourceType resourceType, int amount) {
	// Simple implementation: increase unit counts based on resource type
	if (amount <= 0) return;
	switch (resourceType) {
	case GOLD: soldiers += amount; break;
	case FOOD: archers += amount; break;
	case WOOD: cavalry += amount; break;
	case STONE: siegeUnits += amount; break;
	}
}

void Military::takeCasualties(int amount) {
	int totalUnits = soldiers + archers + cavalry + siegeUnits;
	if (totalUnits == 0) return;

	int soldierLoss = (amount * soldiers) / totalUnits;
	int archerLoss = (amount * archers) / totalUnits;
	int cavalryLoss = (amount * cavalry) / totalUnits;
	int siegeLoss = amount - soldierLoss - archerLoss - cavalryLoss;

	soldiers = max(0, soldiers - soldierLoss);
	archers = max(0, archers - archerLoss);
	cavalry = max(0, cavalry - cavalryLoss);
	siegeUnits = max(0, siegeUnits - siegeLoss);
}

void Military::saveToFile(ofstream& outFile) {
	outFile.write((char*)&soldiers, sizeof(soldiers));
	outFile.write((char*)&archers, sizeof(archers));
	outFile.write((char*)&cavalry, sizeof(cavalry));
	outFile.write((char*)&siegeUnits, sizeof(siegeUnits));
}

void Military::loadFromFile(ifstream& inFile) {
	inFile.read((char*)&soldiers, sizeof(soldiers));
	inFile.read((char*)&archers, sizeof(archers));
	inFile.read((char*)&cavalry, sizeof(cavalry));
	inFile.read((char*)&siegeUnits, sizeof(siegeUnits));
}

void Military::display() const {
	cout << "Military Status:\n";
	cout << "Soldiers: " << soldiers << endl;
	cout << "Archers: " << archers << endl;
	cout << "Cavalry: " << cavalry << endl;
	cout << "Siege Units: " << siegeUnits << endl;
}

// Technology class implementation
Technology::Technology() : agricultureAdvanced(false), militaryAdvanced(false),
constructionAdvanced(false), economyAdvanced(false), researchPoints(0) {
}

void Technology::addResearchPoints(int points) { researchPoints += points; }

bool Technology::researchTechnology(ResourceType type) {
	if (researchPoints < 100) return false;
	switch (type) {
	case FOOD:
		if (!agricultureAdvanced) {
			agricultureAdvanced = true;
			researchPoints -= 100;
			return true;
		}
		break;
	case GOLD:
		if (!economyAdvanced) {
			economyAdvanced = true;
			researchPoints -= 100;
			return true;
		}
		break;
	case WOOD:
		if (!constructionAdvanced) {
			constructionAdvanced = true;
			researchPoints -= 100;
			return true;
		}
		break;
	case STONE:
		if (!militaryAdvanced) {
			militaryAdvanced = true;
			researchPoints -= 100;
			return true;
		}
		break;
	}
	return false;
}

bool Technology::isAgricultureAdvanced() const { return agricultureAdvanced; }
bool Technology::isMilitaryAdvanced() const { return militaryAdvanced; }
bool Technology::isConstructionAdvanced() const { return constructionAdvanced; }
bool Technology::isEconomyAdvanced() const { return economyAdvanced; }

void Technology::saveToFile(ofstream& outFile) {
	outFile.write((char*)&agricultureAdvanced, sizeof(agricultureAdvanced));
	outFile.write((char*)&militaryAdvanced, sizeof(militaryAdvanced));
	outFile.write((char*)&constructionAdvanced, sizeof(constructionAdvanced));
	outFile.write((char*)&economyAdvanced, sizeof(economyAdvanced));
	outFile.write((char*)&researchPoints, sizeof(researchPoints));
}

void Technology::loadFromFile(ifstream& inFile) {
	inFile.read((char*)&agricultureAdvanced, sizeof(agricultureAdvanced));
	inFile.read((char*)&militaryAdvanced, sizeof(militaryAdvanced));
	inFile.read((char*)&constructionAdvanced, sizeof(constructionAdvanced));
	inFile.read((char*)&economyAdvanced, sizeof(economyAdvanced));
	inFile.read((char*)&researchPoints, sizeof(researchPoints));
}

void Technology::display() const {
	cout << "Technology Status:\n";
	cout << "Agriculture Advanced: " << (agricultureAdvanced ? "Yes" : "No") << endl;
	cout << "Military Advanced: " << (militaryAdvanced ? "Yes" : "No") << endl;
	cout << "Construction Advanced: " << (constructionAdvanced ? "Yes" : "No") << endl;
	cout << "Economy Advanced: " << (economyAdvanced ? "Yes" : "No") << endl;
	cout << "Research Points: " << researchPoints << endl;
}

// Building class implementation
Building::Building() : level(1), resourceBoost(GOLD), boostAmount(0) {
	strcpy_s(name, "None");
}

Building::Building(const char* buildingName, ResourceType resourceType, int boost)
	: level(1), resourceBoost(resourceType), boostAmount(boost) {
	strncpy_s(name, buildingName, MAX_NAME_LENGTH - 1);
	name[MAX_NAME_LENGTH - 1] = '\0';
}

const char* Building::getName() const { return name; }
int Building::getLevel() const { return level; }
ResourceType Building::getResourceBoost() const { return resourceBoost; }
int Building::getBoostAmount() const { return boostAmount; }

void Building::upgrade() { level++; boostAmount += 10; }

void Building::saveToFile(ofstream& outFile) {
	outFile.write(name, MAX_NAME_LENGTH);
	outFile.write((char*)&level, sizeof(level));
	outFile.write((char*)&resourceBoost, sizeof(resourceBoost));
	outFile.write((char*)&boostAmount, sizeof(boostAmount));
}

void Building::loadFromFile(ifstream& inFile) {
	inFile.read(name, MAX_NAME_LENGTH);
	inFile.read((char*)&level, sizeof(level));
	inFile.read((char*)&resourceBoost, sizeof(resourceBoost));
	inFile.read((char*)&boostAmount, sizeof(boostAmount));
}

// Kingdom class implementation
Kingdom::Kingdom() : population(100), happiness(50), buildingCount(0), x(-1), y(-1) {
	strcpy_s(name, "Unknown");
	resources = Resource(1000, 500, 200, 200);
}

Kingdom::Kingdom(const char* kingdomName) : population(100), happiness(50), buildingCount(0), x(-1), y(-1) {
	strncpy_s(name, kingdomName, MAX_NAME_LENGTH - 1);
	name[MAX_NAME_LENGTH - 1] = '\0';
	resources = Resource(1000, 500, 200, 200);
}

const char* Kingdom::getName() const { return name; }
int Kingdom::getPopulation() const { return population; }
int Kingdom::getHappiness() const { return happiness; }

int Kingdom::getGold() const { return resources.gold; }
int Kingdom::getFood() const { return resources.food; }
int Kingdom::getWood() const { return resources.wood; }
int Kingdom::getStone() const { return resources.stone; }

void Kingdom::addGold(int amount) { resources.gold += amount; }
void Kingdom::addFood(int amount) { resources.food += amount; }
void Kingdom::addWood(int amount) { resources.wood += amount; }
void Kingdom::addStone(int amount) { resources.stone += amount; }

bool Kingdom::spendGold(int amount) {
	if (resources.gold >= amount) {
		resources.gold -= amount;
		return true;
	}
	return false;
}

bool Kingdom::spendFood(int amount) {
	if (resources.food >= amount) {
		resources.food -= amount;
		return true;
	}
	return false;
}

bool Kingdom::spendWood(int amount) {
	if (resources.wood >= amount) {
		resources.wood -= amount;
		return true;
	}
	return false;
}

bool Kingdom::spendStone(int amount) {
	if (resources.stone >= amount) {
		resources.stone -= amount;
		return true;
	}
	return false;
}

void Kingdom::setPosition(int newX, int newY) { x = newX; y = newY; }
int Kingdom::getX() const { return x; }
int Kingdom::getY() const { return y; }

Military& Kingdom::getMilitary() { return military; }

void Kingdom::processTurn() {
	// Simple resource production
	resources.food += tech.isAgricultureAdvanced() ? 100 : 50;
	resources.gold += tech.isEconomyAdvanced() ? 200 : 100;
	resources.wood += tech.isConstructionAdvanced() ? 50 : 20;
	resources.stone += tech.isMilitaryAdvanced() ? 50 : 20;

	// Building boosts
	for (int i = 0; i < buildingCount; i++) {
		switch (buildings[i].getResourceBoost()) {
		case GOLD: resources.gold += buildings[i].getBoostAmount(); break;
		case FOOD: resources.food += buildings[i].getBoostAmount(); break;
		case WOOD: resources.wood += buildings[i].getBoostAmount(); break;
		case STONE: resources.stone += buildings[i].getBoostAmount(); break;
		}
	}

	// Population consumption
	resources.food -= population;
	happiness = resources.food >= 0 ? min(100, happiness + 5) : max(0, happiness - 10);
	population = resources.food >= 0 ? population + 10 : population - 10;
	if (population <= 0) population = 0;
	if (resources.food < 0) resources.food = 0;
}

void Kingdom::collectTaxes() {
	int tax = population * 2;
	resources.gold += tax;
	happiness -= 5;
	if (happiness < 0) happiness = 0;
	cout << "Collected " << tax << " gold in taxes.\n";
}

void Kingdom::buildStructure() {
	if (buildingCount >= 10) {
		cout << "Maximum buildings reached!\n";
		return;
	}
	cout << "Choose structure to build:\n";
	cout << "1. Farm (Boosts Food, Cost: 100 Gold, 50 Wood)\n";
	cout << "2. Market (Boosts Gold, Cost: 150 Gold, 50 Stone)\n";
	cout << "3. Quarry (Boosts Stone, Cost: 100 Gold, 50 Wood)\n";
	cout << "4. Sawmill (Boosts Wood, Cost: 100 Gold, 50 Stone)\n";
	int choice;
	cin >> choice;
	ResourceType type;
	const char* name;
	int goldCost = 100, woodCost = 0, stoneCost = 0;
	switch (choice) {
	case 1: type = FOOD; name = "Farm"; woodCost = 50; break;
	case 2: type = GOLD; name = "Market"; stoneCost = 50; goldCost = 150; break;
	case 3: type = STONE; name = "Quarry"; woodCost = 50; break;
	case 4: type = WOOD; name = "Sawmill"; stoneCost = 50; break;
	default: cout << "Invalid choice.\n"; return;
	}
	if (spendGold(goldCost) && spendWood(woodCost) && spendStone(stoneCost)) {
		buildings[buildingCount++] = Building(name, type, 20);
		cout << name << " built successfully!\n";
	}
	else {
		cout << "Not enough resources!\n";
	}
}

void Kingdom::recruitUnits() {
	cout << "Enter number of soldiers to recruit (Cost: 10 Gold, 5 Food each): ";
	int count;
	cin >> count;
	if (count <= 0) return;
	if (spendGold(count * 10) && spendFood(count * 5)) {
		military.addSoldiers(count);
		cout << count << " soldiers recruited.\n";
	}
	else {
		cout << "Not enough resources!\n";
	}
}

void Kingdom::trainTroops() {
	cout << "Choose unit type to train:\n";
	cout << "1. Soldiers (Cost: 10 Gold)\n";
	cout << "2. Archers (Cost: 15 Gold)\n";
	cout << "3. Cavalry (Cost: 20 Gold)\n";
	cout << "4. Siege Units (Cost: 25 Gold)\n";
	int choice;
	cin >> choice;
	cout << "Enter amount: ";
	int amount;
	cin >> amount;
	if (amount <= 0) return;
	ResourceType type;
	int cost;
	switch (choice) {
	case 1: type = GOLD; cost = 10; break;
	case 2: type = FOOD; cost = 15; break;
	case 3: type = WOOD; cost = 20; break;
	case 4: type = STONE; cost = 25; break;
	default: cout << "Invalid choice.\n"; return;
	}
	if (spendGold(amount * cost)) {
		military.trainUnits(type, amount);
		cout << amount << " units trained.\n";
	}
	else {
		cout << "Not enough gold!\n";
	}
}

void Kingdom::managePopulation() {
	cout << "1. Increase Happiness (Cost: 100 Gold, 50 Food)\n";
	cout << "2. Boost Population (Cost: 200 Gold, 100 Food)\n";
	int choice;
	cin >> choice;
	if (choice == 1 && spendGold(100) && spendFood(50)) {
		happiness += 20;
		if (happiness > 100) happiness = 100;
		cout << "Happiness increased!\n";
	}
	else if (choice == 2 && spendGold(200) && spendFood(100)) {
		population += 50;
		cout << "Population increased!\n";
	}
	else {
		cout << "Not enough resources or invalid choice!\n";
	}
}

void Kingdom::researchTechnology() {
	cout << "Choose technology to research (Cost: 100 Research Points):\n";
	cout << "1. Agriculture\n";
	cout << "2. Economy\n";
	cout << "3. Construction\n";
	cout << "4. Military\n";
	int choice;
	cin >> choice;
	ResourceType type;
	switch (choice) {
	case 1: type = FOOD; break;
	case 2: type = GOLD; break;
	case 3: type = WOOD; break;
	case 4: type = STONE; break;
	default: cout << "Invalid choice.\n"; return;
	}
	if (tech.researchTechnology(type)) {
		cout << "Technology researched!\n";
	}
	else {
		cout << "Not enough research points or already researched!\n";
	}
	tech.addResearchPoints(20); // Gain some points each turn
}

void Kingdom::fortify() {
	if (spendStone(50) && spendGold(100)) {
		cout << "Fortifications strengthened!\n";
		// Increase defense power (simplified)
		military.addSoldiers(10);
	}
	else {
		cout << "Not enough resources!\n";
	}
}

void Kingdom::recruitSoldiers(int count) {
	if (spendGold(count * 10) && spendFood(count * 5)) {
		military.addSoldiers(count);
	}
}

void Kingdom::displayStatus() const {
	cout << "\nKingdom: " << name << endl;
	cout << "Position: (" << x << "," << y << ")\n";
	cout << "Population: " << population << endl;
	cout << "Happiness: " << happiness << "%\n";
	cout << "Resources:\n";
	cout << "Gold: " << resources.gold << endl;
	cout << "Food: " << resources.food << endl;
	cout << "Wood: " << resources.wood << endl;
	cout << "Stone: " << resources.stone << endl;
	tech.display();
	cout << "Buildings:\n";
	for (int i = 0; i < buildingCount; i++) {
		cout << buildings[i].getName() << " (Level " << buildings[i].getLevel() << ")\n";
	}
}

void Kingdom::displayMilitary() const {
	military.display();
}

void Kingdom::spyOn(Kingdom* target) {
	if (spendGold(50)) {
		cout << "Spying on " << target->getName() << ":\n";
		target->displayStatus();
	}
	else {
		cout << "Not enough gold to spy!\n";
	}
}

void Kingdom::saveToFile(ofstream& outFile) {
	outFile.write(name, MAX_NAME_LENGTH);
	outFile.write((char*)&population, sizeof(population));
	outFile.write((char*)&happiness, sizeof(happiness));
	outFile.write((char*)&resources, sizeof(resources));
	military.saveToFile(outFile);
	tech.saveToFile(outFile);
	outFile.write((char*)&buildingCount, sizeof(buildingCount));
	for (int i = 0; i < buildingCount; i++) {
		buildings[i].saveToFile(outFile);
	}
	outFile.write((char*)&x, sizeof(x));
	outFile.write((char*)&y, sizeof(y));
}

void Kingdom::loadFromFile(ifstream& inFile) {
	inFile.read(name, MAX_NAME_LENGTH);
	inFile.read((char*)&population, sizeof(population));
	inFile.read((char*)&happiness, sizeof(happiness));
	inFile.read((char*)&resources, sizeof(resources));
	military.loadFromFile(inFile);
	tech.loadFromFile(inFile);
	inFile.read((char*)&buildingCount, sizeof(buildingCount));
	for (int i = 0; i < buildingCount; i++) {
		buildings[i].loadFromFile(inFile);
	}
	inFile.read((char*)&x, sizeof(x));
	inFile.read((char*)&y, sizeof(y));
}

// Map class implementation
Map::Map() : width(MAP_SIZE), height(MAP_SIZE) {
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			grid[i][j] = 0;
			for (int k = 0; k < MAX_KINGDOMS; k++) {
				territoryControl[k][i][j] = 0;
			}
		}
	}
}

Map::Map(int w, int h) : width(min(w, MAP_SIZE)), height(min(h, MAP_SIZE)) {
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			grid[i][j] = 0;
			for (int k = 0; k < MAX_KINGDOMS; k++) {
				territoryControl[k][i][j] = 0;
			}
		}
	}
}

bool Map::isOccupied(int x, int y) const {
	if (x < 0 || x >= width || y < 0 || y >= height) return true;
	return grid[x][y] != 0;
}

void Map::placeKingdom(Kingdom* kingdom, int x, int y) {
	if (isOccupied(x, y)) {
		cout << "Cannot place kingdom on occupied tile!\n";
		return;
	}
	int kingdomIndex = -1;
	for (int i = 0; i < MAX_KINGDOMS; i++) {
		if (grid[x][y] == 0) {
			kingdomIndex = i;
			break;
		}
	}
	if (kingdomIndex == -1) {
		cout << "Maximum kingdoms reached!\n";
		return;
	}
	grid[x][y] = kingdomIndex + 1;
	kingdom->setPosition(x, y);
	territoryControl[kingdomIndex][x][y] = 100;
	expandTerritory(kingdom);
}

bool Map::moveKingdom(Kingdom* kingdom) {
	int currentX = kingdom->getX();
	int currentY = kingdom->getY();
	int kingdomIndex = grid[currentX][currentY] - 1;
	if (kingdomIndex < 0) {
		cout << "Kingdom not found!\n";
		return false;
	}
	cout << "Enter new X position (0-" << width - 1 << "): ";
	int newX;
	cin >> newX;
	cout << "Enter new Y position (0-" << height - 1 << "): ";
	int newY;
	cin >> newY;
	if (newX < 0 || newX >= width || newY < 0 || newY >= height || isOccupied(newX, newY)) {
		cout << "Invalid or occupied position!\n";
		return false;
	}
	grid[currentX][currentY] = 0;
	grid[newX][newY] = kingdomIndex + 1;
	kingdom->setPosition(newX, newY);
	expandTerritory(kingdom);
	return true;
}

void Map::expandTerritory(Kingdom* kingdom) {
	int x = kingdom->getX();
	int y = kingdom->getY();
	int kingdomIndex = grid[x][y] - 1;
	if (kingdomIndex < 0) return;
	int strength = kingdom->getMilitary().calculateAttackPower() / 10;
	if (strength < 1) strength = 1;
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			int distance = abs(i - x) + abs(j - y);
			if (distance <= strength) {
				int influence = 100 - (distance * 10);
				if (influence > territoryControl[kingdomIndex][i][j]) {
					territoryControl[kingdomIndex][i][j] = influence;
				}
			}
		}
	}
}

void Map::displayMap() const {
	cout << "\nWorld Map:\n  ";
	for (int i = 0; i < width; i++) cout << i << " ";
	cout << endl;
	for (int j = 0; j < height; j++) {
		cout << j << " ";
		for (int i = 0; i < width; i++) {
			if (grid[i][j] == 0) cout << ". ";
			else cout << grid[i][j] << " ";
		}
		cout << endl;
	}
}

void Map::displayTerritory(Kingdom* kingdom) const {
	int kingdomIndex = -1;
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			if (grid[i][j] > 0 && strcmp(kingdom->getName(), "") != 0) {
				kingdomIndex = grid[i][j] - 1;
				break;
			}
		}
	}
	if (kingdomIndex == -1) {
		cout << "Kingdom not found!\n";
		return;
	}
	cout << "\nTerritory for " << kingdom->getName() << ":\n  ";
	for (int i = 0; i < width; i++) cout << i << " ";
	cout << endl;
	for (int j = 0; j < height; j++) {
		cout << j << " ";
		for (int i = 0; i < width; i++) {
			int control = territoryControl[kingdomIndex][i][j];
			if (control >= 75) cout << "# ";
			else if (control >= 50) cout << "O ";
			else if (control >= 25) cout << "o ";
			else if (control > 0) cout << ". ";
			else cout << "  ";
		}
		cout << endl;
	}
}

void Map::launchAttack(Kingdom* attacker, Kingdom* defender) {
	int distance = abs(attacker->getX() - defender->getX()) + abs(attacker->getY() - defender->getY());
	if (distance > 3) {
		cout << "Target too far to attack!\n";
		return;
	}
	int attackPower = attacker->getMilitary().calculateAttackPower();
	int defensePower = defender->getMilitary().calculateDefensePower();
	cout << attacker->getName() << " attacks " << defender->getName() << "!\n";
	attackPower = attackPower * (80 + rand() % 41) / 100;
	defensePower = defensePower * (80 + rand() % 41) / 100;
	int attackerCasualties = defensePower / 10;
	int defenderCasualties = attackPower / 8;
	if (attackPower > defensePower) {
		cout << attacker->getName() << " wins!\n";
		attacker->getMilitary().takeCasualties(attackerCasualties);
		defender->getMilitary().takeCasualties(defenderCasualties);
		int goldPlunder = defender->getGold() / 5;
		int foodPlunder = defender->getFood() / 5;
		defender->spendGold(goldPlunder);
		defender->spendFood(foodPlunder);
		attacker->addGold(goldPlunder);
		attacker->addFood(foodPlunder);
	}
	else {
		cout << defender->getName() << " defends successfully!\n";
		attacker->getMilitary().takeCasualties(attackerCasualties * 2);
		defender->getMilitary().takeCasualties(defenderCasualties / 2);
	}
}

void Map::saveToFile(ofstream& outFile) {
	outFile.write((char*)&width, sizeof(width));
	outFile.write((char*)&height, sizeof(height));
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			outFile.write((char*)&grid[i][j], sizeof(grid[i][j]));
		}
	}
	for (int k = 0; k < MAX_KINGDOMS; k++) {
		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {
				outFile.write((char*)&territoryControl[k][i][j], sizeof(territoryControl[k][i][j]));
			}
		}
	}
}

void Map::loadFromFile(ifstream& inFile) {
	inFile.read((char*)&width, sizeof(width));
	inFile.read((char*)&height, sizeof(height));
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			inFile.read((char*)&grid[i][j], sizeof(grid[i][j]));
		}
	}
	for (int k = 0; k < MAX_KINGDOMS; k++) {
		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {
				inFile.read((char*)&territoryControl[k][i][j], sizeof(territoryControl[k][i][j]));
			}
		}
	}
}

// DiplomacyManager class implementation
DiplomacyManager::DiplomacyManager() : treatyCount(0) {
	for (int i = 0; i < MAX_KINGDOMS; i++) {
		for (int j = 0; j < MAX_KINGDOMS; j++) {
			relations[i][j] = NEUTRAL;
		}
	}
}

bool DiplomacyManager::hasTreaty(Kingdom* k1, Kingdom* k2) const {
	for (int i = 0; i < treatyCount; i++) {
		if ((strcmp(treaties[i].kingdom1, k1->getName()) == 0 && strcmp(treaties[i].kingdom2, k2->getName()) == 0) ||
			(strcmp(treaties[i].kingdom1, k2->getName()) == 0 && strcmp(treaties[i].kingdom2, k1->getName()) == 0)) {
			return treaties[i].active;
		}
	}
	return false;
}

bool DiplomacyManager::proposeTreaty(Kingdom* proposer, Kingdom* receiver) {
	if (treatyCount >= MAX_TREATIES || hasTreaty(proposer, receiver)) {
		cout << "Cannot propose treaty!\n";
		return false;
	}
	cout << "Choose treaty type:\n1. Peace\n2. Alliance\n3. Trade\n4. Non-Aggression\n";
	int choice;
	cin >> choice;
	if (choice < 1 || choice > 4) {
		cout << "Invalid treaty type!\n";
		return false;
	}
	TreatyType type = static_cast<TreatyType>(choice - 1);
	cout << "Enter duration (turns): ";
	int duration;
	cin >> duration;
	if (duration <= 0) return false;
	Treaty& t = treaties[treatyCount++];
	strcpy_s(t.kingdom1, proposer->getName());
	strcpy_s(t.kingdom2, receiver->getName());
	t.type = type;
	t.turnEstablished = 0;
	t.duration = duration;
	t.active = true;
	updateRelations(proposer, receiver, 2);
	cout << "Treaty proposed!\n";
	return true;
}

bool DiplomacyManager::breakTreaty(Kingdom* kingdom) {
	cout << "Select treaty to break:\n";
	int validCount = 0;
	for (int i = 0; i < treatyCount; i++) {
		if (treaties[i].active && (strcmp(treaties[i].kingdom1, kingdom->getName()) == 0 ||
			strcmp(treaties[i].kingdom2, kingdom->getName()) == 0)) {
			validCount++;
			cout << validCount << ". " << (treaties[i].type == PEACE ? "Peace" : treaties[i].type == ALLIANCE ? "Alliance" : treaties[i].type == TRADE ? "Trade" : "Non-Aggression")
				<< " with " << (strcmp(treaties[i].kingdom1, kingdom->getName()) == 0 ? treaties[i].kingdom2 : treaties[i].kingdom1) << endl;
		}
	}
	if (validCount == 0) {
		cout << "No treaties to break.\n";
		return false;
	}
	cout << "Enter number (0 to cancel): ";
	int choice;
	cin >> choice;
	if (choice <= 0 || choice > validCount) return false;
	int counter = 0;
	for (int i = 0; i < treatyCount; i++) {
		if (treaties[i].active && (strcmp(treaties[i].kingdom1, kingdom->getName()) == 0 ||
			strcmp(treaties[i].kingdom2, kingdom->getName()) == 0)) {
			counter++;
			if (counter == choice) {
				treaties[i].active = false;
				cout << "Treaty broken!\n";
				return true;
			}
		}
	}
	return false;
}

bool DiplomacyManager::breakTreaty(Kingdom* k1, Kingdom* k2) {
	for (int i = 0; i < treatyCount; i++) {
		if (treaties[i].active && ((strcmp(treaties[i].kingdom1, k1->getName()) == 0 && strcmp(treaties[i].kingdom2, k2->getName()) == 0) ||
			(strcmp(treaties[i].kingdom1, k2->getName()) == 0 && strcmp(treaties[i].kingdom2, k1->getName()) == 0))) {
			treaties[i].active = false;
			updateRelations(k1, k2, -2);
			return true;
		}
	}
	return false;
}

void DiplomacyManager::viewTreaties(Kingdom* kingdom) const {
	bool found = false;
	cout << "Treaties for " << kingdom->getName() << ":\n";
	for (int i = 0; i < treatyCount; i++) {
		if (treaties[i].active && (strcmp(treaties[i].kingdom1, kingdom->getName()) == 0 ||
			strcmp(treaties[i].kingdom2, kingdom->getName()) == 0)) {
			found = true;
			cout << (treaties[i].type == PEACE ? "Peace" : treaties[i].type == ALLIANCE ? "Alliance" : treaties[i].type == TRADE ? "Trade" : "Non-Aggression")
				<< " with " << (strcmp(treaties[i].kingdom1, kingdom->getName()) == 0 ? treaties[i].kingdom2 : treaties[i].kingdom1)
				<< " (" << treaties[i].duration << " turns)\n";
		}
	}
	if (!found) cout << "No active treaties.\n";
}

void DiplomacyManager::checkRelations(Kingdom* kingdom) const {
	cout << "Relations for " << kingdom->getName() << ":\n";
	// Simplified: relations are not tied to kingdom index
}

void DiplomacyManager::declareWar(Kingdom* declarer, Kingdom* target) {
	if (hasTreaty(declarer, target)) {
		cout << "Breaking treaty to declare war!\n";
		breakTreaty(declarer, target);
	}
	updateRelations(declarer, target, -3);
	cout << declarer->getName() << " declares war on " << target->getName() << "!\n";
}

RelationshipStatus DiplomacyManager::getRelationship(Kingdom* k1, Kingdom* k2) const {
	return NEUTRAL; // Simplified
}

void DiplomacyManager::updateRelations(Kingdom* k1, Kingdom* k2, int change) {
	// Simplified: no actual relation update
}

void DiplomacyManager::saveToFile(ofstream& outFile) {
	outFile.write((char*)&treatyCount, sizeof(treatyCount));
	for (int i = 0; i < treatyCount; i++) {
		outFile.write((char*)&treaties[i], sizeof(Treaty));
	}
}

void DiplomacyManager::loadFromFile(ifstream& inFile) {
	inFile.read((char*)&treatyCount, sizeof(treatyCount));
	for (int i = 0; i < treatyCount; i++) {
		inFile.read((char*)&treaties[i], sizeof(Treaty));
	}
}

// MarketPlace class implementation
MarketPlace::MarketPlace() : offerCount(0) {
	prices[GOLD] = 100;
	prices[FOOD] = 10;
	prices[WOOD] = 20;
	prices[STONE] = 30;
}

void MarketPlace::displayPrices() const {
	cout << "Market Prices:\n";
	cout << "Gold: " << prices[GOLD] << endl;
	cout << "Food: " << prices[FOOD] << endl;
	cout << "Wood: " << prices[WOOD] << endl;
	cout << "Stone: " << prices[STONE] << endl;
}

void MarketPlace::updatePrices() {
	for (int i = 0; i < 4; i++) {
		prices[i] = prices[i] * (90 + rand() % 21) / 100;
	}
}

void MarketPlace::buyResources(Kingdom* kingdom) {
	cout << "Buy Resources:\n";
	cout << "1. Food (Price: " << prices[FOOD] << " Gold)\n";
	cout << "2. Wood (Price: " << prices[WOOD] << " Gold)\n";
	cout << "3. Stone (Price: " << prices[STONE] << " Gold)\n";
	int choice;
	cin >> choice;
	cout << "Enter quantity: ";
	int quantity = 0;
	cin >> choice;
	if (quantity <= 0) return;
	ResourceType type;
	int price;
	switch (choice) {
	case 1: type = FOOD; price = prices[FOOD]; break;
	case 2: type = WOOD; price = prices[WOOD]; break;
	case 3: type = STONE; price = prices[STONE]; break;
	default: cout << "Invalid choice.\n"; return;
	}
	if (kingdom->spendGold(price * quantity)) {
		switch (type) {
		case FOOD: kingdom->addFood(quantity); break;
		case WOOD: kingdom->addWood(quantity); break;
		case STONE: kingdom->addStone(quantity); break;
		default: break;
		}
		cout << "Purchased " << quantity << " units.\n";
	}
	else {
		cout << "Not enough gold!\n";
	}
}

void MarketPlace::sellResources(Kingdom* kingdom) {
	cout << "Sell Resources:\n";
	cout << "1. Food (Price: " << prices[FOOD] / 2 << " Gold)\n";
	cout << "2. Wood (Price: " << prices[WOOD] / 2 << " Gold)\n";
	cout << "3. Stone (Price: " << prices[STONE] / 2 << " Gold)\n";
	int choice;
	cin >> choice;
	cout << "Enter quantity: ";
	int quantity;
	cin >> quantity;
	if (quantity <= 0) return;
	ResourceType type;
	int price;
	switch (choice) {
	case 1: type = FOOD; price = prices[FOOD] / 2; break;
	case 2: type = WOOD; price = prices[WOOD] / 2; break;
	case 3: type = STONE; price = prices[STONE] / 2; break;
	default: cout << "Invalid choice.\n"; return;
	}
	bool success;
	switch (type) {
	case FOOD: success = kingdom->spendFood(quantity); break;
	case WOOD: success = kingdom->spendWood(quantity); break;
	case STONE: success = kingdom->spendStone(quantity); break;
	default: success = false; break;
	}
	if (success) {
		kingdom->addGold(price * quantity);
		cout << "Sold " << quantity << " units.\n";
	}
	else {
		cout << "Not enough resources!\n";
	}
}

bool MarketPlace::proposeTrade(Kingdom* offerer, Kingdom* receiver) {
	if (offerCount >= MAX_TRADE_OFFERS) {
		cout << "Maximum trade offers reached!\n";
		return false;
	}
	cout << "Enter resources to offer (Gold Food Wood Stone): ";
	int g, f, w, s;
	cin >> g >> f >> w >> s;
	cout << "Enter resources to request (Gold Food Wood Stone): ";
	int rg, rf, rw, rs;
	cin >> rg >> rf >> rw >> rs;
	TradeOffer& offer = tradeOffers[offerCount++];
	strcpy_s(offer.offerer, offerer->getName());
	strcpy_s(offer.receiver, receiver->getName());
	offer.offering = Resource(g, f, w, s);
	offer.requesting = Resource(rg, rf, rw, rs);
	offer.isSmuggling = false;
	offer.accepted = false;
	cout << "Trade proposed!\n";
	return true;
}

void MarketPlace::viewTradeOffers(Kingdom* kingdom) const {
	bool found = false;
	cout << "Trade Offers for " << kingdom->getName() << ":\n";
	for (int i = 0; i < offerCount; i++) {
		if (strcmp(tradeOffers[i].receiver, kingdom->getName()) == 0 && !tradeOffers[i].accepted) {
			found = true;
			cout << i + 1 << ". From " << tradeOffers[i].offerer << ": Offer("
				<< tradeOffers[i].offering.gold << "G, " << tradeOffers[i].offering.food << "F, "
				<< tradeOffers[i].offering.wood << "W, " << tradeOffers[i].offering.stone << "S) Request("
				<< tradeOffers[i].requesting.gold << "G, " << tradeOffers[i].requesting.food << "F, "
				<< tradeOffers[i].requesting.wood << "W, " << tradeOffers[i].requesting.stone << "S)\n";
		}
	}
	if (!found) cout << "No trade offers.\n";
}

bool MarketPlace::respondToOffer(Kingdom* kingdom, int offerIndex, bool accept) {
	if (offerIndex < 0 || offerIndex >= offerCount || tradeOffers[offerIndex].accepted) return false;
	if (strcmp(tradeOffers[offerIndex].receiver, kingdom->getName()) != 0) return false;
	if (!accept) {
		tradeOffers[offerIndex].accepted = true; // Mark as processed
		return true;
	}
	// Simplified: assume trade is valid
	tradeOffers[offerIndex].accepted = true;
	cout << "Trade accepted!\n";
	return true;
}

void MarketPlace::initiateSmuggling(Kingdom* kingdom) {
	cout << "Smuggling not implemented.\n";
}

void MarketPlace::saveToFile(ofstream& outFile) {
	outFile.write((char*)prices, sizeof(prices));
	outFile.write((char*)&offerCount, sizeof(offerCount));
	for (int i = 0; i < offerCount; i++) {
		outFile.write((char*)&tradeOffers[i], sizeof(TradeOffer));
	}
}

void MarketPlace::loadFromFile(ifstream& inFile) {
	inFile.read((char*)prices, sizeof(prices));
	inFile.read((char*)&offerCount, sizeof(offerCount));
	for (int i = 0; i < offerCount; i++) {
		inFile.read((char*)&tradeOffers[i], sizeof(TradeOffer));
	}
}

// CommunicationSystem class implementation
CommunicationSystem::CommunicationSystem() : messageCount(0) {}

void CommunicationSystem::sendMessage(const char* sender, const char* receiver, const char* content) {
	if (messageCount >= MAX_MESSAGES) return;
	Message& msg = messages[messageCount++];
	strcpy_s(msg.sender, sender);
	strcpy_s(msg.receiver, receiver);
	strncpy_s(msg.content, content, MAX_MESSAGE_LENGTH - 1);
	msg.content[MAX_MESSAGE_LENGTH - 1] = '\0';
	msg.read = false;
}
void CommunicationSystem::showMessages(const char* kingdomName) {
	bool found = false;
	cout << "Messages for " << kingdomName << ":\n";
	for (int i = 0; i < messageCount; i++) {
		if (strcmp(messages[i].receiver, kingdomName) == 0) {
			found = true;
			cout << (messages[i].read ? "[Read] " : "[Unread] ") << "From " << messages[i].sender << ": " << messages[i].content << endl;
			messages[i].read = true; // Now allowed without const
		}
	}
	if (!found) cout << "No messages.\n";
}

void CommunicationSystem::sendNewMessage(Kingdom* sender) {
	cout << "Enter recipient kingdom name: ";
	char receiver[MAX_NAME_LENGTH];
	cin.ignore();
	cin.getline(receiver, MAX_NAME_LENGTH);
	cout << "Enter message (max " << MAX_MESSAGE_LENGTH << " chars): ";
	char content[MAX_MESSAGE_LENGTH];
	cin.getline(content, MAX_MESSAGE_LENGTH);
	sendMessage(sender->getName(), receiver, content);
	cout << "Message sent!\n";
}

void CommunicationSystem::saveToFile(ofstream& outFile) {
	outFile.write((char*)&messageCount, sizeof(messageCount));
	for (int i = 0; i < messageCount; i++) {
		outFile.write((char*)&messages[i], sizeof(Message));
	}
}

void CommunicationSystem::loadFromFile(ifstream& inFile) {
	inFile.read((char*)&messageCount, sizeof(messageCount));
	for (int i = 0; i < messageCount; i++) {
		inFile.read((char*)&messages[i], sizeof(Message));
	}
}