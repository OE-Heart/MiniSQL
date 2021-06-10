[TOC]

`BufferManager` 模块包括两个组件：

- Block
- BufferManager

# Block

该文件是`Page`的抽象，每个`Block`的大小为`4096kb`，具体结构如下：

```c++
class Block {
public:
    char data[BLOCK_SIZE];  // Store data
    bool isPinned;          // Is pinned?
    bool isDirty;           // Is dirty?
    std::string filename;
    int offset;             // Offset is similar to file pointer in files
    int refcnt;             // Number of reference
    
    Block();
};
```

- `data` 用于存储具体的数据
- `isPinned` 表示该块是否被Pin
- `isDirty` 表示该块是否是脏块，如果是，在释放的时候会被写入disk中
- `filename` 存储的就是其对应的文件名
- `offset` 是其在文件内部的偏移量，以 `4096` 为一个单位
- `refcnt` 是其引用数



该文件更类似于一个Struct，是为BufferManager服务的，不再做过多介绍。



# BufferManager

`BufferManager`是具体提供给外部使用的，其定义如下：

```c++
class BufferManager{
private:
    Block *blocks;
    BID getblk(std::string&, int);
    void Bpanic(const char *)const;
public:
    BufferManager();
    ~BufferManager();
    void pinBlock(BID bid);
    void dirtBlock(BID bid);
    void unpinBlock(BID bid);
    void undirtBlock(BID bid);
    BID  bread(std::string &, int);
    bool bwrite(BID bid);
    void brelease(BID bid);
    void*baddr(BID bid);
};
```

- `PinBlock` 即Pin一个块
- `dirtyBlock` 即使一个块Dirty
- `unpinBlock undirtyBlock` 顾名思义的作用！ 



下面具体介绍`4`个外界使用的函数

### bread

该函数会读取文件的内容并存储到内存里的`Block`中，其接受两个参数：

- `string &str` - 要读取的文件名称，后缀默认为`.data`，不需要自己加上
- `int off` - 文件内部的偏移量，以4096为一个单位

返回值是 `BID`，即块描述符(仿文件描述符fd)

**Example:**

```c++
string filename = "Test"
BID bid = bread(filename, 0);// 读取 Test.data 的第一个块
BID bid = bread(filename, 4);// 读取 Test.data 的第5个块
```

另一个值得注意的是，如果文件不存在，会自动创建该文件并打开，在使用时其实可以不考虑这一点。



### baddr

bread读取块内容到Block中后，需要通过baddr获取其内部的buffer地址，如此之后才可以往buffer内写入或读取，其接受一个参数：

- `BID bid` - 即指定的块描述符

返回值是`buffer`的地址

**Example:**

```c++
char *buffer = baddr(bid);
strcpy(buffer, "Test string");
// 这样就把字符串写到buffer里了
```



### bwrite

该函数将指定的`Block`写回到Disk上，其接受一个参数：

- `BID bid` - 即块描述符

返回值是 `bool`，成功则返回true，否则返回false

**Example:**

```c++
char *buffer = baddr(bid);
strcpy(buffer, "Test string");
if(!bwrite(bid))
  printf("Error!\n");
// 这样就写入到disk上了
```



### brelease

该函数释放一个Block，其接受一个参数：

- `BID bid` - 即块描述符

当一个Block不再使用时，应该使用brelease将其释放掉



### Example

一个整体使用的样例如下：

```c++
BufferManager Buffer; // 创建Buffer
string str = "DBSTest";

BID bid = Buffer.bread(str, 1); // 读取第2个块
char* data = Buffer.baddr(bid); // 获取Block内部的Buffer缓冲区
strcpy(data, "This is a teststring!\n"); // 写入到Buffer区
Buffer.bwrite(bid);	  // 写入到disk
Buffer.brelease(bid); // 释放使用完的Block

bid = Buffer.bread(str, 1); // 重新读取块
data = Buffer.baddr(bid);   // 获取Block内部的Buffer区
printf("%s", data);				 // 输出Buffer的内容
Buffer.brelease(bid);
```

