#include "/home/codeleaded/System/Static/Library/WindowEngine1.0.h"
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
#define WFC_TILE_COUNT				48U // 36U
#define WFC_TILE_INVALID			0xFFU
#define WFC_TILE_ENTROPY_MAX		0xFFFFFFFFU

#define WFC_TILE_X	100
#define WFC_TILE_Y	100

WFC_Option WFC_ALLOWED[WFC_TILE_COUNT][WFC_DIRECTIONS] = {
	{ 1U,1U,1U,1U },
	{ 1U,1U,7U,7U },
	{ 1U,1U,7U,7U },
	{ 1U,2U,5U,5U },
	{ 1U,3U,5U,5U },
	{ 2U,1U,6U,6U },
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
	{ 2U,7U,6U,6U },
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
	{ 5U,5U,1U,2U },
	{ 5U,5U,1U,3U },
	{ 5U,5U,7U,2U },
	{ 6U,2U,2U,5U },
	{ 6U,2U,5U,2U },
	{ 6U,6U,2U,1U },
	{ 6U,6U,2U,7U },
	{ 6U,6U,3U,1U },
	{ 7U,2U,5U,5U },
	{ 7U,7U,1U,1U },
	{ 7U,7U,1U,1U }

	/*
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
	*/
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

typedef struct WFC_Cell {
	WFC_Tile_Id collapsed;
	unsigned char possible[WFC_TILE_COUNT];
	unsigned int entropy;
} WFC_Cell;

WFC_Cell* grid;
Vector backtrack_stack;

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
	return i < WFC_TILE_COUNT;
}
Vic2 WFC_Tile_Direction(WFC_Tile_Dir dir){
	if(dir == 0U) return (Vic2){  0,-1 };
	if(dir == 1U) return (Vic2){  0, 1 };
	if(dir == 2U) return (Vic2){ -1, 0 };
	if(dir == 3U) return (Vic2){  1, 0 };
	return (Vic2){  0, 0 };
}

