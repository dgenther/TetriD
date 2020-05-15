#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h> 
#include <math.h>
#include <string.h>
#include <Windows.h>

#define FONT_CHAR_SIZE 27
#define DATA_NUM_OFFSET 200
#define squareLen 40

#define ROWS 21
#define COLS 12

#define WINDOW_X 960
#define WINDOW_Y squareLen * ROWS

#define TRUE 1
#define FALSE 0
#define BEVEL 5


float red, green, blue;
//int fieldOffsetX = (WINDOW_X / 2) - (squareLen * 6);			// Centers the playing field
int fieldOffsetX = 0;											// Left-aligns playingfield
int fieldOffsetY = 1;
float dataFieldOffsetX = (COLS) * squareLen;
float dataFieldOffsetY = squareLen * 2;
float boxFieldOffsetY = squareLen * 2.5;	
float holdBoxOffsetY = 1;
float nextBox1OffsetY = 5;
float nextBox2OffsetY = 8.5;

int rewardTime = 8;			// how much time you're rewarded per line

int iColor = 0;

char GameOn = TRUE;
int score = 0;
unsigned char c; 					//key pressed
float textScale = 0.5;
float gameTime = 120;
void timer();
void setupBag();
int bagCounter = 0;					// Which slot of the bag we're on
int haveHold = 0;
int alreadySwapped = 0;
int lines = 0;
int level = 1;
float colorSpeed = 5;
int rotation = 0;


float gameSpeed[20] = {			// 20 ridiculous levels of difficult
	1.00000,
	0.79300,
	0.61780,
	0.47273,
	0.35520,
	0.26200,
	0.18968,
	0.13473,
	0.09388,
	0.06415,
	0.04298,
	0.02822,
	0.01815,
	0.01144,
	0.00706,
	0.00426,
	0.00252,
	0.00146,
	0.00082,
	0.00046
	};

int bag[7] = {-1};

int getPieceFromBag(){
	if (bagCounter > 6){
		setupBag();
		bagCounter = 0;
	}
	int temp;
	temp = bagCounter;
	bagCounter++;
	return bag[temp];
}

void setupBag(){
	int i, j, temp;
	for (i = 0; i < 7; i++){		//reset bag to -1's, otherwise first piece of new bag is always last piece of previous bag.
		bag[i] = -1;
	}
	for (i = 0; i < 7; i++){
		temp = rand()%7;

		while ( (temp == bag[0]) | (temp == bag[1]) | (temp == bag[2]) | (temp == bag[3]) | (temp == bag[4]) | (temp == bag[5]) ){
			temp = rand()%7;
		}
		
		bag[i] = temp;
	}
	printf("Bag = ");
	for (i = 0; i < 7; i++){
		printf("%d ", bag[i]);
	}
	printf("\n");
}

char Table[21][12] = {
	{0,0,0,0,0,0,0,0,0,0,0,0},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,1,1,1,1,1,1,1,1,1,1,1}
};

void reset(){
	int i, j;
	score = 0;
	gameTime = 120;
	haveHold = 0;
	lines = 0;
	
	for (i = 0; i < 20; i++){
		for (j = 1; j < 11; j++){
			Table[i][j] = 0;
		}
	}
	GameOn = TRUE;
}

// Reverses a string 'str' of length 'len' 
void reverse(char* str, int len) { 
    int i = 0, j = len - 1, temp; 
    while (i < j) { 
        temp = str[i]; 
        str[i] = str[j]; 
        str[j] = temp; 
        i++; 
        j--; 
    } 
} 
  
// Converts a given integer x to string str[].  
// d is the number of digits required in the output.  
// If d is more than the number of digits in x,  
// then 0s are added at the beginning. 
int intToStr(int x, char str[], int d) { 
    int i = 0; 
    while (x) { 
        str[i++] = (x % 10) + '0'; 
        x = x / 10; 
    } 
  
    // If number of digits required is more, then 
    // add 0s at the beginning 
    while (i < d) 
        str[i++] = '0'; 
  
    reverse(str, i); 
    str[i] = '\0'; 
    return i; 
} 
  
// Converts a floating-point/double number to a string. 
void ftoa(float n, char* res, int afterpoint) { 
    // Extract integer part 
    int ipart = (int)n; 
  
    // Extract floating part 
    float fpart = n - (float)ipart; 
  
    // convert integer part to string 
    int i = intToStr(ipart, res, 0); 
  
    // check for display option after point 
    if (afterpoint != 0) { 
        res[i] = '.'; // add dot 
  
        // Get the value of fraction part upto given no. 
        // of points after dot. The third parameter  
        // is needed to handle cases like 233.007 
        fpart = fpart * pow(10, afterpoint); 
  
        intToStr((int)fpart, res + i + 1, afterpoint); 
    } 
}

typedef struct {
	char **array;
	int width;
	float *color;		//{R,G,B}  between 0 and 1;
	int row, col;
}Shape ;
Shape current, next1, next2, hold;

const Shape ShapesArray[7]= {
    {(char *[]){(char []){ 0, 's','s'},
				(char []){'s','s', 0}, 
				(char []){ 0,  0,  0}}, 	3, 	(float []){0,1,0}}, //S_shape     
		
    {(char *[]){(char []){'z','z', 0},
				(char []){ 0, 'z','z'}, 
				(char []){ 0,  0,  0}}, 	3, 	(float []){0,1,1}}, //Z_shape     
    
	{(char *[]){(char []){ 0, 't', 0},
				(char []){'t','t','t'}, 
				(char []){ 0,  0,  0}}, 	3, 	(float []){1,.6,.1}}, //T_shape     
    
	{(char *[]){(char []){ 0,  0, 'l'},
				(char []){'l','l','l'}, 
				(char []){ 0,  0,  0}}, 	3, 	(float []){1,0,1}}, //L_shape     
    
	{(char *[]){(char []){'j', 0,  0},
				(char []){'j','j','j'}, 
				(char []){ 0,  0,  0}}, 	3, 	(float []){.8,.8,.8}}, //J_shape    
    
	{(char *[]){(char []){'o','o'},
				(char []){'o','o'}}, 	2, 		(float []){.1,.1,1}}, //O_shape
    
	{(char *[]){(char []){ 0,  0,  0,  0}, 
				(char []){'i','i','i','i'}, 
				(char []){ 0,  0,  0,  0}, 
				(char []){ 0,  0,  0,  0}}, 4, 	(float []){0,1,0}} //I_shape
};

