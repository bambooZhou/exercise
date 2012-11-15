#ifndef BINARY_TREE_H_
#define BINARY_TREE_H_

#include<vector>
#include<queue>
#include<stack>

template<typename TypeOfNode>
class BinaryTree{
public:
	enum AlgorithmType{
		RECURSIVE,
		NON_RECURSIVE
	};
	BinaryTree();
	~BinaryTree();
	bool IsEmpty(){ return !root;}
	int Depth() const;
	int Width() const;
	int NumberOfDegreeTwo() const;
	bool IsComplete() const;
	bool IsPerfect() const;
	bool IsFull() const;
	bool IsSymmetry() const;
	bool IsArc() const;
	std::vector<TypeOfNode> PreOrderTravel(AlgorithmType algorithm_type=RECURSIVE);
	std::vector<TypeOfNode> InOrderTravel(AlgorithmType algorithm_type=RECURSIVE);
	std::vector<TypeOfNode> PostOrderTravel(AlgorithmType algorithm_type=RECURSIVE);
	std::vector<TypeOfNode> LevelTravel();
	void BuildTree(std::vector<TypeOfNode> elements,TypeOfNode placeholder);
	void Clear();
	void SwapLR();

private:
	struct TreeNode{
		TypeOfNode data;
		TreeNode *left_child;
		TreeNode *right_child;
		TreeNode(TypeOfNode data,TreeNode *left_child,TreeNode *right_child):
			data(data),left_child(left_child),right_child(right_child){}
	};
	TreeNode *root;
	TypeOfNode placeholder_;
	int depth(TreeNode *node) const;
	void width(TreeNode *node,int &min,int &max,int span) const;
	void numberOfDegreeTwo(TreeNode *node,int& count) const;
	bool isFull(TreeNode *node) const;
	bool isSymmetry(TreeNode *t1,TreeNode *t2) const;
	bool isArc(TreeNode *t1,TreeNode *t2) const;
	void preOrderTravelRecursive(TreeNode *node,std::vector<TypeOfNode> &result);
	void preOrderTravelNonRecursive(std::vector<TypeOfNode> &result);
	void inOrderTravelRecursive(TreeNode *node,std::vector<TypeOfNode> &result);
	void inOrderTravelNonRecursive(std::vector<TypeOfNode> &result);
	void postOrderTravelRecursive(TreeNode *node,std::vector<TypeOfNode> &result);
	void postOrderTravelNonRecursive(std::vector<TypeOfNode> &result);
	void swapLR(TreeNode *node);
	void clear(TreeNode *node);
};

//public member functions
template<typename TypeOfNode>
BinaryTree<TypeOfNode>::BinaryTree(){
	std::cout << "BinaryTree()" <<std::endl;
	root=NULL;
}

template<typename TypeOfNode>
BinaryTree<TypeOfNode>::~BinaryTree(){
	std::cout << "~BinaryTree()" <<std::endl;
	if(NULL!=root)
		clear(root);
	root=NULL;
}

template<typename TypeOfNode>
int BinaryTree<TypeOfNode>::Width() const{
	if(NULL==root)
		return 0;
	int min=0,max=0;
	width(root,min,max,0);
	return max-min;
}

template<typename TypeOfNode>
int BinaryTree<TypeOfNode>::Depth() const{
	if(NULL==root)
		return 0;
	return depth(root);
}

template<typename TypeOfNode>
int BinaryTree<TypeOfNode>::NumberOfDegreeTwo() const{
	int count=0;
	if(NULL==root)
		return count;
	numberOfDegreeTwo(root,count);
	return count;
}

