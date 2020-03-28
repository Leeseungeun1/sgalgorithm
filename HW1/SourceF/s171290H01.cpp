#include "Graph_adj_list.h"

extern void errorExit(const char *s);

SLListD pool;
unsigned long UsedMemoryForArray = 0;

void Read_Graph_adj_list(int Vnum, vertex *V, int Enum, edge *E) {
	// ���Ͽ��� �׷��� ������ �о� V�� E �迭�� �����Ѵ�.
	// V[ ].name�� E[ ].name�� ������ �ʿ䰡 ������ ���ǻ� �� �迭������
	//   index�� �����Ѵ�.
	// V[ ].S�� adjacent list header�� ���� ����ε� ���ȴ�.
	// ���� edge (i,j)�� ������ V[i].S�� edge index k�� push�ϰ�
	//  ���������� V[j].S�� k�� push�Ѵ�.
	// V[].flag�� E[].flag�� �ݵ�� false�� �����ؾ� �Ѵ�.


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
	
	// ***�� �Լ��� �ۼ��Ѵ�
	
}

void Free_Graph_adj_list(int Vnum, vertex *V) {
	// V �迭�� adjacency list�� ���Ҹ� pop()�� ����Ͽ�
	// ���  pool�� �ǵ��� �ش�

	// ***�� �Լ��� �ۼ��Ѵ�

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
	// DFS�� ����Ͽ� DFS tree�� ���ϴ� ������ flag�� true�� �����Ѵ�.
	// DFS ���� vertex�� �Է� ���Ͽ��� �����ȴ�(src).
	// V[]�� adjacent list Ž���� .top() ��� �Լ��� ����Ͽ�
	//  adj list ����� ��� �̸� ���󰡸� ���� vertex�� Ž���Ѵ�.
	// DFS tree�� ���� ������ cost ���� return �Ѵ�
	//   (��� �� edge cost�� 1�̸� unique)
	// recursive �Լ��� �ۼ��ص� �����ϴ�.

	// ***�� �Լ��� �ۼ��Ѵ�
	
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
// �Ʒ� �Լ����� 02_04_CSE3081_CPrg&DataStructure_�߰��ڷ�.pdf��
//   �����Ͽ� �� �Լ����� ä�� ����.
// ���ø� �ʿ��ϹǷ� ť�� ��� �Լ��� �Ʒ� �Լ� ����Ʈ�� ����

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
	// ***�� �Լ��� �ۼ��Ѵ�

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
	
	// ***�� �Լ��� �ۼ��Ѵ�
	
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
	// ***�� �Լ��� �ۼ��Ѵ�

	if (SLL_cnt != 0) {
		errorExit("Non-zero SLL_cnt after cleanup.");
	}
}
