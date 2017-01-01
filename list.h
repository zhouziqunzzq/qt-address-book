#ifndef LIST_H
#define LIST_H

#include "listNode.h"
template <typename T> class List { //列表模板类

private:
   int _size; ListNodePosi(T) header; ListNodePosi(T) trailer; //规模、头哨兵、尾哨兵

protected:
   void init(); //列表创建时的初始化
   void copyNodes ( ListNodePosi(T), int ); //复制列表中自位置p起的n项
   void merge ( ListNodePosi(T)&, int, List<T>&, ListNodePosi(T), int ); //归并
   void mergeSort ( ListNodePosi(T)&, int ); //对从p开始连续的n个节点归并排序
   void selectionSort ( ListNodePosi(T), int ); //对从p开始连续的n个节点选择排序
   void insertionSort ( ListNodePosi(T), int ); //对从p开始连续的n个节点插入排序

public:
// 构造函数
   List() { init(); } //默认
   List ( List<T> const& L ); //整体复制列表L
   List ( List<T> const& L, Rank r, int n ); //复制列表L中自第r项起的n项
   List ( ListNodePosi(T) p, int n ); //复制列表中自位置p起的n项
// 析构函数
   ~List(); //释放（包含头、尾哨兵在内的）所有节点
// 只读访问接口
   Rank size() const { return _size; } //规模
   bool empty() const { return _size <= 0; } //判空
   T& operator[] ( Rank r ) const; //重载，支持循秩访问（效率低）
   ListNodePosi(T) first() const { return header->succ; } //首节点位置
   ListNodePosi(T) last() const { return trailer->pred; } //末节点位置
   bool valid ( ListNodePosi(T) p ) //判断位置p是否对外合法
   { return p && ( trailer != p ) && ( header != p ); } //将头、尾节点等同于NULL
   int disordered() const; //判断列表是否已排序
   ListNodePosi(T) find ( T const& e ) const //无序列表查找
   { return find ( e, _size, trailer ); }
   ListNodePosi(T) find ( T const& e, int n, ListNodePosi(T) p ) const; //无序区间查找
   ListNodePosi(T) search ( T const& e ) const //有序列表查找
   { return search ( e, _size, trailer ); }
   ListNodePosi(T) search ( T const& e, int n, ListNodePosi(T) p ) const; //有序区间查找
   ListNodePosi(T) selectMax ( ListNodePosi(T) p, int n ); //在p及其n-1个后继中选出最大者
   ListNodePosi(T) selectMax() { return selectMax ( header->succ, _size ); } //整体最大者
// 可写访问接口
   ListNodePosi(T) insertAsFirst ( T const& e ); //将e当作首节点插入
   ListNodePosi(T) insertAsLast ( T const& e ); //将e当作末节点插入
   ListNodePosi(T) insertA ( ListNodePosi(T) p, T const& e ); //将e当作p的后继插入
   ListNodePosi(T) insertB ( ListNodePosi(T) p, T const& e ); //将e当作p的前驱插入
   T remove ( ListNodePosi(T) p ); //删除合法位置p处的节点,返回被删除节点
   void merge ( List<T>& L ) { merge ( first(), size, L, L.first(), L._size ); } //全列表归并
   void sort() { sort ( first(), _size ); } //列表整体排序
   void sort ( ListNodePosi(T) p, int n ); //列表区间排序
   int deduplicate(); //无序去重
   int uniquify(); //有序去重
   void reverse(); //前后倒置
   int clear(); //清除所有节点
// 遍历
   void traverse ( void (* ) ( T& ) ); //遍历，依次实施visit操作（函数指针，只读或局部性修改）
   template <typename VST> //操作器
   void traverse ( VST& ); //遍历，依次实施visit操作（函数对象，可全局性修改）
}; //List

template <typename T> //重载下标操作符，以通过秩直接访问列表节点（虽方便，效率低，需慎用）
T& List<T>::operator[] ( Rank r ) const { //assert: 0 <= r < size
   ListNodePosi(T) p = first(); //从首节点出发
   while ( 0 < r-- ) p = p->succ; //顺数第r个节点即是
   return p->data; //目标节点，返回其中所存元素
}

template <typename T> void List<T>::init() { //列表初始化，在创建列表对象时统一调用
   header = new ListNode<T>; //创建头哨兵节点
   trailer = new ListNode<T>; //创建尾哨兵节点
   header->succ = trailer; header->pred = NULL;
   trailer->pred = header; trailer->succ = NULL;
   _size = 0; //记录规模
}

template <typename T> //列表内部方法：复制列表中自位置p起的n项
void List<T>::copyNodes ( ListNodePosi(T) p, int n ) { //p合法，且至少有n-1个真后继节点
   init(); //创建头、尾哨兵节点并做初始化
   while ( n-- ) { insertAsLast ( p->data ); p = p->succ; } //将起自p的n项依次作为末节点插入
}

