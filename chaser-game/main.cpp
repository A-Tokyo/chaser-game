#include <GLUT/glut.h>
#include <iostream>
#include <time.h>
#include <math.h>
//#include <stdio.h>
//#include <Windows.h>
#define PI 3.14159265
#define toDeg 57.2957795131

//structures
typedef struct vector {
    float x;
    float y;
    float z;
    vector(float x, float y, float z){
        this->x = x;
        this->y = y;
        this->z =z;
    }
    vector(){
        this->x = 0;
        this->y = 0;
        this->z =0;
    }
    std::string toString(){
        return "["+ std::to_string(x) + ", "+ std::to_string(y) + ", "+std::to_string(z)+"]" ;
    }
    vector unitVector(){
        float magnitude = sqrtf(pow(x,2)+pow(y,2)+pow(z,2));
        vector unitVector(x/magnitude,y/magnitude,z/magnitude);
        return unitVector;
    }
    float magnitude(){
        return sqrtf(pow(x,2)+pow(y,2)+pow(z,2));
    }
}vector;
typedef struct quadraple {
    float a;
    float x;
    float y;
    float z;
    quadraple(float a, float x, float y, float z){
        this-> a = a;
        this->x = x;
        this->y = y;
        this->z =z;
    }
    quadraple(){
        this-> a = 0;
        this->x = 0;
        this->y = 0;
        this->z =0;
    }
    std::string toString(){
        return "["+ std::to_string(a) + ", "+ std::to_string(x) + ", "+ std::to_string(y) + std::to_string(z)+"]" ;
    }
}quadraple;
typedef struct rgbColor {
    float r;
    float g;
    float b;
    rgbColor(float r, float g, float b){
        this->r = r;
        this->g = g;
        this->b =b;
    }
    rgbColor(){
        this->r = 1;
        this->g = 1;
        this->b =1;
    }
}rgbColor;
typedef struct square {
    float x;
    float y;
    float length;
    rgbColor color;
    square(float x, float y, float length, rgbColor color){
        this->x = x;
        this->y = y;
        this->length =length;
        this->color = color;
    }
}square;
typedef struct triangle {
    float x;
    float y;
    float length;
    float height;
    rgbColor color;
    triangle(float x, float y, float length, float height, rgbColor color){
        this->x = x;
        this->y = y;
        this->length =length;
        this->height =height;
        this->color = color;
    }
}triangle;
typedef struct character {
    square *sq;
    triangle *tri;
    float velocity;
    vector *translation;
    quadraple *rotation;
    character(square *sq, triangle *tri, float velocity, vector *translation, quadraple *rotation){
        this->sq = sq;
        this->tri = tri;
        this->velocity = velocity;
        this->translation = translation;
        this->rotation = rotation;
    }
    void setCharacterTranslation(vector toTranslate){
        translation->x = toTranslate.x;
        translation->y = toTranslate.y;
        translation->z = toTranslate.z;
    }
}character;
typedef struct playerStatus {
    int score;
    int multiplier;
    std::string specialMoveState;
    int specialMove; //0 for none, 1 for double score, 2 for chasers stun
    time_t specialMoveStartTime;
    playerStatus(){
        score = 0;
        multiplier = 1;
        specialMoveState = "NA";
        specialMoveStartTime = time(0);
    }
}playerStatus;
typedef struct gameStatus {
    std::string gameMode;
    int scorePoint;
    bool gameOver;
    gameStatus(std::string gameMode, int scorePoint){
        this->gameMode = gameMode;
        this->scorePoint = scorePoint;
        this->gameOver = false;
    }
}gameStatus;

