
#include<DarkGDK.h>
#include<iostream>
#include<string>
#include"main.h"
#include<time.h>

int EXIST;

using namespace std;

void DarkGDK()
{

	srand (time(NULL)); // Seed the RND



	for(int ANGLE = 0; ANGLE < 361;ANGLE++){ // Initialize the COS and SIN lookup tables
        _COS[ANGLE] = cos((double)ANGLE * 3.14159 / 180);
        _SIN[ANGLE] = sin((double)ANGLE * 3.14159 / 180);
    }

	
for(int X= 0; X < SCREENX;X++){
	for(int Y = 0;Y < SCREENY;Y++){
	    WORLD[X][Y] = 0;
	}
}





	for(int count = 0;count < (MAX_CREATURES);count++){ //Initalize Starting creatures
        CREATURES[count] = CREATURE(0, rand() % WORLD_WIDTH,  rand() % WORLD_HEIGHT, rand() % 6, rand() % 360,rand() % 2, dbRgb(rand() % 255,rand() % 255,rand() % 255),rand() % 30,rand()%15) ;   
 	}

	for(int count = 0;count < AMOUNT_FOOD;count++){
		int x= rand()%SCREENX;
	    int y= rand()%SCREENY;
		WORLD[x][y] = -2;
	}

	for(int COUNT = 0;COUNT < 1000;COUNT++){ // MAKE THE INITIAL MAZE
		int length = rand() % 200;
		int X = CREATURE::WRAPX(rand()%SCREENX);
		int Y = CREATURE::WRAPX(rand()%SCREENY);
		for (int c = 0;c < length;c++){
		 X += rand()%3 - 1;
		 X = CREATURE::WRAPX(X);
		 Y += rand()%3 - 1;
		 Y = CREATURE::WRAPY(Y);
			WORLD[X][Y] = -1;}
	}


	
	dbSetDisplayMode(SCREENX,SCREENY,32);
dbSyncOn();
	dbSyncRate(90);
//=====================================================================================================================
//                            MAIN GAME LOOP                                                                                
//                            DATE:  11/22/17                                                                                
//                            DEPENDANCIES: DarkGDK.lib
//                                          DDraw.lib                                                   
//                                          DirectX 9.0(MAYBE 7.0 Not sure)                                                  
//=====================================================================================================================
 dbInk(dbRGB(0,0,255),dbRGB(0,0,0));
    while(LoopGDK()){
		dbCLS();
		 
		if (dbSpaceKey() == TRUE && SHOW_VISION == TRUE){SHOW_VISION = FALSE;Sleep(100);}
        if (dbUpKey() == TRUE && SHOW_COLLISION == TRUE){SHOW_COLLISION = FALSE;Sleep(100);}



		MAIN_LOOP_SPEED = GetTickCount();
	   _MOVE_CREATURES();
	   _PRINT_STATS();


		EXIST = 0;
		if (dbSpaceKey() == TRUE && SHOW_VISION == FALSE){SHOW_VISION = TRUE;Sleep(100);}
		if (dbUpKey() == TRUE && SHOW_COLLISION == FALSE){SHOW_COLLISION = TRUE;Sleep(100);}
		dbSync();
     }

	dbPrint("Done");
	dbWaitKey();
}



