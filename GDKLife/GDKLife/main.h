
#ifndef MAINH
#define MAINH
//#define VIS
//#define COL
#include<DarkGDK.h>
#include<iostream>
#include<stdio.h>


//=============================================================================================================================
//============================MOSTLY CONTANTS THAT EFFECT THE WAY THE CREATURES BREED AND ARE CREATED =============================================================================



#define MAZE_SIZE 1000

#define SCREENX 640//1024
#define SCREENY 400//800 

#define MAX_AGE  100
#define MAX_HEALTH 200
#define MAX_CREATURES 5000
#define WORLD_WIDTH SCREENX  
#define WORLD_HEIGHT SCREENY
#define LEFT_BOUNDS  0.0f
#define TOP_BOUNDS  0.0f

#define MALE  0
#define FEMALE 1
#define _PI 3.14159
#define NUM_NEURONS 3
#define NUM_RESPONSES 4

#define NORTHING 0
#define ENEMY 1
#define MATE 2
#define WALL 3

#define MUTATION_RATE ((rand()%101)-50)

#define TRUE 1
#define FALSE 0

#define AMOUNT_FOOD 10000




//=================================== THE CREATURE AND WORLD CLASSES THAT HOLD ALL THE METHODS AND DATA ===================================================================
//=============================================================================================================================


class WORLD{
	int TOP      , LEFT;
	int WIDTH    , HEIGHT;
	int CAMERA_X , CAMERA_Y;
	int SCREEN_X , SCREEN_Y;
	int VIEW_X   , VIEW_Y;

	void DRAW_SCALE(float);
};

class CREATURE{
       public:
        	   
        	 DWORD COLOR;
        	 int SPEED, 
				   AGE, 
			    HEALTH;

			 int LAST_BREED,
				  LAST_MEAL;
        	 int ID;
			 int COLLISION_ID;
			 int SIGHT_ID;
			 float POTENTIAL_X, POTENTIAL_Y;
        	 float X,Y,ANGLE;

        	 float FOV, SIGHT_DISTANCE;

			 bool EXIST;
        	 bool SEX;

        	 CREATURE::CREATURE();
        	 CREATURE::CREATURE(int parent, int x,int y, int speed,float angle,bool sex,DWORD color,int FOV,int SIGHT_DISTANCE);
			 //CREATURE::CREATURE(         ID,   NX,   NY, int speed,float angle,bool sex,DWORD color,int fov,int sight)
        	 CREATURE::~CREATURE();
        	 
             void MOVE(float SPEED);
             void DRAW();
             void ROTATE(float CHANGE);
             void KILL();
			 
			 int  SIGHT();
             int  HIT();
             int  BREED(CREATURE OTHER);
             
             void SET_POSITION();
             void RANDOM_MOVE();
             void SIGHT_HANDLER(int SIGHT);
             
			 static float WRAPX(float INPUT);
             static float WRAPY(float INPUT);
			 
			 int BRAIN[NUM_NEURONS][NUM_RESPONSES];

        private:
        
             float CREATURE::NEWX(float X, float ANGLE,float DIST);
             float CREATURE::NEWY(float Y, float ANGLE,float DIST);
             
             void SET_X(float x);
             void SET_Y(float y);
             void SET_ANGLE(float angle);
             
             void WRAPX();
             void WRAPY();
			 void SLIDING_COLLISION();
			    
};

//=============================================================================================================================

//=================== GLOBAL VARIABLES ========================================================================================

float WX = LEFT_BOUNDS + (WORLD_WIDTH);
float WY = TOP_BOUNDS  + (WORLD_HEIGHT);

unsigned long AVG_BRAIN1,
	AVG_BRAIN2,
	AVG_BRAIN3,
	AVG_BRAIN4,
    AVG_HEALTH;

int FPS, DAYS;

int MAIN_LOOP_SPEED;
int NUM_OF_COLLISIONS;
int NUM_CREATURES;

int DEATHS,    BIRTHS;
int CAMERA_X, CAMERA_Y;

int WORLD[SCREENX][SCREENY] = {0};

CREATURE CREATURES[MAX_CREATURES] = {}; 

float _COS[360], _SIN[360];

