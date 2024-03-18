//#include <iostream>
template <typename T>
class Stack
{
  public:
    Stack();
    Stack(const Stack&);
    ~Stack();
    Stack& operator=(const Stack&);
    int size();
    void push(const T&);
    void pop();
    T& top();
    void status();
  private:
    static const int chunk_size;
    unsigned maximum_size;
    int current_size;
    T* base;
};

template <typename T>
const int Stack<T>::chunk_size=5;

template <typename T>
Stack<T>::Stack()
:current_size(-1),maximum_size(chunk_size),base((T*)(std::malloc(chunk_size * sizeof(T))))
{
}

template <typename T>
Stack<T>::Stack(const Stack& s)
:maximum_size(s.maximum_size), current_size(s.current_size), base((T*)(std::malloc(chunk_size * sizeof(T))))
{
  int new_elements = 0; //Number of new elements successfully constructed
    try {
        for (int i = 0; i <= current_size; i++) {
            new (&base[i]) T(s.base[i]);
            new_elements++;
        }
    } catch (...) {
        //Exception occured during copy constructor uh oh! destruct elements and free memory
      for (int x = new_elements - 1; x >= 0; x--){
        base[x].~T();
      }
      std::free(base);
      throw;
    }
}
template <typename T>
Stack<T>::~Stack()
{
  //Destruct elements in reverse order
  for(int i=0; i<current_size+1; ++i)
    base[(current_size)-i].~T();
  free(base);
}

template <typename T>
Stack<T>& Stack<T>::operator=(const Stack& s)
{
  T* new_base = (T*)(std::malloc(s.maximum_size * sizeof(T)));
  int new_elements = 0; //Number of new elements successfully constructed

  try {
    for (new_elements = 0; new_elements <= s.current_size; new_elements++) {
      new (&new_base[new_elements]) T(s.base[new_elements]);
    }
  } catch (...) {
    //Exception occured during assignment uh oh! Destruct newly created elements in reverse order
    for (int x = new_elements - 1; x >= 0; x--) {
      new_base[x].~T();
    }
    free(new_base);
    throw;
  }

  //Destruct old elements in reverse order
  for (int i = current_size; i >= 0; i--) {
    base[i].~T();
  }

  free(base);

  maximum_size = s.maximum_size;
  current_size = s.current_size;
  base = new_base;

  return *this;
}

template <typename T>
void Stack<T>::push(const T& element)
{
    if (current_size == maximum_size - 1) //Check need for realocation
    {
        maximum_size += chunk_size;
        T* new_base = (T*)(std::malloc(maximum_size * sizeof(T)));
        int new_elements = 0; 
        try {
            for (new_elements = 0; new_elements <= current_size; new_elements++){
                new (&new_base[new_elements]) T(base[new_elements]);
            }
            for (int x = 0; x <= current_size; x++){
                base[x].~T();
            }
            free(base);
            base = new_base;
            current_size++;
            new (&base[current_size]) T(element);
        } catch (...) {
            for (int x = new_elements - 1; x >= 0; x--){
                new_base[x].~T();
            }
            //Set max size back to what it wouldve been before reallocation
            maximum_size -= chunk_size;
            free(new_base);
            throw;
        }
    } else {//No reallocation needed, increment size and add the new element
      try
      {
        current_size++;
        new (&base[current_size]) T(element);
      }
      catch(...){
        current_size--;
        throw;
      }
    }
}
template <typename T>
void Stack<T>::pop()
{
  if(current_size<0)
    throw "Nothing to pop!";
    base[current_size].~T();
    current_size--;
}

template <typename T>
T& Stack<T>::top()
{
  if(current_size<0)
    throw "Nothing to top!";
  return base[current_size];
}

template <typename T>
int Stack<T>::size()
{
  return current_size+1;
}

template <typename T>
void Stack<T>::status()
{
  std::cerr << "Stack with a capacity of " << maximum_size << " filled with " << size() << " items." << std::endl;
  return;
}