// Oguzhan Yigit, CSE225, 150115042
// It processes input text files and and it puts words into BST and AVL trees, and make some deletion operations
// And it runs benchmark tests on these trees (both memory usage and execution time)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define NUMBER_OF_DELETED_TEXT1 19
#define NUMBER_OF_DELETED_TEXT2 16

struct nodeBst {
    char st[50];
    struct nodeBst *leftPtr;
    struct nodeBst *rightPtr;
};
typedef struct nodeBst NodeTextBst;

struct nodeAvl {
    char st[50];
    struct nodeAvl *leftPtr;
    struct nodeAvl *rightPtr;
    unsigned int height;
};
typedef struct nodeAvl NodeTextAvl;

FILE *inpf1 = NULL;
FILE *inpf2 = NULL;

// A utility function to do inorder traversal of BST
void printInOrderBST(NodeTextBst *root) {
    if (root != NULL) {
        // firstly reach to left nodes
        printInOrderBST(root->leftPtr);
        // print it
        printf("%s \n", root->st);
        //and then reach to right nodes
        printInOrderBST(root->rightPtr);
    }
}

int getNumOfNodes(NodeTextBst *root, NodeTextAvl *root2, int isBst) {
    if (isBst) {
        if(root == NULL){
            return 0;
        } else {
            return 1 + getNumOfNodes(root->leftPtr, NULL, 1) + getNumOfNodes(root->rightPtr, NULL, 1);
        }
    } else {
        if(root2 == NULL){
            return 0;
        } else {
            return 1 + getNumOfNodes(NULL, root2->leftPtr, 0) + getNumOfNodes(NULL, root2->rightPtr, 0);
        }
    }

}

/* A utility function to insert a new node with given key in BST */
NodeTextBst* insertToBst(NodeTextBst *root, char st[50])
{
    /* If the tree is empty, return a new node */
    if (root == NULL) {
        NodeTextBst *temp =  (NodeTextBst *)malloc(sizeof(NodeTextBst));
        strcpy(temp->st,st);
        temp->leftPtr = temp->rightPtr = NULL;
        root = temp;
    } else {
        /* Otherwise, recur down the tree */
        if (strcmp(st,root->st)<0)
            root->leftPtr  = insertToBst(root->leftPtr, st);
        else if (strcmp(st,root->st)>0)
            root->rightPtr = insertToBst(root->rightPtr, st);
        /* EXISTENCE CHECK: we are not covering
         * strcmp == 0 case here (same strings)
         * so dont mess with root ptr
         * and return untouched root
         */
    }
    /* return the (unchanged) node pointer */
    return root;
}

/* Given a non-empty binary search tree, return the node with minimum
   key value found in that tree. Note that the entire tree does not
   need to be searched. */
NodeTextBst * minValueNodeOfBst (NodeTextBst *node) {
    NodeTextBst *current = node;

    /* loop down to find the leftmost leaf */
    while (current->leftPtr != NULL)
        current = current->leftPtr;

    return current;
}

NodeTextBst* deleteNodeFromBst(NodeTextBst* root, char st[50]) {
    // base case
    if (root == NULL)
        return root;

    // If the key to be deleted is smaller than the root's key,
    // then it lies in left subtree
    if (strcmp(st,root->st)<0) {
        root->leftPtr = deleteNodeFromBst(root->leftPtr, st);

        // If the key to be deleted is greater than the root's key,
        // then it lies in right subtree
    } else if (strcmp(st,root->st)>0) {
        root->rightPtr = deleteNodeFromBst(root->rightPtr, st);

        // if key is same as root's key, then This is the node
        // to be deleted
    } else {
        // node with only one child or no child
        if (root->leftPtr == NULL) {
            NodeTextBst *temp = root->rightPtr;
            free(root);
            return temp;
        } else if (root->rightPtr == NULL) {
            NodeTextBst *temp = root->leftPtr;
            free(root);
            return temp;
        }

        // node with two children: Get the inorder successor (smallest
        // in the right subtree)
        NodeTextBst* temp = minValueNodeOfBst(root->rightPtr);

        // Copy the inorder successor's content to this node
        strcpy(root->st,temp->st);

        // Delete the inorder successor
        root->rightPtr = deleteNodeFromBst(root->rightPtr, temp->st);
    }
    return root;
}
////////////////////////////// BST //////////////////////////////////


///////////////////////////// AVL ///////////////////////////////////
// A utility function to get height of the tree
unsigned int height(NodeTextAvl *N)
{
    if (N == NULL)
        return 0;
    return N->height;
}

