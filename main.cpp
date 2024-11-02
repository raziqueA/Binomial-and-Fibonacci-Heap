#include <stdio.h>
#include <iostream>
using namespace std;
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <vector>
#include <map>
#include <list>
#include <queue>

struct binomialHeap{
  struct binomialHeap* parent;
  int key;
  int weight;
  int degree;
  struct binomialHeap* left;
  struct binomialHeap* rightSibling;
};

map<int, list<pair<int,int>>>convertToAdjL(vector<vector<int>>adjM, int n, int d){
  map<int, list<pair<int,int>>>adjL;
  for(int i=1; i <=n; i++){
    for(int j=1; j<=n; j++){
        if(adjM[i][j]!=999999 && adjM[i][j]!=0){
          adjL[i].push_back(make_pair(j, adjM[i][j]));
        }
    }
  }
  return adjL;
}
void printAdjL(map<int, list<pair<int,int>>>adjL){
  for(auto it: adjL){
    int u = it.first;
    cout<<u<<" -> ";
    for(auto it2: it.second){
      int v = it2.first;
      int w = it2.second;
      cout<<v<<"("<<w<<"), ";
    }
    cout<<endl;
  }
}

bool bellmanFord(map<int, list<pair<int,int>>>&adjL, vector<int>&dist, int n){
  dist[0]=0;
  for(int i=0; i<n-1; i++){
    int flag = 0;
    for(auto it: adjL){
      for(auto it2 : it.second){
        int u = it.first;
        int v = it2.first;
        int w = it2.second;
        if(dist[u]+w < dist[v]){
          flag = 1;
          dist[v] = dist[u]+w;
        }
      }
    }
    if(flag==0){
      break;
    }
  }
  for(auto it: adjL){
    for(auto it2 : it.second){
      int u = it.first;
      int v = it2.first;
      int w = it2.second;
      if(dist[u]+w < dist[v]){
        return true;
      }
    }
  }
  return false;
}

void minV(vector<pair<int,int>>&h, int& n){
  int minIndex = 0;
  for(int i=0; i<n; i++){
    if(h[i].first < h[minIndex].first){
      minIndex = i;
    }
  }
  swap(h[minIndex], h[n-1]);
}



void dijkistraArray(map<int, list<pair<int,int>>>& adjL_pos, int n, int src, vector<vector<int>>&dij_dist){
  dij_dist[src][src] = 0;
  priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
  vector<pair<int,int>>h;
  h.push_back({0, src});
  int h_size = 1;
  while(!h.empty()){
    minV(h, h_size);
    pair<int, int>temp = h[h_size-1];
    h_size--;
    int u = temp.second;
    h.pop_back();
    for(auto it: adjL_pos[u]){
      int v = it.first;
      int w = it.second;
      if(dij_dist[src][u]+w < dij_dist[src][v]){
        dij_dist[src][v] = dij_dist[src][u]+w;
        h.push_back({dij_dist[src][u]+w, v});
        h_size++;
      }
    }
  }
}




void dijkistraBinary(map<int, list<pair<int,int>>>& adjL_pos, int n, int src, vector<vector<int>>&dij_dist){
  dij_dist[src][src] = 0;
  priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
  pq.push(make_pair(0, src));
  while(!pq.empty()){
    int u = pq.top().second;
    // int dist = pq.top().first;
    pq.pop();
    for(auto it: adjL_pos[u]){
      int v = it.first;
      int w = it.second;
      if(dij_dist[src][u]+w < dij_dist[src][v]){
        dij_dist[src][v] = dij_dist[src][u]+w;
        pq.push(make_pair(dij_dist[src][u]+w, v));
      }
    }
  }
}
//ek baar dekhlena pure cases handle hore ya nhi
struct binomialHeap* rearrange(struct binomialHeap* dummyList){
  if(dummyList == NULL || dummyList->rightSibling == NULL)
    return dummyList;
  struct binomialHeap* t1 = dummyList;
  struct binomialHeap* t2 = dummyList;
  struct binomialHeap* t3 = dummyList;

