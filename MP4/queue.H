/*Implementation of Queue for Thread Scheduler*/

class Queue{

private:
	
	typedef struct list{
        	Thread *thread;
        	struct list *next;
	};

	struct list **Q;

public:
	int isEmpty();
	void enqueue(Thread *th);
	Thread * dequeue();

}
