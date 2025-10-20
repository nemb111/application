#[derive(Debug, PartialEq)]
pub struct CustomSet<T> {
    data: Vec<T>,
}

impl<T: Copy + std::cmp::Ord> CustomSet<T> {
    pub fn new(input: &[T]) -> Self {
        let mut data: Vec<_> = input.iter().copied().collect();
        data.sort_unstable();
        data.dedup();
        Self { data }
    }

    pub fn contains(&self, element: &T) -> bool {
        self.data.binary_search(element).is_ok()
    }

    pub fn add(&mut self, element: T) {
        if let Err(pos) = self.data.binary_search(&element) {
            self.data.insert(pos, element);
        }
    }

    pub fn is_subset(&self, other: &Self) -> bool {
        if other.data.len() < self.data.len() || (other.data.is_empty() && !self.data.is_empty()) {
            return false;
        }

        self.data.iter().map(|x| other.contains(x)).all(|x| x)
    }

    pub fn is_empty(&self) -> bool {
        self.data.is_empty()
    }

    pub fn is_disjoint(&self, other: &Self) -> bool {
        !other.data.iter().map(|x| self.contains(x)).any(|x| x)
    }

    pub fn intersection(&self, other: &Self) -> Self {
        let (first, second) = if self.data.len() < other.data.len() {
            (self, other)
        } else {
            (other, self)
        };

        let data: Vec<_> = first
            .data
            .iter()
            .copied()
            .filter(|x| second.contains(&x))
            .collect();

        Self { data }
    }

    pub fn difference(&self, other: &Self) -> Self {
        let data = self
            .data
            .iter()
            .copied()
            .filter(|x| !other.contains(x))
            .collect();

        Self { data }
    }

    pub fn union(&self, other: &Self) -> Self {
        let (first, second) = if self.data.len() < other.data.len() {
            (self, other)
        } else {
            (other, self)
        };

        let mut res = Self {
            data: first.data.iter().copied().collect(),
        };

        for i in &second.data {
            res.add(*i);
        }

        res
    }
}
