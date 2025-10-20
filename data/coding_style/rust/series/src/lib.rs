pub fn series(digits: &str, len: usize) -> Vec<String> {
    if 0 == len {
        return vec![String::from(""); digits.len() + 1];
    }

    digits
        .as_bytes()
        .windows(len)
        .map(|bs| String::from_utf8(bs.to_vec()).unwrap())
        .collect()
}
