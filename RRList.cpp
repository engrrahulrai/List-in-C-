#include<iostream>
#include<string.h>
#define bool int 
#define true 1
#define True 1
#define TRUE 1
#define False 0
#define false 0
#define FALSE 0
using  namespace std;

class Iterator
{
private:
Iterator *iterator;
int releaseIteratorAfterIteration;
public:
Iterator()
{
this->iterator=NULL;
this->releaseIteratorAfterIteration=1;
}
Iterator(Iterator *iterator)
{
this->iterator=iterator;
this->releaseIteratorAfterIteration=1;
}
Iterator(const Iterator &other)
{
this->iterator=other.iterator;
this->releaseIteratorAfterIteration=1;
}
virtual ~Iterator()
{
if(this->releaseIteratorAfterIteration==1) delete iterator;
}
Iterator & operator=(const Iterator &other)
{
this->iterator=other.iterator;
this->releaseIteratorAfterIteration=1;
return *this;
}
void setReleaseIteratorAfterIteration(int x)
{
this->releaseIteratorAfterIteration=x;
}
virtual int hasMoreElements()
{
if(this->iterator==NULL) return 0;
return this->iterator->hasMoreElements();
}
virtual int getNext()
{
if(this->iterator==NULL) return 0;
return this->iterator->getNext();
}
};
class Iterable
{
public:
virtual Iterator getIterator()=0;
};

class RRList:public Iterable
{
public:
virtual void add(int data, bool *success)=0;
virtual void insertAt(int index, int data, bool *success)=0;
virtual void update(int index, int data, bool * success)=0;
virtual int removeAt(int index, bool *success)=0;
virtual int get(int index, bool *success) const=0;
virtual int getSize() const=0;
virtual void removeAll()=0;
virtual void clear()=0;
};

// RRArrayList implementation starts here 

class RRArrayList:public RRList
{
private:
int **ptr;
int size;
int capacity;
char allocationFlag; // for handeling the issue of + operator
bool addRow();

class RRArrayListIterator:public Iterator
{
private:
int index,size;
int **ptr;
public:
RRArrayListIterator(int  **ptr, int size)
{
this->ptr=ptr;
this->size=size;
this->index=0;
}
RRArrayListIterator & operator=(const RRArrayListIterator &other)
{
this->ptr=other.ptr;
this->size=other.size;
return *this;
}
int hasMoreElements()
{
return index<this->size;
}
int getNext()
{
if(index==this->size) return 0;
int rowIndex=index/10;
int columnIndex=index%10;
int data=this->ptr[rowIndex][columnIndex];
this->index++;
return data;
}
};
public:
Iterator getIterator()
{
RRArrayListIterator *rrArrayListIterator;
rrArrayListIterator=new RRArrayListIterator(this->ptr,this->size);
Iterator k(rrArrayListIterator);
k.setReleaseIteratorAfterIteration(0);
return k;
}

// constructors
RRArrayList();
RRArrayList(int bufferSize);
RRArrayList(const RRList &other);
RRArrayList(const RRArrayList &other);
~RRArrayList();

// operators
RRArrayList & operator=(const RRList &other);
RRArrayList operator+(const RRList &other);
void operator+=(const RRList &other);
RRArrayList & operator=(const RRArrayList &other);
RRArrayList operator+(const RRArrayList &other);
void operator+=(const RRArrayList &other);

// methods
void add(int data, bool *success);
void insertAt(int index, int data, bool *success);
void update(int index, int data, bool * success);
int removeAt(int index, bool *success);
int get(int index, bool *success) const;
int getSize() const;
void removeAll();
void clear();
};


bool RRArrayList::addRow()
{
if(this->capacity%100==0)
{
int numberOfPointers=this->capacity/10;
int **tmp=new int*[numberOfPointers+10];
if(tmp==NULL) return false;
for(int e=0;e<numberOfPointers;e++) tmp[e]=this->ptr[e];
delete [] this->ptr;
this->ptr=tmp;
}
int i=this->capacity/10;
this->ptr[i]=new int[10];
if(this->ptr[i]==NULL) return false;
this->capacity=this->capacity+10;
return true;
}
// constructors RRArrayList