void _MOVE_CREATURES(){

dbLockPixels();

//for(int Y = 0;Y<SCREENY;Y++){for(int X = 0;X<SCREENX;X++){if(WORLD[(int)X][(int)Y] != 0)dbDot(X,Y,dbRGB(0,0,250));}}
    AVG_BRAIN1=0;
	AVG_BRAIN2=0;
	AVG_BRAIN3=0;
	AVG_BRAIN4=0;
	AVG_HEALTH=0;


	for(int COUNT = 0;COUNT < MAX_CREATURES;COUNT++){
        NUM_OF_COLLISIONS = 0;


		if(CREATURES[COUNT].EXIST == TRUE){
			EXIST++;
			   AVG_BRAIN4+=CREATURES[COUNT].BRAIN[2][0];
			   AVG_BRAIN1+=CREATURES[COUNT].BRAIN[2][1];
	           AVG_BRAIN2+=CREATURES[COUNT].BRAIN[2][2];
	           AVG_BRAIN3+=CREATURES[COUNT].BRAIN[2][3];
			   AVG_HEALTH+=CREATURES[COUNT].HEALTH;

			CREATURES[COUNT].AGE++;
			CREATURES[COUNT].LAST_BREED++;
			CREATURES[COUNT].LAST_MEAL++;
			//CREATURES[COUNT].COLOR = dbRGB(255 / CREATURES[COUNT].AGE,255,255);
			float OLDX = CREATURES[COUNT].X;
			float OLDY = CREATURES[COUNT].Y;

			WORLD[int(OLDX)][(int)(OLDY)] = 0;

			int SEES = 0;
			SEES = CREATURES[COUNT].SIGHT();
		

//=================RUN SIGHT METHOD =====================================i NEED TO ADD FOOD WHICH IS -2 ON THE WORLD GRID =======================================================

			if(SEES != 0){
				if(SEES == -1){
					CREATURES[COUNT].SIGHT_HANDLER(WALL); //SEES A WALL 
				}
			 else if(CREATURES[COUNT].SEX == CREATURES[SEES].SEX){   // IF IT SEES SAME SEX CREATURE
					CREATURES[COUNT].SIGHT_HANDLER(ENEMY);
				}

				if(CREATURES[SEES].SEX != CREATURES[COUNT].SEX){  // IF IT SEES OPPOSITE SEX CREATURES
					CREATURES[COUNT].SIGHT_HANDLER(MATE);
				}

			}
			else{
				CREATURES[COUNT].SIGHT_HANDLER(0); // SEES NOTHING 
			}

	//====================================COLLISION DETECTION AND REACTION ==================================================================
		
			int HIT = CREATURES[COUNT].HIT();

			if(HIT != 0){
				if(HIT == -1){
					CREATURES[COUNT].HEALTH -=50;
				}else if(HIT == -2){
				    CREATURES[COUNT].HEALTH +=50;
				}else if(CREATURES[HIT].SEX != CREATURES[COUNT].SEX){  if( CREATURES[COUNT].LAST_BREED >= 30){ 
    			    CREATURES[COUNT].LAST_BREED = 0;
				    CREATURES[COUNT].BREED(CREATURES[HIT]);					
				}}
				else{

					CREATURES[COUNT].HEALTH -= 50;
				}
 		        CREATURES[COUNT].X = OLDX;
				CREATURES[COUNT].Y = OLDY;
	    	    WORLD[int(OLDX)][int(OLDY)] = COUNT;
	    	    //if NUM_OF_COLLISIONS > 7 THEN _KILL_CREATURE(COUNT)				
			}
			else{
				CREATURES[COUNT].X = CREATURES[COUNT].POTENTIAL_X;
				CREATURES[COUNT].Y = CREATURES[COUNT].POTENTIAL_Y;
               WORLD[(int)CREATURES[COUNT].X][(int)CREATURES[COUNT].Y] = COUNT;
			}

		    if(CREATURES[COUNT].AGE >= MAX_AGE)
			{
				CREATURES[COUNT].HEALTH-=1;
			}

            if(CREATURES[COUNT].HEALTH <= 0)
			{
				CREATURES[COUNT].KILL();
			}
		 
			if(CREATURES[COUNT].EXIST == TRUE)CREATURES[COUNT].DRAW();
			//CREATURES[COUNT].SET_POSITION();

			if(CREATURES[COUNT].LAST_MEAL > 100)CREATURES[COUNT].KILL();





	   } //<--- creature exist


	}
	//	for(int Y = 0;Y<SCREENY;Y++){for(int X = 0;X<SCREENX;X++){if(WORLD[(int)X][(int)Y] != 0)dbDot(X,Y,dbRGB(255,0,0));}}
	for(int Y = 0;Y<SCREENY;Y++){
		for(int X = 0;X<SCREENX;X++){
		     if(WORLD[(int)X][(int)Y] == -1){
				 dbDot(X,Y,dbRGB(100,100,100)) ;dbDot(X,Y-1,dbRGB(155,155,155)) ; dbDot(X,Y-2,dbRGB(255,255,255));}
			  if(WORLD[(int)X][(int)Y] == -2)dbDot(X,Y,dbRGB(0,255,0));
		}
	}

dbUnlockPixels();
return;}


int _CHECK_SPACE(int XX,int YY){
    int A = WORLD[(int)CREATURE::WRAPX(XX)][(int)(CREATURE::WRAPY(YY))];
	return A;
}

//=================FOR DEBUGGING, IT ALLOWS YOU TO CONTROL CREATURE 1 IF NEEDED. =======================================================================================

