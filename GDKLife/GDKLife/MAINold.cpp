#include<DarkGDK.h>
#include<iostream>

#include"main.h"

CREATURE::CREATURE(){};

CREATURE::CREATURE(int x,int y, int speed,float angle){
     X = X  + (rand()%3) - 1;  
     Y = Y  + (rand()%3) - 1;
	 WRAPX();
	 WRAPY();
     ANGLE = angle;
     SPEED = SPEED;
     COLOR = COLOR + ((rand()% 1000) - 500);
     HEALTH = 1000;
	 FOV = 30;
	 SIGHT_DISTANCE = 10;
};

CREATURE::~CREATURE(){};

void CREATURE::MOVE(float SPEED){
    X = _WRAPX(X + SPEED * cos(ANGLE));
    Y = _WRAPY(Y + SPEED * sin(ANGLE));
}

void CREATURE::DRAW(){dbDot(X,Y,COLOR);}
void CREATURE::ROTATE(float CHANGE){ANGLE = dbWrapValue(ANGLE + CHANGE);}

int CREATURE::SIGHT(){
	float XX,YY;
	int ANG;
	for(int A  = -FOV; A < FOV; A += 10){	
    	for(int DIST = 2; DIST < SIGHT_DISTANCE; DIST++){
    	    ANG = (int)dbWrapValue(ANGLE + A);
			XX = X + DIST * _COS[ANG];
			YY = Y + DIST * _SIN[ANG];
			XX = WRAPX(XX);
			YY = WRAPY(YY);
			int VALUE = WORLD[(int)XX][(int)YY];
			if(VALUE !=  0)return VALUE;}}
return 0;
}

void CREATURE::BREED(int OTHER){

	if(NUM_CREATURES >= MAX_CREATURES) return;
	    BIRTHS++;
		CREATURE *CNEW;

        CNEW->X = X  + (rand()%3) - 1;  
        CNEW->Y = Y  + (rand()%3) - 1;
		CNEW->WRAPX();
		CNEW->WRAPY();
        CNEW->ANGLE = rand()%360;
        CNEW->SPEED = rand()%6;
        CNEW->COLOR = COLOR + ((rand()% 1000) - 500);
        CNEW->HEALTH = 1000;
        //int COUNT = _MAKE_CREATURE(X,Y, 1000, ANGLE,(rand() % 2),SPEED,COLOR);

		for(int SEES_OBJECT = 0;SEES_OBJECT < NUM_NEURONS;SEES_OBJECT++){
		   for(int RESPONSE = 0;RESPONSE < NUM_RESPONSES;RESPONSE++){
			 CNEW->BRAIN[SEES_OBJECT][RESPONSE] = rand()%10000; }}
}


void CREATURE::KILL(){
	AGE  = 0;
	COLOR = 0;
	SEX = 0;
	HEALTH = 0;
	EXIST = 0;
		
	WORLD[(int)X][(int)Y] = 0;

	for(int SEES_OBJECT = 0;SEES_OBJECT < NUM_NEURONS;SEES_OBJECT++){
	    for(int RESPONSE = 0;RESPONSE < NUM_RESPONSES;RESPONSE++){
			  BRAIN[SEES_OBJECT][RESPONSE] = 0;}}

	NUM_CREATURES--;
	DEATHS++;
}


void CREATURE::WRAPX(){
	if (X < LEFT_BOUNDS) X = WX + (int)X;
	if (X > WORLD_WIDTH) X = X - (WORLD_WIDTH);
	}

void CREATURE::WRAPY(){
	if (Y < TOP_BOUNDS) Y = WY + (int)Y; 
	if (Y > WORLD_HEIGHT) Y = Y - (WORLD_HEIGHT);
}

float CREATURE::WRAPX(float INPUT){
	if (INPUT < LEFT_BOUNDS) INPUT = WX + (int)INPUT;
	if (INPUT > WORLD_WIDTH) INPUT = INPUT - (WORLD_WIDTH);
return INPUT;
}