RRArrayList::RRArrayList()
{
this->ptr=new int*[10];
this->ptr[0]=new int[10];
this->size=0;
this->allocationFlag=0;
this->capacity=10;
}
RRArrayList::RRArrayList(int bufferSize)
{
if(bufferSize<=0)
{
this->ptr=new int*[10];
this->ptr[0]=new int[10];
this->size=0;
this->allocationFlag=0;
this->capacity=10;
}
else
{
int rows=bufferSize/10;
if(bufferSize%10!=0) rows++;
int numberOfPointers=rows+(10-(rows%10));
this->ptr=new int*[numberOfPointers];
for(int e=0;e<rows;e++) this->ptr[e]=new int[10];
this->capacity=rows*10;
this->size=0;
this->allocationFlag=0;
}
}
RRArrayList::RRArrayList(const RRList &other)
{
int bufferSize=other.getSize();
if(bufferSize<=0)
{
this->ptr=new int*[10];
this->ptr[0]=new int[10];
this->size=0;
this->allocationFlag=0;
this->capacity=10;
}
else
{
int rows=bufferSize/10;
if(bufferSize%10!=0) rows++;
int numberOfPointers=rows+(10-(rows%10));
this->ptr=new int*[numberOfPointers];
for(int e=0;e<rows;e++) this->ptr[e]=new int[10];
this->capacity=rows*10;
this->size=0;
this->allocationFlag=0;
int succ;
for(int e=0;e<other.getSize();e++) this->add(other.get(e,&succ),&succ);
}
}
RRArrayList::RRArrayList(const RRArrayList &other)
{
this->ptr=new int*[10];
this->ptr[0]=new int[10];
this->size=0;
this->capacity=10;
this->allocationFlag=0;
int succ;
for(int e=0;e<other.size;e++) this->add(other.get(e,&succ),&succ);
}
RRArrayList::~RRArrayList()
{
if(allocationFlag=0)
{
int  i=this->capacity/10;
for(int e=0;e<i;e++) delete  [] this->ptr[e];
delete [] this->ptr;
}
}

// operators RRArrayList
RRArrayList & RRArrayList::operator=(const RRList &other)
{
this->size=0;
int succ;
for(int e=0;e<other.getSize();e++) 
{
this->add(other.get(e,&succ),&succ);
}
return *this;
}
RRArrayList RRArrayList::operator+(const RRList &other)
{
RRArrayList k;
k=(*this);
k+=(other);
k.allocationFlag=1;
return k;
}
void RRArrayList::operator+=(const RRList &other)
{
int succ;
for(int e=0;e<other.getSize();e++) this->add(other.get(e,&succ),&succ);
}
RRArrayList & RRArrayList::operator=(const RRArrayList &other)
{
this->size=0;
if(allocationFlag==1)
{
this->ptr=other.ptr;
this->size=other.size;
this->capacity=other.capacity;
}
else
{
int succ;
for(int e=0;e<other.size;e++) this->add(other.get(e,&succ),&succ);
}
return *this;
}

RRArrayList RRArrayList::operator+(const RRArrayList &other)
{
RRArrayList k;
k=(*this);
k+=(other);
k.allocationFlag=1;
return k;
}
void RRArrayList::operator+=(const RRArrayList &other)
{
int succ;
for(int e=0;e<other.size;e++) this->add(other.get(e,&succ),&succ);
}