char WFC_Tile_Compatible(WFC_Tile_Id a, WFC_Tile_Dir dir_from_a, WFC_Tile_Id b) {
	if (!WFC_Tile_Valid(a) || !WFC_Tile_Valid(b)) return 0;
	WFC_Tile_Dir dir_from_b = WFC_Tile_Inv(dir_from_a);
	return (WFC_ALLOWED[a][dir_from_a] == WFC_ALLOWED[b][dir_from_b]);
}
void WFC_UpdateEntropy(unsigned int idx) {
	unsigned int cnt = 0;
	for (unsigned int t = 0; t < WFC_TILE_COUNT; t++) {
		if (grid[idx].possible[t]) cnt++;
	}
	grid[idx].entropy = cnt;
}
void WFC_Init() {
	for (unsigned int i = 0; i < (unsigned int)(WFC_TILE_X * WFC_TILE_Y); i++) {
		grid[i].collapsed = WFC_TILE_INVALID;
		for (unsigned int t = 0; t < WFC_TILE_COUNT; t++) {
			grid[i].possible[t] = 1U;
		}
		grid[i].entropy = WFC_TILE_COUNT;
	}
	if (map) {
		memset(map, WFC_TILE_INVALID, sizeof(WFC_Tile_Id) * WFC_TILE_X * WFC_TILE_Y);
	}
	Vector_Clear(&backtrack_stack);
}
void WFC_Propagate(Vic2 start) {
	Vector queue = Vector_New(sizeof(unsigned int));
	unsigned int sidx = (unsigned int)(start.y * WFC_TILE_X + start.x);
	Vector_Push(&queue, &sidx);

	while (queue.size > 0) {
		unsigned int idx = *(unsigned int*)Vector_Get(&queue, 0);
		Vector_Remove(&queue, 0);  /* dequeue front */

		Vic2 pos = { idx % WFC_TILE_X, idx / WFC_TILE_X };

		if (grid[idx].collapsed == WFC_TILE_INVALID) continue;

		WFC_Tile_Id fixed = grid[idx].collapsed;
		for (WFC_Tile_Dir d = 0; d < WFC_DIRECTIONS; d++) {
			Vic2 npos = Vic2_Add(pos, WFC_Tile_Direction(d));
			if (npos.x < 0 || npos.y < 0 || npos.x >= WFC_TILE_X || npos.y >= WFC_TILE_Y) continue;

			unsigned int nidx = (unsigned int)(npos.y * WFC_TILE_X + npos.x);
			if (grid[nidx].collapsed != WFC_TILE_INVALID) continue;

			char changed = 0;
			for (WFC_Tile_Id t = 0; t < WFC_TILE_COUNT; t++) {
				if (grid[nidx].possible[t] && !WFC_Tile_Compatible(fixed, d, t)) {
					grid[nidx].possible[t] = 0;
					changed = 1;
				}
			}
			if (changed) {
				WFC_UpdateEntropy(nidx);
				Vector_Push(&queue, &nidx);

				if (grid[nidx].entropy == 0) {
					Vector_Free(&queue);
					return; /* contradiction */
				}
			}
		}
	}
	Vector_Free(&queue);
}
char WFC_Tile_BT_Push(Vic2 pos, WFC_Tile_Id chosen) {
	WFC_Tile_BT bt = WFC_Tile_BT_New(pos);
	bt.types[0] = chosen;
	Vector_Push(&backtrack_stack, &bt);
	return 1;
}
char WFC_Tile_BT_Pop(Vic2* out_pos, WFC_Tile_Id* out_chosen) {
	if (backtrack_stack.size == 0) return 0;
	unsigned int last = backtrack_stack.size - 1;
	WFC_Tile_BT* bt = (WFC_Tile_BT*)Vector_Get(&backtrack_stack, last);
	if (bt) {
		*out_pos = bt->pos;
		*out_chosen = bt->types[0];
		Vector_Remove(&backtrack_stack, last);
		return 1;
	}
	return 0;
}
Vic2 WFC_FindMinEntropy() {
	Vic2 best = {-1, -1};
	unsigned int min_e = WFC_TILE_ENTROPY_MAX;
	unsigned int candidates[64];
	unsigned int cand_count = 0;

	for (unsigned int y = 0; y < WFC_TILE_Y; y++) {
		for (unsigned int x = 0; x < WFC_TILE_X; x++) {
			unsigned int idx = y * WFC_TILE_X + x;
			if (grid[idx].collapsed == WFC_TILE_INVALID && grid[idx].entropy > 0) {
				if (grid[idx].entropy < min_e) {
					min_e = grid[idx].entropy;
					cand_count = 0;
				}
				if (grid[idx].entropy == min_e && cand_count < 64) {
					candidates[cand_count++] = idx;
				}
			}
		}
	}

	if (cand_count == 0) return best;

	unsigned int chosen_idx = candidates[Random_u64_MinMax(0U, cand_count - 1U)];
	return (Vic2){ chosen_idx % WFC_TILE_X, chosen_idx / WFC_TILE_X };
}
void WFC_SeedRandomStart() {
	Vic2 seed = {
		Random_u64_MinMax(0U, WFC_TILE_X - 1U),
		Random_u64_MinMax(0U, WFC_TILE_Y - 1U)
	};
	unsigned int idx = seed.y * WFC_TILE_X + seed.x;

	unsigned int cnt = 0;
	WFC_Tile_Id cands[WFC_TILE_COUNT];
	for (WFC_Tile_Id t = 0; t < WFC_TILE_COUNT; t++) {
		if (grid[idx].possible[t]) cands[cnt++] = t;
	}
	if (cnt > 0) {
		WFC_Tile_Id choice = cands[Random_u64_MinMax(0U, cnt-1U)];
		grid[idx].collapsed = choice;
		map[idx] = choice;
		for (WFC_Tile_Id t = 0; t < WFC_TILE_COUNT; t++)
			grid[idx].possible[t] = (t == choice);
		grid[idx].entropy = 1U;

		WFC_Tile_BT_Push(seed, choice);
		WFC_Propagate(seed);
	}
}
WFC_Tile_Id WFC_Collapse(Vic2 pos) {
	unsigned int idx = (unsigned int)(pos.y * WFC_TILE_X + pos.x);
	unsigned int cnt = 0;
	WFC_Tile_Id cands[WFC_TILE_COUNT];

	for (WFC_Tile_Id t = 0; t < WFC_TILE_COUNT; t++) {
		if (grid[idx].possible[t]) cands[cnt++] = t;
	}
	if (cnt == 0) return WFC_TILE_INVALID;

	WFC_Tile_Id choice = cands[Random_u64_MinMax(0U, cnt - 1U)];

	//if (Random_u64_MinMax(0U, 100U) < 15) { // 15% Chance
	//	if (grid[idx].possible[0]) choice = 0;           // Gray Center
	//	else if (grid[idx].possible[1]) choice = 1;      // andere Gray-Variante
	//}

	grid[idx].collapsed = choice;
	map[idx] = choice;
	for (WFC_Tile_Id t = 0; t < WFC_TILE_COUNT; t++)
		grid[idx].possible[t] = (t == choice ? 1U : 0U);
	grid[idx].entropy = 1U;

	return choice;
}
void WFC_DebugStats() {
	int count[36] = {0};
	for (int i = 0; i < WFC_TILE_X*WFC_TILE_Y; i++) {
		if (map[i] < 36) count[map[i]]++;
	}
	printf("Tile counts:\n");
	for (int i = 0; i < 36; i++) {
		if (count[i] > 0) printf("  Tile %2d: %d\n", i, count[i]);
	}
}
char WFC_Solve() {
	WFC_Init();
	//WFC_SeedRandomStart();

	unsigned int iterations = 0;
	const unsigned int max_iter = (unsigned int)(WFC_TILE_X * WFC_TILE_Y); // noch höher

	while (iterations < max_iter) {
		Vic2 cell = WFC_FindMinEntropy();
		if (cell.x == -1) {
			printf("[WFC]: Full success!\n");
			//WFC_DebugStats(); // <--- Debug
			return 1;
		}

		WFC_Tile_Id chosen = WFC_Collapse(cell);
		if (chosen == WFC_TILE_INVALID) {
			Vic2 p; WFC_Tile_Id ch;
			if (WFC_Tile_BT_Pop(&p, &ch)) {
				unsigned int idx = (unsigned int)(p.y * WFC_TILE_X + p.x);
				grid[idx].collapsed = WFC_TILE_INVALID;
				map[idx] = WFC_TILE_INVALID;
				for (unsigned int t = 0; t < WFC_TILE_COUNT; t++) grid[idx].possible[t] = 1U;
				WFC_UpdateEntropy(idx);
			} else {
				printf("[WFC]: Unsolvable.\n");
				return 0;
			}
			iterations++;
			continue;
		}

		WFC_Tile_BT_Push(cell, chosen);
		WFC_Propagate(cell);

		char contradiction = 0;
		for (unsigned int i = 0; i < (unsigned int)(WFC_TILE_X * WFC_TILE_Y); i++) {
			if (grid[i].entropy == 0 && grid[i].collapsed == WFC_TILE_INVALID) {
				contradiction = 1;
				break;
			}
		}

		if (contradiction) {
			Vic2 p; WFC_Tile_Id ch;
			WFC_Tile_BT_Pop(&p, &ch);
			unsigned int idx = (unsigned int)(p.y * WFC_TILE_X + p.x);
			grid[idx].collapsed = WFC_TILE_INVALID;
			map[idx] = WFC_TILE_INVALID;
			for (unsigned int t = 0; t < WFC_TILE_COUNT; t++) grid[idx].possible[t] = 1U;
			WFC_UpdateEntropy(idx);
		}
		iterations++;
	}
	printf("[WFC]: Max iterations reached. Press Space to retry.\n");
	return 0;
}

