use std::iter::FromIterator;

struct Node<T> {
    data: T,
    next: Option<Box<Node<T>>>,
}

impl<T> Node<T> {
    pub fn new(data: T, next: Option<Box<Node<T>>>) -> Self {
        Node { data, next }
    }

    pub fn get(self) -> (T, Option<Box<Node<T>>>) {
        (self.data, self.next)
    }
}

pub struct SimpleLinkedList<T> {
    head: Option<Box<Node<T>>>,
}

impl<T> SimpleLinkedList<T> {
    pub fn new() -> Self {
        SimpleLinkedList { head: None }
    }

    pub fn len(&self) -> usize {
        let mut res: usize = 0;
        let mut node = &self.head;
        while let Some(n) = node {
            node = &n.next;
            res += 1;
        }

        res
    }

    pub fn push(&mut self, element: T) {
        let node = Node::new(element, self.head.take());
        self.head = Some(Box::new(node));
    }

    pub fn pop(&mut self) -> Option<T> {
        let res = self.head.take();
        if let Some(b) = res {
            let (data, node) = b.get();

            self.head = node;
            return Some(data);
        }

        None
    }

    pub fn peek(&self) -> Option<&T> {
        let node = &self.head;
        match node {
            Some(b) => Some(&b.data),
            None => None,
        }
    }

    pub fn rev(self) -> SimpleLinkedList<T> {
        let mut list = SimpleLinkedList::new();

        let mut node = self.head;
        while let Some(n) = node {
            list.push(n.data);
            node = n.next;
        }

        list
    }
}

impl<T> FromIterator<T> for SimpleLinkedList<T> {
    fn from_iter<I: IntoIterator<Item = T>>(iter: I) -> Self {
        let mut res = SimpleLinkedList::new();
        for i in iter {
            res.push(i);
        }

        res
    }
}

// In general, it would be preferable to implement IntoIterator for SimpleLinkedList<T>
// instead of implementing an explicit conversion to a vector. This is because, together,
// FromIterator and IntoIterator enable conversion between arbitrary collections.
// Given that implementation, converting to a vector is trivial:
//
// let vec: Vec<_> = simple_linked_list.into_iter().collect();
//
// The reason this exercise's API includes an explicit conversion to Vec<T> instead
// of IntoIterator is that implementing that interface is fairly complicated, and
// demands more of the student than we expect at this point in the track.

impl<T> Into<Vec<T>> for SimpleLinkedList<T> {
    fn into(self) -> Vec<T> {
        let mut vec = Vec::with_capacity(self.len());
        let mut rev_list = self.rev();

        while let Some(n) = rev_list.pop() {
            vec.push(n);
        }

        vec
    }
}
