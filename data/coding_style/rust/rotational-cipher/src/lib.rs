pub fn rotate(input: &str, key: i8) -> String {
    let rot = |c: char, base: u8| -> char {
        (((c as u8 - base + (26 + key) as u8) % 26u8) + base) as char
    };

    input
        .chars()
        .map(|c| {
            if c.is_ascii_lowercase() {
                return rot(c, b'a');
            } else if c.is_ascii_uppercase() {
                return rot(c, b'A');
            }

            c
        })
        .collect()
}
