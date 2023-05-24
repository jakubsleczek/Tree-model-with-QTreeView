#ifndef TREEITEM_H
#define TREEITEM_H

#include <string>
//#include<QMetaObject>

class TreeItem
{
public:
    TreeItem(std::string pdata = "", TreeItem* pparent = nullptr); //C-tor
    ~TreeItem(); //De-tor
    bool AddSub(std::string pdata); //Append child
    int GetSubCount(); //Number of children
    bool setData(std::string pdata){mdata = pdata; return true;} //Set data
    TreeItem* child(int row); //Get child at position
    int columnCount() const; //Number of column - always 1
    std::string data() const; //Get data
    int row() const; //Returns a row of the object
    TreeItem* parentItem(); //Get parent
    TreeItem* getFirstChild(); //Get firstChild
    TreeItem* getNextSibling(); //Get nextSibling
    void setParent(TreeItem* objptr); //Set parent
    void setFirstChild(TreeItem* objptr); //Set firstChild
    void setNextSibling(TreeItem* objptr); //Set nextSibling
    bool Del(int index); //Deleta child and its branch
    bool insertChild(int position); //Insert new child at position
private:
    std::string mdata;
    TreeItem* nextSibling;
    TreeItem* firstChild;
    TreeItem* parent;
};
//Q_DECLARE_METATYPE(TreeItem)
#endif // TREEITEM_H
