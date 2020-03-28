#include "Graph_adj_array.h"

void Read_Graph_adj_array(int Vnum, vertex V[], int Enum, edge E[]) {
	
	for (int i = 0; i < Vnum; i++) {
		V[i].name = i;
		V[i].flag = false;
		V[i].f_hd = -1;
		V[i].r_hd = -1;
	}
	for (int i = 0; i < Enum; i++) {
		E[i].name = i;
		E[i].vf = -1;
		E[i].vr = -1;
		E[i].flag = false;
		E[i].cost = 1;
		E[i].fp = -1;
		E[i].rp = -1;
	}
	for (int i = 0; i < Enum; i++) {
		scanf("%d %d %d", &(E[i].vf), &(E[i].vr), &(E[i].cost));
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

}

int DFS_Tree_adj_array(
	int     src,   // source node index
	int     Vnum,  // number of vertices
	vertex *V,     // vertex structure array (starting index 0)
	int     Enum,  // number of edges
	edge   *E     // edge structure array (starting index 0)
) {
	V[src].flag = true;
	int returncost = 0;
	for (int edgenum = V[src].f_hd; edgenum != -1; edgenum = E[edgenum].fp) {
		int another;
		if (E[edgenum].vr == src) {
			another = E[edgenum].vf;
		}
		else {
			another = E[edgenum].vr;
		}
		if (V[another].flag == false) {
			E[edgenum].flag = true;
			returncost = returncost + E[edgenum].cost + DFS_Tree_adj_array(another, Vnum, V, Enum, E);
		}
	}
	for (int edgenum = V[src].r_hd; edgenum != -1; edgenum = E[edgenum].rp) {
		int another;
		if (E[edgenum].vf == src) {
			another = E[edgenum].vr;
		}
		else {
			another = E[edgenum].vf;
		}
		if (V[another].flag == false) {
			E[edgenum].flag = true;
			returncost = returncost + E[edgenum].cost + DFS_Tree_adj_array(another, Vnum, V, Enum, E);
		}
	}
	return returncost;
}