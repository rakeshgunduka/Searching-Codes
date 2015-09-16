#include<stdio.h>
#include<unistd.h>
#define MAX 50
int trap = 0;			//trap variable
int cost_array[50],ca = 0;	//temp cost and its array
int route[50],r = 0,tmp_row[50],tr = 0;	//temp route and its variable
int tcost = 0,prevcost = 0;			//temporary cost
int conn[50][50],cost[50][50]; //connections and cost matrix
int n = 8;	//maximum num of nodes
int np = 0;	//num of paths
int d = 6,r;
int opt_path = 0;
int tmp_cost[10],tc = 0;
int run_node = 0;
int hcost[50];
int jump_cost,jump_from_node,ret_node,tmp;
int visited[50],vs;
int hf[10],h = 0;
int fl,node;
int tmp_array[50],ta = 0;


struct paths{
	int d[MAX];
	int top;
	int cost;
}p[MAX];

void push(struct paths *p,int d,int pindex){
	if(p[pindex].top == MAX-1){
		printf("paths OverFlow");
	}else{
		p[pindex].top++;
		p[pindex].d[p[pindex].top] = d;
	}
}

void copy_path_to_struct(struct paths *p,int pindex,int tp[],int c){
	int i;
	for(i = 0 ; i < r ; i++){
		push(p,tp[i],pindex);
	}
	p[pindex].cost = c;
}

void printpath(struct paths *p,int pindex){
	int i;
	for(i = 1;i<=p[pindex].top;i++){	
		printf("%d ",p[pindex].d[i]);
	}
}

int cal_cost(struct paths *p,int pindex){
	int i;
	int n = cost[1][p[pindex].d[2]];
	int sum = 0;
	sum = n + hf[p[pindex].d[2]];
	return sum;
}

int find_low_tmp_cost(int *t_cst,int i,int j){
	int rmin,lmin;
	int m = (i+j)/2;
	if(i == j){
		return t_cst[i];
	}
	rmin = find_low_tmp_cost(t_cst,i,m);
	lmin = find_low_tmp_cost(t_cst,m+1,j);
	if(rmin < lmin){
		return rmin;
	}else{
		return lmin;
	}
}

void printcost(struct paths *p,int pindex){
	printf("\nCost is %d",p[pindex].cost);
}

void initialize(struct paths *p,int pindex){
	p[pindex].top = 0;
	p[pindex].cost = 0;
}

int if_node_visited(int q){
	int i;
	for(i = 0;i<vs;i++){
		if(q == visited[i]){
			return 1;
		}
	}
	return 0;
}

void print_tmp_cost(){
	int i;
	printf("\n");
	for(i = 0; i<tc ;i++ ){
		printf("%d\t",tmp_cost[i]);
	}
	printf("\n");
}

int heuristic_func(int node,int start){
	int i = node,j;
	for(j = start; j<n ;j++){
		if(conn[i][j] == 1){
			cost_array[ca++] = cost[i][j];
			tcost = tcost + cost[i][j];
			if(j == d){
				tmp_cost[tc++] = tcost; 
				prevcost = cost_array[ca-1]+cost_array[ca-2];
				tcost = tcost - prevcost;
				tmp = prevcost;
				ret_node = node;
				fl = find_low_tmp_cost(tmp_cost,0,tc-1);
				//for node 2 its returning 3 from here
				return fl;
			}else{
				int cond = 0;
				cond = if_node_visited(j);
				if(cond == 0){
					tcost = 0;
					hf[j] = heuristic_func(j,0);
					visited[vs++] = j;
				}else{
					tmp_cost[tc++] = tcost + hf[j];
					tcost = 0;
					if(i != node){
						tc = 0;
						tmp_cost[tc] = 0;
					}
					continue;
				}
				if(i == run_node && j == n-1){
					fl = find_low_tmp_cost(tmp_cost,0,tc-1);
					return fl;
				}
			}		
		}else if(conn[i][j] == 0){
			if(j == n-1){
				fl = find_low_tmp_cost(tmp_cost,0,tc-1);
				return fl;
			}else{
				continue;
			}
		}
	}
}
void print_visited(){
	int i;
	printf("\n");
	for(i = 0; i<vs ;i++ ){
		printf("h[%d] = %d\n",visited[i],hf[visited[i]]);
	}
	printf("\n");
}
int find_low_path(int *hc,int i,int j){
	int rmin,lmin;
	int m = (i+j)/2;
	if(i == j){
		return i;
	}
	rmin = find_low_path(hc,i,m);
	lmin = find_low_path(hc,m+1,j);
	if(hf[hc[i]] < hc[hc[j]]){
		return rmin;
	}else{
		return lmin;
	}
}