void   _MOVE_CREATURES    (void);
void   _CONTROL_CREATURE  (void);
void   _PRINT_STATS(void);
int    _CHECK_SPACE       (int X, int Y);

bool SHOW_VISION, SHOW_COLLISION;

int FAILS;  /// I JUST ADDED THIS TO COUNT HOW MANY TIMES IT ATTEMPTED TO PLACE A CREATURE AND FAILED BECAUSE THERE WAS ALREADY ONE THERE. ITS MY SAD ATTEMPT AT DEBUGGING WHY THEY ARE STACKING ON TOP OF EACH OTHER
//==================================================================================================

/*
 void SLIDING_COLLISION(){
    if(X >                           // FIRST VERSION i DID LIKE THIS i USED 3D AND COULD HANDLE AROUND 50-100 CREATURES AND IT USED SLIDING COLLISION SO SINCE THAT WAS BETTER i THOUGH i WOULD SIMULATE THAT SAME THING EHRE IN 2D, JUST STARTED IT WHEN i BEGAN TALKING TO  YALL.


 }*/
//===================== CREATURE CLASS METHODS=======================================================


// //=================================== THIS IS MY COLLISION DETECTION, ITS USING COS AND SIN DOING PIXEL LEVEL COLLISION DETECTION ===========================================================================
//======================================BECAUSE i EVENTIALLY WANT TO MAKE THE CREATURES HAVE LARGER SIZES. =======================================================================

int CREATURE::HIT(){

	int NX = X,NY = Y;
	float NEW_ANGLE = ANGLE - 90.0;
	float DIST = 1.0;
	float XX,YY;
	int VALUE = WORLD[int(POTENTIAL_X)][int(POTENTIAL_Y)];
	if(VALUE != 0 && VALUE != ID){
		ANGLE += 180;
		if (VALUE == -2){
			WORLD[int(POTENTIAL_X)][int(POTENTIAL_Y)] = 0;
			int *V = &WORLD[int(rand()%WORLD_WIDTH)][int(rand()%WORLD_HEIGHT)];
			if(*V == 0)*V = -2;
			HEALTH += 50;
			LAST_MEAL = 0;
		}
		
		else{
			return WORLD[int(POTENTIAL_X)][int(POTENTIAL_Y)];}
	}
return 0;
}



void CREATURE::MOVE(float SPEED){
	float RAD = ANGLE * _PI / 180;
    float NX = WRAPX(X + SPEED * cos(RAD));
    float NY = WRAPY(Y + SPEED * sin(RAD));
	POTENTIAL_X = NX;
	POTENTIAL_Y = NY;
}


//========================BASIC DESTRUCTOR. i DID IT LIKE THIS BECAUSE i WAS HAVING ISSUES EARLY ON AND NEVER CHANGED IT BACK TO NORMAL ========================================================================================
CREATURE::CREATURE(){
	ANGLE = SPEED = COLOR = HEALTH = FOV = SIGHT_DISTANCE = AGE = NULL;
};


//=================== BASIC CONSTRUCTOR ANY QUESTIONS JUST ASK ==========================================================================================

