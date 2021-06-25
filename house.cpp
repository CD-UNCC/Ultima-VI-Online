//#include "stdafx.h" //CRITICAL header information

#include "data_both.h"
#include "define_host.h"
#include "function_host.h"
#include "function_both.h" // remove this if logadd is not needed




// b111 build tiles
void cleararea(int x, int y, int lengthx, int lengthy);

void buildwall(int x, int y, int length, int buildtype, int wallstyle, int floorstyle);
void buildfloor(int x, int y, int lengthx, int lengthy, int floorstyle);
void buildwallrect(int x, int y, int lengthx, int lengthy, int buildtype, int wallstyle, int floorstyle);
void buildwallrect(int x, int y, int lengthx, int lengthy, int buildtype, int wallstyle, int floorstyle, int skipsideoption);

void buildwall2(int x, int y, int length, int buildtype, int wallstyle, int floorstyle, int begintileoption, int endtileoption);
void buildwallrect2(int x, int y, int lengthx, int lengthy, int buildtype, int wallstyle, int floorstyle, int skipsideoption);

// b111 build objects
//void buildwallobj(int x, int y, int length, int buildtype, int wallstyle, int floorstyle, int begintileoption, int endtileoption);
void buildwallobj(int x, int y, int length, int buildtype, int objnum, int begintileoption, int endtileoption);
void buildwallrectobj(int x, int y, int lengthx, int lengthy, int buildtype, int objnum, int skipsideoption);
void buildarch(int x, int y, int length, int buildtype, int archstart, int archlength, int wallstyle, int begintileoption, int endtileoption);

object* builddoor(int x, int y, int type, int doorstyle, int wallstyle, int floorstyle);
object* buildlever(int x, int y, object* door);
void buildstorage(int x, int y, int lengthx, int lengthy, int storagetype);


//void buildwallh(int x, int y, int length, int walltype, int wallstyle, int floorstyle);
//void buildwallv(int x, int y, int length, int walltype, int wallstyle, int floorstyle);


void house(){
	static object *myobj=OBJnew(),*myobj2=OBJnew(),*myobj3=OBJnew(),*myobj4=OBJnew();

	static long i=0,i2=0,i3=0,i4=0,i5=0,i6=0,i7=0,i8=0,i9=0;
	static long x=0,x2=0,x3=0,x4=0,x5=0,x6=0,x7=0,x8=0,x9=0;
	static long y=0,y2=0,y3=0,y4=0,y5=0,y6=0,y7=0,y8=0,y9=0;
	static long z=0,z2=0,z3=0,z4=0,z5=0,z6=0,z7=0,z8=0,z9=0;
	static double d=0,d2=0,d3=0,d4=0,d5=0,d6=0,d7=0,d8=0,d9=0;
	static unsigned char b=0,b2=0,b3=0,b4=0,b5=0,b6=0,b7=0,b8=0,b9=0;
	static float f=0,f2=0,f3=0,f4=0,f5=0,f6=0,f7=0,f8=0,f9=0;


	//init. house arrays
	ZeroMemory(&housex1,sizeof(housex1)); ZeroMemory(&housey1,sizeof(housey1));
	ZeroMemory(&housex2,sizeof(housex2)); ZeroMemory(&housey2,sizeof(housey2));
	ZeroMemory(&housepnext,sizeof(housepnext));
	ZeroMemory(&housepx,sizeof(housepx)); ZeroMemory(&housepy,sizeof(housepy));
	ZeroMemory(&housecost,sizeof(housecost));
	ZeroMemory(&houseinitialcost,sizeof(houseinitialcost));
	ZeroMemory(&housestoragenext,sizeof(housestoragenext));
	ZeroMemory(&housestoragex,sizeof(housestoragex)); ZeroMemory(&housestoragey,sizeof(housestoragey));

/* TODO/FIXME!
 * the inefficient way of calling BTset (not in a loop etc) in the patches, 
 * causes a huge delay when optimizing house.cpp, and even when optimizing for size,
 * still makes more than 1 MB of obj file. I think this contributes most to the size
 * of the host/both versions. Speed is not needed here as it is a one time process.
 * Consider patching basetiles file directly, or at least read in the patches at runtime and
 * parse them, so we don't have an object file full of placing things on the stack,
 * calling a function, rinse, repeat.
 */

// t111
int basex=0;
int basey=0;
int basecx = 0;
int basecy = 0;
int houseid;
int arenaid;

// init locations
hlocx[BUILD_HOUSEID_MANOR1][BUILD_SECTIONID_BASE] = 377;
hlocy[BUILD_HOUSEID_MANOR1][BUILD_SECTIONID_BASE] = 384-3;



#include "map_patches/house1.txt"
#include "map_patches/house2.txt"
#include "map_patches/house3.txt"
#include "map_patches/castlerooms.txt"
#include "map_patches/castlenonhousetiles.txt"
#include "map_patches/castlehousetiles.txt"
#include "map_patches/castlehousepatch.txt"
#include "map_patches/guardianguild.txt"
#include "map_patches/ragnor.txt"
#include "map_patches/katish.txt"
#include "map_patches/mose.txt"
#include "map_patches/bryan.txt"
#include "map_patches/darrell.txt"
#include "map_patches/notir.txt"
//#include "map_patches/toth.txt" //a house floating in the void with respawning staff. A BIG NO NO
#include "map_patches/forestfix.txt"
#include "map_patches/farm.txt"
#include "map_patches/spiritwood.txt"
#include "map_patches/steel.txt"
#include "map_patches/misc.txt"
#include "map_patches/shop.txt"
//#include "map_patches/dungeontest.txt"

/*
if (easymodehostn1) {
#include "map_patches/newhouse01.txt"
#include "map_patches/newhouse02.txt"
#include "map_patches/newhouse03.txt"
}
*/

// s111 check house storage slots used/placed and show warning on host if it is exceeded.  Items may/will be lost.
int housenumnew = 0;
int storageusednew = 0;

for (int i=0; i < HOUSEMAX; i++) {
	if (housecost[i] > 0) {
		if (housestoragenext[i] > storageusednew) {
			storageusednew = housestoragenext[i];
			housenumnew = i;
		}
	}
}

//if (TRUE) {
//if (storageusednew >= 240) {
if (storageusednew >= HOUSESTORAGESLOTMAX - 30) {
	txt *t2=txtnew();
	txt *t3=txtnew();

	txtset(t2, "--- WARNING!!! ---> One or more house has exceeded (or almost exceeded) the house storage slots limit!  ITEMS may/will be LOST!");
	LOGadd(t2);
	txtset(t2, "--- Recommended action ---> Shutdown the server, backup saves, and fix the problem.");
	LOGadd(t2);
	txtset(t2, "storage used = housestoragenext = ");
	txtnumint(t3, storageusednew);
	txtadd(t2, t3);
	LOGadd(t2);
	txtset(t2, "storage max = HOUSESTORAGESLOTMAX = ");
	txtnumint(t3, HOUSESTORAGESLOTMAX);
	txtadd(t2, t3);
	LOGadd(t2);
	txtset(t2, "house number affected = ");
	txtnumint(t3, housenumnew);
	txtadd(t2, t3);
	LOGadd(t2);
}



}//house






