// methods RRArrayList
void RRArrayList::add(int data, bool *success)
{
if(success) *success=false;
if(this->size==this->capacity)
{
if(!addRow()) return;
}
int rowIndex=this->size/10;
int columnIndex=this->size%10;
this->ptr[rowIndex][columnIndex]=data;
this->size++;
if(success) *success=true;
}
void RRArrayList::insertAt(int index, int data, bool *success)
{
if(success) *success=false;
if(index<0 || index>this->size) return;
if(index==this->size)
{
this->add(data,success);
return;
}
int succ;
int k=this->get(this->size-1,&succ);
this->add(k,&succ);
if(!succ) return;
int j=this->size-3;
while(j>=index)
{
this->update(j+1,this->get(j,&succ),&succ);
j--;
}
this->update(index,data,&succ);
if(success) *success=true;
}
void RRArrayList::update(int index, int data, bool * success)
{
if(success) *success=false;
if(index<0 || index>=this->size) return;
int rowIndex=index/10;
int columnIndex=index%10;
this->ptr[rowIndex][columnIndex]=data;
if(success) *success=true;
}
int RRArrayList::removeAt(int index, bool *success)
{
if(success) *success=false;
if(index<0 || index>=this->size) return 0;
int succ;
int data=this->get(index,&succ);
int j=index;
int ep=this->size-2;
while(j<=ep)
{
this->update(j,this->get(j+1,&succ),&succ);
j++;
}
this->size--;
if(success) *success=true;
return data;
}
int RRArrayList::get(int index, bool *success) const
{
if(success) *success=false;
if(index<0 || index>=this->size) return 0;
int rowIndex=index/10;
int columnIndex=index%10;
if(success) *success=true;
return this->ptr[rowIndex][columnIndex];
}
int RRArrayList::getSize() const
{
return this->size;
}
void RRArrayList::removeAll()
{
this->clear();
}
void RRArrayList::clear()
{
int  i=this->capacity/10;
for(int e=0;e<i;e++) delete [] this->ptr[e];
this->size=0;
this->capacity=0;
delete [] this->ptr;
}

// RRForwardList implementation starts here

class RRForwardList:public RRList
{
class RRNode
{
public:
int data;
RRNode *next;
RRNode()
{
this->next=NULL;
}
};
private:
RRNode *start,*end;
int size;
char allocationFlag;

class RRForwardListIterator:public Iterator
{
private:
RRNode *ptr;
public:
RRForwardListIterator()
{
this->ptr=NULL;
}
RRForwardListIterator(RRNode *ptr)
{
this->ptr=ptr;
}
RRForwardListIterator & operator=(const RRForwardListIterator &other)
{
this->ptr=other.ptr;
return *this;
}
int hasMoreElements()
{
return this->ptr!=NULL;
}
int getNext()
{
if(this->ptr==NULL) return 0;
int data=this->ptr->data;
this->ptr=this->ptr->next;
return data;
}
};
public:
Iterator getIterator()
{
RRForwardListIterator *rrForwardListIterator;
rrForwardListIterator=new RRForwardListIterator(this->start);
Iterator k(rrForwardListIterator);
k.setReleaseIteratorAfterIteration(0);
return k;
}

// constructors
RRForwardList();
RRForwardList(int bufferSize);
RRForwardList(const RRList &other);
RRForwardList(const RRForwardList &other);
~RRForwardList();

// operators
RRForwardList & operator=(const RRList &other);
RRForwardList operator+(const RRList &other);
void operator+=(const RRList &other);
RRForwardList & operator=(const RRForwardList &other);
RRForwardList operator+(const RRForwardList &other);
void operator+=(const RRForwardList &other);

// methods
void add(int data, bool *success);
void insertAt(int index, int data, bool *success);
void update(int index, int data, bool * success);
int removeAt(int index, bool *success);
int get(int index, bool *success) const;
int getSize() const;
void removeAll();
void clear();
};

// constructors RRForwardList
RRForwardList::RRForwardList()
{
this->start=this->end=NULL;
this->size=0;
this->allocationFlag=0;
}
RRForwardList::RRForwardList(int bufferSize)
{
this->start=this->end=NULL;
this->size=0;
this->allocationFlag=0;
}
RRForwardList::RRForwardList(const RRList &other)
{
this->start=this->end=NULL;
this->size=0;
this->allocationFlag=0;
int succ;
for(int e=0;e<other.getSize();e++) this->add(other.get(e,&succ),&succ);
}
RRForwardList::RRForwardList(const RRForwardList &other)
{
this->start=this->end=NULL;
this->size=0;
this->allocationFlag=0;
int succ;
for(int e=0;e<other.getSize();e++) this->add(other.get(e,&succ),&succ);
}
RRForwardList::~RRForwardList()
{
if(allocationFlag==0) this->clear();
}

