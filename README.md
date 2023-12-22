# Ant-Food_Simulation
*An object-oriented programming project with some visuals and graphs.*

## Description
A visual simulation using SFML of ants and their population stability based on food. 
Graphs were generated using python and the matplotlib library.
> **SFML Guide** https://www.sfml-dev.org/tutorials/2.6/

## Simulation explanation

https://github.com/Goldmite/Ant-Food_Simulation/assets/52678568/9fdc3e78-90fa-40b0-b656-5afecdd8c72e

The simulation is blank board with little dots (ants) moving around with slight variations. 
- Food is also added at the start and after every generation. 
- When an ant comes within smelling distance of the food dot it has a rough sense of it's location and is encouraged to go in that general direction.
- Every so often ants lose health points - when at zero they perish.
- Eating food adds health to the ant.
- If the ant has eaten enough food, it duplicates.

## Graph explanation
You can see the graphs I've generated from my testing with different values (https://github.com/Goldmite/Ant-Food_Simulation/tree/main/GraphsFolder)
### How to understand the graph names?
**Example:**
"A20 F20 NF2 AS10 S1 I10.png"
| Name | Description |
| ------------- | ------------- |
| A20 | 20 ants at the start. |
| F20 | 20 food at the start. |
| NF2 | 2 new food spawns when a food is eaten. |
| AS10 | 10 speed of ants (recommended 5-10). |
| S1 | starve (lose health) every 1 generation. |
| I10 | 10 iterations of the simulation. |

Now let's look at an average graph of 50 generations: "Avrg A20 F20 NF2 AS8 S1 TI5.png"
> This is the average of 5 iterations

![Example](https://github.com/Goldmite/Ant-Food_Simulation/blob/main/GraphsFolder/Avrg%20A20%20F20%20NF2%20AS8%20S1%20TI5.png)
> generated using matplotlib in Python. (code: https://github.com/Goldmite/Ant-Food_Simulation/blob/a808b3e38905b819f5737599a5f2b791790fcdae/AntPopulation/AntPopulation.py)

## Code explanation
Since this is OOP, let's take a look at the class and it's functions.

1. Movement
- Just the starting parametres for every ant:
```
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
```

  - Move function which makes the ant move randomly in an angle cone if there is no food near.
```
void AntMove(std::vector<food>& Food, float sec) { //ant updates position and angle
  <...>
  if (!isFoodNear) { //if theres no food nearby continue random movement
    int R = rand() % 2;
    Angle = (R == 1) ? (Angle + rand() % TurningAngle) : (Angle - rand() % TurningAngle); //updates angle after every move
  }
  <...>
  x += sin((Angle * M_PI) / 180) * Speed * sec;
  y -= cos((Angle * M_PI) / 180) * Speed * sec;
  AntBody.setPosition(x, y); //updates ant position
}
```
  - However, if there's food then we cycle through all the food and find the coordinates of the closest one.
```
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
}
```
  - Then the ant locates in which quarter of it's circle vision the food is located in and accordingly finds the angle facing that quarter.
```
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
```
2. Eating
- Next comes the function for when the ant finds the food.
- Using the SFML function: .getGlobalBounds().intersects
> **.intersects()** This overload returns the overlapped rectangle in the intersection parameter.
Returns
True if rectangles overlap, false otherwise

- Then we increase the counter of EatenFood and randomly generate more food and push back the food into the vector.
```
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
```

## Conclusion
This is one of my first attempts of OOP and I achieved the goals at mind that I wanted. 
The simulation itself is quite interesting because there has to be the right number of ants and food for it to be a stable population over generations.
I'm sure further data analytical and statistical work can be done to analyze the data. Especially, with recently aqcuired statistical skills it would be interesting to find the best values and determine the statistical model.











