use std::collections::BTreeMap;

pub fn transform(h: &BTreeMap<i32, Vec<char>>) -> BTreeMap<char, i32> {
    let mut res = BTreeMap::new();

    for (&key, value) in h.iter() {
        value.iter().for_each(|&e| {
            res.insert(e.to_ascii_lowercase(), key);
        });
    }

    res
}
