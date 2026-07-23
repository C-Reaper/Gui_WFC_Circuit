#include "/home/codeleaded/System/Static/Library/WindowEngine.h"
#include "/home/codeleaded/System/Static/Library/Random.h"
#include "/home/codeleaded/System/Static/Library/TransformedView.h"

typedef unsigned char WFC_Tile_Id;
typedef unsigned char WFC_Tile_Dir;
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
#define WFC_TILE_INVALID			0xFFU
#define WFC_TILE_ENTROPY_MAX		0xFFFFFFFFU

#define WFC_TILE_RETURN_CONTINUE	0U
#define WFC_TILE_RETURN_CANCEL		1U
#define WFC_TILE_RETURN_REDO		2U

#define WFC_TILE_X	100
#define WFC_TILE_Y	100

WFC_Option WFC_ALLOWED[WFC_TILE_COUNT][WFC_DIRECTIONS] = {
	{ 1U,1U,1U,1U },
	{ 1U,3U,5U,5U },
	{ 2U,2U,2U,2U },
	{ 2U,2U,2U,3U },
	{ 2U,2U,3U,2U },
	{ 2U,2U,3U,3U },
	{ 2U,2U,3U,3U },
	{ 2U,2U,3U,4U },
	{ 2U,2U,4U,3U },
	{ 2U,2U,4U,4U },
	{ 2U,3U,2U,2U },
	{ 2U,3U,2U,3U },
	{ 2U,3U,3U,2U },
	{ 2U,3U,3U,3U },
	{ 2U,5U,6U,2U },
	{ 2U,6U,2U,6U },
	{ 3U,1U,6U,6U },
	{ 3U,2U,2U,2U },
	{ 3U,2U,2U,3U },
	{ 3U,2U,3U,2U },
	{ 3U,2U,3U,3U },
	{ 3U,3U,2U,2U },
	{ 3U,3U,2U,2U },
	{ 3U,3U,2U,3U },
	{ 3U,3U,3U,2U },
	{ 3U,3U,3U,3U },
	{ 3U,3U,3U,3U },
	{ 3U,3U,4U,4U },
	{ 3U,4U,2U,2U },
	{ 4U,3U,2U,2U },
	{ 4U,4U,2U,2U },
	{ 4U,4U,3U,3U },
	{ 5U,5U,1U,3U },
	{ 6U,2U,2U,5U },
	{ 6U,2U,5U,2U },
	{ 6U,6U,3U,1U }
};

typedef struct WFC_Tile_Pair {
	unsigned int i;
	unsigned int entropy;
} WFC_Tile_Pair;

typedef struct WFC_Tile_BT {
	unsigned char paths[WFC_DIRECTIONS];
	WFC_Tile_Id types[WFC_TILE_COUNT];
	Vic2 pos;
} WFC_Tile_BT;

typedef struct WFC_Tile {
	WFC_Option u;
	WFC_Option d;
	WFC_Option l;
	WFC_Option r;
} WFC_Tile;

TransformedView tv;
Vector sprites;
WFC_Tile_Id* map;
Thread builder;

WFC_Tile_BT WFC_Tile_BT_New(Vic2 p){
	WFC_Tile_BT bt;
	memset(&bt,0,sizeof(bt));
	bt.pos = p;
	return bt;
}
WFC_Tile_Dir WFC_Tile_Inv(WFC_Tile_Dir i){
	const WFC_Tile_Dir opposite[4] = { 1U,0U,3U,2U };
	return opposite[i];
}
char WFC_Tile_Valid(WFC_Tile_Id i){
	return i < sizeof(WFC_ALLOWED) / sizeof(*WFC_ALLOWED);
}
Vic2 WFC_Tile_Direction(WFC_Tile_Dir dir){
	if(dir == 0U) return (Vic2){  0,-1 };
	if(dir == 1U) return (Vic2){  0, 1 };
	if(dir == 2U) return (Vic2){ -1, 0 };
	if(dir == 3U) return (Vic2){  1, 0 };
	return (Vic2){  0, 0 };
}
char WFC_Tile_Visited(WFC_Tile t){
	return !(t.u == WFC_OPTION_NONE && t.d == WFC_OPTION_NONE && t.l == WFC_OPTION_NONE && t.r == WFC_OPTION_NONE);
}
WFC_Tile_Id WFC_Tile_Index(WFC_Tile t){
	WFC_Option* top = (WFC_Option*)&t;
	
	for(WFC_Tile_Id i = 0;i<sizeof(WFC_ALLOWED) / sizeof(*WFC_ALLOWED);i++){
		for(WFC_Tile_Dir j = 0;j<sizeof(*WFC_ALLOWED) / sizeof(**WFC_ALLOWED);j++){
			if(top[j] != WFC_ALLOWED[i][j]) 
				break;
			if(j + 1U == sizeof(*WFC_ALLOWED) / sizeof(**WFC_ALLOWED))
				return i;
		}
	}
	return WFC_TILE_INVALID;
}
void WFC_Tile_Reset(WFC_Tile_Id* map,unsigned int w,unsigned int h,Vic2 c){
	map[c.y * w + c.x] = WFC_TILE_INVALID;
}

