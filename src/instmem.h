#ifndef INSTMEM_H
#define INSTMEM_H

#include<vector>
#include<QStringList>
class InstMem
{
public:
    InstMem();
    void push_back(int index);
    void setInst(QStringList list);
    int getSize() const;
    int getLineNum(int index) const;
    const QString getInst(int index) const;
    ~InstMem() {}
private:
    std::vector<int> lineNum;
    QStringList inst;
};

#endif // INSTMEM_H