// operators RRForwardList
RRForwardList & RRForwardList::operator=(const RRList &other)
{
this->clear();
int succ;
for(int e=0;e<other.getSize();e++) this->add(other.get(e,&succ),&succ);
return *this;
}
RRForwardList RRForwardList::operator+(const RRList &other)
{
RRForwardList k;
k=(*this);
k+=(other);
return k;
}
void RRForwardList::operator+=(const RRList &other)
{
int succ;
for(int e=0;e<other.getSize();e++) this->add(other.get(e,&succ),&succ);
}
RRForwardList & RRForwardList::operator=(const RRForwardList &other)
{
this->clear();
if(other.allocationFlag==1)
{
this->start=other.start;
this->end=other.end;
this->size=other.size;
}
else
{
int succ;
for(int e=0;e<other.size;e++) this->add(other.get(e,&succ),&succ);
}
return *this;
}
RRForwardList RRForwardList::operator+(const RRForwardList &other)
{
RRForwardList k;
k=(*this);
k+=(other);
return k;
}
void RRForwardList::operator+=(const RRForwardList &other)
{
int succ;
for(int e=0;e<other.size;e++) this->add(other.get(e,&succ),&succ);
}

// methods RRForwardList
void RRForwardList::add(int data, bool *success)
{
if(success) *success=false;
RRNode *t;
t=new RRNode;
if(t==NULL) return;
t->data=data;
if(this->start==NULL)
{
this->start=this->end=t;
}
else
{
this->end->next=t;
this->end=t;
}
this->size++;
if(success) *success=true;
}
void RRForwardList::insertAt(int index, int data, bool *success)
{
if(success) *success=false;
if(index<0 || index>this->size) return;
if(index==this->size)
{
this->add(data,success);
return;
}
RRNode *t;
t=new RRNode;
if(t==NULL) return;
t->data=data;
RRNode *p1,*p2;
p1=this->start;
for(int e=0;e<index;e++,p2=p1,p1=p1->next);
if(p1==this->start)
{
t->next=this->start;
this->start=t;
}
else
{
p2->next=t;
t->next=p1;
}
this->size++;
if(success) *success=true;
}
void RRForwardList::update(int index, int data, bool * success)
{
if(success) *success=false;
if(index<0 || index>=this->size) return;
RRNode *p1;
p1=this->start;
for(int x=0;x<index;x++,p1=p1->next);
p1->data=data;
if(success) *success=true;
}
int RRForwardList::removeAt(int index, bool *success)
{
if(success) *success=false;
if(index<0 || index>=this->size) return 0;
RRNode *p1,*p2;
p1=this->start;
for(int x=0;x<index;x++,p2=p1,p1=p1->next);
int data=p1->data;
if(this->start==this->end) // only one node case
{
this->start=this->end=NULL;
}
else if(p1==this->start) // many nodes but remove the 1st one
{
this->start=this->start->next;
}
else if(p1==this->end) // many nodes but remove the last one
{
p2->next=NULL;
this->end=p2;
}
else
{
p2->next=p1->next;
}
delete p1;
this->size--;
if(success) *success=true;
return data;
}
int RRForwardList::get(int index, bool *success) const
{
if(success) *success=false;
if(index<0 || index>=this->size) return 0;
RRNode *p1=this->start;
for(int x=0;x<index;x++,p1=p1->next);
int data=p1->data;
if(success) *success=true;
return data;
}
int RRForwardList::getSize() const
{
return this->size;
}
void RRForwardList::removeAll()
{
this->clear();
}
void RRForwardList::clear()
{
RRNode *t;
while(this->start!=NULL)
{
t=this->start;
this->start=this->start->next;
delete t;
}
this->size=0;
this->end=NULL;
}







// RRTwoWayList implementation starts here