float CREATURE::WRAPY(float INPUT){
	if (INPUT < TOP_BOUNDS) INPUT = WY + (int)INPUT;
	if (INPUT > WORLD_HEIGHT) INPUT = INPUT - (WORLD_HEIGHT);
return INPUT;
}

void CREATURE::SET_X(float x){X= x;}
void CREATURE::SET_Y(float y){Y= y;}
void CREATURE::SET_ANGLE(float angle){ANGLE = angle;}






using namespace std;

void DarkGDK()
{
	for(int WTF = 0;WTF < MAX_CREATURES;WTF++){CREATURES[WTF].EXIST = 0;}
	


	for(int ANGLE = 0; ANGLE < 360;ANGLE++){
        _COS[ANGLE] = cos((float)ANGLE);
        _SIN[ANGLE] = sin((float)ANGLE);
    }
	  
	int ID;

	for(int count = 0;count < MAX_CREATURES / 2;count++){
		NUM_CREATURES++;
          
        CREATURES[count].X = rand() % WORLD_HEIGHT;
        CREATURES[count].Y = rand() % WORLD_WIDTH;
        CREATURES[count].HEALTH = rand() % MAX_HEALTH ;
        CREATURES[count].ANGLE =  rand() % 360;
        CREATURES[count].COLOR = dbRgb(rand() % 255,rand() % 255,rand() % 255);
        
        CREATURES[count].EXIST  = 1;
        CREATURES[count].SEX = rand() % 2;
        CREATURES[count].SPEED = rand() % 10;
        CREATURES[count].AGE = 1;
 	
	 	for(int SEES_OBJECT = 1;SEES_OBJECT < NUM_NEURONS;SEES_OBJECT++){
		for(int RESPONSE = 1;RESPONSE < NUM_RESPONSES;RESPONSE++){
			  BRAINS[count][SEES_OBJECT][RESPONSE] = rand()%10000;}}
    dbDot(CREATURES[count].X,CREATURES[count].Y,CREATURES[count].COLOR);
	}

	dbSyncOn();
	dbSyncRate(60);
	dbSetDisplayMode(SCREENX,SCREENY,32);

//=====================================================================================================================
//                            MAIN GAME LOOP                                                                                
//                            DATE:  11/22/17                                                                                
//                            DEPENDANCIES: DarkGDK.lib
//                                          DDraw.lib                                                   
//                                          DirectX 9.0                                                   
//=====================================================================================================================
int MAIN_LOOP_SPEED;

    while(LoopGDK()){
		dbCLS();
		MAIN_LOOP_SPEED = GetTickCount(); 

//-----------------------------Print Various Debug Information --------------------------------------------------------------
    	dbSetCursor(0,0);
    	cout <<  GetTickCount() - MAIN_LOOP_SPEED << endl;
    	cout << (NUM_CREATURES) << "  DEATHS: " << DEATHS << "   Number of Births" << BIRTHS << endl;
    	cout << BRAINS[1][0][1] << " " <<  BRAINS[1][0][2] << BRAINS[1][0][3] << " " <<  BRAINS[1][0][4] << endl;
    	cout << BRAINS[1][1][1] << " " <<  BRAINS[1][1][2] << BRAINS[1][1][3] << " " <<  BRAINS[1][1][4] << endl;
    	cout << BRAINS[1][2][1] << " " <<  BRAINS[1][2][2] << BRAINS[1][2][3] << " " <<  BRAINS[1][2][4] << endl;
    	cout << " ______________________________________" << endl;
    	cout << "X:" << dbMouseX() << " Y:" << dbMouseY() << endl;
//----------------------------------------------------------------------------------------------------------------------------
		int h= MAIN_LOOP_SPEED - GetTickCount();
		dbPrint(dbStr(h));
		

       	_MOVE_CREATURES();
		dbDot(100,100, dbRGB(255,0,0));
		
		dbSync();
     	//if (dbEscapeKey()==1){break;} // If program Ends
	}


	dbPrint("Done");
	dbWaitKey();
}


