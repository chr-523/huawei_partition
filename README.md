<<<<<<< HEAD
# huawei_4
huawei_4
=======
2024.05.22


目前思路：

1. 图

   1. ```
      写了部分数据结构, 每个数据结构的复制没写
      class 图 = V+ E 多加一个 MODULE = V
      
      E=e+A
      
      limbo -> 
      	pin,assigmane,wire,(module) -> edge
      	instance ->( gate ->) vertex -> module -> graph
      							->graph
      	gate: input output is_clk index name weight
      	edge ? 两个点+weight
      	in/output graph ?
      
      
      ```

   2. 应该一个module一个class graph，嵌套

2. 最大化 partition（全都合法地分开）

3. 聚类？comb应该有不少可以聚类的部分

4. 合并partition

5. xxx

6. xxx

问题：

1. 如何聚类：只能以register为单位横向聚合

   1. 若timingpath AB，存在A起点指向B起点，不能合并（ ？）
   2. 若timingpath AB，存在A起点等于B终点，不能合并

2. 还是数据结构问题


#### Fast那篇 ####


3. DAG

   ```
   vertex -> pin		edge -> pin2pin
   edge_weight -> 共享逻辑大小
   source_point -> path起始点 (PI)
   end_point -> path上的结束点 (only datapath)	(触发器输入/PO)
   ```


   ```
   //数据结构:(CSR)
   	1.vertex_offsets_array：E
   	2.edge_destination：A
   	3.edge_weight:ed -> w -> 用离PI的距离深度表征
   	--------------------
   	fan-in + fan-out graph
   	--------------------
   endpoint Graph生成是遍历logic cone的方式Breadth First Search, BFS生成
   	think：	这里只有"一层"节点, 每个register同时标记为source和end,遍历到了end改成source
   				
   重复logic cone则用一个边指向搜索过的点
   ```


   ```
   logic_cone_13 -> [9,5,1,2]	logic_cone_14 -> [10,5->13,6,7,3]
   logic_cone_15 -> [10->14,11]	logic_cone_16 -> [3->15,12,8,4]
   ```

   

#### High-Quality Hypergraph Partitioning ####

- ```
  //数据结构：超图 -> V+E
  Vector:		[0][0,1][0][0][1][1]
  Edge:	E+A	
  		E:	[0,↓      4][4,↓   3] (else+)[6,0]
  		A:	[0, 1, 2, 3, 1, 4, 5]
  |-e0-\     /-e1-|		0--\       /--4
  | 0	   \ /    4 |		2---+--1--+---5
  | 2    |1|      |		3--/      
  |_3____/ \____5_|		
  ```

  ![image-20240518202524700](C:\Users\dell\AppData\Roaming\Typora\typora-user-images\image-20240518202524700.png)

  ![image-20240509170153203](C:\Users\dell\AppData\Roaming\Typora\typora-user-images\image-20240509170153203.png)

  ```
  vector 顺序 9 10 5 6 1 2
  vector 	[1][2][1,2,3,4][2,4][3][3,4]	对应点 连接的超边标签
  E+A	E:	[0,2][2,3][5,3][8,3](else+)[11,0] 偏移
  	A:	[9,5,10,5,6,5,1,2,5,6,2]	对应边连接的点列表
  think：	如何将重合度 -> "逻辑大小" edge_weight
  ```

#### Local Graph Edge Partitioning with a Two-Stage Heuristic Method ####

- 选择某一节点，然后开始合并

- two_stage ：

  1. 优先度较大的点进行合并
  2. 更靠近的点进行合并

  $$
  \mu_{s1}(v_i)=\max_{v_j\in N(v_i) \cap P_k}(\frac{\abs{N(v_i)\cup N(v_j)}}{\abs {N(v_j)}})\\
  \mu_{s2}(v_i)=1-\frac1{1+\Delta M}~~~~~~~\Delta M=M'(P_k)-M(P_k)\\
  v = \arg\max_{v_i\in N(P_k)}\mu_{s12}(v_i)
  $$

  3. 对于s1，度数越高分子越大，越倾向于先择该v，对s2划分更紧凑

- modulatity和p负相关

  - 也许划分stage12的节点可以根据P选择

- 其他大差不差

#### 赛题中

![image-20240513151308458](C:\Users\dell\AppData\Roaming\Typora\typora-user-images\image-20240513151308458.png)

与Two_Stage那一篇内部 $RF-1$ 定义相同
$$
{RF}=\frac{\sum^p_{k=1}\abs{V(P_k)}}{\abs{V}}
$$

>>>>>>> a7d180b (main)
