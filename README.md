# ticket-system

## BPT

分为 BPT 与 multiBPT

缓存：有

省内存（value改为外存）：有

空间回收：无

multiBPT效率问题：已解决

## 用户权限部分

class user，包括 username, password, mailAddr, name, privilege

BPT(username,user) login, users 管理

## 车票部分

### 火车本体

class train，包括 trainID, stationNum, stations, seatNum, prices, startTime, travelTimes, stopoverTimes, saleDate, type

released 与 unreleased 分别存储，用 BPT(trainID,train) released, trains 管理

座位情况由 BPT((trainID,date),seat) seats 管理

### add_train 和 delete_train

对 trains 操作，对 seats 无影响

### release_train

对 released 操作

### query_train

需同时询问 trains 与 seats

### query_ticket

用 multiBPT(station,(trainID,saleDate,startTime,station_place)) stops 管理，路过则记录。

查询时，按 -s 与 -t 找到所有 trainID，排除不合法，双指针找合法，利用 trainID 在 released 查询 train 本体，储存所有信息，排序输出

### query_transfer

按 -s 与 -t 找到所有 train，遍历所有 -s，每次枚举中间站，hashmap 存信息，枚举 -t 的 train 查看有无重合站点。

### buy_ticket

seats 中查询，验证是否可购票

query 按询问时间排序，描述一次请求

候补队列由 multiBPT((trainID,date),query) pendings 管理（写 iterator 可提速）

### query_order

全部订单由 multiBPT(username,query) orders 管理

### refund_ticket

更改 seats

在 pendings 中查询，依次模拟是否可补，更改 seats, orders 并从 pendings 中删除

### clean

全部清除

### exit

只清除 login