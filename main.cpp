#include "Stronghold.h"
#include<cstring>
#include <limits>

// Global variables
Kingdom* kingdoms[MAX_KINGDOMS];
int kingdomCount = 0;
Map* gameMap;
MarketPlace* market;
DiplomacyManager* diplomacy;
CommunicationSystem* comms;

// Function prototypes
void initializeGame();
void gameLoop();
void saveGameState();
void loadGameState();
void displayKingdomMenu(Kingdom* kingdom);
void handleKingdomAction(Kingdom* kingdom);
void handleDiplomacyAction(Kingdom* kingdom);
void handleTradeAction(Kingdom* kingdom);
void handleWarAction(Kingdom* kingdom);
void handleMapAction(Kingdom* kingdom);
void simulateOtherKingdoms();
Kingdom* selectTargetKingdom(Kingdom* currentKingdom);
void clearScreen();
void waitForEnter();

int main() {
	srand(static_cast<unsigned int>(time(nullptr)));

	cout << "===============================\n";
	cout << "      STRONGHOLD GAME          \n";
	cout << "===============================\n";

	bool newGame = true;
	char choice;
	cout << "Do you want to load a saved game? (y/n): ";
	cin >> choice;

	if (choice == 'y' || choice == 'Y') {
		loadGameState();
		newGame = false;
	}

	if (newGame) {
		initializeGame();
	}

	gameLoop();

	// Clean up
	for (int i = 0; i < kingdomCount; i++) {
		delete kingdoms[i];
	}
	delete gameMap;
	delete market;
	delete diplomacy;
	delete comms;

	return 0;
}

void initializeGame() {
	gameMap = new Map(MAP_SIZE, MAP_SIZE);
	market = new MarketPlace();
	diplomacy = new DiplomacyManager();
	comms = new CommunicationSystem();

	cout << "Enter a name for your kingdom: ";
	char kingdomName[MAX_NAME_LENGTH];
	cin.ignore();
	cin.getline(kingdomName, MAX_NAME_LENGTH);

	kingdoms[0] = new Kingdom(kingdomName);
	kingdomCount++;

	int x = rand() % MAP_SIZE;
	int y = rand() % MAP_SIZE;
	gameMap->placeKingdom(kingdoms[0], x, y);

	const char* aiNames[] = { "Northland", "Westeros", "Eastfall", "Southreach" };
	for (int i = 0; i < 4; i++) {
		kingdoms[kingdomCount] = new Kingdom(aiNames[i]);
		kingdoms[kingdomCount]->addGold(500 + rand() % 500);
		kingdoms[kingdomCount]->addFood(300 + rand() % 300);
		kingdoms[kingdomCount]->addWood(400 + rand() % 200);
		kingdoms[kingdomCount]->addStone(200 + rand() % 200);
		kingdoms[kingdomCount]->recruitSoldiers(50 + rand() % 50);

		bool validPosition = false;
		int kx, ky;
		while (!validPosition) {
			kx = rand() % MAP_SIZE;
			ky = rand() % MAP_SIZE;
			if (!gameMap->isOccupied(kx, ky)) {
				validPosition = true;
			}
		}
		gameMap->placeKingdom(kingdoms[kingdomCount], kx, ky);
		kingdomCount++;
	}

	cout << "Game initialized with " << kingdomCount << " kingdoms!\n";
	waitForEnter();
}

void gameLoop() {
	bool gameRunning = true;
	int turn = 1;

	while (gameRunning) {
		clearScreen();
		cout << "======= TURN " << turn << " =======\n";

		Kingdom* playerKingdom = kingdoms[0];
		displayKingdomMenu(playerKingdom);

		simulateOtherKingdoms();

		for (int i = 0; i < kingdomCount; i++) {
			kingdoms[i]->processTurn();
		}

		if (playerKingdom->getPopulation() <= 0) {
			cout << "Your kingdom has fallen! Game over!\n";
			gameRunning = false;
		}

		turn++;
		saveGameState();
	}
}

void displayKingdomMenu(Kingdom* kingdom) {
	bool backToMain = false;
	while (!backToMain) {
		clearScreen();
		cout << "====== " << kingdom->getName() << " ======\n";
		kingdom->displayStatus();

		cout << "\nOptions:\n";
		cout << "1. Internal Kingdom Management\n";
		cout << "2. Diplomacy and Treaties\n";
		cout << "3. Trade and Market\n";
		cout << "4. Military and Warfare\n";
		cout << "5. View Map\n";
		cout << "6. Messages and Communication\n";
		cout << "7. End Turn\n";
		cout << "8. Save and Exit\n";

		int choice;
		cout << "Enter your choice: ";
		cin >> choice;
		if (cin.fail()) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Invalid input.\n";
			waitForEnter();
			continue;
		}

		switch (choice) {
		case 1: handleKingdomAction(kingdom); break;
		case 2: handleDiplomacyAction(kingdom); break;
		case 3: handleTradeAction(kingdom); break;
		case 4: handleWarAction(kingdom); break;
		case 5: handleMapAction(kingdom); break;
		case 6:
			comms->showMessages(kingdom->getName());
			comms->sendNewMessage(kingdom);
			waitForEnter();
			break;
		case 7: backToMain = true; break;
		case 8: saveGameState(); exit(0);
		default: cout << "Invalid option.\n"; waitForEnter();
		}
	}
}

