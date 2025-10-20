use std::cmp::Ordering;
use std::iter::Iterator;
use std::ops::Add;
use std::ops::Mul;
use std::ops::Sub;

/// Type implementing arbitrary-precision decimal arithmetic

#[derive(Eq, PartialEq)]
pub struct Decimal {
    integer: Vec<u8>,
    fractional: Vec<u8>,
    is_positive: bool,
}

impl Decimal {
    pub fn try_from(input: &str) -> Option<Decimal> {
        let (input, is_positive) = if input.starts_with('-') {
            (&input[1..], false)
        } else {
            (&input[..], true)
        };
        let split: Vec<_> = input.split('.').collect();

        let (integer, fractional) = match (split.get(0), split.get(1)) {
            (Some(integer), Some(fractional)) => (
                integer.trim_start_matches('+').trim_start_matches('0'),
                fractional.trim_end_matches('0'),
            ),
            (Some(integer), None) => (integer.trim_start_matches('+').trim_start_matches('0'), ""),
            _ => return None,
        };

        let to_vec_u8 = |inp: &str| -> Option<Vec<u8>> {
            inp.chars()
                .rev()
                .map(|c| c.to_string().parse::<u8>())
                .collect::<Result<Vec<_>, _>>()
                .ok()
        };

        let integer: Vec<u8> = to_vec_u8(integer)?;
        let fractional: Vec<u8> = to_vec_u8(fractional)?;

        Some(Decimal {
            integer,
            fractional,
            is_positive,
        })
    }

    fn add_intern(self, other: Self) -> Self {
        let len_fractional = self.fractional.len().max(other.fractional.len());
        let fractional_iter = Self::zip_to_longer_fractional(self.fractional, other.fractional);
        let integer_iter = Self::zip_to_longer_integer(self.integer, other.integer);

        let mut chained_iter = fractional_iter
            .chain(integer_iter.chain(std::iter::once((0, 0))))
            .scan(0u8, |state, (a, b)| {
                let res = *state + a + b;
                *state = res / 10;

                Some(res % 10)
            });

        let fractional = chained_iter
            .by_ref()
            .take(len_fractional)
            .skip_while(|&x| x == 0)
            .collect();
        let mut integer: Vec<_> = chained_iter.collect();
        while let Some(0) = integer.last() {
            integer.pop();
        }

        Self {
            integer,
            fractional,
            is_positive: self.is_positive,
        }
    }

    fn sub_intern(self, other: Self) -> Self {
        let len_fractional = self.fractional.len().max(other.fractional.len());
        let (first, second, is_positive) = if self > other {
            (self, other, true)
        } else {
            (other, self, false)
        };
        let fractional_iter = Self::zip_to_longer_fractional(first.fractional, second.fractional);
        let integer_iter = Self::zip_to_longer_integer(first.integer, second.integer);

        let mut chained_iter = fractional_iter
            .chain(integer_iter.chain(std::iter::once((0, 0))))
            .scan(0u8, |state, (a, b)| {
                let res = 10 + a - *state - b;
                *state = 1 - (res / 10);

                Some(res % 10)
            });

        let fractional = chained_iter
            .by_ref()
            .take(len_fractional)
            .skip_while(|&x| x == 0)
            .collect();
        let mut integer: Vec<_> = chained_iter.collect();
        while let Some(0) = integer.last() {
            integer.pop();
        }

        Self {
            integer,
            fractional,
            is_positive,
        }
    }

    fn zip_to_longer_integer<ESI>(iter1: ESI, iter2: ESI) -> impl Iterator<Item = (u8, u8)>
    where
        ESI: IntoIterator<Item = u8>,
        ESI::IntoIter: ExactSizeIterator<Item = u8>,
    {
        let iter1 = iter1.into_iter();
        let iter2 = iter2.into_iter();

        let (tk1, tk2) = if iter1.len() > iter2.len() {
            (0, iter1.len() - iter2.len())
        } else {
            (iter2.len() - iter1.len(), 0)
        };

        iter1
            .chain(std::iter::repeat(0).take(tk1))
            .zip(iter2.chain(std::iter::repeat(0).take(tk2)))
    }

