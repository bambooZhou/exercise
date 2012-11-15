#include<stdio.h>

#include<iostream>
#include<utility>
#include<vector>
#include<queue>

template<typename TypeOfVertex,typename TypeOfWeight>
class Graph{
public:
	Graph(){ num_of_vertices_=0;num_of_edges_=0;}
	virtual ~Graph(){};
	virtual bool Insert(TypeOfVertex u,TypeOfVertex v,TypeOfWeight w)=0;
	virtual bool Remove(TypeOfVertex u,TypeOfVertex v)=0;
	virtual bool Exist(TypeOfVertex u,TypeOfVertex v) const=0;
	int num_of_vertices(){ return num_of_vertices_;}
	int num_of_edges(){ return num_of_edges_;}

protected:
	int num_of_vertices_;
	int num_of_edges_;
};

template<typename TypeOfVertex,typename TypeOfWeight>
class AdjMatrixGraph : public Graph<TypeOfVertex,TypeOfWeight>{
public:
	AdjMatrixGraph(int num_of_vertices,const TypeOfVertex vertexs[],const TypeOfWeight no_edge_flag);
	~AdjMatrixGraph();
	virtual bool Insert(TypeOfVertex u,TypeOfVertex v,TypeOfWeight w);
	virtual bool Remove(TypeOfVertex u,TypeOfVertex v);
	virtual bool Exist(TypeOfVertex u,TypeOfVertex v) const;
	void PrintAdjMatrixGraph(){
		for(int i=0;i<Graph<TypeOfVertex,TypeOfWeight>::num_of_vertices_;i++){
			for(int j=0;j<num_of_vertices_;j++)
				std::cout << edges_[i][j] << " ";
			std::cout << std::endl;
		}
	}
private:
	TypeOfWeight **edges_;
	TypeOfVertex *vertices_;
	TypeOfWeight no_edge_flag_;

	int GetIndex(const TypeOfVertex v) const{
		for(int i=0;i<num_of_vertices_;i++){
			if(v==vertices_[i])
				return i;
		}
		return -1;
	}
};

template<typename TypeOfVertex,typename TypeOfWeight>
AdjMatrixGraph<TypeOfVertex,TypeOfWeight>::AdjMatrixGraph(int num_of_vertices,const TypeOfVertex vertexs[],const TypeOfWeight no_edge_flag){
	num_of_vertices_=num_of_vertices;
	vertices_=new TypeOfVertex[num_of_vertices_];
	no_edge_flag_=no_edge_flag;
	for(int i=0;i<num_of_vertices_;i++)
		vertices_[i]=vertexs[i];
	edges_=new TypeOfWeight* [num_of_vertices_];
	for(int i=0;i<num_of_vertices_;i++)
		edges_[i]=new TypeOfWeight[num_of_vertices_];
	for(int i=0;i<num_of_vertices_;i++)
		for(int j=0;j<num_of_vertices_;j++)
			edges_[i][j]=no_edge_flag;
}

template<typename TypeOfVertex,typename TypeOfWeight>
AdjMatrixGraph<TypeOfVertex,TypeOfWeight>::~AdjMatrixGraph(){
	std::cout << "~AdjMatrixGraph()" <<std::endl;
	delete[] vertices_;
	for(int i=0;i<num_of_vertices_;i++)
		delete[] edges_[i];
	delete[] edges_;
}

template<typename TypeOfVertex,typename TypeOfWeight>
bool AdjMatrixGraph<TypeOfVertex,TypeOfWeight>::Insert(TypeOfVertex u,TypeOfVertex v,TypeOfWeight w){
	int i=GetIndex(u);
	int j=GetIndex(v);
	if(-1==i || -1==j)//顶点不存在
		return false;
	if(edges_[i][j]!=no_edge_flag_)//边已存在
		return false;

	edges_[i][j]=w;
	num_of_edges_++;
	return true;
}