template<typename TypeOfNode>
bool BinaryTree<TypeOfNode>::IsComplete() const{
	using namespace std;
	struct NodeInfo{
		int id;
		TreeNode *node;
		NodeInfo(int id,TreeNode *node):id(id),node(node){}
	};
	int node_count=0;
	int max_node_id=0;
	queue<NodeInfo> q;
	if(NULL==root)//空树
		return true;
	NodeInfo node_info(1,root);
	q.push(node_info);
	while(!q.empty()){
		node_info=q.front();
		if(node_info.node->left_child){
			NodeInfo node_info_left(2*node_info.id,node_info.node->left_child);
			q.push(node_info_left);
		}
		if(node_info.node->right_child){
			NodeInfo node_info_right(2*node_info.id+1,node_info.node->right_child);
			q.push(node_info_right);
		}

		max_node_id=node_info.id;
		q.pop();
		node_count++;
	}
	if(node_count==max_node_id)
		return true;
	else
		return false;
}

template<typename TypeOfNode>
bool BinaryTree<TypeOfNode>::IsPerfect() const{
	using namespace std;
	struct NodeInfo{
		int id;
		TreeNode *node;
		NodeInfo(int id,TreeNode *node):id(id),node(node){}
	};
	int node_count=0;
	int max_node_id=0;
	queue<NodeInfo> q;
	if(NULL==root)//空树
		return true;
	NodeInfo node_info(1,root);
	q.push(node_info);
	while(!q.empty()){
		node_info=q.front();
		if(node_info.node->left_child){
			NodeInfo node_info_left(2*node_info.id,node_info.node->left_child);
			q.push(node_info_left);
		}
		if(node_info.node->right_child){
			NodeInfo node_info_right(2*node_info.id+1,node_info.node->right_child);
			q.push(node_info_right);
		}

		max_node_id=node_info.id;
		q.pop();
		node_count++;
	}
	if(node_count==max_node_id && (node_count&(node_count+1))==0)//2^(n+1)-1,n is the level number
		return true;
	else
		return false;
}

template<typename TypeOfNode>
bool BinaryTree<TypeOfNode>::IsFull() const{
	if(NULL==root)
		return true;
	return isFull(root);
}

template<typename TypeOfNode>
bool BinaryTree<TypeOfNode>::IsSymmetry() const{
	if(NULL==root)
		return true;
	return isSymmetry(root->left_child,root->right_child);
}

template<typename TypeOfNode>
bool BinaryTree<TypeOfNode>::IsArc() const{
	if(NULL==root)
		return true;
	return isArc(root->left_child,root->right_child);
}

template<typename TypeOfNode>
std::vector<TypeOfNode> BinaryTree<TypeOfNode>::PreOrderTravel(AlgorithmType algorithm_type=RECURSIVE){
	using namespace std;
	vector<TypeOfNode> result;
	if(NULL==root)
		return result;
	if(algorithm_type==RECURSIVE)
		preOrderTravelRecursive(root,result);
	if(algorithm_type==NON_RECURSIVE)
		preOrderTravelNonRecursive(result);
	return result;
}

template<typename TypeOfNode>
std::vector<TypeOfNode> BinaryTree<TypeOfNode>::InOrderTravel(AlgorithmType algorithm_type=RECURSIVE){
	using namespace std;
	vector<TypeOfNode> result;
	if(NULL==root)
		return result;
	if(algorithm_type==RECURSIVE)
		inOrderTravelRecursive(root,result);
	if(algorithm_type==NON_RECURSIVE)
		inOrderTravelNonRecursive(result);
	return result;
}

template<typename TypeOfNode>
std::vector<TypeOfNode> BinaryTree<TypeOfNode>::PostOrderTravel(AlgorithmType algorithm_type=RECURSIVE){
	using namespace std;
	vector<TypeOfNode> result;
	if(NULL==root)
		return result;
	if(algorithm_type==RECURSIVE)
		postOrderTravelRecursive(root,result);
	if(algorithm_type==NON_RECURSIVE)
		postOrderTravelNonRecursive(result);
	return result;
}

template<typename TypeOfNode>
std::vector<TypeOfNode> BinaryTree<TypeOfNode>::LevelTravel(){
	using namespace std;
	vector<TypeOfNode> result;
	if(NULL==root)//空树
		return result;

	queue<TreeNode*> q;
	q.push(root);
	while(!q.empty()){
		TreeNode *p=q.front();
		result.push_back(p->data);
		if(p->left_child)
			q.push(p->left_child);
		if(p->right_child)
			q.push(p->right_child);
		q.pop();
	}
	return result;
}