  if(dummyList->rightSibling->rightSibling == NULL){
    t2 = dummyList->rightSibling;
    t3 = dummyList->rightSibling->rightSibling;
  }
  else{
    t2 = dummyList->rightSibling;
    t3 = dummyList->rightSibling->rightSibling;
  }
  while(t1 != NULL){
    if(t2==NULL){
      t1 = t1->rightSibling;
    }
    else if(t3!=NULL && t1->degree == t2->degree && t2->degree==t3->degree){
      t1 = t1->rightSibling;
      t2 = t2->rightSibling;
      t3 = t3->rightSibling;
    }
    else if(t1->degree < t2->degree){
      t1 = t1->rightSibling;
      t2 = t2->rightSibling;
      if(t3!=NULL){
        t3 = t3->rightSibling;
      }
    }
    else if(t1->degree == t2->degree){
      //merge last 2 node of same degree
      if(t1->weight < t2->weight){
        // struct binomialHeap* temp = t2;
        t1->rightSibling = t2->rightSibling;
        t2->rightSibling = t1->left;//
        t1->left = t2;//
        t2->parent = t1;//
        t1->degree +=1; 
        t2 = t1->rightSibling;
        if(t3!=NULL){
          t3 = t3->rightSibling;
        }
      }
      else{
        struct binomialHeap* temp = t1;
         if(t1 == dummyList){
           dummyList = t2;
           t1->parent = t2;
           t1->rightSibling = t2->left;
           t2->left=t1;
           t2->degree += 1;
           t1 = t2;
           t2 = t2->rightSibling;
           if(t3!=NULL){
             t3 = t3->rightSibling;
           }
         }
        else{
          //iterate up temp->rightSibling == t1
          while(temp->rightSibling != t1){
            temp = temp->rightSibling;
          }
          temp->rightSibling = t2;
          t1->parent = t2;
          t1->rightSibling = t2->left;
          t2->left=t1;
          t2->degree += 1;
          t1 = t2;
          t2 = t2->rightSibling;
          if(t3!=NULL){
             t3 = t3->rightSibling;
          }
        }
      }
    }
  }
  return dummyList;
}





struct binomialHeap* mergeRootList(struct binomialHeap* rootList, struct binomialHeap* rootList1){
  if(rootList == NULL){
    return rootList1;
  }
  else if(rootList1 == NULL){
    return rootList;
  }
  //try diffferent approach 
  //did
  struct binomialHeap* Y = rootList;
  struct binomialHeap* Z = rootList1;
  struct binomialHeap* dummyList = NULL;
  struct binomialHeap* dummyListTail = NULL;
  while(Y != NULL && Z != NULL){
    if(Y->degree == Z->degree){
      if(Y->weight == Z->weight){
        if(Z->key > Y->key){
          if(dummyList == NULL){
            dummyList = Y;
            dummyListTail = Y;
            Y = Y->rightSibling;
            dummyListTail->rightSibling = NULL;
          }
          else{
            dummyListTail->rightSibling = Y;
            dummyListTail = dummyListTail->rightSibling;
            Y = Y->rightSibling;
            dummyListTail->rightSibling = NULL;
          }
        }
        else{
          if(dummyList == NULL){
            dummyList = Z;
            dummyListTail = Z;
            Z = Z->rightSibling;
            dummyListTail->rightSibling = NULL;
          }
          else{
            dummyListTail->rightSibling = Z;
            dummyListTail = dummyListTail->rightSibling;
            Z = Z->rightSibling;
            dummyListTail->rightSibling = NULL;
          }
        }
      }
      else if(Y->weight > Z->weight){
        if(dummyList == NULL){
          dummyList = Z;
          dummyListTail = Z;
          Z = Z->rightSibling;
          dummyListTail->rightSibling = NULL;
        }
        else{
          dummyListTail->rightSibling = Z;
          dummyListTail = dummyListTail->rightSibling;
          Z = Z->rightSibling;
          dummyListTail->rightSibling = NULL;
        }
      }
      else{
        //Y->weight < Z->weight
        if(dummyList == NULL){
          dummyList = Y;
          dummyListTail = Y;
          Y = Y->rightSibling;
          dummyListTail->rightSibling = NULL;
        }
        else{
          dummyListTail->rightSibling = Y;
          dummyListTail = dummyListTail->rightSibling;
          Y = Y->rightSibling;
          dummyListTail->rightSibling = NULL;
        }
      }
    }
    else if(Y->degree > Z->degree){
      if(dummyList == NULL){
        dummyList = Z;
        dummyListTail = Z;
        Z = Z->rightSibling;
        dummyListTail->rightSibling = NULL;
      }
      else{
        dummyListTail->rightSibling = Z;
        dummyListTail = dummyListTail->rightSibling;
        Z = Z->rightSibling;
        dummyListTail->rightSibling = NULL;
      }
    }
    else{
      //Y->degree < Z->degree
      if(dummyList == NULL){
        dummyList = Y;
        dummyListTail = Y;
        Y = Y->rightSibling;
        dummyListTail->rightSibling = NULL;
      }
      else{
        dummyListTail->rightSibling = Y;
        dummyListTail = dummyListTail->rightSibling;
        Y = Y->rightSibling;
        dummyListTail->rightSibling = NULL;
      }
    }
  }
  while(Y != NULL && Z == NULL){
    if(dummyList == NULL){
      dummyList = Y;
      dummyListTail = Y;
      Y = Y->rightSibling;
      dummyListTail->rightSibling = NULL;
    }
    else{
      dummyListTail->rightSibling = Y;
      dummyListTail = dummyListTail->rightSibling;
      Y = Y->rightSibling;
      dummyListTail->rightSibling = NULL;
    }
  }
  while(Y == NULL && Z != NULL){
    if(dummyList == NULL){
      dummyList = Z;
      dummyListTail = Z;
      Z = Z->rightSibling;
      dummyListTail->rightSibling = NULL;
    }
    else{
      dummyListTail->rightSibling = Z;
      dummyListTail = dummyListTail->rightSibling;
      Z = Z->rightSibling;
      dummyListTail->rightSibling = NULL;
    }
  }
  dummyList = rearrange(dummyList);
  if(dummyList == NULL || dummyList->rightSibling == NULL){
    dummyListTail = dummyList;
  }
  else{
    dummyListTail = dummyList;
    while(dummyListTail->rightSibling != NULL){
      dummyListTail = dummyListTail->rightSibling;
    }
  }

