#include "/home/codeleaded/System/Static/Library/WindowEngine.h"
#include "/home/codeleaded/System/Static/Library/Random.h"
#include "/home/codeleaded/System/Static/Library/TransformedView.h"

typedef unsigned char WFC_Option;

#define WFC_OPTIONS					7U
#define WFC_OPTIONS_COUNT			(WFC_OPTIONS - 1U)
#define WFC_OPTION_NONE				0U
#define WFC_OPTION_GRAY				1U
#define WFC_OPTION_GREEN			2U
#define WFC_OPTION_GTURG			3U
#define WFC_OPTION_GGRAYG			4U
#define WFC_OPTION_GRAYGREEN		5U
#define WFC_OPTION_GREENGRAY		6U

#define WFC_DIRECTIONS				4U
#define WFC_TILE_COUNT				36U
#define WFC_TILE_INVALID			0xFFFFFFFFU
#define WFC_TILE_ENTROPY_MAX		0xFFFFFFFFU

#define WFC_TILE_RETURN_CONTINUE	0U
#define WFC_TILE_RETURN_CANCEL		1U
#define WFC_TILE_RETURN_REDO		2U

#define WFC_TILE_X	15
#define WFC_TILE_Y	15

unsigned char WFC_OPTION[6][6][6][6] = {
	{
		{ {0,0,0,0,0,1},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0} },
		{ {0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0} },
		{ {0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0} },
		{ {0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0} },
		{ {0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,1,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0} },
		{ {0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0} }
	},
	{
		{ {0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0} },
		{ {0,0,0,0,0,0},{0,0,0,1,1,0},{0,0,0,1,1,0},{0,0,0,0,0,0},{1,0,0,0,0,0},{0,0,0,0,0,0} },
		{ {0,0,0,0,0,0},{0,0,1,1,1,0},{0,0,0,1,1,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0} },
		{ {0,0,0,0,0,0},{0,0,1,1,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0} },
		{ {0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0} },
		{ {0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,1,0} }
	},
	{
		{ {0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0} },
		{ {0,0,0,0,0,0},{0,0,0,1,1,0},{0,0,0,1,1,0},{0,0,0,0,1,0},{0,0,0,0,0,0},{0,0,0,0,0,0} },
		{ {0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,1,1,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0} },
		{ {0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,1,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0} },
		{ {0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0} },
		{ {1,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0} }
	},
	{
		{ {0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0} },
		{ {0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,1,0},{0,0,0,0,1,0},{0,0,0,0,0,0},{0,0,0,0,0,0} },
		{ {0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,1,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0} },
		{ {0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0} },
		{ {0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0} },
		{ {0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0} }
	},
	{
		{ {0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0} },
		{ {0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0} },
		{ {0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,1},{0,0,0,0,0,0} },
		{ {0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0} },
		{ {0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0} },
		{ {0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0} }
	},
	{
		{ {0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,1,0,0} },
		{ {0,0,0,0,0,0},{0,1,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0} },
		{ {0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0} },
		{ {0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0} },
		{ {0,0,0,0,0,0},{0,0,0,0,1,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0} },
		{ {0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0} }
	},
};
WFC_Option WFC_ALLOWED[WFC_TILE_COUNT][WFC_DIRECTIONS] = {
	{ 1U,1U,1U,1U },
	{ 1U,5U,3U,5U },
	{ 2U,2U,2U,2U },
	{ 2U,2U,2U,3U },
	{ 2U,2U,3U,2U },
	{ 2U,2U,3U,3U },
	{ 2U,2U,5U,6U },
	{ 2U,3U,2U,2U },
	{ 2U,3U,2U,3U },
	{ 2U,3U,2U,3U },
	{ 2U,3U,2U,4U },
	{ 2U,3U,3U,2U },
	{ 2U,3U,3U,3U },
	{ 2U,4U,2U,3U },
	{ 2U,4U,2U,4U },
	{ 2U,6U,6U,2U },
	{ 3U,2U,2U,2U },
	{ 3U,2U,2U,3U },
	{ 3U,2U,3U,2U },
	{ 3U,2U,3U,2U },
	{ 3U,2U,3U,3U },
	{ 3U,2U,4U,2U },
	{ 3U,3U,2U,2U },
	{ 3U,3U,2U,3U },
	{ 3U,3U,3U,2U },
	{ 3U,3U,3U,3U },
	{ 3U,3U,3U,3U },
	{ 3U,4U,3U,4U },
	{ 3U,6U,1U,6U },
	{ 4U,2U,3U,2U },
	{ 4U,2U,4U,2U },
	{ 4U,3U,4U,3U },
	{ 5U,3U,5U,1U },
	{ 6U,1U,6U,3U },
	{ 6U,2U,2U,5U },
	{ 6U,5U,2U,2U }
};