// b111 building stuff
void cleararea(int x, int y, int lengthx, int lengthy) {
	int xi, yi;
	object* obj;

	for (yi = y; yi < y + lengthy; yi++) {
		for (xi = x; xi < x + lengthx; xi++) {
			obj = OBJfindlast(xi, yi);

			if (obj)
				OBJremove(obj);
		}
	}
}

void buildwallh(int x, int y, int length, int walltype, int wallstyle, int floorstyle) {
	int lefttile, midtile, righttile;
	int i;

	if (wallstyle == 1) {
		if (walltype == WALL_NORTH) {
			// top
			lefttile = 146;
			midtile = 144;

			if (floorstyle == 1)
				righttile = 152;
			else if (floorstyle == 2)
				righttile = 147;
		} else if (walltype == WALL_SOUTH) {
			// bottom
			righttile = 149;
			midtile = 144;

			if (floorstyle == 1)
				lefttile = 153;
			else if (floorstyle == 2)
				lefttile = 148;
		} else if (walltype == 3) {
			// mid
			lefttile = 144;
			midtile = 144;

			if (floorstyle == 1)
				righttile = 150;
			else if (floorstyle == 2)
				righttile = 150;
		}
	} else if (wallstyle == 2) {
	}

	BTset(x, y, lefttile);
	BTset(x + length - 1, y, righttile);

	for (i = x + 1; i < x + length - 1; i++)
		BTset(i, y, midtile);
}

void buildwallv(int x, int y, int length, int walltype, int wallstyle, int floorstyle) {
	int begintile, endtile, tile;
	int i;

	if (wallstyle == 1) {
		if (walltype == WALL_WEST) {
			// top
			begintile = 145;
			tile = 145;

			if (floorstyle == 1)
				endtile = 145;
			else if (floorstyle == 2)
				endtile = 145;
		} else if (walltype == WALL_EAST) {
			// bottom
			endtile = 145;
			tile = 145;

			if (floorstyle == 1)
				begintile = 145;
			else if (floorstyle == 2)
				begintile = 145;
		} else if (walltype == 3) {
			// mid
			begintile = 145;
			tile = 145;

			if (floorstyle == 1)
				endtile = 145;
			else if (floorstyle == 2)
				endtile = 145;
		}
	} else if (wallstyle == 2) {
	}

	BTset(x, y, begintile);
	BTset(x, y + length - 1, endtile);

	for (i = y + 1; i < y + length - 1; i++)
		BTset(x, i, tile);
}

void buildwall(int x, int y, int length, int buildtype, int wallstyle, int floorstyle) {
	int begintile, tile, endtile;
	int buildtype1;
	int i;

	if (wallstyle == 1) {
		if (buildtype == WALL_NORTH) {
			// north
			buildtype1 = BUILD_TYPE_HORIZONTAL;
			begintile = 146;
			tile = 144;

			if (floorstyle == 1)
				endtile = 152;
			else if (floorstyle == 2)
				endtile = 147;
		} else if (buildtype == WALL_SOUTH) {
			// south
			buildtype1 = BUILD_TYPE_HORIZONTAL;
			endtile = 149;
			tile = 144;

			if (floorstyle == 1)
				begintile = 153;
			else if (floorstyle == 2)
				begintile = 148;
		} else if (buildtype == BUILD_TYPE_HORIZONTAL) {
			// mid
			buildtype1 = BUILD_TYPE_HORIZONTAL;
			begintile = 144;
			tile = 144;

			if (floorstyle == 1)
				endtile = 150;
			else if (floorstyle == 2)
				endtile = 150;
		} else if (buildtype == WALL_WEST) {
			// west
			buildtype1 = BUILD_TYPE_VERTICAL;
			begintile = 145;
			tile = 145;

			if (floorstyle == 1)
				endtile = 145;
			else if (floorstyle == 2)
				endtile = 145;
		} else if (buildtype == WALL_EAST) {
			// east
			buildtype1 = BUILD_TYPE_VERTICAL;
			endtile = 145;
			tile = 145;

			if (floorstyle == 1)
				begintile = 145;
			else if (floorstyle == 2)
				begintile = 145;
		} else if (buildtype == BUILD_TYPE_VERTICAL) {
			// mid
			buildtype1 = BUILD_TYPE_VERTICAL;
			begintile = 145;
			tile = 145;

			if (floorstyle == 1)
				endtile = 145;
			else if (floorstyle == 2)
				endtile = 145;
		}
	} else if (wallstyle == 2) {
	}

	if (buildtype1 == BUILD_TYPE_HORIZONTAL) {
		BTset(x, y, begintile);
		BTset(x + length - 1, y, endtile);

		for (i = x + 1; i < x + length - 1; i++)
			BTset(i, y, tile);
	} else if (buildtype1 == BUILD_TYPE_VERTICAL) {
		BTset(x, y, begintile);
		BTset(x, y + length - 1, endtile);

		for (i = y + 1; i < y + length - 1; i++)
			BTset(x, i, tile);
	}
}

int getwalltile(int floorstyle, int wallstyle, int tileoption) {
	int tile = 0;

	if (wallstyle == BUILD_WALLSTYLE_STONE1) {
		if (tileoption == BUILD_TILEOPTION_H) {
			tile = 144;
		} else if (tileoption == BUILD_TILEOPTION_V) {
			tile = 145;
		} else if (tileoption == BUILD_TILEOPTION_NW) {
			tile = 146;
		} else if (tileoption == BUILD_TILEOPTION_NE) {
			if ((floorstyle == BUILD_FLOORSTYLE_BLUE1) || (floorstyle == BUILD_FLOORSTYLE_BLUE2)) {
				tile = 147;
			}
		} else if (tileoption == BUILD_TILEOPTION_SW) {
			if ((floorstyle == BUILD_FLOORSTYLE_BLUE1) || (floorstyle == BUILD_FLOORSTYLE_BLUE2)) {
				tile = 148;
			}
		} else if (tileoption == BUILD_TILEOPTION_SE) {
			tile = 149;
		}
	}

	return tile;
}

