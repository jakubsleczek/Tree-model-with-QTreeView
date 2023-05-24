#include "treeitem.h"

TreeItem::TreeItem(std::string pdata, TreeItem* pparent)
    :mdata{pdata}, nextSibling{nullptr}, firstChild{nullptr}, parent{pparent}{

}

TreeItem::~TreeItem(){
    if(firstChild != nullptr){
        delete firstChild;
    }
    if(nextSibling != nullptr){
        delete nextSibling;
    }
}

bool TreeItem::AddSub(std::string pdata){
    TreeItem* current = firstChild;
    if(current == nullptr){
        firstChild = new TreeItem(pdata, this);
    }
    else{
        while(current->nextSibling != nullptr)
            current = current->nextSibling;
        current->nextSibling = new TreeItem(pdata, this);
    }
    return true;
}



bool TreeItem::insertChild(int position){
    TreeItem* current = firstChild;
    TreeItem* child = new TreeItem("", this);
    if(position == 0){
        child->nextSibling = firstChild;
        firstChild = child;
    }
    else{
        for(int i=1; i < position; i++)
            current = current->nextSibling;
        child->nextSibling = current->nextSibling;
        current->nextSibling = child;
    }
    return true;
}

int TreeItem::GetSubCount(){
    int result{0};
    TreeItem* current = firstChild;
    while(current != nullptr){
        result++;
        current = current->nextSibling;
    }
    return result;
}

TreeItem* TreeItem::child(int row){
    TreeItem *ochild = this->firstChild;
    if(row < 0 || row >= GetSubCount())
        return nullptr;
    while(row != 0){
        ochild = ochild->nextSibling;
        row--;
    }
    return ochild;
}

int TreeItem::columnCount() const{
    return 1;
}

std::string TreeItem::data() const{
    return mdata;
}

int TreeItem::row() const{
    if(parent != nullptr){
    int row = 0;
    TreeItem* current = this->parent->firstChild;
    while(this != current){
        current = current->nextSibling;
        row++;
    }
    return row;
    }
    return 0;
}

TreeItem* TreeItem::parentItem(){
    return parent;
}

TreeItem* TreeItem::getFirstChild(){
    return firstChild;
}


TreeItem* TreeItem::getNextSibling(){
    return nextSibling;
}

void TreeItem::setParent(TreeItem* objptr){
    parent = objptr;
}

void TreeItem::setFirstChild(TreeItem* objptr){
    firstChild = objptr;
}

void TreeItem::setNextSibling(TreeItem* objptr){
    nextSibling = objptr;
}

bool TreeItem::Del(int index){
    TreeItem* current = nullptr;
    TreeItem* previous = nullptr;
    if(index == 0){
        current = firstChild;
        firstChild = firstChild->nextSibling;
    }
    else{
        previous = firstChild;
        current = firstChild->nextSibling;
        for(int i=1; i<index; ++i){
            current = current->nextSibling;
            previous = previous->nextSibling;
        }
        previous->nextSibling = current->nextSibling;
    }
        current->nextSibling = nullptr;
        current->parent = nullptr;
        delete current;
    return true;
}