    fn zip_to_longer_fractional<ESI>(iter1: ESI, iter2: ESI) -> impl Iterator<Item = (u8, u8)>
    where
        ESI: IntoIterator<Item = u8>,
        ESI::IntoIter: ExactSizeIterator<Item = u8>,
    {
        let iter1 = iter1.into_iter();
        let iter2 = iter2.into_iter();

        let (tk1, tk2) = if iter1.len() > iter2.len() {
            (0, iter1.len() - iter2.len())
        } else {
            (iter2.len() - iter1.len(), 0)
        };

        std::iter::repeat(0)
            .take(tk1)
            .chain(iter1)
            .zip(std::iter::repeat(0).take(tk2).chain(iter2))
    }
}

impl std::fmt::Debug for Decimal {
    fn fmt(&self, f: &mut std::fmt::Formatter) -> std::fmt::Result {
        write!(
            f,
            "{}{}.{}",
            if self.is_positive { "" } else { "-" },
            self.integer
                .iter()
                .rev()
                .map(|&i| i.to_string())
                .collect::<String>(),
            self.fractional
                .iter()
                .rev()
                .map(|&i| i.to_string())
                .collect::<String>()
        )
    }
}

impl Ord for Decimal {
    fn cmp(&self, other: &Self) -> Ordering {
        match (self.is_positive, other.is_positive) {
            (true, false) => return Ordering::Greater,
            (false, true) => return Ordering::Less,
            _ => (),
        }

        let res = self
            .integer
            .len()
            .cmp(&other.integer.len())
            .then(self.integer.iter().rev().cmp(other.integer.iter().rev()))
            .then(
                self.fractional
                    .iter()
                    .rev()
                    .cmp(other.fractional.iter().rev()),
            );

        if !self.is_positive {
            res.reverse()
        } else {
            res
        }
    }
}

impl PartialOrd for Decimal {
    fn partial_cmp(&self, other: &Decimal) -> Option<Ordering> {
        Some(self.cmp(other))
    }
}

impl Add for Decimal {
    type Output = Self;

    fn add(mut self, mut other: Self) -> Self {
        match (self.is_positive, other.is_positive) {
            (true, false) => {
                other.is_positive = true;
                self.sub(other)
            }
            (false, true) => {
                self.is_positive = true;
                other.sub(self)
            }
            _ => self.add_intern(other),
        }
    }
}

impl Sub for Decimal {
    type Output = Self;

    fn sub(self, mut other: Self) -> Self {
        match (self.is_positive, other.is_positive) {
            (false, false) => {
                other.is_positive = true;
                other - self
            }
            (false, true) => {
                other.is_positive = false;
                self + other
            }
            (true, false) => {
                other.is_positive = true;
                self + other
            }
            _ => self.sub_intern(other),
        }
    }
}

impl Mul for Decimal {
    type Output = Self;

    fn mul(self, other: Self) -> Self {
        let is_positive = match (self.is_positive, other.is_positive) {
            (true, false) => false,
            (false, true) => false,
            _ => true,
        };

        let mut tmp = Self {
            integer: vec![],
            fractional: vec![],
            is_positive,
        };

        let (longer, shorter) = if (self.fractional.len() + self.integer.len())
            > (other.fractional.len() + self.integer.len())
        {
            (self, other)
        } else {
            (other, self)
        };

        let longer_fractional_len = longer.fractional.len();
        let shorter_fractional_len = shorter.fractional.len();

        let longer_iter = longer
            .fractional
            .into_iter()
            .chain(longer.integer.into_iter())
            .chain(std::iter::once(0));
        let shorter_iter = shorter
            .fractional
            .into_iter()
            .chain(shorter.integer.into_iter());

        for (i, a) in shorter_iter.enumerate() {
            let mut one_step_mul_iter =
                std::iter::repeat(0)
                    .take(i)
                    .chain(longer_iter.clone().scan(0u8, |state, b| {
                        let res = *state + (a * b);
                        *state = res / 10;
                        Some(res % 10)
                    }));

            let fractional = one_step_mul_iter
                .by_ref()
                .chain(std::iter::repeat(0))
                .take(longer_fractional_len + shorter_fractional_len)
                .collect();
            let integer = one_step_mul_iter.collect();

            tmp = tmp
                + Self {
                    fractional,
                    integer,
                    is_positive,
                }
        }

        tmp
    }
}