// A utility function to get maximum of two uints
unsigned int max(unsigned int a, unsigned int b)
{
    return (a > b)? a : b;
}

// A utility function to right rotate subtree rooted with y
// See the diagram given above.
NodeTextAvl *rightRotateTree(NodeTextAvl *y)
{
    NodeTextAvl *x = y->leftPtr;
    NodeTextAvl *T2 = x->rightPtr;

    // Perform rotation
    x->rightPtr = y;
    y->leftPtr = T2;

    // Update heights
    y->height = max(height(y->leftPtr), height(y->rightPtr))+1;
    x->height = max(height(x->leftPtr), height(x->rightPtr))+1;

    // Return new root
    return x;
}

// A utility function to left rotate subtree rooted with x
// See the diagram given above.
NodeTextAvl *leftRotateTree(NodeTextAvl *x)
{
    NodeTextAvl *y = x->rightPtr;
    NodeTextAvl *T2 = y->leftPtr;

    // Perform rotation
    y->leftPtr = x;
    x->rightPtr = T2;

    //  Update heights
    x->height = max(height(x->leftPtr), height(x->rightPtr))+1;
    y->height = max(height(y->leftPtr), height(y->rightPtr))+1;

    // Return new root
    return y;
}

// Get Balance factor of node N
int getBalance(NodeTextAvl *N)
{
    if (N == NULL)
        return 0;
    return height(N->leftPtr) - height(N->rightPtr);
}

NodeTextAvl * insertToAvl(NodeTextAvl* node, char st[50])
{
    /* 1.  Perform the normal BST rotation */
    if (node == NULL) {
        NodeTextAvl *temp = (NodeTextAvl*)
                malloc(sizeof(NodeTextAvl));
        strcpy(temp->st,st);
        temp->leftPtr = NULL;
        temp->rightPtr  = NULL;
        temp->height = 1;  // new node is initially added at leaf
        return temp;
    }

    if (strcmp(st,node->st)<0)
        node->leftPtr  = insertToAvl(node->leftPtr, st);
    else if (strcmp(st,node->st)>0)
        node->rightPtr = insertToAvl(node->rightPtr, st);
    else // Dont insert equal words into AVL
        return node;

    /* 2. Update height of this ancestor node */
    node->height = 1 + max(height(node->leftPtr),
                           height(node->rightPtr));

    /* 3. Get the balance factor of this ancestor
          node to check whether this node became
          unbalanced */
    int balance = getBalance(node);

    // If this node becomes unbalanced, then there are 4 cases

    // Left Left Case
    if (balance > 1 && strcmp(st,node->leftPtr->st)<0)
        return rightRotateTree(node);

    // Right Right Case
    if (balance < -1 && strcmp(st,node->rightPtr->st)>0)
        return leftRotateTree(node);

    // Left Right Case
    if (balance > 1 && strcmp(st,node->leftPtr->st)>0) {
        node->leftPtr =  leftRotateTree(node->leftPtr);
        return rightRotateTree(node);
    }

    // Right Left Case
    if (balance < -1 && strcmp(st,node->rightPtr->st)<0) {
        node->rightPtr = rightRotateTree(node->rightPtr);
        return leftRotateTree(node);
    }

    /* return the (unchanged) node pointer */
    return node;
}

/* Given a non-empty binary search tree, return the
   node with minimum key value found in that tree.
   Note that the entire tree does not need to be
   searched. */
NodeTextAvl * minValueNodeOfAvl(NodeTextAvl* node)
{
    NodeTextAvl* current = node;

    /* loop down to find the leftmost leaf */
    while (current->leftPtr != NULL)
        current = current->leftPtr;

    return current;
}

