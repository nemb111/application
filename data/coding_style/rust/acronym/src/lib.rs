pub fn abbreviate(phrase: &str) -> String {
    phrase
        .split(|e: char| !e.is_alphabetic() && '\'' != e)
        .flat_map(|e| e.split_whitespace())
        .flat_map(|e| {
            e.char_indices().scan(false, |state, (pos, ch)| {
                if ch.is_lowercase() {
                    *state = true;
                }

                if 0usize == pos || (ch.is_uppercase() && *state) {
                    *state = false;
                    return Some(ch);
                }
                Some('-')
            })
        })
        .filter(|&f| '-' != f)
        .flat_map(|e| e.to_uppercase())
        .collect()
}