  return dummyList;

}


struct binomialHeap* insertBinomialRoot(struct binomialHeap* rootList, int key, int weight){
  struct binomialHeap* rootList1 = (struct binomialHeap*)malloc(sizeof(binomialHeap));
    rootList1->parent = NULL;
    rootList1->key = key;
    rootList1->weight = weight;
    rootList1->degree=0;
    rootList1->left = NULL;
    rootList1->rightSibling = NULL;
    if(rootList == NULL){
      rootList = rootList1;
      return rootList;
    }
    rootList = mergeRootList(rootList, rootList1);
    rootList = rearrange(rootList);
    return rootList;
}

struct binomialHeap* findMinBino(struct binomialHeap* rootList){
  struct binomialHeap* min = rootList;
  struct binomialHeap* temp = rootList;

  while(temp != NULL){
    if(temp->weight < min->weight){
      min = temp;
    }
    temp = temp->rightSibling;
  }
  return min;
}

struct binomialHeap* reverseList(struct binomialHeap* rootList1){
  if(rootList1 == NULL){
    return NULL;
  }
  struct binomialHeap* rootList2 = NULL;
  //remove each node from rootList1 and insert in front of rootList2
  struct binomialHeap* temp = rootList1;
  while(rootList1 != NULL){
    //dont use function do insert here only
    temp = rootList1;
    rootList1 = rootList1->rightSibling;
    // if(rootList2 == NULL){
    //   rootList2 = temp;
    //   rootList2->rightSibling = NULL;
    // }
    // else{
    temp->rightSibling = rootList2;
    rootList2 = temp;
  }
  return rootList2;
}



//ExtractMin 
struct binomialHeap* extractMinBinomial(struct binomialHeap* rootList){
  //handle case for single tree
  // pair<int, int>ans;
  //single tree
  if(rootList->rightSibling == NULL){
    struct binomialHeap* temp = rootList;
    rootList = rootList->left;
    free(temp);
    return rootList;
  } 

  struct binomialHeap* minEleIndex = findMinBino(rootList);
  struct binomialHeap* temp = rootList;
  //first element is min
  if(temp == minEleIndex){
    rootList = rootList->rightSibling;
    struct binomialHeap* temp = minEleIndex->left;
    free(minEleIndex);
    temp = reverseList(temp);
    rootList = mergeRootList(rootList, temp);
    return rootList;
  }

