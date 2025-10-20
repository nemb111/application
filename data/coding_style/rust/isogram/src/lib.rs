use std::collections::HashSet;

pub fn check(candidate: &str) -> bool {
    let mut res = true;

    let mut hash_set = HashSet::new();

    candidate
        .chars()
        .filter(|c| c.is_ascii_alphabetic())
        .map(|c| c.to_ascii_lowercase())
        .for_each(|c| {
            res = res && hash_set.insert(c);
        });

    res
}
