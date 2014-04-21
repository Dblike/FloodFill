class Node {
  public : 
          byte walls;
          int cost;  
          boolean visited;
};

//Map
const int mapSize = 5;
Node node[mapSize][mapSize];
//Distance
long timeDiff=0, currentTime=0;
int motorSpeed=5, tempDist=0;
int botDistX=0, botDistY=0;
int curNodeX=0, curNodeY=0;
enum Direction {NORTH=1, EAST, SOUTH, WEST};
Direction direct=NORTH;
//Constants
const int tileSize = 20;
const int timeDelay = 2000;

void setup()  { 
  for(int i=0; i<mapSize; i++) {
    for(int j=0; j<mapSize; j++){
    node[i][j].cost = abs(i-mapSize/2)+abs(j-mapSize/2);
    node[i][j].walls = 0;
    node[i][j].visited = false;  
    }
  }
  testBench();
  Serial.begin(9600);
  //Initial Settings
  Serial.println("Current Direction: " + String(direct));
  Serial.println("Distance traveled: " + String(botDistX) + "," + String(botDistY));
  Serial.println("Current Node: " + String(curNodeX) + "," + String(curNodeY));
  Serial.println("Wall information: " + String(node[curNodeY][curNodeX].walls));
  for(int i=0; i<mapSize; i++) {
      for(int j=0; j<mapSize; j++) {
        if(i==curNodeY && j==curNodeX) {
          Serial.print("X  ");
        }
        else
          Serial.print(String(node[i][j].cost) + "  ");
      }
      Serial.println();
    }
  for(int i=0; i<mapSize; i++) {
    for(int j=0; j<mapSize; j++){
      Serial.print(String(node[i][j].walls) + "  ");  
    }
    Serial.println();
  }
  Serial.println();
} 

void testBench() {
  node[0][0].walls = 11;
  node[0][1].walls = 11; 
  node[0][2].walls = 11;
  node[0][3].walls = 11;
  node[0][4].walls = 11;
  node[1][0].walls = 8;
  node[1][1].walls = 2; 
  node[1][2].walls = 10;
  node[1][3].walls = 12;
  node[1][4].walls = 6;
  node[2][0].walls = 10;
  node[2][1].walls = 10; 
  node[2][2].walls = 10;
  node[2][3].walls = 9;
  node[2][4].walls = 7;
  node[3][0].walls = 8;
  node[3][1].walls = 0; 
  node[3][2].walls = 2;
  node[3][3].walls = 10;
  node[3][4].walls = 9;
  node[4][0].walls = 13;
  node[4][1].walls = 13; 
  node[4][2].walls = 12;
  node[4][3].walls = 4;
  node[4][4].walls = 6;
}

// the loop routine runs over and over again forever:
void loop()  {
  while(!(curNodeY == mapSize/2 && curNodeX == mapSize/2)) {
    //Time to run program
    timeDiff = millis() - currentTime;
    currentTime = millis();
    
    tempDist = (timeDiff * motorSpeed)/1000;
    addDist(tempDist);  
    curNodeX = botDistX / tileSize;
    curNodeY = botDistY / tileSize;
    
    if(!node[curNodeY][curNodeX].visited){
//      node[curNodeY][curNodeX].walls=getWalls();
      updateCosts();
      getNextDirection();
      node[curNodeY][curNodeX].visited = true;
    }
   
   //Debug information
    Serial.println("Program run time: " + String(timeDiff-timeDelay) + " ms");
    Serial.println("Current Direction: " + String(direct));
    Serial.println("Distance traveled: " + String(botDistX) + "," + String(botDistY));
    Serial.println("Current Node: " + String(curNodeX) + "," + String(curNodeY));
    Serial.println("Wall information: " + String(node[curNodeY][curNodeX].walls));
    for(int i=0; i<mapSize; i++) {
      for(int j=0; j<mapSize; j++) {
        if(i==curNodeY && j==curNodeX) {
          Serial.print("X  ");
        }
        else
          Serial.print(String(node[i][j].cost) + "  ");
      }
      Serial.println();
    }
    delay(timeDelay);
  } 
//  Serial.println("TARGET FOUND!!!!");
}

