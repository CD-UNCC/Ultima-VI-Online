/*
	Helper functions to test flags/bits and improve readability of code
	I couldn't inline these functions in data_both.h because they require globals defined later
	Xenkan 2010-12-05
*/

#ifdef HOST

#include "data_both.h"	// for object and player structs
#include "function_host.h"	// for OBJlist

bool object::IsStackable()
	{ return obji[sprlnk[type&1023]+(type>>10)].flags&4096; }

bool object::IsContainer()
	{ return obji[sprlnk[type&1023]].flags&1024; }

void player::GetPartyInventory(object*** Items, uint4* ItemCount)
	{
	for (int p = 0; p < 8 && tplayer->party[p]; p++)
		*ItemCount = OBJlist((object*)(((npc*)(tplayer->party[p]->more))->items->more));
	*Items = OBJlist_list;
	OBJlist_last = 0;
	}

#endif //HOST