Shape CopyShape(Shape shape){
    Shape new_shape = shape;
    char **copyshape = shape.array;
    new_shape.array = (char**)malloc(new_shape.width*sizeof(char*));
    int i, j;
    for(i = 0; i < new_shape.width; i++){
        new_shape.array[i] = (char*)malloc(new_shape.width*sizeof(char));
        for(j=0; j < new_shape.width; j++) {
            new_shape.array[i][j] = copyshape[i][j];
        }
    }
    return new_shape;
}

void DeleteShape(Shape shape){
    int i;
    for(i = 0; i < shape.width; i++){
        free(shape.array[i]);
    }
    free(shape.array);
}

int CheckPosition(Shape shape){ //Check the position of the copied shape, 
    char **array = shape.array;	
    int i, j;
    for(i = 0; i < shape.width;i++) {
        for(j = 0; j < shape.width ;j++){
        	if(Table[shape.row+i][shape.col+j] && array[i][j])
                return FALSE;
        }
    }
    return TRUE;
}

void GetNewShape(){ //returns random shape
    next1.col = 1 + rand()%(COLS-next1.width-2);
    next1.row = 1;
	
    DeleteShape(current);
    current = next1;
    next1 = next2;
    next2 = CopyShape(ShapesArray[getPieceFromBag()]);

	alreadySwapped = 0;				// reset the alreadySwapped flag to 0
	
    if(!CheckPosition(current)){
        GameOn = FALSE;       
    }
}

void RotateShape(Shape shape){ //rotates clockwise
    Shape temp = CopyShape(shape);
    int i, j, k, width;
    width = shape.width;
    for(i = 0; i < width ; i++){
        for(j = 0, k = width-1; j < width ; j++, k--){
                shape.array[i][j] = temp.array[k][i];
        }
    }
    DeleteShape(temp);
}

void swapPiece(){
	int i;
	if (haveHold){
		Shape temp = CopyShape(current);
		current = hold;
		current.row = 1;
		hold = temp;
		while (rotation != 0){
			RotateShape(hold);
			rotation += 1;
			rotation = (rotation > 3) ? 0 : rotation;
		}
		
	}else {
		hold = CopyShape(current);
		GetNewShape();
		haveHold = 1;
		while (rotation != 0){
			RotateShape(hold);
			rotation += 1;
			rotation = (rotation > 3) ? 0 : rotation;
		}
	}
}

void WriteToTable(){		// WHEN A PIECE LANDS
    int i, j;
    for(i = 0; i < current.width ;i++){
        for(j = 0; j < current.width ; j++){
            if(current.array[i][j])
                Table[current.row+i][current.col+j] = current.array[i][j];
        }
    }
    rotation = 0;
}

void Halleluyah_Baby(){ //checks lines
    int i, j, sum, count=0;
    for(i=1;i<ROWS-1;i++){
        sum = 0;
        for(j=1;j< COLS-1;j++) {
            sum+=(Table[i][j])? 1 : 0;
        }
        if(sum==COLS-2){
            count++;
            int l, k;
            for(k = i;k >=1;k--)
                for(l=1;l<COLS-1;l++)
                    Table[k][l]=Table[k-1][l];
            for(l=1;l<COLS-1;l++)
                Table[k][l]=0;
        }
        //printf("Row %d: sum = %d\n", i, sum);
        //printf("Count = %d\n", count);
    }
    if(GameOn){
    	lines += count;
    	gameTime += rewardTime*count;
    	switch (count){
    		case 1:
    			score += 100; break;
    		case 2:
    			score += 300; break;
    		case 3:
    			score += 500; break;
    		case 4:
    			score += 800; break;
    		default:
    			break;
    	}
    }
}

