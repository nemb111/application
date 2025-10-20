pub struct AppendIterator<T: Iterator<Item = I>, U: Iterator<Item = I>, I> {
    first_iter: T,
    second_iter: U,
}

impl<T: Iterator<Item = I>, U: Iterator<Item = I>, I> Iterator for AppendIterator<T, U, I> {
    type Item = <T as Iterator>::Item;

    fn next(&mut self) -> Option<Self::Item> {
        if let Some(item) = self.first_iter.next() {
            return Some(item);
        }
        if let Some(item) = self.second_iter.next() {
            return Some(item);
        }
        None
    }
}

pub fn append<T: Iterator<Item = I>, U: Iterator<Item = I>, I>(
    first_iter: T,
    second_iter: U,
) -> impl Iterator<Item = I> {
    AppendIterator {
        first_iter,
        second_iter,
    }
}

pub struct ConcatIterator<T: Iterator<Item = I>, I> {
    iterators: T,
    current_iter: Option<T::Item>,
}

impl<T: Iterator<Item = I>, I: Iterator> Iterator for ConcatIterator<T, I> {
    type Item = <T::Item as Iterator>::Item;

    fn next(&mut self) -> Option<Self::Item> {
        if let Some(ref mut current_iter) = self.current_iter {
            if let Some(item) = current_iter.next() {
                return Some(item);
            }
        }
        if let Some(next_iter) = self.iterators.next() {
            self.current_iter = Some(next_iter);
            return self.next();
        }

        None
    }
}

pub fn concat<T: Iterator<Item = I>, I: Iterator>(
    nested_iter: T,
) -> impl Iterator<Item = <T::Item as Iterator>::Item> {
    ConcatIterator {
        iterators: nested_iter,
        current_iter: None,
    }
}

pub struct FilterIterator<T: Iterator<Item = I>, F, I> {
    iter: T,
    predicate: F,
}

#[allow(clippy::manual_find)]
impl<T: Iterator<Item = I>, F, I> Iterator for FilterIterator<T, F, I>
where
    F: Fn(&I) -> bool,
{
    type Item = I;

    fn next(&mut self) -> Option<Self::Item> {
        for item in self.iter.by_ref() {
            if (self.predicate)(&item) {
                return Some(item);
            }
        }
        None
    }
}

pub fn filter<T: Iterator<Item = I>, F, I>(iter: T, predicate: F) -> impl Iterator<Item = I>
where
    F: Fn(&I) -> bool,
{
    FilterIterator { iter, predicate }
}

pub struct MapIterator<T: Iterator, F> {
    iter: T,
    function: F,
}

impl<T: Iterator, F, U> Iterator for MapIterator<T, F>
where
    F: Fn(T::Item) -> U,
{
    type Item = U;

    fn next(&mut self) -> Option<Self::Item> {
        if let Some(item) = self.iter.next() {
            return Some((self.function)(item));
        }
        None
    }
}

pub fn map<T: Iterator, F, U>(iter: T, function: F) -> impl Iterator<Item = U>
where
    F: Fn(T::Item) -> U,
{
    MapIterator { iter, function }
}

pub struct ReverseIterator<T: DoubleEndedIterator> {
    iter: T,
}

impl<T: DoubleEndedIterator> Iterator for ReverseIterator<T> {
    type Item = T::Item;

    fn next(&mut self) -> Option<Self::Item> {
        self.iter.next_back()
    }
}

pub fn reverse<T: DoubleEndedIterator>(iter: T) -> impl Iterator<Item = T::Item> {
    ReverseIterator { iter }
}
