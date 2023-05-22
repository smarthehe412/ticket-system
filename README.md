# ticket-system

## BPT：OK

（有缓存）（value在外存）（空间回收计划中）

## 用户权限部分：OK

## 车票部分

### 火车本体

class train，包括 trainID, stationNum, stations, seatNum, prices, startTime, travelTimes, stopoverTimes, saleDate, type

released 与 unreleased 分别存储，用 BPT(trainID,train) trains 管理

### query_ticket

用 multiBPT(station,trainID) 管理，路过则记录。

查询时，按 -s 与 -t 找到两个