void PrintTable(){
    char Buffer[ROWS][COLS] = {0};
    int i, j;
    for(i = 0; i < current.width ;i++){
        for(j = 0; j < current.width ; j++){
            if(current.array[i][j])
                Buffer[current.row+i][current.col+j] = current.array[i][j];
        }
    }
    //clear();
    for(i = 0; i < ROWS ;i++){
        for(j = 0; j < COLS ; j++){
            //printf("%c ", (Table[i][j] + Buffer[i][j])? 'O': '.');
        }
        //printf("\n");
    }
    //printf("\nScore: %d\n\n\n\n\n\n\n\n", score);
	
	for (i = 0; i < ROWS;i++){
		for (j = 0; j < COLS; j++){
			// IF EDGE OF FIELD, DRAW ICE BLUE X's
			if ( (j == 0 || j == COLS-1 || i == ROWS-1) && i != 0 ) {	
				red = 0; green = 0; blue = 0;
				glBegin(GL_QUADS);
				glColor3f(red/255.0,green/255.0,blue/255.0);
				glVertex2i(squareLen*j		+fieldOffsetX, squareLen*i 		-fieldOffsetY);		//black background behind *'s
				glVertex2i(squareLen*(j+1)	+fieldOffsetX, squareLen*i 		-fieldOffsetY);
				glVertex2i(squareLen*(j+1)	+fieldOffsetX, squareLen*(i+1) 	-fieldOffsetY);
				glVertex2i(squareLen*j		+fieldOffsetX, squareLen*(i+1)	-fieldOffsetY);
				glEnd();
				
				red = 99; green = 98; blue = 255;
				glLineWidth(5);
																			
				glColor3f(red/255.0,green/255.0,blue/255.0);			//STAR SHAPE FOR FIELD BORDER, MADE FROM CROSSING LINES
				glBegin(GL_LINES);
				glVertex2i(squareLen*j		+fieldOffsetX, squareLen*i 		-fieldOffsetY);		//   /			Bottom
				glVertex2i(squareLen*(j+1)	+fieldOffsetX, squareLen*(i+1) 	-fieldOffsetY);		//				Top
				glEnd();
				
				glBegin(GL_LINES);
				glVertex2i(squareLen*j		+fieldOffsetX, squareLen*(i+1)	-fieldOffsetY);		//				Top
				glVertex2i(squareLen*(j+1)	+fieldOffsetX, squareLen*i 		-fieldOffsetY);		//	 \			Bottom
				glEnd();
				
				glBegin(GL_LINES);
				glVertex2i(squareLen*(j+.5)	+fieldOffsetX, squareLen*i 		-fieldOffsetY);		//	 |			Top
				glVertex2i(squareLen*(j+.5)	+fieldOffsetX, squareLen*(i+1)	-fieldOffsetY);		//				Bottom 
				glEnd();
				
				glBegin(GL_LINES);
				glVertex2i(squareLen*j		+fieldOffsetX, squareLen*(i+.5) -fieldOffsetY);		//	 -			Left
				glVertex2i(squareLen*(j+1)	+fieldOffsetX, squareLen*(i+.5)	-fieldOffsetY);		//				Right
				glEnd();
			} else if ( !(j == 0 || j == COLS-1 || i == ROWS-1) && (Table[i][j] + Buffer[i][j]) != 0) {		
			
				switch(Table[i][j] + Buffer[i][j]){
					case 'i':
						red = 186; green = 17; blue = 1; break;	//Red for I
					case 't':
						red = 179; green = 89; blue = 1; break;	//Orange for T
					case 's':
						red = 0; green = 144; blue = 2; break;	//Green for S
					case 'z':
						red = 0; green = 152; blue = 166; break;	//Cyan for Z
					case 'o':
						red = 0; green = 9; blue = 168; break;	//Blue for O
					case 'j':
						red = 155; green = 155; blue = 155; break;	//Gray/White for J
					case 'l':
						red = 183; green = 26; blue = 168; break;	//Magenta for L
				}
				
				glBegin(GL_POLYGON);
				glColor3f((red/255.0)*.8		,(green/255.0)*.8		,(blue/255.0)*.8		);
				glVertex2i(squareLen*j		+fieldOffsetX, squareLen*i 		-fieldOffsetY);						//Bottom left triangle
				glColor3f((red/255.0)*.7		,(green/255.0)*.7		,(blue/255.0)*.7		);
				glVertex2i(squareLen*(j+1)	+fieldOffsetX, squareLen*(i+1) 	-fieldOffsetY);
				glColor3f((red/255.0)*.6		,(green/255.0)*.6		,(blue/255.0)*.6		);
				glVertex2i(squareLen*j		+fieldOffsetX, squareLen*(i+1)	-fieldOffsetY);
				glEnd();
				
				glBegin(GL_POLYGON);
				glColor3f((red/255.0)*1.1		,(green/255.0)*1.1		,(blue/255.0)*1.1		);
				glVertex2i(squareLen*j		+fieldOffsetX, squareLen*i 		-fieldOffsetY);						//Top right triangle
				glColor3f((red/255.0)*1.2		,(green/255.0)*1.2		,(blue/255.0)*1.2		);
				glVertex2i(squareLen*(j+1)	+fieldOffsetX, squareLen*(i+1) 	-fieldOffsetY);
				glColor3f((red/255.0)*1.3		,(green/255.0)*1.3		,(blue/255.0)*1.3		);
				glVertex2i(squareLen*(j+1)	+fieldOffsetX, squareLen*i 		-fieldOffsetY);
				glEnd();
				
				glBegin(GL_QUADS);
				glColor3f((red/255.0)*.8		,(green/255.0)*.8		,(blue/255.0)*.8		);						//Top left corner 
				glVertex2i(squareLen*j		+fieldOffsetX + BEVEL, squareLen*i 		-fieldOffsetY + BEVEL);
				
				glColor3f((red/255.0)*1		,(green/255.0)*1		,(blue/255.0)*1		);						//Top right corner
				glVertex2i(squareLen*(j+1)	+fieldOffsetX - BEVEL, squareLen*i 		-fieldOffsetY + BEVEL);
				
				glColor3f((red/255.0)*.8		,(green/255.0)*.8		,(blue/255.0)*.8		);						//Bottom right corner
				glVertex2i(squareLen*(j+1)	+fieldOffsetX - BEVEL, squareLen*(i+1) 	-fieldOffsetY - BEVEL);
				
				glColor3f((red/255.0)*1		,(green/255.0)*1		,(blue/255.0)*1		);						//Bottom left corner
				glVertex2i(squareLen*j		+fieldOffsetX + BEVEL, squareLen*(i+1)	-fieldOffsetY - BEVEL);
				glEnd();
				//printf("Piece X,Y  =  %d,%d\n", squareLen*j	+fieldOffsetX, squareLen*i 	-fieldOffsetY );
			}else if (!(Table[i][j] + Buffer[i][j])){
				// IF NO PIECE THERE, DRAW BLACK SQUARES
				red = 0; green = 0; blue = 0;		
				glBegin(GL_QUADS);
				glColor3f(red,green,blue);
				glVertex2i(squareLen*j		+fieldOffsetX, squareLen*i 		-fieldOffsetY);
				glVertex2i(squareLen*(j+1)	+fieldOffsetX, squareLen*i 		-fieldOffsetY);
				glVertex2i(squareLen*(j+1)	+fieldOffsetX, squareLen*(i+1) 	-fieldOffsetY);
				glVertex2i(squareLen*j		+fieldOffsetX, squareLen*(i+1)	-fieldOffsetY);
				glEnd();
			}
		}		
	}
}