//function signatures
void passM(int x,int y);
void keyUp(unsigned char k, int x,int y);
void drawEquTriangleR(triangle *tri);
void drawSquare(square *sq);
void drawCharacter(character *character);
void drawString (void * font, char *s, float x, float y, float z);
void drawStringOnScreenLogic();
void setCHaracterPosCenterScreen(character *character);
void setCharacterPosOrigin(character *character);
void gameMainLogic();
void chaserCaughtPlayer();
void playerCaughtChased();
void addToScore(int score);
void activateSpecialMoveMode(int move);
void endSpecialMoveMode();
void drawGameCharacters();
void characterOverlapLogic();
bool checkEaten();
bool checkAte();
void chaserCharacterRotationLogic(character *characterA, character *characterB);
void chasedCharacterRotationLogic(character *characterA, character *characterB);
void chaserCharacterTranslationLogic(character *characterA, character *characterB);
void chasedCharacterTranslationLogic(character *characterA, character *characterB);
bool characterOverlap(character *characterA, character *characterB);
void windowEnclosureLogic();
void endGame();
void Display(void);
void Anim(void);
//void glutFullScreen(void);

//global variables
int windowWidth = 1080;
int windowHeight = 768;
int passX=0; //used in main character translation (follows mouse position)
int passY=0; //used in main character translation (follows mouse position)
time_t startTime = time(0);
double seconds_since_start = 0.0;
float caughtMagnitude = 70;
double play_time = 0;

gameStatus gameStat("time_based", 10);
//gameStatus gameStat("one_shot_kill", 10);

playerStatus currPlayerStatus;

//main character
rgbColor mainTraingleColor(1,0,1);
triangle mainTriangle (50,0,50,40, mainTraingleColor);
rgbColor mainSquareColor(0,0,1);
square mainSquare(0.0f, 0.0f, 50.0f, mainSquareColor);
vector mainChacacterTranslation(passX-50, passY-90, 0);
quadraple mainChacacterRotation(0,0,0,1);
character mainChacacter(&mainSquare, &mainTriangle, 1, &mainChacacterTranslation, &mainChacacterRotation);

//chaser character
rgbColor chaserTraingleColor(1.0,0.2,0.2);
triangle chaserTriangle (50,0,50,40, chaserTraingleColor);
rgbColor chaserSquareColor(1.0,0.2,0.2);
square chaserSquare(0.0f, 0.0f, 50.0f, chaserSquareColor);
vector chaserChacacterTranslation(1, 1, 0);
quadraple chaserChacacterRotation(0,0,0,1);
character chaserChacacter(&chaserSquare, &chaserTriangle, 4, &chaserChacacterTranslation, &chaserChacacterRotation);
//chaser2 character
rgbColor chaser2TraingleColor(1.0,0.2,0.2);
triangle chaser2Triangle (50,0,50,40, chaser2TraingleColor);
rgbColor chaser2SquareColor(1.0,0.2,0.2);
square chaser2Square(0.0f, 0.0f, 50.0f, chaser2SquareColor);
vector chaser2ChacacterTranslation(0, 0, 0);
quadraple chaser2ChacacterRotation(0,0,0,1);
character chaser2Chacacter(&chaser2Square, &chaser2Triangle, 2, &chaser2ChacacterTranslation, &chaser2ChacacterRotation); // different velocity

int chasedX = windowWidth/2 -100;
int chasedY = windowHeight/2 -100;
rgbColor chasedTraingleColor(1,1,0);
triangle chasedTriangle (0+50,0,50,40, chasedTraingleColor);
rgbColor chasedSquareColor(0,1,0);
square chasedSquare(0, 0, 50.0f, chasedSquareColor);
vector chasedChacacterTranslation(0,0,0);
quadraple chasedChacacterRotation(0,0,0,1);
character chasedChacacter(&chasedSquare, &chasedTriangle,4, &chasedChacacterTranslation, &chasedChacacterRotation);

rgbColor chased2TraingleColor(1,1,0);
triangle chased2Triangle (0+50,0,50,40, chased2TraingleColor);
rgbColor chased2SquareColor(0,1,0);
square chased2Square(0, 0, 50.0f, chased2SquareColor);
vector chased2ChacacterTranslation(0, 0, 0);
quadraple chased2ChacacterRotation(0,0,0,1);
character chased2Chacacter(&chased2Square, &chased2Triangle,2, &chased2ChacacterTranslation, &chased2ChacacterRotation); //different velocity
int gameCount = 0;