void handleKingdomAction(Kingdom* kingdom) {
	clearScreen();
	cout << "===== Kingdom Management =====\n";
	cout << "1. Build Structure\n";
	cout << "2. Recruit Units\n";
	cout << "3. Collect Taxes\n";
	cout << "4. Research Technology\n";
	cout << "5. Manage Population\n";
	cout << "6. Back\n";

	int subchoice;
	cout << "Enter your choice: ";
	cin >> subchoice;
	if (cin.fail()) {
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Invalid input.\n";
		waitForEnter();
		return;
	}

	switch (subchoice) {
	case 1: kingdom->buildStructure(); break;
	case 2: kingdom->recruitUnits(); break;
	case 3: kingdom->collectTaxes(); break;
	case 4: kingdom->researchTechnology(); break;
	case 5: kingdom->managePopulation(); break;
	case 6: return;
	default: cout << "Invalid option.\n";
	}
	waitForEnter();
}

void handleDiplomacyAction(Kingdom* kingdom) {
	clearScreen();
	cout << "===== Diplomacy and Treaties =====\n";
	cout << "1. View Current Treaties\n";
	cout << "2. Propose New Treaty\n";
	cout << "3. Break Existing Treaty\n";
	cout << "4. Check Relations\n";
	cout << "5. Back\n";

	int subchoice;
	cout << "Enter your choice: ";
	cin >> subchoice;
	if (cin.fail()) {
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Invalid input.\n";
		waitForEnter();
		return;
	}

	switch (subchoice) {
	case 1: diplomacy->viewTreaties(kingdom); break;
	case 2: {
		Kingdom* target = selectTargetKingdom(kingdom);
		if (target) diplomacy->proposeTreaty(kingdom, target);
		break;
	}
	case 3: diplomacy->breakTreaty(kingdom); break;
	case 4: diplomacy->checkRelations(kingdom); break;
	case 5: return;
	default: cout << "Invalid option.\n";
	}
	waitForEnter();
}

void handleTradeAction(Kingdom* kingdom) {
	clearScreen();
	cout << "===== Trade and Market =====\n";
	cout << "1. View Market Prices\n";
	cout << "2. Buy Resources\n";
	cout << "3. Sell Resources\n";
	cout << "4. Propose Trade Deal\n";
	cout << "5. View Trade Offers\n";
	cout << "6. Smuggling Operations\n";
	cout << "7. Back\n";

	int subchoice;
	cout << "Enter your choice: ";
	cin >> subchoice;
	if (cin.fail()) {
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Invalid input.\n";
		waitForEnter();
		return;
	}

	switch (subchoice) {
	case 1: market->displayPrices(); break;
	case 2: market->buyResources(kingdom); break;
	case 3: market->sellResources(kingdom); break;
	case 4: {
		Kingdom* target = selectTargetKingdom(kingdom);
		if (target) market->proposeTrade(kingdom, target);
		break;
	}
	case 5: market->viewTradeOffers(kingdom); break;
	case 6: market->initiateSmuggling(kingdom); break;
	case 7: return;
	default: cout << "Invalid option.\n";
	}
	waitForEnter();
}

void handleWarAction(Kingdom* kingdom) {
	clearScreen();
	cout << "===== Military and Warfare =====\n";
	cout << "1. View Military Status\n";
	cout << "2. Train Troops\n";
	cout << "3. Declare War\n";
	cout << "4. Launch Attack\n";
	cout << "5. Fortify Position\n";
	cout << "6. Spy on Kingdom\n";
	cout << "7. Back\n";

	int subchoice;
	cout << "Enter your choice: ";
	cin >> subchoice;
	if (cin.fail()) {
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Invalid input.\n";
		waitForEnter();
		return;
	}

	switch (subchoice) {
	case 1: kingdom->displayMilitary(); break;
	case 2: kingdom->trainTroops(); break;
	case 3: {
		Kingdom* target = selectTargetKingdom(kingdom);
		if (target) diplomacy->declareWar(kingdom, target);
		break;
	}
	case 4: {
		Kingdom* target = selectTargetKingdom(kingdom);
		if (target) gameMap->launchAttack(kingdom, target);
		break;
	}
	case 5: kingdom->fortify(); break;
	case 6: {
		Kingdom* target = selectTargetKingdom(kingdom);
		if (target) kingdom->spyOn(target);
		break;
	}
	case 7: return;
	default: cout << "Invalid option.\n";
	}
	waitForEnter();
}

