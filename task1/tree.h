#pragma once

#include <memory>

namespace bintree {
    template <typename T>
    struct TNode : public std::enable_shared_from_this<TNode<T>> { //to get weak_ptr from this for parent link
        using TNodePtr = std::shared_ptr<TNode<T>>;
        using TNodeConstPtr = std::shared_ptr<const TNode<T>>;

        bool hasLeft() const {
            return bool(left);
        }

        bool hasRight() const {
            return bool(right);
        }

        bool hasParent() const {
            return bool(parent.lock());
        }

        T& getValue() {
            return value;
        }

        const T& getValue() const {
            return value;
        }

        TNodePtr getLeft() {
            return left;
        }

        TNodeConstPtr getLeft() const {
            return left;
        }

        TNodePtr getRight() {
            return right;
        }

        TNodeConstPtr getRight() const {
            return right;
        }

        TNodePtr getParent() {
            return parent.lock();
        }

        TNodeConstPtr getParent() const {
            return parent.lock();
        }

        static TNodePtr createLeaf(T v) {
            return TNodePtr(new TNode(v)); //constructor is private, so it is not possible to get shared_ptr or weak_ptr (to prevent cycling) directly
        }

        static TNodePtr fork(T v, TNode* left, TNode* right) {
            TNodePtr ptr = TNodePtr(new TNode(v, left, right));//std::make_shared<TNode>(v, left, right);
	    //constructor is private, so it is not possible to get shared_ptr or weak_ptr (to prevent cycling) directly
            setParent(ptr->getLeft(), ptr);
            setParent(ptr->getRight(), ptr);
            return ptr;
        }

        TNodePtr replaceLeft(TNodePtr l) {
            setParent(l, this->shared_from_this()); // get shared_ptr to make weak_ptr for parent
            setParent(left, std::weak_ptr<TNode>());
            std::swap(l, left);
            return l;
        }

        TNodePtr replaceRight(TNodePtr r) {
            setParent(r, this->shared_from_this()); // get shared_ptr to make weak_ptr for parent
            setParent(right, std::weak_ptr<TNode>());
            std::swap(r, right);
            return r;
        }

        TNodePtr replaceRightWithLeaf(T v) {
            return replaceRight(createLeaf(v));
        }

        TNodePtr replaceLeftWithLeaf(T v) {
            return replaceLeft(createLeaf(v));
        }

        TNodePtr removeLeft() {
          //  delete left; //because constructor is private, strict delete needed
            return replaceLeft(std::weak_ptr<TNode>());
        }
        TNodePtr removeRight() {
           // delete right; //because constructor is private, strict delete needed
            return replaceRight(std::weak_ptr<TNode>());
        }

    private:
        T value;
        TNodePtr left = nullptr;
        TNodePtr right = nullptr;
        std::weak_ptr<TNode> parent = std::weak_ptr<TNode>(); //change for weak_ptr to make right memory release

        TNode(T v)
            : value(v)
        {
        }
        TNode(T v, TNode* left, TNode* right)
            : value(v)
            , left(left)
            , right(right)
        {
        }

        static void setParent(TNodePtr node, std::weak_ptr<TNode> parent) {
            
            if (node)
                node->parent = parent;
        }

    };
}