void buttons(unsigned char key, int x, int y){
	int i, j;
	Shape temp = CopyShape(current);
    switch(key){
        case 's':
            temp.row++;  //move down
            if(CheckPosition(temp))
                current.row++;
            else {
                WriteToTable();
                Halleluyah_Baby(); //check full lines, after putting it down
                GetNewShape();
            }
            break;
        case 'd':
            temp.col++;  //move right
            if(CheckPosition(temp))
                current.col++;
            break;
        case 'a':
            temp.col--;  //move left
            if(CheckPosition(temp))
                current.col--;
            break;
        case 'w':
            RotateShape(temp);  //yes
            if(CheckPosition(temp)){
            	RotateShape(current);
            	rotation += 1;
				rotation = (rotation > 3) ? 0 : rotation;
            }else {
            	///////////////////////////////////////////////////////////////////////////////////////////////
            	for (i = 0; i < temp.width; i++){									
            		
					if (Table[temp.row+i][temp.col] && temp.array[i][0]){			//check left side
            			temp.col++;
            			if(CheckPosition(temp)){
            				current.col++;
            				RotateShape(current);
            				rotation += 1;
							rotation = (rotation > 3) ? 0 : rotation;
            			}else if (temp.width == 4){
            				temp.col++;
            				if(CheckPosition(temp)){
            					current.col++;			//moveing I-piece to the right twice to prevent clipping out, if allowed!
            					current.col++;
            					RotateShape(current);
            					rotation += 1;
								rotation = (rotation > 3) ? 0 : rotation;
            				}
            			}else{
            				temp.col--;
            			}
            		}
            		if (Table[temp.row+i][temp.col + temp.width-1] && temp.array[i][temp.width-1]){		//check right side
            			temp.col--;
            			if(CheckPosition(temp)){
            				current.col--;
            				RotateShape(current);
            				rotation += 1;
							rotation = (rotation > 3) ? 0 : rotation;
            			}else if (temp.width == 4){
            				temp.col--;
            				if(CheckPosition(temp)){
            					current.col--;
            					current.col--;
            					RotateShape(current);
            					rotation += 1;
								rotation = (rotation > 3) ? 0 : rotation;
            				}
            			}else {
            				temp.col++;
            			}
            		}
					if (Table[temp.row + temp.width-1][temp.col+i] && temp.array[temp.width-1][i]){
						temp.row--;
            			if(CheckPosition(temp)){
            				current.row--;
            				RotateShape(current);
            				rotation += 1;
							rotation = (rotation > 3) ? 0 : rotation;
            			}else if (temp.width == 4){
            				temp.row--;
            				if(CheckPosition(temp)){
            					current.row--;
            					current.row--;
            					RotateShape(current);
            					rotation += 1;
								rotation = (rotation > 3) ? 0 : rotation;
            				}
            			}
					}	
            	}
			}
            break;
        case 'y':
        	DeleteShape(temp);
        	reset();
        	break;
        case 'q':
        	if (alreadySwapped == 0){
        		swapPiece();
        		alreadySwapped = 1;			// Can only swap once per piece
        	}
        	break;
    }
    
    c = key;
    
    DeleteShape(temp);
    glutPostRedisplay();
}

void pieceFall(){
	Shape temp = CopyShape(current);
	temp.row++;  //move down
	if(CheckPosition(temp))
		current.row++;
	else {
		WriteToTable();
		Halleluyah_Baby(); //check full lines, after putting it down
		GetNewShape();
	}
	DeleteShape(temp);
    glutPostRedisplay();
}

void drawText(unsigned char* string, float x, float y, float scale){
	const char *c;
	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_LINE_SMOOTH);
	
	glPushMatrix();
    glTranslated(dataFieldOffsetX + x, dataFieldOffsetY + y, 0);
    glScaled(1 * textScale * scale, (-1) * textScale * scale, 0); 
	for (c = string; *c != '\0' ; c++){
		glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
	}
	glPopMatrix();
}

void GameOver(){
	red = ShapesArray[iColor].color[0]; green = ShapesArray[iColor].color[1]; blue = ShapesArray[iColor].color[2];
	glColor3f(red,green,blue);
	glLineWidth(4);
	drawText("G A M E O V E R", -480, 300, 1.2);
}

void drawPieceBoxes(){


	glBegin(GL_QUADS);		//HOLD BOX
	glColor3f(0,0,0);
	glVertex2i(dataFieldOffsetX + squareLen/2, 					boxFieldOffsetY + holdBoxOffsetY*squareLen/2					);		
	glVertex2i(dataFieldOffsetX + squareLen/2 + squareLen * 5, 	boxFieldOffsetY + holdBoxOffsetY*squareLen/2					);
	glVertex2i(dataFieldOffsetX + squareLen/2 + squareLen * 5, 	boxFieldOffsetY + holdBoxOffsetY*squareLen/2 + squareLen * 3	);
	glVertex2i(dataFieldOffsetX + squareLen/2, 					boxFieldOffsetY + holdBoxOffsetY*squareLen/2 + squareLen * 3	);
	glEnd();
	
	glBegin(GL_QUADS);		// NEXT PIECE BOX 1
	glColor3f(0,0,0);
	glVertex2i(dataFieldOffsetX + squareLen/2, 					boxFieldOffsetY + nextBox1OffsetY*squareLen					);		
	glVertex2i(dataFieldOffsetX + squareLen/2 + squareLen * 5, 	boxFieldOffsetY + nextBox1OffsetY*squareLen					);
	glVertex2i(dataFieldOffsetX + squareLen/2 + squareLen * 5, 	boxFieldOffsetY + nextBox1OffsetY*squareLen + squareLen * 3	);
	glVertex2i(dataFieldOffsetX + squareLen/2, 					boxFieldOffsetY + nextBox1OffsetY*squareLen + squareLen * 3	);
	glEnd();
	
	glBegin(GL_QUADS);		// NEXT PIECE BOX 2
	glColor3f(0,0,0);
	glVertex2i(dataFieldOffsetX + squareLen/2, 					boxFieldOffsetY + nextBox2OffsetY*squareLen					);		
	glVertex2i(dataFieldOffsetX + squareLen/2 + squareLen * 5, 	boxFieldOffsetY + nextBox2OffsetY*squareLen					);
	glVertex2i(dataFieldOffsetX + squareLen/2 + squareLen * 5, 	boxFieldOffsetY + nextBox2OffsetY*squareLen + squareLen * 3	);
	glVertex2i(dataFieldOffsetX + squareLen/2, 					boxFieldOffsetY + nextBox2OffsetY*squareLen + squareLen * 3	);
	glEnd();
	
	
}

