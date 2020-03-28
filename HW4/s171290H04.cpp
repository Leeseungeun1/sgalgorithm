#include <stdio.h>
#include <limits.h>
#include <math.h>

#define min_init INT_MAX 
// input  : X[]   (x position array of N points)
//          Xid[] (index array of X (initially [0,1,...,N-1])
//          TMP[] (temporary array of size N. may or may not be used)
//          N   number of points to sort
// output : Xid[] (sorted indices by x position in X[])

double combine(unsigned *Yid, unsigned L, unsigned R, double dLR, unsigned *pt1, unsigned *pt2, double *X, double *Y, double middle, unsigned *TMP);
void mergeY(double *Y, unsigned *Yid, unsigned L, unsigned R, unsigned *TMP);

extern void sortXid(double * X, unsigned * Xid, unsigned * TMP, unsigned N)
{
	if (N > 1) {
		int m = N / 2;
		int h = N - m;
		sortXid(X, Xid, TMP, m);
		sortXid(X, Xid+m, TMP, h);
		int i = 0, j = m, k = 0;
		while (i < m&&j < N) {
			if (X[Xid[i]] <= X[Xid[j]]) {
				TMP[k] = Xid[i];
				i++;
			}
			else {
				TMP[k] = Xid[j];
				j++;
			}
			k++;
		}
		if (i >= m) {
			while (j < N) {
				TMP[k] = Xid[j];
				j++; k++;
			}
		}
		else {
			while (i < m) {
				TMP[k] = Xid[i];
				i++; k++;
			}
		}
		//sort된 값 Xid에 넣기
		for (i = 0; i < N; i++) {
			Xid[i] = TMP[i];
		}
	}
	
}


extern double closestPairDC(
	unsigned L, unsigned R,	  // current leftmost and rightmost indices
	unsigned *pt1, unsigned *pt2, // closest pair points indices 
	double *X, double *Y,         // (x,y) positions array(input)
	unsigned *Xid,  // point index array(sorted by x coordinates)
	unsigned *Yid,  // point index array(gradually sorted by y coordinates)
	unsigned *TMP,  // temporal index array
	unsigned THR	// threshold value
) {
	if (R - L + 1 <= THR) {
		double min = min_init;

		//find minimum distance
		for (unsigned i = L; i < R; i++) {
			unsigned j = i + 1;
				unsigned leftidx = Xid[i], rightidx = Xid[j];
				double distance= sqrt((Y[leftidx] - Y[rightidx])*(Y[leftidx] - Y[rightidx]) + (X[leftidx] - X[rightidx])*(X[leftidx] - X[rightidx]));
				if (distance < min) {
					min = distance;
					*pt1 = rightidx;
					*pt2 = leftidx;
				}
		}
		//Ysort-selection sort
		for (int i = L; i <= R; i++) {
			Yid[i] = Xid[i];
		}
		for (int i = L; i < R; i++) {
			unsigned minidx =i;
			for (int j = i + 1; j<= R; j++) {
				if (Y[Yid[j]] < Y[Yid[minidx]]) {
					minidx = j;
				}
			}
			unsigned temp = Yid[i];
			Yid[i] = Yid[minidx];
			Yid[minidx] = temp;
		}
		return min;
	}

	else {
		unsigned M = (L + R) / 2;
		unsigned lpt1, lpt2, rpt1, rpt2;
		double dL = closestPairDC(L, M, &lpt1, &lpt2, X, Y, Xid, Yid, TMP, THR);
		double dR = closestPairDC(M + 1, R, &rpt1, &rpt2, X, Y, Xid, Yid, TMP, THR);
		mergeY(Y, Yid, L, R, TMP);
		//dL과  dR 중 minimum찾기
		double dLR;
		if (dL < dR) {
			dLR = dL; *pt1 = lpt1; *pt2 = lpt2;
		}
		else {
			dLR = dR; *pt1 = rpt1; *pt2 = rpt2;
		}

		//중간값 찾기
		double middle = (X[Xid[L]] + X[Xid[R]]) / 2;
		
		double dmin = combine(Yid, L, R, dLR, pt1, pt2, X, Y, middle, TMP);
		return dmin;
	}
}
double combine(unsigned *Yid, unsigned L, unsigned R, double dLR, unsigned *pt1, unsigned *pt2, double *X, double *Y, double middle, unsigned *TMP) {
	double min = dLR;
	double left = middle - dLR;
	double right = middle + dLR;
	unsigned  k = 0;
	for (unsigned i = L; i <= R; i++) {
		if (X[Yid[i]] >= left && X[Yid[i]] <= right) {
			TMP[k] = Yid[i];
			k++;
		}
	}

	//중간 영역에 점이 없거나 한개면 더 탐색할 점이 존재하지 않는다.
	if (k == 0 || k == 1|| L==R) {
		return min;
	}
	
	for (unsigned i = 0; i < k-1; i++) {
		for (unsigned j = i + 1; j < k; j++) {
			if (Y[TMP[j]] - Y[TMP[i]] >= dLR) break;
			unsigned upindex = TMP[j];
			unsigned downindex = TMP[i];
			double temp = sqrt((Y[upindex] - Y[downindex])*(Y[upindex] - Y[downindex]) + (X[upindex] - X[downindex])*(X[upindex] - X[downindex]));
			if (temp < min) {
				*pt1 = upindex;
				*pt2 = downindex;
				min = temp;
			}
		}
	}
	return min;
}
void mergeY(double *Y, unsigned *Yid, unsigned L, unsigned R, unsigned *TMP) {
	int M = (L + R) / 2;
	int i = L, j = M + 1, k = L;
	while (i <= M && j <= R) {
		if (Y[Yid[i]] <= Y[Yid[j]]) {
			TMP[k] = Yid[i];
			i++;
		}
		else {
			TMP[k] = Yid[j];
			j++;
		}
		k++;
	}

	if (i > M) {
		while (j <= R) {
			TMP[k] = Yid[j];
			k++; j++;
		}
	}
	else {
		while (i <= M) {
			TMP[k] = Yid[i];
			k++; i++;
		}
	}
	//Yid에 sort된 값 복사
	for (int t = L; t <= R; t++) {
		Yid[t] = TMP[t];
	}
}