  //else
  while(temp->rightSibling != minEleIndex){
    temp = temp->rightSibling;
  }
  temp->rightSibling = minEleIndex->rightSibling;
  struct binomialHeap* rootList1 = minEleIndex->left;
  rootList1->parent = NULL;
  free(minEleIndex);
  rootList1 = reverseList(rootList1);
  rootList = mergeRootList(rootList, rootList1);
  return rootList;
}

void dijkistraBinomial(map<int, list<pair<int,int>>>& adjL_pos, int n, int src, vector<vector<int>>&dij_dist){
  dij_dist[src][src] = 0;
  struct binomialHeap* rootList = NULL;
  rootList = insertBinomialRoot(rootList, src, 0);
  while(rootList!=NULL){
    struct binomialHeap* minEleIndex = findMinBino(rootList);
    int u = minEleIndex->key;
    rootList = extractMinBinomial(rootList);
    for(auto it: adjL_pos[u]){
      int v = it.first;
      int w = it.second;
      if(dij_dist[src][u]+w < dij_dist[src][v]){
        dij_dist[src][v] = dij_dist[src][u]+w;
        rootList = insertBinomialRoot(rootList, v, dij_dist[src][u]+w);
      }
    }
  }
}


//-------------------fibonacci --------------------------------


struct FibonacciNode {
    int weight;
    int key;
    int degree;
    int marked;
    struct FibonacciNode* parent;
    struct FibonacciNode* lefftMostChild;
    struct FibonacciNode* rightSibling;
    struct FibonacciNode* leftSibling;
};

struct FibonacciHeap {
    struct FibonacciNode* min_node;
    int num_nodes;
};

struct FibonacciNode* create_node(int key, int weight) {
    struct FibonacciNode* new_node = (struct FibonacciNode*)malloc(sizeof(struct FibonacciNode));
    new_node->key = key;
    new_node->weight = weight;
    new_node->degree = 0;
    new_node->marked = 0;
    new_node->parent = NULL;
    new_node->lefftMostChild = NULL;
    new_node->rightSibling = new_node;
    new_node->leftSibling = new_node;
    return new_node;
}

// Function to initialize a new Fibonacci heap
struct FibonacciHeap* initialize_heap() {
    struct FibonacciHeap* new_heap = (struct FibonacciHeap*)malloc(sizeof(struct FibonacciHeap));
    new_heap->min_node = NULL;
    new_heap->num_nodes = 0;
    return new_heap;
}

struct FibonacciNode* ajuBajuLagado(struct FibonacciNode* node1, struct FibonacciNode* node2);

// done - Function to insert a new weight into the Fibonacci heap
void insertInFib(struct FibonacciHeap* fib_heap, int key, int weight) {
  struct FibonacciNode* new_node = create_node(key, weight);

  if (fib_heap->min_node == NULL) {
      fib_heap->min_node = new_node;
  } 
  else {
      new_node->rightSibling = fib_heap->min_node->rightSibling;
      new_node->leftSibling = fib_heap->min_node;
      fib_heap->min_node->rightSibling->leftSibling = new_node;
      fib_heap->min_node->rightSibling = new_node;
      if (new_node->weight < fib_heap->min_node->weight) {
          fib_heap->min_node = new_node;
      }
  }
  (fib_heap->num_nodes)++;
}


void linkKarDo(struct FibonacciNode* y, struct FibonacciNode* x) {
  // Make y a lefftMostChild of x
  struct FibonacciNode* node1 = NULL;
  struct FibonacciNode* node2 = NULL;
  // y->leftSibling = y->rightSibling = y; //isolate krr diya - waise already tha
  if(node1!=node2){
    y->rightSibling = x->lefftMostChild;
    // Link y to the root list of x
    if(x->lefftMostChild != NULL){
      y->leftSibling = x->lefftMostChild->leftSibling;
      x->lefftMostChild->leftSibling->rightSibling = y;
      x->lefftMostChild->leftSibling = y;
    }
    x->lefftMostChild = y;
  }
  //-----------ye hi likh do directly
  y->parent = x;
  x->degree++;
  x->lefftMostChild = ajuBajuLagado(x->lefftMostChild, y);
}


