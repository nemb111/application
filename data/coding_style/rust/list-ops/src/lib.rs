mod iterators;

/// Yields each item of a and then each item of b
pub fn append<I, J>(_a: I, _b: J) -> impl Iterator<Item = I::Item>
where
    I: Iterator,
    J: Iterator<Item = I::Item>,
{
    iterators::append(_a, _b)
}

/// Combines all items in all nested iterators inside into one flattened iterator
pub fn concat<I>(_nested_iter: I) -> impl Iterator<Item = <I::Item as Iterator>::Item>
where
    I: Iterator,
    I::Item: Iterator,
{
    iterators::concat(_nested_iter)
}

/// Returns an iterator of all items in iter for which `predicate(item)` is true
pub fn filter<I, F>(_iter: I, _predicate: F) -> impl Iterator<Item = I::Item>
where
    I: Iterator,
    F: Fn(&I::Item) -> bool,
{
    iterators::filter(_iter, _predicate)
}

pub fn length<I: Iterator>(_iter: I) -> usize {
    let mut i = 0usize;

    for _ in _iter {
        i += 1;
    }

    i
}

/// Returns an iterator of the results of applying `function(item)` on all iter items
pub fn map<I, F, U>(_iter: I, _function: F) -> impl Iterator<Item = U>
where
    I: Iterator,
    F: Fn(I::Item) -> U,
{
    iterators::map(_iter, _function)
}

pub fn foldl<I, F, U>(mut _iter: I, _initial: U, _function: F) -> U
where
    I: Iterator,
    F: Fn(U, I::Item) -> U,
{
    let mut res = _initial;

    for item in _iter {
        res = _function(res, item);
    }
    res
}

pub fn foldr<I, F, U>(mut _iter: I, _initial: U, _function: F) -> U
where
    I: DoubleEndedIterator,
    F: Fn(U, I::Item) -> U,
{
    foldl(reverse(_iter), _initial, _function)
}

/// Returns an iterator with all the original items, but in reverse order
pub fn reverse<I: DoubleEndedIterator>(_iter: I) -> impl Iterator<Item = I::Item> {
    iterators::reverse(_iter)
}
