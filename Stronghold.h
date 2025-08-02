#ifndef STRONGHOLD_H
#define STRONGHOLD_H

#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <string>
#include<cstring>

using namespace std;

// Constants
const int MAX_KINGDOMS = 5;
const int MAX_MESSAGES = 20;
const int MAX_TREATIES = 10;
const int MAX_TRADE_OFFERS = 15;
const int MAP_SIZE = 10;
const int MAX_NAME_LENGTH = 50;
const int MAX_MESSAGE_LENGTH = 200;

// Enums
enum ResourceType {
	GOLD,
	FOOD,
	WOOD,
	STONE
};

enum TreatyType {
	PEACE,
	ALLIANCE,
	TRADE,
	NON_AGGRESSION
};

enum RelationshipStatus {
	FRIENDLY,
	NEUTRAL,
	HOSTILE,
	WAR
};

// Structures
struct Resource {
	int gold;
	int food;
	int wood;
	int stone;

	Resource() : gold(0), food(0), wood(0), stone(0) {}
	Resource(int g, int f, int w, int s) : gold(g), food(f), wood(w), stone(s) {}
};

struct Message {
	char sender[MAX_NAME_LENGTH];
	char receiver[MAX_NAME_LENGTH];
	char content[MAX_MESSAGE_LENGTH];
	bool read;

	Message() {
		strcpy_s(sender, "");
		strcpy_s(receiver, "");
		strcpy_s(content, "");
		read = false;
	}
};

struct Treaty {
	char kingdom1[MAX_NAME_LENGTH];
	char kingdom2[MAX_NAME_LENGTH];
	TreatyType type;
	int turnEstablished;
	int duration;
	bool active;

	Treaty() {
		strcpy_s(kingdom1, "");
		strcpy_s(kingdom2, "");
		type = PEACE;
		turnEstablished = 0;
		duration = 0;
		active = false;
	}
};

struct TradeOffer {
	char offerer[MAX_NAME_LENGTH];
	char receiver[MAX_NAME_LENGTH];
	Resource offering;
	Resource requesting;
	bool isSmuggling;
	bool accepted;

	TradeOffer() {
		strcpy_s(offerer, "");
		strcpy_s(receiver, "");
		isSmuggling = false;
		accepted = false;
	}
};

// Classes
class Military {
private:
	int soldiers;
	int archers;
	int cavalry;
	int siegeUnits;

public:
	Military();

	void addSoldiers(int count);
	void addArchers(int count);
	void addCavalry(int count);
	void addSiegeUnits(int count);

	int getSoldiers() const;
	int getArchers() const;
	int getCavalry() const;
	int getSiegeUnits() const;

	int calculateDefensePower() const;
	int calculateAttackPower() const;

	void trainUnits(ResourceType resourceType, int amount);
	void takeCasualties(int amount);

	void saveToFile(ofstream& outFile);
	void loadFromFile(ifstream& inFile);

	void display() const;
};

class Technology {
private:
	bool agricultureAdvanced;
	bool militaryAdvanced;
	bool constructionAdvanced;
	bool economyAdvanced;
	int researchPoints;

public:
	Technology();

	void addResearchPoints(int points);
	bool researchTechnology(ResourceType type);

	bool isAgricultureAdvanced() const;
	bool isMilitaryAdvanced() const;
	bool isConstructionAdvanced() const;
	bool isEconomyAdvanced() const;

	void saveToFile(ofstream& outFile);
	void loadFromFile(ifstream& inFile);

	void display() const;
};

class Building {
private:
	char name[MAX_NAME_LENGTH];
	int level;
	ResourceType resourceBoost;
	int boostAmount;

public:
	Building();
	Building(const char* buildingName, ResourceType resourceType, int boost);

	const char* getName() const;
	int getLevel() const;
	ResourceType getResourceBoost() const;
	int getBoostAmount() const;

	void upgrade();

	void saveToFile(ofstream& outFile);
	void loadFromFile(ifstream& inFile);
};