WFC_Tile WFC_Tile_GetTile(unsigned int t_index){
	if(t_index < sizeof(WFC_ALLOWED) / sizeof(*WFC_ALLOWED)){
		return (WFC_Tile){
			.u = WFC_ALLOWED[t_index][0],
			.d = WFC_ALLOWED[t_index][1],
			.l = WFC_ALLOWED[t_index][2],
			.r = WFC_ALLOWED[t_index][3]
		};
	}else{
		return (WFC_Tile){
			.u = WFC_OPTION_NONE,
			.d = WFC_OPTION_NONE,
			.l = WFC_OPTION_NONE,
			.r = WFC_OPTION_NONE
		};
	}
}
unsigned int WFC_Tile_GenerateTile(WFC_Tile t,WFC_Tile_BT* bt){
	WFC_Option* top = (WFC_Option*)&t;
	unsigned int acc[WFC_TILE_COUNT];
	unsigned int count = 0U;
	
	for(WFC_Tile_Id i = 0;i<sizeof(WFC_ALLOWED) / sizeof(*WFC_ALLOWED);i++){
		for(WFC_Tile_Dir j = 0;j<sizeof(*WFC_ALLOWED) / sizeof(**WFC_ALLOWED);j++){
			if(top[j] != WFC_OPTION_NONE && top[j] != WFC_ALLOWED[i][j]) 
				break;

			if(j + 1U == sizeof(*WFC_ALLOWED) / sizeof(**WFC_ALLOWED) && !bt->types[i]){
				acc[count] = i;
				count++;
			}
		}
	}

	if(count > 0U){
		const WFC_Tile_Id rand_index = Random_u64_MinMax(0U,count);
		const WFC_Tile_Id real_index = acc[rand_index];
		return real_index;
	}

	return WFC_TILE_INVALID;
}
WFC_Tile WFC_Tile_NeighbourToTile(WFC_Tile_Id* map,unsigned int w,unsigned int h,const Vic2 c){
	WFC_Tile tile = {
		.u = WFC_OPTION_NONE,
		.d = WFC_OPTION_NONE,
		.l = WFC_OPTION_NONE,
		.r = WFC_OPTION_NONE
	};
	WFC_Option* top = (WFC_Option*)&tile;

	for(WFC_Tile_Dir i = 0;i<WFC_DIRECTIONS;i++){
		const Vic2 d = WFC_Tile_Direction(i);
		const Vic2 p = Vic2_Add(c,d);

		if(p.x < 0 || p.y < 0 || p.x >= w || p.y >= h)
			continue;

		const WFC_Tile_Id nt = map[p.y * w + p.x];
		
		if(WFC_Tile_Valid(nt)){
			const WFC_Tile_Dir invi = WFC_Tile_Inv(i);
			const WFC_Option op = WFC_ALLOWED[nt][invi];
			top[i] = op;
		}else{
			top[i] = WFC_OPTION_NONE;
		}
	}

	return tile;
}
unsigned int WFC_Tile_EntropyCount(WFC_Tile t){
	WFC_Option* top = (WFC_Option*)&t;
	unsigned int count = 0U;
	
	for(WFC_Tile_Id i = 0;i<sizeof(WFC_ALLOWED) / sizeof(*WFC_ALLOWED);i++){
		for(WFC_Tile_Dir j = 0;j<sizeof(*WFC_ALLOWED) / sizeof(**WFC_ALLOWED);j++){
			if(top[j] != WFC_OPTION_NONE && top[j] != WFC_ALLOWED[i][j]) 
				break;
			if(j + 1U == sizeof(*WFC_ALLOWED) / sizeof(**WFC_ALLOWED)){
				count++;
			}
		}
	}

	return count;
}
unsigned int WFC_Tile_Entropy(WFC_Tile_Id* map,unsigned int w,unsigned int h,Vic2 c){
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

		if(p.x < 0 || p.y < 0 || p.x >= w || p.y >= h)
			continue;

		const WFC_Tile_Id nt = map[p.y * w + p.x];
		
		if(WFC_Tile_Valid(nt)){
			const WFC_Tile_Dir invi = WFC_Tile_Inv(i);
			const WFC_Option op = WFC_ALLOWED[nt][invi];
			top[i] = op;
		}else{
			top[i] = WFC_OPTION_NONE;
		}
	}

	return WFC_Tile_EntropyCount(tile);
}