typedef struct WFC_Tile_Pair {
	unsigned int i;
	unsigned int entropy;
} WFC_Tile_Pair;

typedef struct WFC_Tile {
	WFC_Option u;
	WFC_Option d;
	WFC_Option l;
	WFC_Option r;
} WFC_Tile;

Vic2 WFC_Tile_Direction(unsigned int dir){
	if(dir == 0U) return (Vic2){  0,-1 };
	if(dir == 1U) return (Vic2){  0, 1 };
	if(dir == 2U) return (Vic2){ -1, 0 };
	if(dir == 3U) return (Vic2){  1, 0 };
	return (Vic2){  0, 0 };
}
char WFC_Tile_Visited(WFC_Tile t){
	return !(t.u == WFC_OPTION_NONE && t.d == WFC_OPTION_NONE && t.l == WFC_OPTION_NONE && t.r == WFC_OPTION_NONE);
}
unsigned int WFC_Tile_Index(WFC_Tile t){
	WFC_Option* top = (WFC_Option*)&t;
	
	for(int i = 0;i<sizeof(WFC_ALLOWED) / sizeof(*WFC_ALLOWED);i++){
		for(int j = 0;j<sizeof(*WFC_ALLOWED) / sizeof(**WFC_ALLOWED);j++){
			if(top[j] != WFC_ALLOWED[i][j]) 
				break;
			if(j + 1U == sizeof(*WFC_ALLOWED) / sizeof(**WFC_ALLOWED))
				return i;
		}
	}
	return WFC_TILE_INVALID;
}
void WFC_Tile_Reset(WFC_Tile* map,unsigned int w,unsigned int h,Vic2 c){
	WFC_Tile* const set = map + c.y * w + c.x;
	set->u = WFC_OPTION_NONE;
	set->d = WFC_OPTION_NONE;
	set->l = WFC_OPTION_NONE;
	set->r = WFC_OPTION_NONE;
}




TransformedView tv;
Vector sprites;
WFC_Tile* map;
Thread builder;