class RRTwoWayList:public RRList
{
class RRNode
{
public:
int data;
RRNode *next;
RRNode *prev;
RRNode()
{
this->next=NULL;
this->prev=NULL;
}
};
private:
RRNode *start,*end;
int size;
char allocationFlag;

class RRTwoWayListIterator:public Iterator
{
private:
RRNode *ptr;
public:
RRTwoWayListIterator()
{
this->ptr=NULL;
}
RRTwoWayListIterator(RRNode *ptr)
{
this->ptr=ptr;
}
RRTwoWayListIterator & operator=(const RRTwoWayListIterator &other)
{
this->ptr=other.ptr;
return *this;
}
int hasMoreElements()
{
return this->ptr!=NULL;
}
int getNext()
{
if(this->ptr==NULL) return 0;
int data=this->ptr->data;
this->ptr=this->ptr->next;
return data;
}
};
public:
Iterator getIterator()
{
RRTwoWayListIterator *rrTwoWayListIterator;
rrTwoWayListIterator=new RRTwoWayListIterator(this->start);
Iterator k(rrTwoWayListIterator);
k.setReleaseIteratorAfterIteration(0);
return k;
}

// constructors
RRTwoWayList();
RRTwoWayList(int bufferSize);
RRTwoWayList(const RRList &other);
RRTwoWayList(const RRTwoWayList &other);
~RRTwoWayList();

// operators
RRTwoWayList & operator=(const RRList &other);
RRTwoWayList operator+(const RRList &other);
void operator+=(const RRList &other);
RRTwoWayList & operator=(const RRTwoWayList &other);
RRTwoWayList operator+(const RRTwoWayList &other);
void operator+=(const RRTwoWayList &other);

// methods
void add(int data, bool *success);
void insertAt(int index, int data, bool *success);
void update(int index, int data, bool * success);
int removeAt(int index, bool *success);
int get(int index, bool *success) const;
int getSize() const;
void removeAll();
void clear();
};

// constructors RRTwoWayList
RRTwoWayList::RRTwoWayList()
{
this->start=this->end=NULL;
this->size=0;
this->allocationFlag=0;
}
RRTwoWayList::RRTwoWayList(int bufferSize)
{
this->start=this->end=NULL;
this->size=0;
this->allocationFlag=0;
}
RRTwoWayList::RRTwoWayList(const RRList &other)
{
this->start=this->end=NULL;
this->size=0;
this->allocationFlag=0;
int succ;
for(int e=0;e<other.getSize();e++) this->add(other.get(e,&succ),&succ);
}
RRTwoWayList::RRTwoWayList(const RRTwoWayList &other)
{
this->start=this->end=NULL;
this->size=0;
this->allocationFlag=0;
int succ;
for(int e=0;e<other.getSize();e++) this->add(other.get(e,&succ),&succ);
}
RRTwoWayList::~RRTwoWayList()
{
if(allocationFlag==0) this->clear();
}

// operators RRTwoWayList
RRTwoWayList & RRTwoWayList::operator=(const RRList &other)
{
this->clear();
int succ;
for(int e=0;e<other.getSize();e++) this->add(other.get(e,&succ),&succ);
return *this;
}
RRTwoWayList RRTwoWayList::operator+(const RRList &other)
{
RRTwoWayList k;
k=(*this);
k+=(other);
return k;
}
void RRTwoWayList::operator+=(const RRList &other)
{
int succ;
for(int e=0;e<other.getSize();e++) this->add(other.get(e,&succ),&succ);
}
RRTwoWayList & RRTwoWayList::operator=(const RRTwoWayList &other)
{
this->clear();
if(other.allocationFlag==1)
{
this->start=other.start;
this->end=other.end;
this->size=other.size;
}
else
{
int succ;
for(int e=0;e<other.size;e++) this->add(other.get(e,&succ),&succ);
}
return *this;
}
RRTwoWayList RRTwoWayList::operator+(const RRTwoWayList &other)
{
RRTwoWayList k;
k=(*this);
k+=(other);
k.allocationFlag=1;
return k;
}
void RRTwoWayList::operator+=(const RRTwoWayList &other)
{
int succ;
for(int e=0;e<other.size;e++) this->add(other.get(e,&succ),&succ);
}