//CREATURE(int &parent, int &x,int &y, int &speed,float &angle,bool &sex,DWORD &color,int &FOV,int &SIGHT_DISTANCE);
CREATURE::CREATURE(int parent, int x,int y, int speed,float angle,bool sex,DWORD color,int fov,int sight){
	EXIST = FALSE;
//===============================FINDS A FREE CREATURE ID IN THE ARRAY ======================================================================================
	if(NUM_CREATURES >= MAX_CREATURES)  return;  // CHECK TO MAKE SURE ITS NOT OUT OF RANGE

//==============================================================================================================================================================
//======================THIS IS WHAT SHOULD PREVENT THE FROM STACKING OR ATLEAST ONE OF THE CHECKS THERE IS MORE THEN JUST THIS========================================================================

	if(WORLD[(int)x][(int)y] != 0) {    
		FAILS++;
		return;  // CHECK TO MAKE SURE CREATURE IS NOT ALREADY THERE
	}
//=============================================================================================================================
//==============THIS CHECKS TO SEE IF MAX AMOUNT OF CREATUERS ALREADY EXIST, IF SO THEN DONT MAKE ANY MORE... IT SEEMS TO WORK FINE ================================================================================================
	
	for(ID = 0;ID < MAX_CREATURES;ID++){
		if(CREATURES[ID].EXIST == FALSE){
			break;
		} // Check to find free ID
	}
//=============================================================================================================================
	
	 POTENTIAL_X = WRAPX(x),POTENTIAL_Y = WRAPY(y);
	 X=POTENTIAL_X;
 	 Y=POTENTIAL_Y;
	 SEX = sex;
     ANGLE = angle;
     SPEED = speed;
     COLOR = color ;
     HEALTH = 1000;
	 FOV = fov;
	 SIGHT_DISTANCE = sight;
     AGE = 1;
	 LAST_BREED = 0;
	 LAST_MEAL = 0;
	 for(int SEES_OBJECT = 0;SEES_OBJECT < NUM_NEURONS;SEES_OBJECT++){
		 for(int RESPONSE = 0;RESPONSE < NUM_RESPONSES;RESPONSE++){

			 if (parent != 0){
				  BRAIN[SEES_OBJECT][RESPONSE] = CREATURES[parent].BRAIN[SEES_OBJECT][RESPONSE] + ((rand() % 2001) - 1000);
			 }else{
				 BRAIN[SEES_OBJECT][RESPONSE] = rand() % 100000; //(rand()%100000001); 
			 }
		 }
	 }
     
	 EXIST = TRUE;	 //// THIS IS HOW I DETERMINE IF ITS ALIVE OR DEAD
	 NUM_CREATURES++;
};

CREATURE::~CREATURE(){};


void CREATURE::DRAW(){dbDot(X,Y,COLOR);}
void CREATURE::ROTATE(float CHANGE){ANGLE = dbWrapValue(ANGLE + CHANGE);}
//==================THE SIGHT IS A RAY CASTER THAT SCANS ITS FIELD OF VISION IF THE RAY HITS SOMETHING IT RETURNS THE NUMBER OF THE CRETURE OR OBJECT IT SEES ============================================================================================

int CREATURE::SIGHT(){
	float XX,YY,x1,y1;
	float ANG;
	 x1=X;
	 y1=Y;
	 float Theta;
	
    	for(float  DIST = 2; DIST < (float)SIGHT_DISTANCE; DIST++ ){
			Theta = (360 / ( 8 * DIST));
			for(float A  = -FOV; A < FOV; A+=Theta){	
    	    ANG = dbWrapValue(ANGLE + A);
			XX = x1 + DIST * cos(ANG * _PI / 180);
			YY = y1 + DIST * sin(ANG * _PI / 180);
			XX = WRAPX(XX);
			YY = WRAPY(YY);
	
			if(SHOW_VISION == TRUE)dbDot(XX, YY, dbRGB(70,70,70));;

			int VALUE = WORLD[(int)XX][(int)YY];
			if(VALUE !=  0)return VALUE;}}
return 0;
}



//=============i QUESTION THIS METHOD BUT IF TWO CREATUERS OF OPPOSITE SEX COLLIDE IT CALLS BREED AND MAKES A NEW CREATURE THAT SHARES PROPERTIES OF THE PARENT THAT INITIATED THE CONTACT=================================================================================================

int CREATURE::BREED(CREATURE OTHER){
 int NX=0,NY=0;
		if(NUM_CREATURES >= MAX_CREATURES)return 0;
	       
		    BIRTHS++;
            NX = WRAPX(CREATURES[ID].X  + (rand()%7) - 3); 
			NY = WRAPY(CREATURES[ID].Y  + (rand()%7) - 3); 

	       CREATURE CNEW;

		   if (WORLD[NX][NY] ==0 )
		   {
			//CREATURE(ID, NX,NY, int speed,float angle,bool sex,DWORD color,int fov,int sight){
			   int colormr = 2;
int newcol = COLOR ;/// + (rand()%10000-5000);//OTHER.COLOR / 2 ;//dbRGB((dbRGBR(COLOR) + dbRGBR(OTHER.COLOR) / 2)  + ((rand()%colormr) - (colormr / 2)),(dbRGBG(COLOR) + dbRGBG(OTHER.COLOR) / 2)+ ((rand()%colormr) - (colormr / 2)),(dbRGBB(COLOR) + dbRGBB(OTHER.COLOR) / 2) + ((rand()%colormr) - (colormr / 2)));
			    


	 	   CNEW = CREATURE(ID, NX, NY, rand()%6, rand()%360, rand() % 2, newcol , FOV + (rand()%3-1), SIGHT_DISTANCE + (rand()%3-1));
		   CREATURES[CNEW.ID] = CNEW;
		   }
		   else{return 0;}
  

return CNEW.ID;}


