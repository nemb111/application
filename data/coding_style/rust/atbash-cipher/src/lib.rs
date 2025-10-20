fn transform(ch: char) -> char {
    let ch = ch.to_ascii_lowercase();
    if ch.is_ascii_alphabetic() {
        (b'z' - (ch as u8 - b'a')) as char
    } else {
        ch
    }
}

/// "Encipher" with the Atbash cipher.
pub fn encode(plain: &str) -> String {
    plain
        .chars()
        .filter(|c| c.is_alphanumeric())
        .enumerate()
        .flat_map(|(i, c)| {
            std::iter::once(' ')
                .filter(move |_| i % 5 == 0 && i != 0)
                .chain(std::iter::once(transform(c)))
        })
        .collect()
}

/// "Decipher" with the Atbash cipher.
pub fn decode(cipher: &str) -> String {
    cipher
        .split_ascii_whitespace()
        .flat_map(|c| c.chars())
        .map(transform)
        .collect()
}