// methods RRTwoWayList
void RRTwoWayList::add(int data, bool *success)
{
if(success) *success=false;
RRNode *t;
t=new RRNode;
if(t==NULL) return;
t->data=data;
if(this->start==NULL)
{
this->start=this->end=t;
}
else
{
t->prev=this->end;
this->end->next=t;
this->end=t;
}
this->size++;
if(success) *success=true;
}
void RRTwoWayList::insertAt(int index, int data, bool *success)
{
if(success) *success=false;
if(index<0 || index>this->size) return;
if(index==this->size)
{
this->add(data,success);
return;
}
RRNode *t;
t=new RRNode;
if(t==NULL) return;
t->data=data;
RRNode *p1;
p1=this->start;
for(int e=0;e<index;e++,p1=p1->next);
if(p1==this->start)
{
t->next=this->start;
this->start->prev=t;
this->start=t;
}
else
{
p1->prev->next=t;
t->prev=p1->prev;
p1->prev=t;
t->next=p1;
}
this->size++;
if(success) *success=true;
}
void RRTwoWayList::update(int index, int data, bool * success)
{
if(success) *success=false;
if(index<0 || index>=this->size) return;
RRNode *p1;
p1=this->start;
for(int x=0;x<index;x++,p1=p1->next);
p1->data=data;
if(success) *success=true;
}
int RRTwoWayList::removeAt(int index, bool *success)
{
if(success) *success=false;
if(index<0 || index>=this->size) return 0;
RRNode *p1;
p1=this->start;
for(int x=0;x<index;x++,p1=p1->next);
int data=p1->data;
if(this->start==this->end) // only one node case
{
this->start=this->end=NULL;
}
else if(p1==this->start) // many nodes but remove the 1st one
{
this->start=this->start->next;
p1->next->prev=NULL;
}
else if(p1==this->end) // many nodes but remove the last one
{
p1->prev->next=NULL;
this->end=p1->prev;
}
else
{
p1->prev->next=p1->next;
p1->next->prev=p1->prev;
}
delete p1;
this->size--;
if(success) *success=true;
return data;
}
int RRTwoWayList::get(int index, bool *success) const
{
if(success) *success=false;
if(index<0 || index>=this->size) return 0;
RRNode *p1=this->start;
for(int x=0;x<index;x++,p1=p1->next);
int data=p1->data;
if(success) *success=true;
return data;
}
int RRTwoWayList::getSize() const
{
return this->size;
}
void RRTwoWayList::removeAll()
{
this->clear();
}
void RRTwoWayList::clear()
{
RRNode *t;
while(this->start!=NULL)
{
t=this->start;
this->start=this->start->next;
delete t;
}
this->size=0;
this->end=NULL;
}

