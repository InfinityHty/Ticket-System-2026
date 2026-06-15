# 火车票管理系统

SJTU CS1951 课程大作业

## 项目概况

25级ACM班大一下学期数据结构课程的大作业`Ticket System`，作者`InfinityHty`

本项目基于C++开发编写，模拟实现了一个简易的火车票系统，其功能模块覆盖用户信息管理查询，火车信息管理查询，车票换乘信息管理查询，购票补票等。

## 功能概要
### 用户管理
- `add_user` — 注册用户（首个用户自动赋予权限 10）
- `login / logout` — 登录/登出
- `query_profile` — 查询用户信息
- `modify_profile` — 修改用户信息

### 车次管理
- `add_train / delete_train` — 添加/删除车次
- `release_train` — 发布车次（发布后方可售票）
- `query_train` — 查询车次详情

### 车票查询与购买
- `query_ticket` — 按时间/价格排序查询车票
- `query_transfer` — 查询最优一次换乘方案
- `buy_ticket` — 购票（支持候补队列）
- `query_order` — 查询订单历史
- `refund_ticket` — 退票（自动处理候补队列）

### 系统指令
- `clean` — 清空所有数据
- `exit` — 退出程序

## 数据库设计

- 基于火车票管理系统数据量极大的特性，项目采用BPT实现外存上高效的index到value的查询，大大减少了文件读写次数，并且支持重复index不同value。

- 同时由于单个用户/火车信息占用较大空间，数据库采用MemoryRiver设计，避免了大量信息的冗余重复存储。在BPT中value存储的是该数据块在memoryriver中的首地址。

- 同时基于大部分index是定长字符串这一性质，使用多项式Hash压缩成整数。

## 项目结构详述

### 项目结构示意图
```text
Ticket-System-2026/
├── cmake-build-debug/        
├── include/                  
│   ├── Account.h             # 用户账户类定义
│   ├── BPT_Database.h        # B+树底层数据库存储模块
│   ├── exception.h           
│   ├── Keys.h                # 索引键值相关定义
│   ├── MemoryRiver.h        
│   ├── Order.h               # 订单实体类
│   ├── Ticket.h              # 车票实体类
│   ├── TicketManagement.h    # 车票业务管理模块
│   ├── Tool_functions.h      # 通用工具函数
│   ├── Train.h               # 列车实体类
│   ├── TrainSystem.h         
│   ├── User.h                # 用户实体类
│   └── vector.h              # 自定义 vector 容器实现
├── src/                      
│   ├── bpt_storage_test.cpp  # B+树存储单元测试代码
│   ├── main.cpp              # 程序入口主函数
│   └── oj_submit.cpp         # 前置作业OJ提交代码
├── .gitignore               
├── bonus.md                  
├── CMakeLists.txt           
├── code                      # 可执行文件
├── README.md                
└── run-test                  # 测试脚本
```
### include

- Account.h:封装成一个类，包含所有和用户信息相关的操作，包括添加用户，用户登录，用户退出，用户信息查询，用户信息修改。同时包括用户购买车票信息的记录。

- BPT_Database.h 手写bpt数据库，相比前置作业添加了判断是否存在这个index，获取这个index对应的一个或多个value等功能
- User.h 用户个人信息封装
```
class User {
public:
    char username[21];
    char password[31];
    char name[21];
    char mailAddr[41];
    int privilege;
};
```
- Train.h火车信息封装，把剩余车票信息放在了这里，这使得单个train占据较大空间
```
class Train {
public:
    char trainID[21];
    int stationNum;
    int seatNum;
    char stations[30][41];
    int prices[99];
    Time startTime;
    int travelTimes[99];
    int stopoverTimes[98];
    Date saleDate[2];
    char type;
    bool release = false;
    int remainTickets[92][99];// 发车日期6.1~8.31 + 区间
};
```
- TrainSystem.h 封装了和火车信息相关的操作，包含添加列车，删除列车，发布列车，查询列车信息，也记录了与每一辆车关联的候补订单序列。
- Keys.h把一些常用的key，比如定长为20，40的char数组，两个定长字符串组成的pair（记录车票时用到）等封装起来。但其实可以全部采用Hash省掉这一步，但是Hash优化做的不太彻底，遂保留。
- TicketManagement.h 封装和车票相关的操作，包括release train时向里添加车票，根据起点终点查询车票，查询换乘（由于这个模拟比较简易此处只考虑恰好换乘一次的情形。车票一旦被加入就不会被删除。
- Ticket.h 封装一张票的信息
```
class Ticket {
public:
    char trainID[21];
    char start[41];
    char end[41];
    Train::Date date[2];
    Train::Time time[2];
    long long duration;
    long long price;
    int seat;
};
```
- Order.h 封装一个订单的信息，ticket被嵌入其中
```
class Order {
public:
    char username[21];
    int timestamp;
    enum Status{success,pending,refunded};
    Status status;
    Ticket ticket;
    int number;
};
```
- Tool_functions.h 一些必要的工具函数
- vector.h 手写了一个vector，功能基本和stl中的vector一致，支持迭代器等。
- exception.h 写vector时配的异常处理
- MemoryRiver.h 负责数据实际存储，写入时返回地址，根据地址可以查询读出数据

### src
- main.cpp项目入口，核心逻辑框架

## 特别鸣谢
在项目设计、开发、调试过程中给予我指导帮助的同学和助教们~