void CREATURE::KILL(){

	WORLD[(int)X][(int)Y] = 0;
	EXIST = 0;

	X = Y =  ANGLE = SPEED = COLOR = HEALTH = FOV = SIGHT_DISTANCE = AGE = 0;

	for(int SEES_OBJECT = 0;SEES_OBJECT < NUM_NEURONS;SEES_OBJECT++){
	    for(int RESPONSE = 0;RESPONSE < NUM_RESPONSES;RESPONSE++){
			  BRAIN[SEES_OBJECT][RESPONSE] = 0;}}
	    
	NUM_CREATURES--;
	DEATHS++;
}


void CREATURE::RANDOM_MOVE(){
	X = rand() % 5;
	Y = rand() % 5;

	ANGLE += rand() % 5;
	this->MOVE(1);
}

//===============THIS IS THE BRAINS OF THE AI i CAN EXPLAIN MORE IN DETAIL JUST ASK =========================================================================================
//===========IT WORKS VERY WELL AND I HAVE USED IT MULTIPLE TIMES FOR AI ITS PROBABILITY BASED ======================================================================================================

void CREATURE::SIGHT_HANDLER(int SIGHT){
int TURN_SPEED = 15;
int TRIGGER;
							  
		TRIGGER = (rand() % 100001);if( BRAIN[SIGHT][1] >= TRIGGER) {ROTATE(-TURN_SPEED);}
		TRIGGER = (rand() % 100001);if( BRAIN[SIGHT][2] >= TRIGGER) {ROTATE(TURN_SPEED);}
		TRIGGER = (rand() % 100001);if( BRAIN[SIGHT][3] >= TRIGGER) {MOVE(1);}
		TRIGGER = (rand() % 100001);if( BRAIN[SIGHT][4] >= TRIGGER) {MOVE(-1);}

return;
}
//=============================================================================================================================
//=============================================================================================================================


void CREATURE::SET_POSITION(){
	X = WRAPX(POTENTIAL_X);
	Y = WRAPY(POTENTIAL_Y);
	ANGLE = dbWrapValue(ANGLE);
	WORLD[(int)X][(int)Y] = ID;
return;
}


float CREATURE::NEWX(float x, float A,float DIST){
    float NX = x + DIST * cos(A * 3.14159 / 180) ;
	return NX;
}

float CREATURE::NEWY(float y,float A,float DIST){
    float NY = y + DIST * sin(A * 3.14159 / 180);
return NY;
}


void CREATURE::WRAPX(){
	if (X < LEFT_BOUNDS) X = (float)SCREENX + (int)X;
	if (X > WORLD_WIDTH) X = X - (WORLD_WIDTH);
	}

void CREATURE::WRAPY(){
	if (Y < TOP_BOUNDS) Y = (float)SCREENY + (int)Y; 
	if (Y > WORLD_HEIGHT) Y = Y - (WORLD_HEIGHT);
}

float CREATURE::WRAPX(float INPUT){
	if (INPUT < LEFT_BOUNDS) INPUT = (float)WX + INPUT;
	if (INPUT >= (float)WORLD_WIDTH) INPUT = INPUT - ((float)WORLD_WIDTH);
return INPUT;
}

float CREATURE::WRAPY(float INPUT){
	if (INPUT < TOP_BOUNDS) INPUT = (float)WY + INPUT;
	if (INPUT >= (float)WORLD_HEIGHT) INPUT = INPUT - ((float)WORLD_HEIGHT);
return INPUT;
}

void CREATURE::SET_X(float x){X=x;}
void CREATURE::SET_Y(float y){Y=y;}
void CREATURE::SET_ANGLE(float angle){ANGLE=angle;}