template<typename TypeOfVertex,typename TypeOfWeight>
bool AdjMatrixGraph<TypeOfVertex,TypeOfWeight>::Remove(TypeOfVertex u,TypeOfVertex v){
	int i=GetIndex(u);
	int j=GetIndex(v);
	if(-1==i || -1==j)//顶点不存在
		return false;
	if(edges_[i][j]==no_edge_flag_)//边不存在
		return false;

	edges_[i][j]=no_edge_flag_;
	num_of_edges_--;
	return true;
}

template<typename TypeOfVertex,typename TypeOfWeight>
bool AdjMatrixGraph<TypeOfVertex,TypeOfWeight>::Exist(TypeOfVertex u,TypeOfVertex v) const{
	int i=GetIndex(u);
	int j=GetIndex(v);
	if(-1==i || -1==j)
		return false;
	if(edges_[i][j]==no_edge_flag_)
		return false;
	else
		return true;
}

template<typename TypeOfVertex,typename TypeOfWeight>
class AdjListDirectGraph:public Graph<TypeOfVertex,TypeOfWeight>{
public:
	AdjListDirectGraph(int num_of_vertices,TypeOfVertex vertexs[]);
	~AdjListDirectGraph();
	std::vector<TypeOfVertex> DFSRecursive();
	std::vector<TypeOfVertex> BFS();
	void GetIndegree(std::vector<std::pair<TypeOfVertex,int>>& indegrees);
	void GetOutdegree(std::vector<std::pair<TypeOfVertex,int>>& outdegrees);
	virtual bool Insert(TypeOfVertex u,TypeOfVertex v,TypeOfWeight w);
	virtual bool Remove(TypeOfVertex u,TypeOfVertex v);
	virtual bool Exist(TypeOfVertex u,TypeOfVertex v) const;
	void PrintAdjMatrixGraph();
private:
	struct EdgeNode{
		int nearby_vertex_index;//边的终点在顶点结点数组中的索引
		TypeOfWeight weight;
		EdgeNode *next;
		EdgeNode(int nearby_vertex_index,TypeOfWeight weight,EdgeNode *next):
			nearby_vertex_index(nearby_vertex_index),weight(weight),next(next){}
	};
	struct VertexNode{
		TypeOfVertex vertex;
		EdgeNode *edge_list;
	};

	VertexNode *vertex_node_;

	int GetVertexIndex(const TypeOfVertex v) const;
	void DFS(VertexNode& v,std::vector<TypeOfVertex>& result,bool visit_flag[]);
};

template<typename TypeOfVertex,typename TypeOfWeight>
AdjListDirectGraph<TypeOfVertex, TypeOfWeight>::AdjListDirectGraph(int num_of_vertices,TypeOfVertex vertexs[]){
	vertex_node_=new VertexNode[num_of_vertices];
	num_of_vertices_=num_of_vertices;
	for(int i=0;i<num_of_vertices_;i++){
		vertex_node_[i].vertex=vertexs[i];
		vertex_node_[i].edge_list=NULL;
	}
}

template<typename TypeOfVertex,typename TypeOfWeight>
AdjListDirectGraph<TypeOfVertex, TypeOfWeight>::~AdjListDirectGraph(){
	for(int i=0;i<num_of_vertices_;i++){
		EdgeNode *p=vertex_node_[i].edge_list;
		while(p){
			EdgeNode *temp=p;
			p=p->next;
			delete temp;
		}
	}
	delete[] vertex_node_;
}

template<typename TypeOfVertex,typename TypeOfWeight>
std::vector<TypeOfVertex> AdjListDirectGraph<TypeOfVertex, TypeOfWeight>::DFSRecursive(){
	std::vector<TypeOfVertex> result;
	bool *visit_flag=new bool[num_of_vertices_];
	for(int i=0;i<num_of_vertices_;i++)
		visit_flag[i]=false;
	for(int i=0;i<num_of_vertices_;i++){
		if(visit_flag[i])
			continue;
		else
			DFS(vertex_node_[i],result,visit_flag);
	}
	return result;
}