WFC_Tile_Dir WFC_Tile_EntropyNeighbour(WFC_Tile_Id* map,Vector* bt,unsigned int w,unsigned int h){
	WFC_Tile_BT* const top = (WFC_Tile_BT*)Vector_Get(bt,bt->size - 1U);
	const Vic2 pos = top->pos;

	WFC_Tile_Dir entropy_i = WFC_TILE_INVALID;
	unsigned int entropy_min = WFC_TILE_ENTROPY_MAX;
	
	for(unsigned int i = 0;i<WFC_DIRECTIONS;i++){
		const Vic2 n_d = WFC_Tile_Direction(i);
		const Vic2 n_p = Vic2_Add(pos,n_d);
		
		if(n_p.x < 0 || n_p.y < 0 || n_p.x >= w || n_p.y >= h)
			continue;

		const unsigned int nt = map[n_p.y * w + n_p.x];
		const unsigned int entropy = WFC_Tile_Entropy(map,w,h,n_p);

		if(!WFC_Tile_Valid(nt) && !top->paths[i] && entropy < entropy_min){
			entropy_i = i;
			entropy_min = entropy;
		}
	}

	return entropy_i;
}
void WFC_Tile_Step(WFC_Tile_Id* map,Vector* bt,unsigned int w,unsigned int h){
	WFC_Tile_BT* const top = (WFC_Tile_BT*)Vector_Get(bt,bt->size - 1U);
	const Vic2 pos = top->pos;
	const WFC_Tile_Dir entropy_i = WFC_Tile_EntropyNeighbour(map,bt,w,h);
	
	if(entropy_i < WFC_DIRECTIONS){
		top->paths[entropy_i] = 1U;
		const Vic2 n_d = WFC_Tile_Direction(entropy_i);
		const Vic2 n_p = Vic2_Add(pos,n_d);
		Vector_Push(bt,(WFC_Tile_BT[]){ WFC_Tile_BT_New(n_p) });
	}else if(bt->size < WFC_TILE_X * WFC_TILE_Y){
		WFC_Tile_Reset(map,w,h,pos);
	}else{
		Vector_Clear(bt);
	}
}
void WFC_Tile_AllN(WFC_Tile_Id* map,Vector* bt,unsigned int w,unsigned int h,Vic2 start_pos){
	Vector_Push(bt,(WFC_Tile_BT[]){ WFC_Tile_BT_New(start_pos) });
	
	while(bt->size > 0){
		WFC_Tile_BT* const top = (WFC_Tile_BT*)Vector_Get(bt,bt->size - 1U);
		const Vic2 pos = top->pos;
		const WFC_Tile_Id tile = map[pos.y * w + pos.x];
		
		if(!WFC_Tile_Valid(tile)){
			WFC_Tile allowed_tile = WFC_Tile_NeighbourToTile(map,w,h,pos);
			const WFC_Tile_Id t_index = WFC_Tile_GenerateTile(allowed_tile,top);
			memset(top->paths,0,sizeof(top->paths));
			
			if(WFC_Tile_Valid(t_index)){
				top->types[t_index] = 1U;
				map[pos.y * w + pos.x] = t_index;
				WFC_Tile_Step(map,bt,w,h);
			}else{
				Vector_PopTop(bt);
			}
		}else{
			WFC_Tile_Step(map,bt,w,h);
		}
	}
}
void WFC_Tile_Fill(WFC_Tile_Id* map,unsigned int w,unsigned int h,Vic2 c){
	Vector bt = Vector_New(sizeof(WFC_Tile_BT));
	WFC_Tile_AllN(map,&bt,w,h,c);
	Vector_Free(&bt);
}