void drawNextPieces(){
	int i, j, limitWidth;
	int next1X = dataFieldOffsetX + squareLen;
	int next1Y = boxFieldOffsetY + nextBox1OffsetY*squareLen + squareLen/2;
	int next2X = dataFieldOffsetX + squareLen;
	int next2Y = boxFieldOffsetY + nextBox2OffsetY*squareLen + squareLen/2;
	int holdX = dataFieldOffsetX + squareLen;
	int holdY = boxFieldOffsetY + holdBoxOffsetY*squareLen/2 + squareLen/2;
	
	//next1.array[i][j]
	//next2.array[i][j]
	
	limitWidth = next1.width;		///////////////////////////////////////////////////////////////////////////
	if (limitWidth == 4) limitWidth = 3;		//DRAW NEXT 1
	for (i = 0; i < limitWidth; i++){//////////////////////////////////////////////////////
		for (j = 0; j < next1.width; j++){
			if (next1.array[i][j] != 0){
				switch(next1.array[i][j]){
				case 'i':
					red = 186; green = 17; blue = 1; break;	//Red for I
				case 't':
					red = 179; green = 89; blue = 1; break;	//Orange for T
				case 's':
					red = 0; green = 144; blue = 2; break;	//Green for S
				case 'z':
					red = 0; green = 152; blue = 166; break;	//Cyan for Z
				case 'o':
					red = 0; green = 9; blue = 168; break;	//Blue for O
				case 'j':
					red = 155; green = 155; blue = 155; break;	//Gray/White for J
				case 'l':
					red = 183; green = 26; blue = 168; break;	//Magenta for L
				default:
					red = 0; green = 0; blue = 0; break;	//Black area around piece
				}
			
				glBegin(GL_POLYGON);
				glColor3f((red/255.0)*.8		,(green/255.0)*.8		,(blue/255.0)*.8		);
				glVertex2i(squareLen*j		+next1X, squareLen*i 		+next1Y);						//Bottom left triangle
				glColor3f((red/255.0)*.7		,(green/255.0)*.7		,(blue/255.0)*.7		);
				glVertex2i(squareLen*(j+1)	+next1X, squareLen*(i+1) 	+next1Y);
				glColor3f((red/255.0)*.6		,(green/255.0)*.6		,(blue/255.0)*.6		);
				glVertex2i(squareLen*j		+next1X, squareLen*(i+1)	+next1Y);
				glEnd();
				
				glBegin(GL_POLYGON);
				glColor3f((red/255.0)*1.1		,(green/255.0)*1.1		,(blue/255.0)*1.1		);
				glVertex2i(squareLen*j		+next1X, squareLen*i 		+next1Y);						//Top right triangle
				glColor3f((red/255.0)*1.2		,(green/255.0)*1.2		,(blue/255.0)*1.2		);
				glVertex2i(squareLen*(j+1)	+next1X, squareLen*(i+1) 	+next1Y);
				glColor3f((red/255.0)*1.3		,(green/255.0)*1.3		,(blue/255.0)*1.3		);
				glVertex2i(squareLen*(j+1)	+next1X, squareLen*i 		+next1Y);
				glEnd();
				
				glBegin(GL_QUADS);
				glColor3f((red/255.0)*.8		,(green/255.0)*.8		,(blue/255.0)*.8		);						//Top left corner 
				glVertex2i(squareLen*j		+next1X + BEVEL, squareLen*i 		+next1Y + BEVEL);
				
				glColor3f((red/255.0)*1		,(green/255.0)*1		,(blue/255.0)*1		);						//Top right corner
				glVertex2i(squareLen*(j+1)	+next1X - BEVEL, squareLen*i 		+next1Y + BEVEL);
				
				glColor3f((red/255.0)*.8		,(green/255.0)*.8		,(blue/255.0)*.8		);						//Bottom right corner
				glVertex2i(squareLen*(j+1)	+next1X - BEVEL, squareLen*(i+1) 	+next1Y - BEVEL);
				
				glColor3f((red/255.0)*1		,(green/255.0)*1		,(blue/255.0)*1		);						//Bottom left corner
				glVertex2i(squareLen*j		+next1X + BEVEL, squareLen*(i+1)	+next1Y - BEVEL);
				glEnd();
			}
		}
	}
	limitWidth = next2.width;////////////////////////////////////////////////////////////////////////////////////////////////////
	if (limitWidth == 4) limitWidth = 3;		//DRAW NEXT2
	for (i = 0; i < limitWidth; i++){////////////////////////////////////////////////////////////////////////////////////////////
		for (j = 0; j < next2.width; j++){
			if (next2.array[i][j] != 0){
				switch(next2.array[i][j]){
				case 'i':
					red = 186; green = 17; blue = 1; break;	//Red for I
				case 't':
					red = 179; green = 89; blue = 1; break;	//Orange for T
				case 's':
					red = 0; green = 144; blue = 2; break;	//Green for S
				case 'z':
					red = 0; green = 152; blue = 166; break;	//Cyan for Z
				case 'o':
					red = 0; green = 9; blue = 168; break;	//Blue for O
				case 'j':
					red = 155; green = 155; blue = 155; break;	//Gray/White for J
				case 'l':
					red = 183; green = 26; blue = 168; break;	//Magenta for L
				default:
					red = 0; green = 0; blue = 0; break;	//Black area around piece
				}
			
				glBegin(GL_POLYGON);
				glColor3f((red/255.0)*.8		,(green/255.0)*.8		,(blue/255.0)*.8		);
				glVertex2i(squareLen*j		+next2X, squareLen*i 		+next2Y);						//Bottom left triangle
				glColor3f((red/255.0)*.7		,(green/255.0)*.7		,(blue/255.0)*.7		);
				glVertex2i(squareLen*(j+1)	+next2X, squareLen*(i+1) 	+next2Y);
				glColor3f((red/255.0)*.6		,(green/255.0)*.6		,(blue/255.0)*.6		);
				glVertex2i(squareLen*j		+next2X, squareLen*(i+1)	+next2Y);
				glEnd();
				
				glBegin(GL_POLYGON);
				glColor3f((red/255.0)*1.1		,(green/255.0)*1.1		,(blue/255.0)*1.1		);
				glVertex2i(squareLen*j		+next2X, squareLen*i 		+next2Y);						//Top right triangle
				glColor3f((red/255.0)*1.2		,(green/255.0)*1.2		,(blue/255.0)*1.2		);
				glVertex2i(squareLen*(j+1)	+next2X, squareLen*(i+1) 	+next2Y);
				glColor3f((red/255.0)*1.3		,(green/255.0)*1.3		,(blue/255.0)*1.3		);
				glVertex2i(squareLen*(j+1)	+next2X, squareLen*i 		+next2Y);
				glEnd();
				
				glBegin(GL_QUADS);
				glColor3f((red/255.0)*.8		,(green/255.0)*.8		,(blue/255.0)*.8		);						//Top left corner 
				glVertex2i(squareLen*j		+next2X + BEVEL, squareLen*i 		+next2Y + BEVEL);
				
				glColor3f((red/255.0)*1		,(green/255.0)*1		,(blue/255.0)*1		);						//Top right corner
				glVertex2i(squareLen*(j+1)	+next2X - BEVEL, squareLen*i 		+next2Y + BEVEL);
				
				glColor3f((red/255.0)*.8		,(green/255.0)*.8		,(blue/255.0)*.8		);						//Bottom right corner
				glVertex2i(squareLen*(j+1)	+next2X - BEVEL, squareLen*(i+1) 	+next2Y - BEVEL);
				
				glColor3f((red/255.0)*1		,(green/255.0)*1		,(blue/255.0)*1		);						//Bottom left corner
				glVertex2i(squareLen*j		+next2X + BEVEL, squareLen*(i+1)	+next2Y - BEVEL);
				glEnd();
			}
		}
	}
	limitWidth = next2.width;////////////////////////////////////////////////////////////////////////////////////////////////////
	if (limitWidth == 4) limitWidth = 3;		//DRAW NEXT2
	for (i = 0; i < limitWidth; i++){////////////////////////////////////////////////////////////////////////////////////////////
		for (j = 0; j < next2.width; j++){
			if (next2.array[i][j] != 0){
				switch(next2.array[i][j]){
				case 'i':
					red = 186; green = 17; blue = 1; break;	//Red for I
				case 't':
					red = 179; green = 89; blue = 1; break;	//Orange for T
				case 's':
					red = 0; green = 144; blue = 2; break;	//Green for S
				case 'z':
					red = 0; green = 152; blue = 166; break;	//Cyan for Z
				case 'o':
					red = 0; green = 9; blue = 168; break;	//Blue for O
				case 'j':
					red = 155; green = 155; blue = 155; break;	//Gray/White for J
				case 'l':
					red = 183; green = 26; blue = 168; break;	//Magenta for L
				default:
					red = 0; green = 0; blue = 0; break;	//Black area around piece
				}
			
				glBegin(GL_POLYGON);
				glColor3f((red/255.0)*.8		,(green/255.0)*.8		,(blue/255.0)*.8		);
				glVertex2i(squareLen*j		+next2X, squareLen*i 		+next2Y);						//Bottom left triangle
				glColor3f((red/255.0)*.7		,(green/255.0)*.7		,(blue/255.0)*.7		);
				glVertex2i(squareLen*(j+1)	+next2X, squareLen*(i+1) 	+next2Y);
				glColor3f((red/255.0)*.6		,(green/255.0)*.6		,(blue/255.0)*.6		);
				glVertex2i(squareLen*j		+next2X, squareLen*(i+1)	+next2Y);
				glEnd();
				
				glBegin(GL_POLYGON);
				glColor3f((red/255.0)*1.1		,(green/255.0)*1.1		,(blue/255.0)*1.1		);
				glVertex2i(squareLen*j		+next2X, squareLen*i 		+next2Y);						//Top right triangle
				glColor3f((red/255.0)*1.2		,(green/255.0)*1.2		,(blue/255.0)*1.2		);
				glVertex2i(squareLen*(j+1)	+next2X, squareLen*(i+1) 	+next2Y);
				glColor3f((red/255.0)*1.3		,(green/255.0)*1.3		,(blue/255.0)*1.3		);
				glVertex2i(squareLen*(j+1)	+next2X, squareLen*i 		+next2Y);
				glEnd();
				
				glBegin(GL_QUADS);
				glColor3f((red/255.0)*.8		,(green/255.0)*.8		,(blue/255.0)*.8		);						//Top left corner 
				glVertex2i(squareLen*j		+next2X + BEVEL, squareLen*i 		+next2Y + BEVEL);
				
				glColor3f((red/255.0)*1		,(green/255.0)*1		,(blue/255.0)*1		);						//Top right corner
				glVertex2i(squareLen*(j+1)	+next2X - BEVEL, squareLen*i 		+next2Y + BEVEL);
				
				glColor3f((red/255.0)*.8		,(green/255.0)*.8		,(blue/255.0)*.8		);						//Bottom right corner
				glVertex2i(squareLen*(j+1)	+next2X - BEVEL, squareLen*(i+1) 	+next2Y - BEVEL);
				
				glColor3f((red/255.0)*1		,(green/255.0)*1		,(blue/255.0)*1		);						//Bottom left corner
				glVertex2i(squareLen*j		+next2X + BEVEL, squareLen*(i+1)	+next2Y - BEVEL);
				glEnd();
			}
		}
	}
	if(haveHold){
		limitWidth = hold.width;////////////////////////////////////////////////////////////////////////////////////////////////////
		if (limitWidth == 4) limitWidth = 3;		//DRAW HOLD
		for (i = 0; i < limitWidth; i++){////////////////////////////////////////////////////////////////////////////////////////////
			for (j = 0; j < hold.width; j++){
				if (hold.array[i][j] != 0){
					switch(hold.array[i][j]){
					case 'i':
						red = 186; green = 17; blue = 1; break;	//Red for I
					case 't':
						red = 179; green = 89; blue = 1; break;	//Orange for T
					case 's':
						red = 0; green = 144; blue = 2; break;	//Green for S
					case 'z':
						red = 0; green = 152; blue = 166; break;	//Cyan for Z
					case 'o':
						red = 0; green = 9; blue = 168; break;	//Blue for O
					case 'j':
						red = 155; green = 155; blue = 155; break;	//Gray/White for J
					case 'l':
						red = 183; green = 26; blue = 168; break;	//Magenta for L
					default:
						red = 0; green = 0; blue = 0; break;	//Black area around piece
					}
				
					glBegin(GL_POLYGON);
					glColor3f((red/255.0)*.8		,(green/255.0)*.8		,(blue/255.0)*.8		);
					glVertex2i(squareLen*j		+holdX, squareLen*i 		+holdY);						//Bottom left triangle
					glColor3f((red/255.0)*.7		,(green/255.0)*.7		,(blue/255.0)*.7		);
					glVertex2i(squareLen*(j+1)	+holdX, squareLen*(i+1) 	+holdY);
					glColor3f((red/255.0)*.6		,(green/255.0)*.6		,(blue/255.0)*.6		);
					glVertex2i(squareLen*j		+holdX, squareLen*(i+1)		+holdY);
					glEnd();
					
					glBegin(GL_POLYGON);
					glColor3f((red/255.0)*1.1		,(green/255.0)*1.1		,(blue/255.0)*1.1		);
					glVertex2i(squareLen*j		+holdX, squareLen*i 		+holdY);						//Top right triangle
					glColor3f((red/255.0)*1.2		,(green/255.0)*1.2		,(blue/255.0)*1.2		);
					glVertex2i(squareLen*(j+1)	+holdX, squareLen*(i+1) 	+holdY);
					glColor3f((red/255.0)*1.3		,(green/255.0)*1.3		,(blue/255.0)*1.3		);
					glVertex2i(squareLen*(j+1)	+holdX, squareLen*i 		+holdY);
					glEnd();
					
					glBegin(GL_QUADS);
					glColor3f((red/255.0)*.8		,(green/255.0)*.8		,(blue/255.0)*.8		);						//Top left corner 
					glVertex2i(squareLen*j		+holdX + BEVEL, squareLen*i 	+holdY + BEVEL);
					
					glColor3f((red/255.0)*1		,(green/255.0)*1		,(blue/255.0)*1		);						//Top right corner
					glVertex2i(squareLen*(j+1)	+holdX - BEVEL, squareLen*i 	+holdY + BEVEL);
					
					glColor3f((red/255.0)*.8		,(green/255.0)*.8		,(blue/255.0)*.8		);						//Bottom right corner
					glVertex2i(squareLen*(j+1)	+holdX - BEVEL, squareLen*(i+1) +holdY - BEVEL);
					
					glColor3f((red/255.0)*1		,(green/255.0)*1		,(blue/255.0)*1		);						//Bottom left corner
					glVertex2i(squareLen*j		+holdX + BEVEL, squareLen*(i+1)	+holdY - BEVEL);
					glEnd();
				}
			}
		}
	}
}