void buildwall2(int x, int y, int length, int buildtype, int wallstyle, int floorstyle, int begintileoption, int endtileoption) {
	int begintile, tile, endtile;
	//int buildtype1;
	int i;

	/*
	if (wallstyle == 1) {
		if (begintileoption == BUILD_TILEOPTION_NW)
			begintile = 146;
		else if (begintileoption == BUILD_TILEOPTION_NE)

		if (buildtype == WALL_NORTH) {
			// north
			buildtype1 = BUILD_TYPE_HORIZONTAL;
			begintile = 146;
			tile = 144;

			if (floorstyle == 1)
				endtile = 152;
			else if (floorstyle == 2)
				endtile = 147;
		} else if (buildtype == WALL_SOUTH) {
			// south
			buildtype1 = BUILD_TYPE_HORIZONTAL;
			endtile = 149;
			tile = 144;

			if (floorstyle == 1)
				begintile = 153;
			else if (floorstyle == 2)
				begintile = 148;
		} else if (buildtype == BUILD_TYPE_HORIZONTAL) {
			// mid
			buildtype1 = BUILD_TYPE_HORIZONTAL;
			begintile = 144;
			tile = 144;

			if (floorstyle == 1)
				endtile = 150;
			else if (floorstyle == 2)
				endtile = 150;
		} else if (buildtype == WALL_WEST) {
			// west
			buildtype1 = BUILD_TYPE_VERTICAL;
			begintile = 145;
			tile = 145;

			if (floorstyle == 1)
				endtile = 145;
			else if (floorstyle == 2)
				endtile = 145;
		} else if (buildtype == WALL_EAST) {
			// east
			buildtype1 = BUILD_TYPE_VERTICAL;
			endtile = 145;
			tile = 145;

			if (floorstyle == 1)
				begintile = 145;
			else if (floorstyle == 2)
				begintile = 145;
		} else if (buildtype == BUILD_TYPE_VERTICAL) {
			// mid
			buildtype1 = BUILD_TYPE_VERTICAL;
			begintile = 145;
			tile = 145;

			if (floorstyle == 1)
				endtile = 145;
			else if (floorstyle == 2)
				endtile = 145;
		}
	} else if (wallstyle == 2) {
	}
	*/

	begintile = getwalltile(floorstyle, wallstyle, begintileoption);
	endtile = getwalltile(floorstyle, wallstyle, endtileoption);

	if (buildtype == BUILD_TYPE_HORIZONTAL) {
		tile = getwalltile(floorstyle, wallstyle, BUILD_TILEOPTION_H);

		if (length > 0)
			BTset(x, y, begintile);

		if (length > 1)
			BTset(x + length - 1, y, endtile);

		for (i = x + 1; i < x + length - 1; i++)
			BTset(i, y, tile);
	} else if (buildtype == BUILD_TYPE_VERTICAL) {
		tile = getwalltile(floorstyle, wallstyle, BUILD_TILEOPTION_V);

		if (length > 0)
			BTset(x, y, begintile);

		if (length > 1)
			BTset(x, y + length - 1, endtile);

		for (i = y + 1; i < y + length - 1; i++)
			BTset(x, i, tile);
	}
}

void buildwallrect(int x, int y, int lengthx, int lengthy, int buildtype, int wallstyle, int floorstyle, int skipsideoption) {
	/*
	bool skipnorth = FALSE;
	bool skipsouth = FALSE;
	bool skipeast = FALSE;
	bool skipwest = FALSE;
	int xadj = 0, yadj = 0;
	int lengthxadj = 0, lengthyadj = 0;

	if (skipsideoption - BUILD_SKIPSIDE_WEST >= 0) {
		skipwest = TRUE;
		skipsideoption -= BUILD_SKIPSIDE_WEST;
	}

	if (skipsideoption - BUILD_SKIPSIDE_EAST >= 0) {
		skipeast = TRUE;
		skipsideoption -= BUILD_SKIPSIDE_EAST;
	}

	if (skipsideoption - BUILD_SKIPSIDE_SOUTH >= 0) {
		skipsouth = TRUE;
		skipsideoption -= BUILD_SKIPSIDE_SOUTH;
	}

	if (skipsideoption - BUILD_SKIPSIDE_NORTH >= 0) {
		skipnorth = TRUE;
		skipsideoption -= BUILD_SKIPSIDE_NORTH;
	}


	if (skipnorth) {
		yadj--;
		lengthyadj++;
	}

	if (skipsouth) {
		// yadj--;
		lengthyadj++;
	}

	if (skipeast) {
		xadj--;
		lengthxadj++;
	}

	if (skipwest) {
		// yadj--;
		lengthxadj++;
	}
	*/

	buildwall(x, y, lengthx, WALL_NORTH, wallstyle, floorstyle);
	buildwall(x, y + lengthy - 1, lengthx, WALL_SOUTH, wallstyle, floorstyle);
	buildwall(x, y + 1, lengthy - 2, WALL_WEST, wallstyle, floorstyle);
	buildwall(x + lengthx - 1, y + 1, lengthy - 2, WALL_EAST, wallstyle, floorstyle);
	buildfloor(x + 1, y + 1, lengthx - 2, lengthy - 2, floorstyle);
}

