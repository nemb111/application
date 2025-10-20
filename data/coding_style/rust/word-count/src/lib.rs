use std::collections::HashMap;

/// Count occurrences of words.
pub fn word_count(words: &str) -> HashMap<String, u32> {
    let split_words =
        words.split(|c: char| c.is_whitespace() || (c.is_ascii_punctuation() && c != '\''));
    split_words
        .map(|s| {
            s.trim_matches('\'')
                .chars()
                .filter(|c| c.is_alphanumeric() || *c == '\'')
                .flat_map(|c| c.to_lowercase())
                .collect::<String>()
        })
        .filter(|s| !s.is_empty())
        .fold(HashMap::new(), |mut map, i| {
            *map.entry(i).or_insert(0) += 1;
            map
        })
}