template<typename TypeOfNode>
void BinaryTree<TypeOfNode>::BuildTree(std::vector<TypeOfNode> elements,TypeOfNode placeholder){
	using namespace std;
	placeholder_=placeholder;
	queue<TreeNode *> q;
	int size=elements.size();
	if(size==0)
		return;
	int i=0;
	root=new TreeNode(elements[i],NULL,NULL);
	i++;//下标往前移动
	q.push(root);
	while(i<size && !q.empty()){
		TreeNode *node=q.front();
		if(elements[i]!=placeholder){
			node->left_child=new TreeNode(elements[i],NULL,NULL);
			q.push(node->left_child);
		}
		i++;//下标往前移动
		if(elements[i]!=placeholder){
			node->right_child=new TreeNode(elements[i],NULL,NULL);
			q.push(node->right_child);
		}
		i++;//下标往前移动
		q.pop();
	}
}

template<typename TypeOfNode>
void BinaryTree<TypeOfNode>::Clear(){
	clear(root);
	root=NULL;
}

template<typename TypeOfNode>
void BinaryTree<TypeOfNode>::SwapLR(){
	swapLR(root);
}

//private member functions
template<typename TypeOfNode>
int BinaryTree<TypeOfNode>::depth(TreeNode *node) const{
	if(NULL==node)
		return -1;
	int left_depth=depth(node->left_child);
	int right_depth=depth(node->right_child);
	return left_depth>right_depth ? left_depth+1:right_depth+1;
}

template<typename TypeOfNode>
void BinaryTree<TypeOfNode>::width(TreeNode *node,int &min,int &max,int span) const{
	if(NULL==node->left_child && NULL==node->right_child){
		if(span<min)
			min=span;
		if(span>max)
			max=span;
		return ;
	}
	if(node->left_child){
		width(node->left_child,min,max,span-1);
	}

	if(node->right_child){
		width(node->right_child,min,max,span+1);
	}
	
}

template<typename TypeOfNode>
void BinaryTree<TypeOfNode>::numberOfDegreeTwo(TreeNode *node,int& count) const{
	if(NULL==node)
		return;
	if(node->left_child && node->right_child)
		count++;
	numberOfDegreeTwo(node->left_child,count);
	numberOfDegreeTwo(node->right_child,count);
}

template<typename TypeOfNode>
bool BinaryTree<TypeOfNode>::isFull(TreeNode *node) const{
	if(!node->left_child && !node->right_child)//叶子结点
		return true;
	if(node->left_child && node->right_child)//度为2的结点
		return isFull(node->left_child) && isFull(node->right_child);
	return false;
}

template<typename TypeOfNode>
bool BinaryTree<TypeOfNode>::isSymmetry(TreeNode *t1,TreeNode *t2) const{
	if(NULL==t1 && NULL==t2)
		return true;
	if(NULL==t1 || NULL==t2)
		return false;
	return isSymmetry(t1->left_child,t2->right_child) && 
		isSymmetry(t1->right_child,t2->left_child);
}

template<typename TypeOfNode>
bool BinaryTree<TypeOfNode>::isArc(TreeNode *t1,TreeNode *t2) const{
	if(NULL==t1 && NULL==t2)
		return true;
	if(NULL==t1 || NULL==t2)
		return false;
	if(t1->right_child || t2->left_child)
		return false;
	return isArc(t1->left_child,t2->right_child);
}

template<typename TypeOfNode>
void BinaryTree<TypeOfNode>::preOrderTravelRecursive(TreeNode *node,std::vector<TypeOfNode> &result){
	if(NULL==node)
		return;
	result.push_back(node->data);
	preOrderTravelRecursive(node->left_child,result);
	preOrderTravelRecursive(node->right_child,result);
}

