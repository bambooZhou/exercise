#ifndef GRAPH_H_
#define GRAPH_H_

#include<iostream>
#include<utility>
#include<vector>
#include<queue>

/**
*图的抽象类
*
*/
template<typename TypeOfVertex,typename TypeOfWeight>
class Graph{
public:
	Graph(){ num_of_vertices_=0;num_of_edges_=0;}
	virtual ~Graph(){};
	/**
	*功能：返回图的深度度优先遍历序列
	*返回值：深度优先遍历序列
	*/
	virtual std::vector<TypeOfVertex> DFS() const=0;
	/**
	*功能：返回图的广度优先遍历序列
	*返回值：广度优先遍历序列
	*/
	virtual std::vector<TypeOfVertex> BFS() const=0;
	/**
	*功能：往图中插入一条边
	*参数：u----边的起点	v----边的终点	w----边的权值
	*返回值：如果插入成功，返回true，否则，返回false
	*/
	virtual bool Insert(TypeOfVertex u,TypeOfVertex v,TypeOfWeight w)=0;
	/**
	*功能：从图中移除一条边
	*参数：u----边的起点	v----边的终点
	*返回值：如果移除成功，返回true，否则，返回false
	*/
	virtual bool Remove(TypeOfVertex u,TypeOfVertex v)=0;
	/**
	*功能：判断图中一条边是否存在
	*参数：u----边的起点	v----边的终点
	*返回值：如果存在，返回true，否则，返回false
	*/
	virtual bool Exist(TypeOfVertex u,TypeOfVertex v) const=0;
	/**
	*功能：输出图的相关信息
	*/
	virtual void PrintGraphInfo() const=0;
	/**
	*功能：返回图的顶点数
	*/
	int num_of_vertices(){ return num_of_vertices_;}
	/**
	*功能：返回图的边数
	*/
	int num_of_edges(){ return num_of_edges_;}
protected:
	int num_of_vertices_;//保存顶点数
	int num_of_edges_;//保存边数
};

/**
*使用邻接列表存储的有向图实现类
*/
template<typename TypeOfVertex,typename TypeOfWeight>
class AdjListDirectGraph:public Graph<TypeOfVertex,TypeOfWeight>{
public:
	/**
	*构造函数
	*参数：	num_of_vertices----顶点的个数	vertexs----顶点集合
	*/
	AdjListDirectGraph(int num_of_vertices,TypeOfVertex vertexs[]);
	/**
	*析构函数
	*/
	~AdjListDirectGraph();
	/**
	*功能：返回有向图的深度度优先遍历序列
	*返回值：深度优先遍历序列
	*/
	std::vector<TypeOfVertex> DFS() const;
	/**
	*功能：返回有向图的广度优先遍历序列
	*返回值：广度优先遍历序列
	*/
	std::vector<TypeOfVertex> BFS() const;
	/**
	*功能：获取有向图的每个结点的入度
	*参数：入度关联对向量的引用，此成员函数将结果注入此引用的对象
	*/
	void GetIndegree(std::vector<std::pair<TypeOfVertex,int>>& indegrees);
	/**
	*功能：获取有向图的每个结点的出度
	*参数：出度关联对向量的引用，此成员函数将结果注入此引用的对象
	*/
	void GetOutdegree(std::vector<std::pair<TypeOfVertex,int>>& outdegrees);
	/**
	*功能：往有向图插入一条边
	*参数：u----边的起点	v----边的终点	w----边的权值
	*返回值：如果插入成功，返回true，否则，返回false
	*/
	virtual bool Insert(TypeOfVertex u,TypeOfVertex v,TypeOfWeight w);
	/**
	*功能：从有向图移除一条边
	*参数：u----边的起点	v----边的终点
	*返回值：如果移除成功，返回true，否则，返回false
	*/
	virtual bool Remove(TypeOfVertex u,TypeOfVertex v);
	/**
	*功能：判断有向图一条边是否存在
	*参数：u----边的起点	v----边的终点
	*返回值：如果存在，返回true，否则，返回false
	*/
	virtual bool Exist(TypeOfVertex u,TypeOfVertex v) const;
	/**
	*功能：输出图的相关信息，顶点数和边数
	*/
	virtual void PrintGraphInfo() const;
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
	void dfs(VertexNode& v,std::vector<TypeOfVertex>& result,bool visit_flag[]) const;
};

//公有成员函数实现
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
std::vector<TypeOfVertex> AdjListDirectGraph<TypeOfVertex, TypeOfWeight>::DFS() const{
	std::vector<TypeOfVertex> result;
	bool *visit_flag=new bool[num_of_vertices_];
	for(int i=0;i<num_of_vertices_;i++)
		visit_flag[i]=false;
	for(int i=0;i<num_of_vertices_;i++){
		if(visit_flag[i])
			continue;
		else
			dfs(vertex_node_[i],result,visit_flag);
	}
	return result;
}

template<typename TypeOfVertex,typename TypeOfWeight>
std::vector<TypeOfVertex> AdjListDirectGraph<TypeOfVertex, TypeOfWeight>::BFS() const{
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
void AdjListDirectGraph<TypeOfVertex, TypeOfWeight>::PrintGraphInfo() const{
	using namespace std;
	cout << "number of vertices is " << num_of_vertices_ << endl;
	cout << "number of edges is " << num_of_edges_ << endl;
}

//私有成员函数实现
template<typename TypeOfVertex,typename TypeOfWeight>
int AdjListDirectGraph<TypeOfVertex, TypeOfWeight>::GetVertexIndex(const TypeOfVertex v) const{
	for(int i=0;i<num_of_vertices_;i++){
		if(vertex_node_[i].vertex==v)
			return i;
	}
	return -1;
}

template<typename TypeOfVertex,typename TypeOfWeight>
void AdjListDirectGraph<TypeOfVertex, TypeOfWeight>::dfs(VertexNode& v,std::vector<TypeOfVertex>& result,bool visit_flag[]) const{
	result.push_back(v.vertex);
	int i=GetVertexIndex(v.vertex);
	visit_flag[i]=true;
	EdgeNode *p=v.edge_list;
	while(p){
		int i=p->nearby_vertex_index;
		if(!visit_flag[i])
			dfs(vertex_node_[i],result,visit_flag);
		p=p->next;
	}
}

#endif // GRAPH_H_