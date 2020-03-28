#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <queue>

#define NONE -1

typedef struct elm_edge {
	int  vf, vr;  // e = (vf, vr) where vf, vr are vertex array indices
	int  cost;    // edge cost
	bool flag;    // true if the edge is in the SP spanning tree, false otherwise 
	int  fp, rp;  // adj list ptr of forward and reverse (-1 if none)
} edge;

typedef struct elm_vertex {
	int  f_hd, r_hd;	// adj list header(-1 if none)
	int  distance;		// distance from src to this vertex
	int  heap_idx;		// minHeap index
	bool inS;			// true if SP length found(true when move from V-S to S) 
} vertex;

void insert_Heap(vertex *V, int index, int *minHeap, int *heap_size) {
	int heap_idx=++(*heap_size);
	//insert한 node의 위치를 찾는다.
	while (heap_idx != 1 && (V[index].distance < V[minHeap[heap_idx / 2]].distance)) {
		minHeap[heap_idx] = minHeap[heap_idx / 2];
		V[minHeap[heap_idx / 2]].heap_idx = heap_idx;
		heap_idx = heap_idx / 2;
	}
	minHeap[heap_idx] = index;
	V[index].heap_idx = heap_idx;
}

int delete_heap(vertex *V, int *minHeap, int *heap_size) {
	int return_value = minHeap[1];
	V[minHeap[1]].inS = true;
	V[minHeap[1]].heap_idx = -1;
	int heap_idx = (*heap_size)--;
	int parent = 1, child = 2;
	//맨 마지막 node를 알맞은 위치에 재배치한다.
	while (child <= (*heap_size)) {
		if (child < (*heap_size) && V[minHeap[child]].distance > V[minHeap[child + 1]].distance) {
			child = child + 1;
		}
		if (V[minHeap[heap_idx]].distance < V[minHeap[child]].distance) break;
		V[minHeap[child]].heap_idx = parent;
		minHeap[parent] = minHeap[child];
		parent = child;
		child = 2 * child;
	}
	V[minHeap[heap_idx]].heap_idx = parent;
	minHeap[parent] = minHeap[heap_idx];

	//delete하는 node의 index를 반환
	return return_value;
}

void moveup_heap(vertex *V, int index, int *minHeap) {
	int heap_index = V[index].heap_idx;
	//min_heap 정의에 맞게 해당 node가 위치를 찾을 때 까지 이동
	while (heap_index!=1&&V[minHeap[heap_index]].distance < V[minHeap[heap_index / 2]].distance) {
		V[minHeap[heap_index]].heap_idx = heap_index / 2;
		V[minHeap[heap_index / 2]].heap_idx = heap_index;
		int tmp = minHeap[heap_index];
		minHeap[heap_index] = minHeap[heap_index / 2];
		minHeap[heap_index / 2] = tmp;
		heap_index = heap_index / 2;
	}
}

int SPT_Dijkstra(
	int src,	// source vertex index
	// graph structure array
	// 1. the adjacency list structure is the same as PHW02
	// 2. additional fields are added for Dijkstra's algorithm(see .h file)
	int Vnum, vertex *V,	// Vertex array size and the array
	int Enum, edge *E,		// Edge array size and the array

	int *minHeap	// array for min heap (array size = Vnum+1)
		// heap index range is 1 ~ (Vnum - 1) note: src must not in the initial heap
		// just arry is passed. must construct min-heap in this function

	// OUTPUT
	// 1. V[].distance : shortest path length from src to this vertex
	// 2. shortest path spanning tree : set E[].flag = true if the edge is in the tree
	// 3. return the sum of edge costs in the shortest path spanning tree.
	//    should be small as possible (think a way to make it small)
) {
	int treeCost = 0;
	// *** 이 함수를 작성하자 ***
	// 반드시 min-heap을 사용하여 O((n+m)logn) 알고리즘을 구현해야 한다(아니면 trivial한 프로그램임)
	// heap 연산 등 필요한 함수는 자유롭게 작성하여 추가한다.
	// 그러나 global 변수, dynamic array 등은 추가로 사용하지 않는다(실제로 필요 없다)
	
	int heap_size = 0;
	
	V[src].inS = true;
	V[src].distance = 0;
	
	for (int i = 0; i < Vnum; i++) {
		insert_Heap(V, i, minHeap, &heap_size);
	}

	//src index를 heap에 넣고 시작하며, 맨 마지막에 Dijkstra vertex set에 추가되는
	//vertex도 표시 해 주기 위해서 (Vnum-2)+1+1번 loop를 돌린다.
	for (int i = 0; i < Vnum ; i++) {
		//find minimum edge && include vertex && delete that vertex
		int minimumedge_v = delete_heap(V, minHeap, &heap_size);
		//update distance && heap arrange
 		for (int edgenum = V[minimumedge_v].f_hd; edgenum != -1; edgenum = E[edgenum].fp) {
			int another;
			if (E[edgenum].vr == minimumedge_v) {
				another = E[edgenum].vf;
			}
			else {
				another = E[edgenum].vr;
			}
			if (V[another].inS==false&& (V[minimumedge_v].distance + E[edgenum].cost < V[another].distance)) {
				V[another].distance = V[minimumedge_v].distance + E[edgenum].cost;
				moveup_heap(V, another, minHeap);
			}
		}
		for (int edgenum = V[minimumedge_v].r_hd; edgenum != -1; edgenum = E[edgenum].rp) {
			int another;
			if (E[edgenum].vr == minimumedge_v) {
				another = E[edgenum].vf;
			}
			else {
				another = E[edgenum].vr;
			}
			if (V[another].inS == false && (V[minimumedge_v].distance + E[edgenum].cost < V[another].distance)) {
 				V[another].distance = V[minimumedge_v].distance + E[edgenum].cost;
				moveup_heap(V, another, minHeap);
			}
		}
	}
	
	//find edge which is contained Dijkstra
	//if there exist multiple roots from src to one vertex, take a shortest root
	for (int i = 0; i < Vnum; i++) {
		if (i == src) continue;
		int minimum = -1;
		for (int edgenum = V[i].f_hd; edgenum != -1; edgenum = E[edgenum].fp) {
			int another;
			if (E[edgenum].vr == i) {
				another = E[edgenum].vf;
			}
			else {
				another = E[edgenum].vr;
			}
			if (V[i].distance == V[another].distance + E[edgenum].cost && E[edgenum].flag==false) {
				if (minimum == -1 || E[edgenum].cost < E[minimum].cost) {
					minimum = edgenum;
				}
			}
		}
		for (int edgenum = V[i].r_hd; edgenum != -1; edgenum = E[edgenum].rp) {
			int another;
			if (E[edgenum].vr == i) {
				another = E[edgenum].vf;
			}
			else {
				another = E[edgenum].vr;
			}
			if (V[i].distance == V[another].distance + E[edgenum].cost&& E[edgenum].flag == false) {
				if (minimum == -1 || E[edgenum].cost < E[minimum].cost) {
					minimum = edgenum;
				}
			}
		}
		E[minimum].flag = true;
		treeCost = treeCost + E[minimum].cost;
	}

	return treeCost;
}

