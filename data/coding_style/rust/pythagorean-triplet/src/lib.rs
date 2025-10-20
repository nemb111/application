use std::collections::HashSet;

pub fn find(sum: u32) -> HashSet<[u32; 3]> {
    let mut res = HashSet::new();

    for c in (3..sum - 2).rev() {
        for b in (2..c).rev() {
            if let Some(a) = sum.checked_sub(b + c) {
                if a > b {
                    break;
                }

                if a * a + b * b == c * c {
                    res.insert([a, b, c]);
                }
            }
        }
    }

    res
}