//    
//     
//


int _MAKE_CREATURE(int X ,int Y ,int HEALTH, int ANGLE ,bool SEX,int SPEED,DWORD COLOR){

	NUM_CREATURES++;
    int count = 1;
    while(CREATURES[count].EXIST){count++;} 
	
    CREATURES[count].X = X;
	CREATURES[count].Y = Y;
	CREATURES[count].HEALTH = HEALTH;
	CREATURES[count].ANGLE = ANGLE;
	CREATURES[count].COLOR = COLOR;
 
	CREATURES[count].EXIST  = TRUE;
	CREATURES[count].SEX = SEX;
	CREATURES[count].SPEED = SPEED;
	CREATURES[count].AGE = 1;
return count;	
}


void _MOVE_CREATURES(){
	// NEEDED: FIND A DBLOCK PIXEL COMMAND.... C++ LOCKBITS MIGHT WORK
	cout << "1" <<endl;
	for(int COUNT = 1;COUNT < MAX_CREATURES;COUNT++){
        NUM_OF_COLLISIONS = 0;
		if(CREATURES[COUNT].EXIST = TRUE){
			CREATURES[COUNT].AGE++;
			CREATURES[COUNT].LAST_BREED++;
			float OLDX = CREATURES[COUNT].X, OLDY = CREATURES[COUNT].Y;
			WORLD[int(OLDX)][(int)(OLDY)] = 0;
			
			int SEES = _SIGHT(COUNT,10);

			if(SEES != 0){
				if(CREATURES[SEES].SEX = CREATURES[COUNT].SEX){
					_SIGHT_HANDLER(COUNT,ENEMY);
				}

				if(CREATURES[SEES].SEX != CREATURES[COUNT].SEX){
					_SIGHT_HANDLER(COUNT,MATE);
				}
			}

			if( SEES = 0)  _SIGHT_HANDLER(COUNT,0); // OPTIMIZE THIS AS AN ELSE STATMENT ON THE if SEE NOT = TO 0 CHECK

			int HIT = _OBJECT_HIT(COUNT);
			if(HIT != 0){
//============++++++++++++===== BREED ===================================================================================
				if(CREATURES[HIT].SEX != CREATURES[COUNT].SEX && CREATURES[COUNT].LAST_BREED>1){ CREATURES[COUNT].LAST_BREED = 0; _BREED(COUNT,HIT);}
//============================= FIGHT ==================================================================================
		     	if(CREATURES[HIT].SEX = CREATURES[COUNT].SEX) CREATURES[COUNT].HEALTH -= 500;
				CREATURES[COUNT].X = OLDX;  CREATURES[COUNT].Y = OLDY;
           }
			if(HIT = 0)WORLD[(int)CREATURES[COUNT].X][(int)CREATURES[COUNT].Y] = COUNT;
		if(CREATURES[COUNT].AGE > MAX_AGE) {
				CREATURES[COUNT].HEALTH -=10;
                if(CREATURES[COUNT].HEALTH < 0)_KILL_CREATURE(COUNT);}

			if(CREATURES[COUNT].EXIST != 0 )dbDot(CREATURES[COUNT].X,CREATURES[COUNT].Y, CREATURES[COUNT].COLOR);

			_SET_PLAYER_POSITION(COUNT);
	   } //<--- creature exist
	}
return;}


void  _MOVE_OBJECT(int OBJ,float SPEED){
   float ANGLE = CREATURES[OBJ].ANGLE;
   float NEWX = _WRAPX(CREATURES[OBJ].X + SPEED * cos(ANGLE));
   float NEWY = _WRAPY(CREATURES[OBJ].Y + SPEED * sin(ANGLE));
	if( WORLD[int(NEWX)][int(NEWY) ]= 0){
	    CREATURES[OBJ].X = NEWX; CREATURES[OBJ].Y = NEWY;
	}
}