template<typename TypeOfVertex,typename TypeOfWeight>
std::vector<TypeOfVertex> AdjListDirectGraph<TypeOfVertex, TypeOfWeight>::BFS(){
	using namespace std;
	vector<TypeOfVertex> result;
	queue<VertexNode> q;
	bool *visit_flag=new bool[num_of_vertices_];
	for(int i=0;i<num_of_vertices_;i++)
		visit_flag[i]=false;

	for(int i=0;i<num_of_vertices_;i++){
		if(visit_flag[i])
			continue;
		q.push(vertex_node_[i]);
		visit_flag[i]=true;
		while(!q.empty()){
			VertexNode v=q.front();
			int index_of_visit=GetVertexIndex(v.vertex);
			result.push_back(v.vertex);
			EdgeNode *p=v.edge_list;
			q.pop();
			while(p){
				int index_of_nearby=p->nearby_vertex_index;
				if(!visit_flag[index_of_nearby]){
					q.push(vertex_node_[index_of_nearby]);
					visit_flag[index_of_nearby]=true;
				}
				p=p->next;
			}
		}
	}
	return result;
}

template<typename TypeOfVertex,typename TypeOfWeight>
void AdjListDirectGraph<TypeOfVertex, TypeOfWeight>::GetIndegree(std::vector<std::pair<TypeOfVertex,int>>& indegrees){
	for(int i=0;i<num_of_vertices_;i++){
		int indegree=0;
		for(int j=0;j<num_of_vertices_;j++){
			EdgeNode *p=vertex_node_[j].edge_list;
			while(p){
				if(p->nearby_vertex_index==i){
					indegree++;
					break;
				}
				p=p->next;
			}
		}
		std::pair<TypeOfVertex,int> indgegree_result(vertex_node_[i].vertex,indegree);
		indegrees.push_back(indgegree_result);
	}
}

template<typename TypeOfVertex,typename TypeOfWeight>
void AdjListDirectGraph<TypeOfVertex, TypeOfWeight>::GetOutdegree(std::vector<std::pair<TypeOfVertex,int>>& outdegrees){
	for(int i=0;i<num_of_vertices_;i++){
		int outdegree=0;
		EdgeNode *p=vertex_node_[i].edge_list;
		while(p){
			outdegree++;
			p=p->next;
		}
		std::pair<TypeOfVertex,int> outdegree_result(vertex_node_[i].vertex,outdegree);
		outdegrees.push_back(outdegree_result);
	}
}

template<typename TypeOfVertex,typename TypeOfWeight>
bool AdjListDirectGraph<TypeOfVertex, TypeOfWeight>::Insert(TypeOfVertex u,TypeOfVertex v,TypeOfWeight w){
	int i=GetVertexIndex(u);
	int j=GetVertexIndex(v);
	if(-1==i)//边的起点不存在
		return false;
	if(-1==j)//边的终点不存在
		return false;
	EdgeNode *edge_node=new EdgeNode(j,w,NULL);
	edge_node->next=vertex_node_[i].edge_list;
	vertex_node_[i].edge_list=edge_node;
	num_of_edges_++;
	return true;
}

template<typename TypeOfVertex,typename TypeOfWeight>
bool AdjListDirectGraph<TypeOfVertex, TypeOfWeight>::Remove(TypeOfVertex u,TypeOfVertex v){
	int i=GetVertexIndex(u);
	int j=GetVertexIndex(v);
	if(-1==i)//边的起点不存在
		return false;
	if(-1==j)//边的终点不存在
		return false;
	EdgeNode *p=vertex_node_[i].edge_list;//当前被检查结点的父节点，当被检查结点为头结点时，其父节点就是它自己
	EdgeNode *q=p;//当前需要检查的结点
	while(q){
		if(q->nearby_vertex_index==j){
			if(q==p){
				vertex_node_[i].edge_list=q->next;
				delete q;
				num_of_edges_--;
				return true;
			}else{
				p->next=q->next;
				delete q;
				num_of_edges_--;
				return true;
			}
		}
		p=q;
		q=q->next;
	}
	return false;
}