template <typename T> //复制列表中自位置p起的n项（assert: p为合法位置，且至少有n-1个后继节点）
List<T>::List ( ListNodePosi(T) p, int n ) { copyNodes ( p, n ); }

template <typename T> //整体复制列表L
List<T>::List ( List<T> const& L ) { copyNodes ( L.first(), L._size ); }

template <typename T> //复制L中自第r项起的n项（assert: r+n <= L._size）
List<T>::List ( List<T> const& L, int r, int n ) { copyNodes ( L[r], n ); }

template <typename T> List<T>::~List() //列表析构器
{ clear(); delete header; delete trailer; } //清空列表，释放头、尾哨兵节点

template <typename T> int List<T>::disordered() const { //统计逆序相邻元素对的总数
   int n = 0; ListNode<T>* p = first();
   for ( int i = 0; i < _size - 1; p = p->succ, i++ )
      if ( p->data > p->succ->data ) n++;
   return n;
}

template <typename T> //在无序列表内节点p（可能是trailer）的n个（真）前驱中，找到等于e的最后者
ListNodePosi(T) List<T>::find ( T const& e, int n, ListNodePosi(T) p ) const {
   while ( 0 < n-- ) //（0 <= n <= rank(p) < _size）对于p的最近的n个前驱，从右向左
      if ( e == ( p = p->pred )->data ) return p; //逐个比对，直至命中或范围越界
   return NULL; //p越出左边界意味着区间内不含e，查找失败
} //失败时，返回NULL

template <typename T> //在有序列表内节点p（可能是trailer）的n个（真）前驱中，找到不大于e的最后者
ListNodePosi(T) List<T>::search ( T const& e, int n, ListNodePosi(T) p ) const {
// assert: 0 <= n <= rank(p) < _size
   while ( 0 <= n-- ) //对于p的最近的n个前驱，从右向左逐个比较
      if ( ( ( p = p->pred )->data ) <= e ) break; //直至命中、数值越界或范围越界
// assert: 至此位置p必符合输出语义约定——尽管此前最后一次关键码比较可能没有意义（等效于与-inf比较）
   return p; //返回查找终止的位置
} //失败时，返回区间左边界的前驱（可能是header）——调用者可通过valid()判断成功与否

template <typename T> ListNodePosi(T) List<T>::insertAsFirst ( T const& e )
{  _size++; return header->insertAsSucc ( e );  } //e当作首节点插入

template <typename T> ListNodePosi(T) List<T>::insertAsLast ( T const& e )
{  _size++; return trailer->insertAsPred ( e );  } //e当作末节点插入

template <typename T> ListNodePosi(T) List<T>::insertA ( ListNodePosi(T) p, T const& e )
{  _size++; return p->insertAsSucc ( e );  } //e当作p的后继插入（After）

template <typename T> ListNodePosi(T) List<T>::insertB ( ListNodePosi(T) p, T const& e )
{  _size++; return p->insertAsPred ( e );  } //e当作p的前驱插入（Before）

template <typename T> T List<T>::remove ( ListNodePosi(T) p ) { //删除合法节点p，返回其数值
   T e = p->data; //备份待删除节点的数值（假定T类型可直接赋值）
   p->pred->succ = p->succ; p->succ->pred = p->pred; //后继、前驱
   delete p; _size--; //释放节点，更新规模
   return e; //返回备份的数值
}

template <typename T> int List<T>::clear() { //清空列表
   int oldSize = _size;
   while ( 0 < _size ) remove ( header->succ ); //反复删除首节点，直至列表变空
   return oldSize;
}

template <typename T> void List<T>::traverse ( void ( *visit ) ( T& ) ) //借助函数指针机制遍历
{  for ( ListNodePosi(T) p = header->succ; p != trailer; p = p->succ ) visit ( p->data );  }

template <typename T> template <typename VST> //元素类型、操作器
void List<T>::traverse ( VST& visit ) //借助函数对象机制遍历
{  for ( ListNodePosi(T) p = header->succ; p != trailer; p = p->succ ) visit ( p->data );  }

template <typename T>
void List<T>::sort ( ListNodePosi(T) p, int n )  //决定内部排序调用的方法
{
    mergeSort(p, n);
}

template <typename T> //列表的插入排序算法：对起始于位置p的n个元素排序
void List<T>::insertionSort ( ListNodePosi(T) p, int n ) { //valid(p) && rank(p) + n <= size
   for ( int r = 0; r < n; r++ ) { //逐一为各节点
      insertA ( search ( p->data, r, p ), p->data ); //查找适当的位置并插入
      p = p->succ; remove ( p->pred ); //转向下一节点
   }
}

