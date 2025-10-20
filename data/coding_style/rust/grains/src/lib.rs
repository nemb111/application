pub fn square(s: u32) -> u64 {
    if 0 == s || 64 < s {
        panic!("Square must be between 1 and 64");
    }

    2u64.pow(s - 1)
}

pub fn total() -> u64 {
    let mut res = 0;
    for i in 1..=64 {
        res += square(i);
    }

    res
}