void buildwallrect2(int x, int y, int lengthx, int lengthy, int buildtype, int wallstyle, int floorstyle, int skipsideoption) {
	/*
	buildwall2(x, y, lengthx, BUILD_TYPE_H, wallstyle, floorstyle, BUILD_TILEOPTION_NW, BUILD_TILEOPTION_NE);
	buildwall2(x, y + lengthy - 1, lengthx, BUILD_TYPE_H, wallstyle, floorstyle, BUILD_TILEOPTION_SW, BUILD_TILEOPTION_SE);
	buildwall2(x, y + 1, lengthy - 2, BUILD_TYPE_V, wallstyle, floorstyle, BUILD_TILEOPTION_V, BUILD_TILEOPTION_V);
	buildwall2(x + lengthx - 1, y + 1, lengthy - 2, BUILD_TYPE_V, wallstyle, floorstyle, BUILD_TILEOPTION_V, BUILD_TILEOPTION_V);
	buildfloor(x + 1, y + 1, lengthx - 2, lengthy - 2, floorstyle);
	*/

	bool skipnorth = FALSE;
	bool skipsouth = FALSE;
	bool skipwest = FALSE;
	bool skipeast = FALSE;

	int xadj = 1;
	int yadj = 1;
	int lengthxadj = -2;
	int lengthyadj = -2;
	int begintileoption1 = BUILD_TILEOPTION_NW;
	int endtileoption1 = BUILD_TILEOPTION_NE;
	int begintileoption2 = BUILD_TILEOPTION_SW;
	int endtileoption2 = BUILD_TILEOPTION_SE;
	int begintileoption3 = BUILD_TILEOPTION_V;
	int endtileoption3 = BUILD_TILEOPTION_V;
	int begintileoption4 = BUILD_TILEOPTION_V;
	int endtileoption4 = BUILD_TILEOPTION_V;


	//begintileoption = BUILD_TILEOPTION_H;
	//endtileoption = BUILD_TILEOPTION_H;

	if (skipsideoption - BUILD_SKIPSIDE_EAST >= 0) {
		skipeast = TRUE;
		skipsideoption -= BUILD_SKIPSIDE_EAST;
	}

	if (skipsideoption - BUILD_SKIPSIDE_WEST >= 0) {
		skipwest = TRUE;
		skipsideoption -= BUILD_SKIPSIDE_WEST;
	}

	if (skipsideoption - BUILD_SKIPSIDE_SOUTH >= 0) {
		skipsouth = TRUE;
		skipsideoption -= BUILD_SKIPSIDE_SOUTH;
	}

	if (skipsideoption - BUILD_SKIPSIDE_NORTH >= 0) {
		skipnorth = TRUE;
		skipsideoption -= BUILD_SKIPSIDE_NORTH;
	}


	if (skipnorth) {
		yadj--;
		lengthyadj++;
	}

	if (skipsouth) {
		// yadj--;
		lengthyadj++;
	}

	if (skipwest) {
		xadj--;
		lengthxadj++;
		begintileoption1 = BUILD_TILEOPTION_H;
		begintileoption2 = BUILD_TILEOPTION_H;
	}

	if (skipeast) {
		lengthxadj++;
		endtileoption1 = BUILD_TILEOPTION_H;
		endtileoption2 = BUILD_TILEOPTION_H;
	}

	/*
	if (skipnorth) {
		begintileoption3 = BUILD_TILEOPTION_NW;
		begintileoption4 = BUILD_TILEOPTION_NE;
	}

	if (skipsouth) {
		endtileoption3 = BUILD_TILEOPTION_SW;
		endtileoption4 = BUILD_TILEOPTION_SE;
	}
	*/

	if (!skipnorth)
		buildwall2(x, y, lengthx, BUILD_TYPE_H, wallstyle, floorstyle, begintileoption1, endtileoption1);

	if (!skipsouth)
		buildwall2(x, y + lengthy - 1, lengthx, BUILD_TYPE_H, wallstyle, floorstyle, begintileoption2, endtileoption2);

	if (!skipwest)
		buildwall2(x, y + yadj, lengthy + lengthyadj, BUILD_TYPE_V, wallstyle, floorstyle, begintileoption3, endtileoption3);

	if (!skipeast)
		buildwall2(x + lengthx - 1, y + yadj, lengthy + lengthyadj, BUILD_TYPE_V, wallstyle, floorstyle, begintileoption4, endtileoption4);

	buildfloor(x + xadj, y + yadj, lengthx + lengthxadj, lengthy + lengthyadj, floorstyle);
}

void buildwallrect(int x, int y, int lengthx, int lengthy, int buildtype, int wallstyle, int floorstyle) {
	buildwallrect(x, y, lengthx, lengthy, buildtype, wallstyle, floorstyle, BUILD_SKIPSIDE_NONE);
}

void buildfloor(int x, int y, int lengthx, int lengthy, int floorstyle) {
	int lefttile, midtile, righttile;
	int xi, yi;

	if (floorstyle == 1) {
		midtile = 214;
	} else if (floorstyle == 2) {
		midtile = 213;
	} else if (floorstyle == BUILD_FLOORSTYLE_BLUE1) {
		midtile = 213;
	} else if (floorstyle == BUILD_FLOORSTYLE_BLUE2) {
		midtile = 214;
		//midtile = 214;
		/*
		if (floorstyle == WALL_NORTH) {
			// top
			lefttile = 146;
			midtile = 144;

			if (floorstyle == 1)
				righttile = 152;
			else if (floorstyle == 2)
				righttile = 147;
		} else if (floorstyle == WALL_SOUTH) {
			// bottom
			righttile = 149;
			midtile = 144;

			if (floorstyle == 1)
				lefttile = 153;
			else if (floorstyle == 2)
				lefttile = 148;
		} else if (floorstyle == 3) {
			// mid
			lefttile = 144;
			midtile = 144;

			if (floorstyle == 1)
				righttile = 150;
			else if (floorstyle == 2)
				righttile = 150;
		}
		*/
	}

	//BTset(x, y, lefttile);
	//BTset(x + length - 1, y, righttile);

	for (yi = y; yi < y + lengthy; yi++) {
		for (xi = x; xi < x + lengthx; xi++) {
			BTset(xi, yi, midtile);
		}
	}
}



