#define _USE_MATH_DEFINES
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <cmath>
#include <fstream>
#include <string>
#include <ctime>

//window parameters
const int WINDOW_HEIGHT = 1080;
const int WINDOW_WIDTH = 1080;
//user choices
const int ITERATIONS = 10; //how many iterations to run the simulation
const int BRIGHTNESS = 5; //recommended 1-5
const int START_ANT = 25; //starting ant amount
const int START_FOOD = 20; //starting food amount
const int NEW_FOOD = 2; //food amount can be spawned when food is eaten
const float ANT_SIZE = 3; //size of ants body
const float ANT_SPEED = 10; //speed of ant (recommended 5-10)
const float SMELL_RANGE = 12.5; //range from food in which the ant goes towards it
const int STARVE = 1; //every how many generations to decrease food
const int WANTED_GEN = 30; //run this amount of generations
bool TrailsON = false; //ant trail
bool PERFORMANCE_MODE = true; //dont save dead ants for performance
bool ClearFile = true; //dont change (false skips 1st gen)

sf::Clock Clock;
class food {
public:
	float x = 0, y = 0, distance = 0;
	sf::RectangleShape FoodBody;
	food() {
		//food random spawn position
		x = rand() % WINDOW_WIDTH;
		y = rand() % WINDOW_HEIGHT;
		//set color, size and position of food
		FoodBody.setFillColor(sf::Color(127, 0, 255, 51 * BRIGHTNESS));
		FoodBody.setSize(sf::Vector2f(6,6));
		FoodBody.setPosition(x, y);
	}
	void FoodDraw(sf::RenderWindow& window) {
		window.draw(FoodBody);
	}
};
class ant {
public:
	float x = 0, y = 0, Speed = 0, EatenFood = 0;
	int AntName, Age = 0, Gen = 0, Angle = 0, TurningAngle = 20; 
	std::string State;
	sf::RectangleShape AntBody;
	sf::Color AntColor = sf::Color(255, 255, 255, 51 * BRIGHTNESS);
	ant() {
		//spawn in center of window
		x = WINDOW_WIDTH / 2;
		y = WINDOW_HEIGHT / 2;
		AntBody.setPosition(x, y);
		//size, speed, angle of ant
		AntBody.setSize(sf::Vector2f(ANT_SIZE, ANT_SIZE));
		
		Speed = ANT_SPEED * 60;
		Angle = rand() % 360;
		AntName = 0;
		State = "Alive";
	}
	void AntMove(std::vector<food>& Food, float sec) { //ant updates position and angle
		if (State != "Dead") {
			int Quarter = 0;
			bool isFoodNear = false;
			//temporary units until we find the nearest food for the ant for which we save the Food X Y
			float tempX = 0, tempY = 0, FoodX = 0, FoodY = 0;
			float shortest_distance = SMELL_RANGE;
			if (Food.size() > 0) {
				for (auto nearbyFood = Food.begin(); nearbyFood != Food.end(); nearbyFood++) {
					tempX = nearbyFood->x;
					tempY = nearbyFood->y;
					nearbyFood->distance = sqrt((tempX - x) * (tempX - x) + (tempY - y) * (tempY - y)); //find nearest food for ant
					if (nearbyFood->distance < shortest_distance) {//save the coords for the closest food to the ant
						shortest_distance = nearbyFood->distance;
						FoodX = nearbyFood->x;
						FoodY = nearbyFood->y;
					}
				}
				if (shortest_distance < SMELL_RANGE) {//ant smells closest food in range to find it
					isFoodNear = true;
					//food in trigonometry circle quarters from ant's perspective
					if (FoodX > x) {
						if (FoodY < y) {
							Quarter = 0; //1st quarter
						}
						else {
							Quarter = 90; //4th quarter
						}
					}
					else {
						if (FoodY < y) {
							Quarter = 270; //2nd quarter
						}
						else {
							Quarter = 180; //3rd quarter
						}
					}
					Angle = rand() % 90 + Quarter; //direct the ant towards the food
				}
			}
			if (!isFoodNear) { //if theres no food nearby continue random movement
				int R = rand() % 2;
				Angle = (R == 1) ? (Angle + rand() % TurningAngle) : (Angle - rand() % TurningAngle); //updates angle after every move
			}
			//teleportation border
			if (y <= 1) { //top border
				y = WINDOW_HEIGHT - 1;
				x = WINDOW_WIDTH - x;
			}
			else if (y >= WINDOW_HEIGHT - 1) { //top border
				y = 1;
				x = WINDOW_WIDTH - x;
			}
			else if (x <= 1) { //left border
				x = WINDOW_WIDTH - 1;
				y = WINDOW_HEIGHT - y;
			}
			else if (x >= WINDOW_WIDTH - 1) { //right border
				x = 1;
				y = WINDOW_HEIGHT - y;
			}
			x += sin((Angle * M_PI) / 180) * Speed * sec;
			y -= cos((Angle * M_PI) / 180) * Speed * sec;
			AntBody.setPosition(x, y); //updates ant position
		}
	}
	void AntEat(std::vector<food>& checkFoodVector) { // ant eat food
		std::vector<food> AliveFood;
		for (auto checkFood = checkFoodVector.begin(); checkFood != checkFoodVector.end(); checkFood++) {
			if (AntBody.getGlobalBounds().intersects(checkFood->FoodBody.getGlobalBounds())) {
				EatenFood++;
				//eaten food disappears and spawns more food
				int FoodSpawn = rand() % (NEW_FOOD + 1);
				for (int i = 0; i < FoodSpawn; i++) {
					food Apple;
					AliveFood.push_back(Apple);
				}
			}
			else {
				AliveFood.push_back(*checkFood); //put non-eaten food in vector
			}
		}
		//ant changes colour depending on how much food it has eaten
		if (EatenFood < 0) {
			AntColor = sf::Color(51, 51, 51, 51 * BRIGHTNESS);
		}
		else if (EatenFood == 0) {
			AntColor = sf::Color(255, 255, 255, 51 * BRIGHTNESS);
		}
		else if (EatenFood == 1) {
			AntColor = sf::Color(0, 0, 255, 51 * BRIGHTNESS);
		}
		else if (EatenFood == 2) {
			AntColor = sf::Color(0, 255, 255, 51 * BRIGHTNESS);
		}
		else if (EatenFood == 3) {
			AntColor = sf::Color(0, 255, 0, 51 * BRIGHTNESS);
		}
		else if (EatenFood == 4) {
			AntColor = sf::Color(255, 255, 0, 51 * BRIGHTNESS);
		}
		else if (EatenFood >= 5) {
			AntColor = sf::Color(255, 0, 0, 51 * BRIGHTNESS);
		}
		AntBody.setFillColor(AntColor);
		checkFoodVector = AliveFood;
	}
	void AntData(std::vector<ant> AntVector, int iteration_count, int AliveAntCount) { //gen, age, state of ant -> data to files
		Age++; //could make it so the same age is applied to all ants -> if there's more ants then the generation goes by faster
		if (Age == round(72000.0 / ANT_SPEED / AliveAntCount)) { //adjust for amount of alive ants
			Age = 0;
			Gen++; //progress gen, reset age
			char FileName[20];
			std::ofstream Data;
			sprintf_s(FileName, "Gen_%03i.txt", Gen); //name file by its gen number
			if (ClearFile) {
				ClearFile = false;
				if (iteration_count == 1) {
					Data.open(FileName, Data.trunc); //clear file previous test's contents
					Data.close();
				}
				printf("Gen %i\n", Gen);
				Data.open(FileName, Data.app);
				Data << "ITERATION: " << iteration_count << std::endl;
				Data.close();
			}
			Data.open(FileName, Data.app); //open and write data in file
			Data << "Ant: " << AntName << " ";
			if (EatenFood > 4) { //set state to Birth and write to file
				//EatenFood = 1;
				State = "Birth";
				Data << "Alive, Birth ";
			}
			else if (EatenFood <= 0) { //set state to Dead and write to file
				State = "Dead";
				Data << "Dead ";
			}
			else {
				State = "Alive"; //set state to Alive and write to file
				Data << "Alive ";
			}
			Data << "Food: " << EatenFood;
			Data << std::endl;
			if (AntName == AntVector.size()) {
				ClearFile = true; //last ant means file should close and should be cleared the next time it is opened
				Data.close();
			}
			EatenFood -= 1.00 / STARVE; //starve mechanic
		}
	}
	void AntDraw(sf::RenderWindow& window, float sec) {
		window.draw(AntBody);
	}
};
void AntLife(std::vector<ant>& checkAntVector, int& AliveAntCount) {//update ant vector when they multiply or die
	std::vector<ant>AliveAnts;
	int temp_name = 0;
	for (auto checkAnt = checkAntVector.begin(); checkAnt != checkAntVector.end(); checkAnt++) {
		temp_name++;
		checkAnt->AntName = temp_name; //number each ant first to last
		if (checkAnt->State == "Alive") {
			AliveAnts.push_back(*checkAnt);
			AliveAntCount++;
		}
		else if (checkAnt->State == "Dead") {
			checkAnt->Speed = 0;
			if (!PERFORMANCE_MODE) {
				AliveAnts.push_back(*checkAnt);
			}
		}
		else if (checkAnt->State == "Birth") {
			for (int i = 0; i < 2 + checkAnt->EatenFood - 5; i++) {
				ant BobJr;
				BobJr.x = checkAnt->x;
				BobJr.y = checkAnt->y;
				BobJr.Gen = checkAnt->Gen;
				BobJr.AntName = checkAnt->AntName;
				AliveAnts.push_back(BobJr);
				AliveAntCount++;
			}
		}
	}
	checkAntVector.resize(AliveAnts.size()); //resize array to the updated one 
	checkAntVector = AliveAnts;
}
int main()
{
	for (int track_iteration = 1; track_iteration < ITERATIONS + 1; track_iteration++) {
		printf("ITERATION: %i\n", track_iteration);
		srand(time(0)); // reset rng
		std::ofstream StartData("StartData.txt");//pass parameter details to file for graph
		StartData << "START_ANT " << START_ANT << "\nSTART_FOOD " << START_FOOD << "\nNEW_FOOD " << NEW_FOOD << "\nANT_SPEED " << ANT_SPEED << "\nSTARVE " << STARVE << "\nITERATION " << ITERATIONS << "\nGENERATIONS " << WANTED_GEN << "\nPERFORMANCE " << PERFORMANCE_MODE;
		StartData.close();
		sf::RenderWindow Board(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Art", sf::Style::Default); //create board 
		//initialize ant, food, vectors
		std::vector<ant> AntVector;
		std::vector<food> FoodVector;
		for (int i = 0; i < START_ANT; i++) {
			ant Bob;
			AntVector.push_back(Bob);
		}
		for (int i = 0; i < START_FOOD; i++) {
			food Grape;
			FoodVector.push_back(Grape);
		}
		//the action of the ants while the window is open
		int TrailResetCounter = 0;
		int track_gen = 0; // track current generation number
		int displayBuffer = 2;
		while (Board.isOpen()) {
			//time
			sf::Time elapsed = Clock.restart();
			float sec = elapsed.asSeconds();
			sf::Event Event;
			while (Board.pollEvent(Event)) {
				if (Event.type == sf::Event::Closed) {
					Board.close();
				}
			}
			if (TrailsON) {
				TrailResetCounter++;
				if (TrailResetCounter == 29) {
					Board.clear(sf::Color::Black);
					TrailResetCounter = 0;
				}
			}
			else {
				Board.clear(sf::Color::Black);
			}
			int AliveAntCount = 3; //limit alive ant amount, buffer 
			AntLife(AntVector, AliveAntCount);
			for (auto it = AntVector.begin(); it != AntVector.end(); it++) {//update every ant position and draw it
				it->AntMove(FoodVector, sec);
				it->AntEat(FoodVector);
				it->AntData(AntVector, track_iteration, AliveAntCount);
				it->AntDraw(Board, sec);
				track_gen = it->Gen;
			}
			for (auto it = FoodVector.begin(); it != FoodVector.end(); it++) {
				it->FoodDraw(Board);
			}
			if (displayBuffer == 2) {
				Board.display();
				displayBuffer = 0;
			}
			displayBuffer++;
			if (track_gen == WANTED_GEN or AliveAntCount == 3) {
				Board.close();
			}
		}
	}
}