// Recursive function to delete a node with given key
// from subtree with given root. It returns root of
// the modified subtree.
NodeTextAvl *deleteNodeFromAvl(NodeTextAvl* root, char st[50])
{
    // STEP 1: PERFORM STANDARD BST DELETE

    if (root == NULL)
        return root;

    // If the key to be deleted is smaller than the
    // root's key, then it lies in left subtree
    if (strcmp(st,root->st)<0) {
        root->leftPtr = deleteNodeFromAvl(root->leftPtr, st);

        // If the key to be deleted is greater than the
        // root's key, then it lies in right subtree
    } else if(strcmp(st,root->st)>0) {
        root->rightPtr = deleteNodeFromAvl(root->rightPtr, st);

        // if key is same as root's key, then This is
        // the node to be deleted
    } else {
        // node with only one child or no child
        if((root->leftPtr == NULL) || (root->rightPtr == NULL)) {
            NodeTextAvl *temp = root->leftPtr ? root->leftPtr :
                             root->rightPtr;

            // No child case
            if (temp == NULL) {
                temp = root;
                root = NULL;
            } else { // One child case
                // Copy the contents of
                // the non-empty child
                *root = *temp;
            }
            free(temp);
        } else {
            // node with two children: Get the inorder
            // successor (smallest in the right subtree)
            NodeTextAvl * temp = minValueNodeOfAvl(root->rightPtr);

            // Copy the inorder successor's data to this node
            strcpy(root->st,temp->st);

            // Delete the inorder successor
            root->rightPtr = deleteNodeFromAvl(root->rightPtr, temp->st);
        }
    }

    // If the tree had only one node then return
    if (root == NULL)
        return root;

    // STEP 2: UPDATE HEIGHT OF THE CURRENT NODE
    root->height = 1 + max(height(root->leftPtr),
                           height(root->rightPtr));

    // STEP 3: GET THE BALANCE FACTOR OF THIS NODE (to
    // check whether this node became unbalanced)
    int balance = getBalance(root);

    // If this node becomes unbalanced, then there are 4 cases

    // Left Left Case
    if (balance > 1 && getBalance(root->leftPtr) >= 0)
        return rightRotateTree(root);

    // Left Right Case
    if (balance > 1 && getBalance(root->leftPtr) < 0) {
        root->leftPtr =  leftRotateTree(root->leftPtr);
        return rightRotateTree(root);
    }

    // Right Right Case
    if (balance < -1 && getBalance(root->rightPtr) <= 0)
        return leftRotateTree(root);

    // Right Left Case
    if (balance < -1 && getBalance(root->rightPtr) > 0) {
        root->rightPtr = rightRotateTree(root->rightPtr);
        return leftRotateTree(root);
    }

    return root;
}

// A utility function to print preorder traversal of
// the tree.
// The function also prints height of every node
void printInOrderAVL(NodeTextAvl *root) {
    if(root != NULL) {
        printInOrderAVL(root->leftPtr);
        printf("%s height: %d\n", root->st,root->height);
        printInOrderAVL(root->rightPtr);
    }
}
////////////////////////////  AVL ///////////////////////////////////




