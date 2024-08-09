#include <tuple>
#include <vector>
#include <algorithm>
#include <cassert>
#include <stdexcept>

/**
 * An abstract template base of the KDTree class
 */
template<typename...>
class KDTree;

/**
 * A partial template specialization of the KDTree class
 * The time complexity of functions are based on n and k
 * n is the size of the KDTree
 * k is the number of dimensions
 * @typedef Key         key type
 * @typedef Value       value type
 * @typedef Data        key-value pair
 * @static  KeySize     k (number of dimensions)
 */
template<typename ValueType, typename... KeyTypes>
class KDTree<std::tuple<KeyTypes...>, ValueType> {
public:
    typedef std::tuple<KeyTypes...> Key;
    typedef ValueType Value;
    typedef std::pair<const Key, Value> Data;
    static inline constexpr size_t KeySize = std::tuple_size<Key>::value;
    static_assert(KeySize > 0, "Can not construct KDTree with zero dimension");
protected:
    struct Node {
        Data data;
        Node *parent;
        Node *left = nullptr;
        Node *right = nullptr;

        Node(const Key &key, const Value &value, Node *parent) : data(key, value), parent(parent) {}

        const Key &key() { return data.first; }

        Value &value() { return data.second; }
    };

public:
    /**
     * A bi-directional iterator for the KDTree
     * ! ONLY NEED TO MODIFY increment and decrement !
     */
    class Iterator {
    private:
        KDTree *tree;
        Node *node;

        Iterator(KDTree *tree, Node *node) : tree(tree), node(node) {}

        /**
         * Increment the iterator
         * Time complexity: O(log n)
         */
        void increment() {
            // TODO: implement this function
            if(!node) return;
            // if there is right subtree, go to its smallest element
            if(node->right){
                node = node->right;
                while(node->left) node = node->left;
                return;
            }
            // else find the parent where the node is in its left subtree
            while(node->parent && node->parent->left!=node)
                node = node->parent;
            // parent is either null or a node
            node = node->parent;
        }

        /**
         * Decrement the iterator
         * Time complexity: O(log n)
         */
        void decrement() {
            // TODO: implement this function
            // if not node, then get to the largest element
            if(!node){
                node = tree->root;
                while(node->right) node = node->right;
                return;
            } // else if there is a left subtree, find the next greatest element
            if(node->left){
                node = node->left;
                while(node->right) node = node->right;
                return;
            } // otherwise, get to the node where the current node is in its right subtree
            Node *cp = node;
            while(node->parent && node->parent->left == node){
                node = node->parent;
            }
            if(!node->parent){
                node = cp;
            } // parent is either a node or null
            else node = node->parent;
        }

    public:
        friend class KDTree;

        Iterator() = delete;

        Iterator(const Iterator &) = default;

        Iterator &operator=(const Iterator &) = default;

        Iterator &operator++() {
            increment();
            return *this;
        }

        Iterator operator++(int) {
            Iterator temp = *this;
            increment();
            return temp;
        }

        Iterator &operator--() {
            decrement();
            return *this;
        }

        Iterator operator--(int) {
            Iterator temp = *this;
            decrement();
            return temp;
        }

        bool operator==(const Iterator &that) const {
            return node == that.node;
        }

        bool operator!=(const Iterator &that) const {
            return node != that.node;
        }

        Data *operator->() {
            return &(node->data);
        }

        Data &operator*() {
            return node->data;
        }
    };

protected:                      // DO NOT USE private HERE!
    Node *root = nullptr;       // root of the tree
    size_t treeSize = 0;        // size of the tree

    /**
     * Find the node with key
     * Time Complexity: O(k log n)
     * @tparam DIM current dimension of node
     * @param key
     * @param node
     * @return the node with key, or nullptr if not found
     */
    template<size_t DIM>
    Node *find(const Key &key, Node *node) {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        // TODO: implement this function
        if(!node) return nullptr;
        if(key == node->key()) return node;
        if(std::get<DIM>(node->key()) <= std::get<DIM>(key))
            return find<DIM_NEXT>(key, node->right);
        return find<DIM_NEXT>(key, node->left);
    }

    /**
     * Insert the key-value pair, if the key already exists, replace the value only
     * Time Complexity: O(k log n)
     * @tparam DIM current dimension of node
     * @param key
     * @param value
     * @param node
     * @param parent
     * @return whether insertion took place (return false if the key already exists)
     */
    template<size_t DIM>
    bool insert(const Key &key, const Value &value, Node *&node, Node *parent) {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        // TODO: implement this function
        // if not node, create a node and return it
        if(!node){
            node = new Node(key, value, parent);
            treeSize++;
            return true;
        }                               // if key is found (all dimensions are equal), change its value and return false
        if(key == node->key()){
            node->value() = value;
            return false;
        }                               // otherwise, go to the proper subtree
        if(std::get<DIM>(node->key()) <= std::get<DIM>(key))
            return insert<DIM_NEXT>(key, value, node->right, node);
        return insert<DIM_NEXT>(key, value, node->left, node);
    }