void addDist(int dist){
  Serial.println("Entering addDist()");
  if(direct==NORTH){
    botDistY -= dist;
  }
  else if(direct==SOUTH){
    botDistY += dist;
  }
  else if(direct==EAST){
    botDistX += dist;
  }
  else
    botDistX -= dist;  
  Serial.println("Exiting addDist()");
  Serial.println();
}

byte getWalls() {
  Serial.println("Entering getWalls()");
  Serial.println("Exiting getWalls()");
  Serial.println();
  return 00000000;
}

void updateCosts() {
  Serial.println("Entering updateCosts()");
  boolean check = true;
  while(check) {
    check = false;
    for(int i=0; i<mapSize; i++) {
      for(int j=0; j<mapSize; j++) {
        int neighbors[4] = {-1,-1,-1,-1};
        int tempCost = mapSize*mapSize;
        byte walls = node[i][j].walls;
        if(!bitRead(walls,0) && i-1 >= 0){
          neighbors[0] = node[i-1][j].cost;    
        }
        if(!bitRead(walls,1) && j+1 < mapSize) {
          neighbors[1] = node[i][j+1].cost;
        }
        if(!bitRead(walls,2) && i+1 < mapSize) {
          neighbors[2] = node[i+1][j].cost;
        }
        if(!bitRead(walls,3) && j-1 >= 0) {
          neighbors[3] = node[i][j-1].cost;
        }
        for(int k=0; k<4; k++) {
          if(neighbors[k] < tempCost && neighbors[k] != -1) {
            tempCost = neighbors[k];
          }       
        }
//        Serial.println("Neighbors for " + String(i) + "," + String(j) + " :" + String(neighbors[0]) + "," + String(neighbors[1]) + "," + String(neighbors[2]) + "," + String(neighbors[3]));
//        tempCost = min(min(neighbors[0], neighbors[1]), min(neighbors[2], neighbors[3]));
//        Serial.println("Temp Cost is : " + String(tempCost));
        if(tempCost > node[i][j].cost && tempCost != mapSize*mapSize && !(i == mapSize/2 && j == mapSize/2)) {
//          Serial.println("Changing Node Cost");
          node[i][j].cost = tempCost + 1;
          check = true;
        }
      }
    }
  }
  Serial.println("Exiting updateCosts()");
}

void getNextDirection() {
  Serial.println("Entering getNextDirection()");
  Node tempNode = node[curNodeY][curNodeX];
  byte walls = node[curNodeY][curNodeX].walls;
  Serial.println("Current wall information" + String(node[curNodeY][curNodeX].walls));
  int tempCost = 1000;
  if(!bitRead(walls,0) && curNodeY > 0 && !node[curNodeY-1][curNodeX].visited){
    direct = NORTH;
    tempCost = node[curNodeY-1][curNodeX].cost;    
  }
  if(!bitRead(walls,1) && node[curNodeY][curNodeX+1].cost < tempCost && curNodeX < mapSize-1 && !node[curNodeY][curNodeX+1].visited) {
    direct = EAST;
    tempCost = node[curNodeY][curNodeX+1].cost;
  }
  if(!bitRead(walls,2) && node[curNodeY+1][curNodeX].cost < tempCost && curNodeY < mapSize-1 && !node[curNodeY+1][curNodeX].visited) {
    direct = SOUTH;
    tempCost = node[curNodeY+1][curNodeX].cost;
  }
  if(!bitRead(walls,3) && node[curNodeY][curNodeX-1].cost < tempCost && curNodeX > 0 && !node[curNodeY][curNodeX-1].visited) {
    direct = WEST;
    tempCost = node[curNodeY][curNodeX-1].cost;
  }
  Serial.println("Next Direction: " + String(direct));
  Serial.println("Exiting getNextDirection()");
  Serial.println();
}