#endif




/*
	if(ANGLE > 0 && ANGLE < 45){
		while(_CHECK_SPACE(XX,YY) != 0)
		{
			YY--;
			
		}
	}
	else if(ANGLE > 45 && ANGLE < 90){
		while(_CHECK_SPACE(XX,YY) != 0)
		{
			YY++;
		}
	}
	else if(ANGLE > 90 && ANGLE < 180){
		while(_CHECK_SPACE(XX,YY) != 0)
		{
			XX++;
		}
	}
	else if(ANGLE > 180 && ANGLE < 360){
		while(_CHECK_SPACE(XX,YY) != 0)
		{
			XX--;
		}
	}


	X = WRAPX(XX);
	Y = WRAPY(YY);*/
//  CHECK = _CHECK_SPACE(X,Y+1);	  if( CHECK != 0)  return CHECK;
//  CHECK = _CHECK_SPACE(X-1,Y);      if( CHECK != 0)  return CHECK;
//  CHECK = _CHECK_SPACE(X-1,Y+1);    if( CHECK != 0)  return CHECK;
//  CHECK = _CHECK_SPACE(X-1,Y-1);    if( CHECK != 0)  return CHECK;
//  CHECK = _CHECK_SPACE(X+1,Y);      if( CHECK != 0)  return CHECK;
//  CHECK = _CHECK_SPACE(X+1,Y+1);    if( CHECK != 0)  return CHECK;  
//  CHECK = _CHECK_SPACE(X+1,Y-1);    if( CHECK != 0)  return CHECK;  
//  CHECK = _CHECK_SPACE(X,Y-1);      if( CHECK != 0)  return CHECK;



/*	int  CHECK;
	int NX = X,NY = Y;

    CHECK = _CHECK_SPACE(NX  ,NY+1);	  if( CHECK != 0)  return CHECK;
#ifdef COL
	dbDot(NX  ,NY+1,dbRGB(255,0,0)); 
#endif
    CHECK = _CHECK_SPACE(NX-1,NY  );    if( CHECK != 0)  return CHECK;
#ifdef COL
	COL   dbDot(NX-1,NY  ,dbRGB(255,0,0));  
#endif   
    CHECK = _CHECK_SPACE(NX-1,NY+1);    if( CHECK != 0)  return CHECK;
#ifdef	COL
    dbDot(NX-1,NY+1,dbRGB(255,0,0));  
#endif
    CHECK = _CHECK_SPACE(NX-1,NY-1);    if( CHECK != 0)  return CHECK;
#ifdef COL
    dbDot(NX-1,NY-1,dbRGB(255,0,0)); 
#endif 
    CHECK = _CHECK_SPACE(NX+1,NY  );    if( CHECK != 0)  return CHECK;
#ifdef COL
    dbDot(NX+1,NY  ,dbRGB(255,0,0));  
#endif 
    CHECK = _CHECK_SPACE(NX+1,NY+1);    if( CHECK != 0)  return CHECK; 
#ifdef COL
	dbDot(NX+1,NY+1,dbRGB(255,0,0)); 
#endif 
    CHECK = _CHECK_SPACE(NX+1,NY-1);    if( CHECK != 0)  return CHECK; 
#ifdef COL
   dbDot(NX+1,NY-1,dbRGB(255,0,0));
#endif 
    CHECK = _CHECK_SPACE(NX  ,NY-1);    if( CHECK != 0)  return CHECK;
#ifdef COL
  dbDot(NX  ,NY-1,dbRGB(255,0,0)); 
#endif*/

/*
    WORLD[int(NX)][int(NY)] = 0;
	for(float A = ANGLE;A < ANGLE + 180;A+=40){
		XX = X + DIST * cos(dbWrapValue(A) * 3.14159 / 180);
		YY = Y + DIST * sin(dbWrapValue(A) * 3.14159 / 180);

		if(SHOW_COLLISION == TRUE)dbDot(XX, YY, dbRGB(255,0,0));;

		int HIT = WORLD[(int)WRAPX(XX)][(int)WRAPY(YY)];
		if(HIT != 0 &&  HIT != ID) return HIT;
	}*/