template <typename T> //从起始于位置p的n个元素中选出最大者
ListNodePosi(T) List<T>::selectMax ( ListNodePosi(T) p, int n ) {
   ListNodePosi(T) max = p; //最大者暂定为首节点p
   for ( ListNodePosi(T) cur = p; 1 < n; n-- ) //从首节点p出发，将后续节点逐一与max比较
      if ( !lt ( ( cur = cur->succ )->data, max->data ) ) //若当前元素不小于max，则
         max = cur; //更新最大元素位置记录
   return max; //返回最大节点位置
}

template <typename T> //列表的选择排序算法：对起始于位置p的n个元素排序
void List<T>::selectionSort ( ListNodePosi(T) p, int n ) { //valid(p) && rank(p) + n <= size
   ListNodePosi(T) head = p->pred; ListNodePosi(T) tail = p;
   for ( int i = 0; i < n; i++ ) tail = tail->succ; //待排序区间为(head, tail)
   while ( 1 < n ) { //在至少还剩两个节点之前，在待排序区间内
      ListNodePosi(T) max = selectMax ( head->succ, n ); //找出最大者（歧义时后者优先）
      insertB ( tail, remove ( max ) ); //将其移至无序区间末尾（作为有序区间新的首元素）
      tail = tail->pred; n--;
   }
}

template <typename T> //有序列表的归并：当前列表中自p起的n个元素，与列表L中自q起的m个元素归并
void List<T>::merge ( ListNodePosi(T) & p, int n, List<T>& L, ListNodePosi(T) q, int m ) {
// assert:  this.valid(p) && rank(p) + n <= size && this.sorted(p, n)
//          L.valid(q) && rank(q) + m <= L._size && L.sorted(q, m)
// 注意：在归并排序之类的场合，有可能 this == L && rank(p) + n = rank(q)
   ListNodePosi(T) pp = p->pred; //借助前驱（可能是header），以便返回前 ...
   while ( 0 < m ) //在q尚未移出区间之前
      if ( ( 0 < n ) && ( p->data <= q->data ) ) //若p仍在区间内且v(p) <= v(q)，则
         { if ( q == ( p = p->succ ) ) break; n--; } //p归入合并的列表，并替换为其直接后继
      else //若p已超出右界或v(q) < v(p)，则
         { insertB ( p, L.remove ( ( q = q->succ )->pred ) ); m--; } //将q转移至p之前
   p = pp->succ; //确定归并后区间的（新）起点
}

template <typename T> //列表的归并排序算法：对起始于位置p的n个元素排序
void List<T>::mergeSort ( ListNodePosi(T) & p, int n ) { //valid(p) && rank(p) + n <= size
   if ( n < 2 ) return; //若待排序范围已足够小，则直接返回；否则...
   int m = n >> 1; //以中点为界
   ListNodePosi(T) q = p; for ( int i = 0; i < m; i++ ) q = q->succ; //均分列表
   mergeSort ( p, m ); mergeSort ( q, n - m ); //对前、后子列表分别排序
   merge ( p, m, *this, q, n - m ); //归并
} //注意：排序后，p依然指向归并后区间的（新）起点

template <typename T> int List<T>::deduplicate() { //剔除无序列表中的重复节点
   if ( _size < 2 ) return 0; //平凡列表自然无重复
   int oldSize = _size; //记录原规模
   ListNodePosi(T) p = header; Rank r = 0; //p从首节点开始
   while ( trailer != ( p = p->succ ) ) { //依次直到末节点
      ListNodePosi(T) q = find ( p->data, r, p ); //在p的r个（真）前驱中查找雷同者
      q ? remove ( q ) : r++; //若的确存在，则删除之；否则秩加一
   } //assert: 循环过程中的任意时刻，p的所有前驱互不相同
   return oldSize - _size; //列表规模变化量，即被删除元素总数
}

template <typename T> int List<T>::uniquify() { //成批剔除有序列表重复元素，效率更高
   if ( _size < 2 ) return 0; //平凡列表自然无重复
   int oldSize = _size; //记录原规模
   ListNodePosi(T) p = first(); ListNodePosi(T) q; //p为各区段起点，q为其后继
   while ( trailer != ( q = p->succ ) ) //反复考查紧邻的节点对(p, q)
      if ( p->data != q->data ) p = q; //若互异，则转向下一区段
      else remove ( q ); //否则（雷同），删除后者
   return oldSize - _size; //列表规模变化量，即被删除元素总数
}

template <typename T> void List<T>::reverse() { //前后倒置
   ListNodePosi(T) p = header; ListNodePosi(T) q = trailer; //头、尾节点
   for ( int i = 1; i < _size; i += 2 ) //（从首、末节点开始）由外而内，捉对地
      swap ( ( p = p->succ )->data, ( q = q->pred )->data ); //交换对称节点的数据项
}

#endif // LIST_H