int getwallobj(int objnum, int tileoption) {
	int tile = 0;

	if (objnum == OBJ_TABLE) {
		if (tileoption == BUILD_TILEOPTION_H) {
			tile = OBJ_TABLE + 1024 * 4;
		}
		else if (tileoption == BUILD_TILEOPTION_V) {
			tile = OBJ_TABLE + 1024 * 1;
		}
		else if (tileoption == BUILD_TILEOPTION_NW) {
			tile = OBJ_TABLE + 1024 * 7;
		}
		else if (tileoption == BUILD_TILEOPTION_NE) {
			tile = OBJ_TABLE + 1024 * 8;
		}
		else if (tileoption == BUILD_TILEOPTION_SW) {
			tile = OBJ_TABLE + 1024 * 9; //
		}
		else if (tileoption == BUILD_TILEOPTION_SE) {
			tile = OBJ_TABLE + 1024 * 10; //
		}
		else if (tileoption == BUILD_TILEOPTION_N) {
			tile = OBJ_TABLE + 1024 * 0;
		}
		else if (tileoption == BUILD_TILEOPTION_S) {
			tile = OBJ_TABLE + 1024 * 2;
		}
		else if (tileoption == BUILD_TILEOPTION_W) {
			tile = OBJ_TABLE + 1024 * 3;
		}
		else if (tileoption == BUILD_TILEOPTION_E) {
			tile = OBJ_TABLE + 1024 * 5;
		}
	} else if (objnum == OBJ_FENCE) {
		if (tileoption == BUILD_TILEOPTION_H) {
			tile = OBJ_FENCE + 1024 * 0;
		}
		else if (tileoption == BUILD_TILEOPTION_V) {
			tile = OBJ_FENCE + 1024 * 1;
		}
		else if (tileoption == BUILD_TILEOPTION_NW) {
			tile = OBJ_FENCE + 1024 * 2;
		}
		else if (tileoption == BUILD_TILEOPTION_NE) {
			tile = OBJ_FENCE + 1024 * 3;
		}
		else if (tileoption == BUILD_TILEOPTION_SW) {
			tile = OBJ_FENCE + 1024 * 5;
		}
		else if (tileoption == BUILD_TILEOPTION_SE) {
			tile = OBJ_FENCE + 1024 * 4;
		}
	} else if (objnum == OBJ_ARCHWAY) {
		if (tileoption == BUILD_TILEOPTION_H) {
			tile = OBJ_ARCHWAY + 1024 * 0;
		} else if (tileoption == BUILD_TILEOPTION_V) {
			tile = OBJ_ARCHWAY + 1024 * 2;
		} else if (tileoption == BUILD_TILEOPTION_N) {
			//tile = OBJ_ARCHWAY + 1024 * 10;
			tile = OBJ_DOORWAY + 1024 * 2;
		} else if (tileoption == BUILD_TILEOPTION_S) {
			tile = OBJ_ARCHWAY + 1024 * 3;
		} else if (tileoption == BUILD_TILEOPTION_W) {
			//tile = OBJ_ARCHWAY + 1024 * 11;
			tile = OBJ_DOORWAY + 1024 * 0;
		} else if (tileoption == BUILD_TILEOPTION_E) {
			tile = OBJ_ARCHWAY + 1024 * 1;
		}
	} else if (objnum == OBJ_DOORWAY) {
		if (tileoption == BUILD_TILEOPTION_H) {
			//tile = OBJ_DOORWAY + 1024 * 0;
		} else if (tileoption == BUILD_TILEOPTION_V) {
			//tile = OBJ_DOORWAY + 1024 * 2;
		} else if (tileoption == BUILD_TILEOPTION_N) {
			tile = OBJ_DOORWAY + 1024 * 2;
		} else if (tileoption == BUILD_TILEOPTION_S) {
			tile = OBJ_DOORWAY + 1024 * 3;
		} else if (tileoption == BUILD_TILEOPTION_W) {
			tile = OBJ_DOORWAY + 1024 * 0;
		} else if (tileoption == BUILD_TILEOPTION_E) {
			tile = OBJ_DOORWAY + 1024 * 1;
		}
	} else {
		tile = objnum + 1024 * 0;
	}

	return tile;
}

void buildwallobj(int x, int y, int length, int buildtype, int objnum, int begintileoption, int endtileoption) {
	int begintile, tile, endtile;
	int somenumber = 0;
	int i, x2, y2;
	bool storageobj = FALSE;

	begintile = getwallobj(objnum, begintileoption);
	endtile = getwallobj(objnum, endtileoption);

	if (objnum == OBJ_TABLE) {
		storageobj = TRUE;
		somenumber = 5;
	}

	if (buildtype == BUILD_TYPE_HORIZONTAL) {
		tile = getwallobj(objnum, BUILD_TILEOPTION_H);

		//OBJaddnew(x, y, tile, 0, 5);

		if (length > 0) {
			x2 = x;
			y2 = y;

			//OBJaddnew(x, y, begintile, 0, somenumber);
			OBJaddnew(x2, y2, begintile, 0, somenumber);
			if (storageobj) {
				if (buildtablewithstorage == BUILD_TABLEWITHSTORAGE_YES) {
					housestoragex[basehousenumber + 0][housestoragenext[basehousenumber + 0]] = patchx + x2;
					housestoragey[basehousenumber + 0][housestoragenext[basehousenumber + 0]] = patchy + y2;
					housestoragenext[basehousenumber + 0]++;
				}
			}
		}

		if (length > 1) {
			x2 = x + length - 1;
			y2 = y;

			//OBJaddnew(x + length - 1, y, endtile, 0, somenumber);
			OBJaddnew(x2, y2, endtile, 0, somenumber);
			if (storageobj) {
				if (buildtablewithstorage == BUILD_TABLEWITHSTORAGE_YES) {
					housestoragex[basehousenumber + 0][housestoragenext[basehousenumber + 0]] = patchx + x2;
					housestoragey[basehousenumber + 0][housestoragenext[basehousenumber + 0]] = patchy + y2;
					housestoragenext[basehousenumber + 0]++;
				}
			}
		}

		y2 = y;
		for (i = x + 1; i < x + length - 1; i++) {
			x2 = i;

			//OBJaddnew(i, y, tile, 0, somenumber);
			OBJaddnew(x2, y2, tile, 0, somenumber);

			if (storageobj) {
				if (buildtablewithstorage == BUILD_TABLEWITHSTORAGE_YES) {
					housestoragex[basehousenumber + 0][housestoragenext[basehousenumber + 0]] = patchx + x2;
					housestoragey[basehousenumber + 0][housestoragenext[basehousenumber + 0]] = patchy + y2;
					housestoragenext[basehousenumber + 0]++;
				}
			}
		}

	} else if (buildtype == BUILD_TYPE_VERTICAL) {
		tile = getwallobj(objnum, BUILD_TILEOPTION_V);

		if (length > 0) {
			x2 = x;
			y2 = y;

			//OBJaddnew(x, y, begintile, 0, somenumber);
			OBJaddnew(x2, y2, begintile, 0, somenumber);

			if (storageobj) {
				if (buildtablewithstorage == BUILD_TABLEWITHSTORAGE_YES) {
					housestoragex[basehousenumber + 0][housestoragenext[basehousenumber + 0]] = patchx + x2;
					housestoragey[basehousenumber + 0][housestoragenext[basehousenumber + 0]] = patchy + y2;
					housestoragenext[basehousenumber + 0]++;
				}
			}
		}

		if (length > 1) {
			x2 = x;
			y2 = y + length - 1;

			//OBJaddnew(x, y + length - 1, endtile, 0, somenumber);
			OBJaddnew(x2, y2, endtile, 0, somenumber);

			if (storageobj) {
				if (buildtablewithstorage == BUILD_TABLEWITHSTORAGE_YES) {
					housestoragex[basehousenumber + 0][housestoragenext[basehousenumber + 0]] = patchx + x2;
					housestoragey[basehousenumber + 0][housestoragenext[basehousenumber + 0]] = patchy + y2;
					housestoragenext[basehousenumber + 0]++;
				}
			}
		}

		x2 = x;
		for (i = y + 1; i < y + length - 1; i++) {
			y2 = i;

			//OBJaddnew(x, i, tile, 0, somenumber);
			OBJaddnew(x2, y2, tile, 0, somenumber);

			if (storageobj) {
				if (buildtablewithstorage == BUILD_TABLEWITHSTORAGE_YES) {
					housestoragex[basehousenumber + 0][housestoragenext[basehousenumber + 0]] = patchx + x2;
					housestoragey[basehousenumber + 0][housestoragenext[basehousenumber + 0]] = patchy + y2;
					housestoragenext[basehousenumber + 0]++;
				}
			}
		}

	}
}