void changeColor(float r, float g, float b){
	red = r; green = g; blue = b; 
	glColor3f(red/255.0,green/255.0,blue/255.0);
}

void drawControls(){
	int temp = WINDOW_X - dataFieldOffsetX + squareLen/2 + squareLen * 5;	//halfway between black boxes and right edge of screen
	int offsetQX = temp/2 - dataFieldOffsetX/2 + squareLen * 3 + 10;	// between black boxes and R from Rotate
	int offsetQY = -30;
	int offsetRX = 160;
	
	changeColor(186,17,1);
	drawText("Swap", offsetQX, nextBox1OffsetY + squareLen*5 -40 + offsetQY, .4);
	drawText("|", offsetQX+25, nextBox1OffsetY + squareLen*5-10 + offsetQY, .4);
	drawText("q", offsetQX+20, nextBox1OffsetY + squareLen*5 + 20 + offsetQY, .4);

	changeColor(179,89,1);
	drawText("Reset", offsetQX + offsetRX, nextBox1OffsetY + squareLen*5 -40 + offsetQY, .4);
	drawText("|", offsetQX+25 + offsetRX, nextBox1OffsetY + squareLen*5-10 + offsetQY, .4);
	drawText("y", offsetQX+20 + offsetRX, nextBox1OffsetY + squareLen*5 + 20 + offsetQY, .4);

	changeColor(0,144,2);
	drawText("Rotate", temp/2-10-30, nextBox1OffsetY + squareLen*5 -40, .4);
	drawText("|", temp/2-5, nextBox1OffsetY + squareLen*5-10, .4);
	drawText("w", temp/2-10, nextBox1OffsetY + squareLen*5 + 20, .4);

	changeColor(0,152,166);
	drawText("s", temp/2-10, nextBox1OffsetY + squareLen*5 + 80, .4);
	drawText("|", temp/2-5, nextBox1OffsetY + squareLen*5 + 110, .4);
	drawText("Down", temp/2-10-25, nextBox1OffsetY + squareLen*5 + 140, .4);

	changeColor(0,9,168);
	drawText("Left", temp/2-10-105, nextBox1OffsetY + squareLen*5 + 50, .4);
	drawText("-", temp/2-10-55, nextBox1OffsetY + squareLen*5 + 50, .4);
	drawText("a", temp/2-10-30, nextBox1OffsetY + squareLen*5 + 50, .4);
	
	changeColor(183,26,168);
	drawText("d", temp/2-10+30, nextBox1OffsetY + squareLen*5 + 50, .4);
	drawText("-", temp/2-10+50, nextBox1OffsetY + squareLen*5 + 50, .4);
	drawText("Right", temp/2-10+70, nextBox1OffsetY + squareLen*5 + 50, .4);
}