    /**
     * Compare two keys on a dimension
     * Time Complexity: O(1)
     * @tparam DIM comparison dimension
     * @tparam Compare
     * @param a
     * @param b
     * @param compare
     * @return relationship of two keys on a dimension with the compare function
     */
    template<size_t DIM, typename Compare>
    static bool compareKey(const Key &a, const Key &b, Compare compare = Compare()) {
        if (std::get<DIM>(a) != std::get<DIM>(b)){
            return compare(std::get<DIM>(a), std::get<DIM>(b));
        }
        return compare(a, b);
    }

    /**
     * Compare two nodes on a dimension
     * Time Complexity: O(1)
     * @tparam DIM comparison dimension
     * @tparam Compare
     * @param a
     * @param b
     * @param compare
     * @return the minimum / maximum of two nodes
     */
    template<size_t DIM, typename Compare>
    static Node *compareNode(Node *a, Node *b, Compare compare = Compare()) {
        if (!a) return b;
        if (!b) return a;
        return compareKey<DIM, Compare>(a->key(), b->key(), compare) ? a : b;
    }

    /**
     * Find the minimum node on a dimension
     * Time Complexity: ?
     * @tparam DIM_CMP comparison dimension
     * @tparam DIM current dimension of node
     * @param node
     * @return the minimum node on a dimension
     */
    template<size_t DIM_CMP, size_t DIM>
    Node *findMin(Node *node) {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        // TODO: implement this function
        if(!node) return nullptr;
        // find the minimum in left subtree
        Node *min = findMin<DIM_CMP, DIM_NEXT>(node->left);
        if(DIM_CMP!=DIM){
            // if current node doesnt have the same DIM, then compare with the minimum from right subtree too
            Node *rightMin = findMin<DIM_CMP, DIM_NEXT>(node->right);
            min = compareNode<DIM_CMP, std::less<>>(min, rightMin);
        }
        // compare with the current node and return the minimum one
        return compareNode<DIM_CMP, std::less<>>(node, min);
    }

    /**
     * Find the maximum node on a dimension
     * Time Complexity: ?
     * @tparam DIM_CMP comparison dimension
     * @tparam DIM current dimension of node
     * @param node
     * @return the maximum node on a dimension
     */
    template<size_t DIM_CMP, size_t DIM>
    Node *findMax(Node *node) {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        // TODO: implement this function
        if(!node) return nullptr;
        // find the maximum from right subtree
        Node *max = findMax<DIM_CMP, DIM_NEXT>(node->right);
        if(DIM_CMP!=DIM){
            // if current node doesnt have the same DIM, then compare with the maximum from left subtree too
            Node *leftMax = findMax<DIM_CMP, DIM_NEXT>(node->left);
            max = compareNode<DIM_CMP, std::greater<>>(max, leftMax);
        }
        // compare with the current node and return the maximum one
        return compareNode<DIM_CMP, std::greater<>>(node, max);
    }

    template<size_t DIM>
    Node *findMinDynamic(size_t dim) {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        if (dim >= KeySize) {
            dim %= KeySize;
        }
        if (dim == DIM) return findMin<DIM, 0>(root);
        return findMinDynamic<DIM_NEXT>(dim);
    }

    template<size_t DIM>
    Node *findMaxDynamic(size_t dim) {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        if (dim >= KeySize) {
            dim %= KeySize;
        }
        if (dim == DIM) return findMax<DIM, 0>(root);
        return findMaxDynamic<DIM_NEXT>(dim);
    }

    /**
     * Erase a node with key (check the pseudocode in project description)
     * Time Complexity: max{O(k log n), O(findMin)}
     * @tparam DIM current dimension of node
     * @param node
     * @param key
     * @return nullptr if node is erased, else the (probably) replaced node
     */
    template<size_t DIM>
    Node *erase(Node *node, const Key &key) {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        // TODO: implement this function
        if(!node) return nullptr;

        if(key==node->key()){
            if(!node->left && !node->right){
                delete node;
                treeSize--;
                return nullptr;
            }
            else if(node->right){
                Node *minNode = findMin<DIM, DIM_NEXT>(node->right);
                //Key tmpKey = minNode->key();
                const_cast<Key &>(node->key()) = minNode->key();
                node->value() = minNode->value();
                node->right = erase<DIM_NEXT>(node->right, minNode->key());
            }
            else if(node->left){
                Node *maxNode = findMax<DIM, DIM_NEXT>(node->left);
                //Key tmpKey = maxNode->key();
                const_cast<Key &>(node->key()) = maxNode->key();
                node->value() = maxNode->value();
                node->left = erase<DIM_NEXT>(node->left, maxNode->key());
            }
        }
        else{
            if(std::get<DIM>(key) < std::get<DIM>(node->key())){
                node->left = erase<DIM_NEXT>(node->left, key);
            }
            else{
                node->right = erase<DIM_NEXT>(node->right, key);
            }
        }
        return node;
    }

