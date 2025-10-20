use std::collections::HashSet;

#[derive(Debug, PartialEq, Eq)]
pub struct Palindrome {
    m_factors: HashSet<(u64, u64)>,
    m_value: u64,
}

impl Palindrome {
    pub fn new(a: u64, b: u64) -> Palindrome {
        let value = a * b;
        let mut factors = HashSet::new();
        factors.insert((a, b));

        Palindrome {
            m_factors: factors,
            m_value: value,
        }
    }

    pub fn value(&self) -> u64 {
        self.m_value
    }

    pub fn insert(&mut self, a: u64, b: u64) {
        self.m_factors.insert((a, b));
    }
}

fn is_palindrome(num: u64) -> bool {
    const RADIX: u64 = 10;
    let mut n = num;
    let mut reversed = 0;

    while n != 0 {
        reversed = reversed * RADIX + n % RADIX;
        n /= RADIX;
    }

    num == reversed
}

fn test_and_create_palindrome((a, b): (u64, u64)) -> Option<Palindrome> {
    let possible_palindrome = a * b;

    if is_palindrome(possible_palindrome) {
        let mut res_pali = Palindrome::new(a, b);

        for div in (a + 1)..possible_palindrome {
            if (possible_palindrome % div) == 0 {
                let div2 = possible_palindrome / div;
                if div > div2 {
                    break;
                } else {
                    res_pali.insert(div, div2);
                }
            }
        }

        return Some(res_pali);
    }

    None
}

pub fn palindrome_products(min: u64, max: u64) -> Option<(Palindrome, Palindrome)> {
    let mut min_palindrome: Option<Palindrome> = None;
    let mut max_palindrome: Option<Palindrome> = None;

    // loop for finding the smallest palindrome
    'outer_min: for i in min..=max {
        for j in i..=max {
            min_palindrome = match (&min_palindrome, test_and_create_palindrome((i, j))) {
                (Some(min_pal), Some(new_pal)) if new_pal.value() < min_pal.value() => {
                    Some(new_pal)
                }
                (None, Some(new_pal)) => Some(new_pal),
                _ => min_palindrome,
            };
            if let Some(min_pal) = &min_palindrome {
                if i * i >= min_pal.value() {
                    break 'outer_min;
                }
            }
        }
    }

    // loop for finding the largest palindrome
    'outer_max: for i in (min..=max).rev() {
        for j in (min..=i).rev() {
            max_palindrome = match (&max_palindrome, test_and_create_palindrome((j, i))) {
                (Some(max_pal), Some(new_pal)) if new_pal.value() > max_pal.value() => {
                    Some(new_pal)
                }
                (None, Some(new_pal)) => Some(new_pal),
                _ => max_palindrome,
            };
            if let Some(max_pal) = &max_palindrome {
                if i * i <= max_pal.value() {
                    break 'outer_max;
                }
            }
        }
    }

    match (min_palindrome, max_palindrome) {
        (Some(min_pal), Some(max_pal)) => Some((min_pal, max_pal)),
        (None, None) => None,
        _ => unreachable!(),
    }
}
