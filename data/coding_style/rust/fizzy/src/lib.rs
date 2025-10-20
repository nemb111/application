// the PhantomData instances in this file are just to stop compiler complaints
// about missing generics; feel free to remove them

/// A Matcher is a single rule of fizzbuzz: given a function on T, should
/// a word be substituted in? If yes, which word?
pub struct Matcher<T> {
    matcher: Box<dyn Fn(T) -> bool>,
    subs: String,
}

impl<T> Matcher<T> {
    pub fn new<F, S>(matcher: F, subs: S) -> Matcher<T>
    where
        F: Fn(T) -> bool + 'static,
        S: ToString,
    {
        Self {
            matcher: Box::new(matcher),
            subs: subs.to_string(),
        }
    }
}

/// A Fizzy is a set of matchers, which may be applied to an iterator.
///
/// Strictly speaking, it's usually more idiomatic to use `iter.map()` than to
/// consume an iterator with an `apply` method. Given a Fizzy instance, it's
/// pretty straightforward to construct a closure which applies it to all
/// elements of the iterator. However, we're using the `apply` pattern
/// here because it's a simpler interface for students to implement.
///
/// Also, it's a good excuse to try out using impl trait.
pub struct Fizzy<T> {
    matchers: Vec<Matcher<T>>,
}

impl<T> Fizzy<T> {
    pub fn new() -> Self {
        Self { matchers: vec![] }
    }

    // feel free to change the signature to `mut self` if you like
    pub fn add_matcher(self, matcher: Matcher<T>) -> Self {
        let mut ret = self;
        ret.matchers.push(matcher);
        ret
    }

    /// map this fizzy onto every element of an iterator, returning a new iterator
    pub fn apply<I>(self, iter: I) -> impl Iterator<Item = String>
    where
        I: Iterator<Item = T>,
        T: std::fmt::Display + Copy,
    {
        iter.map(move |x| {
            let res = self
                .matchers
                .iter()
                .map(|m| {
                    if (m.matcher)(x) {
                        m.subs.clone()
                    } else {
                        "".to_string()
                    }
                })
                .collect::<Vec<_>>()
                .join("");
            if "" == res {
                x.to_string()
            } else {
                res
            }
        })
    }
}

/// convenience function: return a Fizzy which applies the standard fizz-buzz rules
pub fn fizz_buzz<T>() -> Fizzy<T>
where
    T: std::ops::Rem<T, Output = T> + PartialEq + From<u8>,
{
    Fizzy::new()
        .add_matcher(Matcher::new(|x| x % 3.into() == 0.into(), "fizz"))
        .add_matcher(Matcher::new(|x| x % 5.into() == 0.into(), "buzz"))
}
