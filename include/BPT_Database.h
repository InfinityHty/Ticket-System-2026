//
// Created by HTY on 2026/4/26.
//

#ifndef TICKET_SYSTEM_2026_BPT_DATABASE_H
#define TICKET_SYSTEM_2026_BPT_DATABASE_H
#include<fstream>
#include<iostream>
#include "vector.h"
template<class T1,class T2,int id_block_size = 40,int block_size = 40>
class Database {
private:
    std::fstream file;
    std::fstream index_file;
    std::string index_file_name,file_name;
    struct Content {
        T1 index;
        T2 value;
        Content() = default;
        Content(T1 idx,T2 val) : index(idx), value(val){}
    };
    // 所有数组都多开了一位
    struct IdNode {
        // 儿子最多id_block_size个
        Content list[id_block_size];// 索引节点中 直接把(key,value)一起作为索引 防止一个key对应一大堆value引起插入位置问题
        long long address[id_block_size + 1];// 儿子的地址(元素个数为list的个数+1)
        long long par = -1;// 父节点开头地址
        int num = 0;// 节点里的索引个数
        bool leaf = true;// 是不是叶节点
    };
    struct ContentNode {
        Content cont[block_size + 1];
        long long nex = -1;
        int body_size = 0;
    };
    // < 比较
    bool cmp(Content &a, Content &b) {
        if (a.index < b.index) return true;
        else if (a.index > b.index) return false;
        else return a.value < b.value;
    }

    long long root = 0;
    long long seq = 0;

    void SplitNode(IdNode &cur,Content new_id,int target,long long cur_add,long long split_add) {
        // 移动list 和 address
        for (int i = cur.num; i > target; i--) {
            cur.list[i] = cur.list[i - 1];
            cur.address[i + 1] = cur.address[i];
        }
        cur.list[target] = new_id;
        cur.address[target + 1] = split_add;
        ++cur.num;
        if (cur.num <= id_block_size - 1) {
            index_file.seekp(cur_add);
            index_file.write(reinterpret_cast<char *>(&cur),sizeof(IdNode));
            return;
        }
        else {
            // 裂开
            IdNode split;
            index_file.seekp(0,std::ios::end);
            long long first_pos = cur_add,second_pos = index_file.tellp();
            cur.num = id_block_size / 2;
            split.num = id_block_size - cur.num - 1;
            split.par = cur.par;
            split.leaf = cur.leaf;
            for (int i = 0; i < split.num; i++) {
                split.list[i] = cur.list[i + cur.num + 1];
            }
            for (int i = 0; i <= split.num; i++) {
                split.address[i] = cur.address[i + cur.num + 1];
                // 如果不是叶子节点 要修改孩子的par值!
                IdNode child;
                if (split.leaf == false) {
                    index_file.seekg(split.address[i]);
                    index_file.read(reinterpret_cast<char *>(&child),sizeof(IdNode));
                    child.par = second_pos;
                    index_file.seekp(split.address[i]);
                    index_file.write(reinterpret_cast<char *>(&child),sizeof(IdNode));
                }
            }
            index_file.seekp(second_pos);
            index_file.write(reinterpret_cast<char *>(&split),sizeof(IdNode));
            index_file.seekp(cur_add);
            index_file.write(reinterpret_cast<char *>(&cur),sizeof(IdNode));
            if (cur.par == -1) {
                // 已经递归到了根节点 创建新的根节点
                index_file.seekg(0,std::ios::end);
                root = index_file.tellg();
                cur.par = split.par = root;
                // 更新一下索引文件
                index_file.seekp(second_pos);
                index_file.write(reinterpret_cast<char *>(&split),sizeof(IdNode));
                index_file.seekp(first_pos);
                index_file.write(reinterpret_cast<char *>(&cur),sizeof(IdNode));
                //
                IdNode new_root;
                new_root.num = 1;
                new_root.list[0] = cur.list[id_block_size / 2];// 中间的索引往上移一层
                new_root.address[0] = first_pos;
                new_root.address[1] = second_pos;
                new_root.par = -1;
                new_root.leaf = false;
                index_file.seekp(0,std::ios::end);
                index_file.write(reinterpret_cast<char *>(&new_root),sizeof(IdNode));
                return;
            }
            else {
                // 找到插入的位置target后 向上递归
                index_file.seekg(cur.par);
                IdNode parent;
                index_file.read(reinterpret_cast<char *>(&parent),sizeof(IdNode));
                int nex_target = 0;
                for (int i = 0; i <= parent.num; i++) {
                    if (parent.address[i] == cur_add) {
                        nex_target = i;
                        break;
                    }
                }
                SplitNode(parent,cur.list[id_block_size / 2],nex_target,cur.par,second_pos);
            }
        }
    }