void buildarch(int x, int y, int length, int buildtype, int archstart, int archlength, int wallstyle, int begintileoption, int endtileoption) {
	int floorstyle = BUILD_FLOORSTYLE_BLUE2;

	if (buildtype == BUILD_TYPE_HORIZONTAL) {
		if (archstart > 0)
			buildwall2(x, y, archstart, buildtype, wallstyle, floorstyle, begintileoption, BUILD_TILEOPTION_H);
		//for (i = x; i < x + archstart; i++)
		//	BTset(i, y, tile);

		if (archstart + archlength < length)
			buildwall2(x + archstart + archlength, y, length - archstart - archlength, buildtype, wallstyle, floorstyle, BUILD_TILEOPTION_H, endtileoption);
		//for (i = x + archstart + archlength; i < x + length; i++)
			//BTset(i, y, tile);

		buildfloor(x + archstart, y, archlength, 1, floorstyle);
		buildwallobj(x + archstart, y, archlength, buildtype, OBJ_ARCHWAY, BUILD_TILEOPTION_W, BUILD_TILEOPTION_E);
	} else if (buildtype == BUILD_TYPE_VERTICAL) {
		if (archstart > 0)
			buildwall2(x, y, archstart, buildtype, wallstyle, floorstyle, begintileoption, BUILD_TILEOPTION_V);

		if (archstart + archlength < length)
			buildwall2(x, y + archstart + archlength, length - archstart - archlength, buildtype, wallstyle, floorstyle, BUILD_TILEOPTION_V, endtileoption);

		buildfloor(x, y + archstart, 1, archlength, floorstyle);
		buildwallobj(x, y + archstart, archlength, buildtype, OBJ_ARCHWAY, BUILD_TILEOPTION_N, BUILD_TILEOPTION_S);
	}
}

void buildwallrectobj(int x, int y, int lengthx, int lengthy, int buildtype, int objnum, int skipsideoption) {
	bool skipnorth = FALSE;
	bool skipsouth = FALSE;
	bool skipwest = FALSE;
	bool skipeast = FALSE;
	bool endpoints = FALSE;

	int xadj = 1;
	int yadj = 1;
	int lengthxadj = -2;
	int lengthyadj = -2;
	int begintileoption1 = BUILD_TILEOPTION_NW;
	int endtileoption1 = BUILD_TILEOPTION_NE;
	int begintileoption2 = BUILD_TILEOPTION_SW;
	int endtileoption2 = BUILD_TILEOPTION_SE;
	int begintileoption3 = BUILD_TILEOPTION_V;
	int endtileoption3 = BUILD_TILEOPTION_V;
	int begintileoption4 = BUILD_TILEOPTION_V;
	int endtileoption4 = BUILD_TILEOPTION_V;


	//begintileoption = BUILD_TILEOPTION_H;
	//endtileoption = BUILD_TILEOPTION_H;

	if (skipsideoption - BUILD_SKIPSIDE_ENDPOINTS >= 0) {
		endpoints = TRUE;
		skipsideoption -= BUILD_SKIPSIDE_ENDPOINTS;
	}

	if (skipsideoption - BUILD_SKIPSIDE_EAST >= 0) {
		skipeast = TRUE;
		skipsideoption -= BUILD_SKIPSIDE_EAST;
	}

	if (skipsideoption - BUILD_SKIPSIDE_WEST >= 0) {
		skipwest = TRUE;
		skipsideoption -= BUILD_SKIPSIDE_WEST;
	}

	if (skipsideoption - BUILD_SKIPSIDE_SOUTH >= 0) {
		skipsouth = TRUE;
		skipsideoption -= BUILD_SKIPSIDE_SOUTH;
	}

	if (skipsideoption - BUILD_SKIPSIDE_NORTH >= 0) {
		skipnorth = TRUE;
		skipsideoption -= BUILD_SKIPSIDE_NORTH;
	}


	if (skipnorth) {
		yadj--;
		lengthyadj++;

		if (endpoints) {
			begintileoption3 = BUILD_TILEOPTION_N;
			begintileoption4 = BUILD_TILEOPTION_N;
		}
	}

	if (skipsouth) {
		// yadj--;
		lengthyadj++;

		if (endpoints) {
			endtileoption3 = BUILD_TILEOPTION_S;
			endtileoption4 = BUILD_TILEOPTION_S;
		}
	}

	if (skipwest) {
		xadj--;
		lengthxadj++;
		begintileoption1 = BUILD_TILEOPTION_H;
		begintileoption2 = BUILD_TILEOPTION_H;

		if (endpoints) {
			begintileoption1 = BUILD_TILEOPTION_W;
			begintileoption2 = BUILD_TILEOPTION_W;
		}
	}

	if (skipeast) {
		lengthxadj++;
		endtileoption1 = BUILD_TILEOPTION_H;
		endtileoption2 = BUILD_TILEOPTION_H;

		if (endpoints) {
			endtileoption1 = BUILD_TILEOPTION_E;
			endtileoption2 = BUILD_TILEOPTION_E;
		}
	}

	/*
	if (skipnorth) {
		begintileoption3 = BUILD_TILEOPTION_NW;
		begintileoption4 = BUILD_TILEOPTION_NE;
	}

	if (skipsouth) {
		endtileoption3 = BUILD_TILEOPTION_SW;
		endtileoption4 = BUILD_TILEOPTION_SE;
	}
	*/

	if (!skipnorth)
		buildwallobj(x, y, lengthx, BUILD_TYPE_H, objnum, begintileoption1, endtileoption1);

	if (!skipsouth)
		buildwallobj(x, y + lengthy - 1, lengthx, BUILD_TYPE_H, objnum, begintileoption2, endtileoption2);

	if (!skipwest)
		buildwallobj(x, y + yadj, lengthy + lengthyadj, BUILD_TYPE_V, objnum, begintileoption3, endtileoption3);

	if (!skipeast)
		buildwallobj(x + lengthx - 1, y + yadj, lengthy + lengthyadj, BUILD_TYPE_V, objnum, begintileoption4, endtileoption4);

}