void drawDataField(){
	char *gameTimeString, *scoreString, *lineString, *levelString;
	int levelOffsetY = 525;
	int timeOffsetY = 575;
	int linesOffsetY = 625;
	int scoreOffsetY = 675;
	
	int levelNumOffsetX = 200 + DATA_NUM_OFFSET;
	int timeNumOffsetX = 200 - FONT_CHAR_SIZE*2 + DATA_NUM_OFFSET;
	int linesNumOffsetX = 200 + DATA_NUM_OFFSET;
	int scoreNumOffsetX = 200 + DATA_NUM_OFFSET;
	
	levelNumOffsetX -= (level >= 10) ? FONT_CHAR_SIZE: 0;
	
	timeNumOffsetX += (gameTime < 100) ? FONT_CHAR_SIZE : 0;
	timeNumOffsetX += (gameTime < 10)  ? FONT_CHAR_SIZE : 0;
	
	linesNumOffsetX -= (lines >= 10) ? FONT_CHAR_SIZE : 0;
	linesNumOffsetX -= (lines >= 100) ? FONT_CHAR_SIZE : 0;
	linesNumOffsetX -= (lines >= 1000) ? FONT_CHAR_SIZE : 0;
	linesNumOffsetX -= (lines >= 10000) ? FONT_CHAR_SIZE : 0;
	
	scoreNumOffsetX -= (score >= 100) ? FONT_CHAR_SIZE * 2 : 0;
	scoreNumOffsetX -= (score >= 1000)  ? FONT_CHAR_SIZE : 0;
	scoreNumOffsetX -= (score >= 10000)  ? FONT_CHAR_SIZE : 0;
	scoreNumOffsetX -= (score >= 100000)  ? FONT_CHAR_SIZE : 0;
	scoreNumOffsetX -= (score >= 1000000)  ? FONT_CHAR_SIZE : 0;
	
	
	gameTimeString = (char*)malloc(7*sizeof(char));
	scoreString = 	 (char*)malloc(7*sizeof(char));
	lineString = 	 (char*)malloc(7*sizeof(char));
	levelString = 	 (char*)malloc(7*sizeof(char));
	
	itoa(gameTime, gameTimeString, 10);
	itoa(score, scoreString, 10);
	itoa(lines, lineString, 10);
	itoa(level, levelString, 10);

	changeColor(0,0,0);
	drawPieceBoxes();
	drawNextPieces();

	glLineWidth(2);
	red = ShapesArray[iColor].color[0]; green = ShapesArray[iColor].color[1]; blue = ShapesArray[iColor].color[2];	
	glColor3f(red,green,blue);
	drawText("T E T R I D", squareLen, -50 + squareLen, .8);

	drawControls();
	
	changeColor(0,0,0);
	glLineWidth(6);
	drawText("Lines = +8 sec", (WINDOW_X - dataFieldOffsetX + squareLen/2 + squareLen * 5)/2-10-105, nextBox1OffsetY + squareLen*10.5, .4);
	drawText("Hold", squareLen-20, 35, .4);
	drawText("Next", squareLen-20, 35+squareLen*4.5, .4);
	
	drawText("LEVEL:", squareLen/2, levelOffsetY + squareLen, .8);
	drawText(levelString, squareLen + levelNumOffsetX, levelOffsetY + squareLen, .8);
	
	drawText("TIME:", squareLen/2, timeOffsetY + squareLen, .8);
	drawText(gameTimeString, squareLen + timeNumOffsetX, timeOffsetY + squareLen, .8);
	
	drawText("LINES:", squareLen/2, linesOffsetY + squareLen, .8);
	drawText(lineString, squareLen + linesNumOffsetX, linesOffsetY + squareLen, .8);
	
	drawText("SCORE:", squareLen/2, scoreOffsetY + squareLen, .8);
	drawText(scoreString, squareLen + scoreNumOffsetX, scoreOffsetY + squareLen, .8);

	changeColor(255,255,255);
	glLineWidth(2);
	drawText("Lines = +8 sec", (WINDOW_X - dataFieldOffsetX + squareLen/2 + squareLen * 5)/2-10-105, nextBox1OffsetY + squareLen*10.5, .4);
	drawText("Hold", squareLen-20, 35, .4);
	drawText("Next", squareLen-20, 35+squareLen*4.5, .4);
	
	drawText("LEVEL:", squareLen/2, levelOffsetY + squareLen, .8);
	drawText(levelString, squareLen + levelNumOffsetX, levelOffsetY + squareLen, .8);
	
	drawText("TIME:", squareLen/2, timeOffsetY + squareLen, .8);
	drawText(gameTimeString, squareLen + timeNumOffsetX, timeOffsetY + squareLen, .8);
	
	drawText("LINES:", squareLen/2, linesOffsetY + squareLen, .8);
	drawText(lineString, squareLen + linesNumOffsetX, linesOffsetY + squareLen, .8);
	
	drawText("SCORE:", squareLen/2, scoreOffsetY + squareLen, .8);
	drawText(scoreString, squareLen + scoreNumOffsetX, scoreOffsetY + squareLen, .8);
}