class Kingdom {
private:
	char name[MAX_NAME_LENGTH];
	int population;
	int happiness;
	Resource resources;
	Military military;
	Technology tech;
	Building buildings[10];
	int buildingCount;
	int x, y; // Position on map

public:
	Kingdom();
	Kingdom(const char* kingdomName);

	const char* getName() const;
	int getPopulation() const;
	int getHappiness() const;

	int getGold() const;
	int getFood() const;
	int getWood() const;
	int getStone() const;

	void addGold(int amount);
	void addFood(int amount);
	void addWood(int amount);
	void addStone(int amount);

	bool spendGold(int amount);
	bool spendFood(int amount);
	bool spendWood(int amount);
	bool spendStone(int amount);

	void setPosition(int newX, int newY);
	int getX() const;
	int getY() const;

	Military& getMilitary();

	void processTurn();
	void collectTaxes();
	void buildStructure();
	void recruitUnits();
	void trainTroops();
	void managePopulation();
	void researchTechnology();
	void fortify();

	void recruitSoldiers(int count);

	void displayStatus() const;
	void displayMilitary() const;

	void spyOn(Kingdom* target);

	void saveToFile(ofstream& outFile);
	void loadFromFile(ifstream& inFile);
};

class Map {
private:
	int width;
	int height;
	int grid[MAP_SIZE][MAP_SIZE]; // 0 for empty, >0 for kingdom index+1
	int territoryControl[MAX_KINGDOMS][MAP_SIZE][MAP_SIZE]; // Territory control strength per kingdom

public:
	Map();
	Map(int w, int h);

	bool isOccupied(int x, int y) const;
	void placeKingdom(Kingdom* kingdom, int x, int y);
	bool moveKingdom(Kingdom* kingdom);
	void expandTerritory(Kingdom* kingdom);

	void displayMap() const;
	void displayTerritory(Kingdom* kingdom) const;

	void launchAttack(Kingdom* attacker, Kingdom* defender);

	void saveToFile(ofstream& outFile);
	void loadFromFile(ifstream& inFile);
};

class DiplomacyManager {
private:
	Treaty treaties[MAX_TREATIES];
	int treatyCount;
	RelationshipStatus relations[MAX_KINGDOMS][MAX_KINGDOMS];

public:
	DiplomacyManager();

	bool hasTreaty(Kingdom* k1, Kingdom* k2) const;
	bool proposeTreaty(Kingdom* proposer, Kingdom* receiver);
	bool breakTreaty(Kingdom* kingdom);
	bool breakTreaty(Kingdom* k1, Kingdom* k2);

	void viewTreaties(Kingdom* kingdom) const;
	void checkRelations(Kingdom* kingdom) const;

	void declareWar(Kingdom* declarer, Kingdom* target);
	RelationshipStatus getRelationship(Kingdom* k1, Kingdom* k2) const;

	void updateRelations(Kingdom* k1, Kingdom* k2, int change);

	void saveToFile(ofstream& outFile);
	void loadFromFile(ifstream& inFile);
};

class MarketPlace {
private:
	int prices[4]; // Prices for gold, food, wood, stone
	TradeOffer tradeOffers[MAX_TRADE_OFFERS];
	int offerCount;

public:
	MarketPlace();

	void displayPrices() const;
	void updatePrices();

	void buyResources(Kingdom* kingdom);
	void sellResources(Kingdom* kingdom);

	bool proposeTrade(Kingdom* offerer, Kingdom* receiver);
	void viewTradeOffers(Kingdom* kingdom) const;
	bool respondToOffer(Kingdom* kingdom, int offerIndex, bool accept);

	void initiateSmuggling(Kingdom* kingdom);

	void saveToFile(ofstream& outFile);
	void loadFromFile(ifstream& inFile);
};

class CommunicationSystem {
private:
	Message messages[MAX_MESSAGES];
	int messageCount;

public:
	CommunicationSystem();

	void sendMessage(const char* sender, const char* receiver, const char* content);
	void showMessages(const char* kingdomName);
	void sendNewMessage(Kingdom* sender);

	void saveToFile(ofstream& outFile);
	void loadFromFile(ifstream& inFile);
};

#endif // STRONGHOLD_Hheaderfile