void handleMapAction(Kingdom* kingdom) {
	clearScreen();
	cout << "===== Map View =====\n";
	cout << "1. View World Map\n";
	cout << "2. View Territory Details\n";
	cout << "3. Move Kingdom\n";
	cout << "4. Expand Territory\n";
	cout << "5. Back\n";

	int subchoice;
	cout << "Enter your choice: ";
	cin >> subchoice;
	if (cin.fail()) {
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Invalid input.\n";
		waitForEnter();
		return;
	}

	switch (subchoice) {
	case 1: gameMap->displayMap(); break;
	case 2: gameMap->displayTerritory(kingdom); break;
	case 3: gameMap->moveKingdom(kingdom); break;
	case 4: gameMap->expandTerritory(kingdom); break;
	case 5: return;
	default: cout << "Invalid option.\n";
	}
	waitForEnter();
}

void simulateOtherKingdoms() {
	for (int i = 1; i < kingdomCount; i++) {
		Kingdom* aiKingdom = kingdoms[i];
		int action = rand() % 5;
		switch (action) {
		case 0: aiKingdom->collectTaxes(); break;
		case 1: aiKingdom->buildStructure(); break;
		case 2: aiKingdom->recruitUnits(); break;
		case 3: aiKingdom->trainTroops(); break;
		case 4: aiKingdom->managePopulation(); break;
		}
		if (rand() % 10 < 3) {
			int targetIndex = rand() % kingdomCount;
			if (targetIndex != i) {
				diplomacy->proposeTreaty(aiKingdom, kingdoms[targetIndex]);
			}
		}
	}
	cout << "\nAI kingdoms have taken their turns.\n";
}

Kingdom* selectTargetKingdom(Kingdom* currentKingdom) {
	clearScreen();
	cout << "Select target kingdom:\n";
	int validCount = 0;
	for (int i = 0; i < kingdomCount; i++) {
		if (kingdoms[i] != currentKingdom) {
			cout << validCount + 1 << ". " << kingdoms[i]->getName() << endl;
			validCount++;
		}
	}
	cout << validCount + 1 << ". Cancel\n";
	int choice;
	cout << "Enter your choice: ";
	cin >> choice;
	if (cin.fail() || choice <= 0 || choice > validCount + 1) {
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		return nullptr;
	}
	if (choice == validCount + 1) return nullptr;
	int index = 0, counter = 0;
	for (int i = 0; i < kingdomCount; i++) {
		if (kingdoms[i] != currentKingdom) {
			counter++;
			if (counter == choice) {
				index = i;
				break;
			}
		}
	}
	return kingdoms[index];
}

void saveGameState() {
	ofstream outFile("savegame.dat", ios::binary);
	if (!outFile) {
		cout << "Error saving game.\n";
		return;
	}
	outFile.write((char*)&kingdomCount, sizeof(kingdomCount));
	for (int i = 0; i < kingdomCount; i++) {
		kingdoms[i]->saveToFile(outFile);
	}
	gameMap->saveToFile(outFile);
	diplomacy->saveToFile(outFile);
	market->saveToFile(outFile);
	comms->saveToFile(outFile);
	outFile.close();
	cout << "Game saved successfully!\n";
}

void loadGameState() {
	ifstream inFile("savegame.dat", ios::binary);
	if (!inFile) {
		cout << "No saved game found.\n";
		initializeGame();
		return;
	}
	for (int i = 0; i < kingdomCount; i++) {
		delete kingdoms[i];
		kingdoms[i] = nullptr;
	}
	delete gameMap;
	delete market;
	delete diplomacy;
	delete comms;

	inFile.read((char*)&kingdomCount, sizeof(kingdomCount));
	for (int i = 0; i < kingdomCount; i++) {
		kingdoms[i] = new Kingdom();
		kingdoms[i]->loadFromFile(inFile);
	}
	gameMap = new Map();
	gameMap->loadFromFile(inFile);
	diplomacy = new DiplomacyManager();
	diplomacy->loadFromFile(inFile);
	market = new MarketPlace();
	market->loadFromFile(inFile);
	comms = new CommunicationSystem();
	comms->loadFromFile(inFile);
	inFile.close();
	cout << "Game loaded successfully!\n";
}

void clearScreen() {
#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif
}

void waitForEnter() {
	cout << "\nPress Enter to continue...";
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	cin.get();
}