//////////////////////////// MAIN FUNCTION ///////////////////////////
int main() {
    NodeTextBst *bstInput1 = NULL;
    NodeTextAvl *avlInput1 = NULL;
    NodeTextBst *bstInput2 = NULL;
    NodeTextAvl *avlInput2 = NULL;
    char delTexts1[NUMBER_OF_DELETED_TEXT1][51];
    char delTexts2[NUMBER_OF_DELETED_TEXT2][51];
    char string1[50];
    char string2[50];
    struct timeval stop, start;

    // read files and construct LLs
    inpf1 = fopen("input1.txt","r");

    if (inpf1 == NULL) {
        printf("WARNING: Problem is detected while opening input1.txt file\n\n");
        printf("Make sure the input file is in the same directory as with executable\n");
        return -1;
    }

    printf("\n////////////////// Processing input1.txt //////////////////\n\n");

    printf("/************** BST implementation *********************/\n\n");
    printf("CREATION OF TREE:\n");
    gettimeofday(&start, NULL);
    int i = 0;
    while (fscanf(inpf1, " %255s", string1) == 1) {
        // store first 19 text in an multidimensional static array
        if (i < NUMBER_OF_DELETED_TEXT1) {
            strcpy(delTexts1[i++],string1);
        }
        // add into BST
        bstInput1 = insertToBst(bstInput1, string1);
    }
    gettimeofday(&stop, NULL);
    printf("Total Memory Used: %d byte\nTotal Execution Time: %lu microseconds\n",getNumOfNodes(bstInput1, NULL, 1)*sizeof(NodeTextBst), stop.tv_usec - start.tv_usec);

    printf("\n");

    printf("DELETION NODES FROM BST TREE(first sentence, 19 words):\n");
    // Delete first 19 words (first sentence) from BST
    gettimeofday(&start, NULL);
    for(int h = 0; h < NUMBER_OF_DELETED_TEXT1; h++) {
        bstInput1 = deleteNodeFromBst(bstInput1, delTexts1[h]);
    }
    gettimeofday(&stop, NULL);
    printf("Total Memory Freed: %d byte\nTotal Execution Time: %lu microseconds\n",NUMBER_OF_DELETED_TEXT1*sizeof(NodeTextBst), stop.tv_usec - start.tv_usec);

    // Seek pointer to beginning of file
    fseek(inpf1, 0, SEEK_SET);

    printf("\n/************** AVL implementation *********************/\n\n");
    printf("CREATION OF AVL TREE:\n");
    gettimeofday(&start, NULL);
    while (fscanf(inpf1, " %255s", string1) == 1) {
        // add into AVL
        avlInput1 = insertToAvl(avlInput1, string1);
    }
    gettimeofday(&stop, NULL);
    printf("Total Memory Used: %d byte\nTotal Execution Time: %lu microseconds\n",getNumOfNodes(NULL, avlInput1, 0)*sizeof(NodeTextAvl), stop.tv_usec - start.tv_usec);

    printf("\n");

    // Delete first 19 words (first sentence) from AVL
    printf("DELETION NODES FROM AVL TREE(first sentence, 19 words):\n");
    gettimeofday(&start, NULL);
    for(int h = 0; h < NUMBER_OF_DELETED_TEXT1; h++) {
        avlInput1 = deleteNodeFromAvl(avlInput1, delTexts1[h]);
    }
    gettimeofday(&stop, NULL);
    printf("Total Memory Freed: %d byte\nTotal Execution Time: %lu microseconds\n",NUMBER_OF_DELETED_TEXT1*sizeof(NodeTextAvl), stop.tv_usec - start.tv_usec);

    // close the file
    fclose(inpf1);

    //////////////////////// END OF PROCESSING INPUT 1 ///////////////////////////////////////

    // read files and construct LLs
    inpf2 = fopen("input2.txt","r");

    if (inpf2 == NULL) {
        printf("WARNING: Problem is detected while opening input2.txt file\n\n");
        printf("Make sure the input file is in the same directory as with executable\n");
        return -1;
    }

    printf("\n--------------------------------------------------------------------------------------------------\n");

    printf("\n////////////////// Processing input2.txt //////////////////\n\n");

    printf("/************** BST implementation *********************/\n\n");
    printf("CREATION OF BST TREE:\n");
    gettimeofday(&start, NULL);
    int d = 0;
    while (fscanf(inpf2, " %255s", string2) == 1) {
        // store first 19 text in an multidimensional static array
        if (d < NUMBER_OF_DELETED_TEXT2) {
            strcpy(delTexts2[d++],string2);
        }
        // add into BST
        bstInput2 = insertToBst(bstInput2, string2);
    }
    gettimeofday(&stop, NULL);
    printf("Total Memory Used: %d byte\nTotal Execution Time: %lu microseconds\n",getNumOfNodes(bstInput2, NULL, 1)*sizeof(NodeTextBst), stop.tv_usec - start.tv_usec);

    printf("\n");

    // Delete first 19 words (first sentence) from BST
    printf("DELETION NODES FROM BST TREE(first sentence, 16 words):\n");
    gettimeofday(&start, NULL);
    for(int h = 0; h < NUMBER_OF_DELETED_TEXT2; h++) {
        bstInput2 = deleteNodeFromBst(bstInput2, delTexts2[h]);
    }
    gettimeofday(&stop, NULL);
    printf("Total Memory Freed: %d byte\nTotal Execution Time: %lu microseconds\n",NUMBER_OF_DELETED_TEXT2*sizeof(NodeTextBst), stop.tv_usec - start.tv_usec);

    // Seek pointer to beginning of file (for re-reading it)
    fseek(inpf2, 0, SEEK_SET);

    printf("\n/************** AVL implementation *********************/\n\n");
    printf("CREATION OF AVL TREE:\n");
    gettimeofday(&start, NULL);
    while (fscanf(inpf2, " %255s", string2) == 1) {
        // add into AVL
        avlInput2 = insertToAvl(avlInput2, string2);
    }
    gettimeofday(&stop, NULL);
    printf("Total Memory Used: %d byte\nTotal Execution Time: %lu microseconds\n",getNumOfNodes(NULL, avlInput1, 0)*sizeof(NodeTextAvl), stop.tv_usec - start.tv_usec);

    printf("\n");

    // Delete first 19 words (first sentence) from AVL
    printf("DELETION NODES FROM AVL TREE(first sentence, 16 words):\n");
    gettimeofday(&start, NULL);
    for(int h = 0; h < NUMBER_OF_DELETED_TEXT2; h++) {
        avlInput2 = deleteNodeFromAvl(avlInput2, delTexts2[h]);
    }
    gettimeofday(&stop, NULL);
    printf("Total Memory Freed: %d byte\nTotal Execution Time: %lu microseconds\n",NUMBER_OF_DELETED_TEXT2*sizeof(NodeTextAvl), stop.tv_usec - start.tv_usec);

    // close the file
    fclose(inpf2);

    return 0;

}
