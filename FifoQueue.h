#ifndef _FIFOQUEUE_
#define _FIFOQUEUE_

#include "Mutex.h"

using namespace std;

/**
 * Template linked list node object
 */
template <class Object>
class QNode
{

  public:

    /** Stored data **/
    Object data;

    /** Next element **/
    QNode* next;

    /**
     * Default Constructor
     */
    QNode(const Object & o) { next = NULL; data = o; };
};

/**
 * Template linked list FIFO Queue implementation
 */
template <class Object>
class Queue
{

  private:

    /** The number of nodes **/
    int qsize;

    /** First/Front node **/
    class QNode<Object>* head;

    /** End/Tail node **/
    class QNode<Object>* tail;

    /** Tracking node **/
    class QNode<Object>* current;

    /** Mutex for thread safe operations **/
    Mutex mutex;

  public:

    /**
     * Default Constructor
     */
    Queue()
    {
      qsize = 0; head = tail = current = NULL;
    };

    /**
     * Adds an element to the queue
     */
    void enqueue(const Object & o)
    {
      mutex.lock();
      current = new QNode<Object>(o);
      ++qsize;
      if(!head)
      {
        tail = head = current;
        head->next = NULL;
      }
      else
      {
        if(head == tail)
          head->next = tail;
        tail->next = current;
        current->next = NULL;
        tail = current;
      }
      mutex.unlock();
    };

    /**
     * Removes an element from the queue
     */
    Object dequeue()
    {
      Object o;
      mutex.lock();
      if(head)
      {
        --qsize;
        o = head->data;
        current = head->next;
        delete head;
        if(!current)
          head = tail = current = NULL;
        else
          head = current;
      }
      mutex.unlock();
      return o;
    };

    /**
     * Retrieves the size of the queue
     */
    const int& size() const {return qsize;};

    /**
     * Indicates the whether or not the queue is empty
     */
    bool empty() {return qsize == 0;};

    /**
     * Destructor
     */
    ~Queue()
    {
      while(head)
      {
        current = head;
        head = head->next;
        delete current;
        current = NULL;
      }
    };
};
#endif//_FIFOQUEUE_