void display(){
	int i;
	glClear(GL_COLOR_BUFFER_BIT);
	PrintTable();
	drawDataField();
	
	printf("rotation = %d\n", rotation);
	
	if (!GameOn){
		GameOver();
	}
	
	glFlush();
	glutSwapBuffers();
}

void init(){
	glClearColor(0.3,0.3,0.3,0);
	gluOrtho2D(0,WINDOW_X,WINDOW_Y,0);
}

void speedTimer(){
	int i;
	if (GameOn){
		pieceFall();
	}
	level = 1 + (lines / 10);
	
	glutPostRedisplay();
	glutTimerFunc(1000*gameSpeed[level-1], speedTimer, 0);

}

void colorTimer(){									//used to make the multicolor stuff
	iColor += 1;
	iColor = (iColor > 6) ? 0 : iColor;
	
	glutPostRedisplay();
	glutTimerFunc((30000.0 / colorSpeed)/60.0, colorTimer, 0);
	
}

void gameTimer(){
	if(gameTime == 0){		// if time hits 0, Game Over
		GameOn = FALSE;
	}
	if (GameOn){			// while game is on, countdown the timer
		gameTime -= 1;
	}	
	glutPostRedisplay();
	glutTimerFunc(1000, gameTimer, 0);
}

int main(int argc, char** argv){
	srand(time(NULL));
	glutInit(&argc, argv); 
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(WINDOW_X,WINDOW_Y);
	glutCreateWindow("Mort's Game");
	init();
	setupBag();
	
	next1 = CopyShape(ShapesArray[getPieceFromBag()]);
	next2 = CopyShape(ShapesArray[getPieceFromBag()]);
	GetNewShape();
	
	glutDisplayFunc(display);
	glutKeyboardFunc(buttons);
	glutTimerFunc(1000, gameTimer, 0);
	glutTimerFunc(1000*gameSpeed[level-1],speedTimer,0);
	glutTimerFunc((30000.0 / colorSpeed)/60.0, colorTimer, 0);
	glutMainLoop();
	return 0;
}