template<typename TypeOfVertex,typename TypeOfWeight>
bool AdjListDirectGraph<TypeOfVertex, TypeOfWeight>::Exist(TypeOfVertex u,TypeOfVertex v) const{
	int i=GetVertexIndex(u);
	int j=GetVertexIndex(v);
	if(-1==i)//边的起点不存在
		return false;
	if(-1==j)//边的终点不存在
		return false;
	EdgeNode *p=vertex_node_[i].edge_list;//当前被检查结点的父节点，当被检查结点为头结点时，其父节点就是它自己
	EdgeNode *q=p;//当前需要检查的结点
	while(q){
		if(q->nearby_vertex_index==j)
			return true;
		p=q;
		q=q->next;
	}
	return false;
}

template<typename TypeOfVertex,typename TypeOfWeight>
void AdjListDirectGraph<TypeOfVertex, TypeOfWeight>::PrintAdjMatrixGraph(){

}

template<typename TypeOfVertex,typename TypeOfWeight>
int AdjListDirectGraph<TypeOfVertex, TypeOfWeight>::GetVertexIndex(const TypeOfVertex v) const{
	for(int i=0;i<num_of_vertices_;i++){
		if(vertex_node_[i].vertex==v)
			return i;
	}
	return -1;
}

template<typename TypeOfVertex,typename TypeOfWeight>
void AdjListDirectGraph<TypeOfVertex, TypeOfWeight>::DFS(VertexNode& v,std::vector<TypeOfVertex>& result,bool visit_flag[]){
	result.push_back(v.vertex);
	int i=GetVertexIndex(v.vertex);
	visit_flag[i]=true;
	EdgeNode *p=v.edge_list;
	while(p){
		int i=p->nearby_vertex_index;
		if(!visit_flag[i])
			DFS(vertex_node_[i],result,visit_flag);
		p=p->next;
	}
}

void TestGraph(){
	using namespace std;
	int cases=0,num_of_vertices=0,num_of_edges=0;
	char *vertices;
	scanf("%d",&cases);//读入测试用例数
	while(cases--){
		scanf("%d",&num_of_vertices);//读入顶点数
		char discard;
		scanf("%c",&discard);
		vertices=new char[num_of_vertices];
		for(int i=0;i<num_of_vertices;i++)
			scanf("%c",&vertices[i]);//读入顶点集合
		AdjListDirectGraph<char,int> *a=new AdjListDirectGraph<char,int>(num_of_vertices,vertices);
		scanf("%c",&discard);
		scanf("%d",&num_of_edges);//读入边数
		scanf("%c",&discard);
		while(num_of_edges--){
			char start,end;
			scanf("%c %c",&start,&end);//读入边集合
			scanf("%c",&discard);
			a->Insert(start,end,1);
		}
		//输出入度
		vector<pair<char,int>> indegrees;
		a->GetIndegree(indegrees);
		printf("indegrees:\n");
		for(int i=0;i<indegrees.size();i++)
			printf("%c ",indegrees[i].first);
		printf("\n");
		for(int i=0;i<indegrees.size();i++)
			printf("%d ",indegrees[i].second);
		printf("\n");
		//输出出度
		vector<pair<char,int>> outdegrees;
		a->GetOutdegree(outdegrees);
		printf("indegree:\n");
		for(int i=0;i<outdegrees.size();i++)
			printf("%c ",outdegrees[i].first);
		printf("\n");
		for(int i=0;i<outdegrees.size();i++)
			printf("%d ",outdegrees[i].second);
		printf("\n");
		//广度优先遍历
		vector<char> result;
		result=a->BFS();
		printf("BFS:\n");
		for(int i=0;i<result.size();i++)
			printf("%c ",result[i]);
		printf("\n");
		//深度优先遍历
		result=a->DFSRecursive();
		printf("DFS:\n");
		for(int i=0;i<result.size();i++)
			printf("%c ",result[i]);
		printf("\n");
	}
}

int main(int argc, char *argv[]) {
	TestGraph();
	return 0;
}