void consolidate(struct FibonacciHeap* fib_heap) {
  int size = (int)(log2(fib_heap->num_nodes)) + 1;

  // struct FibonacciNode* degree_table[max_degree];
  vector<struct FibonacciNode*>DegreeArray(size, NULL);
  vector<struct FibonacciNode*>tempArray; 
  // for (int i = 0; i < max_degree; i++) {
  //     degree_table[i] = NULL;
  // }
  struct FibonacciNode* start = fib_heap->min_node;
  struct FibonacciNode* temp = start;
  int count =0;
  do{
    tempArray.push_back(temp);
    count++;
    temp=temp->rightSibling;
  }while(temp != start);

  for(int i=0; i<count; i++){
    // isolate each node
    struct FibonacciNode* x = tempArray[i];
    x->rightSibling = x->leftSibling = x; // ya NULL
  }

  for(int i=0; i<count; i++){
    int degree = tempArray[i]->degree;
    struct FibonacciNode* x = tempArray[i];
    while(DegreeArray[degree] != NULL){
      struct FibonacciNode* y = DegreeArray[degree];
      if(x->weight > y->weight){
        struct FibonacciNode* temp1 = x;
        x = y;
        y = temp1;
      }
      else if(x->weight == y->weight){
        if(x->key > y->key){
          struct FibonacciNode* temp1 = x;
          x = y;
          y = temp1;
        }
      }
      // linkKarDo(fib_heap, y, x); degree yaad se update krna iske ander
      //        x
      //      /
      //    y
      y->parent = x;
      x->degree++;
      x->lefftMostChild = ajuBajuLagado(x->lefftMostChild, y);
      //
      // iddhr min bhi update krr sakte hai
      DegreeArray[degree] = NULL;
      degree++;
    }
    DegreeArray[degree] = x; //ya DegreeArray[x->degree] = x;
    fib_heap->min_node = x; // is bahar nikal k null bhi krr sakte hai
  }

  //fib_heap->min_node = min(degreeArray[i] for all i)
  for(int i=0; i<size; i++){
    if(DegreeArray[i] != NULL){
      if(fib_heap->min_node == NULL){
        fib_heap->min_node = DegreeArray[i];
      }
      else{
        if(DegreeArray[i]->weight < fib_heap->min_node->weight){
          fib_heap->min_node = DegreeArray[i];
        }
        else if(DegreeArray[i]->weight == fib_heap->min_node->weight){
          if(DegreeArray[i]->key < fib_heap->min_node->key){
             fib_heap->min_node = DegreeArray[i];         
          }
        }
      }
    }
  }

  //ab sab ko merge karenge 
  for(int i=0; i<size; i++){
    if(DegreeArray[i] != NULL){
      if(fib_heap->min_node != DegreeArray[i])
        fib_heap->min_node = ajuBajuLagado(fib_heap->min_node, DegreeArray[i]);
    }
  }
}



void makeParentNull(struct FibonacciNode* node){
  if(node == NULL){
    return;
  }
  struct FibonacciNode* temp = node;
  do{
    temp->parent = NULL;
    temp = temp->rightSibling;
  }while(temp != node);
}

struct FibonacciNode* ajuBajuLagado(struct FibonacciNode* node1, struct FibonacciNode* node2){
  if(node1 == NULL){
    return node2;
  }
  if(node2 == NULL){
    return node1;
  }
  struct FibonacciNode* node1R = node1->rightSibling; 
  struct FibonacciNode* node2L = node2->leftSibling;
  node1->rightSibling = node2;
  node2->leftSibling = node1;
  node2L->rightSibling = node1R;
  node1R->leftSibling = node2L;
  return node1; 
}

void removeThisNode(struct FibonacciNode* node){
  if(node == NULL || node->rightSibling == node){
    return;
  }
  struct FibonacciNode* nodeR = node->rightSibling;
  struct FibonacciNode* nodeL = node->leftSibling;
  nodeL->rightSibling = nodeR;
  nodeR->leftSibling = nodeL;
  node->leftSibling = node;
  node->rightSibling = node;
  // free(node);
}