    void MergeNode(IdNode &cur,int target,long long cur_add) {
        // 先删
        for (int i = target; i < cur.num; i++) {
            cur.list[i - 1] = cur.list[i];
            cur.address[i] = cur.address[i + 1];
        }
        --cur.num;

        if (cur_add == root && cur.num >= 1 || cur.num >= (id_block_size + 1) / 2 - 1) {
            // 直接写入即可
            index_file.seekp(cur_add);
            index_file.write(reinterpret_cast<char *>(&cur),sizeof(IdNode));
            return;
        }
        else {
            // 需要调整
            IdNode parent;
            index_file.seekg(cur.par);
            index_file.read(reinterpret_cast<char *>(&parent),sizeof(IdNode));
            int nex_target = 0;
            for (int i = 0; i <= parent.num; i++) {
                if (parent.address[i] == cur_add) {
                    nex_target = i;
                    break;
                }
            }
            bool complete = false;
            IdNode left,right;
            if (nex_target > 0) {
                index_file.seekg(parent.address[nex_target - 1]);
                index_file.read(reinterpret_cast<char *>(&left),sizeof(IdNode));
                if (left.num > (id_block_size + 1) / 2 - 1) {
                    complete = true;
                    // 左边节点最后一个地址给右边
                    for (int i = cur.num; i > 0; i--) {
                        cur.list[i] = cur.list[i - 1];
                    }
                    for (int i = cur.num + 1; i > 0; i--) {
                        cur.address[i] = cur.address[i - 1];
                    }
                    cur.address[0] = left.address[left.num];
                    cur.list[0] = parent.list[nex_target - 1];
                    parent.list[nex_target - 1] = left.list[left.num - 1];
                    --left.num;
                    ++cur.num;
                    if (left.leaf == false) {
                        // 修改索引子节点的父亲
                        IdNode child;
                        index_file.seekg(cur.address[0]);
                        index_file.read(reinterpret_cast<char *>(&child),sizeof(IdNode));
                        child.par = cur_add;
                        index_file.seekp(cur.address[0]);
                        index_file.write(reinterpret_cast<char *>(&child),sizeof(IdNode));
                    }
                    // 写入文件
                    index_file.seekp(cur_add);
                    index_file.write(reinterpret_cast<char *>(&cur),sizeof(IdNode));
                    index_file.seekp(parent.address[nex_target - 1]);
                    index_file.write(reinterpret_cast<char *>(&left),sizeof(IdNode));
                    index_file.seekp(cur.par);
                    index_file.write(reinterpret_cast<char *>(&parent),sizeof(IdNode));
                }
            }
            if (!complete && nex_target < parent.num) {
                index_file.seekg(parent.address[nex_target + 1]);
                index_file.read(reinterpret_cast<char *>(&right),sizeof(IdNode));
                if (right.num > (id_block_size + 1) / 2 - 1) {
                    complete = true;
                    // 右边节点第一个地址给左边
                    cur.address[cur.num + 1] = right.address[0];
                    cur.list[cur.num] = parent.list[nex_target];
                    parent.list[nex_target] = right.list[0];
                    for (int i = 0; i < right.num - 1; i++) {
                        right.list[i] = right.list[i + 1];
                    }
                    for (int i = 0; i < right.num; i++) {
                        right.address[i] = right.address[i + 1];
                    }
                    ++cur.num;
                    --right.num;
                    if (right.leaf == false) {
                        IdNode child;
                        index_file.seekg(cur.address[cur.num]);
                        index_file.read(reinterpret_cast<char *>(&child),sizeof(IdNode));
                        child.par = cur_add;
                        index_file.seekp(cur.address[cur.num]);
                        index_file.write(reinterpret_cast<char *>(&child),sizeof(IdNode));
                    }
                    // 写入文件
                    index_file.seekp(cur_add);
                    index_file.write(reinterpret_cast<char *>(&cur),sizeof(IdNode));
                    index_file.seekp(parent.address[nex_target + 1]);
                    index_file.write(reinterpret_cast<char *>(&right),sizeof(IdNode));
                    index_file.seekp(cur.par);
                    index_file.write(reinterpret_cast<char *>(&parent),sizeof(IdNode));
                }
            }
            if (!complete) {
                // 父节点拉一个下来 继续向上递归
                if (root == cur.par && parent.num == 1) {
                    // 根节点往下移动一层
                    IdNode new_root,merge;
                    if (cur_add == parent.address[0]) {
                        new_root = cur;
                        index_file.seekg(parent.address[1]);
                        index_file.read(reinterpret_cast<char *>(&merge),sizeof(IdNode));
                    }
                    else {
                        index_file.seekg(parent.address[0]);
                        index_file.read(reinterpret_cast<char *>(&new_root),sizeof(IdNode));
                        merge = cur;
                    }
                    root = parent.address[0];

                    new_root.list[new_root.num] = parent.list[0];
                    for (int i = new_root.num + 1; i < new_root.num + 1 + merge.num; i++) {
                        new_root.list[i] = merge.list[i - new_root.num - 1];
                    }
                    for (int i = new_root.num + 1; i <= new_root.num + 1 + merge.num; i++) {
                        new_root.address[i] = merge.address[i - new_root.num - 1];
                        IdNode child;
                        if (merge.leaf == false) {
                            index_file.seekg(new_root.address[i]);
                            index_file.read(reinterpret_cast<char *>(&child),sizeof(IdNode));
                            child.par = root;
                            index_file.seekp(new_root.address[i]);
                            index_file.write(reinterpret_cast<char *>(&child),sizeof(IdNode));
                        }
                    }

                    new_root.par = -1;
                    new_root.num = new_root.num + 1 + merge.num;
                    index_file.seekp(root);
                    index_file.write(reinterpret_cast<char *>(&new_root),sizeof(IdNode));
                    return;
                }
                else if (nex_target > 0) {
                    index_file.seekg(parent.address[nex_target - 1]);
                    index_file.read(reinterpret_cast<char *>(&left),sizeof(IdNode));
                    left.list[left.num] = parent.list[nex_target - 1];
                    for (int i = left.num + 1; i < left.num + 1 + cur.num; i++) {
                        left.list[i] = cur.list[i - left.num - 1];
                    }
                    for (int i = left.num + 1; i <= left.num + 1 + cur.num; i++) {
                        left.address[i] = cur.address[i - left.num - 1];
                        IdNode child;
                        if (left.leaf == false) {
                            index_file.seekg(left.address[i]);
                            index_file.read(reinterpret_cast<char *>(&child),sizeof(IdNode));
                            child.par = parent.address[nex_target - 1];
                            index_file.seekp(left.address[i]);
                            index_file.write(reinterpret_cast<char *>(&child),sizeof(IdNode));
                        }
                    }
                    left.num = left.num + 1 + cur.num;
                    index_file.seekp(parent.address[nex_target - 1]);
                    index_file.write(reinterpret_cast<char *>(&left),sizeof(IdNode));
                    MergeNode(parent,nex_target,cur.par);
                }
                else {
                    index_file.seekg(parent.address[nex_target + 1]);
                    index_file.read(reinterpret_cast<char *>(&right),sizeof(IdNode));
                    cur.list[cur.num] = parent.list[nex_target];
                    for (int i = cur.num + 1; i < cur.num + 1 + right.num; i++) {
                        cur.list[i] = right.list[i - cur.num - 1];
                    }
                    for (int i = cur.num + 1; i <= cur.num + 1 + right.num; i++) {
                        cur.address[i] = right.address[i - cur.num - 1];
                        IdNode child;
                        if (cur.leaf == false) {
                            index_file.seekg(cur.address[i]);
                            index_file.read(reinterpret_cast<char *>(&child),sizeof(IdNode));
                            child.par = cur_add;
                            index_file.seekp(cur.address[i]);
                            index_file.write(reinterpret_cast<char *>(&child),sizeof(IdNode));
                        }
                    }
                    cur.num = cur.num + right.num + 1;
                    index_file.seekp(cur_add);
                    index_file.write(reinterpret_cast<char *>(&cur),sizeof(IdNode));
                    MergeNode(parent,nex_target + 1,cur.par);
                }
            }
        }
    }

public:
    // 初始化
    void Initialize(const std::string &index_file_name_, const std::string &file_name_) {
        index_file_name = index_file_name_;
        file_name = file_name_;
        index_file.open(index_file_name,std::ios::in | std::ios::binary);
        file.open(file_name,std::ios::binary | std::ios::in);
        // root的地址存在index_file的开头 占据sizeof(long long)
        if (!index_file.good()) {
            index_file.open(index_file_name,std::ios::out | std::ios::binary);
            index_file.seekp(0,std::ios::beg);
            root = sizeof(long long);
            index_file.write(reinterpret_cast<char *>(&root),sizeof(long long));
        }
        else{
            index_file.seekg(0,std::ios::beg);
            index_file.read(reinterpret_cast<char *>(&root),sizeof(long long));
        }
        // seq
        if (!file.good()) {
            file.open(file_name,std::ios::out | std::ios::binary);
            file.seekp(0,std::ios::beg);
            seq = sizeof(long long);
            file.write(reinterpret_cast<char *>(&seq),sizeof(long long));
        }
        else {
            file.seekg(0,std::ios::beg);
            file.read(reinterpret_cast<char *>(&seq),sizeof(long long));
        }
        index_file.close();
        file.close();
        index_file.open(index_file_name,std::ios::in | std::ios::binary | std::ios::out);
        file.open(file_name,std::ios::binary | std::ios::in | std::ios::out);
    }
    void Close() {
        // index_file.open(index_file_name,std::ios::binary | std::ios::in | std::ios::out);
        index_file.seekp(0,std::ios::beg);
        index_file.write(reinterpret_cast<char *>(&root),sizeof(long long));
        index_file.close();
        file.close();

    }
    void Clear() {
        index_file.close();
        index_file.open(index_file_name,std::ios::out | std::ios::binary);
        index_file.seekp(0,std::ios::beg);
        root = sizeof(long long);
        index_file.write(reinterpret_cast<char *>(&root),sizeof(long long));

        file.close();
        file.open(file_name,std::ios::out | std::ios::binary);
        file.seekp(0,std::ios::beg);
        seq = sizeof(long long);
        file.write(reinterpret_cast<char *>(&seq),sizeof(long long));
    }
    void Insert(T1 index,T2 value) {
        bool exist = false;
        index_file.seekg(root,std::ios::beg);
        IdNode cur;
        index_file.read(reinterpret_cast<char *>(&cur),sizeof(IdNode));
        if (cur.num == 0) {
            // 特判:空树开始插入
            cur.list[cur.num++] = Content(index,value);
            cur.leaf = true;
            cur.par = -1;
            cur.address[0] = seq;
            cur.address[1] = seq + sizeof(ContentNode);
            index_file.clear();// 清除错误标记
            index_file.seekp(root,std::ios::beg);
            index_file.write(reinterpret_cast<char *>(&cur),sizeof(IdNode));
            //
            ContentNode first,second;
            first.nex = seq + sizeof(ContentNode);
            second.nex = -1;
            second.cont[second.body_size++] = Content(index,value);
            file.seekp(seq,std::ios::beg);
            file.write(reinterpret_cast<char *>(&first),sizeof(ContentNode));
            file.write(reinterpret_cast<char *>(&second),sizeof(ContentNode));
            return;
        }

        int target,cur_add = root;
        Content new_cont(index,value);
        // 索引树里面至少有一个元素
        // 找到这个索引对应的最小位置
        while (true) {
            target = cur.num;
            for (int i = 0; i < cur.num; i++) {
                if (cur.list[i].index == index && cur.list[i].value == value) {
                    exist = true;
                    break;
                }
                if (cmp(new_cont,cur.list[i])) {
                    target = i;
                    break;
                }
            }
            if (cur.leaf == true) break;
            else {
                index_file.seekg(cur.address[target]);
                cur_add = cur.address[target];
                index_file.read(reinterpret_cast<char *>(&cur),sizeof(IdNode));
            }
        }
        //
        index_file.seekp(cur_add);
        file.seekg(cur.address[target]);
        ContentNode tmp;
        file.read(reinterpret_cast<char *>(&tmp),sizeof(ContentNode));
        // 不能重复插入
        for (int i = 0; i < tmp.body_size; i++) {
            if (tmp.cont[i].index == index && tmp.cont[i].value == value) {
                exist = true;
                break;
            }
        }
        if (exist) {
            return;
        }
        // 先按顺序加入到原序列
        int pos = tmp.body_size;
        for (int i = 0; i < tmp.body_size; i++) {
            if (cmp(new_cont,tmp.cont[i])) {
                pos = i;
                break;
            }
        }
        for (int i = tmp.body_size; i > pos; --i) tmp.cont[i] = tmp.cont[i - 1];
        tmp.cont[pos] = new_cont;
        ++tmp.body_size;

        if (tmp.body_size <= block_size) {
            // 没有超直接插入即可
            file.seekp(cur.address[target]);
            file.write(reinterpret_cast<char *>(&tmp),sizeof(ContentNode));
        }
        else {
            // 需要调整
            bool complete = false;
            ContentNode left,right;
            // 左边有空位置
            if (target > 0) {
                file.seekg(cur.address[target - 1]);
                file.read(reinterpret_cast<char *>(&left),sizeof(ContentNode));
                if (left.body_size < block_size) {
                    complete = true;
                    // 取出最小的移到左边
                    Content move = tmp.cont[0];
                    for (int i = 0; i < tmp.body_size - 1; i++) {
                        tmp.cont[i] = tmp.cont[i + 1];
                    }
                    --tmp.body_size;
                    left.cont[left.body_size++] = move;
                    cur.list[target - 1] = tmp.cont[0];
                    // 更新文件
                    index_file.seekp(cur_add);
                    index_file.write(reinterpret_cast<char *>(&cur),sizeof(IdNode));
                    file.seekp(cur.address[target]);
                    file.write(reinterpret_cast<char *>(&tmp),sizeof(ContentNode));
                    file.seekp(cur.address[target - 1]);
                    file.write(reinterpret_cast<char *>(&left),sizeof(ContentNode));
                }
            }
            // 右边有空位置
            if (!complete && target < cur.num) {
                file.seekg(cur.address[target + 1]);
                file.read(reinterpret_cast<char *>(&right),sizeof(ContentNode));
                if (right.body_size < block_size) {
                    complete = true;
                    // 取出最大的移到右边
                    Content move = tmp.cont[tmp.body_size - 1];
                    for (int i = right.body_size; i > 0; i--) {
                        right.cont[i] = right.cont[i - 1];
                    }
                    right.cont[0] = move;
                    ++right.body_size;
                    --tmp.body_size;
                    cur.list[target] = right.cont[0];
                    // 更新文件
                    index_file.seekp(cur_add);
                    index_file.write(reinterpret_cast<char *>(&cur),sizeof(IdNode));
                    file.seekp(cur.address[target]);
                    file.write(reinterpret_cast<char *>(&tmp),sizeof(ContentNode));
                    file.seekp(cur.address[target + 1]);
                    file.write(reinterpret_cast<char *>(&right),sizeof(ContentNode));
                }
            }
            if (!complete) {
                // 分裂后递归往上 数据存储区域直接处理 索引树修改递归向上(在SplitNode函数里)
                ContentNode split;
                long long origin_pos = cur.address[target];
                tmp.body_size = (block_size + 1) / 2;
                split.body_size = block_size + 1 - tmp.body_size;
                for (int i = 0; i < split.body_size; i++) {
                    split.cont[i] = tmp.cont[i + tmp.body_size];
                }

                file.seekp(0,std::ios::end);
                long long split_pos = file.tellp();

                split.nex = tmp.nex;
                tmp.nex = split_pos;

                file.write(reinterpret_cast<char *>(&split),sizeof(ContentNode));
                file.seekp(origin_pos);
                file.write(reinterpret_cast<char *>(&tmp),sizeof(ContentNode));

                SplitNode(cur,split.cont[0],target,cur_add,split_pos);
                // 向上递归分裂索引节点 参数 增加的索引，新索引插入的位置，当前索引节点的地址，新数据块的地址（在索引的下面一层）
            }
        }
        // index_file.close();
        // file.close();
    }
    void Delete(T1 index,T2 value) {
        IdNode cur;
        index_file.seekg(root);
        index_file.read(reinterpret_cast<char *>(&cur),sizeof(IdNode));
        Content tar_cont(index,value);
        int target;
        long long cur_add = root;
        index_file.clear();
        if (cur.num == 0) return;
        while (true) {
            target = cur.num;
            for (int i = 0; i < cur.num; i++) {
                if (cmp(tar_cont,cur.list[i])) {
                    target = i;
                    break;
                }
            }
            if (cur.leaf == true) break;
            else {
                index_file.seekg(cur.address[target]);
                cur_add = cur.address[target];
                index_file.read(reinterpret_cast<char *>(&cur),sizeof(IdNode));
            }
        }
        ContentNode tmp;
        file.seekg(cur.address[target]);
        file.read(reinterpret_cast<char *>(&tmp),sizeof(ContentNode));
        // 检查是否存在
        bool exist = false;
        int pos = 0;
        for (int i = 0; i < tmp.body_size; i++) {
            if (tmp.cont[i].index == index && tmp.cont[i].value == value) {
                exist = true;
                pos = i;
                break;
            }
        }
        if (!exist) {
            // 不存在
            return;
        }
        // 存在 先删掉再调整
        for (int i = pos; i < tmp.body_size - 1; i++) {
            tmp.cont[i] = tmp.cont[i + 1];
        }
        --tmp.body_size;
        if (target > 0) {
            // 删除可能改变索引id
            cur.list[target - 1] = tmp.cont[0];
            index_file.seekp(cur_add);
            index_file.write(reinterpret_cast<char *>(&cur),sizeof(IdNode));
        }
        else if (cur_add != root){
            // 要找到可能被修改的index
            IdNode parent,son;
            son = cur;
            index_file.seekg(cur.par);
            index_file.read(reinterpret_cast<char *>(&parent),sizeof(IdNode));
            long long son_add = cur_add;
            int par_target = 0;
            while (true) {
                for (int i = 0; i <= parent.num; i++) {
                    if (parent.address[i] == son_add) {
                        par_target = i;
                        break;
                    }
                }
                if (par_target > 0) {
                    parent.list[par_target - 1] = tmp.cont[0];
                    index_file.seekp(son.par);
                    index_file.write(reinterpret_cast<char *>(&parent),sizeof(IdNode));
                    break;
                }
                else if (parent.par == -1) break;
                else {
                    son_add = son.par;
                    son = parent;
                    index_file.seekg(parent.par);
                    index_file.read(reinterpret_cast<char *>(&parent),sizeof(IdNode));
                }
            }
        }
        // 根节点和数据块直接相连 直接删除不管是不是空
        if (cur_add == root && cur.num == 1) {
            file.seekp(cur.address[target]);
            file.write(reinterpret_cast<char *>(&tmp),sizeof(ContentNode));
            ContentNode other;
            if (target == 1 && tmp.body_size == 0) {
                file.seekg(cur.address[0]);
                file.read(reinterpret_cast<char *>(&other),sizeof(ContentNode));
                if (tmp.body_size + other.body_size == 0) {
                    cur.num = 0;
                    index_file.seekp(root);
                    index_file.write(reinterpret_cast<char *>(&cur),sizeof(IdNode));
                }
                else {
                    for (int i = 0; i < other.body_size; i++) tmp.cont[i] = other.cont[i];
                    tmp.body_size = other.body_size;
                    other.body_size = 0;
                    cur.list[0] = tmp.cont[0];
                    index_file.seekp(root);
                    index_file.write(reinterpret_cast<char *>(&cur),sizeof(IdNode));
                    file.seekp(cur.address[0]);
                    file.write(reinterpret_cast<char *>(&other),sizeof(ContentNode));
                    file.seekp(cur.address[1]);
                    file.write(reinterpret_cast<char *>(&tmp),sizeof(ContentNode));
                }
            }
        }
        else if (tmp.body_size >= (block_size + 1) / 2) {
            // 直接删除不需要调整
            file.seekp(cur.address[target]);
            file.write(reinterpret_cast<char *>(&tmp),sizeof(ContentNode));
        }
        else {
            // 需要调整
            bool complete = false;
            // 左边可以借一个
            ContentNode left,right;
            if (target > 0) {
                file.seekg(cur.address[target - 1]);
                file.read(reinterpret_cast<char *>(&left),sizeof(ContentNode));
                if (left.body_size > (block_size + 1) / 2) {
                    complete = true;
                    // 左边最大的移过来
                    Content move = left.cont[left.body_size - 1];
                    --left.body_size;
                    for (int i = tmp.body_size; i > 0; i--) {
                        tmp.cont[i] = tmp.cont[i - 1];
                    }
                    tmp.cont[0] = move;
                    ++tmp.body_size;
                    // 更新索引
                    cur.list[target - 1] = move;
                    // 写回文件
                    index_file.seekp(cur_add);
                    index_file.write(reinterpret_cast<char *>(&cur),sizeof(IdNode));
                    file.seekp(cur.address[target - 1]);
                    file.write(reinterpret_cast<char *>(&left),sizeof(ContentNode));
                    file.seekp(cur.address[target]);
                    file.write(reinterpret_cast<char *>(&tmp),sizeof(ContentNode));
                }
            }
            if (!complete && target < cur.num) {
                file.seekg(cur.address[target + 1]);
                file.read(reinterpret_cast<char *>(&right),sizeof(ContentNode));
                if (right.body_size > (block_size + 1) / 2) {
                    complete = true;
                    // 右边最小的移过来
                    Content move = right.cont[0];
                    tmp.cont[tmp.body_size++] = move;
                    for (int i = 0; i < right.body_size - 1; i++) {
                        right.cont[i] = right.cont[i + 1];
                    }
                    --right.body_size;
                    cur.list[target] = right.cont[0];
                    // 写回文件
                    index_file.seekp(cur_add);
                    index_file.write(reinterpret_cast<char *>(&cur),sizeof(IdNode));
                    file.seekp(cur.address[target]);
                    file.write(reinterpret_cast<char *>(&tmp),sizeof(ContentNode));
                    file.seekp(cur.address[target + 1]);
                    file.write(reinterpret_cast<char *>(&right),sizeof(ContentNode));
                }
            }
            if (!complete) {
                // 合并节点
                if (target > 0) {
                    // 和左边节点合并
                    file.seekg(cur.address[target - 1]);
                    file.read(reinterpret_cast<char *>(&left),sizeof(ContentNode));
                    for (int i = left.body_size; i < left.body_size + tmp.body_size; i++) {
                        left.cont[i] = tmp.cont[i - left.body_size];
                    }
                    left.body_size += tmp.body_size;
                    left.nex = tmp.nex;
                    file.seekp(cur.address[target - 1]);
                    file.write(reinterpret_cast<char *>(&left),sizeof(ContentNode));
                    MergeNode(cur,target,cur_add);// 合并节点向上递归
                    // 被删除的address位置 索引节点的地址
                }
                else {
                    // 和右边节点合并
                    file.seekg(cur.address[target + 1]);
                    file.read(reinterpret_cast<char *>(&right),sizeof(ContentNode));
                    for (int i = tmp.body_size; i < tmp.body_size + right.body_size; i++) {
                        tmp.cont[i] = right.cont[i - tmp.body_size];
                    }
                    tmp.body_size += right.body_size;
                    tmp.nex = right.nex;
                    file.seekp(cur.address[target]);
                    file.write(reinterpret_cast<char *>(&tmp),sizeof(ContentNode));
                    MergeNode(cur,target + 1,cur_add);
                }
            }
        }
    }