void* Fn_Build(void* arg) {
	WFC_Solve();
	return NULL;
}
void Fn_Rebuild() {
	Thread_Free(&builder);
	builder = Thread_New(NULL, Fn_Build, NULL);
	Thread_Start(&builder);
}

void Setup(AlxWindow* w){
	Random_Set(Time_Nano());

	tv = TransformedView_Make(
		(Vec2){ GetWidth(),GetHeight() },
		(Vec2){ 0.0f,0.0f },
		(Vec2){ 0.02f,0.02f },
		(float)GetWidth() / (float)GetHeight()
	);

	sprites = Vector_New(sizeof(Sprite));
	Vector_PushCount(&sprites, (Sprite[]){
		Sprite_Load("./assets/_1111.png"),
		Sprite_Load("./assets/_1177_0.png"),
		Sprite_Load("./assets/_1177.png"),
		Sprite_Load("./assets/_1255.png"),
		Sprite_Load("./assets/_1355.png"),
		Sprite_Load("./assets/_2166.png"),
		Sprite_Load("./assets/_2222.png"),
		Sprite_Load("./assets/_2223.png"),
		Sprite_Load("./assets/_2232.png"),
		Sprite_Load("./assets/_2233_G.png"),
		Sprite_Load("./assets/_2233.png"),
		Sprite_Load("./assets/_2234.png"),
		Sprite_Load("./assets/_2243.png"),
		Sprite_Load("./assets/_2244.png"),
		Sprite_Load("./assets/_2322.png"),
		Sprite_Load("./assets/_2323.png"),
		Sprite_Load("./assets/_2332.png"),
		Sprite_Load("./assets/_2333.png"),
		Sprite_Load("./assets/_2562.png"),
		Sprite_Load("./assets/_2626.png"),
		Sprite_Load("./assets/_2766.png"),
		Sprite_Load("./assets/_3166.png"),
		Sprite_Load("./assets/_3222.png"),
		Sprite_Load("./assets/_3223.png"),
		Sprite_Load("./assets/_3232.png"),
		Sprite_Load("./assets/_3233.png"),
		Sprite_Load("./assets/_3322_G.png"),
		Sprite_Load("./assets/_3322.png"),
		Sprite_Load("./assets/_3323.png"),
		Sprite_Load("./assets/_3332.png"),
		Sprite_Load("./assets/_3333_0.png"),
		Sprite_Load("./assets/_3333_1.png"),
		Sprite_Load("./assets/_3344.png"),
		Sprite_Load("./assets/_3422.png"),
		Sprite_Load("./assets/_4322.png"),
		Sprite_Load("./assets/_4422.png"),
		Sprite_Load("./assets/_4433.png"),
		Sprite_Load("./assets/_5512.png"),
		Sprite_Load("./assets/_5513.png"),
		Sprite_Load("./assets/_5572.png"),
		Sprite_Load("./assets/_6225.png"),
		Sprite_Load("./assets/_6252.png"),
		Sprite_Load("./assets/_6621.png"),
		Sprite_Load("./assets/_6627.png"),
		Sprite_Load("./assets/_6631.png"),
		Sprite_Load("./assets/_7255.png"),
		Sprite_Load("./assets/_7711_0.png"),
		Sprite_Load("./assets/_7711.png")

		/*
		Sprite_Load("./assets/_1111.png"),
		Sprite_Load("./assets/_1355.png"),
		Sprite_Load("./assets/_2222.png"),
		Sprite_Load("./assets/_2223.png"),
		Sprite_Load("./assets/_2232.png"),
		Sprite_Load("./assets/_2233_G.png"),
		Sprite_Load("./assets/_2233.png"),
		Sprite_Load("./assets/_2234.png"),
		Sprite_Load("./assets/_2243.png"),
		Sprite_Load("./assets/_2244.png"),
		Sprite_Load("./assets/_2322.png"),
		Sprite_Load("./assets/_2323.png"),
		Sprite_Load("./assets/_2332.png"),
		Sprite_Load("./assets/_2333.png"),
		Sprite_Load("./assets/_2562.png"),
		Sprite_Load("./assets/_2626.png"),
		Sprite_Load("./assets/_3166.png"),
		Sprite_Load("./assets/_3222.png"),
		Sprite_Load("./assets/_3223.png"),
		Sprite_Load("./assets/_3232.png"),
		Sprite_Load("./assets/_3233.png"),
		Sprite_Load("./assets/_3322_G.png"),
		Sprite_Load("./assets/_3322.png"),
		Sprite_Load("./assets/_3323.png"),
		Sprite_Load("./assets/_3332.png"),
		Sprite_Load("./assets/_3333_0.png"),
		Sprite_Load("./assets/_3333_1.png"),
		Sprite_Load("./assets/_3344.png"),
		Sprite_Load("./assets/_3422.png"),
		Sprite_Load("./assets/_4322.png"),
		Sprite_Load("./assets/_4422.png"),
		Sprite_Load("./assets/_4433.png"),
		Sprite_Load("./assets/_5513.png"),
		Sprite_Load("./assets/_6225.png"),
		Sprite_Load("./assets/_6252.png"),
		Sprite_Load("./assets/_6631.png")
		*/
	}, WFC_TILE_COUNT);

	map = (WFC_Tile_Id*)malloc(sizeof(WFC_Tile_Id) * WFC_TILE_X * WFC_TILE_Y);
	grid = (WFC_Cell*)malloc(sizeof(WFC_Cell) * WFC_TILE_X * WFC_TILE_Y);
	backtrack_stack = Vector_New(sizeof(WFC_Tile_BT));

	memset(map, WFC_TILE_INVALID, sizeof(WFC_Tile_Id) * WFC_TILE_X * WFC_TILE_Y);

	Thread_Null(&builder);
	Fn_Rebuild();
}
void Update(AlxWindow* w){
	if(Stroke(ALX_KEY_SPACE).PRESSED) Fn_Rebuild();

	TransformedView_HandlePanZoom(&tv, w->Strokes, GetMouse());

	const Vec2 size = TransformedView_WorldScreenLength(&tv, (Vec2){1.0f,1.0f});
	for(int i = 0; i < sprites.size; i++){
		Sprite* sp = (Sprite*)Vector_Get(&sprites, i);
		Sprite_Reload(sp, size.x + 1, size.y + 1);
	}

	Clear(BLACK);

	const Vec2 lt = TransformedView_ScreenWorldPos(&tv, (Vec2){0.0f,0.0f});
	const Vec2 rb = TransformedView_ScreenWorldPos(&tv, (Vec2){GetWidth(), GetHeight()});

	for (float y = lt.y; y < rb.y + 1.0f; y += 1.0f) {
		for (float x = lt.x; x < rb.x + 1.0f; x += 1.0f) {
			unsigned int px = (unsigned int)x;
			unsigned int py = (unsigned int)y;
			if (px >= WFC_TILE_X || py >= WFC_TILE_Y) continue;

			WFC_Tile_Id it = map[py * WFC_TILE_X + px];
			if (WFC_Tile_Valid(it)) {
				Vec2 spos = TransformedView_WorldScreenPos(&tv, (Vec2){px, py});
				Sprite* sp = (Sprite*)Vector_Get(&sprites, it);
				Sprite_Render(WINDOW_STD_ARGS, sp, spos.x, spos.y);
			}
		}
	}
}
void Delete(AlxWindow* w){
	Thread_Free(&builder);
	for(int i = 0; i < sprites.size; i++){
		Sprite_Free((Sprite*)Vector_Get(&sprites, i));
	}
	Vector_Free(&sprites);
	Vector_Free(&backtrack_stack);
	if(map) free(map);
	if(grid) free(grid);
}

int main(){
    if(Create("Wave Function Collapse",2500,1200,1,1,Setup,Update,Delete))
        Start();
    return 0;
}