// Function to extract the minimum node from the Fibonacci heap
struct FibonacciNode* extractMinFib(struct FibonacciHeap* fib_heap) {
  struct FibonacciNode* min_node = fib_heap->min_node;
  //child ko root list me merge krne wala O(1) code krna hai isko hata dena
  if (min_node != NULL) {
      // Remove min_node from the root list
    //only tree
    if(min_node->rightSibling  == min_node && min_node->lefftMostChild == NULL){
      fib_heap->min_node = NULL;
      fib_heap->num_nodes--;
      return min_node;
    }
    //make parent null
    makeParentNull(min_node->lefftMostChild);
    min_node = ajuBajuLagado(min_node, min_node->lefftMostChild);
    if(min_node == min_node->rightSibling){
      fib_heap->min_node = NULL;
    }
    else{
      fib_heap->min_node = min_node->rightSibling;
    }
    removeThisNode(min_node);
    if(fib_heap->min_node != NULL){
      consolidate(fib_heap);
    }
  }
  else{
    return NULL;
  }
  fib_heap->num_nodes--;
  return min_node;
}


// // Function to free the memory used by a node and its lefftMostChildren
void free_node(struct FibonacciNode* node) {
    if (node != NULL) {
        struct FibonacciNode* current = node;
        do {
            struct FibonacciNode* rightSibling = current->rightSibling;
            free_node(current->lefftMostChild);
            free(current);
            current = rightSibling;
        } while (current != node);
    }
}

// // Function to free the memory used by the Fibonacci heap
void free_heap(struct FibonacciHeap* fib_heap) {
    free_node(fib_heap->min_node);
    free(fib_heap);
}

void dijkistraFib(map<int, list<pair<int,int>>>& adjL_pos, int n, int src, vector<vector<int>>&dij_dist){
  dij_dist[src][src] = 0;
  struct FibonacciHeap* fib_heap = initialize_heap();
  insertInFib(fib_heap, src, 0);
  // cout<<fib_heap->min_node->weight<<" "<<fib_heap->min_node->key<<endl;

  // extractMinFib(fib_heap);
  // if(fib_heap->min_node == NULL){
  //   cout<<"Empty Heap"<<endl;
  // }

  while(fib_heap->min_node!=NULL){
    int u = fib_heap->min_node->key;
    // struct FibonacciNode* min_node = 
    extractMinFib(fib_heap);
    // free(min_node);
    for(auto it: adjL_pos[u]){
      int v = it.first;
      int w = it.second;
      if(dij_dist[src][u]+w < dij_dist[src][v]){
        dij_dist[src][v] = dij_dist[src][u]+w;
        insertInFib(fib_heap, v, dij_dist[src][u]+w);
      }
    }
  }
  // free_heap(fib_heap);
}