int RRArrayListmain()
{
RRArrayList list1;
int k;
for(int e=30;e<50;e++) list1.add(e,&k);

cout<<"Size : "<<list1.getSize()<<endl;
for(int e=0;e<list1.getSize();e++) cout<<list1.get(e,&k)<<endl;
/*
list1.removeAt(0,&k);
cout<<"after removing on index 0"<<endl;
cout<<"Size : "<<list1.getSize()<<endl;
for(int e=0;e<list1.getSize();e++) cout<<list1.get(e,&k)<<endl;

list1.removeAt(5,&k);
cout<<"after removing on index 5"<<endl;
cout<<"Size : "<<list1.getSize()<<endl;
for(int e=0;e<list1.getSize();e++) cout<<list1.get(e,&k)<<endl;

list1.removeAt(17,&k);
cout<<"after removing on index 17"<<endl;
cout<<"Size : "<<list1.getSize()<<endl;
for(int e=0;e<list1.getSize();e++) cout<<list1.get(e,&k)<<endl;

list1.removeAt(100,&k);
cout<<"after removing on index 100"<<endl;
cout<<"Size : "<<list1.getSize()<<endl;
for(int e=0;e<list1.getSize();e++) cout<<list1.get(e,&k)<<endl;


list1.insertAt(0,11111,&k);
cout<<"after inserting on index 0"<<endl;
cout<<"Size : "<<list1.getSize()<<endl;
for(int e=0;e<list1.getSize();e++) cout<<list1.get(e,&k)<<endl;

list1.insertAt(5,22222,&k);
cout<<"after inserting on index 5"<<endl;
cout<<"Size : "<<list1.getSize()<<endl;
for(int e=0;e<list1.getSize();e++) cout<<list1.get(e,&k)<<endl;

list1.insertAt(22,3333,&k);
cout<<"after inserting on index 22"<<endl;
cout<<"Size : "<<list1.getSize()<<endl;
for(int e=0;e<list1.getSize();e++) cout<<list1.get(e,&k)<<endl;

list1.insertAt(100,44444,&k);
cout<<"after inserting on index 100"<<endl;
cout<<"Size : "<<list1.getSize()<<endl;
for(int e=0;e<list1.getSize();e++) cout<<list1.get(e,&k)<<endl;
*/

list1.update(0,11111,&k);
cout<<"after updating on index 0"<<endl;
cout<<"Size : "<<list1.getSize()<<endl;
for(int e=0;e<list1.getSize();e++) cout<<list1.get(e,&k)<<endl;

list1.update(5,22222,&k);
cout<<"after updating on index 5"<<endl;
cout<<"Size : "<<list1.getSize()<<endl;
for(int e=0;e<list1.getSize();e++) cout<<list1.get(e,&k)<<endl;

list1.update(19,3333,&k);
cout<<"after updating on index 19"<<endl;
cout<<"Size : "<<list1.getSize()<<endl;
for(int e=0;e<list1.getSize();e++) cout<<list1.get(e,&k)<<endl;

list1.update(100,44444,&k);
cout<<"after updating on index 100"<<endl;
cout<<"Size : "<<list1.getSize()<<endl;
for(int e=0;e<list1.getSize();e++) cout<<list1.get(e,&k)<<endl;


return 0;
}

int RRForwardListmain()
{
RRForwardList list1;
int k;
list1.add(100,&k);
list1.add(4973,&k);
list1.add(48002,&k);
list1.add(3898,&k);
list1.add(48200,&k);
list1.add(3082,&k);
list1.add(3823,&k);

Iterator iterator=list1.getIterator();
while(iterator.hasMoreElements())
{
cout<<iterator.getNext()<<endl;
}


RRArrayList list2;
list2.add(100,&k);
list2.add(4973,&k);
list2.add(48002,&k);
list2.add(3898,&k);
list2.add(48200,&k);
list2.add(3082,&k);
list2.add(3823,&k);
cout<<"Elements of list 2 ArrayList "<<endl;
iterator=list2.getIterator();
while(iterator.hasMoreElements())
{
cout<<iterator.getNext()<<endl;
}

return 0;
}



int main()
{
RRTwoWayList list1;
int k;
list1.add(84239,&k);
list1.add(85449,&k);
list1.add(239,&k);
list1.add(559,&k);
list1.add(29,&k);
list1.add(629,&k);
list1.add(8439,&k);
list1.add(329,&k);
list1.add(4,&k);
list1.add(439,&k);
list1.add(339,&k);
cout<<"Elements of list 1 "<<endl;
Iterator iterator=list1.getIterator();
while(iterator.hasMoreElements())
{
cout<<iterator.getNext()<<endl;
}

RRForwardList list2(list1);
list2.add(22222,&k);
list1.add(84239,&k);
cout<<"Elements of list 2 "<<endl;
iterator=list2.getIterator();
while(iterator.hasMoreElements())
{
cout<<iterator.getNext()<<endl;
}

RRArrayList list3;
list3=list2+list1;
cout<<"Elements of list 3 "<<endl;
iterator=list3.getIterator();
while(iterator.hasMoreElements())
{
cout<<iterator.getNext()<<endl;
}


return 0;
}