void _CONTROL_CREATURE()
{
	int COUNT;
	
	int OX = CREATURES[1].X,OY = CREATURES[1].Y;
	WORLD[OX][OY] = 0;
	if(dbUpKey())CREATURES[1].MOVE(1);
	if(dbDownKey())CREATURES[1].MOVE(-1);
	if(dbRightKey())CREATURES[1].ROTATE(1);
	if(dbLeftKey())CREATURES[1].ROTATE(-1);
	
	CREATURES[1].SIGHT();
	CREATURES[1].HIT();
	if(CREATURES[1].HIT()){
		CREATURES[1].X = OX;
		CREATURES[1].Y = OY;
	
	}
	CREATURES[1].DRAW();
	CREATURES[1].SET_POSITION();
	return;
}


void _PRINT_STATS()
{
			float speed =  GetTickCount() - MAIN_LOOP_SPEED; dbPrint(speed);

		dbPrintC("BIRTHS:");dbPrint(dbStr(BIRTHS));
		dbPrintC("DEATHS:");dbPrint(dbStr(DEATHS));
	    dbPrintC("NUM OF CREATURES: ");dbPrint(dbStr(NUM_CREATURES));
		dbPrintC("EXIST");dbPrint(dbStr(EXIST));
		dbPrint("________________________________________");
		dbPrint(dbStr((_CHECK_SPACE(dbMouseX(),dbMouseY()))));
		dbPrint("________________________________________");
        dbPrint(dbStr(CREATURES[5].BRAIN[1][3]));
		dbPrint(dbStr(CREATURES[5].HEALTH));
		dbPrint("________________________________________");
		dbPrintC(dbStr((long)(AVG_BRAIN1 / EXIST)));
		dbPrintC("  ");
		dbPrintC(dbStr((long)(AVG_BRAIN2 / EXIST)));
		dbPrintC("  ");
		dbPrintC(dbStr((long)(AVG_BRAIN3 / EXIST)));
		dbPrintC("  ");
		dbPrint(dbStr((long)(AVG_BRAIN4 / EXIST)));

		dbPrint(dbStr((long)(AVG_BRAIN4 / AVG_HEALTH)));
		

		dbPrintC("FPS");dbPrint(dbStr(dbScreenFPS()));
		
			return;
}





/*-----------------------------Print Various Debug Information --------------------------------------------------------------
    	dbSetCursor(0,0);
    	cout <<  GetTickCount() - MAIN_LOOP_SPEED << endl;
    	cout << (NUM_CREATURES) << "  DEATHS: " << DEATHS << "   Number of Births" << BIRTHS << endl;
    	cout << BRAINS[1][0][1] << " " <<  BRAINS[1][0][2] << BRAINS[1][0][3] << " " <<  BRAINS[1][0][4] << endl;
    	cout << BRAINS[1][1][1] << " " <<  BRAINS[1][1][2] << BRAINS[1][1][3] << " " <<  BRAINS[1][1][4] << endl;
    	cout << BRAINS[1][2][1] << " " <<  BRAINS[1][2][2] << BRAINS[1][2][3] << " " <<  BRAINS[1][2][4] << endl;
    	cout << " ______________________________________" << endl;
    	cout << "X:" << dbMouseX() << " Y:" << dbMouseY() << endl;
//----------------------------------------------------------------------------------------------------------------------------*/

/*
	bool BCOLLISION;
	float RESPONSEX,RESPONSEY;


for(A = ANG;A  < ANG + 180;A +=1){
	 NX = X + DIST * cos(A * 3.14159/180 );
     NY = Y + DIST * sin(A * 3.14159/180 );
	RESPONSEX += RESPONSEX - NX;
	RESPONSEY += RESPONSEY - NY;
	COLLISION = WORLD[(int)WRAPX(NX)][(int)WRAPY(NY)];
	if((COLLISION != 0) &&  (COLLISION != ID)){
		



		//WORLD[(int)X][(int)Y]= 0;
//for(A = ANG;A  < ANG + 180;A +=1){ // 10 IS THE THETA FIX THIS
//	 NX = X + DIST * cos(A * 3.14159/180 );
//	 NY = Y + DIST * sin(A * 3.14159/180 );
//#ifdef COL
//	 dbDot(NX,NY,dbRGB(255,0,0));
//#endif
//	COLLISION = WORLD[(int)WRAPX(NX)][(int)WRAPY(NY)];
//	if((COLLISION != 0) &&  (COLLISION != ID)){ANGLE = -(ANGLE); return COLLISION;}
//}
//







	}*/