void _BREED(int creature1,int creature2){

	if(NUM_CREATURES >= MAX_CREATURES) return;
	    BIRTHS++;
        int X = _WRAPX(CREATURES[creature1].X  + (rand()%3) - 1);  
        int Y = _WRAPY(CREATURES[creature1].Y  + (rand()%3) - 1);
        int ANGLE = rand()%360;
        int SPEED = rand()%6;
        DWORD COLOR = CREATURES[creature1].COLOR + ((rand()% 1000) - 500);

        
        int COUNT = _MAKE_CREATURE(X,Y, 1000, ANGLE,(rand() % 2),SPEED,COLOR);
        CREATURES[COUNT].HEALTH = 1000;

		for(int SEES_OBJECT = 0;SEES_OBJECT < NUM_NEURONS;SEES_OBJECT++){
		   for(int RESPONSE = 0;RESPONSE < NUM_RESPONSES;RESPONSE++){
			  BRAINS[COUNT][SEES_OBJECT][RESPONSE] = rand()%10000 ;}}

}

void _KILL_CREATURE(int OBJ){
	CREATURES[OBJ].AGE  = 0;
	CREATURES[OBJ].COLOR = 0;
	CREATURES[OBJ].SEX = 0;
	CREATURES[OBJ].HEALTH = 0;
	CREATURES[OBJ].EXIST = 0;
		
	WORLD[(int)CREATURES[OBJ].X][(int)CREATURES[OBJ].Y] = 0;
	for(int SEES_OBJECT = 0;SEES_OBJECT < NUM_NEURONS;SEES_OBJECT++){
	    for(int RESPONSE = 0;RESPONSE < NUM_RESPONSES;RESPONSE++){
			  BRAINS[OBJ][SEES_OBJECT][RESPONSE] = 0;}}

	NUM_CREATURES--;
	DEATHS++;
return;
}

float NEWX(float X, float ANGLE,float DIST){
    float NX = X + DIST * cos(ANGLE) ;
	return NX;}

float NEWY(float Y,float ANGLE,float DIST){
    float NY = Y - DIST * sin(ANGLE);
return NY;
}

float _WRAPX(float VALUE){
	if (VALUE < LEFT_BOUNDS) VALUE = WX + (int)VALUE;
	if (VALUE > WORLD_WIDTH) VALUE = VALUE - (WORLD_WIDTH);
	return VALUE;}

float _WRAPY(float VALUE){
	if (VALUE < TOP_BOUNDS) VALUE = WY + (int)VALUE; 
	if (VALUE > WORLD_HEIGHT) VALUE = VALUE - (WORLD_HEIGHT);
	return VALUE;
}

int _CHECK_SPACE(int X,int Y){
    int A = WORLD[(int)_WRAPX(X)][(int)(_WRAPY(Y))];
	return A;
}

void _ROTATE_OBJECT(int OBJ,float ANGLE){
	    CREATURES[OBJ].ANGLE = dbWrapValue(CREATURES[OBJ].ANGLE + ANGLE);
return;}

void _SET_PLAYER_POSITION(int OBJ){
	CREATURES[OBJ].X = _WRAPX(CREATURES[OBJ].X);
	CREATURES[OBJ].Y = _WRAPY(CREATURES[OBJ].Y);
	CREATURES[OBJ].ANGLE = dbWrapValue(CREATURES[OBJ].ANGLE);
	return;
}

int _OBJECT_HIT(int OBJ){
	int RET = 0;
	int HIT;
	float X = CREATURES[OBJ].X;
	float Y = CREATURES[OBJ].Y;
	float NX,NY,A,ANGLE;
	float DIST = 1.0;
						
	
	ANGLE = CREATURES[OBJ].ANGLE - 90.0;


	WORLD[(int)X][(int)Y]= 0;

	for( A = ANGLE;A  < ANGLE + 180;A +=10){ // 10 IS THE THETA FIX THIS
		 NX = X + DIST * cos(A);
		 NY = Y + DIST * sin(A);
		//dbDot( NX,NY,dbRGB(255,0,0));
		HIT = WORLD[(int)_WRAPX(NX)][(int)_WRAPY(NY)];

		if(HIT != 0 &&  HIT != OBJ){RET = HIT;return RET;}
	}

return RET;
}