/*
unsigned int WFC_Tile_AccValid(unsigned int indecies[],unsigned int dir){
	unsigned char* options = (
		dir == 3U ?
		WFC_OPTION[indecies[0]][indecies[1]][indecies[2]] :
		(unsigned char*)WFC_OPTION
	);
	
	unsigned int acc = 0U;
	for(int i = 0;i<WFC_OPTIONS_COUNT;i++){
		if(dir < WFC_DIRECTIONS - 1U){
			indecies[dir] = i;
			acc += WFC_Tile_AccValid(indecies,dir + 1U);
		}else{
			acc += options[i];
		}
	}
	return acc;
}
char WFC_Tile_SetValid(WFC_Tile* tile,unsigned int dir,WFC_Option op){
	WFC_Option* tile_op = (WFC_Option*)tile;
	unsigned int indecies[WFC_DIRECTIONS];
	
	for(int i = 0;i<dir;i++){
		indecies[i] = (tile_op[i] > 0U ? tile_op[i] : 1U) - 1U;
	}

	if(op > 0U){
		indecies[dir] = op;
		const unsigned int acc = WFC_Tile_AccValid(indecies,dir + 1U);
		
		if(acc > 0U)	tile_op[dir] = op;
		else			return WFC_TILE_RETURN_CANCEL;
		
		return WFC_TILE_RETURN_CONTINUE;
	}else{
		unsigned int acc[WFC_OPTIONS_COUNT];
		unsigned int found = 0U;
		
		for(int i = 0;i<WFC_OPTIONS_COUNT;i++){
			indecies[dir] = i;
			acc[i] = WFC_Tile_AccValid(indecies,dir + 1U);
			found += acc[i];
		}

		while(found > 0U){
			const WFC_Option ops = (WFC_Option)Random_u32_MinMax(0U,WFC_OPTIONS_COUNT);
			if(acc[ops] > 0U){
				tile_op[dir] = ops + 1U;
				return WFC_TILE_RETURN_CONTINUE;
			}
		}

		return WFC_TILE_RETURN_CANCEL;
	}
}
char WFC_Tile_SetR(WFC_Tile* map,unsigned int w,unsigned int h,const Vic2 c){
	WFC_Tile* sett = map + c.y * w + c.x;
	WFC_Option* opt = (WFC_Option*)sett;
	
	for(unsigned int i = 0;i<WFC_DIRECTIONS;i++){
		const Vic2 d = WFC_Tile_Direction(i);
		const Vic2 p = Vic2_Add(c,d);

		if(p.x >= w || p.y >= h){
			//opt[i] = (WFC_Option)Random_u32_MinMax(1U,1U + WFC_OPTIONS);
			opt[i] = 2U;
			continue;
		}

		WFC_Tile* const nt = map + p.y * w + p.x;
		WFC_Option* const nop = (WFC_Option*)nt;

		const unsigned int invi = (i / 2 * 2) + ((i + 1) % 2);
		WFC_Option const op = nop[invi];

		if(WFC_Tile_SetValid(sett,i,op))
			return WFC_TILE_RETURN_CANCEL;
	}
	
	return WFC_TILE_RETURN_CONTINUE;
}
char WFC_Tile_AllN(WFC_Tile* map,unsigned int w,unsigned int h,Vic2 c){
	if(c.x >= w || c.y >= h)
		return 0;

	if(WFC_Tile_SetR(map,w,h,c)){
		WFC_Tile_Reset(map,w,h,c);
		return WFC_TILE_RETURN_CANCEL;
	}

	char neighbours_set[WFC_DIRECTIONS];
	memset(neighbours_set,0,sizeof(neighbours_set));
	
	for(int i = 0;i<WFC_DIRECTIONS;i++){
		const Vic2 d = WFC_Tile_Direction(i);
		const Vic2 p = Vic2_Add(c,d);

		if(p.x >= w || p.y >= h)
			continue;

		WFC_Tile* const nt = map + p.y * w + p.x;
		WFC_Option* const nop = (WFC_Option*)nt;

		if(!WFC_Tile_Visited(*nt)){
			const char rec = WFC_Tile_AllN(map,w,h,p);
			if(rec == WFC_TILE_RETURN_CANCEL || (rec == WFC_TILE_RETURN_REDO && i + 1 == WFC_DIRECTIONS)){
				WFC_Tile_Reset(map,w,h,c);

				for(int j = 0;j<WFC_DIRECTIONS;j++){
					const Vic2 d_j = WFC_Tile_Direction(j);
					const Vic2 p_j = Vic2_Add(c,d_j);
					if(neighbours_set[j])
						WFC_Tile_Reset(map,w,h,p_j);
				}

				return WFC_TILE_RETURN_REDO;
			}
			if(rec == WFC_TILE_RETURN_CONTINUE){
				neighbours_set[i] = 1;
			}
			//else if(rec == WFC_TILE_RETURN_REDO){
			//	i--;
			//}
		}
	}

	return WFC_TILE_RETURN_CONTINUE;
}
*/

WFC_Tile WFC_Tile_GenerateTile(WFC_Tile t){
	WFC_Option* top = (WFC_Option*)&t;
	unsigned int acc[WFC_TILE_COUNT];
	unsigned int count = 0U;
	
	for(unsigned int i = 0;i<sizeof(WFC_ALLOWED) / sizeof(*WFC_ALLOWED);i++){
		for(unsigned int j = 0;j<sizeof(*WFC_ALLOWED) / sizeof(**WFC_ALLOWED);j++){
			if(top[j] != WFC_OPTION_NONE && top[j] != WFC_ALLOWED[i][j]) 
				break;
			if(j + 1U == sizeof(*WFC_ALLOWED) / sizeof(**WFC_ALLOWED)){
				acc[count] = i;
				count++;
			}
		}
	}

	if(count > 0U){
		const unsigned int rand_index = Random_u64_MinMax(0U,count);
		const unsigned int real_index = acc[rand_index];
		
		return (WFC_Tile){
			.u = WFC_ALLOWED[real_index][0],
			.d = WFC_ALLOWED[real_index][1],
			.l = WFC_ALLOWED[real_index][2],
			.r = WFC_ALLOWED[real_index][3]
		};
	}

	return (WFC_Tile){
		.u = WFC_OPTION_NONE,
		.d = WFC_OPTION_NONE,
		.l = WFC_OPTION_NONE,
		.r = WFC_OPTION_NONE
	};
}
unsigned int WFC_Tile_EntropyCount(WFC_Tile t){
	WFC_Option* top = (WFC_Option*)&t;
	unsigned int count = 0U;
	
	for(unsigned int i = 0;i<sizeof(WFC_ALLOWED) / sizeof(*WFC_ALLOWED);i++){
		for(unsigned int j = 0;j<sizeof(*WFC_ALLOWED) / sizeof(**WFC_ALLOWED);j++){
			if(top[j] != WFC_OPTION_NONE && top[j] != WFC_ALLOWED[i][j]) 
				break;
			if(j + 1U == sizeof(*WFC_ALLOWED) / sizeof(**WFC_ALLOWED)){
				count++;
			}
		}
	}

	return count;
}
unsigned int WFC_Tile_Entropy(WFC_Tile* map,unsigned int w,unsigned int h,Vic2 c){
	WFC_Tile tile = {
		.u = WFC_OPTION_NONE,
		.d = WFC_OPTION_NONE,
		.l = WFC_OPTION_NONE,
		.r = WFC_OPTION_NONE
	};
	WFC_Option* top = (WFC_Option*)&tile;

	for(int i = 0;i<WFC_DIRECTIONS;i++){
		const Vic2 d = WFC_Tile_Direction(i);
		const Vic2 p = Vic2_Add(c,d);

		if(p.x >= w || p.y >= h)
			continue;

		WFC_Tile* const nt = map + p.y * w + p.x;
		WFC_Option* const nop = (WFC_Option*)nt;

		const unsigned int invi = (i / 2 * 2) + ((i + 1) % 2);
		WFC_Option const op = nop[invi];

		top[i] = op;
	}

	return WFC_Tile_EntropyCount(tile);
}

