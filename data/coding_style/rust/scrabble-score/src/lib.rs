/// Compute the Scrabble score for a word.
pub fn score(word: &str) -> u64 {
    word.chars()
        .flat_map(|c| c.to_lowercase())
        .map(|c| u64::from(letter_to_value_mapping(c)))
        .sum()
}

fn letter_to_value_mapping(ch: char) -> u8 {
    match ch {
        'a' | 'e' | 'i' | 'o' | 'u' | 'l' | 'n' | 'r' | 's' | 't' => 1,
        'd' | 'g' => 2,
        'b' | 'c' | 'm' | 'p' => 3,
        'f' | 'h' | 'v' | 'w' | 'y' => 4,
        'k' => 5,
        'j' | 'x' => 8,
        'q' | 'z' => 10,
        _ => 0,
    }
}