    template<size_t DIM>
    Node *eraseDynamic(Node *node, size_t dim) {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        if (dim >= KeySize) {
            dim %= KeySize;
        }
        if (dim == DIM) return erase<DIM>(node, node->key());
        return eraseDynamic<DIM_NEXT>(node, dim);
    }

    // TODO: define your helper functions here if necessary
    // copy the other tree through its root node
    Node* copyFrom(Node *parent, Node *thatRoot){
        if(!thatRoot) return nullptr;
        Node *curr = new Node(thatRoot->key(), thatRoot->value(), parent);
        curr->left = copyFrom(curr, thatRoot->left);
        curr->right = copyFrom(curr, thatRoot->right);
        return curr;
    }
    // delete the tree using its root node
    void destruct(Node *node){
        if(!node) return;
        destruct(node->left);
        destruct(node->right);
        delete node;
    }
    static bool uniqueComp(const Data &a, const Data &b) {
        return a.first == b.first;
    }

    template <size_t DIM>
    static bool sortComp(const Data &a, const Data &b) {
        return compareKey<DIM, std::less<>>(a.first, b.first);
    }
    // deep copy helper function, creates a tree out of vector
    template<size_t DIM>
    Node *vectorConstruct(Node *parent, int left, int right, std::vector<std::pair<Key, Value>> &v){
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        if(left>right) return nullptr;

        int mid = (left+right)/2;
        std::nth_element(v.begin() + left, v.begin() + mid, v.begin()+right, sortComp<DIM>);

        Node* curr = new Node(v[mid].first, v[mid].second, parent);
        curr->left = vectorConstruct<DIM_NEXT>(curr, left, mid-1, v);
        curr->right = vectorConstruct<DIM_NEXT>(curr, mid+1, right, v);
        return curr;
    }

public:
    KDTree() = default;

    /**
     * Time complexity: O(kn log n)
     * @param v we pass by value here because v need to be modified
     */
    explicit KDTree(std::vector<std::pair<Key, Value>> v) {
        // TODO: implement this function
        if (v.empty()){
            return;
        }
        destruct(root);
        root=nullptr;
        std::stable_sort(v.begin(), v.end(), sortComp<0>);
        auto ip = std::unique(v.rbegin(), v.rend(), uniqueComp);
        v.erase(v.begin(), ip.base());
        root = vectorConstruct<0>(nullptr, 0, (int)v.size()-1, v);
        treeSize = v.size();
    }

    /**
     * Time complexity: O(n)
     */
    KDTree(const KDTree &that) {
        // TODO: implement this function
        if(this==&that) return;
        destruct(this->root);
        this->root = copyFrom(nullptr, that.root);
        this->treeSize = that.treeSize;
    }

    /**
     * Time complexity: O(n)
     */
    KDTree &operator=(const KDTree &that) {
        // TODO: implement this function
        if(this==&that) return *this;
        destruct(this->root);
        this->root = copyFrom(nullptr, that.root);
        this->treeSize = that.treeSize;
        return *this;
    }

    /**
     * Time complexity: O(n)
     */
    ~KDTree() {
        // TODO: implement this function
        destruct(this->root);
        this->treeSize = 0;
    }

    Iterator begin() {
        if (!root) return end();
        auto node = root;
        while (node->left) node = node->left;
        return Iterator(this, node);
    }

    Iterator end() {
        return Iterator(this, nullptr);
    }

    Iterator find(const Key &key) {
        return Iterator(this, find<0>(key, root));
    }

    void insert(const Key &key, const Value &value) {
        insert<0>(key, value, root, nullptr);
    }

    template<size_t DIM>
    Iterator findMin() {
        return Iterator(this, findMin<DIM, 0>(root));
    }

    Iterator findMin(size_t dim) {
        return Iterator(this, findMinDynamic<0>(dim));
    }

    template<size_t DIM>
    Iterator findMax() {
        return Iterator(this, findMax<DIM, 0>(root));
    }

    Iterator findMax(size_t dim) {
        return Iterator(this, findMaxDynamic<0>(dim));
    }

    bool erase(const Key &key) {
        auto prevSize = treeSize;
        erase<0>(root, key);
        return prevSize > treeSize;
    }

    Iterator erase(Iterator it) {
        if (it == end()) return it;
        auto node = it.node;
        if (!it.node->left && !it.node->right) {
            it.node = it.node->parent;
        }
        size_t depth = 0;
        auto temp = node->parent;
        while (temp) {
            temp = temp->parent;
            ++depth;
        }
        eraseDynamic<0>(node, depth % KeySize);
        return it;
    }

    size_t size() const { return treeSize; }
};