int WFC_Tile_Entropy_Cmp(const void* e1,const void* e2){
	const WFC_Tile_Pair v1 = *(WFC_Tile_Pair*)e1;
	const WFC_Tile_Pair v2 = *(WFC_Tile_Pair*)e2;
	return v1.entropy > v2.entropy ? 1 : (v1.entropy < v2.entropy ? -1 : 0);
}
char WFC_Tile_SetR(WFC_Tile* map,unsigned int w,unsigned int h,const Vic2 c){
	WFC_Tile* sett = map + c.y * w + c.x;
	WFC_Option* opt = (WFC_Option*)sett;
	
	WFC_Tile tile = {
		.u = WFC_OPTION_NONE,
		.d = WFC_OPTION_NONE,
		.l = WFC_OPTION_NONE,
		.r = WFC_OPTION_NONE
	};
	WFC_Option* top = (WFC_Option*)&tile;

	for(int i = 0;i<WFC_DIRECTIONS;i++){
		const Vic2 d = WFC_Tile_Direction(i);
		const Vic2 p = Vic2_Add(c,d);

		if(p.x >= w || p.y >= h)
			continue;

		WFC_Tile* const nt = map + p.y * w + p.x;
		WFC_Option* const nop = (WFC_Option*)nt;

		const unsigned int invi = (i / 2 * 2) + ((i + 1) % 2);
		WFC_Option const op = nop[invi];

		top[i] = op;
	}

	*sett = WFC_Tile_GenerateTile(tile);
	return WFC_TILE_RETURN_CONTINUE;
}
char WFC_Tile_AllN(WFC_Tile* map,unsigned int w,unsigned int h,Vic2 c){
	if(c.x >= w || c.y >= h)
		return 0;

	WFC_Tile_SetR(map,w,h,c);
	
	WFC_Tile_Pair entropies[WFC_DIRECTIONS];
	memset(entropies,0,sizeof(entropies));

	for(unsigned int i = 0;i<WFC_DIRECTIONS;i++){
		entropies[i].i = i;

		const Vic2 d = WFC_Tile_Direction(i);
		const Vic2 p = Vic2_Add(c,d);

		if(p.x >= w || p.y >= h){
			entropies[i].entropy = WFC_TILE_ENTROPY_MAX;
			continue;
		}

		WFC_Tile* const nt = map + p.y * w + p.x;
		WFC_Option* const nop = (WFC_Option*)nt;

		if(WFC_Tile_Visited(*nt)){
			entropies[i].entropy = WFC_TILE_ENTROPY_MAX;
		}else{
			entropies[i].entropy = WFC_Tile_Entropy(map,w,h,p);
		}
	}

	qsort(
		entropies,
		sizeof(entropies) / sizeof(*entropies),
		sizeof(*entropies),
		WFC_Tile_Entropy_Cmp
	);

	for(unsigned int i = 0;i<sizeof(entropies) / sizeof(*entropies);i++){
		if(entropies[i].entropy == WFC_TILE_ENTROPY_MAX) break;
		
		const int e_index = entropies[i].i;
		const Vic2 d = WFC_Tile_Direction(e_index);
		const Vic2 p = Vic2_Add(c,d);
		WFC_Tile_AllN(map,w,h,p);
	}

	return WFC_TILE_RETURN_CONTINUE;
}
void WFC_Tile_Fill(WFC_Tile* map,unsigned int w,unsigned int h,Vic2 c){
	while(WFC_Tile_AllN(map,w,h,c));
}
