//Albert Baker
//alb190010

#ifndef PROJECT4_NODE_H
#define PROJECT4_NODE_H

template<typename T>
class Node {
private:
    Node* up;
    Node* down;
    Node* left;
    Node* right;
    T payload;
public:
    Node();
    Node(T payload);
    Node* getUp(){ return up; }
    Node* getDown(){ return down; }
    Node* getLeft(){ return left; }
    Node* getRight(){ return right; }
    T getPayload(){ return payload; }
    void setUp(Node* n){ up = n; }
    void setDown(Node* n){ down = n; }
    void setLeft(Node* n){ left = n; }
    void setRight(Node* n){ right = n; }
    void setPayload(T payload){ this->payload = payload; }
    ~Node();
};

template<typename T>
Node<T>::Node(){
    up = nullptr;
    down = nullptr;
    left = nullptr;
    right = nullptr;
    payload = nullptr;
}

template<typename T>
Node<T>::Node(T payload){
    up = nullptr;
    down = nullptr;
    left = nullptr;
    right = nullptr;
    this->payload = payload;
}
template<typename T>
Node<T>::~Node(){
    delete payload;
}

#endif //PROJECT4_NODE_H
