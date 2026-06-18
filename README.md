### 仓库结构

```include/```

- `baseclass.h`

- `bpt.h`

- `exceptions.h`

- `lexer.h`

- `map.h`

- `priority_queue.h`

- `ticket.h`

- `train.h`
- `user.h`
- `utility.h`
- `vector.h`

```src/```

- `baseclass.cpp`

- `lexer.cpp`
- `main.cpp`
- `ticket.cpp`
- `train.cpp`
- `user.cpp`

###  文件概述

`map.h`， `priority_queue.h`，`vector.h`，`utility.h` 实现了与 STL 相似的功能

`bpt.h` 实现了一个 B+ 树，用于在外存中存储数据

`baseclass.h`，`baseclass.cpp` 实现了一些基本的函数，时间与日期间的转换，以及日期的输出

`user.h`，`user.cpp` 实现了用户类和管理系统用户的基本功能（登录，创建，查询等）

`lexer.h`，`lexer.cpp` 实现了对输入指令的词元拆分，以及将字符串转为整数的功能

`train.h`，`train.cpp` 实现了火车类和车次的基本操作

`ticket.h`，`ticket.cpp` 实现了车票类和购票，查询，退票等基本功能

### 数据库概述

用 B+ 树存储信息。

`bpt<Usrname, User> nameToUser`：存储用户名到用户信息的映射

`bpt<TrainID, Train> idToTrain`：存储车次到火车信息的映射

`bpt<Station, TrainID> stationToID`：存储车站到车次的映射

`bpt<TrainKey, int> dailySeat`：存储车次和日期构成的二元组到一个动态下标的映射

`MemoryRiver<RemainSeat, 0> remainSeats`：存储上面的动态下标到座位数的映射

`bpt<int, TrainID> dateToTrain`：存储日期到车次的映射

`bpt<Usrname, int> userToTicket`：存储用户到车票的动态下标的映射

`bpt<TrainKey, int> trainToPendingTicket`：存储车次和日期构成的二元组到对应车票的动态下标的映射

`MemoryRiver<Ticket, 0> tickets` 存储车票动态下标到车票的映射

`MemoryRiver<TrainID, 0> ticketToTrain` 存储车票动态下标到车次的映射