//START CODE
void Display()
{
    glClear(GL_COLOR_BUFFER_BIT);//clear buffer/ memory if it's not cleared then expect anything to appear
    seconds_since_start = difftime( time(0), startTime);
    windowWidth = glutGet(GLUT_WINDOW_WIDTH);
    windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
    
    if(!gameStat.gameOver){
        if (gameCount==0) {
            setCHaracterPosCenterScreen(&chasedChacacter);
            setCHaracterPosCenterScreen(&chased2Chacacter);
            gameCount++;
        }
        play_time = seconds_since_start;
        drawGameCharacters();// draw characters
    }
    //TODO else game over display
    drawStringOnScreenLogic();
    glFlush();//to display everything on the screen
}

void Anim()
{
    if(!gameStat.gameOver){
        
        gameMainLogic();// game logic
        drawStringOnScreenLogic();
        glutPostRedisplay();
    }
}

void drawStringOnScreenLogic(){
    std::string onScreenString = "Score: ";
    onScreenString += std::to_string(currPlayerStatus.score);
    onScreenString += ", Time: ";
    if(gameStat.gameOver)
        onScreenString += std::to_string(play_time);
    else
        onScreenString += std::to_string(seconds_since_start);
    onScreenString += ", Special Move state: ";
    onScreenString += currPlayerStatus.specialMoveState;
    onScreenString += ", Game Mode: ";
    onScreenString += gameStat.gameMode;
    char *onScreenCharacters = new char[onScreenString.length() + 1];
    strcpy(onScreenCharacters, onScreenString.c_str());
    drawString(GLUT_BITMAP_HELVETICA_18, onScreenCharacters , 2, 2, 0);
    delete [] onScreenCharacters;
};

void drawGameCharacters(){
    //draw characters //order matter for layering
    drawCharacter(&chasedChacacter);//foe character chased
    drawCharacter(&chased2Chacacter);//foe character chased
    drawCharacter(&mainChacacter);
    drawCharacter(&chaserChacacter);//foe character chaser
    drawCharacter(&chaser2Chacacter);//foe character chaser
}

void gameMainLogic(){
    if(gameStat.gameMode=="time_based" && seconds_since_start>=300){
        endGame();
    }// if game mode is time based end game after 300 seconds (5 mins)
    if(seconds_since_start>=30 && currPlayerStatus.specialMoveState == "NA"){
        currPlayerStatus.specialMoveState = "ready";
    }// if 30 seconds have passed since start time, enable special moves
    if(currPlayerStatus.specialMoveState == "active" && difftime(time(0), currPlayerStatus.specialMoveStartTime)>=30){
        endSpecialMoveMode();
    }// if 30 seconds have passed since special move mode start time, end special move mode.
    //window blocking
    chaserCharacterRotationLogic(&chaserChacacter, &mainChacacter);
    chaserCharacterTranslationLogic(&chaserChacacter, &mainChacacter);
    chaserCharacterRotationLogic(&chaser2Chacacter, &mainChacacter);
    chaserCharacterTranslationLogic(&chaser2Chacacter, &mainChacacter);
    windowEnclosureLogic();
    
    chasedCharacterRotationLogic(&chasedChacacter, &mainChacacter);
    chasedCharacterTranslationLogic(&chasedChacacter, &mainChacacter);
    chasedCharacterRotationLogic(&chased2Chacacter, &mainChacacter);
    chasedCharacterTranslationLogic(&chased2Chacacter, &mainChacacter);
    
    
    characterOverlapLogic();
}