object* builddoor(int x, int y, int type, int doorstyle, int wallstyle, int floorstyle) {
	int begintile, endtile, floortile;
	int i;
	int doorframe1, doorframe2;
	int door1, door2;
	object* door = NULL;

	if (floorstyle == 1) {
	} else if (floorstyle == 2) {
		floortile = 214;
	}

	if (type == BUILD_TYPE_HORIZONTAL) {
		if (doorstyle == 1) {
			door1 = OBJ_PORTCULLIS + 1024 * 0;
			door2 = OBJ_PORTCULLIS + 1024 * 1;
		} else if (doorstyle == 2) {
		}

		if (wallstyle == 1) {
			doorframe1 = OBJ_DOORWAY + 1024 * 0;
			doorframe2 = OBJ_DOORWAY + 1024 * 1;
		} else if (wallstyle == 2) {
		}

		BTset(x - 1, y, floortile);
		BTset(x, y, floortile);

		OBJaddnew(x - 1, y, doorframe1, 0, 0);
		OBJaddnew(x, y, doorframe2, 0, 0);
		//myobj=OBJaddnew(hx,hy,OBJ_STEEL_DOOR+1024*6,0,170);//matched door pair
		//myobj->more=(object*)OBJaddnew(hx,hy+1,OBJ_STEEL_DOOR+1024*7,0,(unsigned long)myobj);//matched door pair
		door = OBJaddnew(x - 1, y, door1, 0, 0);
		object* item102 = OBJaddnew(x, y, door2, 0, 0);
		door->more = item102;
	} else if (type == BUILD_TYPE_VERTICAL) {
		if (doorstyle == 1) {
			door1 = OBJ_PORTCULLIS + 1024 * 2;
			door2 = OBJ_PORTCULLIS + 1024 * 3;
		} else if (doorstyle == 2) {
		}

		if (wallstyle == 1) {
			doorframe1 = OBJ_DOORWAY + 1024 * 2;
			doorframe2 = OBJ_DOORWAY + 1024 * 3;
		} else if (wallstyle == 2) {
		}

		BTset(x, y - 1, floortile);
		BTset(x, y, floortile);

		OBJaddnew(x, y - 1, doorframe1, 0, 0);
		OBJaddnew(x, y, doorframe2, 0, 0);
		//myobj=OBJaddnew(hx,hy,OBJ_STEEL_DOOR+1024*6,0,170);//matched door pair
		//myobj->more=(object*)OBJaddnew(hx,hy+1,OBJ_STEEL_DOOR+1024*7,0,(unsigned long)myobj);//matched door pair
		door = OBJaddnew(x, y - 1, door1, 0, 0);
		object* item102 = OBJaddnew(x, y, door2, 0, 0);
		door->more = item102;
	}

	return door;
}

object* buildlever(int x, int y, object* door) {
	object* lever = OBJaddnew(x, y, OBJ_LEVER + 1024 * 0, 0, 0);
	lever->more = door;

	return lever;
}

