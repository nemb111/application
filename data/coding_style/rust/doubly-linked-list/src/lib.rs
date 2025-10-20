use std::ptr::NonNull;
use std::sync::Arc;

// this module adds some functionality based on the required implementations
// here like: `LinkedList::pop_back` or `Clone for LinkedList<T>`
// You are free to use anything in it, but it's mainly for the test framework.
mod pre_implemented;

struct Node<T> {
    value: Arc<T>,
    prev: Option<NonNull<Node<T>>>,
    next: Option<NonNull<Node<T>>>,
}

impl<T> Node<T> {
    fn new(value: T) -> Self {
        Self {
            value: Arc::new(value),
            prev: None,
            next: None,
        }
    }
}

pub struct LinkedList<T> {
    head: Option<NonNull<Node<T>>>,
    tail: Option<NonNull<Node<T>>>,
    len: usize,
}

pub struct Cursor<'a, T> {
    list: &'a mut LinkedList<T>,
    current_node: Option<NonNull<Node<T>>>,
}

pub struct Iter<'a, T> {
    current_node: Option<NonNull<Node<T>>>,
    _lifetime: std::marker::PhantomData<&'a T>,
}

impl<T> LinkedList<T> {
    pub fn new() -> Self {
        Self {
            head: None,
            tail: None,
            len: 0,
        }
    }

    // You may be wondering why it's necessary to have is_empty()
    // when it can easily be determined from len().
    // It's good custom to have both because len() can be expensive for some types,
    // whereas is_empty() is almost always cheap.
    // (Also ask yourself whether len() is expensive for LinkedList)
    pub fn is_empty(&self) -> bool {
        self.head.is_none() && self.tail.is_none()
    }

    pub fn len(&self) -> usize {
        self.len
    }

    /// Return a cursor positioned on the front element
    pub fn cursor_front(&mut self) -> Cursor<'_, T> {
        let head = self.head;
        Cursor {
            list: self,
            current_node: head,
        }
    }

    /// Return a cursor positioned on the back element
    pub fn cursor_back(&mut self) -> Cursor<'_, T> {
        let tail = self.tail;
        Cursor {
            list: self,
            current_node: tail,
        }
    }

    /// Return an iterator that moves from front to back
    pub fn iter(&self) -> Iter<'_, T> {
        Iter {
            current_node: self.head,
            _lifetime: std::marker::PhantomData,
        }
    }
}

impl<T> Drop for LinkedList<T> {
    fn drop(&mut self) {
        while !self.is_empty() {
            self.cursor_front().take();
        }
    }
}

// the cursor is expected to act as if it is at the position of an element
// and it also has to work with and be able to insert into an empty list.
impl<T> Cursor<'_, T> {
    /// Take a mutable reference to the current element
    pub fn peek_mut(&mut self) -> Option<&mut T> {
        if let Some(mut node) = self.current_node {
            unsafe { Arc::get_mut(&mut (node.as_mut()).value) }
        } else {
            None
        }
    }

    /// Move one position forward (towards the back) and
    /// return a reference to the new position
    #[allow(clippy::should_implement_trait)]
    pub fn next(&mut self) -> Option<&mut T> {
        let next;
        if let Some(node) = self.current_node {
            next = unsafe { node.as_ref().next };
            self.current_node = next; // Update the current node to the next one
        }

        if let Some(mut node) = self.current_node {
            unsafe { Arc::get_mut(&mut (node.as_mut()).value) }
        } else {
            None
        }
    }

    /// Move one position backward (towards the front) and
    /// return a reference to the new position
    pub fn prev(&mut self) -> Option<&mut T> {
        let prev;
        if let Some(node) = self.current_node {
            prev = unsafe { node.as_ref().prev };
            self.current_node = prev; // Update the current node to the next one
        }

        if let Some(mut node) = self.current_node {
            unsafe { Arc::get_mut(&mut (node.as_mut()).value) }
        } else {
            None
        }
    }

    /// Remove and return the element at the current position and move the cursor
    /// to the neighboring element that's closest to the back. This can be
    /// either the next or previous position.
    pub fn take(&mut self) -> Option<T> {
        if let Some(cur_node_ptr) = self.current_node {
            self.list.len -= 1;

            let cur_node = unsafe { cur_node_ptr.as_ref() };
            let prev_node = cur_node.prev;
            let next_node = cur_node.next;

            if let Some(mut prev) = prev_node {
                unsafe {
                    prev.as_mut().next = next_node;
                }
            } else {
                self.list.head = next_node;
            }

            if let Some(mut next) = next_node {
                unsafe {
                    next.as_mut().prev = prev_node;
                }
            } else {
                self.list.tail = prev_node;
            }

            let res = unsafe { Box::from_raw(cur_node_ptr.as_ptr()) };
            if next_node.is_some() {
                self.current_node = next_node;
            } else {
                self.current_node = prev_node;
            }

            return Arc::try_unwrap(res.value).ok();
        }

        None
    }

    pub fn insert_before(&mut self, _element: T) {
        let node_raw_ptr = Box::into_raw(Box::new(Node::new(_element)));
        self.list.len += 1;

        if self.list.head.is_none() || self.list.tail.is_none() || self.current_node.is_none() {
            self.list.head = NonNull::new(node_raw_ptr);
            self.list.tail = NonNull::new(node_raw_ptr);
            self.current_node = NonNull::new(node_raw_ptr);
            return;
        }

        let cur_node = unsafe { self.current_node.unwrap().as_mut() };

        if let Some(mut prev_nod_ptr) = cur_node.prev {
            let prev_nod = unsafe { prev_nod_ptr.as_mut() };
            prev_nod.next = NonNull::new(node_raw_ptr);
            unsafe {
                (*node_raw_ptr).prev = Some(prev_nod_ptr);
            }
        } else {
            self.list.head = NonNull::new(node_raw_ptr);
        }

        cur_node.prev = NonNull::new(node_raw_ptr);
        unsafe {
            (*node_raw_ptr).next = self.current_node;
        }
    }

    pub fn insert_after(&mut self, _element: T) {
        let node_raw_ptr = Box::into_raw(Box::new(Node::new(_element)));
        self.list.len += 1;

        if self.list.head.is_none() || self.list.tail.is_none() || self.current_node.is_none() {
            self.list.head = NonNull::new(node_raw_ptr);
            self.list.tail = NonNull::new(node_raw_ptr);
            self.current_node = NonNull::new(node_raw_ptr);
            return;
        }

        let cur_node = unsafe { self.current_node.unwrap().as_mut() };

        if let Some(mut next_nod_ptr) = cur_node.next {
            let next_nod = unsafe { next_nod_ptr.as_mut() };
            next_nod.prev = NonNull::new(node_raw_ptr);
            unsafe {
                (*node_raw_ptr).next = Some(next_nod_ptr);
            }
        } else {
            self.list.tail = NonNull::new(node_raw_ptr);
        }

        cur_node.next = NonNull::new(node_raw_ptr);
        unsafe {
            (*node_raw_ptr).prev = self.current_node;
        }
    }
}

impl<'a, T> Iterator for Iter<'a, T> {
    type Item = &'a T;

    fn next(&mut self) -> Option<&'a T> {
        self.current_node?;

        let res = unsafe { Some((self.current_node.unwrap().as_ref()).value.as_ref()) };
        self.current_node = unsafe { (self.current_node.unwrap().as_ref()).next };

        res
    }
}