    void Find(T1 index) {
        IdNode cur;
        index_file.seekg(root);
        index_file.read(reinterpret_cast<char *>(&cur),sizeof(IdNode));
        int target;
        while (true) {
            target = cur.num;
            for (int i = 0; i < cur.num; i++) {
                if (index <= cur.list[i].index) {
                    //std::cerr << cur.list[i].index << " " << cur.list[i].value << std::endl;
                    target = i;
                    break;
                }
            }
            if (cur.leaf == true) break;
            else {
                index_file.seekg(cur.address[target]);
                index_file.read(reinterpret_cast<char *>(&cur),sizeof(IdNode));
            }
        }
        ContentNode tmp;
        file.seekg(cur.address[target]);
        file.read(reinterpret_cast<char *>(&tmp),sizeof(ContentNode));
        bool exist = false;
        while (tmp.cont[0].index <= index) {
            for (int i = 0; i < tmp.body_size; i++) {
                if (tmp.cont[i].index == index) {
                    exist = true;
                    std::cout << tmp.cont[i].value << " ";
                }
            }
            if (tmp.nex == -1) break;
            file.seekg(tmp.nex);
            file.read(reinterpret_cast<char *>(&tmp),sizeof(ContentNode));
        }
        if (!exist) std::cout << "null";
        std::cout << "\n";
        // index_file.close();
        // file.close();
    }
    bool HasData() {
        // 数据库是否为空
        index_file.seekg(root);
        IdNode cur;
        if (!index_file.read(reinterpret_cast<char *>(&cur),sizeof(IdNode))) {
            index_file.clear();
            return false;
        }
        if (cur.num == 0) return false;
        else return true;
    }
    bool Exist(T1 index) {
        IdNode cur;
        index_file.seekg(root);
        if (!index_file.read(reinterpret_cast<char *>(&cur),sizeof(IdNode))) {
            index_file.clear();
            return false;
        }
        int target;
        while (true) {
            target = cur.num;
            for (int i = 0; i < cur.num; i++) {
                if (index <= cur.list[i].index) {
                    //std::cerr << cur.list[i].index << std::endl;
                    target = i;
                    break;
                }
            }
            if (cur.leaf == true) break;
            else {
                index_file.seekg(cur.address[target]);
                index_file.read(reinterpret_cast<char *>(&cur),sizeof(IdNode));
            }
        }
        ContentNode tmp;
        file.seekg(cur.address[target]);
        file.read(reinterpret_cast<char *>(&tmp),sizeof(ContentNode));

        while (tmp.cont[0].index <= index) {
            for (int i = 0; i < tmp.body_size; i++) {
                if (tmp.cont[i].index == index) {
                    return true;
                }
            }
            if (tmp.nex == -1) break;
            file.seekg(tmp.nex);
            file.read(reinterpret_cast<char *>(&tmp),sizeof(ContentNode));
        }
        return false;
        // index_file.close();
        // file.close();
    }
    T2 GetData(T1 index) {
        IdNode cur;
        index_file.seekg(root);
        index_file.read(reinterpret_cast<char *>(&cur),sizeof(IdNode));
        index_file.clear();
        int target;
        while (true) {
            target = cur.num;
            for (int i = 0; i < cur.num; i++) {
                if (index <= cur.list[i].index) {
                    //std::cerr << cur.list[i].index << " " << cur.list[i].value << std::endl;
                    target = i;
                    break;
                }
            }
            if (cur.leaf == true) break;
            else {
                index_file.seekg(cur.address[target]);
                index_file.read(reinterpret_cast<char *>(&cur),sizeof(IdNode));
            }
        }
        ContentNode tmp;
        file.seekg(cur.address[target]);
        file.read(reinterpret_cast<char *>(&tmp),sizeof(ContentNode));
        //bool exist = false;
        while (tmp.cont[0].index <= index) {
            for (int i = 0; i < tmp.body_size; i++) {
                if (tmp.cont[i].index == index) {
                    return tmp.cont[i].value;
                }
            }
            if (tmp.nex == -1) break;
            file.seekg(tmp.nex);
            file.read(reinterpret_cast<char *>(&tmp),sizeof(ContentNode));
        }
        return T2();
        //if (!exist) std::cout << "null";
        //std::cout << "\n";
        // index_file.close();
        // file.close();
    }
    sjtu::vector<T2> GetAllDatas(T1 index) {
        sjtu::vector<T2> datas{};
        IdNode cur;
        index_file.seekg(root);
        index_file.read(reinterpret_cast<char *>(&cur),sizeof(IdNode));
        index_file.clear();
        int target;
        while (true) {
            target = cur.num;
            for (int i = 0; i < cur.num; i++) {
                if (index <= cur.list[i].index) {
                    //std::cerr << cur.list[i].index << " " << cur.list[i].value << std::endl;
                    target = i;
                    break;
                }
            }
            if (cur.leaf == true) break;
            else {
                index_file.seekg(cur.address[target]);
                index_file.read(reinterpret_cast<char *>(&cur),sizeof(IdNode));
            }
        }
        ContentNode tmp;
        file.seekg(cur.address[target]);
        file.read(reinterpret_cast<char *>(&tmp),sizeof(ContentNode));
        //bool exist = false;
        while (tmp.cont[0].index <= index) {
            for (int i = 0; i < tmp.body_size; i++) {
                if (tmp.cont[i].index == index) {
                    datas.push_back(tmp.cont[i].value);
                }
            }
            if (tmp.nex == -1) break;
            file.seekg(tmp.nex);
            file.read(reinterpret_cast<char *>(&tmp),sizeof(ContentNode));
        }
        return datas;
        //if (!exist) std::cout << "null";
        //std::cout << "\n";
        // index_file.close();
        // file.close();
    }
    /*void Print() {
        index_file.open(index_file_name,std::ios::binary | std::ios::in);
        index_file.seekg(root);
        IdNode id;
        index_file.read(reinterpret_cast<char *>(&id),sizeof(IdNode));
        std::cout << "root ";
        for (int i = 0; i < id.num; i++) {
            std::cout << id.list[i].index << " " << id.list[i].value << std::endl;
        }
        if (id.leaf == false) {
            IdNode node;
            for (int i = 0; i <= id.num; i++) {
                index_file.seekg(id.address[i]);
                index_file.read(reinterpret_cast<char *>(&node),sizeof(IdNode));
                for (int j = 0; j < node.num; j++) {
                    std::cout << node.list[j].index << " " << node.list[j].value << std::endl;
                }
            }
        }
        file.open(file_name);
        file.seekg(seq);
        ContentNode tmp;
        file.read(reinterpret_cast<char *>(&tmp),sizeof(ContentNode));
        while (true) {
            for (int i = 0; i < tmp.body_size; i++) {
                std::cout << tmp.cont[i].index << "," << tmp.cont[i].value << ";";
            }
            std::cout << "|";
            if (tmp.nex == -1) break;
            file.seekg(tmp.nex);
            file.read(reinterpret_cast<char *>(&tmp),sizeof(ContentNode));
        }
        std::cout << "\n-------------\n";
        index_file.close();
        file.close();
    }*/

};
#endif //TICKET_SYSTEM_2026_BPT_DATABASE_H