void buildstorage(int x, int y, int lengthx, int lengthy, int storagetype) {
	// right of arena
	// row 1 (starting from top row) storage
	//basecx = basex - 2;
	//basecy = basey;

	int basecx, basecy;
	int hx, hy, hi;

	basecx = x - 20;
	basecy = y - 1;

	hx = basecx + 20; hy = basecy + 1;
	OBJaddnew(hx, hy, OBJ_TABLE + 1024 * 3, 0, 5);
	housestoragex[basehousenumber + 0][housestoragenext[basehousenumber + 0]] = patchx + hx;
	housestoragey[basehousenumber + 0][housestoragenext[basehousenumber + 0]] = patchy + hy;
	housestoragenext[basehousenumber + 0]++;

	hx = basecx + 27; hy = basecy + 1;
	OBJaddnew(hx, hy, OBJ_TABLE + 1024 * 5, 0, 5);
	housestoragex[basehousenumber + 0][housestoragenext[basehousenumber + 0]] = patchx + hx;
	housestoragey[basehousenumber + 0][housestoragenext[basehousenumber + 0]] = patchy + hy;
	housestoragenext[basehousenumber + 0]++;

	hx = basecx + 21; hy = basecy + 1;
	for (hx = basecx + 21; hx < basecx + 27; hx++) {
		OBJaddnew(hx, hy, OBJ_TABLE + 1024 * 4, 0, 5);
		housestoragex[basehousenumber + 0][housestoragenext[basehousenumber + 0]] = patchx + hx;
		housestoragey[basehousenumber + 0][housestoragenext[basehousenumber + 0]] = patchy + hy;
		housestoragenext[basehousenumber + 0]++;
	}
	// row 2 storage
	hx = basecx + 20; hy = basecy + 2;
	OBJaddnew(hx, hy, OBJ_TABLE + 1024 * 3, 0, 5);
	housestoragex[basehousenumber + 0][housestoragenext[basehousenumber + 0]] = patchx + hx;
	housestoragey[basehousenumber + 0][housestoragenext[basehousenumber + 0]] = patchy + hy;
	housestoragenext[basehousenumber + 0]++;

	hx = basecx + 27; hy = basecy + 2;
	OBJaddnew(hx, hy, OBJ_TABLE + 1024 * 5, 0, 5);
	housestoragex[basehousenumber + 0][housestoragenext[basehousenumber + 0]] = patchx + hx;
	housestoragey[basehousenumber + 0][housestoragenext[basehousenumber + 0]] = patchy + hy;
	housestoragenext[basehousenumber + 0]++;

	hx = basecx + 21; hy = basecy + 2;
	for (hx = basecx + 21; hx < basecx + 27; hx++) {
		OBJaddnew(hx, hy, OBJ_TABLE + 1024 * 4, 0, 5);
		housestoragex[basehousenumber + 0][housestoragenext[basehousenumber + 0]] = patchx + hx;
		housestoragey[basehousenumber + 0][housestoragenext[basehousenumber + 0]] = patchy + hy;
		housestoragenext[basehousenumber + 0]++;
	}
	// row 3 storage
	hx = basecx + 20; hy = basecy + 4;
	OBJaddnew(hx, hy, OBJ_TABLE + 1024 * 3, 0, 5);
	housestoragex[basehousenumber + 0][housestoragenext[basehousenumber + 0]] = patchx + hx;
	housestoragey[basehousenumber + 0][housestoragenext[basehousenumber + 0]] = patchy + hy;
	housestoragenext[basehousenumber + 0]++;

	hx = basecx + 27; hy = basecy + 4;
	OBJaddnew(hx, hy, OBJ_TABLE + 1024 * 5, 0, 5);
	housestoragex[basehousenumber + 0][housestoragenext[basehousenumber + 0]] = patchx + hx;
	housestoragey[basehousenumber + 0][housestoragenext[basehousenumber + 0]] = patchy + hy;
	housestoragenext[basehousenumber + 0]++;

	hx = basecx + 21; hy = basecy + 4;
	for (hx = basecx + 21; hx < basecx + 27; hx++) {
		OBJaddnew(hx, hy, OBJ_TABLE + 1024 * 4, 0, 5);
		housestoragex[basehousenumber + 0][housestoragenext[basehousenumber + 0]] = patchx + hx;
		housestoragey[basehousenumber + 0][housestoragenext[basehousenumber + 0]] = patchy + hy;
		housestoragenext[basehousenumber + 0]++;
	}
	// row 4 storage
	hx = basecx + 20; hy = basecy + 5;
	OBJaddnew(hx, hy, OBJ_TABLE + 1024 * 3, 0, 5);
	housestoragex[basehousenumber + 0][housestoragenext[basehousenumber + 0]] = patchx + hx;
	housestoragey[basehousenumber + 0][housestoragenext[basehousenumber + 0]] = patchy + hy;
	housestoragenext[basehousenumber + 0]++;

	hx = basecx + 27; hy = basecy + 5;
	OBJaddnew(hx, hy, OBJ_TABLE + 1024 * 5, 0, 5);
	housestoragex[basehousenumber + 0][housestoragenext[basehousenumber + 0]] = patchx + hx;
	housestoragey[basehousenumber + 0][housestoragenext[basehousenumber + 0]] = patchy + hy;
	housestoragenext[basehousenumber + 0]++;

	hx = basecx + 21; hy = basecy + 5;
	for (hx = basecx + 21; hx < basecx + 27; hx++) {
		OBJaddnew(hx, hy, OBJ_TABLE + 1024 * 4, 0, 5);
		housestoragex[basehousenumber + 0][housestoragenext[basehousenumber + 0]] = patchx + hx;
		housestoragey[basehousenumber + 0][housestoragenext[basehousenumber + 0]] = patchy + hy;
		housestoragenext[basehousenumber + 0]++;
	}
	// row 5 storage
	hx = basecx + 20; hy = basecy + 7;
	OBJaddnew(hx, hy, OBJ_TABLE + 1024 * 3, 0, 5);
	housestoragex[basehousenumber + 0][housestoragenext[basehousenumber + 0]] = patchx + hx;
	housestoragey[basehousenumber + 0][housestoragenext[basehousenumber + 0]] = patchy + hy;
	housestoragenext[basehousenumber + 0]++;

	hx = basecx + 27; hy = basecy + 7;
	OBJaddnew(hx, hy, OBJ_TABLE + 1024 * 5, 0, 5);
	housestoragex[basehousenumber + 0][housestoragenext[basehousenumber + 0]] = patchx + hx;
	housestoragey[basehousenumber + 0][housestoragenext[basehousenumber + 0]] = patchy + hy;
	housestoragenext[basehousenumber + 0]++;

	hx = basecx + 21; hy = basecy + 7;
	for (hx = basecx + 21; hx < basecx + 27; hx++) {
		OBJaddnew(hx, hy, OBJ_TABLE + 1024 * 4, 0, 5);
		housestoragex[basehousenumber + 0][housestoragenext[basehousenumber + 0]] = patchx + hx;
		housestoragey[basehousenumber + 0][housestoragenext[basehousenumber + 0]] = patchy + hy;
		housestoragenext[basehousenumber + 0]++;
	}
	// row 6 storage
	hx = basecx + 20; hy = basecy + 8;
	OBJaddnew(hx, hy, OBJ_TABLE + 1024 * 3, 0, 5);
	housestoragex[basehousenumber + 0][housestoragenext[basehousenumber + 0]] = patchx + hx;
	housestoragey[basehousenumber + 0][housestoragenext[basehousenumber + 0]] = patchy + hy;
	housestoragenext[basehousenumber + 0]++;

	hx = basecx + 27; hy = basecy + 8;
	OBJaddnew(hx, hy, OBJ_TABLE + 1024 * 5, 0, 5);
	housestoragex[basehousenumber + 0][housestoragenext[basehousenumber + 0]] = patchx + hx;
	housestoragey[basehousenumber + 0][housestoragenext[basehousenumber + 0]] = patchy + hy;
	housestoragenext[basehousenumber + 0]++;

	hx = basecx + 21; hy = basecy + 8;
	for (hx = basecx + 21; hx < basecx + 27; hx++) {
		OBJaddnew(hx, hy, OBJ_TABLE + 1024 * 4, 0, 5);
		housestoragex[basehousenumber + 0][housestoragenext[basehousenumber + 0]] = patchx + hx;
		housestoragey[basehousenumber + 0][housestoragenext[basehousenumber + 0]] = patchy + hy;
		housestoragenext[basehousenumber + 0]++;
	}
}