void windowEnclosureLogic(){
    if(chaserChacacter.tri->x + chaserChacacter.tri->height  +  chaserChacacter.translation-> x >= windowWidth -350){
        setCharacterPosOrigin(&chaserChacacter);
    }
    if(chaser2Chacacter.tri->x  + chaser2Chacacter.tri->height +  chaser2Chacacter.translation-> x >= windowWidth -350){
        setCharacterPosOrigin(&chaser2Chacacter);
    }
    if(chaserChacacter.tri->y + chaserChacacter.tri->length  +  chaserChacacter.translation-> y >= windowHeight-250){
        setCharacterPosOrigin(&chaserChacacter);
    }
    if(chaser2Chacacter.tri->y  + chaser2Chacacter.tri->length +  chaser2Chacacter.translation-> y >= windowHeight-250){
        setCharacterPosOrigin(&chaser2Chacacter);
    }
    
    if(chasedChacacter.tri->x + chasedChacacter.tri->height  +  chasedChacacter.translation-> x >= windowWidth -350){
        setCHaracterPosCenterScreen(&chasedChacacter);
    }
    if(chased2Chacacter.tri->x  + chased2Chacacter.tri->height +  chased2Chacacter.translation-> x >= windowWidth -350){
        setCHaracterPosCenterScreen(&chased2Chacacter);
    }
    if(chasedChacacter.tri->y + chasedChacacter.tri->length  +  chasedChacacter.translation-> y >= windowHeight-250){
        setCHaracterPosCenterScreen(&chasedChacacter);
    }
    if(chased2Chacacter.tri->y  + chased2Chacacter.tri->length +  chased2Chacacter.translation-> y >= windowHeight-250){
        setCHaracterPosCenterScreen(&chased2Chacacter);
    }
    
    if(chasedChacacter.tri->x + chasedChacacter.tri->height  +  chasedChacacter.translation-> x <= 0){
        setCHaracterPosCenterScreen(&chasedChacacter);
    }
    if(chased2Chacacter.tri->x  + chased2Chacacter.tri->height +  chased2Chacacter.translation-> x <= 0){
        setCHaracterPosCenterScreen(&chased2Chacacter);
    }
    if(chasedChacacter.tri->y + chasedChacacter.tri->length  +  chasedChacacter.translation-> y <= 0){
        setCHaracterPosCenterScreen(&chasedChacacter);
    }
    if(chased2Chacacter.tri->y  + chased2Chacacter.tri->length +  chased2Chacacter.translation-> y <= 0){
        setCHaracterPosCenterScreen(&chased2Chacacter);
    }
    //end window blocking;
}

void characterOverlapLogic(){
    if(checkEaten()){
        if(gameStat.gameMode=="one_shot_kill"){
            endGame();
        }
        std::cout << "eaten" << std::endl;
        //BONUS SOUND HERE
        //        PlaySound(TEXT("winlosesound.wav"), NULL, SND_ASYNC|SND_FILENAME);
        chaserCaughtPlayer();
    }
    if(checkAte()){
        if(gameStat.gameMode=="one_shot_kill"){
            endGame();
        }
        std::cout << "ate" << std::endl;
        //BONUS SOUND HERE
        //        PlaySound(TEXT("winlosesound.wav"), NULL, SND_ASYNC|SND_FILENAME);
        playerCaughtChased();
    }
}

void chaserCharacterRotationLogic(character *characterA, character *characterB){
    float y2MinusY1 = (characterB->sq->y+characterB->translation->y)-(characterA->sq->y+characterA->translation->y);
    float x2MinusX1 = (characterB->sq->x+characterB->translation->x)-(characterA->sq->x+characterA->translation->x);
    float angle_bet_chaser_main = atan (y2MinusY1/x2MinusX1)*toDeg;
    if(currPlayerStatus.specialMove==2){
        return;
    }
    if (characterA->sq->x+characterA->translation->x<characterB->sq->x+characterB->translation->x) {
        characterA->rotation->a = angle_bet_chaser_main;
    }else{
        characterA->rotation->a = angle_bet_chaser_main+180;
    }
}
void chaserCharacterTranslationLogic(character *characterA, character *characterB){
    float y2MinusY1 = (characterB->sq->y+characterB->translation->y)-(characterA->sq->y+characterA->translation->y);
    float x2MinusX1 = (characterB->sq->x+characterB->translation->x)-(characterA->sq->x+characterA->translation->x);
    vector vector_bet_A_B(x2MinusX1, y2MinusY1, 0);
    vector unit_vector_bet_A_B = vector_bet_A_B.unitVector();
    float step =characterA->velocity;
    if(currPlayerStatus.specialMove == 2){ //if chaser stun is working make step 0
        step=0;
    }
    vector transVector((characterA->translation->x+unit_vector_bet_A_B.x*step),(characterA->translation->y+unit_vector_bet_A_B.y*step), (characterA->translation->z+unit_vector_bet_A_B.z*step));
    characterA->setCharacterTranslation(transVector);
    
}

