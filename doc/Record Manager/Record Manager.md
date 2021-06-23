## RecordManager

### 功能描述

`Record Manager`最核⼼的功能在于直接执⾏对reocrds的操作。记录层⾯的操作在该模块内被实现，具体功能包括：

- `select`：实现对给定表、条件下对记录的查询；当选择条件中包括含索引属性时能够通过`Index Manager`对于查询过程进⾏优化。

- `delete`：与select类似，对给定表名和条件进⾏删除操作。

- `insert`：给定表名、属性值列表后将记录插⼊⾄Buffer Manager。



### 数据结构

```c++
using Value = std::variant<int, double, std::string>;

typedef std::pair<int, int> Piece;
typedef std::vector<Piece> PieceVec;
typedef std::vector<Value> ValueVec;

class RecordManager{
private:
    void        Rpanic(const char *);
    ValueVec    GetRecord(Table &, char *);
    void        PutRecord(Table &, const ValueVec &, char *);
    bool        CheckUnique(Table &, int, const Value &);
    bool        CheckType(Column &, const Value &);
    PieceVec    Intersect(PieceVec , PieceVec );
    PieceVec    SelectPos(Table &, const std::vector<Condition>);
    PieceVec    IndexSelect(Table &t, int ColumnID, const Condition &con);
    void        IndexUpdate(Table &t, const ValueVec &vals, int optype, int offset);
public:
    RecordManager();
    ~RecordManager();
    void    CreateTable(Table &t);
    void    DropTable(Table &t);
    Piece   InsertRecord(Table &t, const ValueVec &vals);
    void    DeleteRecord(Table &t, const std::vector<Condition> &con);
    void    DeleteAllRecord(Table &t);
    std::vector<ValueVec> SelectRecord(Table &t, const std::vector<Condition> &con);
    std::vector<ValueVec> SelectAllRecord(Table &t);
    IndexManager  *im;
};
```

- `Value`是Record中能使用的类型，包括`int double string`三种，以`variant`的形式存储和使用
- `Piece`是一个`pair<int, int>`类型，用以存储一个`record`的位置信息，其中`first`存储record所在的bid，`second`存储其所在bid的offset
- `PieceVec`和`ValueVec`则是`Piece`和`Value`的vector类型



### 接口定义

##### CreateTable

该函数实现文件层面的创建Table，其接受一个参数：

- `Table &t ` - 由更高层的函数将metadata写入到`t`中后再传入到此函数。

文件名即table名，是单文件存储的形式。



##### DropTable

该函数实现文件层面的删除Table，其接受一个参数：

- `Table &t` - 指定删除的Table



##### InsertRecord

该函数实现插入一条record到Buffer和Disk上，其接受两个参数：

- `Table &t` - 指定要插入record的Table
- `const ValueVec &vals` - record的各个attribute的值，以vector的形式传入

返回值 - `Piece`，返回插入的位置信息



##### DeleteRecord

该函数实现删除一条record，其接受两个参数：

- `Table &t` - 指定要删除record的Table
- `const std::vector<Condition> &con` - 要删除record的满足条件，以vector的形式传入，以便传入多个条件

Note - 条件`Condition`由`Catalog`管理



##### SelectRecord

该函数实现对record的选择，其接受两个参数：

- `Table &t` - 指定进行`select`的table
- `const std::vector<Condition> &con` - 要选择record的满足条件，以vector的形式传入，以便传入多个条件

返回值 - `std::vector<ValueVec>`，每一个record的所有属性值存储在`ValueVec`中，由于可能存在多个record返回，故使用`std::vector`进行返回



##### DeleteAllRecord

该函数实现删除表中所有的record，其接受一个参数：

- `Table &t` - 指定要删除record的Table



##### SelectAllRecord

该函数实现选择所有的record，其接受一个参数：

- `Table &t` - 指定要选择record的Table





### 类图



### 设计思路



### 关键代码

> 这里的关键代码主要是一些私有函数

首先是`GetReord`，其实现再Buffer中读取一条record，并以`ValueVec`的形式将其返回：

```c++
ValueVec RecordManager::GetRecord(Table &t, char *data){
    const std::vector<Column> &attr = t.columns;
    std::vector<Value> res;
    int size = attr.size();
    data++; // Skip the first sign byte
    for (int offset = 0, i = 0; i < size; i++){
        if (attr[i].field == Field::FLOAT){
            double val = *(double *)(data + offset);
            res.push_back(Value(val));
        }
        else if (attr[i].field == Field::INT){
            int val = *(int *)(data + offset);
            res.push_back(Value(val));
        }
        else{ // string
            std::string val = std::string((char *)(data + offset));
            res.push_back(Value(val));
        }
        offset += attr[i].size();
    }
    return res;
}
```

- 主体思路是通过Table中的存储的`metadata`得到record相关的属性类型，长度等信息，从而进一步读取信息



然后是`PutRecord`，作用是将record存到Buffer中：

```c++
void RecordManager::PutRecord(Table &t, const ValueVec& v, char *data){
    *data++ = 1; // set as taken
    const std::vector<Column> &attr = t.columns;
    int size = attr.size();
    for (int i = 0, offset = 0; i < size; i++){
        if (attr[i].field == Field::FLOAT)
        {
            double val = std::get<double>(v[i]);
            memcpy(data + offset, &val, attr[i].size());
        }
        else if (attr[i].field == Field::INT)
        {
            int val = std::get<int>(v[i]);
            memcpy(data + offset, &val, attr[i].size());
        }
        else // string
        {
            std::string val = std::get<std::string>(v[i]);
            memcpy(data + offset, val.c_str(), attr[i].size());
        }
        offset += attr[i].size();
    }
}
```

- 其思路和`GetRecord`是类似的，通过table中的metadata判断属性的类型并通过`memcpy`写入到buffer中



然后是Select和Delete都需要用到的函数`SelectPos`，其根据设置的条件读取相应record并返回其位置：

```c++
PieceVec RecordManager::SelectPos(Table &t, const std::vector<Condition> con)
{
    PieceVec v;
    bool flag = false;
    for (const Condition &c : con){
        int index = t.indexOfCol(c.columnName);
        if (t.columns[index].index != ""){	// use index to select
            if(c.op != OP::EQ) break; // Not supported
            if (!flag)
            {
                flag = true;
                v = IndexSelect(t, index, c);
            }
            else
            {
                v = Intersect(v, IndexSelect(t, t.indexOfCol(c.columnName), c));
            }
        }
    }
    if (!flag){ // Scan and search
        int blockcount = t.blockCnt;
        for (int i = 0; i < blockcount; i++){
            BID bid = bm->bread(t.tableName, i);
            char *data = bm->baddr(bid);
            int size = t.size()+1;
            for (int offset = 0; offset < BLOCK_SIZE && offset+size<=BLOCK_SIZE; offset += size){
                if(!data[offset]) continue; // Empty skip
                
                std::vector<Value> res = GetRecord(t, data + offset);
                bool good = true;
                for (auto const &c : con){	// Check each condition
                    int index = t.indexOfCol(c.columnName);
                    if (!c.isTrue(res[index])){
                        good = false;
                        break;
                    }
                }
                if (good)
                    v.push_back(std::make_pair(i, offset));
            }
            bm->brelease(bid);
        }
    }
    return v;
}
```

- 其思路是，对于有index的属性（即index成员变量不为空），则使用Index来select（即`IndexSelect`函数），否则遍历整个表，对每个record都进行条件判断，为真的则push到`v`中；最后返回`v`作为结果。