template<typename TypeOfNode>
void BinaryTree<TypeOfNode>::preOrderTravelNonRecursive(std::vector<TypeOfNode> &result){
	using namespace std;
	if(NULL==root)
		return;
	stack<TreeNode *> s;
	TreeNode *current_node=root;
	s.push(current_node);
	while(!s.empty()){
		current_node=s.top();
		s.pop();
		result.push_back(current_node->data);
		if(current_node->right_child){
			s.push(current_node->right_child);
		}
		if(current_node->left_child){
			s.push(current_node->left_child);
		}
	}
}

template<typename TypeOfNode>
void BinaryTree<TypeOfNode>::inOrderTravelRecursive(TreeNode *node,std::vector<TypeOfNode> &result){
	if(NULL==node)
		return;
	inOrderTravelRecursive(node->left_child,result);
	result.push_back(node->data);
	inOrderTravelRecursive(node->right_child,result);
}

template<typename TypeOfNode>
void BinaryTree<TypeOfNode>::inOrderTravelNonRecursive(std::vector<TypeOfNode> &result){
	using namespace std;
	if(NULL==root)
		return;
	struct StackNode{
		int pop_times;
		TreeNode *node;
		StackNode(int pop_times,TreeNode *node):pop_times(pop_times),node(node){}
	};
	stack<StackNode> s;
	StackNode current_node(0,root);
	s.push(current_node);
	while(!s.empty()){
		current_node=s.top();
		s.pop();
		if(++current_node.pop_times==2){
			result.push_back(current_node.node->data);
			if(current_node.node->right_child){
				current_node.pop_times=0;
				current_node.node=current_node.node->right_child;
				s.push(current_node);
			}
		}else{
			s.push(current_node);
			if(current_node.node->left_child){
				current_node.pop_times=0;
				current_node.node=current_node.node->left_child;
				s.push(current_node);
			}
		}
	}
}

template<typename TypeOfNode>
void BinaryTree<TypeOfNode>::postOrderTravelRecursive(TreeNode *node,std::vector<TypeOfNode> &result){
	if(NULL==node)
		return;
	postOrderTravelRecursive(node->left_child,result);
	postOrderTravelRecursive(node->right_child,result);
	result.push_back(node->data);
}

template<typename TypeOfNode>
void BinaryTree<TypeOfNode>::postOrderTravelNonRecursive(std::vector<TypeOfNode> &result){
	using namespace std;
	if(NULL==root)
		return;
	struct StackNode{
		int pop_times;
		TreeNode *node;
		StackNode(int pop_times,TreeNode *node):pop_times(pop_times),node(node){}
	};
	stack<StackNode> s;
	StackNode current_node(0,root);
	s.push(current_node);
	while(!s.empty()){
		current_node=s.top();
		s.pop();
		current_node.pop_times++;//出栈次数增加1
		if(current_node.pop_times==3){//第三次出栈，左右子树都已经访问
			result.push_back(current_node.node->data);
			continue;
		}
		s.push(current_node);
		if(current_node.pop_times==1){//第一次出栈
			if(current_node.node->left_child)
				s.push(StackNode(0,current_node.node->left_child));
		}else{//第二次出栈
			if(current_node.node->right_child)
				s.push(StackNode(0,current_node.node->right_child));
		}
	}
}

template<typename TypeOfNode>
void BinaryTree<TypeOfNode>::swapLR(TreeNode *node){
	if(NULL==node)
		return;
	TreeNode *temp=node->left_child;
	node->left_child=node->right_child;
	node->right_child=temp;
	swapLR(node->left_child);
	swapLR(node->right_child);
}

template<typename TypeOfNode>
void BinaryTree<TypeOfNode>::clear(TreeNode *node){
	if(!node->left_child && !node->right_child){//叶子结点
		delete node;
		return;
	}
	if(node->left_child)
		clear(node->left_child);
	if(node->right_child)
		clear(node->right_child);
	delete node;
}
#endif // BINARY_TREE_H_