void chasedCharacterRotationLogic(character *characterA, character *characterB){
    float y2MinusY1 = (characterB->sq->y+characterB->translation->y)-(characterA->sq->y+characterA->translation->y);
    float x2MinusX1 = (characterB->sq->x+characterB->translation->x)-(characterA->sq->x+characterA->translation->x);
    float angle_bet_chaser_main = atan (y2MinusY1/x2MinusX1)*toDeg;
    if (characterA->sq->x+characterA->translation->x<characterB->sq->x+characterB->translation->x) {
        characterA->rotation->a = angle_bet_chaser_main+180;
    }else{
        characterA->rotation->a = angle_bet_chaser_main;
    }
}
void chasedCharacterTranslationLogic(character *characterA, character *characterB){
    float y2MinusY1 = ((characterA->sq->y+characterA->translation->y)-(characterB->sq->y+characterB->translation->y));
    float x2MinusX1 = (characterA->sq->x+characterA->translation->x)-(mainChacacter.sq->x+characterB->translation->x);
    
    vector dir(x2MinusX1, y2MinusY1, 0);
    vector unit_vector = dir.unitVector();
    
    characterA->translation->x = characterA->translation->x+(unit_vector.x*characterA->velocity);
    characterA->translation->y = characterA->translation->y+(unit_vector.y*characterA->velocity);
    
}

bool checkEaten(){
    if(characterOverlap(&chaserChacacter, &mainChacacter))
    {
        setCharacterPosOrigin(&chaserChacacter);
        return true;
    }
    if (characterOverlap(&chaser2Chacacter, &mainChacacter)) {
        setCharacterPosOrigin(&chaser2Chacacter);
        return true;
    }
    return false;
}
bool checkAte(){
    if(characterOverlap(&mainChacacter, &chasedChacacter))
    {
        setCHaracterPosCenterScreen(&chasedChacacter);
        return true;
    }
    if (characterOverlap(&mainChacacter, &chased2Chacacter)) {
        setCHaracterPosCenterScreen(&chased2Chacacter);
        return true;
    }
    return false;
}

void chaserCaughtPlayer(){
    addToScore(-1*gameStat.scorePoint);
}
void playerCaughtChased(){
    addToScore(gameStat.scorePoint);
}

// mutatung Functions
void setCHaracterPosCenterScreen(character *character){
    character->translation->x = chasedX;
    character->translation->y = chasedY;
    character->translation->z = 0;
}
void setCharacterPosOrigin(character *character){
    character->tri->x = 50;
    character->tri->y = 0;
    character->sq->x = 0;
    character->sq->y = 0;
    character->translation->x = 0;
    character->translation->y = 0;
    character->translation->z = 0;
}
void addToScore(int score){
    currPlayerStatus.score += score*currPlayerStatus.multiplier;
}
void activateSpecialMoveMode(int move){
    std::cout << "trying to specl move" << std::endl;
    if(currPlayerStatus.specialMoveState=="ready"){
        currPlayerStatus.specialMoveState = "active";
        currPlayerStatus.specialMoveStartTime = time(0);
        std::cout << currPlayerStatus.specialMoveState << std::endl;
        drawStringOnScreenLogic();
        std::cout << "activatedSpecialMoveMode" << std::endl;
        currPlayerStatus.specialMove = move;
        switch (move) {
            case 1:
                currPlayerStatus.multiplier=2;
                break;
            case 2:
                currPlayerStatus.multiplier=1;
                break;
            default: std::cout << "Error in activateSpecialMoveMode, move unknown" << std::endl;
                break;
        }
    }
}
void endSpecialMoveMode(){
    currPlayerStatus.specialMoveState = "used";
    currPlayerStatus.specialMove = 0;
    currPlayerStatus.multiplier=1;
}
bool characterOverlap(character *characterA, character *characterB){
    float y2MinusY1 = (characterB->sq->y+characterB->translation->y)-(characterA->sq->y+characterA->translation->y);
    float x2MinusX1 = (characterB->sq->x+characterB->translation->x)-(characterA->sq->x+characterA->translation->x);
    vector vector_bet_A_B(x2MinusX1, y2MinusY1, 0);
    vector unit_vector_bet_A_B = vector_bet_A_B.unitVector();
    if (vector_bet_A_B.magnitude()<caughtMagnitude){
        std::cout <<  "overlap" << std::endl;
        return true;
    }else
        return false;
}
void endGame(){
    gameStat.gameOver = true;
    glutReshapeWindow(windowWidth,windowHeight);
    std::cout << "Game ended" << std::endl;
}

