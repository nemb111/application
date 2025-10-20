use std::collections::HashSet;

pub fn sum_of_multiples(limit: u32, factors: &[u32]) -> u32 {
    let min_element = match factors.iter().filter(|&&x| x != 0).min() {
        Some(x) => x,
        None => return 0,
    };

    let mut num_iteration = limit / min_element;
    if 0 == limit % min_element {
        num_iteration -= 1;
    }

    let mut book_keeping = HashSet::new();
    let mut result = 0u32;

    for i in 1..=num_iteration {
        result += factors
            .iter()
            .map(|x| x * i)
            .filter(|&x| x < limit && book_keeping.insert(x))
            .sum::<u32>();
    }

    result
}
