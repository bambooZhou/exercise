#include "graph.h"

#include<stdio.h>

#include<iostream>

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
		result=a->DFS();
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