////Generic Functions, helpers
void drawEquTriangleR(triangle *tri){
    glPushMatrix();
    glBegin(GL_POLYGON);
    glColor3f(tri->color.r, tri->color.g, tri->color.b);
    glVertex3f(tri->x,tri->y+tri->length ,0);
    glVertex3f((tri->x+tri->height), tri->y+tri->length/2 ,0);
    glVertex3f(tri->x,tri->y ,0);
    glEnd();
    glPopMatrix();
}

void drawSquare(square *sq){
    glPushMatrix();// square shape matrix
    glBegin(GL_QUADS);
    glColor3f(sq->color.r, sq->color.g, sq->color.b);
    glVertex3f(sq->x, sq->y, 0.0f);
    glVertex3f(sq->x, sq->y+sq->length, 0.0f);
    glVertex3f(sq->x+sq->length, sq->y+sq->length, 0.0f);
    glVertex3f(sq->x+sq->length, sq->y, 0.0f);
    glEnd();
    glPopMatrix();
}

void drawCharacter(character *character){
    glPushMatrix();// main shape matrix
    glTranslated(character->translation->x,character->translation->y,character->translation->z);
    glRotated(character->rotation->a, character->rotation->x,character->rotation->y,character->rotation->z);
    drawSquare(character->sq);
    drawEquTriangleR(character->tri);
    glPopMatrix();
}
void drawString (void * font, char *s, float x, float y, float z){
    unsigned int i;
    glRasterPos3f(x, y, z);
    for (i = 0; i < strlen (s); i++)
        glutBitmapCharacter (font, s[i]);
}

//GL helper functions
//passive motion function takes 2 parameters the x and y positions of the mouse, it allows the user to make use of the mouse motion without clicking on the mouse buttons
void passM(int x,int y)
{
    float opposite = x- passX;
    float adjacent = windowHeight - passY - y;
    float theta = atan2f(adjacent, opposite) * toDeg;
    mainChacacter.rotation->a = theta;
    passX=x;
    passY=windowHeight-y;
    //set Main character translation position
    mainChacacterTranslation.x = passX;
    mainChacacterTranslation.y = passY;
    glutPostRedisplay();
}
void keyUp(unsigned char k, int x,int y)//keyboard up function is called whenever the keyboard key is released
{
    if(k=='q')//if the letter q is pressed, then .
        activateSpecialMoveMode(1);
    if(k=='w')//if the letter w is pressed, then .
        activateSpecialMoveMode(2);
    if(k=='m')
        gameStat.gameMode="one_shot_kill";
    if(k=='z')
        endGame();
    if(k==27)
        endGame();
    glutPostRedisplay();//redisplay to update the screen with the changed
}

//Main
int main(int argc,char** argr)
{
    glutInit(&argc,argr); //used to initialze the glut library
    glutInitWindowSize(windowWidth,windowHeight);
    glutInitWindowPosition(0,0);// initial position from screen left top corner
    glutCreateWindow("Tokyo Graphics1");//the name of the window
    glutDisplayFunc(Display);//setting display function
    glClearColor(1.0f,1.0f,1.0f,0.0f); //the background color or the default color RGBA, to get the color choose , divide /255 and write the value
    glutPassiveMotionFunc(passM); // call passive motion function for mouse movements
    glutKeyboardUpFunc(keyUp);		//call the keyboard up function
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB); //using The RGBA model and using a signle buffer
    gluOrtho2D(0.0,1000.0,0.0,600.0);//type of viewing here its orthographic prespective (left , right , top , bottom)* to be checked
    glutIdleFunc(Anim);
    glutFullScreen();
    glutSetCursor(GLUT_CURSOR_NONE);
    glutMainLoop();//this will let the screen behave as glut and not to be closed as normal c++
}
//END CODE