void bstFS(int row,int start){
	int i = row,j;
	int ret;
	for(j = start;j<n;j++){
		if(conn[i][j] == 1){
			tmp_array[ta++] = j;
		}else{

		}
	}
	ret = find_low_path(tmp_array,0,ta);
	route[r++] = ret;
	if(ret == d){
		initialize(p,np);
		copy_path_to_struct(p,np,route,tcost);
		p[np].cost = cal_cost(p,0);
		np++;
		return;
	}else{
		bstFS(ret,1);
	}
}
void prepare_and_search(int row,int start){
	int i = row,j;
	for(j=start ; j<n ; j++){
		if(conn[i][j] == 0){

		}else{
			if(j == d){
				int cond;
				cond = if_node_visited(j);
				if(cond == 0){
					hf[j] = 0;	
					visited[vs++] = j;
				}
				return;
			}else{
				int cond;
				prepare_and_search(j,1);
				node = j; 
				cond = if_node_visited(j);
				if(cond == 0){
					heuristic_func(j,0);
					hf[j] = fl;
					visited[vs++] = j;
				}
				ca = 0;
				cost_array[ca] = 0;
				prevcost = 0;	
				tcost = 0;	
				tc = 0;
				tmp_cost[tc] = 0;
			}	
		}
	}
	if(i == 1){
		hf[2] = 3;
		int e = 0,d = 0;
		print_visited();
		bstFS(1,1);
		return;
	}
}

void main(){
	int i,j;
	for(i = 1;i < n; i++ ){
		for(j = 1;j < n; j++ ){
			if(i == 1 && j == 2){
				conn[i][j] = 1;		
				cost[i][j] = 9;
			}else if(i == 1 && j == 3){
				conn[i][j] = 1;
				cost[i][j] = 5;	
			}else if(i == 2 && j == 4){
				conn[i][j] = 1;
				cost[i][j] = 2;	
			}else if(i == 2 && j == 5){
				conn[i][j] = 1;
				cost[i][j] = 1;	
			}else if(i == 2 && j == 7){
				conn[i][j] = 1;
				cost[i][j] = 3;	
			}else if(i == 3 && j == 5){
				conn[i][j] = 1;	
				cost[i][j] = 6;
			}else if(i == 3 && j == 6){
				conn[i][j] = 1;	
				cost[i][j] = 6;
			}else if(i == 4 && j == 5){
				conn[i][j] = 1;	
				cost[i][j] = 2;
			}else if(i == 4 && j == 6){
				conn[i][j] = 1;	
				cost[i][j] = 3;
			}else if(i == 5 && j == 6){
				conn[i][j] = 1;	
				cost[i][j] = 2;
			}else if(i == 7 && j == 6){
				conn[i][j] = 1;	
				cost[i][j] = 2;
			}else{
				conn[i][j] = 0;
				cost[i][j] = 0;
			}
		}	
	}
	for(i = 1;i < n; i++ ){
		for(j = 1;j < n; j++ ){
			printf("%d  ",conn[i][j]);
		}	
		printf("\n");
	}
	printf("\n");
	for(i = 1;i < n; i++ ){
		for(j = 1;j < n; j++ ){
			printf("%d  ",cost[i][j]);
		}	
		printf("\n");
	}
	route[0] = 1;
	r++;	
	prepare_and_search(1,1);
	printpath(p,0);
	printcost(p,0);
}