int main() {
  //Johnson Algorithm

  int t;
  cin>>t;
  while(t--){
    int n, d;
    cin>>n>>d;
    bool negEdge = false;
    vector<vector<int>>adjM(n+1, vector<int>(n+1,999999));
    for(int i=1; i<=n; i++){
      for(int j=1; j<=n; j++){
        cin>>adjM[i][j];
        if(adjM[i][j]<0){
          negEdge = true;
        }
      }
    }
    map<int, list<pair<int,int>>>adjL = convertToAdjL(adjM, n, d);
    map<int, list<pair<int,int>>>adjL1 = adjL;
    for(int i=1; i<=n; i++){
      adjL1[0].push_back({i,0});
    }
    vector<int>dist(n+1, 999999);
    bool negCycleExists = bellmanFord(adjL1, dist, n+1);
    if(d == 1 && negCycleExists){
      cout<<"-1"<<endl;
      continue;
    }
    else if(d == 0 && negEdge){
      cout<<"-1"<<endl;
      continue;
    }
    else if(d==0 && !negEdge){
      //directly apply n times dijkishtra
      vector<vector<int>>dij_dist_arr(n+1,vector<int>(n+1, 999999));
      vector<vector<int>>dij_dist_binary(n+1,vector<int>(n+1, 999999));
      vector<vector<int>>dij_dist_bino(n+1,vector<int>(n+1, 999999));
      vector<vector<int>>dij_dist_fib(n+1,vector<int>(n+1, 999999));
      for(int i=1; i<=n; i++){
        dijkistraBinary(adjL, n, i, dij_dist_binary);
      }

      for(int i=1; i<=n; i++){
        cout<<"here";
        dijkistraArray(adjL, n, i, dij_dist_arr);
      }

      for(int i=1; i<=n; i++){
        dijkistraBinomial(adjL, n, i, dij_dist_bino);
      }

      for(int i=1; i<=n; i++){
        dijkistraFib(adjL, n, i, dij_dist_fib);
      }
      cout<<"Printing Result of array: "<<endl;
      for(int i=1 ; i<=n; i++){
        for(int j=1; j<=n; j++){
          cout<<dij_dist_arr[i][j]<<" ";
        }
        cout<<endl;
      }
      cout<<"Printing Result of binary Heap: "<<endl;
      for(int i=1 ; i<=n; i++){
        for(int j=1; j<=n; j++){
          cout<<dij_dist_binary[i][j]<<" ";
        }
        cout<<endl;
      }
      cout<<"Printing Result of binomial Heap: "<<endl;
      for(int i=1 ; i<=n; i++){
        for(int j=1; j<=n; j++){
          cout<<dij_dist_bino[i][j]<<" ";
        }
        cout<<endl;
      }
      cout<<"Printing Result of fibonacci Heap: "<<endl;
      for(int i=1 ; i<=n; i++){
        for(int j=1; j<=n; j++){
          cout<<dij_dist_fib[i][j]<<" ";
        }
        cout<<endl;
      }
      continue;
    }
    map<int, list<pair<int,int>>>adjL_pos = adjL;
    for(auto &it: adjL_pos){
      for(auto &nbr: it.second){
        nbr.second = nbr.second+dist[it.first]-dist[nbr.first]; 
      }
    }
    vector<vector<int>>dij_dist_arr(n+1,vector<int>(n+1, 999999));
    vector<vector<int>>dij_dist_binary(n+1,vector<int>(n+1, 999999));
    vector<vector<int>>dij_dist_bino(n+1,vector<int>(n+1, 999999));
    vector<vector<int>>dij_dist_fib(n+1,vector<int>(n+1, 999999));
    for(int i=1; i<=n; i++){
      dijkistraBinary(adjL_pos, n, i, dij_dist_binary);
    }

    for(int i=1; i<=n; i++){
      dijkistraArray(adjL_pos, n, i, dij_dist_arr);
    }

    for(int i=1; i<=n; i++){
      dijkistraBinomial(adjL_pos, n, i, dij_dist_bino);
    }

    for(int i=1; i<=n; i++){
      dijkistraFib(adjL_pos, n, i, dij_dist_fib);
    }

    for(int i=1; i<=n; i++){
      for(int j=1; j<=n; j++){
        if(dij_dist_arr[i][j] != 999999)
          dij_dist_arr[i][j]=dij_dist_arr[i][j]-dist[i]+dist[j];
      }
    }

    for(int i=1; i<=n; i++){
      for(int j=1; j<=n; j++){
        if(dij_dist_binary[i][j] != 999999)
          dij_dist_binary[i][j]=dij_dist_binary[i][j]-dist[i]+dist[j];
      }
    }
    for(int i=1; i<=n; i++){
      for(int j=1; j<=n; j++){
         if(dij_dist_bino[i][j] != 999999)
            dij_dist_bino[i][j]=dij_dist_bino[i][j]-dist[i]+dist[j];
      }
    }
    for(int i=1; i<=n; i++){
      for(int j=1; j<=n; j++){
         if(dij_dist_fib[i][j] != 999999)
            dij_dist_fib[i][j]=dij_dist_fib[i][j]-dist[i]+dist[j];
      }
    }


    cout<<"Printing Result of array: "<<endl;
    for(int i=1 ; i<=n; i++){
      for(int j=1; j<=n; j++){
        cout<<dij_dist_arr[i][j]<<" ";
      }
      cout<<endl;
    }


    cout<<"Printing Result of binary Heap: "<<endl;
    for(int i=1 ; i<=n; i++){
      for(int j=1; j<=n; j++){
        cout<<dij_dist_binary[i][j]<<" ";
      }
      cout<<endl;
    }

    cout<<"Printing Result of binomial Heap: "<<endl;
    for(int i=1 ; i<=n; i++){
      for(int j=1; j<=n; j++){
        cout<<dij_dist_bino[i][j]<<" ";
      }
      cout<<endl;
    }
    cout<<"Printing Result of fibonacci Heap: "<<endl;
    for(int i=1 ; i<=n; i++){
      for(int j=1; j<=n; j++){
        cout<<dij_dist_fib[i][j]<<" ";
      }
      cout<<endl;
    }

  }
  return 0;
}
