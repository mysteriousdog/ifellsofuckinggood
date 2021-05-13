# ifellsofuckinggood

实现 server收发消息 支持多客户端链接、注册、登录、消息传递交互功能。
结构：
1:Actor 游戏世界中的主角 玩家控制的任务
2:socket CLient 客户端 负责消息收发
3:Command 游戏世界中的控制命令 是actor参数化的命令
4:Game 游戏世界 负责运行、更新游戏世界obj 每秒60帧
5:HandleMsg 负责做消息接受的后处理 客户端与服务器端
6:mysql_pool 实现mysql的链接池子 目前设置有上限链接
7:Player 客户端玩家
8:redis_pool 实现redis的链接池子 目前无上限连结数
SeqAbleObj 可在网络中传输的结构体定义
ThreadPool 实现线程池 
Zpool 实现server socket多链接
server 实现服务器端逻辑 负责初始化 和运行sever逻辑