int _SIGHT(int OBJ,float SIGHT_DIST){
	float X = CREATURES[OBJ].X;
	float Y = CREATURES[OBJ].Y;
	float ANGLE = dbWrapValue(CREATURES[OBJ].ANGLE);

	float XX,YY;
	int FOV = 30;
	int VALUE;
	int DIST;
	int ANG;

	for(int A  = -FOV; A < FOV; A += 10){	

    	for(DIST = 2; DIST < SIGHT_DIST; DIST++){

    	    ANG = (int)dbWrapValue(ANGLE + A);
			XX = X + DIST * _COS[ANG];
			YY = Y + DIST * _SIN[ANG];
		
			//dbDot(XX,YY, dbRGB(0,255,0) );
			XX = _WRAPX(XX);
			YY = _WRAPY(YY);
			VALUE = WORLD[(int)XX][(int)YY];
			if(VALUE !=  0)return VALUE;
		}
		}

return 0;
}

void _SIGHT_HANDLER(int OBJ,int SIGHT){
int TURN_SPEED = 15;
int TRIGGER;

switch (SIGHT){
	case 0: // NOTHING								  
		TRIGGER = (rand() % 10001);
		if( BRAINS[OBJ][0][1] > TRIGGER) { 
			_ROTATE_OBJECT(OBJ,-TURN_SPEED);}
		TRIGGER = (rand() % 10001);
		if( BRAINS[OBJ][0][2] > TRIGGER) { 
			_ROTATE_OBJECT(OBJ,TURN_SPEED);}
		TRIGGER = (rand() % 10001);
		if( BRAINS[OBJ][0][3] > TRIGGER) { 
			_MOVE_OBJECT(OBJ,1);}
		TRIGGER = (rand() % 10001);
		if( BRAINS[OBJ][0][4] > TRIGGER){ 
			_ROTATE_OBJECT(OBJ,180);
		    _MOVE_OBJECT(OBJ,1);}
	break;									  		  
	case 1: // ENEMY						   
		TRIGGER = (rand() % 10001);
		if( BRAINS[OBJ][1][1] > TRIGGER)  {
			_ROTATE_OBJECT(OBJ, -TURN_SPEED);}
		TRIGGER = (rand() % 10001);
		if( BRAINS[OBJ][1][2] > TRIGGER) {
			_ROTATE_OBJECT(OBJ,TURN_SPEED);}
		TRIGGER = (rand() % 10001);
		if( BRAINS[OBJ][1][3] > TRIGGER){ 
			_MOVE_OBJECT(OBJ,1);}
		TRIGGER = (rand() % 10001);
		if( BRAINS[OBJ][1][4] > TRIGGER) {
			_ROTATE_OBJECT(OBJ,180);
		    _MOVE_OBJECT(OBJ,1);}
	break;								   		
	case 2: // MATE						   		
		TRIGGER = (rand() % 10001);
		if( BRAINS[OBJ][2][1] > TRIGGER){ 
			_ROTATE_OBJECT(OBJ, -TURN_SPEED);}
		TRIGGER = (rand() % 10001);
		if( BRAINS[OBJ][2][2] > TRIGGER) { 
			_ROTATE_OBJECT(OBJ, TURN_SPEED);}
		TRIGGER = (rand() % 10001);
		if( BRAINS[OBJ][2][3] > TRIGGER) {
			_MOVE_OBJECT(OBJ,1);}
		TRIGGER = (rand() % 10001);
		if( BRAINS[OBJ][2][4] > TRIGGER){ 
			_ROTATE_OBJECT(OBJ,180);
		   _MOVE_OBJECT(OBJ,1);}
	break;									  

	default:
		cout << "Third Object" << endl;
   }
return;
}
