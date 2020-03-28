#include "Graph_adj_list.h"

extern void errorExit(const char *s);

SLListD pool;
unsigned long UsedMemoryForArray = 0;

void Read_Graph_adj_list(int Vnum, vertex *V, int Enum, edge *E) {
	// 파일에서 그래프 정보를 읽어 V와 E 배열을 구성한다.
	// V[ ].name과 E[ ].name은 설정할 필요가 없으나 편의상 각 배열에서의
	//   index를 저장한다.
	// V[ ].S는 adjacent list header로 스택 헤더로도 사용된다.
	// 따라서 edge (i,j)를 읽으면 V[i].S에 edge index k를 push하고
	//  마찬가지로 V[j].S에 k를 push한다.
	// V[].flag과 E[].flag은 반드시 false로 설정해야 한다.


	int v1, v2, cost;
	for (int i = 0; i < Vnum; i++) {
		V[i].name = i;
		V[i].flag = false;
	}
	for (int i = 0; i < Enum; i++) {
		E[i].name = i;
		E[i].flag = false;
	}
	
	for (int i = 0; i < Enum; i++) {
		scanf_s("%d %d %d", &v1, &v2, &cost);
		SLL* temp1 = pool.allocSLL();
		temp1->i = i;
		temp1->p = NULL;
		V[v1].S.push(temp1);

		SLL* temp2 = pool.allocSLL();
		temp2->i = i;
		temp2->p = NULL;
		V[v2].S.push(temp2);

		E[i].cost = cost;
		E[i].vf = v1;
		E[i].vr = v2;

	}
	
	// ***이 함수를 작성한다
	
}

void Free_Graph_adj_list(int Vnum, vertex *V) {
	// V 배열의 adjacency list의 원소를 pop()을 사용하여
	// 모두  pool로 되돌려 준다

	// ***이 함수를 작성한다

	for (int i = 0; i < Vnum; i++) {
		while (V[i].S.empty() == false) {
			SLL* temp = V[i].S.pop();
			pool.freeSLL(temp);
		}
	}
}

int DFS_Tree_adj_list (
	int     src,   // source node index
	int     Vnum,  // number of vertices
	vertex *V,     // vertex structure array (starting index 0)
	int     Enum,  // number of edges
	edge   *E     // edge structure array (starting index 0)
) {
	// DFS를 사용하여 DFS tree에 속하는 에지의 flag을 true로 설정한다.
	// DFS 시작 vertex는 입력 파일에서 지정된다(src).
	// V[]의 adjacent list 탐색은 .top() 멤버 함수를 사용하여
	//  adj list 헤더를 얻고 이를 따라가며 인접 vertex를 탐색한다.
	// DFS tree에 속한 에지의 cost 함을 return 한다
	//   (모든 각 edge cost가 1이면 unique)
	// recursive 함수로 작성해도 무방하다.

	// ***이 함수를 작성한다
	
	int another= 0;
	int returncost=0;
	V[src].flag = true;
	for (SLL *sll = V[src].S.top(); sll != NULL; sll = sll->p) {
		if (src == E[sll->i].vf) {
			another = E[sll->i].vr;
		}
		else if (src == E[sll->i].vr){
			another = E[sll->i].vf;
		}
		if (V[another].flag == false) {
			E[sll->i].flag = true;
			returncost=returncost+E[sll->i].cost+DFS_Tree_adj_list(another, Vnum, V, Enum, E);
		}
	}
	
	return returncost;

}

// SLL management functions
// 아래 함수들은 02_04_CSE3081_CPrg&DataStructure_추가자료.pdf를
//   참조하여 각 함수들을 채워 넣자.
// 스택만 필요하므로 큐의 멤버 함수는 아래 함수 리스트에 없다

// stack member fuctions
void sllStack::push(SLL *p) {
	// insert p to the top of stack ST
	p->p = ST;
	ST = p;
}

SLL *sllStack::pop(void) {
	// remove and return p at the top of ST
	SLL *p = ST;

	if (ST->p == NULL)
		ST = NULL;
	else
		ST = ST->p;
	return p;
}

SLL *sllStack::top(void) {
	// return p at the top of ST
	return ST;
}

bool sllStack::empty(void) {
	// return true if empty
	if (ST == NULL)
		return true;
	else
		return false;
}

// SLListD member functions
SLL *SLListD::allocSLL(void) {		// allocate an SLL element
	SLL *p;

	if (SLL_pool == NULL) {
		p = new SLL;
		if (p == NULL) {
			errorExit("Memory allocation error(allocSLL)");
		}
		UsedMemoryForSLLs = UsedMemoryForSLLs + sizeof(SLL);
		p->i = NONE;
	}
	else {
		p = SLL_pool;
		SLL_pool = p->p;
	}
	p->p = NULL;
	SLL_cnt = SLL_cnt + 1;
	// ***이 함수를 작성한다

	return(p);
}

void SLListD::freeSLL(SLL *p) {		// free *p (move it to SLL_pool
	if (p->i == NONE) {
		errorExit("This is already freed(freeSLL).");
	}
	p->i = NONE;
	p->p = SLL_pool;
	SLL_pool = p;
	SLL_cnt = SLL_cnt - 1;
	
	// ***이 함수를 작성한다
	
}

void SLListD::freeSLL_pool(void) {	// clear SLL_pool
	SLL *p = SLL_pool;

	while (p != NULL) {
		SLL_pool = p->p;
		p->p = NULL;
		delete[] p;
		UsedMemoryForSLLs = UsedMemoryForSLLs - 1;
		p = SLL_pool;
	}
	// ***이 함수를 작성한다

	if (SLL_cnt != 0) {
		errorExit("Non-zero SLL_cnt after cleanup.");
	}
}
