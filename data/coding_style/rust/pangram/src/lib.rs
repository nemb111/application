/// Determine whether a sentence is a pangram.
pub fn is_pangram(sentence: &str) -> bool {
    let mut pattern_to_check: u32 = 0u32;

    sentence
        .chars()
        .map(|c| c.to_ascii_lowercase())
        .filter(|c| match c {
            'a'..='z' => true,
            _ => false,
        })
        .for_each(|c| pattern_to_check |= 1u32 << (c as u8 - b'a'));

    0b11_1111_1111_1111_1111_1111_1111_u32 == pattern_to_check
}