void Read_Graph(int Vnum, vertex *V, int Enum, edge *E) {
	// Graph 자료구조를 만드는 함수
	// *** 이 함수를 추가하자 ***
	// PHW02의 Read_Graph_adj_array()를 이 과제의 자료구조에 맞춰 살짝 수정하여 사용한다
	// 즉, Read_Graph_adj_array()에서 불필요한 내용을 제거한 후 사용.
	
	int max_distance = 0;
	//initialize to make adj_array()
	for (int i = 0; i < Vnum; i++) {
		V[i].f_hd = -1;
		V[i].r_hd = -1;
		V[i].inS = false;
		V[i].heap_idx = -1;
	}
	for (int i = 0; i < Enum; i++) {
		E[i].flag = false;
		E[i].fp = -1;
		E[i].rp = -1;
	}

	for (int i = 0; i < Enum; i++) {
		scanf("%d %d %d", &(E[i].vf), &(E[i].vr), &(E[i].cost));
		max_distance += E[i].cost;
		if (V[E[i].vf].f_hd == -1) {
			V[E[i].vf].f_hd = i;
		}
		else {
			E[i].fp = V[E[i].vf].f_hd;
			V[E[i].vf].f_hd = i;
		}

		if (V[E[i].vr].r_hd == -1) {
			V[E[i].vr].r_hd = i;
		}
		else {
			E[i].rp = V[E[i].vr].r_hd;
			V[E[i].vr].r_hd = i;
		}
	}
	
	//set infinite with sum of all cost edges
	for (int i = 0; i < Vnum; i++) {
		V[i].distance = max_distance;
	}

}

// the following functions are for testing if the submitted program is correct.
int  Tree_Check(int Vnum, vertex *V, int Enum, edge *E, int *visited);
bool SPT_test(int src, int Vnum, vertex *V, int Enum, edge *E, int *minHeap);
void Error_Exit(const char *s);

int main ( void ) {
	int		src;
	vertex *V;		int Vnum;
	edge   *E;		int Enum;
	int    *minHeap;
	int    Tree_cost;
	int    Tnum; 		// # of test cases
	clock_t start, finish;
	double cmpt;

	scanf_s("%d", &Tnum);		// read # of tests

	for (int t = 0; t < Tnum; t++ ) {
		scanf_s("%d %d %d", &Vnum, &Enum, &src);
		V = new vertex [Vnum];
		E = new edge [Enum];
		minHeap = new int[Vnum + 1];	// heap array allocation
		if ( V == NULL || E == NULL || minHeap == NULL ) {
			Error_Exit("Memory Allocation Error");
		}
		Read_Graph(Vnum, V, Enum, E);

		/**/start = clock();	// start timer

		Tree_cost = SPT_Dijkstra(src, Vnum, V, Enum, E, minHeap);	// code by students

		/**/finish = clock();	// stop timer
		cmpt = ((double)(finish - start)) / (double)CLK_TCK;

		// 아래 Tree_Check와 SPT_test 함수는 첨부한 SPT_test.obj 파일에 있다.
		// 이 테스트에서 오류로 인하여 프로그램이 정지하면 뭔가 잘못된 것이다(제출해도 0점)
        if (Tree_Check(Vnum, V, Enum, E, minHeap) == 0) {
			Error_Exit("   ERROR The result is not a spanning tree");
		}
		if (SPT_test(src, Vnum, V, Enum, E, minHeap) == false) {
			Error_Exit("** Something wrong in applying Dijkstra's");
		}
		if ( t != 0 ) 
			printf("\n");

		printf("**T%d (Dijkstra) (V = %d, E = %d, time = %.3f sec) Tree Cost = %d\n", 
			t+1, Vnum, Enum, cmpt, Tree_cost);

		delete minHeap; delete V; delete E;
	}
	return 0;
}

void Error_Exit (const char *s ) {
  printf("%s\n", s);
  exit(-1);
}
